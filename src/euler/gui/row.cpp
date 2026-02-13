/* SPDX-License-Identifier: ISC */

#include "euler/gui/row.h"
#include "internal.h"
#include "window.h"

#include "euler/gui/widget.h"
#include "euler/util/ext.h"
#include "mruby/string.h"

using euler::gui::Row;

const Row::Settings Row::DEFAULT_SETTINGS = Settings();

Row::~Row() = default;

void
Row::button(const Button::Settings &settings,
    const std::function<void(util::Reference<Button> &)> &on_click)
{
	auto button
	    = util::make_reference<Button>(util::Reference(this), settings);
	button->call(on_click);
}

euler::util::Reference<euler::gui::Button>
Row::button(const Button::Settings &settings)
{
	return util::make_reference<Button>(util::Reference(this), settings);
}

static mrb_value
button_symbol(mrb_state *mrb, const mrb_value self_value)
{
	// const auto self = euler::util::unwrap<euler::gui::Button>(mrb,
	//     self_value, &euler::gui::Button::TYPE);
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<euler::gui::Button>(self_value);
	return euler::gui::from_symbol(mrb, self->symbol());
}

static euler::gui::Button::Settings
read_button_args(mrb_state *mrb, mrb_value *block)
{
	static const std::array KW_NAMES = {
		EULER_SYM(label),
		EULER_SYM(color),
		EULER_SYM(symbol),
	};
	mrb_value kw_values[KW_NAMES.size()];
	mrb_kwargs kwargs = {
		.num = 3,
		.required = 0,
		.table = KW_NAMES.data(),
		.values = kw_values,
		.rest = nullptr,
	};
	const auto state = euler::util::State::get(mrb);
	state->mrb()->get_args(":&", &kwargs, block);
	euler::gui::Button::Settings settings = {};
	if (!mrb_undef_p(kw_values[0]))
		settings.label = state->mrb()->string_cstr(kw_values[0]);
	if (!mrb_undef_p(kw_values[1]))
		settings.color = euler::util::Color::read(mrb, kw_values[1]);
	if (!mrb_undef_p(kw_values[2])) {
		settings.symbol
		    = euler::gui::to_symbol(mrb, mrb_symbol(kw_values[2]));
	}
	return settings;
}

static mrb_value
row_button(mrb_state *mrb, const mrb_value self_value)
{
	using namespace euler;
	auto state = util::State::get(mrb);
	auto self = state->unwrap<Row>(self_value);
	// auto self = util::unwrap<Row>(mrb, self_value, &Row::TYPE);
	if (!state->mrb()->block_given_p()) {
		mrb_raise(mrb, mrb->eStandardError_class,
		    "Block required for Row#button");
	}
	mrb_value block = mrb_nil_value();
	auto settings = read_button_args(mrb, &block);
	const auto klass = state->modules().gui.button;
	mrb_value out = mrb_nil_value();
	self->button(settings, [&](util::Reference<gui::Button> &b) {
		// const auto value
		//     = euler::util::wrap(mrb, b, klass, &gui::Button::TYPE);
		const auto value = state->wrap(b);
		out = mrb_yield(mrb, block, value);
	});
	return out;
}

void
Row::init(mrb_state *mrb, util::State::Modules &mod)
{
	mod.gui.row = mrb_define_class_under(mrb, mod.gui.module, "Row",
	    mrb->object_class);
	const auto row = mod.gui.row;
	MRB_SET_INSTANCE_TT(row, MRB_TT_CDATA);
	mrb_define_method(mrb, row, "button", row_button, MRB_ARGS_KEY(0, 3));
}

Row::Row(const util::Reference<Widget> &window, const Settings &settings)
    : _widget(window)
    , _height(settings.height)
    , _cols(settings.columns)
    , _layout(settings.layout)
{
}

void
Row::call(const std::function<void(util::Reference<Row> &)> &fn)
{
	auto self = util::Reference(this);
	const auto ctx = _widget->gui()->context();
	switch (_layout) {
	case Layout::Dynamic: nk_layout_row_dynamic(ctx, _height, _cols); break;
	case Layout::Custom:
	case Layout::Static:
		throw std::runtime_error("Row layout not yet implemented");
	}
	fn(self);
}

euler::util::Reference<euler::gui::Widget>
Row::widget() const
{
	return _widget;
}
void
Row::end_row()
{
	nk_layout_row_end(_widget->gui()->context());
}
