/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_MATH_H
#define EULER_MATH_MATH_H

#include "euler/util/object.h"
#include "euler/util/math.h"
#include "euler/util/ext.h"

namespace euler::math {

RClass *init(const util::Reference<util::State> &state, RClass *mod,
	RClass *super = nullptr);

} /* namespace euler::math */


#endif /* EULER_MATH_MATH_H */

