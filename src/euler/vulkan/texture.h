/* SPDX-License-Identifier: ISC */

#ifndef EULER_VULKAN_TEXTURE_H
#define EULER_VULKAN_TEXTURE_H

#include <filesystem>
#include <span>

#include "euler/util/object.h"
#include "euler/vulkan/internal.h"

namespace euler::vulkan {

class Texture : public util::Object {
public:
	~Texture() override;

	detail::texture_pointer
	texture() const
	{
		return _texture;
	}

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

	void display(const Frame &spec) const;

	bool is_target() const;
	float width() const;
	float height() const;
	uint32_t id() const;
	bool loaded() const { return _texture != nullptr; }

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
