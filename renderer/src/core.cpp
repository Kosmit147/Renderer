#include "renderer/core.hpp"

namespace renderer {

namespace {

auto renderer_initialized = false;

} // namespace

auto init() -> bool
{
    renderer_initialized = true;
    return true;
}

auto terminate() -> void
{
    renderer_initialized = false;
}

} // namespace renderer
