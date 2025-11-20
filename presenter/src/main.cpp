#include <GLFW/glfw3.h>
#include <renderer/core.hpp>
#include <spdlog/spdlog.h>

#include <cstdlib>

#include "log.hpp"

namespace {

auto glfw_error_callback(int error_code, const char* description) -> void
{
    RND_ERROR("GLFW Error {}: {}", error_code, description);
}

} // namespace

auto main() -> int
{
    RND_INFO("{}", renderer::test());

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        RND_CRITICAL("Failed to initialize GLFW.");
        return EXIT_FAILURE;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Renderer", nullptr, nullptr);

    if (!window)
    {
        RND_CRITICAL("Failed to create a window.");
        glfwTerminate();
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    return EXIT_SUCCESS;
}
