/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_DRAGONRUBY_WINDOW_H
#define EULER_APP_DRAGONRUBY_WINDOW_H

#include "euler/util/window.h"

namespace euler::app::dragonruby {
class State;

class Window final : public util::Window {
public:
	Window(const util::Reference<State> &);
	int16_t width() const override;
	int16_t height() const override;
	util::Reference<State> state() const;
private:
	util::WeakReference<State> _state;
};
} /* namespace euler::app::dragonruby */


#endif /* EULER_APP_DRAGONRUBY_WINDOW_H */

