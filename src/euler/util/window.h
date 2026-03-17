/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_WINDOW_H
#define EULER_UTIL_WINDOW_H

#include "euler/util/object.h"
#include "euler/util/state.h"
#include "euler/util/ext.h"

namespace euler::util {
class Window : public util::Object {
public:
	/* Due to some hard limits, 32kx32k displays are the largest supported,
	 * due to constraints in our GUI library. Fortunately, these do not yet
	 * exist, so we're good for now. */
	virtual int16_t width() const = 0;
	virtual int16_t height() const = 0;
};
} /* namespace euler::util */


#endif /* EULER_UTIL_WINDOW_H */

