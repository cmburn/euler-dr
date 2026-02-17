/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_TEXTURE_H
#define EULER_VULKAN_TEXTURE_H

#include <filesystem>
#include <span>

#include "euler/util/image.h"
#include "euler/util/object.h"
#include "euler/vulkan/internal.h"

namespace euler::vulkan {

class Texture : public util::Image {
public:
	~Texture() override;

	detail::texture_pointer
	texture() const
	{
		return _texture;
	}

	void display(const Frame &spec) const override;
	bool is_target() const override;
	float width() const override;
	float height() const override;
	uint32_t id() const;
	bool
	is_loaded() const override
	{
		return _texture != nullptr;
	}

protected:
	explicit Texture(detail::texture_pointer texture);
	explicit Texture(const char *path);
	explicit Texture(std::span<const std::byte> buffer);
	explicit Texture(float width, float height);

private:
	detail::texture_pointer _texture;
};

} /* namespace euler::vulkan */

#endif /* EULER_VULKAN_TEXTURE_H */
