add_library(stylizer_sdl3)
target_sources(stylizer_sdl3 PUBLIC
	FILE_SET stylizer_sdl3 TYPE CXX_MODULES
	# BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}
	FILES
		common.cppm
		event.cppm
)
target_link_libraries(stylizer_sdl3 PUBLIC stylizer)

add_library(stylizer::sdl3 ALIAS stylizer_sdl3)