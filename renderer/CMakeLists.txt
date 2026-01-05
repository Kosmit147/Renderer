cmake_minimum_required(VERSION 4.1)

add_library(renderer)

target_sources(
	renderer

	PRIVATE
	    src/log.cpp
	    src/vulkan_renderer.cpp

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            include
        FILES
            include/renderer/common.hpp
            include/renderer/log.hpp
            include/renderer/vulkan_renderer.hpp
)

target_compile_features(renderer PUBLIC cxx_std_23)
target_compile_options(renderer PRIVATE "${RND_COMPILE_FLAGS}")

if(RND_WARNING_AS_ERROR)
    set_target_properties(renderer PROPERTIES COMPILE_WARNING_AS_ERROR TRUE)
endif()

if(RND_ASSERTS)
    target_compile_definitions(renderer PUBLIC RND_ASSERTS)
endif()

if(RND_DEBUG_BREAKS)
    target_compile_definitions(renderer PUBLIC RND_DEBUG_BREAKS)
endif()

if(RND_VK_VALIDATION_LAYERS)
    target_compile_definitions(renderer PUBLIC RND_VK_VALIDATION_LAYERS)
endif()

if(RND_VK_DEBUG_UTILS)
    target_compile_definitions(renderer PUBLIC RND_VK_DEBUG_UTILS)
endif()

target_compile_definitions(
	renderer

	PUBLIC
	    VULKAN_HPP_NO_CONSTRUCTORS=1
		VULKAN_HPP_NO_EXCEPTIONS=1
		VULKAN_HPP_RAII_NO_EXCEPTIONS=1
)

target_link_libraries(renderer PUBLIC Vulkan::Vulkan)

target_link_libraries(renderer PUBLIC glfw)

add_library(Renderer::renderer ALIAS renderer)
