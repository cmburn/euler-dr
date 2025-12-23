/* SPDX-License-Identifier: ISC */

#include "euler/gui/ext.h"

#include <mruby.h>

#include "euler/gui/widget.h"

void
euler::gui::init(const util::Reference<util::State> &state)
{
	const auto mrb = state->state();
	auto &mod = state->module();
	mod.gui.module = mrb_define_module_under(mrb, mod.module, "Gui");
	Widget::init(mrb, mod);
	Row::init(mrb, mod);
	Button::init(mrb, mod);
}