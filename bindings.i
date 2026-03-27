%import(module="stylizer.graphics") "../graphics/bindings.i"
%import(module="stylizer.core") "../core/bindings.i"
%import(module="stylizer.window") "../window/bindings.i"

%{
	#include <memory>

	import std.compat;
	import stdmath.c;
	import stylizer;
	import stylizer.window;
	import stylizer.sdl3.window;

	using namespace stylizer;
%}


%inline %{
	namespace stylizer {
		window* create_window(context& ctx, std::string_view title, stdmath_uint2 size, stylizer::window::create_flags flags = stylizer::window::create_flags::None) {
			return new stylizer::sdl3::window(stylizer::sdl3::window::create(ctx, title, (stdmath::uint2&)size, flags));
		}
	}
%}