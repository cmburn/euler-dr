/* SPDX-License-Identifier: ISC */

#include "euler/app/dragonruby/renderer.h"

#include <iomanip>

#include <mapbox/earcut.hpp>

#include "euler/app/dragonruby/ruby_state.h"
#include "euler/app/dragonruby/state.h"
#include "euler/graphics/renderer.h"

using euler::app::dragonruby::Renderer;

static constexpr auto all = Eigen::placeholders::all;

#define STASH_INT(HASH, KEY, VALUE)                                            \
	do {                                                                   \
		ruby()->hash_set(HASH, ruby()->symbol_value(_symbols.KEY),     \
		    ruby()->int_value(VALUE));                                 \
	} while (0)

#define STASH_STR(HASH, KEY, VALUE)                                            \
	do {                                                                   \
		ruby()->hash_set(HASH, ruby()->symbol_value(_symbols.KEY),     \
		    ruby()->str_new_cstr(VALUE));                              \
	} while (0)

#define STASH_FLOAT(HASH, KEY, VALUE)                                          \
	do {                                                                   \
		ruby()->hash_set(HASH, ruby()->symbol_value(_symbols.KEY),     \
		    ruby()->float_value(VALUE));                               \
	} while (0)

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
	if (cmd.line_thickness == 1) {
		thin_line(cmd);
		return;
	}
	Point p1 = cmd.points(0, all);
	Point p2 = cmd.points(1, all);
	const int16_t r = cmd.line_thickness / 2;
	PolygonCommand poly = {
		.points = PointSet(4, 2),
		.color = cmd.color,
		.line_thickness = 1,
		.fill = true,
	};
	const auto dx = p2.x() - p1.x();
	const auto dy = p2.y() - p1.y();
	const auto theta = std::atan2f(dy, dx);
	const auto offset_x = std::cosf(theta) * r;
	const auto offset_y = std::sinf(theta) * r;
	poly.points(0, all) = Point { p1.x() - offset_x, p1.y() - offset_y };
	poly.points(1, all) = Point { p1.x() + offset_x, p1.y() + offset_y };
	poly.points(2, all) = Point { p2.x() + offset_x, p2.y() + offset_y };
	poly.points(3, all) = Point { p2.x() - offset_x, p2.y() - offset_y };
	polygon(poly);
}

void
Renderer::curve(const CurveCommand &)
{
	/* TODO */
}

void
Renderer::rect(const RectCommand &cmd)
{
	const int16_t x = cmd.position(0, 0);
	const int16_t y = cmd.position(0, 1);
	const int16_t w = cmd.size(0, 0);
	const int16_t h = cmd.size(0, 1);
	if (cmd.rounding == 0) {
		PointSet points(4, 2);
		points(0, all) = Point { x, y };
		points(1, all) = Point { x + w, y };
		points(2, all) = Point { x + w, y + h };
		points(3, all) = Point { x, y + h };
		const PolygonCommand polygon_cmd = {
			.points = points,
			.color = cmd.color,
			.line_thickness = cmd.line_thickness,
		};
		polygon(polygon_cmd);
		return;
	}
	const int16_t xc = x + cmd.rounding;
	const int16_t yc = y + cmd.rounding;
	const auto wc = static_cast<int16_t>(w - 2 * cmd.rounding);
	const auto hc = static_cast<int16_t>(h - 2 * cmd.rounding);
	LineCommand line_cmd = {
		.points = {},
		.color = cmd.color,
		.line_thickness = cmd.line_thickness,
	};
	line_cmd.points(0, all) = Point { xc, y };
	line_cmd.points(1, all) = Point { xc + wc, y };
	line(line_cmd);
	line_cmd.points(0, all) = Point { x + w, yc };
	line_cmd.points(1, all) = Point { x + w, yc + hc };
	line(line_cmd);
	line_cmd.points(0, all) = Point { xc, y + h };
	line_cmd.points(1, all) = Point { xc + wc, y + h };
	line(line_cmd);
	line_cmd.points(0, all) = Point { x, yc };
	line_cmd.points(1, all) = Point { x, yc + hc };
	line(line_cmd);
	ArcCommand arc_cmd = {
		.center = {},
		.radius = cmd.rounding,
		.angles = {},
		.color = cmd.color,
		.line_thickness = cmd.line_thickness,
	};
	arc_cmd.center = { xc, yc };
	arc_cmd.angles = { 180, 270 };
	arc(arc_cmd);
	arc_cmd.center = { xc + wc, yc };
	arc_cmd.angles = { 270, 0 };
	arc(arc_cmd);
	arc_cmd.center = { xc + wc, yc + hc };
	arc_cmd.angles = { 0, 90 };
	arc(arc_cmd);
	arc_cmd.center = { xc, yc + hc };
	arc_cmd.angles = { 90, 180 };
	arc(arc_cmd);
}

static std::string
name_circle(const Renderer::CircleCommand &cmd)
{
	const int16_t w = cmd.size(0, 0);
	const int16_t h = cmd.size(0, 1);
	const auto col = cmd.color.hex_string();
	if (cmd.fill) {
		return std::format("euler_circle_{}x{}_{}", w, h, col);
	}
	return std::format("euler_circle_{}x{}_{}_{}px", w, h, col,
	    cmd.line_thickness);
}

