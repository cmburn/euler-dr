/* SPDX-License-Identifier: ISC */

#include "euler/graphics/image.h"

#include <VK2D/Renderer.h>
#include <VK2D/Texture.h>

#include "euler/util/ext.h"

using euler::graphics::Image;

// const mrb_data_type Image::TYPE =
// MAKE_REFERENCE_TYPE(euler::graphics::Image);

static mrb_value
image_from_file(mrb_state *mrb, const mrb_value)
{
	const auto state = euler::util::State::get(mrb);
	char *path = nullptr;
	state->mrb()->get_args("z", &path);
	auto img = Image::from_file(path);
	if (!img->is_loaded()) {
		state->mrb()->raisef(E_ARGUMENT_ERROR,
		    "Unable to load image from file: %s", path);
	}
	// return euler::util::wrap(mrb, img, state->module().graphics.image,
	//     &Image::TYPE);
	return state->wrap(img);
}

static mrb_value
image_from_buffer(mrb_state *mrb, const mrb_value)
{
	const auto state = euler::util::State::get(mrb);
	const char *ptr = nullptr;
	mrb_int len = 0;
	state->mrb()->get_args("s", &ptr, &len);
	const auto buffer = std::span(reinterpret_cast<const std::byte *>(ptr),
	    static_cast<size_t>(len));
	auto img = Image::from_buffer(buffer);
	// return euler::util::wrap(mrb, img, state->module().graphics.image,
	//     &Image::TYPE);
	return state->wrap(img);
}

static mrb_value
image_create_target(mrb_state *mrb, const mrb_value)
{
	const auto state = euler::util::State::get(mrb);
	mrb_float width = 0.0f;
	mrb_float height = 0.0f;
	state->mrb()->get_args("ff", &width, &height);
	auto img = Image::create_target(static_cast<float>(width),
	    static_cast<float>(height));
	// return euler::util::wrap(mrb, img, state->module().graphics.image,
	//     &Image::TYPE);
	return state->wrap(img);
}

static float
read_hash_float(mrb_state *mrb, const mrb_value hash, const mrb_sym key,
    const float default_value = 0.0f)
{
	using namespace euler::util;
	const auto state = State::get(mrb);
	const auto sym = mrb_symbol_value(key);
	const auto value = state->mrb()->hash_get(hash, sym);
	if (mrb_nil_p(value)) return default_value;
	return static_cast<float>(state->mrb()->to_flo(value));
}

static euler::vulkan::Texture::Location
read_location_args(mrb_state *mrb, const mrb_value hash)
{
	using namespace euler::util;
	using Location = euler::vulkan::Texture::Location;
	const auto origin_sym = mrb_symbol_value(EULER_SYM(origin));
	const auto state = State::get(mrb);
	const auto origin = state->mrb()->hash_get(hash, origin_sym);
	float origin_x, origin_y;
	if (mrb_nil_p(origin)) {
		origin_x = 0.5f;
		origin_y = 0.5f;
	} else {
		origin_x = read_hash_float(mrb, origin, EULER_SYM(x), 0.5f);
		origin_y = read_hash_float(mrb, origin, EULER_SYM(y), 0.5f);
	}

	const auto out = Location {
		.origin = {
			.x = origin_x,
			.y = origin_y,
		},
		.x = read_hash_float(mrb, hash, EULER_SYM(x)),
		.y = read_hash_float(mrb, hash, EULER_SYM(y)),
		.width = read_hash_float(mrb, hash, EULER_SYM(width)),
		.height = read_hash_float(mrb, hash, EULER_SYM(height)),
	};
	return out;
}

static euler::vulkan::Texture::Frame
read_frame_args(mrb_state *mrb, const mrb_value hash)
{
	using namespace euler::util;
	using Frame = euler::vulkan::Texture::Frame;
	const auto state = euler::util::State::get(mrb);
	const auto position_sym = mrb_symbol_value(EULER_SYM(position));
	const auto sheet_sym = mrb_symbol_value(EULER_SYM(sheet));
	const auto scale_sym = mrb_symbol_value(EULER_SYM(scale));
	const auto sheet = state->mrb()->hash_get(hash, sheet_sym);
	if (mrb_nil_p(sheet))
		state->mrb()->raise(E_ARGUMENT_ERROR, "Missing sheet location");
	const auto position = state->mrb()->hash_get(hash, position_sym);
	if (mrb_nil_p(position))
		state->mrb()->raise(E_ARGUMENT_ERROR, "Missing position");
	const auto scale = state->mrb()->hash_get(hash, scale_sym);
	if (mrb_nil_p(scale))
		state->mrb()->raise(E_ARGUMENT_ERROR, "Missing scale");

	return Frame {
		.sheet = read_location_args(mrb, sheet),
		.position = {
			.x = read_hash_float(mrb, position, EULER_SYM(x)),
			.y = read_hash_float(mrb, position, EULER_SYM(y)),
		},
		.scale = {
			.x = read_hash_float(mrb, scale, EULER_SYM(x)),
			.y = read_hash_float(mrb, scale, EULER_SYM(y)),
		},
		.theta = read_hash_float(mrb, hash, EULER_SYM(theta), 0.0f),
	};
}

static mrb_value
image_display(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<Image>(self_value);
	mrb_value hash;
	state->mrb()->get_args("H", &hash);
	const auto frame = read_frame_args(mrb, hash);
	self->display(frame);
	return mrb_nil_value();
}

static mrb_value
image_is_target(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<Image>(self_value);
	return mrb_bool_value(self->is_target());
}

static mrb_value
image_width(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<Image>(self_value);
	return state->mrb()->float_value(self->width());
}

static mrb_value
image_height(mrb_state *mrb, const mrb_value self_value)
{
	const auto state = euler::util::State::get(mrb);
	const auto self = state->unwrap<Image>(self_value);
	return state->mrb()->float_value(self->height());
}

RClass *
Image::init(const euler::util::Reference<euler::util::State> &state,
    RClass *mod, RClass *super)
{
	auto cls = state->mrb()->define_class_under(mod, "Image", super);
	MRB_SET_INSTANCE_TT(cls, MRB_TT_DATA);
	state->mrb()->define_class_method(cls, "from_file", image_from_file,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_class_method(cls, "from_buffer", image_from_buffer,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_class_method(cls, "create_target",
	    image_create_target, MRB_ARGS_REQ(2));
	state->mrb()->define_method(cls, "display", image_display,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(cls, "target?", image_is_target,
	    MRB_ARGS_NONE());
	state->mrb()->define_method(cls, "width", image_width, MRB_ARGS_NONE());
	state->mrb()->define_method(cls, "height", image_height,
	    MRB_ARGS_NONE());
	return cls;
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
