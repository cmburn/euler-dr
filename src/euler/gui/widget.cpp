/* SPDX-License-Identifier: ISC */

#include "euler/gui/widget.h"

#include "euler/gui/internal.h"
#include "euler/gui/widget.h"
#include "euler/gui/window.h"
#include "euler/util/ext.h"
#include "mruby/array.h"
#include "mruby/hash.h"

using euler::gui::Widget;

const mrb_data_type Widget::TYPE = MAKE_REFERENCE_TYPE(euler::gui::Widget);
const Widget::Settings Widget::DEFAULT_SETTINGS = Settings();

static euler::gui::Row::Settings
read_row_args(mrb_state *mrb, mrb_value *block)
{
	static constexpr std::array KW_NAMES = {
		static_cast<mrb_sym>(MRB_SYM(height)),
		static_cast<mrb_sym>(MRB_SYM(columns)),
		static_cast<mrb_sym>(MRB_SYM(layout)),
	};
	mrb_value kw_values[KW_NAMES.size()];
	mrb_kwargs kwargs = {
		.num = 3,
		.required = 0,
		.table = KW_NAMES.data(),
		.values = kw_values,
		.rest = nullptr,
	};
	mrb_get_args(mrb, ":&", &kwargs, block);
	euler::gui::Row::Settings settings = {};
	if (!mrb_undef_p(kw_values[0]))
		settings.height = static_cast<float>(mrb_float(kw_values[0]));
	if (!mrb_undef_p(kw_values[1]))
		settings.columns = static_cast<int>(mrb_integer(kw_values[1]));
	if (!mrb_undef_p(kw_values[2])) {
		switch (mrb_symbol(kw_values[2])) {
		case MRB_SYM(static):
			settings.layout = euler::gui::Row::Layout::Static;
			break;
		case MRB_SYM(dynamic):
			settings.layout = euler::gui::Row::Layout::Dynamic;
			break;
		default:
			mrb_raise(mrb, mrb->eStandardError_class,
			    "Invalid layout");
		}
	}
	return settings;
}

static mrb_value
widget_display(mrb_state *mrb, const mrb_value self_value)
{
	auto self = euler::util::unwrap<Widget>(mrb, self_value, &Widget::TYPE);
	self->display();
	return mrb_nil_value();
}

