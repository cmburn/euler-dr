/* SPDX-License-Identifier: ISC */

#include "euler/app/dragonruby/renderer.h"

#include "euler/app/dragonruby/state.h"

using euler::app::dragonruby::Renderer;

static constexpr auto all = Eigen::placeholders::all;

euler::util::Reference<euler::util::State>
Renderer::state() const
{
	return _state.strengthen().cast_to<util::State>();
}

int16_t
Renderer::width() const
{
	const auto val = ruby()->funcall_argv(_grid, _symbols.w_px);
	return mrb_fixnum(val);
}

int16_t
Renderer::height() const
{
	const auto val = ruby()->funcall_argv(_grid, _symbols.h_px);
	return mrb_fixnum(val);
}

void
Renderer::scissor(const ScissorCommand &cmd)
{
	(void)cmd;
	/* This is a no-op for DragonRuby*/
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
	Size delta = cmd.points(1, all) - cmd.points(0, all);
	const auto dx = delta(0, 0);
	const auto dy = delta(0, 1);
	const float d_theta = static_cast<float>(dy) / static_cast<float>(dx);
	const float theta = std::atan(d_theta);
	const float sin_theta = std::sin(theta);
	const float cos_theta = std::cos(theta);
	const float half_thickness
	    = static_cast<float>(cmd.line_thickness) / 2.0f;
	const float offset_x = half_thickness * cos_theta;
	const float offset_y = half_thickness * sin_theta;
	PointSet points(4, 2);
	points(0, all) = Point {
		static_cast<int16_t>(std::round(p1.x() - offset_x)),
		static_cast<int16_t>(std::round(p1.y() + offset_y)),
	};
	points(1, all) = Point {
		static_cast<int16_t>(std::round(p1.x() + offset_x)),
		static_cast<int16_t>(std::round(p1.y() - offset_y)),
	};
	points(2, all) = Point {
		static_cast<int16_t>(std::round(p2.x() + offset_x)),
		static_cast<int16_t>(std::round(p2.y() - offset_y)),
	};
	points(3, all) = Point {
		static_cast<int16_t>(std::round(p2.x() - offset_x)),
		static_cast<int16_t>(std::round(p2.y() + offset_y)),
	};
	const PolygonFilledCommand polygon_cmd = {
		.points = points,
		.color = cmd.color,
	};
	polygon_filled(polygon_cmd);
}

void
Renderer::curve(const CurveCommand &cmd)
{
	static constexpr uint16_t MAX_SEGMENTS
	    = std::numeric_limits<int16_t>::max();
	/* TODO: Currently this is ripped from nuklear_rawfb.h and isn't exactly
	 *       functional */
	assert(cmd.points.rows() == 4
	    && "Only 4-point curves are supported at the moment");
	Point p1 = cmd.points(0, all);
	Point p2 = cmd.points(1, all);
	Point p3 = cmd.points(2, all);
	Point p4 = cmd.points(3, all);
	Point last = p1;
	const int16_t segments
	    = std::clamp<int16_t>(cmd.segments, 1, MAX_SEGMENTS);
	const float t_step = 1.0f / static_cast<float>(segments);
	for (int16_t i_step = 1; i_step <= segments; ++i_step) {
		const float t = t_step * static_cast<float>(i_step);
		const float u = 1.0f - t;
		const float w1 = u * u * u;
		const float w2 = 3 * u * u * t;
		const float w3 = 3 * u * t * t;
		const float w4 = t * t * t;
		const float x
		    = w1 * p1.x() + w2 * p2.x() + w3 * p3.x() + w4 * p4.x();
		const float y
		    = w1 * p1.y() + w2 * p2.y() + w3 * p3.y() + w4 * p4.y();
		const auto pn = Point {
			static_cast<int16_t>(x),
			static_cast<int16_t>(y),
		};
		auto line_points = PointSet(2, 2);
		line_points(0, all) = last;
		line_points(1, all) = pn;
		LineCommand line_cmd = {
			.points = line_points,
			.line_thickness = cmd.line_thickness,
			.color = cmd.color,
		};
		line(line_cmd);
		last = pn;
	}
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
			.line_thickness = cmd.line_thickness,
			.points = points,
			.color = cmd.color,
		};
		polygon(polygon_cmd);
		return;
	}
	const int16_t xc = x + cmd.rounding;
	const int16_t yc = y + cmd.rounding;
	const int16_t wc = static_cast<int16_t>(w - 2 * cmd.rounding);
	const int16_t hc = static_cast<int16_t>(h - 2 * cmd.rounding);
	LineCommand line_cmd = {
		.points = {},
		.line_thickness = cmd.line_thickness,
		.color = cmd.color,
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
		.line_thickness = cmd.line_thickness,
		.angles = {},
		.color = cmd.color,
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

void
Renderer::rect_filled(const RectFilledCommand &cmd)
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
		const PolygonFilledCommand polygon_cmd = {
			.points = points,
			.color = cmd.color,
		};
	}
	const int16_t xc = x + cmd.rounding;
	const int16_t yc = y + cmd.rounding;
	const int16_t wc = static_cast<int16_t>(w - 2 * cmd.rounding);
	const int16_t hc = static_cast<int16_t>(h - 2 * cmd.rounding);
	PointSet points(4, 2);
	points(0, all) = Point { xc, yc };
	points(1, all) = Point { xc + wc, yc };
	points(2, all) = Point { xc + w, yc + hc };
	points(3, all) = Point { xc, yc + hc };
	const PolygonFilledCommand polygon_cmd = {
		.points = points,
		.color = cmd.color,
	};
	polygon_filled(polygon_cmd);
}

