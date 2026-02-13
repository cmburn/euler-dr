/* SPDX-License-Identifier: ISC */

#include "euler/app/state.h"
#include "euler/math/ext.h"
#include "euler/physics/ext.h"
#include "euler/util/ext.h"

#ifndef EULER_GV_STATE
#define EULER_GV_STATE app
#endif

static constexpr auto GV_STATE_SYM = "$" MRB_STRINGIZE(EULER_GV_STATE);

using euler::app::State;

State::State(const Arguments &args)
    : EULER_APP_NAMESPACE::State(args)
{
}

euler::util::State::nthread_t
State::available_threads() const
{
	return std::thread::hardware_concurrency();
}

mrb_value
State::gv_state() const
{
	return mrb()->gv_get(mrb()->intern_cstr(GV_STATE_SYM));
}

bool
State::initialize()
{
	const auto self = util::Reference(this);
	const auto mod = mrb()->define_module("Euler");
	printf("initialized euler mod\n");
	modules().mod = mod;
	util::init(self, mod);
#ifdef EULER_MATH
	math::init(self, mod);
#endif
#ifdef EULER_PHYSICS
	physics::init(self, mod);
#endif
	auto app_mod = mrb()->define_module_under(mod, "App");
	mrb()->define_class_under(app_mod, "State", object_class());
	return true;
}

euler::util::State::tick_t
State::last_tick() const
{
	return 0;
}

float
State::fps() const
{
	return 0;
}

euler::util::State::tick_t
State::total_ticks() const
{
	return 0;
}

void
State::tick() const
{
}

RClass *
State::object_class() const
{
	return this->mrb()->mrb()->object_class;
}

void *
State::unwrap(mrb_value value, const mrb_data_type *type) const
{
	return mrb()->data_check_get_ptr(value, type);
}
