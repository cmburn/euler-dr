/* SPDX-License-Identifier: ISC */

#include "euler/physics/revolute_joint.h"

/* SPDX-License-Identifier: ISC */

#include "euler/physics/revolute_joint.h"

#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/hash.h>

#include "euler/physics/body.h"
#include "euler/physics/joint.h"
#include "euler/physics/util.h"
#include "euler/physics/world.h"

/**
 * @overload Euler::Physics::RevoluteJoint#spring_enabled=(flag)
 *   Enable or disable the revolute joint's spring.
 *   @param flag [Boolean] true to enable the spring, false to disable it.
 *   @return [void]
 */
static mrb_value
revolute_joint_enable_spring(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	b2RevoluteJoint_EnableSpring(joint->id, flag);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::RevoluteJoint#spring_enabled
 *   Check if the revolute joint's spring is enabled.
 *   @return [Boolean] true if the spring is enabled, false otherwise.
 */
static mrb_value
revolute_joint_is_spring_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	const bool flag = b2RevoluteJoint_IsSpringEnabled(joint->id);
	return mrb_bool_value(flag);
}

/**
 * @overload Euler::Physics::RevoluteJoint#spring_hertz=(value)
 *   Set the spring stiffness in Hertz.
 *   @param value [Numeric] The spring stiffness in Hertz.
 *   @return [void]
 */
static mrb_value
revolute_joint_set_spring_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	mrb_float hertz;
	mrb_get_args(mrb, "f", &hertz);
	b2RevoluteJoint_SetSpringHertz(joint->id, (float)hertz);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::RevoluteJoint#spring_hertz
 *   Get the spring stiffness in Hertz.
 *   @return [Numeric] The spring stiffness in Hertz.
 */
static mrb_value
revolute_joint_spring_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	const float hertz = b2RevoluteJoint_GetSpringHertz(joint->id);
	return mrb_float_value(mrb, hertz);
}

/**
 * @overload Euler::Physics::RevoluteJoint#spring_damping_ratio=(value)
 *   Set the spring damping ratio, non-dimensional.
 *   @param value [Numeric] The spring damping ratio.
 *   @return [void]
 */
static mrb_value
revolute_joint_set_spring_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	mrb_float damping_ratio;
	mrb_get_args(mrb, "f", &damping_ratio);
	b2RevoluteJoint_SetSpringDampingRatio(joint->id, (float)damping_ratio);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::RevoluteJoint#spring_damping_ratio
 *   Get the spring damping ratio, non-dimensional.
 *   @return [Numeric] The spring damping ratio.
 */
static mrb_value
revolute_joint_spring_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	const float damping_ratio
	    = b2RevoluteJoint_GetSpringDampingRatio(joint->id);
	return mrb_float_value(mrb, damping_ratio);
}

/**
 * @overload Euler::Physics::RevoluteJoint#target_angle=(value)
 *   Set the revolute joint spring target angle, in radians.
 *   @param value [Numeric] The target angle in radians.
 *   @return [void]
 */
