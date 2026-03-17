/* SPDX-License-Identifier: ISC */

#include "euler/gui/context.h"
// #include "../../../cmake-build-debug/_deps/nuklear-src/nuklear.h"

#include "euler/gui/internal.h"
#include "euler/gui/nuklear.h"
#include "euler/gui/widget.h"
#include "euler/util/ext.h"
#include "euler/util/window.h"

using euler::gui::Context;

static inline bool
in_rect(const struct nk_rect &rect, const int16_t x, const int16_t y)
{
	return y < rect.h && y >= rect.y && x < rect.w && x >= rect.x;
}

mrb_value
Context::display(const util::Reference<Widget> &widget)
{
	if (widget == nullptr) return mrb_nil_value();
	auto mrb = state()->mrb()->mrb();
	if (state()->phase() != util::State::Phase::Draw) {
		// mrb_raise(mrb, mrb->eStandardError_class,
		//     "Widgets can only be displayed during State#draw");
		state()->mrb()->raise(
		    state()->mrb()->mrb()->eStandardError_class,
		    "Widgets can only be displayed during State#draw");
	}
	const auto id = widget->id();
	if (!_widget_callbacks.contains(widget->id())) {
		state()->mrb()->raise(
		    state()->mrb()->mrb()->eStandardError_class,
		    "Widget was not created by this Window, or has already "
		    "been released.");
	}
	const auto fn = _widget_callbacks.at(id);
	if (std::holds_alternative<NativeCallback>(fn)) {
		widget->call(std::get<NativeCallback>(fn));
		return mrb_nil_value();
	}
	auto out = mrb_nil_value();
	widget->call([&](util::Reference<Widget> &w) -> void {
		const auto block = std::get<mrb_value>(fn);
		auto self_value
		    = w.wrap(mrb, state()->modules().gui.widget, &Widget::TYPE);
		state()->mrb()->funcall_argv(block, EULER_SYM(call), 1,
		    &self_value);
	});
	return out;
}

void
Context::release_widget(const Widget::ID id)
{
	const auto fn = _widget_callbacks.find(id);
	if (fn == _widget_callbacks.end()) return;
	if (std::holds_alternative<mrb_value>(fn->second))
		state()->mrb()->gc_unregister(std::get<mrb_value>(fn->second));
	_widget_callbacks.erase(id);
	_available_widget_ids.push_back(id);
}

const nk_context *
Context::context() const
{
	return &_context;
}

nk_context *
Context::context()
{
	return &_context;
}

euler::util::Reference<euler::util::State>
Context::state() const
{
	return _state.strengthen();
}