void
Renderer::circle(const CircleCommand &cmd)
{
	auto name = name_circle(cmd);
	if (!_uploaded.contains(name)) {
		auto canvas = draw_circle(cmd, name);
		state()->upload_image(name.c_str(), canvas);
	}
	draw_uploaded(name, cmd.center);
}

void
Renderer::arc(const ArcCommand &)
{
	/* TODO */
}

void
Renderer::triangle(const TriangleCommand &)
{
	/* TODO */
}

void
Renderer::polygon(const PolygonCommand &cmd)
{
	const auto n = cmd.points.rows();
	if (n < 3) return;
	if (cmd.fill) {
		for (const auto &t : triangulate(cmd)) triangle(t);
		return;
	}
	if (n == 3) {
		const TriangleCommand triangle_cmd = {
			.points = cmd.points,
			.color = cmd.color,
			.line_thickness = cmd.line_thickness,
			.fill = false,
		};
		triangle(triangle_cmd);
		return;
	}
	LineCommand line_cmd = {
		.points = {},
		.color = cmd.color,
		.line_thickness = cmd.line_thickness,
	};
	Point prev = cmd.points(0, all);
	for (int i = 1; i < n; ++i) {
		const auto curr = cmd.points(i, all);
		line_cmd.points(0, all) = prev;
		line_cmd.points(1, all) = curr;
		line(line_cmd);
		prev = curr;
	}
	const auto curr = cmd.points(0, all);
	line_cmd.points(0, all) = prev;
	line_cmd.points(1, all) = curr;
	line(line_cmd);
}

void
Renderer::text(const TextCommand &)
{
	/* TODO */
}

void
Renderer::image(const ImageCommand &)
{
	/* TODO */
}

void
Renderer::draw_line(const std::function<bool(PointCommand &)> &fn)
{
	PointCommand cmd;
	while (fn(cmd)) {
		const auto r = cmd.radius;
		const auto x = cmd.position(0, 0);
		const auto y = cmd.position(0, 1);
		const auto color = cmd.color;
		const CircleCommand circ_cmd = {
			.center = { x, y },
			.size = { r, r },
			.color = color,
			.line_thickness = 1,
			.fill = true,
		};
		circle(circ_cmd);
	}
}

void
Renderer::draw_uploaded(const std::string &label, Point position)
{
	auto hash = ruby()->hash_new_capa(7);
	auto [w, h] = _uploaded.at(label)->dimensions();
	STASH_INT(hash, x, position.x());
	STASH_INT(hash, y, position.y());
	STASH_INT(hash, w, w);
	STASH_INT(hash, h, h);
	STASH_STR(hash, path, label.c_str());
	STASH_FLOAT(hash, anchor_x, 0.5f);
	STASH_FLOAT(hash, anchor_y, 0.5f);
	send_to_output(_symbols.sprites, hash);
}

mrb_value
Renderer::args() const
{
	return state().cast_to<State>()->args();
}

mrb_sym
Renderer::color_sym(const util::Color color)
{
	if (const auto c = _colors.find(color); c != _colors.end()) [[likely]]
		return c->second;
	return register_color(color);
}

std::vector<Renderer::TriangleCommand>
Renderer::triangulate(const PolygonCommand &cmd)
{
	using Triangle = Eigen::Matrix<int16_t, 3, 2>;
	const auto n = cmd.points.rows();
	std::vector<Triangle> points;
	points.reserve(n - 2);
	(void)points;
	return {};
}

void
Renderer::thin_line(const LineCommand &cmd)
{
	const auto hash = ruby()->hash_new_capa(8);
	STASH_INT(hash, x, cmd.points(0, 0));
	STASH_INT(hash, y, cmd.points(0, 1));
	STASH_INT(hash, x2, cmd.points(1, 0));
	STASH_INT(hash, y2, cmd.points(1, 1));
	STASH_INT(hash, r, cmd.color.red());
	STASH_INT(hash, g, cmd.color.green());
	STASH_INT(hash, b, cmd.color.blue());
	STASH_INT(hash, a, cmd.color.alpha());
	auto args = this->args();
	ruby()->funcall_argv(args, _symbols.outputs);
}

