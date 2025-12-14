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

auto VulkanRenderer::create(const char* application_name, u32 glfw_required_extension_count,
                            const char** glfw_required_extensions) -> std::expected<VulkanRenderer, std::string>
{
    auto context = vk::raii::Context{};

    const auto required_extensions = std::span{ glfw_required_extensions, glfw_required_extension_count };
    auto [supported_extensions_result, supported_extensions] = context.enumerateInstanceExtensionProperties();

    if (supported_extensions_result != vk::Result::eSuccess)
        return std::unexpected{ vk::to_string(supported_extensions_result) };

    // Make sure every required extension is supported.
    for (auto& required : required_extensions)
    {
        if (std::ranges::none_of(supported_extensions, [&](auto& supported) {
                return std::string_view{ supported.extensionName } == required;
            }))
            return std::unexpected{ std::format("Required GLFW extension not supported: {}", required) };
    }

    RENDERER_INFO("Required extensions:");
    for (auto& extension : required_extensions)
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
        .enabledExtensionCount = glfw_required_extension_count,
        .ppEnabledExtensionNames = glfw_required_extensions,
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
