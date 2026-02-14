/* SPDX-License-Identifier: ISC */

#include "euler/util/image.h"
#include "euler/app/native/state.h"

using euler::app::native::State;

State::~State() = default;
State::State(const Arguments &args)
{

}

/* TODO */

State::Arguments
State::parse_args(int, char **)
{
	return {};
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
