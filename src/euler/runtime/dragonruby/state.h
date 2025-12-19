/* SPDX-License-Identifier: ISC */

#ifndef EULER_RUNTIME_DRAGONRUBY_STATE_H
#define EULER_RUNTIME_DRAGONRUBY_STATE_H

#include "euler/util/object.h"
#include "euler/util/state.h"
#include "euler/runtime/dragonruby/state.h"

namespace euler::runtime::dragonruby {
class State final : public util::State {
public:
	[[nodiscard]] const util::Reference<util::MRubyState> &
	mrb() const override
	{
		return _mrb_state;
	}
private:
	util::Reference<util::MRubyState> _mrb_state;
};
} /* namespace euler::runtime::dragonruby */


#endif /* EULER_RUNTIME_DRAGONRUBY_STATE_H */

