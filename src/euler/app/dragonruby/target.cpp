/* SPDX-License-Identifier: ISC */

#include "euler/app/dragonruby/target.h"

#include <iomanip>

#include <mapbox/earcut.hpp>

#include "euler/app/dragonruby/ruby_state.h"
#include "euler/app/dragonruby/state.h"

#include "euler/graphics/Target.h"

using euler::app::dragonruby::Renderer;

static constexpr auto all = Eigen::placeholders::all;

#define STASH_VALUE(HASH, KEY, VALUE)                                          \
	do {                                                                   \
		ruby()->hash_set(HASH, ruby()->symbol_value(_symbols.KEY),     \
		    (VALUE));                                                  \
	} while (0)

#define STASH_INT(HASH, KEY, VALUE)                                            \
	STASH_VALUE(HASH, KEY, ruby()->int_value(VALUE))

#define STASH_STR(HASH, KEY, VALUE)                                            \
	STASH_VALUE(HASH, KEY, ruby()->str_new_cstr(VALUE))

#define STASH_FLOAT(HASH, KEY, VALUE)                                          \
	STASH_VALUE(HASH, KEY, ruby()->float_value(VALUE))

#define STASH_SYM(HASH, KEY, VALUE)                                            \
	STASH_VALUE(HASH, KEY, ruby()->symbol_value(_symbols.VALUE))

Renderer::~Renderer() = default;

euler::util::Reference<euler::util::State>
Renderer::state() const
{
	return _state.strengthen().cast_to<util::State>();
}

void
Renderer::scissor(const ScissorCommand &cmd)
{
	(void)cmd;
	/* This is a no-op for DragonRuby */
	state()->log()->warn("Scissor command is not supported for DragonRuby");
}

void
Renderer::line(const LineCommand &cmd)
{
	Vec2i16 p1 = cmd.points(0, all);
	Vec2i16 p2 = cmd.points(1, all);
	const auto th = cmd.line_thickness;
	if (th == 1) {
		const auto hash = ruby()->hash_new_capa(8);
		STASH_INT(hash, x1, p1(0, 0));
		STASH_INT(hash, y1, p1(0, 1));
		STASH_INT(hash, x2, p2(0, 0));
		STASH_INT(hash, y2, p2(0, 1));
		push_output(_symbols.lines, hash);
		return;
	}
	const Vec2i16 l = p2 - p1;
	const float len = math::length(l);
	const float angle = math::rad2deg(math::angle(l));
	const float normal = std::fmod(angle + 90.0f, 360.0f);
	Vec2f vec = p1.cast<float>()
	    - math::deg2vec(normal) * (static_cast<float>(th) / 2);
	/* adjust y value due to how solids get rendered */
	vec(0, 1) += 2;
	const auto hash = ruby()->hash_new_capa(12);
	STASH_FLOAT(hash, x, vec(0, 0));
	STASH_FLOAT(hash, y, vec(0, 1));
	STASH_FLOAT(hash, angle, angle);
	STASH_FLOAT(hash, angle_anchor_x, 0.0f);
	STASH_FLOAT(hash, angle_anchor_y, 0.0f);
	STASH_SYM(hash, path, solid);
	STASH_FLOAT(hash, w, len);
	STASH_FLOAT(hash, h, th);
	cmd.color.store_hash(ruby()->mrb(), hash);
	push_output(_symbols.sprites, hash);
}

void
Renderer::curve(const CurveCommand &cmd)
{
	static constexpr int CURVE_SEGMENTS = 22;
	static constexpr float STEP = 1.0f / CURVE_SEGMENTS;
	const Vec2i16 p1 = cmd.points(0, all);
	const Vec2i16 p2 = cmd.points(1, all);
	const Vec2i16 p3 = cmd.points(2, all);
	const Vec2i16 p4 = cmd.points(3, all);
	Vec2i16 last = p1;
	LineCommand line_cmd = {
		.points = {},
		.color = cmd.color,
		.line_thickness = cmd.line_thickness,
	};
	for (int i = 1; i <= CURVE_SEGMENTS; ++i) {
		const float t = i * STEP;
		const float u = 1 - t;
		const float w1 = u * u * u;
		const float w2 = 3 * u * u * t;
		const float w3 = 3 * u * t * t;
		const float w4 = t * t * t;
		const float x = w1 * p1(0, 0) + w2 * p2(0, 0) + w3 * p3(0, 0)
		    + w4 * p4(0, 0);
		const float y = w1 * p1(0, 1) + w2 * p2(0, 1) + w3 * p3(0, 1)
		    + w4 * p4(0, 1);
		const Vec2i16 point = {
			static_cast<int16_t>(x),
			static_cast<int16_t>(y),
		};
		line_cmd.points(0, all) = last;
		line_cmd.points(1, all) = point;
		line(line_cmd);
		last = point;
	}
}

