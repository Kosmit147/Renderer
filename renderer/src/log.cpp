#include "renderer/log.hpp"

#include <string_view>

namespace renderer {

namespace {

LogCallback log_callback = [](LogLevel, std::string_view) {};

} // namespace

auto register_log_callback(LogCallback callback) -> void
{
    log_callback = callback;
}

auto log_message(LogLevel level, std::string_view message) -> void
{
    log_callback(level, message);
}

} // namespace renderer
