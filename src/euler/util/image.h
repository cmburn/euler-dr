/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_IMAGE_H
#define EULER_UTIL_IMAGE_H

#include "euler/util/object.h"

namespace euler::util {
class Image : public Object {
public:
	~Image() override = default;

	/* Specifies a location on a texture sheet. Generally used for
	 * spritesheets. */
	struct Location {
		struct {
			float x = 0.0f;
			float y = 0.0f;
		} origin;
		float x = 0.0f;
		float y = 0.0f;
		float width = 0.0f;
		float height = 0.0f;
	};

	struct Frame {
		Location sheet;
		struct {
			float x;
			float y;
		} position;
		struct {
			float x;
			float y;
		} scale;
		float theta;
	};

	virtual float width() const = 0;
	virtual float height() const = 0;
	virtual bool is_target() const = 0;
	virtual bool is_loaded() const = 0;
	virtual void display(const Frame &spec) const = 0;
};
} /* namespace euler::util */

#endif /* EULER_UTIL_IMAGE_H */