static mrb_value
revolute_joint_set_target_angle(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	mrb_float angle;
	mrb_get_args(mrb, "f", &angle);
	b2RevoluteJoint_SetTargetAngle(joint->id, (float)angle);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::RevoluteJoint#target_angle
 *   Get the revolute joint spring target angle, in radians.
 *   @return [Numeric] The target angle in radians.
 */
static mrb_value
revolute_joint_target_angle(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	const float angle = b2RevoluteJoint_GetTargetAngle(joint->id);
	return mrb_float_value(mrb, angle);
}

/**
 * @overload Euler::Physics::RevoluteJoint#angle
 *   Get the current joint angle in radians relative to the reference angle.
 *   @return [Numeric] The current joint angle in radians.
 */
static mrb_value
revolute_joint_angle(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	const float angle = b2RevoluteJoint_GetAngle(joint->id);
	return mrb_float_value(mrb, angle);
}

/**
 * @overload Euler::Physics::RevoluteJoint#limit_enabled=(flag)
 *   Enable or disable the revolute joint's limit.
 *   @param flag [Boolean] true to enable the limit, false to disable it.
 *   @return [void]
 */
static mrb_value
revolute_joint_enable_limit(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	b2RevoluteJoint_EnableLimit(joint->id, flag);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::RevoluteJoint#limit_enabled
 *   Check if the revolute joint's limit is enabled.
 *   @return [Boolean] true if the limit is enabled, false otherwise.
 */
static mrb_value
revolute_joint_is_limit_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	const bool flag = b2RevoluteJoint_IsLimitEnabled(joint->id);
	return mrb_bool_value(flag);
}

/**
 * @overload Euler::Physics::RevoluteJoint#limits=(array)
 *   Set the revolute joint limits.
 *   @param array [Array<Numeric>] An array containing the lower and upper
 *          limits in radians.
 *   @return [void]
 */
static mrb_value
revolute_joint_set_limits(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	mrb_value arr = mrb_nil_value();
	mrb_get_args(mrb, "A", &arr);
	const b2Vec2 limits = value_to_b2_vec(mrb, arr);
	b2RevoluteJoint_SetLimits(joint->id, limits.x, limits.y);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::RevoluteJoint#limits
 *   Get the revolute joint limits.
 *   @return [Array<Numeric>] An array containing the lower and upper limits
 *          in radians.
 */
static mrb_value
revolute_joint_limits(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	const float lower_limit = b2RevoluteJoint_GetLowerLimit(joint->id);
	const float upper_limit = b2RevoluteJoint_GetUpperLimit(joint->id);
	const mrb_value out = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, out, mrb_float_value(mrb, lower_limit));
	mrb_ary_push(mrb, out, mrb_float_value(mrb, upper_limit));
	return out;
}

/**
 * @overload Euler::Physics::RevoluteJoint#motor_enabled=(flag)
 *   Enable or disable the revolute joint's motor.
 *   @param flag [Boolean] true to enable the motor, false to disable it.
 *   @return [void]
 */
static mrb_value
revolute_joint_enable_motor(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	b2RevoluteJoint_EnableMotor(joint->id, flag);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::RevoluteJoint#motor_enabled
 *   Check if the revolute joint's motor is enabled.
 *   @return [Boolean] true if the motor is enabled, false otherwise.
 */
static mrb_value
revolute_joint_is_motor_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	const bool flag = b2RevoluteJoint_IsMotorEnabled(joint->id);
	return mrb_bool_value(flag);
}

/**
 * @overload Euler::Physics::RevoluteJoint#motor_speed=(value)
 *   Set the revolute joint motor speed, usually in radians per second.
 *   @param value [Numeric] The motor speed in radians per second.
 *   @return [void]
 */
static mrb_value
revolute_joint_set_motor_speed(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	mrb_float motor_speed;
	mrb_get_args(mrb, "f", &motor_speed);
	b2RevoluteJoint_SetMotorSpeed(joint->id, (float)motor_speed);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::RevoluteJoint#motor_speed
 *   Get the revolute joint motor speed, usually in radians per second.
 *   @return [Numeric] The motor speed in radians per second.
 */
static mrb_value
revolute_joint_motor_speed(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	const float motor_speed = b2RevoluteJoint_GetMotorSpeed(joint->id);
	return mrb_float_value(mrb, motor_speed);
}

/**
 * @overload Euler::Physics::RevoluteJoint#motor_torque
 *   Get the revolute joint current motor torque, usually in newton-meters.
 *   @return [Numeric] The current motor torque in newton-meters.
 */
static mrb_value
revolute_joint_motor_torque(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	const float torque = b2RevoluteJoint_GetMotorTorque(joint->id);
	return mrb_float_value(mrb, torque);
}

/**
 * @overload Euler::Physics::RevoluteJoint#max_motor_torque=(value)
 *   Set the revolute joint maximum motor torque, usually in newton-meters.
 *   @param value [Numeric] The maximum motor torque in newton-meters.
 *   @return [void]
 */
static mrb_value
revolute_joint_set_max_motor_torque(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	mrb_float max_torque;
	mrb_get_args(mrb, "f", &max_torque);
	b2RevoluteJoint_SetMaxMotorTorque(joint->id, (float)max_torque);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::RevoluteJoint#max_motor_torque
 *   Get the revolute joint maximum motor torque, usually in newton-meters.
 *   @return [Numeric] The maximum motor torque in newton-meters.
 */
static mrb_value
revolute_joint_max_motor_torque(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, revolute);
	const float max_torque = b2RevoluteJoint_GetMaxMotorTorque(joint->id);
	return mrb_float_value(mrb, max_torque);
}

struct RClass *
box2d_revolute_joint_init(mrb_state *mrb, struct RClass *mod,
    struct RClass *super)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *joint
	    = mrb_define_class_under(mrb, mod, "RevoluteJoint", super);
	mrb_define_method(mrb, joint,
	    "spring_enabled=", revolute_joint_enable_spring, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_enabled",
	    revolute_joint_is_spring_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "spring_hertz=", revolute_joint_set_spring_hertz, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_hertz",
	    revolute_joint_spring_hertz, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "spring_damping_ratio=", revolute_joint_set_spring_damping_ratio,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_damping_ratio",
	    revolute_joint_spring_damping_ratio, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "target_angle=", revolute_joint_set_target_angle, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "target_angle",
	    revolute_joint_target_angle, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "angle", revolute_joint_angle,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "limit_enabled=", revolute_joint_enable_limit, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "limit_enabled",
	    revolute_joint_is_limit_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "limits=", revolute_joint_set_limits,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "limits", revolute_joint_limits,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "motor_enabled=", revolute_joint_enable_motor, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "motor_enabled",
	    revolute_joint_is_motor_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "motor_speed=", revolute_joint_set_motor_speed, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "motor_speed", revolute_joint_motor_speed,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "motor_torque",
	    revolute_joint_motor_torque, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "max_motor_torque=", revolute_joint_set_max_motor_torque,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "max_motor_torque",
	    revolute_joint_max_motor_torque, MRB_ARGS_REQ(0));
	return joint;
}
