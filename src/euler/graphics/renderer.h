/* SPDX-License-Identifier: ISC */

#ifndef EULER_GRAPHICS_CONTEXT_H
#define EULER_GRAPHICS_CONTEXT_H

#include <vector>

#include <Eigen/Eigen>

#include "euler/util/color.h"
#include "euler/util/object.h"
#include "euler/util/state.h"

#ifndef DEFAULT_CURVE_SEGMENTS
#define DEFAULT_CURVE_SEGMENTS 22
#endif

namespace euler::graphics {
class Renderer : public util::Object {
	BIND_MRUBY("Euler::Graphics::Renderer", Renderer, graphics.renderer);
public:
	static constexpr int16_t DEFAULT_SEGMENTS = DEFAULT_CURVE_SEGMENTS;
	/* TODO: we do not currently use a render context, but we absolutely
	 *       need to. */
	[[nodiscard]] virtual util::Reference<util::State> state() const = 0;
	// [[nodiscard]] virtual int16_t width() const = 0;
	// [[nodiscard]] virtual int16_t height() const = 0;
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
		util::Color color;
		uint16_t line_thickness = 1;
	};
	struct CurveCommand {
		Eigen::Matrix<int16_t, 4, 2> points;
		util::Color color;
		int16_t segments = DEFAULT_SEGMENTS;
		uint16_t line_thickness = 1;
	};
	struct RectCommand {
		Point position;
		Size size;
		util::Color color;
		uint16_t rounding = 0;
		uint16_t line_thickness = 1;
		bool fill = false;
	};
	struct CircleCommand {
		Point center;
		Size size;
		util::Color color;
		uint16_t line_thickness = 1;
		bool fill = false;
	};
	struct ArcCommand {
		Point center;
		uint16_t radius;
		Eigen::Matrix<float, 1, 2> angles;
		util::Color color;
		uint16_t line_thickness = 1;
		bool fill = false;
	};
	struct TriangleCommand {
		Eigen::Matrix<int16_t, 3, 2> points;
		util::Color color;
		uint16_t line_thickness = 1;
		bool fill = false;
	};
	struct PolygonCommand {
		PointSet points;
		util::Color color;
		uint16_t line_thickness = 1;
		bool fill = false;
	};
	struct TextCommand {
		util::Reference<util::Font> font;
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
	virtual void circle(const CircleCommand &cmd) = 0;
	virtual void arc(const ArcCommand &cmd) = 0;
	virtual void triangle(const TriangleCommand &cmd) = 0;
	virtual void polygon(const PolygonCommand &cmd) = 0;
	virtual void text(const TextCommand &cmd) = 0;
	virtual void image(const ImageCommand &cmd) = 0;
};
} /* namespace euler::graphics */

#endif /* EULER_GRAPHICS_CONTEXT_H */
