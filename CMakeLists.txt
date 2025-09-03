cmake_minimum_required(VERSION 3.30)

include("cmake/warnings.cmake")
include(CheckIPOSupported)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

project(
    Renderer
    VERSION 0.0.1
    DESCRIPTION "A simple graphics renderer."
    LANGUAGES C CXX
)

# The script expects build type / config to be one of standard CMake build types, that is either Debug, Release,
# RelWithDebInfo or MinSizeRel.

option(BUILD_SHARED_LIBS OFF)

if(NOT GENERATOR_IS_MULTI_CONFIG AND NOT DEFINED CMAKE_BUILD_TYPE)
    message(WARNING "No build type specified. Defaulting to 'Release'.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

check_ipo_supported(RESULT RND_IPO_SUPPORTED)

if(RND_IPO_SUPPORTED)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_MINSIZEREL ON)
endif()

if(MSVC)
    add_compile_options(
        "/permissive-"     # Standards conformance mode for MSVC compiler.
		"/Zc:preprocessor" # Standards conformance mode for MSVC's preprocessor.
    )

    # Replace /Ob1 with /Ob2 in RelWithDebInfo builds to enable full inlining.
    string(REPLACE "/Ob1" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Ob2")
    string(REPLACE "/Ob1" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /Ob2")
endif()

if(WIN32)
    # Get rid of min & max macro definitions from Windows.h.
    add_compile_definitions("NOMINMAX")
endif()

add_compile_definitions($<$<CONFIG:Debug>:RND_DEBUG>)

set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory("deps/glfw")

set(SPDLOG_ENABLE_PCH ON)
set(SPDLOG_USE_STD_FORMAT ON)
set(SPDLOG_NO_EXCEPTIONS OFF)
set(SPDLOG_DISABLE_DEFAULT_LOGGER OFF)
add_subdirectory("deps/spdlog")

add_executable(
    renderer
    "src/base.hpp"
    "src/log.hpp"
    "src/main.cpp"
    "src/pch.hpp"
)

add_executable(Renderer::renderer ALIAS renderer)

target_compile_features(renderer PRIVATE cxx_std_23)
get_compile_warnings(RND_COMPILE_WARNINGS)
target_compile_options(renderer PRIVATE ${RND_COMPILE_WARNINGS})
target_precompile_headers(renderer PRIVATE "src/pch.hpp")
set_property(TARGET renderer PROPERTY COMPILE_WARNING_AS_ERROR ON)

target_link_libraries(renderer PRIVATE glfw)
target_link_libraries(renderer PRIVATE spdlog::spdlog)
