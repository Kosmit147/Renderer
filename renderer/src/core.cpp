#include "renderer/core.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <algorithm>
#include <expected>
#include <format>
#include <memory>
#include <ranges>
#include <span>
#include <string>
#include <string_view>
#include <utility>

#include "renderer/common.hpp"

namespace renderer {

namespace {

struct RendererState
{
    vk::raii::Context context{};
    vk::raii::Instance instance{ nullptr };
};

auto renderer_state = std::unique_ptr<RendererState>{ nullptr };

} // namespace

auto Renderer::init(const char* application_name, u32 glfw_required_extension_count,
                    const char** glfw_required_extensions) -> std::expected<void, std::string>
{
    auto context = vk::raii::Context{};

    const auto required_extensions = std::span{ glfw_required_extensions, glfw_required_extension_count };
    auto [supported_extensions_result, supported_extensions] = context.enumerateInstanceExtensionProperties();

    if (supported_extensions_result != vk::Result::eSuccess)
        return std::unexpected{ vk::to_string(supported_extensions_result) };

    // Make sure every required extension is supported.
    for (auto& required : required_extensions)
    {
        auto found_required_in_supported = std::ranges::find_if(supported_extensions, [&](auto& supported) {
            return std::string_view{ supported.extensionName } == required;
        });

        if (found_required_in_supported == std::ranges::end(supported_extensions))
            return std::unexpected{ std::format("Required GLFW extension not supported: {}", required) };
    }

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

    renderer_state = std::make_unique<RendererState>(RendererState{
        .context = std::move(context),
        .instance = std::move(instance),
    });

    return {};
}

auto Renderer::terminate() -> void
{
    renderer_state.reset();
}

} // namespace renderer
