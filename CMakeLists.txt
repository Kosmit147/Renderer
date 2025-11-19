cmake_minimum_required(VERSION 4.1)

project(
    Renderer
    VERSION 0.0.1
    LANGUAGES C CXX
)

add_subdirectory(libs/spdlog)

add_subdirectory(renderer)

add_subdirectory(presenter)
