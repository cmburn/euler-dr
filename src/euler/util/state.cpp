/* SPDX-License-Identifier: ISC */

#include <assert.h>

#include "euler/util/state.h"

#include <stdexcept>

euler::util::Reference<euler::util::State>
euler::util::State::get(const mrb_state *mrb)
{
	const auto state = static_cast<State *>(mrb->ud);
	assert(state != nullptr && "State not set in mrb_state user data");
	return Reference(state);
}