/* SPDX-License-Identifier: ISC */

#include "euler/physics/wheel_joint.h"

/* SPDX-License-Identifier: ISC */

#include "euler/physics/wheel_joint.h"

#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/hash.h>

#include "euler/physics/body.h"
#include "euler/physics/util.h"
#include "euler/physics/world.h"
#include "euler/physics/joint.h"


static mrb_value
wheel_joint_enable_spring(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	b2WheelJoint_EnableSpring(joint->id, flag);
	return mrb_nil_value();
}

static mrb_value
wheel_joint_is_spring_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	const bool flag = b2WheelJoint_IsSpringEnabled(joint->id);
	return mrb_bool_value(flag);
}

static mrb_value
wheel_joint_set_spring_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	mrb_float hertz;
	mrb_get_args(mrb, "f", &hertz);
	b2WheelJoint_SetSpringHertz(joint->id, (float)hertz);
	return mrb_nil_value();
}

static mrb_value
wheel_joint_spring_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	const float hertz = b2WheelJoint_GetSpringHertz(joint->id);
	return mrb_float_value(mrb, hertz);
}

static mrb_value
wheel_joint_set_spring_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	mrb_float damping_ratio;
	mrb_get_args(mrb, "f", &damping_ratio);
	b2WheelJoint_SetSpringDampingRatio(joint->id, (float)damping_ratio);
	return mrb_nil_value();
}

static mrb_value
wheel_joint_spring_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	const float damping_ratio
	    = b2WheelJoint_GetSpringDampingRatio(joint->id);
	return mrb_float_value(mrb, damping_ratio);
}

static mrb_value
wheel_joint_enable_limit(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	b2WheelJoint_EnableLimit(joint->id, flag);
	return mrb_nil_value();
}

static mrb_value
wheel_joint_is_limit_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	const bool flag = b2WheelJoint_IsLimitEnabled(joint->id);
	return mrb_bool_value(flag);
}

static mrb_value
wheel_joint_set_limits(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	mrb_value arr = mrb_nil_value();
	mrb_get_args(mrb, "A", &arr);
	const b2Vec2 limits = value_to_b2_vec(mrb, arr);
	b2WheelJoint_SetLimits(joint->id, limits.x, limits.y);
	return mrb_nil_value();
}

static mrb_value
wheel_joint_limits(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	const float lower_limit = b2WheelJoint_GetLowerLimit(joint->id);
	const float upper_limit = b2WheelJoint_GetUpperLimit(joint->id);
	const mrb_value out = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, out, mrb_float_value(mrb, lower_limit));
	mrb_ary_push(mrb, out, mrb_float_value(mrb, upper_limit));
	return out;
}

static mrb_value
wheel_joint_enable_motor(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	b2WheelJoint_EnableMotor(joint->id, flag);
	return mrb_nil_value();
}

static mrb_value
wheel_joint_is_motor_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	const bool flag = b2WheelJoint_IsMotorEnabled(joint->id);
	return mrb_bool_value(flag);
}

static mrb_value
wheel_joint_set_motor_speed(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	mrb_float motor_speed;
	mrb_get_args(mrb, "f", &motor_speed);
	b2WheelJoint_SetMotorSpeed(joint->id, (float)motor_speed);
	return mrb_nil_value();
}

static mrb_value
wheel_joint_motor_speed(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	const float motor_speed = b2WheelJoint_GetMotorSpeed(joint->id);
	return mrb_float_value(mrb, motor_speed);
}

static mrb_value
wheel_joint_set_max_motor_torque(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	mrb_float max_torque;
	mrb_get_args(mrb, "f", &max_torque);
	b2WheelJoint_SetMaxMotorTorque(joint->id, (float)max_torque);
	return mrb_nil_value();
}

static mrb_value
wheel_joint_max_motor_torque(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	const float max_torque = b2WheelJoint_GetMaxMotorTorque(joint->id);
	return mrb_float_value(mrb, max_torque);
}

static mrb_value
wheel_joint_motor_torque(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, wheel);
	const float torque = b2WheelJoint_GetMotorTorque(joint->id);
	return mrb_float_value(mrb, torque);
}

static struct RClass *
wheel_joint_init(mrb_state *mrb, struct RClass *mod, struct RClass *super)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *joint
	    = mrb_define_class_under(mrb, mod, "WheelJoint", super);
	mrb_define_method(mrb, joint,
	    "spring_enabled=", wheel_joint_enable_spring, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_enabled",
	    wheel_joint_is_spring_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "spring_hertz=", wheel_joint_set_spring_hertz, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_hertz", wheel_joint_spring_hertz,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "spring_damping_ratio=", wheel_joint_set_spring_damping_ratio,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_damping_ratio",
	    wheel_joint_spring_damping_ratio, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "limit_enabled=", wheel_joint_enable_limit, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "limit_enabled",
	    wheel_joint_is_limit_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "limits=", wheel_joint_set_limits,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "limits", wheel_joint_limits,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "motor_enabled=", wheel_joint_enable_motor, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "motor_enabled",
	    wheel_joint_is_motor_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "motor_speed=", wheel_joint_set_motor_speed, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "motor_speed", wheel_joint_motor_speed,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "max_motor_torque=", wheel_joint_set_max_motor_torque,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "max_motor_torque",
	    wheel_joint_max_motor_torque, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "motor_torque", wheel_joint_motor_torque,
	    MRB_ARGS_REQ(0));
	return joint;
}
