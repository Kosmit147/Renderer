cmake_minimum_required(VERSION 4.1)

option(RND_ASSERTS "Enable asserts" FALSE)
option(RND_WARNING_AS_ERROR "Treat warnings as errors" FALSE)
option(RND_DEBUG_BREAKS "Enable debug breaks" FALSE)
option(RND_VK_VALIDATION_LAYERS "Enable Vulkan validation layers" FALSE)
option(RND_VK_DEBUG_UTILS "Enable Vulkan debug utils extension" FALSE)

set(RND_COMPILE_FLAGS "" CACHE STRING "Flags to pass to the compiler")

project(
    Renderer
    VERSION 0.0.1
    LANGUAGES C CXX
)

find_package(Vulkan REQUIRED)

add_subdirectory(libs/glfw SYSTEM)
add_subdirectory(libs/spdlog SYSTEM)

add_subdirectory(renderer)

add_subdirectory(presenter)
