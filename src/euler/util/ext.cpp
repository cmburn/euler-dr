/* SPDX-License-Identifier: ISC */

#include "euler/util/ext.h"

#include "euler/util/logger.h"
#include "euler/util/version.h"
#include "euler/util/color.h"

void
euler::util::init(const Reference<State> &state, RClass *euler_mod)
{
	const auto mod = state->mrb()->define_module_under(euler_mod, "Util");
	(void)mod;
	auto &util = state->modules().util;
	util.logger = Logger::init(state, mod);
	util.version = Version::init(state, mod);
	util.color = Color::init(state, mod);
}