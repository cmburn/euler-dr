/* SPDX-License-Identifier: ISC */

#include "euler/gui/widget.h"

#include <VK2D/nuklear_defs.h>

#include "euler/gui/internal.h"
#include "euler/gui/widget.h"
#include "euler/gui/window.h"
#include "euler/util/ext.h"
#include "mruby/array.h"
#include "mruby/hash.h"

using euler::gui::Widget;

const Widget::Settings Widget::DEFAULT_SETTINGS = Settings();

static euler::gui::Row::Settings
read_row_args(mrb_state *mrb, mrb_value *block)
{
	auto state = euler::util::State::get(mrb);
	static const std::array KW_NAMES = {
		(EULER_SYM(height)),
		(EULER_SYM(columns)),
		(EULER_SYM(layout)),
	};
	mrb_value kw_values[KW_NAMES.size()];
	mrb_kwargs kwargs = {
		.num = 3,
		.required = 0,
		.table = KW_NAMES.data(),
		.values = kw_values,
		.rest = nullptr,
	};
	state->mrb()->get_args(":&", &kwargs, block);
	euler::gui::Row::Settings settings = {};
	if (!mrb_undef_p(kw_values[0]))
		settings.height = static_cast<float>(mrb_float(kw_values[0]));
	if (!mrb_undef_p(kw_values[1]))
		settings.columns = static_cast<int>(mrb_integer(kw_values[1]));
	if (!mrb_undef_p(kw_values[2])) {
		if (const auto layout_sym = mrb_symbol(kw_values[2]);
		    layout_sym == EULER_SYM(static)) {
			settings.layout = euler::gui::Row::Layout::Static;
		} else if (layout_sym == EULER_SYM(dynamic)) {
			settings.layout = euler::gui::Row::Layout::Dynamic;
		} else {
			state->mrb()->raise(state->mrb()->argument_error(),
			    "Invalid layout");
		}
	}
	return settings;
}

static mrb_value
widget_display(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<Widget>(self_value);
	self->display();
	return mrb_nil_value();
}

static mrb_value
widget_row(mrb_state *mrb, const mrb_value self_value)
{
	auto state = euler::util::State::get(mrb);
	auto self = state->unwrap<Widget>(self_value);
	mrb_value block = mrb_nil_value();
	auto settings = read_row_args(mrb, &block);
	if (!state->mrb()->block_given_p()) {
		auto row = euler::util::make_reference<euler::gui::Row>(
		    euler::util::Reference(self), settings);
		return state->wrap(row);
	}
	mrb_value retval = mrb_nil_value();
	self->row(settings, [&](auto &row) {
		auto value = state->wrap(row);
		retval = state->mrb()->yield(block, value);
	});
	return retval;
}

static nk_uint
to_nk(const Widget::Flags &flags)
{
	nk_uint out = 0;
	if (flags.border) out |= NK_WINDOW_BORDER;
	if (flags.moveable) out |= NK_WINDOW_MOVABLE;
	if (flags.scalable) out |= NK_WINDOW_SCALABLE;
	if (flags.closeable) out |= NK_WINDOW_CLOSABLE;
	if (flags.minimizable) out |= NK_WINDOW_MINIMIZABLE;
	if (flags.no_scrollbar) out |= NK_WINDOW_NO_SCROLLBAR;
	if (flags.title) out |= NK_WINDOW_TITLE;
	if (flags.scroll_auto_hide) out |= NK_WINDOW_SCROLL_AUTO_HIDE;
	if (flags.background) out |= NK_WINDOW_BACKGROUND;
	if (flags.scale_left) out |= NK_WINDOW_SCALE_LEFT;
	if (flags.no_input) out |= NK_WINDOW_NO_INPUT;
	return out;
}

static mrb_value
widget_title(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<Widget>(self_value);
	return state->mrb()->str_new_cstr(self->title().c_str());
}

static mrb_value
widget_set_title(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<Widget>(self_value);
	char *title = nullptr;
	state->mrb()->get_args("z", &title);
	self->set_title(title);
	return mrb_nil_value();
}

static mrb_value
widget_rect(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<Widget>(self_value);
	const auto &[x, y, w, h] = self->rect();
	// const mrb_value hash = mrb_hash_new(mrb);
	const mrb_value hash = state->mrb()->hash_new();
	state->mrb()->hash_set(hash, mrb_symbol_value(EULER_SYM(x)),
	    state->mrb()->float_value(x));
	state->mrb()->hash_set(hash, mrb_symbol_value(EULER_SYM(y)),
	    state->mrb()->float_value(y));
	state->mrb()->hash_set(hash, mrb_symbol_value(EULER_SYM(w)),
	    state->mrb()->float_value(w));
	state->mrb()->hash_set(hash, mrb_symbol_value(EULER_SYM(h)),
	    state->mrb()->float_value(h));
	return hash;
}

