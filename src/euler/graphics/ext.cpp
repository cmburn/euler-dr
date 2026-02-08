/* SPDX-License-Identifier: ISC */

#include "euler/graphics/ext.h"
#include "image.h"

// const auto mrb = state->mrb();
// auto &mod = state->module();
// mod.graphics.module
//     = mrb_define_module_under(mrb, mod.module, "Graphics");
// Image::init(mrb, mod);

RClass *
euler::graphics::init(const util::Reference<util::State> &state, RClass *mod,
    RClass *)
{
	const auto graphics
	    = state->mrb()->define_module_under(mod, "Graphics");
	auto &gmod = state->modules().graphics;
	gmod.image = Image::init(state, graphics);
	return graphics;
}