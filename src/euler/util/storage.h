/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_STORAGE_H
#define EULER_UTIL_STORAGE_H

#include "euler/util/object.h"
#include "euler/util/ext.h"

namespace euler::util {
class Storage : public Object {
	BIND_MRUBY("Euler::Util::Storage", Storage, util.storage);
public:
};
} /* namespace euler::util */

#endif /* EULER_UTIL_STORAGE_H */
