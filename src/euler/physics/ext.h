/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_EXT_H
#define EULER_PHYSICS_EXT_H

#include "euler/util/object.h"

namespace euler::physics {

void init(const util::Reference<util::State> &mrb, RClass *physics_mod);

} /* namespace euler::physics */


#endif /* EULER_PHYSICS_EXT_H */

