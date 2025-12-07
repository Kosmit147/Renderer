#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <renderer/core.hpp>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <string>

#include "common.hpp"
#include "defer.hpp"
#include "log.hpp"

namespace {

auto glfw_error_callback(int error_code, const char* description) -> void
{
    RND_ERROR("GLFW Error {}: {}", error_code, description);
}

const auto application_name = std::string{ "Renderer" };

} // namespace

auto main() -> int
{
    // There's a bug in VS runtime that can cause the application to deadlock when it exits when using asynchronous
    // loggers. Calling spdlog::shutdown() prevents that.
    Defer shutdown_spdlog{ [] { spdlog::shutdown(); } };

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        RND_CRITICAL("Failed to initialize GLFW.");
        return EXIT_FAILURE;
    }

    Defer terminate_glfw{ [] { glfwTerminate(); } };

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    // Handling resizable windows takes special care, so disable resizing for now.
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    auto window = glfwCreateWindow(1920, 1080, application_name.c_str(), nullptr, nullptr);

    if (!window)
    {
        RND_CRITICAL("Failed to create a window.");
        return EXIT_FAILURE;
    }

    Defer destroy_window{ [&] { glfwDestroyWindow(window); } };

    // glfwMakeContextCurrent(window);
    // glfwSwapInterval(1);

    u32 glfw_required_extension_count = 0;
    auto glfw_required_extensions = glfwGetRequiredInstanceExtensions(&glfw_required_extension_count);

    auto renderer_init_result =
        renderer::Renderer::init(application_name.c_str(), glfw_required_extension_count, glfw_required_extensions);

    if (!renderer_init_result.has_value())
    {
        RND_CRITICAL("Failed to initialize the renderer: {}.", renderer_init_result.error());
        return EXIT_FAILURE;
    }

    Defer terminate_renderer{ [] { renderer::Renderer::terminate(); } };

    while (!glfwWindowShouldClose(window))
    {
        // glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}