void
Context::render()
{
	const nk_command *cmd = nullptr;
	if (enable_clear()) clear();
	nk_foreach (cmd, &_context) {
		switch (cmd->type) {
		case NK_COMMAND_NOP: break;
		case NK_COMMAND_SCISSOR: {
			const auto s
			    = reinterpret_cast<const nk_command_scissor *>(cmd);
			scissor(s->x, s->y, s->w, s->h);
			break;
		}
		case NK_COMMAND_LINE: {
			const auto l
			    = reinterpret_cast<const nk_command_line *>(cmd);
			stroke_line(l->begin.x, l->begin.y, l->end.x, l->end.y,
			    l->line_thickness, Color::from_nk(l->color));
			break;
		}
		case NK_COMMAND_CURVE: {
			// const auto c
			//     = reinterpret_cast<const nk_command_curve
			//     *>(cmd);
			/* TODO */
			break;
		}
		case NK_COMMAND_RECT: {
			const auto r
			    = reinterpret_cast<const nk_command_rect *>(cmd);
			stroke_rect(r->x, r->y, r->w, r->h, r->r,
			    r->line_thickness, Color::from_nk(r->color));
			break;
		}
		case NK_COMMAND_RECT_FILLED: {
			const auto r
			    = reinterpret_cast<const nk_command_rect_filled *>(
				cmd);
			fill_rect(r->x, r->y, r->w, r->h, r->rounding,
			    Color::from_nk(r->color));
			break;
		}
		case NK_COMMAND_RECT_MULTI_COLOR: {
			const auto r = reinterpret_cast<
			    const nk_command_rect_multi_color *>(cmd);
			draw_rect_multi_color(r->x, r->y, r->w, r->h,
			    Color::from_nk(r->left), Color::from_nk(r->top),
			    Color::from_nk(r->right), Color::from_nk(r->bottom));
			break;
		}
		case NK_COMMAND_CIRCLE: {
			const auto c
			    = reinterpret_cast<const nk_command_circle *>(cmd);
			break;
		}
		case NK_COMMAND_CIRCLE_FILLED: {
			const auto c = reinterpret_cast<
			    const nk_command_circle_filled *>(cmd);
			break;
		}
		case NK_COMMAND_ARC: {
			const auto a
			    = reinterpret_cast<const nk_command_arc *>(cmd);
			break;
		}
		case NK_COMMAND_ARC_FILLED: {
			const auto a
			    = reinterpret_cast<const nk_command_arc_filled *>(
				cmd);
			break;
		}
		case NK_COMMAND_TRIANGLE: {
			const auto t
			    = reinterpret_cast<const nk_command_triangle *>(
				cmd);
			break;
		}
		case NK_COMMAND_TRIANGLE_FILLED: {
			const auto t = reinterpret_cast<
			    const nk_command_triangle_filled *>(cmd);
			break;
		}
		case NK_COMMAND_POLYGON: {
			const auto p
			    = reinterpret_cast<const nk_command_polygon *>(cmd);
			break;
		}
		case NK_COMMAND_POLYGON_FILLED: {
			const auto p = reinterpret_cast<
			    const nk_command_polygon_filled *>(cmd);
			break;
		}
		case NK_COMMAND_POLYLINE: {
			const auto p
			    = reinterpret_cast<const nk_command_polyline *>(
				cmd);
			break;
		}
		case NK_COMMAND_TEXT: {
			const auto t
			    = reinterpret_cast<const nk_command_text *>(cmd);
			break;
		}
		case NK_COMMAND_IMAGE: {
			const auto i
			    = reinterpret_cast<const nk_command_image *>(cmd);
			break;
		}
		default: break;
		}
	};
	nk_clear(&_context);
}

uint32_t
Context::width() const
{
	return state()->window()->width();
}

uint32_t
Context::height() const
{
	return state()->window()->height();
}

bool
Context::is_cardinal(int16_t s)
{
	switch (s) {
	case 0:
	case 90:
	case 180:
	case 270: return true;
	default: return false;
	}
}
void
Context::set_arc_point(const int16_t x0, const int16_t y0, const int16_t x,
    const int16_t y, const int16_t s, const util::Color color)
{
	switch (s) {
	case 0: set_pixel(x0 + x, y0 - y, color); break;
	case 90: set_pixel(x0 - x, y0 - y, color); break;
	case 180: set_pixel(x0 - x, y0 + y, color); break;
	case 270: set_pixel(x0 + x, y0 + y, color); break;
	default: std::unreachable();
	}
}
void
Context::set_pixel(const int16_t x, const int16_t y, const util::Color color)
{
	if (in_rect(_scissors, x, y)) _image->set_pixel(x, y, color);
}

void
Context::line_horizontal(const int16_t x0, const int16_t y, const int16_t x1,
    const util::Color color)
{
	/* TODO: this function is called frequently, should optimize */
	for (int16_t i = x0; i < x1; i++) set_pixel(i, y, color);
}

void
Context::scissor(const float x, const float y, const float w, const float h)
{
	const float width = this->width() * 1.0f;
	const float height = this->height() * 1.0f;

	_scissors.x = std::min(std::max(x, 0.0f), width);
	_scissors.y = std::min(std::max(y, 0.0f), height);
	_scissors.w = std::min(std::max(w + x, 0.0f), width);
	_scissors.h = std::min(std::max(h + y, 0.0f), height);
}

