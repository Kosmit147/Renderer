#include <GLFW/glfw3.h>
#include <renderer/core.hpp>
#include <spdlog/spdlog.h>

#include <cstdlib>

#include "defer.hpp"
#include "log.hpp"

namespace {

auto glfw_error_callback(int error_code, const char* description) -> void
{
    RND_ERROR("GLFW Error {}: {}", error_code, description);
}

} // namespace

auto main() -> int
{
    // There's a bug in VS runtime that can cause the application to deadlock when it exits when using asynchronous
    // loggers. Calling spdlog::shutdown() prevents that.
    Defer shutdown_spdlog{ [] { spdlog::shutdown(); } };

    RND_INFO("{}", renderer::test());

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        RND_CRITICAL("Failed to initialize GLFW.");
        return EXIT_FAILURE;
    }

    Defer terminate_glfw{ [] { glfwTerminate(); } };

    auto window = glfwCreateWindow(1920, 1080, "Renderer", nullptr, nullptr);

    if (!window)
    {
        RND_CRITICAL("Failed to create a window.");
        return EXIT_FAILURE;
    }

    Defer destroy_window{ [&] { glfwDestroyWindow(window); } };

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return EXIT_SUCCESS;
}
