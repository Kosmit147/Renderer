#include <GLFW/glfw3.h>

#include <cstdlib>

#include "log.hpp"

namespace rnd {

namespace {

auto glfw_error_callback(int error, const char* description) -> void
{
    RND_LOG_ERROR("[GLFW]: Error code: {}, Description: '{}'.", error, description);
}

auto glfw_key_callback([[maybe_unused]] GLFWwindow* window, int key, [[maybe_unused]] int scancode, int action,
                       [[maybe_unused]] int mods) -> void
{
    auto action_description = [action] {
        if (action == GLFW_PRESS)
        {
            return "pressed";
        }
        else if (action == GLFW_RELEASE)
        {
            return "released";
        }
        else // if action == GLFW_REPEAT
        {
            return "repeated";
        }
    }();

    RND_LOG_TRACE("Key {}: {}", action_description, key);

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

auto glfw_mouse_button_callback([[maybe_unused]] GLFWwindow* window, int button, int action, [[maybe_unused]] int mods)
    -> void
{
    auto action_description = [action] {
        if (action == GLFW_PRESS)
        {
            return "pressed";
        }
        else // if action == GLFW_RELEASE
        {
            return "released";
        }
    }();

    RND_LOG_TRACE("Mouse button {}: {}", action_description, button);
}

} // namespace

} // namespace rnd

using namespace rnd;

auto main() -> int
{
    init_logger();

    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
    {
        RND_LOG_CRITICAL("Failed to initialize GLFW.");
        return EXIT_FAILURE;
    }

    auto* window = glfwCreateWindow(1920, 1080, "Renderer", nullptr, nullptr);

    if (!window)
    {
        RND_LOG_CRITICAL("Failed to create a window.");
        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetMouseButtonCallback(window, glfw_mouse_button_callback);

    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();

    shut_down_logger();
}
