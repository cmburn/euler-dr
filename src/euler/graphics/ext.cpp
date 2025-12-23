/* SPDX-License-Identifier: ISC */

#include "euler/graphics/ext.h"
#include "image.h"

void
euler::graphics::init(const util::Reference<util::State> &state)
{
	const auto mrb = state->state();
	auto &mod = state->module();
	mod.graphics.module
	    = mrb_define_module_under(mrb, mod.module, "Graphics");
	Image::init(mrb, mod);
}