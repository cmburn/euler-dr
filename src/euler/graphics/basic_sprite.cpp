/* SPDX-License-Identifier: ISC */

#include "euler/graphics/basic_sprite.h"

euler::graphics::BasicSprite::BasicSprite(const util::Reference<Image> &image,
    const SpritesheetLocation &frame)
    : Sprite(image)
    , _location(SpritesheetLocation {
	  .origin = frame.origin,
	  .x = frame.x,
	  .y = frame.y,
	  .width = frame.width,
	  .height = frame.height,
      })
{
}

void
euler::graphics::BasicSprite::display(const float x, const float y,
    const float scale_x, const float scale_y, const float theta) const
{
	const vulkan::Texture::Frame frame = {
		.sheet = {
			.origin = {
				.x = _location.origin.x,
				.y = _location.origin.y,
			},
			.x = _location.x,
			.y = _location.y,
			.width = _location.width,
			.height = _location.height,
		},
		.position = { .x = x, .y = y },
		.scale = { .x = scale_x, .y = scale_y },
		.theta = theta,
	};
	image()->display(frame);
}