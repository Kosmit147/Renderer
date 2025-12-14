#include "renderer/vulkan_renderer.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <algorithm>
#include <expected>
#include <format>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "renderer/common.hpp"
#include "renderer/log.hpp"

namespace renderer {

auto VulkanRenderer::create(const char* application_name, std::span<const char* const> layers,
                            std::span<const char* const> extensions) -> std::expected<VulkanRenderer, std::string>
{
    auto context = vk::raii::Context{};

    auto [supported_layers_result, supported_layers] = context.enumerateInstanceLayerProperties();

    if (supported_layers_result != vk::Result::eSuccess)
        return std::unexpected{ vk::to_string(supported_layers_result) };

    // Make sure every layer is supported.
    for (auto& requested_layer : layers)
    {
        if (std::ranges::none_of(supported_layers, [&requested_layer](auto& supported_layer) {
                return std::string_view{ supported_layer.layerName } == requested_layer;
            }))
            return std::unexpected{ std::format("Requested layer {} not supported.", requested_layer) };
    }

    RENDERER_INFO("Requested layers:");
    for (auto& layer : layers)
        RENDERER_INFO("\t{}", layer);

    RENDERER_INFO("Supported layers:");
    for (auto& layer : supported_layers)
        RENDERER_INFO("\t{}", std::string_view{ layer.layerName });

    RENDERER_INFO("");

    auto [supported_extensions_result, supported_extensions] = context.enumerateInstanceExtensionProperties();

    if (supported_extensions_result != vk::Result::eSuccess)
        return std::unexpected{ vk::to_string(supported_extensions_result) };

    // Make sure every extension is supported.
    for (auto& requested_extension : extensions)
    {
        if (std::ranges::none_of(supported_extensions, [&requested_extension](auto& supported_extension) {
                return std::string_view{ supported_extension.extensionName } == requested_extension;
            }))
            return std::unexpected{ std::format("Requested extension {} not supported.", requested_extension) };
    }

    RENDERER_INFO("Requested extensions:");
    for (auto& extension : extensions)
        RENDERER_INFO("\t{}", extension);

    RENDERER_INFO("Supported extensions:");
    for (auto& extension : supported_extensions)
        RENDERER_INFO("\t{}", std::string_view{ extension.extensionName });

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

    return VulkanRenderer{ std::move(context), std::move(instance) };
}

VulkanRenderer::VulkanRenderer(vk::raii::Context&& context, vk::raii::Instance&& instance)
    : _context{ std::move(context) }, _instance{ std::move(instance) }
{}

} // namespace renderer
