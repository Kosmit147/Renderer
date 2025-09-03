#pragma once

// This file is always included through the precompiled header.

#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <source_location>

namespace rnd {

using u8 = std::uint8_t;
using i8 = std::int8_t;
using u16 = std::uint16_t;
using i16 = std::int16_t;
using u32 = std::uint32_t;
using i32 = std::int32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;
using isize = std::intmax_t;
using usize = std::uintmax_t;

// @refactor: Use types from <stdfloat> when they become available.
using f32 = float;
using f64 = double;

static_assert(sizeof(f32) == 4);
static_assert(sizeof(f64) == 8);

} // namespace rnd

#define RND_NOP ((void)0)

// @refactor: Use std::breakpoint from <debugging> once it's supported.

#if defined(RND_DEBUG)

    #if defined(_MSC_VER)

        #define RND_BREAK_POINT __debugbreak()

    #elif defined(__GNUC__)

        #define RND_BREAK_POINT __builtin_trap()

    #elif defined(__clang__)

        #define RND_BREAK_POINT __builtin_debugtrap()

    #else

        #define RND_BREAK_POINT
  // @refactor: Use #warning once it's supported.
        #pragma message("WARNING: Unsupported compiler. RND_BREAK_POINT macro has no effect.")

    #endif

#else

    #define RND_BREAK_POINT RND_NOP

#endif

#define RND_INTERNAL_ASSERT_IMPL(expr, ...)                                                                            \
    do                                                                                                                 \
    {                                                                                                                  \
        if ((expr)) [[likely]]                                                                                         \
        {}                                                                                                             \
        else [[unlikely]]                                                                                              \
        {                                                                                                              \
            constexpr auto source_location = std::source_location::current();                                          \
            std::cerr << source_location.file_name() << '(' << source_location.line() << ':'                           \
                      << source_location.column() << ") `" << source_location.function_name()                          \
                      << "`:\nAssertion failed: (" << #expr << ")" __VA_OPT__(", Message: " << __VA_ARGS__) << ".\n";  \
            RND_BREAK_POINT;                                                                                           \
            std::abort();                                                                                              \
        }                                                                                                              \
    } while (false)

#define RND_RUNTIME_ASSERT(...) RND_INTERNAL_ASSERT_IMPL(__VA_ARGS__)

#if defined(RND_DEBUG)

    #define RND_ASSERT(...) RND_INTERNAL_ASSERT_IMPL(__VA_ARGS__)

#else

    #define RND_ASSERT(...) RND_NOP

#endif
