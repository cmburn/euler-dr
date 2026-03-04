/* SPDX-License-Identifier: ISC */

#include <cassert>

#include "euler/app/dragonruby/state.h"

using euler::app::dragonruby::State;

State::State(const Arguments &args)
    : _api(args.api)
{
	printf("State::State: %p\n", args.state);
	_mrb_state
	    = euler::util::make_reference<RubyState>(args.state, args.api);
	_log = euler::util::make_reference<Logger>("euler",
	    args.api->drb_log_write);
	args.state->ud = util::Reference(this).wrap();
}

bool
State::initialize()
{
	return true;
}

euler::util::Reference<euler::util::Logger>
State::log() const
{
	return _log;
}

euler::util::Reference<euler::util::Image>
State::load_image(const char *)
{
	return nullptr;
}

State::tick_t
State::ticks() const
{
	return _api->SDL_GetTicks();
}
float
State::dt() const
{
	static constexpr float FPS = 1.0 / 60.0;
	return FPS;
}
const std::string &
State::progname() const
{
	static const std::string name = "dragonruby";
	return name;
}
const std::string &
State::title() const
{
	static const std::string title = "DragonRuby";
	return title;
}

bool
State::preinit()
{
	return true;
}
