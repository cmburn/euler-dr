/* SPDX-License-Identifier: ISC */

#ifndef EULER_GRAPHICS_CONTEXT_H
#define EULER_GRAPHICS_CONTEXT_H

#include <vector>

#include <Eigen/Eigen>

#include "euler/util/color.h"
#include "euler/util/object.h"
#include "euler/util/state.h"

namespace euler::graphics {
class Renderer : public util::Object {
	BIND_MRUBY("Euler::Graphics::Renderer", Renderer, graphics.renderer);
public:
	/* TODO: we do not currently use a render context, but we absolutely
	 *       need to. */
	[[nodiscard]] virtual util::Reference<util::State> state() const = 0;
	[[nodiscard]] virtual int16_t width() const = 0;
	[[nodiscard]] virtual int16_t height() const = 0;
	using Point = Eigen::Matrix<int16_t, 1, 2>;
	using Size = Point;
	using Line = Eigen::Matrix<int16_t, 2, 2>;
	using PointSet = Eigen::Matrix<int16_t, Eigen::Dynamic, 2>;
	struct ScissorCommand {
		Point position;
		Size size;
	};
	struct LineCommand {
		Line points;
		uint16_t line_thickness;
		util::Color color;
	};
	struct CurveCommand {
		uint16_t line_thickness;
		Eigen::Matrix<int16_t, 4, 2> points;
		util::Color color;
		int16_t segments;
	};
	struct RectCommand {
		uint16_t rounding;
		uint16_t line_thickness;
		Point position;
		Size size;
		util::Color color;
	};
	struct RectFilledCommand {
		uint16_t rounding;
		Point position;
		Size size;
		util::Color color;
	};
	struct RectMultiColorCommand {
		Point position;
		Size size;
		util::Color left;
		util::Color top;
		util::Color bottom;
		util::Color right;
	};
	struct CircleCommand {
		Point center;
		uint16_t line_thickness;
		Size size;
		util::Color color;
	};
	struct CircleFilledCommand {
		Point center;
		Size size;
		util::Color color;
	};
	struct ArcCommand {
		Point center;
		uint16_t radius;
		uint16_t line_thickness;
		Eigen::Matrix<float, 1, 2> angles;
		util::Color color;
	};
	struct ArcFilledCommand {
		Point center;
		uint16_t radius;
		Eigen::Matrix<float, 1, 2> angles;
		util::Color color;
	};
	struct TriangleCommand {
		uint16_t line_thickness;
		Eigen::Matrix<int16_t, 3, 2> points;
		util::Color color;
	};
	struct TriangleFilledCommand {
		Eigen::Matrix<int16_t, 3, 2> points;
		util::Color color;
	};
	struct PolygonCommand {
		uint16_t line_thickness;
		PointSet points;
		util::Color color;
	};
	struct PolygonFilledCommand {
		PointSet points;
		util::Color color;
	};
	struct PolylineCommand {
		uint16_t line_thickness;
		PointSet points;
		util::Color color;
	};
	struct TextCommand {
		// util::Reference<Font> font;
		util::Color background;
		util::Color foreground;
		Point position;
		Size size;
		float height;
		std::string text;
	};
	struct ImageCommand {
		Point position;
		Size size;
		util::Reference<util::Image> image;
		util::Color color;
	};
	virtual void scissor(const ScissorCommand &cmd) = 0;
	virtual void line(const LineCommand &cmd) = 0;
	virtual void curve(const CurveCommand &cmd) = 0;
	virtual void rect(const RectCommand &cmd) = 0;
	virtual void rect_filled(const RectFilledCommand &cmd) = 0;
	virtual void rect_multi_color(const RectMultiColorCommand &cmd) = 0;
	virtual void circle(const CircleCommand &cmd) = 0;
	virtual void circle_filled(const CircleFilledCommand &cmd) = 0;
	virtual void arc(const ArcCommand &cmd) = 0;
	virtual void arc_filled(const ArcFilledCommand &cmd) = 0;
	virtual void triangle(const TriangleCommand &cmd) = 0;
	virtual void triangle_filled(const TriangleFilledCommand &cmd) = 0;
	virtual void polygon(const PolygonCommand &cmd) = 0;
	virtual void polygon_filled(const PolygonFilledCommand &cmd) = 0;
	virtual void polyline(const PolylineCommand &cmd) = 0;
	virtual void text(const TextCommand &cmd) = 0;
	virtual void image(const ImageCommand &cmd) = 0;
};
} /* namespace euler::graphics */

#endif /* EULER_GRAPHICS_CONTEXT_H */
