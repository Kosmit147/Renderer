#include <spdlog/spdlog.h>

#include <renderer/core.hpp>

auto main() -> int
{
    spdlog::info("{}", renderer::test());
}