void
Renderer::rect_multi_color(const RectMultiColorCommand &cmd)
{
	std::vector<util::Color> edge_buf;
	const int16_t w = cmd.size(0, 0);
	const int16_t h = cmd.size(0, 1);
	const int16_t x = cmd.position(0, 0);
	const int16_t y = cmd.position(0, 1);
	const auto tl = cmd.left;
	const auto tr = cmd.top;
	const auto br = cmd.right;
	const auto bl = cmd.bottom;

	edge_buf.reserve((2 * w) + 2 * h);
	util::Color *edge_t = edge_buf.data();
	util::Color *edge_b = edge_buf.data() + w;
	util::Color *edge_l = edge_buf.data() + (w * 2);
	util::Color *edge_r = edge_buf.data() + (w * 2) + h;
	for (int16_t i = 0; i < w; ++i) {
		const auto factor
		    = static_cast<float>(i) / static_cast<float>(w - 1);
		edge_t[i] = tr.gradient(tl, factor);
		edge_b[i] = br.gradient(bl, factor);
	}
	for (int16_t i = 0; i < h; ++i) {
		const auto factor
		    = static_cast<float>(i) / static_cast<float>(h - 1);
		edge_l[i] = bl.gradient(tl, factor);
		edge_r[i] = br.gradient(tr, factor);
	}
	for (int16_t i = 0; i < h; ++i) {
		for (int16_t j = 0; j < w; ++j) {
			const auto xi = x + j;
			const auto yi = y + i;
			if (i == 0) {
				blend_pixel(xi, yi, edge_t[j]);
				continue;
			}
			if (j == 0) {
				blend_pixel(xi, yi, edge_b[j]);
				continue;
			}
			const auto factor
			    = static_cast<float>(j) / static_cast<float>(w - 1);
			const auto color
			    = edge_r[i].gradient(edge_l[i], factor);
			blend_pixel(xi, yi, color);
		}
	}
}

void
Renderer::circle(const CircleCommand &cmd)
{

}

void
Renderer::circle_filled(const CircleFilledCommand &cmd)
{
}

void
Renderer::arc(const ArcCommand &cmd)
{
}

void
Renderer::arc_filled(const ArcFilledCommand &cmd)
{
}

void
Renderer::triangle(const TriangleCommand &cmd)
{
}

void
Renderer::triangle_filled(const TriangleFilledCommand &cmd)
{
}

void
Renderer::polygon(const PolygonCommand &cmd)
{
}

void
Renderer::polygon_filled(const PolygonFilledCommand &cmd)
{
}

void
Renderer::polyline(const PolylineCommand &cmd)
{
}

void
Renderer::text(const TextCommand &cmd)
{
}

void
Renderer::image(const ImageCommand &cmd)
{
}

mrb_value
Renderer::args() const
{
	return state().cast_to<State>()->args();
}

void
Renderer::thin_line(const LineCommand &cmd) const
{
	/* TODO */
}

void
Renderer::blend_pixel(const int16_t x, const int16_t y, const util::Color color)
{
	if (color.alpha() == 0) return;
	const auto c = _canvas->pixel(x, y).blend(color);
	set_pixel(x, y, c);
}
