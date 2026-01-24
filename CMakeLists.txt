set(STYLIZER_GRAPHICS_ENABLE_SDL3 ON CACHE BOOL "Should the Stylizer Graphics API build support for SDL3?" FORCE)

add_library(stylizer_sdl3)
target_sources(stylizer_sdl3 PUBLIC
	FILE_SET stylizer_sdl3 TYPE CXX_MODULES
	# BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		common.cppm
		event.cppm
		input.cppm
		window.cppm
)
target_link_libraries(stylizer_sdl3 PUBLIC stylizer stylizer::input stylizer::window)
target_compile_definitions(stylizer_sdl3 PUBLIC -DSTYLIZER_SDL3_AVAILABLE)

add_library(stylizer::sdl3 ALIAS stylizer_sdl3)