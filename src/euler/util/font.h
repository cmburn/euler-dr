/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_FONT_H
#define EULER_UTIL_FONT_H

#include "euler/util/object.h"

namespace euler::util {
class Font : public util::Object {
public:
	virtual std::string path() const = 0;

};
} /* namespace euler::util */


#endif /* EULER_UTIL_FONT_H */

