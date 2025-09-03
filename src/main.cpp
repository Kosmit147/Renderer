#include "log.hpp"

using namespace rnd;

auto main() -> int
{
    init_logger();
    RND_LOG_INFO("Hello World!");
    shut_down_logger();
}