static mrb_value
widget_row(mrb_state *mrb, const mrb_value self_value)
{
	auto self = euler::util::unwrap<Widget>(mrb, self_value, &Widget::TYPE);
	mrb_value block = mrb_nil_value();
	auto settings = read_row_args(mrb, &block);
	const auto klass = euler::util::State::get(mrb)->module().gui.row;
	if (!mrb_block_given_p(mrb)) {
		const auto row = euler::util::make_reference<euler::gui::Row>(
		    euler::util::Reference(self), settings);
		return euler::util::wrap(mrb, row, klass,
		    &euler::gui::Row::TYPE);
	}
	mrb_value retval = mrb_nil_value();
	self->row(settings, [&](auto &row) {
		const auto value = euler::util::wrap(mrb, row, klass,
		    &euler::gui::Row::TYPE);
		retval = mrb_yield(mrb, block, value);
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
	const auto self
	    = euler::util::unwrap<Widget>(mrb, self_value, &Widget::TYPE);
	return mrb_str_new_cstr(mrb, self->title().c_str());
}

static mrb_value
widget_set_title(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap<Widget>(mrb, self_value, &Widget::TYPE);
	char *title = nullptr;
	mrb_get_args(mrb, "z", &title);
	self->set_title(title);
	return mrb_nil_value();
}

static mrb_value
widget_rect(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap<Widget>(mrb, self_value, &Widget::TYPE);
	const auto &[x, y, w, h] = self->rect();
	const mrb_value hash = mrb_hash_new(mrb);
	mrb_hash_set(mrb, hash, mrb_symbol_value(MRB_SYM(x)),
	    mrb_float_value(mrb, x));
	mrb_hash_set(mrb, hash, mrb_symbol_value(MRB_SYM(y)),
	    mrb_float_value(mrb, y));
	mrb_hash_set(mrb, hash, mrb_symbol_value(MRB_SYM(w)),
	    mrb_float_value(mrb, w));
	mrb_hash_set(mrb, hash, mrb_symbol_value(MRB_SYM(h)),
	    mrb_float_value(mrb, h));
	return hash;
}

static mrb_value
widget_set_rect(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap<Widget>(mrb, self_value, &Widget::TYPE);
	mrb_value hash;
	mrb_get_args(mrb, "H", &hash);
	self->set_rect(Widget::read_widget_rect(mrb, hash));
	return mrb_nil_value();
}

static mrb_value
widget_flags(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap<Widget>(mrb, self_value, &Widget::TYPE);
	/* ReSharper disable once CppUseStructuredBinding */
	const auto flags = self->flags();
	const mrb_value arr = mrb_ary_new_capa(mrb, 10);
	if (flags.border)
		mrb_ary_push(mrb, arr, mrb_symbol_value(MRB_SYM(border)));
	if (flags.moveable)
		mrb_ary_push(mrb, arr, mrb_symbol_value(MRB_SYM(moveable)));
	if (flags.scalable)
		mrb_ary_push(mrb, arr, mrb_symbol_value(MRB_SYM(scalable)));
	if (flags.closeable)
		mrb_ary_push(mrb, arr, mrb_symbol_value(MRB_SYM(closeable)));
	if (flags.minimizable)
		mrb_ary_push(mrb, arr, mrb_symbol_value(MRB_SYM(minimizable)));
	if (flags.no_scrollbar)
		mrb_ary_push(mrb, arr, mrb_symbol_value(MRB_SYM(no_scrollbar)));
	if (flags.title)
		mrb_ary_push(mrb, arr, mrb_symbol_value(MRB_SYM(title)));
	if (flags.scroll_auto_hide) {
		mrb_ary_push(mrb, arr,
		    mrb_symbol_value(MRB_SYM(scroll_auto_hide)));
	}
	if (flags.background)
		mrb_ary_push(mrb, arr, mrb_symbol_value(MRB_SYM(background)));
	if (flags.scale_left)
		mrb_ary_push(mrb, arr, mrb_symbol_value(MRB_SYM(scale_left)));
	if (flags.no_input)
		mrb_ary_push(mrb, arr, mrb_symbol_value(MRB_SYM(no_input)));
	return arr;
}

static mrb_value
widget_set_flags(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap<Widget>(mrb, self_value, &Widget::TYPE);
	mrb_value arr;
	mrb_get_args(mrb, "A", &arr);
	self->set_flags(Widget::read_widget_flags(arr));
	return mrb_nil_value();
}

void
Widget::init(mrb_state *mrb, util::State::Modules &mod)
{
	mod.gui.widget = mrb_define_class_under(mrb, mod.gui.module, "Widget",
	    mrb->object_class);
	const auto widget = mod.gui.widget;
	MRB_SET_INSTANCE_TT(widget, MRB_TT_CDATA);
	mrb_define_method(mrb, widget, "row", widget_row, MRB_ARGS_KEY(0, 3));
	mrb_define_method(mrb, widget, "display", widget_display,
	    MRB_ARGS_NONE());
	mrb_define_method(mrb, widget, "title", widget_title, MRB_ARGS_NONE());
	mrb_define_method(mrb, widget, "title=", widget_set_title,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, widget, "rect", widget_rect, MRB_ARGS_NONE());
	mrb_define_method(mrb, widget, "rect=", widget_set_rect,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, widget, "flags", widget_flags, MRB_ARGS_NONE());
	mrb_define_method(mrb, widget, "flags=", widget_set_flags,
	    MRB_ARGS_REQ(1));
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
		.x = util::read_hash_float(mrb, hash, MRB_SYM(x)),
		.y = util::read_hash_float(mrb, hash, MRB_SYM(y)),
		.w = util::read_hash_float(mrb, hash, MRB_SYM(w)),
		.h = util::read_hash_float(mrb, hash, MRB_SYM(h)),
	};
}

Widget::Flags
Widget::read_widget_flags(const mrb_value arr)
{
	Flags flags = {};
	for (mrb_int i = 0, len = RARRAY_LEN(arr); i < len; ++i) {
		const auto item = mrb_ary_entry(arr, i);
		if (!mrb_symbol_p(item)) continue;
		switch (mrb_symbol(item)) {
		case MRB_SYM(border): flags.border = true; break;
		case MRB_SYM(moveable): flags.moveable = true; break;
		case MRB_SYM(scalable): flags.scalable = true; break;
		case MRB_SYM(closeable): flags.closeable = true; break;
		case MRB_SYM(minimizable): flags.minimizable = true; break;
		case MRB_SYM(no_scrollbar): flags.no_scrollbar = true; break;
		case MRB_SYM(title): flags.title = true; break;
		case MRB_SYM(scroll_auto_hide):
			flags.scroll_auto_hide = true;
			break;
		case MRB_SYM(background): flags.background = true; break;
		case MRB_SYM(scale_left): flags.scale_left = true; break;
		case MRB_SYM(no_input): flags.no_input = true; break;
		default: break;
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
