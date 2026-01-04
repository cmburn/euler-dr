/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_EXT_H
#define EULER_PHYSICS_EXT_H

#include <box2d/collision.h>

#include "euler/util/object.h"

namespace euler::physics {

RClass *init(const util::Reference<util::State> &mrb, RClass *outer);

mrb_value segment_wrap(mrb_state *mrb, b2Segment segment);
b2Segment segment_unwrap(mrb_state *mrb, mrb_value segment_value);

} /* namespace euler::physics */

#endif /* EULER_PHYSICS_EXT_H */
