/* SPDX-License-Identifier: ISC */

#include "euler/gui/window.h"

#include "euler/gui/internal.h"
#include "euler/gui/widget.h"
#include "euler/util/ext.h"

using euler::gui::Window;

mrb_value
Window::display(const util::Reference<Widget> &widget)
{
	// if (widget == nullptr) return mrb_nil_value();
	// auto mrb = state()->state();
	// if (state()->phase() != util::State::Phase::Draw) {
	// 	mrb_raise(mrb, mrb->eStandardError_class,
	// 	    "Widgets can only be displayed during State#draw");
	// }
	// const auto id = widget->id();
	// if (!_widget_callbacks.contains(widget->id())) {
	// 	mrb_raise(mrb, mrb->eStandardError_class,
	// 	    "Widget was not created by this Window, or has already "
	// 	    "been released.");
	// }
	// const auto fn = _widget_callbacks.at(id);
	// if (std::holds_alternative<NativeCallback>(fn)) {
	// 	widget->call(std::get<NativeCallback>(fn));
	// 	return mrb_nil_value();
	// }
	// auto out = mrb_nil_value();
	// widget->call([&](auto &w) {
	// 	assert(std::holds_alternative<mrb_value>(fn));
	// 	const auto block = std::get<mrb_value>(fn);
	// 	auto self_value = euler::util::wrap(mrb, w,
	// 	    state()->module().gui.widget, &Widget::TYPE);
	// 	out = mrb_funcall_argv(mrb, block, MRB_SYM(call), 1,
	// 	    &self_value);
	// });
	// return out;
}

void
Window::release_widget(const Widget::ID id)
{
	const auto fn = _widget_callbacks.find(id);
	if (fn == _widget_callbacks.end()) return;
	if (std::holds_alternative<mrb_value>(fn->second))
		state()->mrb()->gc_unregister(std::get<mrb_value>(fn->second));
	_widget_callbacks.erase(id);
	_available_widget_ids.push_back(id);
}

bool
Window::process_gui_event(SDL_Event &e)
{
	vk2dGuiProcessEvent(&e);
	return true;
}

euler::gui::Widget::ID
Window::next_widget_id()
{
	if (_available_widget_ids.empty()) return ++_widget_counter;
	const auto id = _available_widget_ids.front();
	_available_widget_ids.pop_front();
	return id;
}

Window::Window(const util::Reference<util::Logger> &parent,
    const std::string &progname)
    : graphics::Window(parent, progname)
{
}

euler::util::Reference<euler::gui::Widget>
Window::widget(const char *title, const Widget::Settings &settings,
    const std::function<void(util::Reference<Widget> &)> &fn)
{
	Widget::ID id = next_widget_id();
	_widget_callbacks.emplace(id, fn);
	return util::make_reference<Widget>(util::Reference(this), id, title,
	    settings);
}

euler::util::Reference<euler::gui::Widget>
Window::widget(const char *title, const Widget::Settings &settings,
    mrb_value block)
{
	Widget::ID id = next_widget_id();
	// mrb_gc_register(state()->state(), block)
	state()->mrb()->gc_register(block);
	_widget_callbacks.emplace(id, block);
	return util::make_reference<Widget>(util::Reference(this), id, title,
	    settings);
}
