/* SPDX-License-Identifier: ISC */

#include <cassert>

#include "euler/app/dragonruby/state.h"

using euler::app::dragonruby::State;



State::State(const Arguments &args)
    : _api(args.api)
{
	printf("State::State: %p\n", args.state);
	_mrb_state = euler::util::make_reference<MRubyState>(args.state, args.api);
	_log = euler::util::make_reference<Logger>("euler", args.api->drb_log_write);
	args.state->ud = util::Reference(this).wrap();
	assert(args.state->ud != nullptr);
}

euler::util::Reference<euler::util::Logger>
State::log() const
{ return _log; }

euler::util::Reference<euler::util::Storage>
State::user_storage() const
{ return _user_storage; }

euler::util::Reference<euler::util::Storage>
State::title_storage() const
{ return _title_storage; }

euler::util::Reference<euler::util::Image>
State::load_image(const char *path)
{ return nullptr; }

void
State::upload_image(const char *label, const util::Reference<util::Image> &img)
{
	// No-op for now
}

State::tick_t
State::ticks() const
{ return _api->SDL_GetTicks(); }
