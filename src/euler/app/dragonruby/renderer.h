/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_DRAGONRUBY_RENDERER_H
#define EULER_APP_DRAGONRUBY_RENDERER_H

#include "euler/app/dragonruby/ruby_state.h"
#include "euler/graphics/renderer.h"
#include "euler/util/image.h"
#include "euler/util/object.h"

namespace euler::app::dragonruby {
class Renderer final : public graphics::Renderer {
public:
	[[nodiscard]] util::Reference<util::State> state() const override;
	[[nodiscard]] int16_t width() const override;
	[[nodiscard]] int16_t height() const override;
	void scissor(const ScissorCommand &cmd) override;
	//
	void line(const LineCommand &cmd) override;
	void curve(const CurveCommand &cmd) override;
	//
	void rect(const RectCommand &cmd) override;
	//
	void rect_filled(const RectFilledCommand &cmd) override;
	void rect_multi_color(const RectMultiColorCommand &cmd) override;
	void circle(const CircleCommand &cmd) override;
	void circle_filled(const CircleFilledCommand &cmd) override;
	void arc(const ArcCommand &cmd) override;
	void arc_filled(const ArcFilledCommand &cmd) override;
	void triangle(const TriangleCommand &cmd) override;
	//
	void triangle_filled(const TriangleFilledCommand &cmd) override;
	void polygon(const PolygonCommand &cmd) override;
	void polygon_filled(const PolygonFilledCommand &cmd) override;
	void polyline(const PolylineCommand &cmd) override;
	//
	void text(const TextCommand &cmd) override;
	//
	void image(const ImageCommand &cmd) override;

	std::vector<TriangleFilledCommand> tessellate(
	    const PolygonCommand &cmd);

private:
	mrb_value args() const;

	void thin_line(const LineCommand &cmd) const;
	void blend_pixel(int16_t x, int16_t y, util::Color color);
	bool is_render_target_ready(const std::string &) const;
	bool is_render_target_queued(const std::string &) const;
	void merge_render_target(mrb_value hash);
	static constexpr int16_t ZERO_I16 = 0;

	template <int Rows>
	void
	clamp(Eigen::Matrix<int16_t, Rows, 2> &points) const
	{
		const auto w = width();
		const auto h = height();
		for (auto i = 0; i < points.rows(); ++i) {
			auto x = points(i, 0);
			auto y = points(i, 1);
			points(i, 0) = std::clamp(x, ZERO_I16, w);
			points(i, 1) = std::clamp(y, ZERO_I16, h);
		}
	}

	util::Reference<RubyState> ruby() const;
	util::WeakReference<RubyState> _ruby;
	util::WeakReference<State> _state;
	struct {
		mrb_sym h_px;
		mrb_sym w_px;
		mrb_sym x;
		mrb_sym y;
		mrb_sym x2;
		mrb_sym y2;
		mrb_sym r;
		mrb_sym g;
		mrb_sym b;
		mrb_sym a;
	} _symbols;
	mrb_value _grid;
	int16_t _curve_resolution = 22;
};
} /* namespace euler::app::dragonruby */

#endif /* EULER_APP_DRAGONRUBY_RENDERER_H */
