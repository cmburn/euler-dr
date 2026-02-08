/* SPDX-License-Identifier: ISC */

#ifndef EULER_GRAPHICS_IMAGE_H
#define EULER_GRAPHICS_IMAGE_H

#include "euler/util/ext.h"
#include "euler/util/state.h"
#include "euler/vulkan/texture.h"

namespace euler::graphics {
class Image final : public vulkan::Texture {
	BIND_MRUBY("Euler::Graphics::Image", Image, graphics.image);
public:
	static util::Reference<Image> from_file(const char *);
	static util::Reference<Image> from_buffer(std::span<const std::byte>);
	static util::Reference<Image> create_target(float, float);

private:
	explicit Image(const char *path);
	explicit Image(std::span<const std::byte> buffer);
	explicit Image(float width, float height);
};
} /* namespace euler::graphics */

#endif /* EULER_GRAPHICS_IMAGE_H */
