/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_DRAGONRUBY_IMAGE_H
#define EULER_APP_DRAGONRUBY_IMAGE_H

#include "euler/util/image.h"

namespace euler::app::dragonruby {
class State;
class Image final : public util::Image {
	friend class State;

public:
	Image(std::string_view label, const uint32_t *data, int16_t width,
	    int16_t height, bool own_data);
	~Image() override;
	float width() const override;
	float height() const override;
	bool is_target() const override;
	void set_pixel(int16_t, int16_t, util::Color) override;
	util::Color pixel(int16_t, int16_t) const override;
	void blend_pixel(int16_t, int16_t, util::Color) override;
	bool is_loaded() const override;
	util::Reference<util::Image> display(const Frame &spec) const override;
	uint32_t *raw_data() override;
	const uint32_t *raw_data() const override;
	util::Reference<util::Image> transform(
	    const Transform &) const override;
	util::Reference<util::Image> rotate(float theta) const override;
	util::Reference<util::Image> stretch(float x, float y) override;

private:
	util::Reference<util::State> _state;
	uint32_t *_data = nullptr;
	int16_t _width = 0;
	int16_t _height = 0;
	bool _own_data = false;
};
} /* namespace euler::app::dragonruby */

#endif /* EULER_APP_DRAGONRUBY_IMAGE_H */
