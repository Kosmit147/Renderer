#pragma once

#include "renderer/common.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <expected>
#include <string>

namespace renderer {

class VulkanRenderer
{
public:
    [[nodiscard]] static auto create(const char* application_name, u32 glfw_required_extension_count,
                                     const char** glfw_required_extensions)
        -> std::expected<VulkanRenderer, std::string>;

private:
    explicit VulkanRenderer(vk::raii::Context&& context, vk::raii::Instance&& instance);

private:
    vk::raii::Context _context{};
    vk::raii::Instance _instance{ nullptr };
};

} // namespace renderer
