cmake_minimum_required(VERSION 4.1)

add_executable(presenter)

target_sources(
	presenter

	PRIVATE
	    src/main.cpp
)

target_compile_features(presenter PRIVATE cxx_std_23)

target_link_libraries(presenter PRIVATE Renderer::renderer)

add_executable(Renderer::presenter ALIAS presenter)
