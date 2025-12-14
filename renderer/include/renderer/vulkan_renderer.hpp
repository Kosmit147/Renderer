#pragma once

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

#include <expected>
#include <span>
#include <string>

namespace renderer {

class VulkanRenderer
{
public:
    [[nodiscard]] static auto create(const char* application_name, std::span<const char* const> layers,
                                     std::span<const char* const> extensions)
        -> std::expected<VulkanRenderer, std::string>;

private:
    explicit VulkanRenderer(vk::raii::Context&& context, vk::raii::Instance&& instance);

private:
    vk::raii::Context _context{};
    vk::raii::Instance _instance{ nullptr };
};

} // namespace renderer
