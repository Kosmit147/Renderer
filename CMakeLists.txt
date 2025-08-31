cmake_minimum_required(VERSION 3.30)

include("cmake/warnings.cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(
    Renderer
    VERSION 0.0.1
    DESCRIPTION "A simple graphics renderer."
    LANGUAGES C CXX)

if(NOT CMAKE_BUILD_TYPE)
    message(WARNING "No build type specified. Defaulting to 'Release'.")
    set(CMAKE_BUILD_TYPE "Release")
endif()

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug"
        AND NOT CMAKE_BUILD_TYPE STREQUAL "Release"
        AND NOT CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"
        AND NOT CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    message(FATAL_ERROR
        "Unsupported build type '${CMAKE_BUILD_TYPE}'. Allowed types are: Debug, Release, RelWithDebInfo, MinSizeRel.")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release"
        OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"
        OR CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
endif()

if(MSVC)
    # Replace /Ob1 with /Ob2 in RelWithDebInfo builds to enable full inlining.

    string(REPLACE "/Ob1" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /Ob2")

    string(REPLACE "/Ob1" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
    set(CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /Ob2")
endif()

get_compiler_warnings(COMPILER_WARNINGS)
set(COMPILER_OPTIONS ${COMPILER_WARNINGS})

if(MSVC)
    list(APPEND COMPILER_OPTIONS "/Zc:preprocessor") # Enable conformance mode for MSVC's preprocessor.
endif()

add_executable(
    renderer
    "src/main.cpp"
)

target_compile_features(renderer PRIVATE cxx_std_23)
target_compile_options(renderer PRIVATE ${COMPILER_OPTIONS})
set_property(TARGET renderer PROPERTY COMPILE_WARNING_AS_ERROR ON)
