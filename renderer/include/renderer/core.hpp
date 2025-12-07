#pragma once

#include "renderer/common.hpp"

#include <expected>
#include <string>

namespace renderer {

class Renderer
{
public:
    [[nodiscard]] static auto init(const char* application_name, u32 glfw_required_extension_count,
                                   const char** glfw_required_extensions) -> std::expected<void, std::string>;
    static auto terminate() -> void;
};

} // namespace renderer