static mrb_value
widget_set_rect(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<Widget>(self_value);
	mrb_value hash;
	state->mrb()->get_args("H", &hash);
	self->set_rect(Widget::read_widget_rect(mrb, hash));
	return mrb_nil_value();
}

static mrb_value
widget_flags(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<Widget>(self_value);
	const mrb_value arr = state->mrb()->ary_new_capa(10);
	/* ReSharper disable once CppUseStructuredBinding */
	const auto flags = self->flags();
	if (flags.border) {
		state->mrb()->ary_push(arr,
		    mrb_symbol_value(EULER_SYM(border)));
	}
	if (flags.moveable) {
		state->mrb()->ary_push(arr,
		    mrb_symbol_value(EULER_SYM(moveable)));
	}
	if (flags.scalable) {
		state->mrb()->ary_push(arr,
		    mrb_symbol_value(EULER_SYM(scalable)));
	}
	if (flags.closeable) {
		state->mrb()->ary_push(arr,
		    mrb_symbol_value(EULER_SYM(closeable)));
	}
	if (flags.minimizable) {
		state->mrb()->ary_push(arr,
		    mrb_symbol_value(EULER_SYM(minimizable)));
	}
	if (flags.no_scrollbar) {
		state->mrb()->ary_push(arr,
		    mrb_symbol_value(EULER_SYM(no_scrollbar)));
	}
	if (flags.title) {
		state->mrb()->ary_push(arr, mrb_symbol_value(EULER_SYM(title)));
	}
	if (flags.scroll_auto_hide) {
		state->mrb()->ary_push(arr,
		    mrb_symbol_value(EULER_SYM(scroll_auto_hide)));
	}
	if (flags.background) {
		state->mrb()->ary_push(arr,
		    mrb_symbol_value(EULER_SYM(background)));
	}
	if (flags.scale_left) {
		state->mrb()->ary_push(arr,
		    mrb_symbol_value(EULER_SYM(scale_left)));
	}
	if (flags.no_input) {
		state->mrb()->ary_push(arr,
		    mrb_symbol_value(EULER_SYM(no_input)));
	}
	return arr;
}

static mrb_value
widget_set_flags(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<Widget>(self_value);
	mrb_value arr;
	state->mrb()->get_args("A", &arr);
	self->set_flags(Widget::read_widget_flags(mrb, arr));
	return mrb_nil_value();
}

