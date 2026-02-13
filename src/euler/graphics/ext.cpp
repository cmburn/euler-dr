/* SPDX-License-Identifier: ISC */

#include "euler/graphics/ext.h"
#include "image.h"

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