module;

#include <SDL3/SDL.h>

export module stylizer.sdl3.common;

import std.compat;
export import stylizer;
import stylizer.graphics.string2magic;

namespace stylizer::sdl3 {
    export constexpr size_t magic_number = graphics::string2magic("SDL3");

    export struct sdl_initializer {
		static sdl_initializer& singleton() {
			static sdl_initializer manager;
			return manager;
		}

		sdl_initializer() {
			if (!SDL_Init(SDL_INIT_VIDEO))
				stylizer::get_error_handler()(error::severity::Error, "Failed to initialize SDL", 0);
		}
		~sdl_initializer() {
			SDL_Quit();
		}
	};
}