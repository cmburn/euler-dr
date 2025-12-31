/* SPDX-License-Identifier: ISC */

#include "euler/app/state.h"
#include "euler/math/ext.h"
#include "euler/physics/ext.h"
#include "euler/util/ext.h"

#ifndef EULER_GV_STATE
#define EULER_GV_STATE "app"
#endif

static constexpr auto GV_STATE_SYM = "$" EULER_GV_STATE;

euler::app::State::State(const Arguments &args)
    : EULER_APP_NAMESPACE::State(args)
{
}

euler::util::State::nthread_t
euler::app::State::available_threads() const
{
	return std::thread::hardware_concurrency();
}

mrb_value
euler::app::State::gv_state() const
{
	return mrb()->gv_get(mrb()->intern_cstr(GV_STATE_SYM));
}

void
euler::app::State::initialize()
{
	const auto self = util::Reference(this);
	const auto mod = mrb()->define_module("Euler");
	util::init(self, mod);
#ifdef EULER_MATH
	math::init(self, mod);
#endif
#ifdef EULER_PHYSICS
	physics::init(self, mod);
#endif
	auto app_mod = mrb()->define_module_under(mod, "App");
}

euler::util::State::tick_t
euler::app::State::last_tick() const
{
	return 0;
}

float
euler::app::State::fps() const
{
	return 0;
}

euler::util::State::tick_t
euler::app::State::total_ticks() const
{
	return 0;
}

void
euler::app::State::tick() const
{
}

RClass *
euler::app::State::object_class() const
{
	return this->mrb()->mrb()->object_class;
}

void *
euler::app::State::unwrap(mrb_value value, const mrb_data_type *type) const
{
	return mrb()->data_check_get_ptr(value, type);
}