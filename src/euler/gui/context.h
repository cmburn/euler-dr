/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_GUI_H
#define EULER_GUI_GUI_H

#include <deque>
#include <functional>

#include "euler/gui/button.h"
#include "euler/gui/widget.h"
#include "euler/util/color.h"
#include "euler/util/object.h"

struct nk_context;
struct nk_sdl;

namespace euler::gui {

/* Our GUI library is heavily patterned after
 * https://github.com/keharriso/love-nuklear/ but adapted to mesh better with
 * our engine architecture and mRuby. Both use Nuklear as the underlying GUI
 * library. */

/* This API should be considered *wildly* unstable as of now. I intend to allow
 * both DragonRuby rendering and SDL3/Vulkan rendering, but I've yet to figure
 * out the portions that can be separated into a common API. This is difficult
 * however as DragonRuby purposely hides graphics APIs (for a good reason). */

class Context : public util::Object {
public:
	static constexpr uint32_t BYTES_PER_PIXEL = 4;
	~Context() override = default;
	Context(int32_t width, int32_t height, int32_t pitch);

	util::Reference<Widget> widget(const char *title,
	    const Widget::Settings &settings,
	    const std::function<void(util::Reference<Widget> &)> &fn);
	util::Reference<Widget> widget(const char *title,
	    const Widget::Settings &settings, mrb_value block);

	util::Reference<Widget>
	widget(const char *title,
	    const std::function<void(util::Reference<Widget> &)> &fn)
	{
		return widget(title, Widget::DEFAULT_SETTINGS, fn);
	}

	util::Reference<Widget>
	widget(const char *title, const mrb_value block)
	{
		return widget(title, Widget::DEFAULT_SETTINGS, block);
	}

	mrb_value display(const util::Reference<Widget> &widget);
	void release_widget(Widget::ID id);
	const nk_context *context() const;
	nk_context *context();
	util::Reference<util::State> state() const;

	struct PixelInfo {
		uint8_t bytes_per_pixel = 4;
		uint8_t b_shift = 16;
		uint8_t g_shift = 8;
		uint8_t r_shift = 24;
		uint8_t a_shift = 0;
		uint8_t b_loss = 0;
		uint8_t g_loss = 0;
		uint8_t r_loss = 0;
		uint8_t a_loss = 0;
	};

	static constexpr PixelInfo pixel_info = {
		.bytes_per_pixel = BYTES_PER_PIXEL,
		.b_shift = 16,
		.g_shift = 8,
		.r_shift = 24,
		.a_shift = 0,
		.b_loss = 0,
		.g_loss = 0,
		.r_loss = 0,
		.a_loss = 0,
	};

	struct RawImage {
		uint32_t *pixels;
		int32_t width;
		int32_t height;
		int32_t pitch;
	};

	void render();
	uint32_t width() const;
	uint32_t height() const;

	// void resize(const RawImage &);
	uint32_t pitch() const
	{
		return width() * BYTES_PER_PIXEL;
	}
	util::Color
	clear_color() const
	{
		return _clear_color;
	}
	bool
	enable_clear() const
	{
		return _enable_clear;
	}

private:
	using Point = std::pair<int16_t, int16_t>;

	static bool is_cardinal(int16_t s);

	void set_arc_point(int16_t x0, int16_t y0, int16_t x, int16_t y,
	    int16_t s, util::Color color);
	void set_pixel(int16_t x, int16_t y, util::Color color);
	void line_horizontal(int16_t x0, int16_t y, int16_t x1,
	    util::Color color);
	void scissor(float x, float y, float w, float h);
	void stroke_line(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
	    int16_t line_thickness, util::Color color);
	void fill_polygon(const std::vector<Point> &points, util::Color color);
	void stroke_arc(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t s,
	    int16_t line_thickness, util::Color color);
	void fill_arc(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t s,
	    util::Color color);
	void stroke_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,
	    int16_t line_thickness, util::Color color);
	void fill_rect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t r,
	    util::Color color);
	void draw_rect_multi_color(int16_t x, int16_t y, int16_t w, int16_t h,
	    util::Color tl, util::Color tr, util::Color br, util::Color bl);
	void fill_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
	    int16_t x2, int16_t y2, util::Color color);
	void stroke_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
	    int16_t x2, int16_t y2, int16_t line_thickness, util::Color color);
	void stroke_polygon(const std::vector<Point> &points,
	    int16_t line_thickness, util::Color color);
	void stroke_polyline(const std::vector<Point> &points,
	    int16_t line_thickness, util::Color color);
	void fill_circle(int16_t x0, int16_t y0, int16_t w, int16_t h,
	    util::Color color);
	void stroke_circle(int16_t x0, int16_t y0, int16_t w, int16_t h,
	    int16_t line_thickness, util::Color color);
	void clear();

	Widget::ID next_widget_id();
	using NativeCallback = std::function<void(util::Reference<Widget> &)>;
	using RubyCallback = mrb_value;
	using Callback = std::variant<NativeCallback, RubyCallback>;
	std::deque<Widget::ID> _available_widget_ids;
	std::unordered_map<Widget::ID, Callback> _widget_callbacks;
	util::WeakReference<util::State> _state;
	util::Reference<util::Image> _image;
	Widget::ID _widget_counter = 0;
	nk_context _context = {};
	struct nk_rect _scissors = {};
	RawImage _fb = {};
	RawImage _font_tex = {};
	nk_font_atlas _atlas = {};
	util::Color _clear_color = util::COLOR_BLACK;
	bool _enable_clear = false;
};

} /* namespace euler::gui */

#endif /* EULER_GUI_GUI_H */
