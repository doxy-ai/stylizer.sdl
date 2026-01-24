module;

#include <SDL3/SDL_events.h>

export module stylizer.sdl3.event;

import std.compat;
import stylizer.sdl3.common;

namespace stylizer::sdl3 {

	export struct event: public stylizer::context::event {
		size_t type = magic_number;
		SDL_Event sdl;
	};

	export const event* event2sdl(const stylizer::context::event& e) {
		auto& sdl = (event&)e;
		return sdl.type == magic_number ? &sdl : nullptr;
	}

	export std::optional<connection>& get_global_sdl_event_handler() {
		static std::optional<connection> global_sdl_event_handler = []() -> std::optional<connection> {
			sdl_initializer::singleton();
			return {};
		}();

		return global_sdl_event_handler;
	}

	export connection_raw setup_sdl_events(context& ctx) {
		return ctx.process_events.connect([](context& ctx){
			for (SDL_Event event; SDL_PollEvent(&event); ) {
				struct event e;
				e.sdl = event;
				ctx.handle_event(e);
			}
		});
	}
}