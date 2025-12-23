/* SPDX-License-Identifier: ISC */

#include "euler/graphics/image.h"

#include <VK2D/Renderer.h>
#include <VK2D/Texture.h>

#include "euler/util/ext.h"

using euler::graphics::Image;

const mrb_data_type Image::TYPE = MAKE_REFERENCE_TYPE(euler::graphics::Image);

static mrb_value
image_from_file(mrb_state *mrb, const mrb_value)
{
	char *path = nullptr;
	mrb_get_args(mrb, "z", &path);
	const auto state = euler::util::State::get(mrb);
	const auto img = Image::from_file(path);
	if (!img->loaded()) {
		mrb_raisef(mrb, E_ARGUMENT_ERROR,
		    "Unable to load image from file: %s", path);
	}
	return euler::util::wrap(mrb, img, state->module().graphics.image,
	    &Image::TYPE);
}

static mrb_value
image_from_buffer(mrb_state *mrb, const mrb_value)
{
	const char *ptr = nullptr;
	mrb_int len = 0;
	mrb_get_args(mrb, "s", &ptr, &len);
	const auto buffer = std::span(reinterpret_cast<const std::byte *>(ptr),
	    static_cast<size_t>(len));
	const auto state = euler::util::State::get(mrb);
	const auto img = Image::from_buffer(buffer);
	return euler::util::wrap(mrb, img, state->module().graphics.image,
	    &Image::TYPE);
}

static mrb_value
image_create_target(mrb_state *mrb, const mrb_value)
{
	mrb_float width = 0.0f;
	mrb_float height = 0.0f;
	mrb_get_args(mrb, "ff", &width, &height);
	const auto state = euler::util::State::get(mrb);
	const auto img = Image::create_target(static_cast<float>(width),
	    static_cast<float>(height));
	return euler::util::wrap(mrb, img, state->module().graphics.image,
	    &Image::TYPE);
}

static euler::vulkan::Texture::Location
read_location_args(mrb_state *mrb, const mrb_value hash)
{
	using namespace euler::util;
	using Location = euler::vulkan::Texture::Location;
	static const auto ORIGIN_SYM = mrb_symbol_value(MRB_SYM(origin));
	const auto origin = mrb_hash_get(mrb, hash, ORIGIN_SYM);
	float origin_x, origin_y;
	if (mrb_nil_p(origin)) {
		origin_x = 0.5f;
		origin_y = 0.5f;
	} else {
		origin_x = read_hash_float(mrb, origin, MRB_SYM(x), 0.5f);
		origin_y = read_hash_float(mrb, origin, MRB_SYM(y), 0.5f);
	}

	const auto out = Location {
		.origin = {
			.x = origin_x,
			.y = origin_y,
		},
		.x = read_hash_float(mrb, hash, MRB_SYM(x)),
		.y = read_hash_float(mrb, hash, MRB_SYM(y)),
		.width = read_hash_float(mrb, hash, MRB_SYM(width)),
		.height = read_hash_float(mrb, hash, MRB_SYM(height)),
	};
	return out;
}

static euler::vulkan::Texture::Frame
read_frame_args(mrb_state *mrb, const mrb_value hash)
{
	using namespace euler::util;
	using Frame = euler::vulkan::Texture::Frame;
	static const auto POSITION_SYM = mrb_symbol_value(MRB_SYM(position));
	static const auto SHEET_SYM = mrb_symbol_value(MRB_SYM(sheet));
	static const auto SCALE_SYM = mrb_symbol_value(MRB_SYM(scale));
	const auto sheet = mrb_hash_get(mrb, hash, SHEET_SYM);
	if (mrb_nil_p(sheet))
		mrb_raise(mrb, E_ARGUMENT_ERROR, "Missing sheet location");
	const auto position = mrb_hash_get(mrb, hash, POSITION_SYM);
	if (mrb_nil_p(position))
		mrb_raise(mrb, E_ARGUMENT_ERROR, "Missing position");
	const auto scale = mrb_hash_get(mrb, hash, SCALE_SYM);
	if (mrb_nil_p(scale)) mrb_raise(mrb, E_ARGUMENT_ERROR, "Missing scale");

	return Frame {
		.sheet = read_location_args(mrb, sheet),
		.position = {
			.x = read_hash_float(mrb, position, MRB_SYM(x)),
			.y = read_hash_float(mrb, position, MRB_SYM(y)),
		},
		.scale = {
			.x = read_hash_float(mrb, scale, MRB_SYM(x)),
			.y = read_hash_float(mrb, scale, MRB_SYM(y)),
		},
		.theta = read_hash_float(mrb, hash, MRB_SYM(theta), 0.0f),
	};
}

static mrb_value
image_display(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap<Image>(mrb, self_value, &Image::TYPE);
	mrb_value hash;
	mrb_get_args(mrb, "H", &hash);
	const auto frame = read_frame_args(mrb, hash);
	self->display(frame);
	return mrb_nil_value();
}

static mrb_value
image_is_target(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap<Image>(mrb, self_value, &Image::TYPE);
	return mrb_bool_value(self->is_target());
}

static mrb_value
image_width(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap<Image>(mrb, self_value, &Image::TYPE);
	return mrb_float_value(mrb, self->width());
}

static mrb_value
image_height(mrb_state *mrb, const mrb_value self_value)
{
	const auto self
	    = euler::util::unwrap<Image>(mrb, self_value, &Image::TYPE);
	return mrb_float_value(mrb, self->height());
}

void
Image::init(mrb_state *mrb, util::State::Modules &mod)
{
	mod.graphics.image = mrb_define_class_under(mrb, mod.graphics.module,
	    "Image", mrb->object_class);
	const auto image = mod.graphics.image;
	MRB_SET_INSTANCE_TT(image, MRB_TT_CDATA);
	mrb_define_class_method(mrb, image, "from_file", image_from_file,
	    MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, image, "from_buffer", image_from_buffer,
	    MRB_ARGS_REQ(1));
	mrb_define_class_method(mrb, image, "create_target",
	    image_create_target, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, image, "display", image_display,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, image, "target?", image_is_target,
	    MRB_ARGS_NONE());
	mrb_define_method(mrb, image, "width", image_width, MRB_ARGS_NONE());
	mrb_define_method(mrb, image, "height", image_height, MRB_ARGS_NONE());
}

euler::util::Reference<Image>
Image::from_file(const char *path)
{
	return util::Reference(new Image(path));
}

euler::util::Reference<Image>
Image::from_buffer(const std::span<const std::byte> buffer)
{
	return util::Reference(new Image(buffer));
}

euler::util::Reference<Image>
Image::create_target(const float width, const float height)
{
	return util::Reference(new Image(width, height));
}

Image::Image(const char *path)
    : Texture(path)
{
}

Image::Image(const std::span<const std::byte> buffer)
    : Texture(buffer)
{
}

Image::Image(float width, float height)
    : Texture(width, height)
{
}
