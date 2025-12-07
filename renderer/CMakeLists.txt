cmake_minimum_required(VERSION 4.1)

add_library(renderer)

target_sources(
	renderer

	PRIVATE
	    src/core.cpp

	PUBLIC
	    FILE_SET HEADERS
		BASE_DIRS
		    include
		FILES
		    include/renderer/common.hpp
		    include/renderer/core.hpp
)

target_compile_features(renderer PUBLIC cxx_std_23)
target_compile_options(renderer PRIVATE "${RND_COMPILE_FLAGS}")

if(RND_WARNING_AS_ERROR)
    set_target_properties(renderer PROPERTIES COMPILE_WARNING_AS_ERROR TRUE)
endif()

target_compile_definitions(
	renderer

	PUBLIC
	    VULKAN_HPP_NO_CONSTRUCTORS=1
		VULKAN_HPP_NO_EXCEPTIONS=1
		VULKAN_HPP_RAII_NO_EXCEPTIONS=1
)

target_link_libraries(renderer PUBLIC Vulkan::Vulkan)

add_library(Renderer::renderer ALIAS renderer)
