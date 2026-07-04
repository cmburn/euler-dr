/* SPDX-License-Identifier: ISC */

#include "euler/graphics/renderer.h"

euler::util::Reference<euler::util::State>
euler::graphics::Target::state() const
{
	return _state;
}