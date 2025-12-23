/* SPDX-License-Identifier: ISC */

#include <cassert>

#include <dragonruby.h>

#include "euler/app/state.h"

extern "C" DRB_FFI_EXPORT void
drb_register_c_extensions_with_api(mrb_state *mrb, drb_api_t *api)
{
	assert(mrb->ud == nullptr && "User data pointer is already set");
	assert(api->drb_log_write != nullptr && "Log function is null");
	const euler::app::State::Arguments args = {
		.state = mrb,
		.api = api,
	};
	const auto state
	    = euler::util::make_reference<euler::app::State>(args);
	state->initialize();
	state->log()->info("Registered Euler with DragonRuby.");
}
