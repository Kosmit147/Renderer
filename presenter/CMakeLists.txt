cmake_minimum_required(VERSION 4.1)

add_executable(presenter)

target_sources(
	presenter

	PRIVATE
	    src/defer.hpp
	    src/log.hpp
	    src/main.cpp
)

target_compile_features(presenter PRIVATE cxx_std_23)
target_compile_options(presenter PRIVATE "${RND_COMPILE_FLAGS}")

if(RND_WARNING_AS_ERROR)
    set_target_properties(presenter PROPERTIES COMPILE_WARNING_AS_ERROR TRUE)
endif()

target_link_libraries(presenter PRIVATE glfw)
target_link_libraries(presenter PRIVATE spdlog::spdlog)
target_link_libraries(presenter PRIVATE Renderer::renderer)

add_executable(Renderer::presenter ALIAS presenter)
