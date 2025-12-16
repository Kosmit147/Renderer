cmake_minimum_required(VERSION 4.1)

add_executable(presenter)

target_sources(
	presenter

	PRIVATE
        src/main.cpp

    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            src
        FILES
            src/assert.hpp
            src/common.hpp
            src/defer.hpp
            src/log.hpp
)

target_compile_features(presenter PRIVATE cxx_std_23)
target_compile_options(presenter PRIVATE "${RND_COMPILE_FLAGS}")

if(RND_WARNING_AS_ERROR)
    set_target_properties(presenter PROPERTIES COMPILE_WARNING_AS_ERROR TRUE)
endif()

if(RND_ASSERTS)
    target_compile_definitions(presenter PRIVATE RND_ASSERTS)
endif()

if(RND_DEBUG_BREAKS)
    target_compile_definitions(presenter PRIVATE RND_DEBUG_BREAKS)
endif()

if(PRESENTER_VK_VALIDATION_LAYERS)
    target_compile_definitions(presenter PRIVATE PRESENTER_VK_VALIDATION_LAYERS)
endif()

if(PRESENTER_VK_DEBUG_UTILS)
    target_compile_definitions(presenter PRIVATE PRESENTER_VK_DEBUG_UTILS)
endif()

target_link_libraries(presenter PRIVATE glfw)
target_link_libraries(presenter PRIVATE spdlog::spdlog)
target_link_libraries(presenter PRIVATE Renderer::renderer)

add_executable(Renderer::presenter ALIAS presenter)
