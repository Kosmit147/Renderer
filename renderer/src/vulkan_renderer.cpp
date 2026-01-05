#include "renderer/vulkan_renderer.hpp"

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_raii.hpp>

#include <algorithm>
#include <array>
#include <expected>
#include <format>
#include <optional>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

#include "renderer/assert.hpp"
#include "renderer/common.hpp"
#include "renderer/log.hpp"

namespace renderer {

namespace {

auto VKAPI_ATTR VKAPI_CALL vk_debug_utils_callback(vk::DebugUtilsMessageSeverityFlagBitsEXT severity,
                                                   vk::DebugUtilsMessageTypeFlagsEXT type,
                                                   const vk::DebugUtilsMessengerCallbackDataEXT* callback_data,
                                                   [[maybe_unused]] void* user_data) -> vk::Bool32
{
    static_assert(std::is_same_v<decltype(&vk_debug_utils_callback), vk::PFN_DebugUtilsMessengerCallbackEXT>);

    auto type_string = [type] {
        using enum vk::DebugUtilsMessageTypeFlagBitsEXT;

        if (type == eGeneral)
            return "General";
        if (type == eValidation)
            return "Validation";
        if (type == ePerformance)
            return "Performance";

        RENDERER_ASSERT(false);
        return "ERROR - UNEXPECTED DEBUG MESSAGE TYPE";
    }();

    switch (severity)
    {
        using enum vk::DebugUtilsMessageSeverityFlagBitsEXT;
    case eVerbose:
    case eInfo:
        RENDERER_INFO("VK - {}: {}", type_string, callback_data->pMessage);
        break;
    case eWarning:
        RENDERER_WARNING("VK - {}: {}", type_string, callback_data->pMessage);
        break;
    case eError:
        RENDERER_ERROR("VK - {}: {}", type_string, callback_data->pMessage);
        break;
    default:
        RENDERER_ASSERT(false);
    }

    return vk::False;
}

} // namespace

auto VulkanRenderer::create_glfw(const char* application_name) -> std::expected<VulkanRenderer, std::string>
{
    auto context = vk::raii::Context{};

    const auto layers = get_vulkan_layers();
    auto validate_layers_result = validate_layers(context, layers);
    if (!validate_layers_result)
        return std::unexpected{ validate_layers_result.error() };

    RENDERER_INFO("");

    const auto extensions = get_vulkan_extensions();
    auto validate_extensions_result = validate_extensions(context, extensions);
    if (!validate_extensions_result)
        return std::unexpected{ validate_extensions_result.error() };

    RENDERER_INFO("");

    const auto app_info = vk::ApplicationInfo{
        .pApplicationName = application_name,
        .applicationVersion = VK_MAKE_VERSION(0, 0, 1),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(0, 0, 1),
        .apiVersion = vk::ApiVersion14,
    };

    const auto instance_create_info = vk::InstanceCreateInfo{
        .pApplicationInfo = &app_info,
        .enabledLayerCount = static_cast<u32>(layers.size()),
        .ppEnabledLayerNames = layers.data(),
        .enabledExtensionCount = static_cast<u32>(extensions.size()),
        .ppEnabledExtensionNames = extensions.data(),
    };

    auto [create_instance_result, instance_handle] = vk::createInstance(instance_create_info);

    if (create_instance_result != vk::Result::eSuccess)
        return std::unexpected{ vk::to_string(create_instance_result) };

    auto instance = vk::raii::Instance{ context, instance_handle };

    auto has_debug_utils_extension = std::ranges::any_of(
        extensions, [](auto& extension) { return std::string_view{ extension } == vk::EXTDebugUtilsExtensionName; });

    auto debug_messenger = vk::raii::DebugUtilsMessengerEXT{ nullptr };

    if (has_debug_utils_extension)
    {
        auto create_debug_messenger_result = create_debug_messenger(instance);

        if (!create_debug_messenger_result)
            return std::unexpected{ create_debug_messenger_result.error() };

        debug_messenger = std::move(*create_debug_messenger_result);
    }

    auto physical_device = pick_physical_device(instance);

    if (!physical_device)
        return std::unexpected{ physical_device.error() };

    return VulkanRenderer{ std::move(context), std::move(instance), std::move(*physical_device),
                           std::move(debug_messenger) };
}

VulkanRenderer::VulkanRenderer(vk::raii::Context&& context, vk::raii::Instance&& instance,
                               vk::raii::PhysicalDevice&& physical_device,
                               vk::raii::DebugUtilsMessengerEXT&& debug_messenger)
    : _context{ std::move(context) }, _instance{ std::move(instance) }, _physical_device{ std::move(physical_device) },
      _debug_messenger{ std::move(debug_messenger) }
{}

auto VulkanRenderer::get_vulkan_layers() -> std::vector<const char*>
{
#if defined(RND_VK_VALIDATION_LAYERS)
    return { "VK_LAYER_KHRONOS_validation" };
#else
    return {};
#endif
}

auto VulkanRenderer::get_vulkan_extensions() -> std::vector<const char*>
{
    u32 glfw_extension_count = 0;
    auto glfw_extensions_ptr = glfwGetRequiredInstanceExtensions(&glfw_extension_count);
    auto glfw_extensions = std::span{ glfw_extensions_ptr, glfw_extension_count };

    auto extensions = std::vector<const char*>{ glfw_extensions.begin(), glfw_extensions.end() };

#if defined(RND_VK_DEBUG_UTILS)
    extensions.push_back(vk::EXTDebugUtilsExtensionName);
#endif

    return extensions;
}

auto VulkanRenderer::validate_layers(const vk::raii::Context& context, std::span<const char* const> layers)
    -> std::expected<void, std::string>
{
    auto [supported_layers_result, supported_layers] = context.enumerateInstanceLayerProperties();

    if (supported_layers_result != vk::Result::eSuccess)
        return std::unexpected{ vk::to_string(supported_layers_result) };

    for (auto& requested_layer : layers)
    {
        if (std::ranges::none_of(supported_layers, [&requested_layer](auto& supported_layer) {
                return std::string_view{ supported_layer.layerName } == requested_layer;
            }))
        {
            return std::unexpected{ std::format("Requested Vulkan layer {} not supported.", requested_layer) };
        }
    }

    RENDERER_INFO("Requested Vulkan layers:");
    for (auto& layer : layers)
        RENDERER_INFO("\t{}", layer);

    RENDERER_INFO("Supported Vulkan layers:");
    for (auto& layer : supported_layers)
        RENDERER_INFO("\t{}", std::string_view{ layer.layerName });

    return {};
}

auto VulkanRenderer::validate_extensions(const vk::raii::Context& context, std::span<const char* const> extensions)
    -> std::expected<void, std::string>
{
    auto [supported_extensions_result, supported_extensions] = context.enumerateInstanceExtensionProperties();

    if (supported_extensions_result != vk::Result::eSuccess)
        return std::unexpected{ vk::to_string(supported_extensions_result) };

    for (auto& requested_extension : extensions)
    {
        if (std::ranges::none_of(supported_extensions, [&requested_extension](auto& supported_extension) {
                return std::string_view{ supported_extension.extensionName } == requested_extension;
            }))
        {
            return std::unexpected{ std::format("Requested Vulkan extension {} not supported.", requested_extension) };
        }
    }

    RENDERER_INFO("Requested Vulkan extensions:");
    for (auto& extension : extensions)
        RENDERER_INFO("\t{}", extension);

    RENDERER_INFO("Supported Vulkan extensions:");
    for (auto& extension : supported_extensions)
        RENDERER_INFO("\t{}", std::string_view{ extension.extensionName });

    return {};
}

auto VulkanRenderer::create_debug_messenger(const vk::raii::Instance& instance)
    -> std::expected<vk::raii::DebugUtilsMessengerEXT, std::string>
{
    // clang-format off
    constexpr static auto severity_flags = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
                                         | vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo
                                         | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
                                         | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;

    constexpr static auto type_flags = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
                                     | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation
                                     | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
    // clang-format on

    auto debug_messenger_create_info =
        vk::DebugUtilsMessengerCreateInfoEXT{ .messageSeverity = severity_flags,
                                              .messageType = type_flags,
                                              .pfnUserCallback = vk_debug_utils_callback };

    auto [create_debug_messenger_result, debug_messenger] =
        instance.createDebugUtilsMessengerEXT(debug_messenger_create_info);

    if (create_debug_messenger_result != vk::Result::eSuccess)
        return std::unexpected{ vk::to_string(create_debug_messenger_result) };

    return std::move(debug_messenger);
}

auto VulkanRenderer::pick_physical_device(const vk::raii::Instance& instance)
    -> std::expected<vk::raii::PhysicalDevice, std::string>
{
    auto [devices_result, devices] = instance.enumeratePhysicalDevices();

    if (devices_result != vk::Result::eSuccess)
        return std::unexpected{ vk::to_string(devices_result) };

    if (devices.empty())
        return std::unexpected{ "No devices with Vulkan support found." };

    RENDERER_INFO("Vulkan devices found:");
    auto picked_device = std::optional<vk::raii::PhysicalDevice>{ std::nullopt };

    for (auto& device : devices)
    {
        auto device_properties = device.getProperties();

        if (is_suitable(device))
            picked_device = device;

        RENDERER_INFO("\t{}", std::string_view{ device_properties.deviceName });
    }

    if (!picked_device)
        return std::unexpected{ "No suitable device with Vulkan support found." };

    return *picked_device;
}

auto VulkanRenderer::is_suitable(const vk::PhysicalDevice& device) -> std::expected<bool, std::string>
{
    const auto device_properties = device.getProperties();

    if (device_properties.apiVersion < VK_API_VERSION_1_3
        || device_properties.deviceType != vk::PhysicalDeviceType::eDiscreteGpu)
    {
        return false;
    }

    auto [device_extensions_result, device_extensions] = device.enumerateDeviceExtensionProperties();

    if (device_extensions_result != vk::Result::eSuccess)
        return std::unexpected{ vk::to_string(device_extensions_result) };

    static constexpr auto required_device_extensions =
        std::array{ vk::KHRSwapchainExtensionName, vk::KHRSpirv14ExtensionName, vk::KHRSynchronization2ExtensionName,
                    vk::KHRCreateRenderpass2ExtensionName };

    for (auto& required_extension : required_device_extensions)
    {
        if (std::ranges::none_of(device_extensions, [&required_extension](auto& device_extension) {
                return std::string_view{ device_extension.extensionName } == required_extension;
            }))
        {
            return false;
        }
    }

    return true;
}

} // namespace renderer
