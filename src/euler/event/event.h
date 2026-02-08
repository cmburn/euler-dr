/* SPDX-License-Identifier: ISC */

#ifndef EULER_EVENT_EVENT_H
#define EULER_EVENT_EVENT_H

#include "euler/util/state.h"

#include <SDL3/SDL.h>

namespace euler::event {

mrb_value sdl_event_to_mrb(const util::Reference<util::State> &state, const SDL_Event &event);
RClass *init(const util::Reference<util::State> &state, RClass *mod, RClass *);

} /* namespace euler::app */

#endif /* EULER_EVENT_EVENT_H */
