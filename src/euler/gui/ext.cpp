/* SPDX-License-Identifier: ISC */

#include "euler/gui/ext.h"

#include <mruby.h>

#include "euler/gui/widget.h"

RClass *
euler::gui::init(const util::Reference<util::State> &state, RClass *mod,
    RClass *)
{
	auto &gui = state->modules().gui;
	gui.mod = state->mrb()->define_module_under(mod, "Gui");
	// Widget::init(state, gui.mod);
	return gui.mod;
}