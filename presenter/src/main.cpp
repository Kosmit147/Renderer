#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <renderer/log.hpp>
#include <renderer/vulkan_renderer.hpp>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <string>
#include <string_view>

#include "assert.hpp"
#include "common.hpp"
#include "defer.hpp"
#include "log.hpp"

namespace presenter {

namespace {

auto glfw_error_callback(int error_code, const char* description) -> void
{
    PRESENTER_ERROR("GLFW Error {}: {}", error_code, description);
}

auto renderer_log_callback(renderer::LogLevel level, std::string_view message) -> void
{
    switch (level)
    {
        using enum renderer::LogLevel;
    case Info:
        PRESENTER_INFO("[Renderer]: {}", message);
        break;
    case Warning:
        PRESENTER_WARN("[Renderer]: {}", message);
        break;
    case Error:
        PRESENTER_ERROR("[Renderer]: {}", message);
        break;
    default:
        PRESENTER_ASSERT(false);
    }
}

const auto application_name = std::string{ "Renderer" };

auto run() -> int
{
    // There's a bug in VS runtime that can cause the application to deadlock when it exits when using asynchronous
    // loggers. Calling spdlog::shutdown() prevents that.
    Defer shutdown_spdlog{ [] { spdlog::shutdown(); } };

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        PRESENTER_CRITICAL("Failed to initialize GLFW.");
        return EXIT_FAILURE;
    }

    Defer terminate_glfw{ [] { glfwTerminate(); } };

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Handling resizable windows takes special care, so disable resizing for now.
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    auto window = glfwCreateWindow(1920, 1080, application_name.c_str(), nullptr, nullptr);

    if (!window)
    {
        PRESENTER_CRITICAL("Failed to create a window.");
        return EXIT_FAILURE;
    }

    Defer destroy_window{ [&] { glfwDestroyWindow(window); } };

    // glfwMakeContextCurrent(window);
    // glfwSwapInterval(1);

    u32 glfw_required_extension_count = 0;
    auto glfw_required_extensions = glfwGetRequiredInstanceExtensions(&glfw_required_extension_count);

    renderer::register_log_callback(renderer_log_callback);

    auto renderer = renderer::VulkanRenderer::create(application_name.c_str(), glfw_required_extension_count,
                                                     glfw_required_extensions);

    if (!renderer.has_value())
    {
        PRESENTER_CRITICAL("Failed to initialize the renderer: {}.", renderer.error());
        return EXIT_FAILURE;
    }

    while (!glfwWindowShouldClose(window))
    {
        // glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}

} // namespace

} // namespace presenter

auto main() -> int
{
    return presenter::run();
}
