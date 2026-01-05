#pragma once

#include <cstdlib>
#include <iostream>
#include <source_location>

#if defined(RND_DEBUG_BREAKS)

    #if defined(_MSC_VER)

        #define RENDERER_DEBUG_BREAK __debugbreak()

    #elif defined(__GNUC__)

        #define RENDERER_DEBUG_BREAK __builtin_trap()

    #elif defined(__clang__)

        #define RENDERER_DEBUG_BREAK __builtin_debugtrap()

    #endif

#else

    #define RENDERER_DEBUG_BREAK ((void)(0))

#endif

#define RENDERER_ASSERT_IMPL(...)                                                                                      \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((__VA_ARGS__)) [[likely]]                                                                                  \
        {}                                                                                                             \
        else [[unlikely]]                                                                                              \
        {                                                                                                              \
            auto source_location = std::source_location::current();                                                    \
            std::cerr << source_location.file_name() << '(' << source_location.line() << ':'                           \
                      << source_location.column() << ") `" << source_location.function_name()                          \
                      << "`:\nAssertion failed: (" << #__VA_ARGS__ << ")\n";                                           \
            RENDERER_DEBUG_BREAK;                                                                                      \
            std::abort();                                                                                              \
        }                                                                                                              \
    } while (false)

#if defined(RND_ASSERTS)

    #define RENDERER_ASSERT(...) RENDERER_ASSERT_IMPL(__VA_ARGS__)

#else

    #define RENDERER_ASSERT(...) ((void)(0))

#endif

#define RENDERER_RUNTIME_ASSERT(...) RENDERER_ASSERT_IMPL(__VA_ARGS__)