void
Renderer::filled_circle(const CircleCommand &cmd,
    const util::Reference<util::Image> &canvas)
{
	const int16_t w = cmd.size(0, 0);
	const int16_t h = cmd.size(0, 1);
	const int16_t a = w / 2;
	const int16_t b = h / 2;
	int16_t x = 0;
	int16_t y = b;
	const auto cx = cmd.center(0, 0);
	const auto cy = cmd.center(0, 1);
	const auto base_y = cy - b;
	auto dx = 2 * b * b * x;
	auto dy = 2 * a * a * y;
	auto d1 = (b * b) - (a * a * b) + (0.25 * a * a);

	std::vector filled(h + 1, false);
	auto spans = Eigen::Matrix<int16_t, Eigen::Dynamic, 2>(h + 1, 2);
	const auto update_span = [&](int16_t y, int16_t x1, int16_t x2) {
		assert(y >= base_y);
		const auto idx = y - base_y;
		assert(idx >= 0);
		assert(idx < spans.rows());
		assert(idx < static_cast<int>(filled.size()));
		if (filled[idx]) {
			spans(idx, 0) = std::min(spans(idx, 0), x1);
			spans(idx, 1) = std::max(spans(idx, 1), x2);
			return;
		}
		spans(idx, 0) = x1;
		spans(idx, 1) = x2;
		filled[idx] = true;
	};
	while (dx < dy) {
		update_span(cy + y, cx - x, cx + x);
		update_span(cy - y, cx - x, cx + x);
		++x;
		dx += 2 * b * b;
		if (d1 < 0) {
			d1 += dx + (b * b);
			continue;
		}
		--y;
		dy -= 2 * a * a;
		d1 += dx - dy + (b * b);
	}
	auto d2 = (b * b * (x + 0.5) * (x + 0.5)) + (a * a * (y - 1) * (y - 1))
	    - (a * a * b * b);
	while (y >= 0) {
		update_span(cy + y, cx - x, cx + x);
		update_span(cy - y, cx - x, cx + x);
		y -= 1;
		dy -= 2 * a * a;
		if (d2 > 0) {
			d2 += (a * a) - dy;
			continue;
		}
		++x;
		dx += 2 * b * b;
		d2 += dx - dy + (a * a);
	}
	for (auto i = 0; i < spans.rows(); ++i) {
		if (!filled[i]) continue;
		const auto y = base_y + i;
		const auto x1 = spans(i, 0);
		const auto x2 = spans(i, 1);
		for (auto x = x1; x <= x2; ++x)
			canvas->set_pixel(x, y, cmd.color);
	}
}

mrb_value
Renderer::grid()
{
	const auto cls = ruby()->module_get("Grid");
	return ruby()->obj_value(cls);
}

mrb_value
Renderer::render_target()
{
	return _render_target;
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

euler::util::Reference<euler::util::Image>
Renderer::draw_circle(const CircleCommand &cmd, const std::string &label)
{
	const int16_t w = cmd.size(0, 0);
	const int16_t h = cmd.size(0, 1);
	auto canvas
	    = state()->create_image(label.c_str(), w, h, util::COLOR_CLEAR);
	if (cmd.fill) {
		filled_circle(cmd, canvas);
		return canvas;
	}
	const int16_t a = w / 2;
	const int16_t b = h / 2;
	int16_t x = 0;
	int16_t y = b;
	const auto cx = cmd.center(0, 0);
	const auto cy = cmd.center(0, 1);
	auto dx = 2 * b * b * x;
	auto dy = 2 * a * a * y;
	auto d1 = (b * b) - (a * a * b) + (0.25 * a * a);

	CircleCommand pc = {
			// .position = { 0, 0 },
			// .radius = static_cast<int16_t>(cmd.line_thickness / 2),
			.center = { },
			.size = {
				static_cast<int16_t>(cmd.line_thickness),
				static_cast<int16_t>(cmd.line_thickness),
			},
			.color = cmd.color,
			.line_thickness = 1,
			.fill = true,
		};
	while (dx < dy) {
		pc.center = Point { cx + x, cy + y };
		filled_circle(pc, canvas);
		pc.center = Point { cx - x, cy + y };
		filled_circle(pc, canvas);
		pc.center = Point { cx + x, cy - y };
		filled_circle(pc, canvas);
		pc.center = Point { cx - x, cy - y };
		filled_circle(pc, canvas);
		if (d1 < 0) {
			d1 += dx + (b * b);
			continue;
		}
		--y;
		dy -= 2 * a * a;
		d1 += dx - dy + (b * b);
	}
	auto d2 = (b * b * (x + 0.5) * (x + 0.5)) + (a * a * (y - 1) * (y - 1))
	    - (a * a * b * b);
	while (y >= 0) {
		pc.center = Point { cx + x, cy + y };
		filled_circle(pc, canvas);
		pc.center = Point { cx - x, cy + y };
		filled_circle(pc, canvas);
		pc.center = Point { cx + x, cy - y };
		filled_circle(pc, canvas);
		pc.center = Point { cx - x, cy - y };
		filled_circle(pc, canvas);
		y -= 1;
		dy -= 2 * a * a;
		if (d2 > 0) {
			d2 += (a * a) - dy;
			continue;
		}
		++x;
		dx += 2 * b * b;
		d2 += dx - dy + (a * a);
	}
	return canvas;
}

void
Renderer::send_to_output(mrb_sym sym, mrb_value value)
{
	const auto val = ruby()->funcall_argv(render_target(), sym);
	ruby()->funcall_argv(val, _symbols.lshift, 1, &value);
}

mrb_sym
Renderer::register_color(const util::Color color)
{
	const auto str = std::format("euler_color_{}", color.hex_string());
	auto sym = ruby()->intern(str.data(), str.size());
	_colors.insert({ color, sym });
	return sym;
}

euler::util::Reference<euler::app::dragonruby::RubyState>
Renderer::ruby() const
{
	return _ruby.strengthen();
}
