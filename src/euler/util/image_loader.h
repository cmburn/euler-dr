/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_IMAGE_LOADER_H
#define EULER_UTIL_IMAGE_LOADER_H

#include "euler/util/image.h"
#include "euler/util/object.h"

namespace euler::util {
class ImageLoader : public Object {
public:
	[[nodiscard]] virtual Reference<Image> load_image(const char *path) = 0;
	[[nodiscard]] virtual Reference<Image> create_image(const char *label,
	    int16_t w, int16_t h, Color)
	    = 0;
};
} /* namespace euler::util */

#endif /* EULER_UTIL_IMAGE_LOADER_H */
