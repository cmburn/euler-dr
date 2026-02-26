/* SPDX-License-Identifier: ISC */

#include "euler/app/native/window.h"

#include "euler/app/native/state.h"

euler::app::native::Window::~Window() = default;

euler::app::native::Window::Window(const util::Reference<util::State> &state)
    : graphics::Window(state->log(), state->title())
{
	_state = state.weaken();
}
euler::util::Reference<euler::util::State>
euler::app::native::Window::state() const
{
	return _state.strengthen();
}