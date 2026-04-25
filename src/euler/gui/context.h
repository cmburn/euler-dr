/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_GUI_H
#define EULER_GUI_GUI_H

#include <deque>
#include <functional>

#include "euler/graphics/Target.h"
#include "euler/gui/button.h"
#include "euler/gui/font.h"
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

private:
	Widget::ID next_widget_id();
	void render();
	using NativeCallback = std::function<void(util::Reference<Widget> &)>;
	using RubyCallback = mrb_value;
	using Callback = std::variant<NativeCallback, RubyCallback>;
	std::deque<Widget::ID> _available_widget_ids;
	std::unordered_map<Widget::ID, Callback> _widget_callbacks;
	util::WeakReference<util::State> _state;
	Widget::ID _widget_counter = 0;
	nk_context _context = {};
	util::Reference<graphics::Target> _renderer;

	// nk_font_atlas _atlas = {};
	// util::Color _clear_color = util::COLOR_BLACK;
	// bool _enable_clear = false;
};

} /* namespace euler::gui */

#endif /* EULER_GUI_GUI_H */