void
Renderer::rect(const RectCommand &cmd)
{
	const int16_t x = cmd.position(0, 0);
	const int16_t y = cmd.position(0, 1);
	const int16_t w = cmd.size(0, 0);
	const int16_t h = cmd.size(1, 0);
	const uint16_t r = cmd.rounding;
	const auto fill = cmd.fill;
	if (r == 0) {
		const auto hash = ruby()->hash_new_capa(cmd.fill ? 9 : 8);
		STASH_INT(hash, x, x);
		STASH_INT(hash, y, y);
		STASH_INT(hash, w, w);
		STASH_INT(hash, h, h);
		cmd.color.store_hash(ruby()->mrb(), hash);
		if (fill) {
			STASH_SYM(hash, path, solid);
			push_output(_symbols.sprites, hash);
		} else {
			push_output(_symbols.borders, hash);
		}
	}
	const int16_t xc = x + r;
	int16_t yc = y + r;
	const int16_t wc = w - 2 * r;
	const int16_t hc = h - 2 * r;
	const uint16_t th = cmd.line_thickness;
	const uint16_t hth = th / 2;
	if (fill) {
		RectCommand rect_cmd = {
			.position = {},
			.size = {},
			.color = cmd.color,
			.rounding = 0,
			.line_thickness = th,
			.fill = fill,
		};
		rect_cmd.position = Vec2i16 { x, yc };
		rect_cmd.size = Vec2i16 { w, hc };
		rect(rect_cmd);
		rect_cmd.position = Vec2i16 { xc, y };
		rect_cmd.size = Vec2i16 { wc, h };
		rect(rect_cmd);
	} else {
		LineCommand line_cmd = {
			.points = {},
			.color = cmd.color,
			.line_thickness = th,
		};
		line_cmd.points(0, all) = Vec2i16 { xc, y + hth };
		line_cmd.points(1, all) = Vec2i16 { xc + wc, y + hth };
		line(line_cmd);
		line_cmd.points(0, all) = Vec2i16 { x + w - hth, yc };
		line_cmd.points(1, all) = Vec2i16 { x + w - hth, yc + hc };
		line(line_cmd);
		line_cmd.points(0, all) = Vec2i16 { xc, y + h - th };
		line_cmd.points(1, all) = Vec2i16 { xc + wc, y + h - th };
		line(line_cmd);
		line_cmd.points(0, all) = Vec2i16 { x + hth, yc };
		line_cmd.points(1, all) = Vec2i16 { x + hth, yc + hc };
		line(line_cmd);
	}
	ArcCommand arc_cmd = {
		.center = {},
		.radius = r,
		.angles = {},
		.color = cmd.color,
		.line_thickness = th,
		.fill = fill,
	};
	if (fill) yc += 2;
	arc_cmd.center = Vec2i16 { xc, yc };
	arc_cmd.angles = Vec2f { 180.0f, 270.0f };
	arc(arc_cmd);
	arc_cmd.center = Vec2i16 { xc + wc, yc };
	arc_cmd.angles = Vec2f { 270.0f, 360.0f };
	arc(arc_cmd);
	arc_cmd.center = Vec2i16 { xc + wc, yc + hc };
	arc_cmd.angles = Vec2f { 0.0f, 90.0f };
	arc(arc_cmd);
	arc_cmd.center = Vec2i16 { xc, yc + hc };
	arc_cmd.angles = Vec2f { 90.0f, 180.0f };
	arc(arc_cmd);
}

void
Renderer::circle(const CircleCommand &cmd)
{
	const char *path = cmd.fill ? "sprites/circle/solid.png"
	                            : "sprites/circle/outline.png";
	const auto hash = ruby()->hash_new_capa(11);
	STASH_INT(hash, x, cmd.center(0, 0));
	STASH_INT(hash, y, cmd.center(0, 1));
	STASH_STR(hash, path, path);
	STASH_FLOAT(hash, anchor_x, 0.5f);
	STASH_FLOAT(hash, anchor_y, 0.5f);
	const int16_t w = cmd.size(0, 0);
	const int16_t h = cmd.size(1, 0);
	if (cmd.fill) {
		STASH_INT(hash, w, w);
		STASH_INT(hash, h, h);
		cmd.color.store_hash(ruby()->mrb(), hash);
		push_output(_symbols.sprites, hash);
	} else {
		for (int th = 0; th <= cmd.line_thickness; ++th) {
			STASH_INT(hash, w, w - 2 * th);
			STASH_INT(hash, h, h - 2 * th);
			cmd.color.store_hash(ruby()->mrb(), hash);
			push_output(_symbols.sprites, ruby()->hash_dup(hash));
		}
	}
}

