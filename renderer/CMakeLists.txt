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

add_library(Renderer::renderer ALIAS renderer)
