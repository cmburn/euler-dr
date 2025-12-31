/* SPDX-License-Identifier: ISC */

#include "euler/util/object.h"

#include "euler/util/state.h"

void *
euler::util::checked_unwrap_ptr(mrb_state *mrb, const mrb_value value,
    const mrb_data_type *type)
{
	return State::get(mrb)->mrb()->data_check_get_ptr(value, type);
}