void
Context::stroke_line(int16_t x0, int16_t y0, int16_t x1, const int16_t y1,
    int16_t line_thickness, const util::Color color)
{
	int32_t dy = y1 - y0;
	int32_t dx = x1 - x0;
	(void)line_thickness;
	/* fast path */
	if (dy == 0) {
		if (dx == 0 || y0 >= _scissors.h || y0 < _scissors.y) return;
		if (dx < 0) std::swap(x0, x1);
		x1 = std::min(_scissors.w, x1 * 1.0f);
		x0 = std::min(_scissors.w, x0 * 1.0f);
		x1 = std::max(_scissors.x, x1 * 1.0f);
		x0 = std::max(_scissors.x, x0 * 1.0f);
		line_horizontal(x0, y0, x1, color);
		return;
	}
	int32_t step_y = 1, step_x = 1;
	if (dy < 0) {
		dy = -dy;
		step_y = -1;
	}
	if (dx < 0) {
		dx = -dx;
		step_x = -1;
	}
	dy <<= 1;
	dx <<= 1;
	set_pixel(x0, y0, color);
	if (dx > dy) {
		int32_t fraction = dy - (dx >> 1);
		while (x0 != x1) {
			if (fraction >= 0) {
				y0 += step_y;
				fraction -= dx;
			}
			x0 += step_x;
			fraction += dy;
			set_pixel(x0, y0, color);
		}
	} else {
		int32_t fraction = dx - (dy >> 1);
		while (y0 != y1) {
			if (fraction >= 0) {
				x0 += step_x;
				fraction -= dy;
			}
			y0 += step_y;
			fraction += dx;
			set_pixel(x0, y0, color);
		}
	}
}

void
Context::fill_polygon(const std::vector<Point> &points, const util::Color color)
{
	const int32_t count = points.size();
	if (count == 0) return;
	int16_t left = std::numeric_limits<int16_t>::max();
	int16_t right = 0;
	int16_t top = std::numeric_limits<int16_t>::max();
	int16_t bottom = 0;
	std::vector<int32_t> node_x;
	node_x.reserve(count);
	for (const auto &[x, y] : points) {
		left = std::min(left, x);
		right = std::max(right, x);
		top = std::min(top, y);
		bottom = std::max(bottom, y);
	}
	++bottom;
	++right;
	for (int32_t pixel_y = top; pixel_y < bottom; ++pixel_y) {
		auto nodes = 0;
		const auto j = count - 1;
		for (auto i = 0; i < count; ++i) {
			const float x_i = points[i].first;
			const float y_i = points[i].second;
			const float x_j = points[j].first;
			const float y_j = points[j].second;
			if ((y_i >= pixel_y || y_j < pixel_y)
			    && (y_j >= pixel_y || y_i < pixel_y)) {
				continue;
			}
			const auto val
			    = x_i + (pixel_y - y_i) * (x_j - x_i) / (y_j - y_i);
			node_x.push_back(static_cast<int32_t>(val));
			++nodes;
		}
		std::ranges::sort(node_x);
		for (int32_t i = 0; i < nodes; i += 2) {
			if (node_x[i] >= right) break;
			if (node_x[i + 1] <= left) continue;
			if (node_x[i] < left) node_x[i] = left;
			if (node_x[i + 1] > right) node_x[i + 1] = right;
			for (auto px = node_x[i]; px < node_x[i + 1]; ++px)
				set_pixel(px, pixel_y, color);
		}
	}
}

void
Context::stroke_arc(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t s,
    const int16_t line_thickness, const util::Color color)
{
	const int a2 = w * w / 4;
	const int b2 = h * h / 4;
	const int fa2 = 4 * a2;
	const int fb2 = 4 * b2;
	s = (s + 360) % 360;
	(void)line_thickness;
	if (!is_cardinal(s)) return;
	if (w < 1 || h < 1) return;
	h = (h + 1) / 2;
	w = (w + 1) / 2;
	x0 += w;
	y0 += h;
	for (int16_t x = 0, y = h, sigma = 2 * b2 + a2 * (1 - 2 * h);
	    x * b2 <= y * a2; ++x) {
		set_arc_point(x0, y0, x, y, s, color);
		if (sigma >= 0) {
			sigma += fa2 * (1 - y);
			--y;
		}
		sigma += b2 * ((4 * x) + 6);
	}
	for (int16_t x = w, y = 0, sigma = 2 * a2 + b2 * (1 - 2 * w);
	    a2 * y <= b2 * x; ++y) {
		set_arc_point(x0, y0, x, y, s, color);
		if (sigma >= 0) {
			sigma += fb2 * (1 - x);
			--x;
		}
		sigma += a2 * ((4 * y) + 6);
	}
}