RClass *
Widget::init(const euler::util::Reference<euler::util::State> &state,
    RClass *mod, RClass *)
{
	const auto widget = state->mrb()->define_class_under(mod, "Widget",
	    state->object_class());
	state->modules().gui.widget = widget;
	MRB_SET_INSTANCE_TT(widget, MRB_TT_CDATA);
	state->mrb()->define_method(widget, "row", widget_row,
	    MRB_ARGS_KEY(0, 3));
	state->mrb()->define_method(widget, "display", widget_display,
	    MRB_ARGS_NONE());
	state->mrb()->define_method(widget, "title", widget_title,
	    MRB_ARGS_NONE());
	state->mrb()->define_method(widget, "title=", widget_set_title,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(widget, "rect", widget_rect,
	    MRB_ARGS_NONE());
	state->mrb()->define_method(widget, "rect=", widget_set_rect,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(widget, "flags", widget_flags,
	    MRB_ARGS_NONE());
	state->mrb()->define_method(widget, "flags=", widget_set_flags,
	    MRB_ARGS_REQ(1));
}

Widget::Rectangle
Widget::bounds() const
{
	auto bounds = nk_window_get_bounds(context());
	return Rectangle {
		.x = bounds.x,
		.y = bounds.y,
		.w = bounds.w,
		.h = bounds.h,
	};
}

euler::util::Vec2
Widget::position() const
{
	auto vec = nk_widget_position(context());
	return util::Vec2 { vec.x, vec.y };
}

euler::util::Vec2
Widget::size() const
{
	auto vec = nk_widget_size(context());
	return util::Vec2 { vec.x, vec.y };
}

float
Widget::width() const
{
	return size()[0];
}

float
Widget::height() const
{
	return size()[1];
}

Widget::Rectangle
Widget::content_region() const
{
	auto rect = nk_window_get_content_region(context());
	return Rectangle {
		.x = rect.x,
		.y = rect.y,
		.w = rect.w,
		.h = rect.h,
	};
}

bool
Widget::are_any_active() const
{
	return nk_item_is_any_active(context());
}


euler::util::UVec2
Widget::scroll() const
{
	// auto vec = nk_window_get_scroll(context());
	// return util::UVec2 {vec.x, vec.y};
	uint32_t x, y;
	nk_window_get_scroll(context(), &x, &y);
	return util::UVec2 { x, y };
}


bool
Widget::has_focus() const
{
	return nk_window_has_focus(context());
}


bool
Widget::is_hovered() const
{
	return nk_window_is_hovered(context());
}

void
Widget::set_bounds(const Rectangle &rect)
{
	struct nk_rect nk_rect = {
		.x = rect.x,
		.y = rect.y,
		.w = rect.w,
		.h = rect.h,
	};
	nk_window_set_bounds(context(), title().c_str(), nk_rect);
}

void
Widget::set_position(const util::Vec2 &pos)
{
	struct nk_vec2 nk_vec = {
		.x = pos[0],
		.y = pos[1],
	};
	nk_window_set_position(context(), title().c_str(), nk_vec);
}

void
Widget::set_size(const util::Vec2 &size)
{
	struct nk_vec2 nk_vec = {
		.x = size[0],
		.y = size[1],
	};
	nk_window_set_size(context(), title().c_str(), nk_vec);
}

void
Widget::focus()
{
	nk_window_set_focus(context(), title().c_str());
}

void
Widget::set_scroll(const util::UVec2 &scroll)
{
	nk_window_set_scroll(context(), scroll[0], scroll[1]);
}

void
Widget::close()
{
	nk_window_close(context(), title().c_str());
}

void
Widget::show()
{
	nk_window_show(context(), title().c_str(), NK_SHOWN);
}

Widget::~Widget() { release(); }

void
Widget::release()
{
	gui()->release_widget(_id);
}

Widget::Rectangle
Widget::read_widget_rect(mrb_state *mrb, mrb_value hash)
{
	return Rectangle {
		.x = util::read_hash_float(mrb, hash, EULER_SYM(x)),
		.y = util::read_hash_float(mrb, hash, EULER_SYM(y)),
		.w = util::read_hash_float(mrb, hash, EULER_SYM(w)),
		.h = util::read_hash_float(mrb, hash, EULER_SYM(h)),
	};
}

Widget::Flags
Widget::read_widget_flags(mrb_state *mrb, const mrb_value arr)
{
	Flags flags = {};
	for (mrb_int i = 0, len = RARRAY_LEN(arr); i < len; ++i) {
		const auto item = mrb_ary_entry(arr, i);
		if (!mrb_symbol_p(item)) continue;
		auto sym = mrb_symbol(item);
		if (sym == EULER_SYM(border)) {
			flags.border = true;
		} else if (sym == EULER_SYM(moveable)) {
			flags.moveable = true;
		} else if (sym == EULER_SYM(scalable)) {
			flags.scalable = true;
		} else if (sym == EULER_SYM(closeable)) {
			flags.closeable = true;
		} else if (sym == EULER_SYM(minimizable)) {
			flags.minimizable = true;
		} else if (sym == EULER_SYM(no_scrollbar)) {
			flags.no_scrollbar = true;
		} else if (sym == EULER_SYM(title)) {
			flags.title = true;
		} else if (sym == EULER_SYM(scroll_auto_hide)) {
			flags.scroll_auto_hide = true;
		} else if (sym == EULER_SYM(background)) {
			flags.background = true;
		} else if (sym == EULER_SYM(scale_left)) {
			flags.scale_left = true;
		} else if (sym == EULER_SYM(no_input)) {
			flags.no_input = true;
		}
	}
	return flags;
}

Widget::Widget(const util::Reference<Window> &gui, const ID id,
    const char *title, const Settings &settings)
    : _gui(gui)
    , _id(id)
    , _title(title)
    , _rect(settings.rect)
    , _flags(settings.flags)
{
}

void
Widget::row(Row::Settings settings,
    const std::function<void(util::Reference<Row> &)> &fn)
{
	const auto row
	    = util::make_reference<Row>(util::Reference(this), settings);
	row->call(fn);
}

void
Widget::call(const std::function<void(util::Reference<Widget> &)> &fn)
{
	const struct nk_rect rect = {
		.x = _rect.x,
		.y = _rect.y,
		.w = _rect.w,
		.h = _rect.h,
	};

	if (nk_begin(gui()->context(), _title.c_str(), rect, to_nk(_flags))) {
		try {
			auto self = util::Reference(this);
			fn(self);
		} catch (...) {
			nk_end(gui()->context());
			throw;
		}
	}
	nk_end(gui()->context());
}

void
Widget::display()
{
	gui()->display(util::Reference(this));
}

euler::util::Reference<euler::gui::Window>
Widget::gui() const
{
	return _gui.strengthen();
}

bool
Widget::in_draw() const
{
	return gui()->state()->phase() == util::State::Phase::Draw;
}
