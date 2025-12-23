/* SPDX-License-Identifier: ISC */

#ifndef EULER_GRAPHICS_BASIC_SPRITE_H
#define EULER_GRAPHICS_BASIC_SPRITE_H

#include "euler/graphics/sprite.h"
#include "euler/util/object.h"

namespace euler::graphics {
class BasicSprite final : public Sprite {
public:
	using SpritesheetLocation = vulkan::Texture::Location;

	BasicSprite(const util::Reference<Image> &image,
	    const SpritesheetLocation &frame);

	void display(float x, float y, float scale_x = 1.0f,
	    float scale_y = 1.0f, float theta = 0.0f) const override;

private:
	SpritesheetLocation _location;
};
} /* namespace euler::graphics */

#endif /* EULER_GRAPHICS_BASIC_SPRITE_H */
