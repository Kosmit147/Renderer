#pragma once

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_raii.hpp>

#include <array>
#include <expected>
#include <span>
#include <string>
#include <tuple>
#include <vector>

#include "renderer/common.hpp"

namespace renderer {

class VulkanRenderer
{
public:
    constexpr static std::array required_device_extensions{ vk::KHRSwapchainExtensionName, vk::KHRSpirv14ExtensionName,
                                                            vk::KHRSynchronization2ExtensionName,
                                                            vk::KHRCreateRenderpass2ExtensionName };

public:
    [[nodiscard]] static auto create_glfw(const char* application_name, GLFWwindow* window)
        -> std::expected<VulkanRenderer, std::string>;

private:
    vk::raii::Context _context{};
    vk::raii::Instance _instance{ nullptr };
    vk::raii::SurfaceKHR _surface{ nullptr };
    vk::raii::PhysicalDevice _physical_device{ nullptr };
    vk::raii::Device _device{ nullptr };
    vk::raii::Queue _graphics_queue{ nullptr };
    vk::raii::DebugUtilsMessengerEXT _debug_messenger{ nullptr };

private:
    explicit VulkanRenderer(vk::raii::Context&& context, vk::raii::Instance&& instance, vk::raii::SurfaceKHR&& surface,
                            vk::raii::PhysicalDevice&& physical_device, vk::raii::Device&& device,
                            vk::raii::Queue&& graphics_queue, vk::raii::DebugUtilsMessengerEXT&& debug_messenger);

    [[nodiscard]] static auto get_vulkan_layers() -> std::vector<const char*>;
    [[nodiscard]] static auto get_vulkan_extensions() -> std::vector<const char*>;

    [[nodiscard]] static auto validate_layers(const vk::raii::Context& context, std::span<const char* const> layers)
        -> std::expected<void, std::string>;
    [[nodiscard]] static auto validate_extensions(const vk::raii::Context& context,
                                                  std::span<const char* const> extensions)
        -> std::expected<void, std::string>;

    [[nodiscard]] static auto create_debug_messenger(const vk::raii::Instance& instance)
        -> std::expected<vk::raii::DebugUtilsMessengerEXT, std::string>;

    [[nodiscard]] static auto create_surface(const vk::raii::Instance& instance, GLFWwindow* window)
        -> std::expected<vk::raii::SurfaceKHR, std::string>;

    [[nodiscard]] static auto pick_physical_device(const vk::raii::Instance& instance)
        -> std::expected<vk::raii::PhysicalDevice, std::string>;
    [[nodiscard]] static auto is_suitable(const vk::PhysicalDevice& physical_device)
        -> std::expected<bool, std::string>;

    [[nodiscard]] static auto create_device(const vk::raii::PhysicalDevice& physical_device)
        -> std::expected<std::tuple<vk::raii::Device, vk::raii::Queue>, std::string>;
    [[nodiscard]] static auto find_graphics_queue_family(const vk::raii::PhysicalDevice& physical_device) -> u32;
};

} // namespace renderer
