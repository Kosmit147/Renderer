#pragma once

#include <spdlog/spdlog.h>

#define RND_TRACE(...) ::spdlog::trace(__VA_ARGS__)
#define RND_DEBUG(...) ::spdlog::debug(__VA_ARGS__)
#define RND_INFO(...) ::spdlog::info(__VA_ARGS__)
#define RND_WARN(...) ::spdlog::warn(__VA_ARGS__)
#define RND_ERROR(...) ::spdlog::error(__VA_ARGS__)
#define RND_CRITICAL(...) ::spdlog::critical(__VA_ARGS__)
