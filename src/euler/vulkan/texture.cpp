/* SPDX-License-Identifier: ISC */

#include "euler/vulkan/texture.h"

#include <VK2D/Renderer.h>
#include <VK2D/Texture.h>

euler::vulkan::Texture::~Texture() { vk2dTextureFree(_texture); }

void
euler::vulkan::Texture::display(const Frame &spec) const
{
	vk2dRendererDrawTexture(_texture, spec.position.x, spec.position.y,
	    spec.scale.x, spec.scale.y, spec.theta, spec.sheet.origin.x,
	    spec.sheet.origin.y, spec.sheet.x, spec.sheet.y, spec.sheet.width,
	    spec.sheet.height);
}

bool
euler::vulkan::Texture::is_target() const
{
	return vk2dTextureIsTarget(_texture);
}

float
euler::vulkan::Texture::width() const
{
	return vk2dTextureWidth(_texture);
}

float
euler::vulkan::Texture::height() const
{
	return vk2dTextureHeight(_texture);
}

uint32_t
euler::vulkan::Texture::id() const
{
	return vk2dTextureGetID(_texture);
}

euler::vulkan::Texture::Texture(VK2DTexture texture)
    : _texture(texture)
{
}

euler::vulkan::Texture::Texture(const char *path)
    : Texture(vk2dTextureLoad(path))
{
}

euler::vulkan::Texture::Texture(const std::span<const std::byte> buffer)
    : Texture(vk2dTextureFrom(buffer.data(), buffer.size()))
{
}

euler::vulkan::Texture::Texture(float width, float height)
    : Texture(vk2dTextureCreate(width, height))
{
}
