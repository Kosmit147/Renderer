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
		    include/renderer/core.hpp
)

target_compile_features(renderer PUBLIC cxx_std_23)
target_compile_options(renderer PRIVATE "${RND_WARNINGS}")

if(RND_WARNING_AS_ERROR)
    set_target_properties(renderer PROPERTIES COMPILE_WARNING_AS_ERROR TRUE)
endif()

add_library(Renderer::renderer ALIAS renderer)
