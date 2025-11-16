cmake_minimum_required(VERSION 4.1)

add_executable(renderer_app)

target_sources(
	renderer_app

	PRIVATE
	    src/main.cpp
)

target_compile_features(renderer_app PRIVATE cxx_std_23)

target_link_libraries(renderer_app PRIVATE Renderer::renderer)

add_executable(Renderer::renderer_app ALIAS renderer_app)
