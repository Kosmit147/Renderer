cmake_minimum_required(VERSION 4.1)

option(RND_WARNING_AS_ERROR "Treat warnings as errors" FALSE)

set(RND_COMPILE_FLAGS "" CACHE STRING "Flags to pass to the compiler")

project(
    Renderer
    VERSION 0.0.1
    LANGUAGES C CXX
)

add_subdirectory(libs/glfw)
add_subdirectory(libs/spdlog)

add_subdirectory(renderer)

add_subdirectory(presenter)
