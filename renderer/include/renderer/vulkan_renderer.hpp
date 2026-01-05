#pragma once

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <expected>
#include <span>
#include <string>
#include <vector>

namespace renderer {

class VulkanRenderer
{
public:
    [[nodiscard]] static auto create_glfw(const char* application_name) -> std::expected<VulkanRenderer, std::string>;

private:
    explicit VulkanRenderer(vk::raii::Context&& context, vk::raii::Instance&& instance,
                            vk::raii::DebugUtilsMessengerEXT&& debug_messenger);

private:
    vk::raii::Context _context{};
    vk::raii::Instance _instance{ nullptr };
    vk::raii::DebugUtilsMessengerEXT _debug_messenger{ nullptr };

private:
    [[nodiscard]] static auto get_vulkan_layers() -> std::vector<const char*>;
    [[nodiscard]] static auto get_vulkan_extensions() -> std::vector<const char*>;

    [[nodiscard]] static auto validate_layers(const vk::raii::Context& context, std::span<const char* const> layers)
        -> std::expected<void, std::string>;
    [[nodiscard]] static auto validate_extensions(const vk::raii::Context& context,
                                                  std::span<const char* const> extensions)
        -> std::expected<void, std::string>;
};

} // namespace renderer