void
Renderer::arc(const ArcCommand &cmd)
{
	float a = cmd.angles(0, 0);
	float b = cmd.angles(0, 1);
	a = std::fmod(a, 360.0f);
	b = std::fmod(b, 360.0f);
	if (a < 0) a += 360.0f;
	if (b < 0) b += 360.0f;
	const auto fill = cmd.fill || cmd.line_thickness >= cmd.radius;
	if (b - a >= 360.0f) {
		const auto d = cmd.radius * 2;
		circle(CircleCommand {
		    .center = cmd.center,
		    .size = { d, d },
		    .color = cmd.color,
		    .line_thickness = cmd.line_thickness,
		    .fill = fill,
		});
		return;
	}
	float sweep = std::ceilf(b - a);
	if (std::abs(sweep) < 1) return;
	if (sweep < 0) sweep += 360.0f;
	const float theta = a;
	assert(sweep <= 360.0f);
	char path[32] = {};
	const auto hash = ruby()->hash_new_capa(12);
	STASH_INT(hash, x, cmd.center(0, 0));
	STASH_INT(hash, y, cmd.center(0, 1));
	STASH_FLOAT(hash, anchor_x, 0.5f);
	STASH_FLOAT(hash, anchor_y, 0.5f);
	STASH_FLOAT(hash, angle, theta);
	cmd.color.store_hash(ruby()->mrb(), hash);
	const auto d = cmd.radius * 2;
	if (fill) {
		snprintf(path, sizeof(path), "sprites/wedge/wedge_%03d.png",
		    static_cast<int>(sweep));
		STASH_STR(hash, path, path);
		STASH_INT(hash, w, d);
		STASH_INT(hash, h, d);
		push_output(_symbols.sprites, hash);
	} else {
		snprintf(path, sizeof(path), "sprites/arc/arc_%03d.png",
		    static_cast<int>(sweep));
		STASH_STR(hash, path, path);
		for (int th = 0; th <= cmd.line_thickness; ++th) {
			STASH_INT(hash, w, d - 2 * th);
			STASH_INT(hash, h, d - 2 * th);
			push_output(_symbols.sprites, ruby()->hash_dup(hash));
		}
	}
}

void
Renderer::triangle(const TriangleCommand &cmd)
{
	const Vec2i16 p1 = cmd.points(0, all);
	const Vec2i16 p2 = cmd.points(1, all);
	const Vec2i16 p3 = cmd.points(2, all);
	const auto hash = ruby()->hash_new_capa(17);
	STASH_INT(hash, x, p1(0, 0));
	STASH_INT(hash, y, p1(0, 1));
	STASH_INT(hash, x2, p2(0, 0));
	STASH_INT(hash, y2, p2(0, 1));
	STASH_INT(hash, x3, p3(0, 0));
	STASH_INT(hash, y3, p3(0, 1));
	STASH_INT(hash, source_x, p1(0, 0));
	STASH_INT(hash, source_y, p1(0, 1));
	STASH_INT(hash, source_x2, p2(0, 0));
	STASH_INT(hash, source_y2, p2(0, 1));
	STASH_INT(hash, source_x3, p3(0, 0));
	STASH_INT(hash, source_y3, p3(0, 1));
	STASH_SYM(hash, path, solid);
	cmd.color.store_hash(ruby()->mrb(), hash);
	push_output(_symbols.sprites, hash);
}

void
Renderer::polygon(const PolygonCommand &cmd)
{
	/* TODO */
}

/* TODO: currently background color is ignored */
void
Renderer::text(const TextCommand &cmd)
{
	const auto hash = ruby()->hash_new_capa(9);
	STASH_INT(hash, x, cmd.position(0, 0));
	STASH_INT(hash, y, cmd.position(0, 1));
	STASH_INT(hash, size_enum, (cmd.height - 22) / 2);
	STASH_STR(hash, text, cmd.text.c_str());
	cmd.foreground.store_hash(ruby()->mrb(), hash);
	STASH_STR(hash, font, cmd.font->path().c_str());
	push_output(_symbols.sprites, hash);
}

void
Renderer::image(const ImageCommand &cmd)
{
	const auto hash = ruby()->hash_new_capa(9);
	STASH_INT(hash, x, cmd.position(0, 0));
	STASH_INT(hash, y, cmd.position(0, 1));
	STASH_INT(hash, w, cmd.size(0, 0));
	STASH_INT(hash, h, cmd.size(0, 1));
	STASH_STR(hash, path, cmd.image->label().c_str());
	if (cmd.color.has_value()) cmd.color->store_hash(ruby()->mrb(), hash);
	push_output(_symbols.sprites, hash);
}

void
Renderer::update_size()
{
	const auto grid = this->grid();
	const auto w = ruby()->funcall_argv(grid, _symbols.w_px);
	_width = mrb_fixnum(w);
	const auto h = ruby()->funcall_argv(grid, _symbols.h_px);
	_height = mrb_fixnum(h);
}

euler::util::Reference<euler::app::dragonruby::RubyState>
Renderer::ruby() const
{
	return _ruby.strengthen();
}
