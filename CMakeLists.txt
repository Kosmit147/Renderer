cmake_minimum_required(VERSION 4.1)

option(RND_ASSERTS "Enable asserts" FALSE)
option(RND_WARNING_AS_ERROR "Treat warnings as errors" FALSE)
option(RND_DEBUG_BREAKS "Enable debug breaks" FALSE)

set(RND_COMPILE_FLAGS "" CACHE STRING "Flags to pass to the compiler")

project(
    Renderer
    VERSION 0.0.1
    LANGUAGES C CXX
)

find_package(Vulkan REQUIRED)

add_subdirectory(libs/glfw)
add_subdirectory(libs/spdlog)

add_subdirectory(renderer)

add_subdirectory(presenter)
