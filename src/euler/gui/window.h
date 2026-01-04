/* SPDX-License-Identifier: ISC */

#ifndef EULER_GUI_GUI_H
#define EULER_GUI_GUI_H

#include <deque>
#include <functional>

#include "euler/graphics/window.h"
#include "euler/gui/button.h"
#include "euler/gui/widget.h"
#include "euler/util/object.h"

struct nk_context;
struct nk_sdl;

namespace euler::gui {

/*
 * Our GUI library is heavily patterned after
 * https://github.com/keharriso/love-nuklear/ but adapted to mesh better with
 * our engine architecture and mRuby. Both use Nuklear as the underlying GUI
 * library.
 */

class Window : public graphics::Window {
public:
	Window(const util::Reference<util::Logger> &parent,
	    const std::string &progname);
	~Window() override = default;

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

	bool process_gui_event(SDL_Event &e) override;

	const vulkan::detail::gui_context *
	context() const
	{
		return renderer()->gui_context();
	}

	vulkan::detail::gui_context *
	context()
	{
		return renderer()->gui_context();
	}

private:
	Widget::ID next_widget_id();
	using NativeCallback = std::function<void(util::Reference<Widget> &)>;
	using RubyCallback = mrb_value;
	using Callback = std::variant<NativeCallback, RubyCallback>;
	std::deque<Widget::ID> _available_widget_ids;
	std::unordered_map<Widget::ID, Callback> _widget_callbacks;
	Widget::ID _widget_counter = 0;
};

} /* namespace euler::gui */

#endif /* EULER_GUI_GUI_H */
