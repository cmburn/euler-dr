/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_EXT_H
#define EULER_MATH_EXT_H

#include "euler/util/state.h"

namespace euler::math {

RClass *init(const util::Reference<util::State> &state, RClass *mod,
    RClass *super = nullptr);

} /* namespace euler::math */

#endif /* EULER_MATH_EXT_H */
