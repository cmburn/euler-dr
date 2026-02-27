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

State::~State() = default;

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

static mrb_value
state_allocate(mrb_state *mrb, const mrb_value self)
{
	auto state = State::get(mrb);
	assert(state != nullptr);
	const auto cl = mrb_class_ptr(self);
	const auto obj = Data_Wrap_Struct(mrb, cl, &State::TYPE, state.wrap());
	return mrb_obj_value(obj);
}

bool
State::initialize()
{
	mrb()->mrb()->ud = util::WeakReference(this).wrap();
	initialize_self();
	if (!EULER_APP_NAMESPACE::State::initialize()) return false;
	// auto self = util::Reference(this);
	// 	util::init(self, mod);
	// #ifdef EULER_MATH
	// 	math::init(self, mod);
	// #endif
	// #ifdef EULER_PHYSICS
	// 	physics::init(self, mod);
	// #endif
	return true;
}

euler::util::State::tick_t
State::last_tick() const
{
	return _last_tick;
}

float
State::fps() const
{
	return _fps;
}

euler::util::State::tick_t
State::total_ticks() const
{
	return _total_ticks;
}

void
State::tick()
{
	_last_tick = _tick;
	_tick = ticks();
	++_total_ticks;
	if (_tick - _last_frame_tick < 1000) return;
	const auto frames = _total_ticks - _last_frame_total_ticks;
	const auto milliseconds = _tick - _last_frame_tick;
	const auto seconds = static_cast<double>(milliseconds) / 1000.0;
	_fps = static_cast<float>(static_cast<double>(frames) / seconds);
	_last_frame_tick = _tick;
	_last_frame_total_ticks = _total_ticks;
}

RClass *
State::object_class() const
{
	return this->mrb()->mrb()->object_class;
}

void *
State::unwrap(const mrb_value value, const mrb_data_type *type) const
{
	return mrb()->data_check_get_ptr(value, type);
}
State::Phase
State::phase() const
{
	return _phase;
}
void
State::set_phase(Phase phase)
{
	_phase = phase;
}

mrb_value
State::self_value() const
{
	// assert(_initialized_self);
	// return _self_value;
	auto gv = gv_state();
	return mrb_nil_p(gv) ? _self_value : gv;
}
void
State::initialize_self()
{
	if (_initialized_self) return;
	const auto mod = mrb()->define_module("Euler");
	modules().mod = mod;
	const auto util_mod = mrb()->define_module_under(mod, "Util");
	modules().util.mod = util_mod;
	const auto cls
	    = mrb()->define_class_under(util_mod, "State", object_class());
	MRB_SET_INSTANCE_TT(cls, MRB_TT_DATA);
	modules().app.state = cls;
	mrb()->define_class_method(cls, "allocate", state_allocate,
	    MRB_ARGS_NONE());
	// Data_Wrap_Struct()
	const auto ptr = util::WeakReference(this).wrap();
	const auto data = mrb()->data_object_alloc(cls, ptr, &State::TYPE);
	_self_value = mrb_obj_value(data);
	_initialized_self = true;
}
