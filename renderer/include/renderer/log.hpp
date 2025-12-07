#pragma once

#include <format>
#include <string_view>

#include "common.hpp"

namespace renderer {

enum class LogLevel : u8
{
    Info,
    Warning,
    Error,
};

using LogCallback = void (*)(LogLevel, std::string_view);

auto register_log_callback(LogCallback callback) -> void;
auto log_message(LogLevel level, std::string_view message) -> void;

} // namespace renderer

#define RENDERER_INFO(...) ::renderer::log_message(::renderer::LogLevel::Info, std::format(__VA_ARGS__))
#define RENDERER_WARNING(...) ::renderer::log_message(::renderer::LogLevel::Warning, std::format(__VA_ARGS__))
#define RENDERER_ERROR(...) ::renderer::log_message(::renderer::LogLevel::Error, std::format(__VA_ARGS__))
