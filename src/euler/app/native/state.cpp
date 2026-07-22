/* SPDX-License-Identifier: ISC */

#include "euler/app/native/state.h"

using euler::app::native::State;

bool
State::loop(int &exit_code)
{
	(void)exit_code;
	return true;
}