void
Context::fill_arc(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t s,
    util::Color color)
{
	const int a2 = w * w / 4;
	const int b2 = h * h / 4;
	const int fa2 = 4 * a2;
	const int fb2 = 4 * b2;
	std::vector<Point> pnts;
	pnts.reserve(3);
	for (int i = 0; i < 3; ++i) pnts.emplace_back(0, 0);
	if (w < 1 || h < 1) return;
	if (!is_cardinal(s)) return;
	h = (h + 1) / 2;
	w = (w + 1) / 2;
	x0 += w;
	y0 += h;
	for (int16_t x = 0, y = h, sigma = 2 * b2 + a2 * (1 - 2 * h);
	    x * b2 <= y * a2; ++x) {
		switch (s) {
		case 0: pnts[0] = { x0 + x, y0 - y }; break;
		case 90: pnts[0] = { x0 - x, y0 - y }; break;
		case 180: pnts[0] = { x0 - x, y0 + y }; break;
		case 270:
			pnts[0] = { x0 + x, y0 + y };
			break;
			/* ReSharper disable once CppDFAUnreachableCode */
		default: std::unreachable();
		}
		fill_polygon(pnts, color);
		pnts[2] = pnts[1];
		if (sigma >= 0) {
			sigma += fa2 * (1 - y);
			--y;
		}
		sigma += b2 * ((4 * x) + 6);
	}

	for (int16_t x = w, y = 0, sigma = 2 * a2 + b2 * (1 - 2 * w);
	    a2 * y <= b2 * x; ++y) {
		switch (s) {
		case 0: pnts[0] = { x0 + x, y0 - y }; break;
		case 90: pnts[0] = { x0 - x, y0 - y }; break;
		case 180: pnts[0] = { x0 - x, y0 + y }; break;
		case 270:
			pnts[0] = { x0 + x, y0 + y };
			break;
			/* ReSharper disable once CppDFAUnreachableCode */
		default: std::unreachable();
		}
		fill_polygon(pnts, color);
		pnts[2] = pnts[1];
		if (sigma >= 0) {
			sigma += fb2 * (1 - x);
			--x;
		}
		sigma += a2 * ((4 * y) + 6);
	}
}

void
Context::stroke_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,
    int16_t line_thickness, util::Color color)
{
	if (r == 0) {
		stroke_line(x, y, x + w, y, line_thickness, color);
		stroke_line(x, y + h, x + w, y + h, line_thickness, color);
		stroke_line(x, y, x, y + h, line_thickness, color);
		stroke_line(x + w, y, x + w, y + h, line_thickness, color);
		return;
	}
	const int16_t xc = x + r;
	const int16_t yc = y + r;
	const int16_t wc = static_cast<int16_t>(w - 2 * r);
	const int16_t hc = static_cast<int16_t>(h - 2 * r);

	stroke_line(xc, y, xc + wc, y, line_thickness, color);
	stroke_line(x + w, yc, x + w, yc + hc, line_thickness, color);
	stroke_line(xc, y + h, xc + wc, y + h, line_thickness, color);
	stroke_line(x, yc, x, yc + hc, line_thickness, color);
	stroke_arc(xc + wc - r, y, r * 2, r * 2, 0, line_thickness, color);
	stroke_arc(x, y, r * 2, r * 2, 0, line_thickness, color);
}

