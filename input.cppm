module;

#include <SDL3/SDL.h>
#include <stylizer/core/util/reaction.hpp>

export module stylizer.sdl3.input;

import std.compat;
import stdmath.slang;
import stylizer.input;
import stylizer.sdl3.event;
import stylizer.sdl3.common;

namespace stylizer::sdl3 {

	input::boolean convert_keycode(SDL_Keycode key) {
		return (input::boolean)key;
	}

	export struct input: public stylizer::input {
		size_t type = magic_number;

		std::unordered_map<boolean, std::unordered_map<device_t, reaction::Var<bool>>> booleans;

		void register_event_listener(context& ctx) override {
			if(!get_global_sdl_event_handler())
				get_global_sdl_event_handler() = setup_sdl_events(ctx);

			ctx.handle_event.connect([this](const context::event& e) {
				auto event = event2sdl(e);
				if(!event) return;

				process_SDL_event(event->sdl);
			});
		}

		bool process_SDL_event(const SDL_Event& event) {
			auto processed = false;
			reaction::batchExecute([&] -> void {
				switch(event.type) {
				break; case SDL_EVENT_KEY_DOWN: [[fallthrough]];
				case SDL_EVENT_KEY_UP: {
					update_if_different(get_integer(current_device), int64_t(event.key.which));

					auto lookup = convert_keycode(event.key.key);
					if(booleans.contains(lookup)) {
						auto& map = booleans[lookup];
						if(map.contains(input::all_devices)) {
							update_if_different(map[all_devices], event.key.down);
							processed = true;
						}

						device_t specific = event.key.which;
						if(map.contains(specific)) {
							update_if_different(map[specific], event.key.down);
							processed = true;
						}
					}
				}

				break; case SDL_EVENT_MOUSE_WHEEL: {
					update_if_different(get_integer(current_device), int64_t(event.wheel.which));

					if(vectors.contains(mouse_wheel)) {
						auto& map = vectors[mouse_wheel];
						if(map.contains(input::all_devices)) {
							update_if_any_different(map[all_devices], stdmath::float2{event.wheel.x, event.wheel.y});
							processed = true;
						}

						device_t specific = event.wheel.which;
						if(map.contains(specific)) {
							update_if_any_different(map[specific], stdmath::float2{event.wheel.x, event.wheel.y});
							processed = true;
						}
					}
				}

				break; case SDL_EVENT_MOUSE_MOTION: {
					update_if_different(get_integer(current_device), int64_t(event.motion.which));

					if(vectors.contains(mouse_position)) {
						auto& map = vectors[mouse_position];
						if(map.contains(input::all_devices)) {
							update_if_any_different(map[all_devices], stdmath::float2{event.motion.x, event.motion.y});
							processed = true;
						}

						device_t specific = event.motion.which;
						if(map.contains(specific)) {
							update_if_any_different(map[specific], stdmath::float2{event.motion.x, event.motion.y});
							processed = true;
						}
					}
				}
				break; default: break;
				}
			});

			return processed;
		}

		reaction::Var<bool>& get_boolean(boolean what, device_t devices = all_devices) override {
			auto& map = booleans[what];
			if(!map.contains(devices))
				return map[devices] = reaction::var(false);
			else return map[devices];
		}

		std::unordered_map<floating, std::unordered_map<device_t, reaction::Var<float>>> floats;

		reaction::Var<float>& get_floating(floating what, device_t devices = all_devices) override {
			auto& map = floats[what];
			if(!map.contains(devices))
				return map[devices] = reaction::var(0.0f);
			else return map[devices];
		}

		std::unordered_map<integer, std::unordered_map<device_t, reaction::Var<int64_t>>> ints;

		reaction::Var<int64_t>& get_integer(integer what, device_t devices = all_devices) override {
			auto& map = ints[what];
			if(!map.contains(devices))
				return map[devices] = reaction::var(int64_t{0});
			else return map[devices];
		}

		std::unordered_map<vector, std::unordered_map<device_t, reaction::Var<stdmath::float2>>> vectors;

		reaction::Var<stdmath::float2>& get_vector(vector what, device_t devices = all_devices) override {
			auto& map = vectors[what];
			if(!map.contains(devices))
				return map[devices] = reaction::var(stdmath::float2{});
			else return map[devices];
		}

	};

	static_assert(input_concept<input>);

}
