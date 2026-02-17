/* SPDX-License-Identifier: ISC */

#include "euler/util/color.h"

using euler::util::Color;

const mrb_data_type Color::TYPE
    = euler::util::pod_datatype<Color>("Euler::Util::Color");

static mrb_value
color_allocate(mrb_state *mrb, const mrb_value self)
{
	auto state = euler::util::State::get(mrb);
	auto obj = state->mrb()->data_object_alloc(mrb_class_ptr(self),
	    new Color(0, 0, 0, 255), &Color::TYPE);
	return mrb_obj_value(obj);
}

static mrb_value
color_initialize(mrb_state *mrb, const mrb_value self_value)
{
	auto self
	    = euler::util::unwrap_data<Color>(mrb, self_value, &Color::TYPE);
	*self = Color::read(mrb, self_value);
	return mrb_nil_value();
}

static mrb_value
color_to_int(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap_data<Color>(mrb, self_value, &Color::TYPE);
	return mrb_fixnum_value(self->to_uint32());
}

static mrb_value
color_to_string(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	static constexpr auto BUFFER_SIZE = sizeof("#RRGGBBAA") + 1;
	const auto self
	    = euler::util::unwrap_data<Color>(mrb, self_value, &Color::TYPE);
	char buffer[BUFFER_SIZE];
	snprintf(buffer, BUFFER_SIZE, "#%02X%02X%02X%02X", self->red(),
	    self->green(), self->blue(), self->alpha());
	return state->mrb()->str_new_cstr(buffer);
}

static mrb_value
color_red(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap_data<Color>(mrb, self_value, &Color::TYPE);
	return mrb_fixnum_value(self->red());
}

static mrb_value
color_green(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap_data<Color>(mrb, self_value, &Color::TYPE);
	return mrb_fixnum_value(self->green());
}

static mrb_value
color_blue(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap_data<Color>(mrb, self_value, &Color::TYPE);
	return mrb_fixnum_value(self->blue());
}

static mrb_value
color_alpha(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap_data<Color>(mrb, self_value, &Color::TYPE);
	return mrb_fixnum_value(self->alpha());
}

static mrb_value
color_set_red(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	auto self
	    = euler::util::unwrap_data<Color>(mrb, self_value, &Color::TYPE);
	mrb_int red;
	state->mrb()->get_args("i", &red);
	if (red < 0 || red > 255) {
		state->mrb()->raise(state->mrb()->argument_error(),
		    "Red component must be in the range 0:255");
	}
	self->set_red(static_cast<uint8_t>(red));
	return mrb_nil_value();
}

static mrb_value
color_set_green(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	auto self
	    = euler::util::unwrap_data<Color>(mrb, self_value, &Color::TYPE);
	mrb_int green;
	state->mrb()->get_args("i", &green);
	if (green < 0 || green > 255) {
		state->mrb()->raise(state->mrb()->argument_error(),
		    "Green component must be in the range 0:255");
	}
	self->set_green(static_cast<uint8_t>(green));
	return mrb_nil_value();
}

static mrb_value
color_set_blue(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	auto self
	    = euler::util::unwrap_data<Color>(mrb, self_value, &Color::TYPE);
	mrb_int blue;
	state->mrb()->get_args("i", &blue);
	if (blue < 0 || blue > 255) {
		state->mrb()->raise(state->mrb()->argument_error(),
		    "Blue component must be in the range 0:255");
	}
	self->set_blue(static_cast<uint8_t>(blue));
	return mrb_nil_value();
}

static mrb_value
color_set_alpha(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	auto self
	    = euler::util::unwrap_data<Color>(mrb, self_value, &Color::TYPE);
	mrb_int alpha;
	state->mrb()->get_args("i", &alpha);
	if (alpha < 0 || alpha > 255) {
		state->mrb()->raise(state->mrb()->argument_error(),
		    "Alpha component must be in the range 0:255");
	}
	self->set_alpha(static_cast<uint8_t>(alpha));
	return mrb_nil_value();
}

