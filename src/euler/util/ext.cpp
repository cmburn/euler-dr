/* SPDX-License-Identifier: ISC */

#include "euler/util/ext.h"

#include "euler/util/logger.h"

void
euler::util::init(const Reference<State> &state, RClass *euler_mod)
{
	const auto mod = state->mrb()->define_module_under(euler_mod, "Util");
	(void)mod;
	Logger::init(state, mod);
}