void
Context::fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,
    util::Color color)
{
	if (r == 0) {
		for (int i = 0; i < h; ++i)
			stroke_line(x, y + i, x + w, y + i, 1, color);
		return;
	}
	const int16_t xc = x + r;
	const int16_t yc = y + r;
	const auto wc = static_cast<int16_t>(w - 2 * r);
	const auto hc = static_cast<int16_t>(h - 2 * r);
	std::vector<Point> pnts;
	pnts.reserve(12);
	pnts[0] = { x, yc };
	pnts[1] = { xc, yc };
	pnts[2] = { xc, y };
	pnts[3] = { xc + wc, y };
	pnts[4] = { xc + wc, yc };
	pnts[5] = { x + w, yc };
	pnts[6] = { x + w, yc + hc };
	pnts[7] = { xc + wc, yc + hc };
	pnts[8] = { xc + wc, y + h };
	pnts[9] = { xc, y + h };
	pnts[10] = { xc, yc + hc };
	pnts[11] = { x, yc + hc };
	fill_polygon(pnts, color);
	fill_arc(xc + wc - r, y, r * 2, r * 2, 0, color);
	fill_arc(x, y, r * 2, r * 2, 90, color);
	fill_arc(xc + wc - r, yc + hc - r, r * 2, r * 2, 180, color);
	fill_arc(x, yc + hc - r, r * 2, r * 2, 270, color);
}

void
Context::draw_rect_multi_color(int16_t x, int16_t y, int16_t w, int16_t h,
    util::Color tl, util::Color tr, util::Color br, util::Color bl)
{
	std::vector<util::Color> edge_buf;
	edge_buf.reserve(2 * (w + h));
	util::Color *edge_t = &edge_buf[0];
	util::Color *edge_b = &edge_buf[w];
	util::Color *edge_l = &edge_buf[w * 2];
	util::Color *edge_r = &edge_buf[w * 2 + h];
	for (int i = 0; i < w; ++i) {
		const float factor = i / (w - 1);
		edge_t[i] = tr.gradient(tl, factor);
		edge_b[i] = bl.gradient(br, factor);
	}
	for (int i = 0; i < h; ++i) {
		const float factor = i / (h - 1);
		edge_l[i] = bl.gradient(tl, factor);
		edge_r[i] = tr.gradient(br, factor);
	}
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			if (i == 0) {
				_image->blend_pixel(x + j, y + i, edge_t[j]);
			} else if (i == h - 1) {
				_image->blend_pixel(x + j, y + i, edge_b[i]);
			} else if (j == 0) {
				_image->blend_pixel(x + j, y + i, edge_l[i]);
			} else if (j == w - 1) {
				_image->blend_pixel(x + j, y + i, edge_r[i]);
			} else {
				const auto color = edge_r[i].gradient(edge_l[i],
				    j / (w - 1));
				_image->blend_pixel(x + j, y + i, color);
			}
		}
	}
}

void
Context::fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
    int16_t x2, int16_t y2, const util::Color color)
{
	std::vector<Point> points;
	points.reserve(3);
	points[0] = { x0, y0 };
	points[1] = { x1, y1 };
	points[2] = { x2, y2 };
	fill_polygon(points, color);
}

void
Context::stroke_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
    int16_t x2, int16_t y2, int16_t line_thickness, util::Color color)
{
	stroke_line(x0, y0, x1, y1, line_thickness, color);
	stroke_line(x1, y1, x2, y2, line_thickness, color);
	stroke_line(x2, y2, x0, y0, line_thickness, color);
}

void
Context::stroke_polygon(const std::vector<Point> &points,
    int16_t line_thickness, util::Color color)
{
	for (size_t i = 0, p = points.size() - 1; i < points.size(); p = i++) {
		const auto [x0, y0] = points[p];
		const auto [x1, y1] = points[i];
		stroke_line(x0, y0, x1, y1, line_thickness, color);
	}
}

void
Context::stroke_polyline(const std::vector<Point> &points,
    int16_t line_thickness, util::Color color)
{
	for (size_t i = 0; i < points.size() - 1; ++i) {
		const auto [x0, y0] = points[i];
		const auto [x1, y1] = points[i + 1];
		stroke_line(x0, y0, x1, y1, line_thickness, color);
	}
}

