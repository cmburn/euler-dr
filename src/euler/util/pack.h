/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_PACK_H
#define EULER_UTIL_PACK_H

#include "euler/util/object.h"

namespace euler::util {

mrb_value pack(mrb_state *mrb, const mrb_value hash);

} /* namespace euler::util */


#endif /* EULER_UTIL_PACK_H */

