/* SPDX-License-Identifier: ISC */

#ifndef EULER_GRAPHICS_CONTEXT_H
#define EULER_GRAPHICS_CONTEXT_H

#include <vector>

#include <Eigen/Eigen>

#include "euler/math/math.h"
#include "euler/util/color.h"
#include "euler/util/font.h"
#include "euler/util/object.h"
#include "euler/util/state.h"
#include "euler/graphics/user_interface.h"

#ifndef DEFAULT_CURVE_SEGMENTS
#define DEFAULT_CURVE_SEGMENTS 22
#endif

namespace euler::graphics {
class Target : public util::Object {
	BIND_MRUBY("Euler::Graphics::Target", Target, graphics.target);
public:
	static constexpr int16_t DEFAULT_SEGMENTS = DEFAULT_CURVE_SEGMENTS;
	/* TODO: we do not currently use a render context, but we absolutely
	 *       need to. */
	[[nodiscard]] virtual util::Reference<util::State> state() const = 0;
	// [[nodiscard]] virtual int16_t width() const = 0;
	// [[nodiscard]] virtual int16_t height() const = 0;
	using Vec2i16 = math::Vec2<int16_t>;
	using Vec2f = math::Vec2<float>;

	using Line = Eigen::Matrix<int16_t, 2, 2>;
	using PointSet = Eigen::Matrix<int16_t, Eigen::Dynamic, 2>;
	struct ScissorCommand {
		Vec2i16 position;
		Vec2i16 size;
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
		Vec2i16 position;
		Vec2i16 size;
		util::Color color;
		uint16_t rounding = 0;
		uint16_t line_thickness = 1;
		bool fill = false;
	};
	struct CircleCommand {
		Vec2i16 center;
		Vec2i16 size;
		util::Color color;
		uint16_t line_thickness = 1;
		bool fill = false;
	};
	struct ArcCommand {
		Vec2i16 center;
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
		Vec2i16 position;
		Vec2i16 size;
		float height;
		std::string text;
	};
	struct ImageCommand {
		Vec2i16 position;
		Vec2i16 size;
		util::Reference<util::Image> image;
		std::optional<util::Color> color;
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

	virtual void render(mrb_value target,
		std::function<void(const util::Reference<UserInterface> &)> ui) = 0;

protected:
	void frame();




private:
	util::Reference<UserInterface> _ui;

};
} /* namespace euler::graphics */

#endif /* EULER_GRAPHICS_CONTEXT_H */
