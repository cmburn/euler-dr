/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_COLOR_H
#define EULER_UTIL_COLOR_H

#include <array>
#include <cstdint>

#ifdef EULER_NATIVE
#include <SDL3/SDL_pixels.h>
#endif

#include "euler/util/ext.h"
#include "euler/util/object.h"
#include "euler/util/state.h"

namespace euler::util {

class Color {
	BIND_MRUBY_DATA("Euler::Util::Color", Color, util.color);

public:
	// static const mrb_data_type TYPE;
	// static void init(mrb_state *mrb, State::Modules &mod);
	static Color read(mrb_state *mrb, mrb_value value);

#ifdef EULER_NATIVE
	explicit constexpr Color(const SDL_Color color)
	    : _red(color.r)
	    , _green(color.g)
	    , _blue(color.b)
	    , _alpha(color.a)
	{
	}
#endif

	explicit constexpr Color(const uint32_t color = 0)
	    : _red((color >> 24) & 0xFF)
	    , _green((color >> 16) & 0xFF)
	    , _blue((color >> 8) & 0xFF)
	    , _alpha(color & 0xFF)
	{
	}

	constexpr Color(const uint8_t red, const uint8_t green,
	    const uint8_t blue, const uint8_t alpha = 255)
	    : _red(red)
	    , _green(green)
	    , _blue(blue)
	    , _alpha(alpha)
	{
	}

	[[nodiscard]] uint8_t
	red() const
	{
		return _red;
	}

	[[nodiscard]] uint8_t
	green() const
	{
		return _green;
	}

	[[nodiscard]] uint8_t
	blue() const
	{
		return _blue;
	}

	[[nodiscard]] uint8_t
	alpha() const
	{
		return _alpha;
	}

#ifdef EULER_NATIVE
	[[nodiscard]] SDL_Color
	to_sdl() const
	{
		return SDL_Color { _red, _green, _blue, _alpha };
	}
#endif

	[[nodiscard]] uint32_t
	to_uint32() const
	{
		return (_red << 24) | (_green << 16) | (_blue << 8) | _alpha;
	}

	void
	set_red(const uint8_t red)
	{
		_red = red;
	}

	void
	set_green(const uint8_t green)
	{
		_green = green;
	}

	void
	set_blue(const uint8_t blue)
	{
		_blue = blue;
	}

	void
	set_alpha(const uint8_t alpha)
	{
		_alpha = alpha;
	}

	bool
	is_clear() const
	{
		return _alpha == 0;
	}

	[[nodiscard]] bool
	operator==(const Color &other) const
	{
		return to_uint32() == other.to_uint32();
	}

	[[nodiscard]] bool
	operator!=(const Color &other) const
	{
		return !(*this == other);
	}

	[[nodiscard]] std::array<float, 4>
	to_float_array() const
	{
		return {
			_red / 255.0f,
			_green / 255.0f,
			_blue / 255.0f,
			_alpha / 255.0f,
		};
	}

private:
	uint8_t _red;
	uint8_t _green;
	uint8_t _blue;
	uint8_t _alpha;
};

static constexpr auto COLOR_NONE = Color(0, 0, 0, 0);
static constexpr auto COLOR_WHITE = Color(255, 255, 255, 255);
static constexpr auto COLOR_BLACK = Color(0, 0, 0, 255);
static constexpr auto COLOR_RED = Color(255, 0, 0, 255);
static constexpr auto COLOR_GREEN = Color(0, 255, 0, 255);
static constexpr auto COLOR_BLUE = Color(0, 0, 255, 255);

} /* namespace euler::util */

#endif /* EULER_UTIL_COLOR_H */
