/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_IMAGE_H
#define EULER_UTIL_IMAGE_H

#include "euler/util/color.h"
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

	struct Transform {
		struct {
			float x;
			float y;
		} scale;
		float theta;
		bool
		is_stable() const
		{
			return scale.x == 1.0f && scale.y == 1.0f
			    && theta == 0.0f;
		}
	};

	struct Frame {
		Location sheet;
		struct {
			float x;
			float y;
		} position;
		Transform transform;
	};

	struct Rectangle {
		float x;
		float y;
		float width;
		float height;
	};

	virtual float width() const = 0;
	virtual float height() const = 0;
	virtual bool is_target() const = 0;
	virtual bool is_loaded() const = 0;
	virtual Reference<Image> transform(const Transform &) const = 0;
	virtual Reference<Image> display(const Frame &spec) const = 0;
	virtual void display(const Location &spec) const = 0;
	virtual Reference<Image> rotate(float theta) const = 0;
	virtual util::Reference<Image> stretch(float x, float y) = 0;
	virtual std::pair<int16_t, int16_t> dimensions() const = 0;

	virtual void set_pixel(int16_t, int16_t, Color) = 0;
	virtual Color pixel(int16_t, int16_t) const = 0;
	virtual void blend_pixel(int16_t, int16_t, Color) = 0;
	virtual uint32_t *raw_data() = 0;
	virtual const uint32_t *raw_data() const = 0;
	virtual std::string label() const = 0;
};
} /* namespace euler::util */

#endif /* EULER_UTIL_IMAGE_H */
