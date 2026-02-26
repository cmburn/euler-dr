/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_NATIVE_WINDOW_H
#define EULER_APP_NATIVE_WINDOW_H

#include "euler/util/object.h"
#include "euler/graphics/window.h"

namespace euler::app::native {
class Window final : public graphics::Window {
	BIND_MRUBY("Euler::App::Window", Window, app.window);
public:
	~Window() override;
	explicit Window(const util::Reference<util::State> &state);
	util::Reference<util::State> state() const override;
private:
	util::WeakReference<util::State> _state;
};
} /* namespace euler::app::native */

#endif /* EULER_APP_NATIVE_WINDOW_H */
