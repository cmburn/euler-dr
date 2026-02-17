/* SPDX-License-Identifier: ISC */

#include "euler/app/native/state.h"
#include "SDL3/SDL_timer.h"

#include "euler/app/native/logger.h"
#include "euler/util/image.h"
#include "spdlog/sinks/stdout_color_sinks.h"

using euler::app::native::State;

State::~State() = default;

State::State(const Arguments &)
{
	_mrb_state = util::make_reference<RubyState>();
}

/* TODO */

State::Arguments
State::parse_args(int, char **)
{
	return {};
}

bool
State::initialize()
{
	_mrb_state->mrb()->ud = util::Reference(this).wrap();
	_log = euler::util::make_reference<Logger>("euler",
	    std::make_shared<spdlog::sinks::stderr_color_sink_mt>(),
	    std::make_shared<spdlog::sinks::stdout_color_sink_mt>());

	return true;
}

euler::util::Reference<euler::util::Logger>
State::log() const
{
	return _log;
}

euler::util::Reference<euler::util::Storage>
State::user_storage() const
{
	return _user_storage;
}

euler::util::Reference<euler::util::Storage>
State::title_storage() const
{
	return _title_storage;
}

euler::util::Reference<euler::util::Image>
State::load_image(const char *path)
{
	return nullptr;
}

void
State::upload_image(const char *label, const util::Reference<util::Image> &img)
{
}

State::tick_t
State::ticks() const
{
	return SDL_GetTicks();
}

bool
State::loop(int &exit_code)
{
	try {
		if (update(exit_code)) return true;
		_phase = Phase::Quit;
		if (_methods.quit) app_quit();
		return false;
	} catch (const std::exception &e) {
		_log->error("Unhandled exception in loop: {}", e.what());
		exit_code = EXIT_FAILURE;
		return false;
	} catch (...) {
		_log->error("Unknown exception in loop");
		exit_code = EXIT_FAILURE;
		return false;
	}
}
