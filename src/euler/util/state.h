/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_STATE_H
#define EULER_UTIL_STATE_H

#include "euler/util/object.h"
#include "euler/util/mruby_state.h"

namespace euler::util {
class State : public Object {
public:
	[[nodiscard]] virtual const Reference<MRubyState> &mrb() const = 0;
};

} /* namespace euler::util */


#endif /* EULER_UTIL_STATE_H */

