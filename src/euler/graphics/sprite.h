/* SPDX-License-Identifier: ISC */

#ifndef EULER_GRAPHICS_SPRITE_H
#define EULER_GRAPHICS_SPRITE_H

#include "euler/graphics/image.h"
#include "euler/vulkan/texture.h"

#include <glm/vec2.hpp>

namespace euler::graphics {
class Window;

/*
 * Correlates to a sprite (likely somewhere on a spritesheet) that can be drawn
 * from an Image.
 */
class Sprite : public util::Object {
public:
	Sprite(const util::Reference<Image> &image);
	~Sprite() override = default;

	virtual void display(float x, float y, float sx = 1.0f, float sy = 1.0f,
	    float theta = 0.0f) const
	    = 0;

	const util::Reference<Image> &image() const { return _image; }

private:
	util::Reference<Image> _image;
};
} /* namespace euler::graphics */

#endif /* EULER_GRAPHICS_SPRITE_H */
