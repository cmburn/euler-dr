/* SPDX-License-Identifier: ISC */

#include "euler/app/dragonruby/window.h"

#include "euler/app/dragonruby/state.h"

euler::app::dragonruby::Window::Window(const util::Reference<State> &state)
    : _state(state.weaken())
{
}
int16_t
euler::app::dragonruby::Window::width() const
{
	const auto rb = state()->mrb();
	const auto mrb = rb->mrb();
	const auto grid = rb->module_get("Grid");
	const auto w = rb->funcall_argv(rb->obj_value(grid), EULER_SYM(w_px));
	return static_cast<int16_t>(mrb_fixnum(w));
}

int16_t
euler::app::dragonruby::Window::height() const
{
	const auto rb = state()->mrb();
	const auto mrb = rb->mrb();
	const auto grid = rb->module_get("Grid");
	const auto h = rb->funcall_argv(rb->obj_value(grid), EULER_SYM(h_px));
	return static_cast<int16_t>(mrb_fixnum(h));
}

euler::util::Reference<euler::app::dragonruby::State>
euler::app::dragonruby::Window::state() const
{
	return _state.strengthen();
}