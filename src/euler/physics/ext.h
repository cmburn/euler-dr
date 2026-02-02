/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_EXT_H
#define EULER_PHYSICS_EXT_H

#include <variant>

#include <box2d/collision.h>

#include "euler/util/object.h"

namespace euler::physics {

RClass *init(const util::Reference<util::State> &mrb, RClass *outer);

} /* namespace euler::physics */

#endif /* EULER_PHYSICS_EXT_H */
