/* SPDX-License-Identifier: ISC */

#include "euler/gui/style.h"

#include "euler/gui/internal.h"

using namespace euler::gui;
using namespace euler;

static nk_color
color_to_nuklear(const util::Color color)
{
	return nk_color {
		.r = color.red(),
		.g = color.green(),
		.b = color.blue(),
		.a = color.alpha(),
	};
}

static euler::util::Color
color_from_nuklear(const nk_color color)
{
	return util::Color {
		color.r,
		color.g,
		color.b,
		color.a,
	};
}

static euler::util::Reference<euler::graphics::Image>
image_from_nuklear(const util::Reference<util::State> &,
    const struct nk_image &)
{
	// TODO: this gets tricky
	return nullptr;
}

static struct nk_image
image_to_nuklear(const util::Reference<util::State> &,
    const euler::util::Reference<euler::graphics::Image> &)
{
	// TODO: this also gets tricky
	return {};
}

static_assert(NK_COLOR_COUNT == ColorTable::ATTRIBUTE_COUNT,
    "Nuklear color count does not match ColorTable attribute count");

void
ColorTable::to_nuklear(const util::Reference<util::State> &state,
    nk_color table[NK_COLOR_COUNT]) const
{
	table[NK_COLOR_TEXT] = gui::to_nuklear(state, text);
	table[NK_COLOR_WINDOW] = gui::to_nuklear(state, window);
	table[NK_COLOR_HEADER] = gui::to_nuklear(state, header);
	table[NK_COLOR_BORDER] = gui::to_nuklear(state, border);
	table[NK_COLOR_BUTTON] = gui::to_nuklear(state, button);
	table[NK_COLOR_BUTTON_HOVER] = gui::to_nuklear(state, button_hover);
	table[NK_COLOR_BUTTON_ACTIVE] = gui::to_nuklear(state, button_active);
	table[NK_COLOR_TOGGLE] = gui::to_nuklear(state, toggle);
	table[NK_COLOR_TOGGLE_HOVER] = gui::to_nuklear(state, toggle_hover);
	table[NK_COLOR_TOGGLE_CURSOR] = gui::to_nuklear(state, toggle_cursor);
	table[NK_COLOR_SELECT] = gui::to_nuklear(state, select);
	table[NK_COLOR_SELECT_ACTIVE] = gui::to_nuklear(state, select_active);
	table[NK_COLOR_SLIDER] = gui::to_nuklear(state, slider);
	table[NK_COLOR_SLIDER_CURSOR] = gui::to_nuklear(state, slider_cursor);
	table[NK_COLOR_SLIDER_CURSOR_HOVER]
	    = gui::to_nuklear(state, slider_cursor_hover);
	table[NK_COLOR_SLIDER_CURSOR_ACTIVE]
	    = gui::to_nuklear(state, slider_cursor_active);
	table[NK_COLOR_PROPERTY] = gui::to_nuklear(state, property);
	table[NK_COLOR_EDIT] = gui::to_nuklear(state, edit);
	table[NK_COLOR_EDIT_CURSOR] = gui::to_nuklear(state, edit_cursor);
	table[NK_COLOR_COMBO] = gui::to_nuklear(state, dropdown);
	table[NK_COLOR_CHART] = gui::to_nuklear(state, chart);
	table[NK_COLOR_CHART_COLOR] = gui::to_nuklear(state, chart_color);
	table[NK_COLOR_CHART_COLOR_HIGHLIGHT]
	    = gui::to_nuklear(state, chart_color_highlight);
	table[NK_COLOR_SCROLLBAR] = gui::to_nuklear(state, scrollbar);
	table[NK_COLOR_SCROLLBAR_CURSOR]
	    = gui::to_nuklear(state, scrollbar_cursor);
	table[NK_COLOR_SCROLLBAR_CURSOR_HOVER]
	    = gui::to_nuklear(state, scrollbar_cursor_hover);
	table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE]
	    = gui::to_nuklear(state, scrollbar_cursor_active);
	table[NK_COLOR_TAB_HEADER] = gui::to_nuklear(state, tab_header);
	table[NK_COLOR_KNOB] = gui::to_nuklear(state, knob);
	table[NK_COLOR_KNOB_CURSOR] = gui::to_nuklear(state, knob_cursor);
	table[NK_COLOR_KNOB_CURSOR_HOVER]
	    = gui::to_nuklear(state, knob_cursor_hover);
	table[NK_COLOR_KNOB_CURSOR_ACTIVE]
	    = gui::to_nuklear(state, knob_cursor_active);
}

nk_style_item
Style::Item::to_nuklear(const util::Reference<util::State> &state) const
{
	if (std::holds_alternative<util::Color>(data)) {
		// item.type = NK_STYLE_ITEM_COLOR;
		// item.data.color = to_nuklear(state,
		// std::get<util::Color>(data)); return;
		return {
			.type = NK_STYLE_ITEM_COLOR,
			.data = {
				.color = gui::to_nuklear(state,
					std::get<util::Color>(data)),
			},
		};
	}
	if (std::holds_alternative<util::Reference<graphics::Image>>(data)) {
		return {
			.type = NK_STYLE_ITEM_IMAGE,
			.data = {
				.image = gui::to_nuklear(
				    state,
				    std::get<
				        util::Reference<graphics::Image>>(
				        data)),
			},
		};
	}
	assert(std::holds_alternative<NineSlice>(data));
	nk_style_item item = {};
	item.type = NK_STYLE_ITEM_NINE_SLICE;
	const NineSlice &slice = std::get<NineSlice>(data);
	item.data.slice.img = gui::to_nuklear(state, slice.image);
	item.data.slice.l = slice.left;
	item.data.slice.t = slice.top;
	item.data.slice.r = slice.right;
	item.data.slice.b = slice.bottom;
	/* ReSharper disable once CppSomeObjectMembersMightNotBeInitialized */
	return item;
}

nk_style_button
Style::Button::to_nuklear(const util::Reference<util::State> &state) const
{
	return {
		.normal = normal.to_nuklear(state),
		.hover = hover.to_nuklear(state),
		.active = active.to_nuklear(state),
		.border_color = gui::to_nuklear(state, border_color),
		.color_factor_background = color_factor_background,
		.text_background = gui::to_nuklear(state, text_background),
		.text_normal = gui::to_nuklear(state, text_normal),
		.text_hover = gui::to_nuklear(state, text_hover),
		.text_active = gui::to_nuklear(state, text_active),
		.text_alignment = gui::to_nuklear_text(state, text_alignment),
		.color_factor_text = color_factor_text,
		.border = border,
		.rounding = rounding,
		.padding = { padding.x, padding.y },
		.image_padding = { image_padding.x, image_padding.y },
		.touch_padding = { touch_padding.x, touch_padding.y },
		.disabled_factor = NK_WIDGET_DISABLED_FACTOR,
		.userdata = { nullptr },
		.draw_begin = nullptr,
		.draw_end = nullptr,
	};
}
