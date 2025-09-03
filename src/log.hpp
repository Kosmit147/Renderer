#pragma once

#include <spdlog/spdlog.h>

#define RND_LOG_TRACE(...) ::spdlog::trace(__VA_ARGS__)
#define RND_LOG_DEBUG(...) ::spdlog::debug(__VA_ARGS__)
#define RND_LOG_INFO(...) ::spdlog::info(__VA_ARGS__)
#define RND_LOG_WARN(...) ::spdlog::warn(__VA_ARGS__)
#define RND_LOG_ERROR(...) ::spdlog::error(__VA_ARGS__)
#define RND_LOG_CRITICAL(...) ::spdlog::critical(__VA_ARGS__)

namespace rnd {

inline auto init_logger() -> void
{
#if defined(RND_DEBUG)
    spdlog::set_level(spdlog::level::trace);
#else
    spdlog::set_level(spdlog::level::info);
#endif
}

inline auto shut_down_logger() -> void
{
    spdlog::shutdown();
}

} // namespace rnd
