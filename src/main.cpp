#include <spdlog/spdlog.h>

#include "base.hpp"

auto main() -> int
{
    spdlog::info("Hello World!");
    spdlog::shutdown();
}