void
Context::fill_circle(int16_t x0, int16_t y0, int16_t w, int16_t h,
    const util::Color color)
{
	const int a2 = (w * w) / 4;
	const int b2 = (h * h) / 4;
	const int fa2 = 4 * a2;
	const int fb2 = 4 * b2;
	h = (h + 1) / 2;
	w = w + 1 / 2;
	x0 += w;
	y0 += h;

	for (int x = 0, y = h, sigma = 2 * b2 + a2 * (1 - 2 * h);
	    b2 * x <= a2 * y; ++x) {
		stroke_line(x0 - x, y0 + y, x0 + x, y0 + y, 1, color);
		stroke_line(x0 - x, y0 - y, x0 + x, y0 - y, 1, color);
		if (sigma >= 0) {
			sigma += fa2 * (1 - y);
			--y;
		}
		sigma += b2 * (4 * x + 6);
	}
	for (int x = w, y = h - 1, sigma = 2 * a2 + b2 * (1 - 2 * w);
	    a2 * y <= b2 * x; ++y) {
		stroke_line(x0 - x, y0 + y, x0 + x, y0 + y, 1, color);
		stroke_line(x0 - x, y0 - y, x0 + x, y0 - y, 1, color);
		if (sigma >= 0) {
			sigma += fb2 * (1 - x);
			--x;
		}
		sigma += a2 * (4 * y + 6);
	}
}

void
Context::stroke_circle(int16_t x0, int16_t y0, int16_t w, int16_t h,
    int16_t line_thickness, util::Color color)
{
	const int a2 = (w * w) / 4;
	const int b2 = (h * h) / 4;
	const int fa2 = 4 * a2;
	const int fb2 = 4 * b2;
	(void)line_thickness;
	h = (h + 1) / 2;
	w = w + 1 / 2;
	x0 += w;
	y0 += h;

	for (int x = 0, y = h, sigma = 2 * b2 + a2 * (1 - 2 * h);
	    b2 * x <= a2 * y; ++x) {
		set_pixel(x0 + x, y0 + y, color);
		set_pixel(x0 - x, y0 - y, color);
		set_pixel(x0 + x, y0 - y, color);
		set_pixel(x0 - x, y0 - 1, color);
		if (sigma >= 0) {
			sigma += fa2 * (1 - y);
			--y;
		}
		sigma += b2 * (4 * x + 6);
	}
	for (int x = w, y = h - 1, sigma = 2 * a2 + b2 * (1 - 2 * w);
	    a2 * y <= b2 * x; ++y) {
		set_pixel(x0 + x, y0 + y, color);
		set_pixel(x0 - x, y0 - y, color);
		set_pixel(x0 + x, y0 - y, color);
		set_pixel(x0 - x, y0 - 1, color);
		if (sigma >= 0) {
			sigma += fb2 * (1 - x);
			--x;
		}
		sigma += a2 * (4 * y + 6);
	}
}

void
Context::clear()
{
	fill_rect(0, 0, width(), height(), 0, clear_color());
}

euler::gui::Widget::ID
Context::next_widget_id()
{
	if (_available_widget_ids.empty()) return ++_widget_counter;
	const auto id = _available_widget_ids.front();
	_available_widget_ids.pop_front();
	return id;
}

euler::util::Reference<euler::gui::Widget>
Context::widget(const char *title, const Widget::Settings &settings,
    const std::function<void(util::Reference<Widget> &)> &fn)
{
	Widget::ID id = next_widget_id();
	_widget_callbacks.emplace(id, fn);
	return util::make_reference<Widget>(util::Reference(this), id, title,
	    settings);
}

euler::util::Reference<euler::gui::Widget>
Context::widget(const char *title, const Widget::Settings &settings,
    mrb_value block)
{
	Widget::ID id = next_widget_id();
	state()->mrb()->gc_register(block);
	_widget_callbacks.emplace(id, block);
	return util::make_reference<Widget>(util::Reference(this), id, title,
	    settings);
}
