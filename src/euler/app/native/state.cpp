/* SPDX-License-Identifier: ISC */

#include "euler/app/native/state.h"

#include "euler/util/image.h"
#include "euler/app/native/logger.h"

using euler::app::native::State;

State::~State() = default;

State::State(const Arguments &args)
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
	_log = euler::util::make_reference<Logger>();

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
}

bool
State::loop(int &exit_code)
{
}