RClass *
Color::init(const Reference<State> &state, RClass *mod, RClass *)
{
	const auto color = state->mrb()->define_class_under(mod, "Color",
	    state->object_class());
	MRB_SET_INSTANCE_TT(color, MRB_TT_DATA);
	state->mrb()->define_class_method(color, "allocate", color_allocate,
	    MRB_ARGS_NONE());
	state->mrb()->define_method(color, "initialize", color_initialize,
	    MRB_ARGS_NONE());
	state->mrb()->define_method(color, "to_i", color_to_int,
	    MRB_ARGS_NONE());
	state->mrb()->define_method(color, "to_s", color_to_string,
	    MRB_ARGS_NONE());
	state->mrb()->define_method(color, "red", color_red, MRB_ARGS_NONE());
	state->mrb()->define_method(color, "green", color_green,
	    MRB_ARGS_NONE());
	state->mrb()->define_method(color, "blue", color_blue, MRB_ARGS_NONE());
	state->mrb()->define_method(color, "alpha", color_alpha,
	    MRB_ARGS_NONE());
	state->mrb()->define_method(color, "red=", color_set_red,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(color, "green=", color_set_green,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(color, "blue=", color_set_blue,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(color, "alpha=", color_set_alpha,
	    MRB_ARGS_REQ(1));
	return color;
}
Color
Color::read(mrb_state *mrb, mrb_value value)
{
	auto state = State::get(mrb);
	if (mrb_nil_p(value)) return COLOR_BLACK;
	if (mrb_integer_p(value)) {
		const auto color = static_cast<uint32_t>(mrb_integer(value));
		return Color(color);
	}
	if (mrb_symbol_p(value)) {
		const auto sym = mrb_symbol(value);
		if (sym == EULER_SYM(white)) return COLOR_WHITE;
		if (sym == EULER_SYM(black)) return COLOR_BLACK;
		if (sym == EULER_SYM(red)) return COLOR_RED;
		if (sym == EULER_SYM(green)) return COLOR_GREEN;
		if (sym == EULER_SYM(blue)) return COLOR_BLUE;
		state->mrb()->raisef(state->mrb()->argument_error(),
		    "Unknown color symbol: %S", value);
	}
	const auto hash = state->mrb()->ensure_hash_type(value);
#define READ_COLOR(OUT, NAME1, NAME2)                                          \
	do {                                                                   \
		if (state->mrb()->hash_key_p(value,                            \
			mrb_symbol_value(EULER_SYM(NAME1)))) {                 \
			OUT = mrb_integer(state->mrb()->hash_get(hash,         \
			    mrb_symbol_value(EULER_SYM(NAME1))));              \
		} else if (state->mrb()->hash_key_p(value,                     \
			       mrb_symbol_value(EULER_SYM(NAME2)))) {          \
			OUT = mrb_integer(state->mrb()->hash_get(hash,         \
			    mrb_symbol_value(EULER_SYM(NAME2))));              \
		}                                                              \
		if (OUT < 0 || OUT > 255) {                                    \
			state->mrb()->raisef(state->mrb()->argument_error(),   \
			    "Color component " #NAME1                          \
			    " (value %ld) must be in the range 0:255",         \
			    OUT);                                              \
		}                                                              \
	} while (0)

	mrb_int r = 0;
	READ_COLOR(r, red, r);
	mrb_int g = 0;
	READ_COLOR(g, green, g);
	mrb_int b = 0;
	READ_COLOR(b, blue, b);
	mrb_int a = 255;
	READ_COLOR(a, alpha, a);
#undef READ_COLOR
	return {
		static_cast<uint8_t>(r),
		static_cast<uint8_t>(g),
		static_cast<uint8_t>(b),
		static_cast<uint8_t>(a),
	};
}
