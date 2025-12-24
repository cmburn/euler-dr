/* SPDX-License-Identifier: ISC */

#include <assert.h>

#include "euler/util/state.h"

using euler::util::State;

#undef EULER_DRAGONRUBY
#ifdef EULER_DRAGONRUBY
static euler::util::Reference<State> global_state;

State::State()
{
	global_state = Reference(this);
}

euler::util::Reference<State>
State::get(const mrb_state *)
{
	assert(global_state != nullptr && "Global State not set");
	return global_state;
}
#else
State::State() = default;
euler::util::Reference<State>
State::get(const mrb_state *mrb)
{
	printf("sizeof(mrb_state) = %zu\n", sizeof(mrb_state));
	const auto state = Reference<State>::unwrap(mrb->ud);
	assert(state != nullptr && "State not set in mrb_state user data");
	return state;
}
#endif

