/* SPDX-License-Identifier: ISC */

#include "euler/physics/prismatic_joint.h"

/* SPDX-License-Identifier: ISC */

#include "euler/physics/prismatic_joint.h"

#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/hash.h>

#include "euler/physics/body.h"
#include "euler/physics/util.h"
#include "euler/physics/world.h"
#include "euler/physics/joint.h"

/**
 * @overload Euler::Physics::PrismaticJoint#spring_enabled=(flag)
 *   Enable or disable the prismatic joint's spring.
 *   @param flag [Boolean] true to enable the spring, false to disable it.
 *   @return [void]
 */
static mrb_value
prismatic_joint_enable_spring(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	b2PrismaticJoint_EnableSpring(joint->id, flag);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::PrismaticJoint#spring_enabled
 *   Check if the prismatic joint's spring is enabled.
 *   @return [Boolean] true if the spring is enabled, false otherwise.
 */
static mrb_value
prismatic_joint_is_spring_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const bool flag = b2PrismaticJoint_IsSpringEnabled(joint->id);
	return mrb_bool_value(flag);
}

/**
 * @overload Euler::Physics::PrismaticJoint#spring_hertz=(value)
 *   Set the spring stiffness in Hertz.
 *   @param value [Numeric] The spring stiffness in Hertz.
 *   @return [void]
 */
static mrb_value
prismatic_joint_set_spring_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	mrb_float hertz;
	mrb_get_args(mrb, "f", &hertz);
	b2PrismaticJoint_SetSpringHertz(joint->id, (float)hertz);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::PrismaticJoint#spring_hertz
 *   Get the spring stiffness in Hertz.
 *   @return [Numeric] The spring stiffness in Hertz.
 */
static mrb_value
prismatic_joint_spring_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const float hertz = b2PrismaticJoint_GetSpringHertz(joint->id);
	return mrb_float_value(mrb, hertz);
}

/**
 * @overload Euler::Physics::PrismaticJoint#spring_damping_ratio=(value)
 *   Set the spring damping ratio (non-dimensional).
 *   @param value [Numeric] The spring damping ratio.
 *   @return [void]
 */
static mrb_value
prismatic_joint_set_spring_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	mrb_float damping_ratio;
	mrb_get_args(mrb, "f", &damping_ratio);
	b2PrismaticJoint_SetSpringDampingRatio(joint->id, (float)damping_ratio);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::PrismaticJoint#spring_damping_ratio
 *   Get the spring damping ratio (non-dimensional).
 *   @return [Numeric] The spring damping ratio.
 */
static mrb_value
prismatic_joint_spring_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const float damping_ratio
	    = b2PrismaticJoint_GetSpringDampingRatio(joint->id);
	return mrb_float_value(mrb, damping_ratio);
}

/**
 * @overload Euler::Physics::PrismaticJoint#target_translation=(value)
 *   Set the spring target translation, usually in meters.
 *   @param value [Numeric] The target translation in meters.
 *   @return [void]
 */
static mrb_value
prismatic_joint_set_target_translation(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	mrb_float translation;
	mrb_get_args(mrb, "f", &translation);
	b2PrismaticJoint_SetTargetTranslation(joint->id, (float)translation);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::PrismaticJoint#target_translation
 *   Get the spring target translation, usually in meters.
 *   @return [Numeric] The target translation in meters.
 */
static mrb_value
prismatic_joint_target_translation(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const float translation
	    = b2PrismaticJoint_GetTargetTranslation(joint->id);
	return mrb_float_value(mrb, translation);
}

/**
 * @overload Euler::Physics::PrismaticJoint#limit_enabled=(flag)
 *   Enable or disable the prismatic joint's limit.
 *   @param flag [Boolean] true to enable the limit, false to disable it.
 *   @return [void]
 */
static mrb_value
prismatic_joint_enable_limit(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	b2PrismaticJoint_EnableLimit(joint->id, flag);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::PrismaticJoint#limit_enabled
 *   Check if the prismatic joint's limit is enabled.
 *   @return [Boolean] true if the limit is enabled, false otherwise.
 */
static mrb_value
prismatic_joint_is_limit_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const bool flag = b2PrismaticJoint_IsLimitEnabled(joint->id);
	return mrb_bool_value(flag);
}

/**
 * @overload Euler::Physics::PrismaticJoint#limits=(array)
 *   Set the prismatic joint limits.
 *   @param array [Array<Numeric>] An array containing two numeric values:
 *     the lower limit and the upper limit.
 *   @return [void]
 */
static mrb_value
prismatic_joint_set_limits(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	mrb_value arr = mrb_nil_value();
	mrb_get_args(mrb, "A", &arr);
	const b2Vec2 limits = value_to_b2_vec(mrb, arr);
	b2PrismaticJoint_SetLimits(joint->id, limits.x, limits.y);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::PrismaticJoint#limits
 *   Get the prismatic joint limits.
 *   @return [Array<Numeric>] An array containing two numeric values:
 *     the lower limit and the upper limit.
 */
static mrb_value
prismatic_joint_limits(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const float lower_limit = b2PrismaticJoint_GetLowerLimit(joint->id);
	const float upper_limit = b2PrismaticJoint_GetUpperLimit(joint->id);
	const mrb_value out = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, out, mrb_float_value(mrb, lower_limit));
	mrb_ary_push(mrb, out, mrb_float_value(mrb, upper_limit));
	return out;
}

/**
 * @overload Euler::Physics::PrismaticJoint#motor_enabled=(flag)
 *   Enable or disable the prismatic joint's motor.
 *   @param flag [Boolean] true to enable the motor, false to disable it.
 *   @return [void]
 */
static mrb_value
prismatic_joint_enable_motor(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	b2PrismaticJoint_EnableMotor(joint->id, flag);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::PrismaticJoint#motor_enabled
 *   Check if the prismatic joint's motor is enabled.
 *   @return [Boolean] true if the motor is enabled, false otherwise.
 */
static mrb_value
prismatic_joint_is_motor_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const bool flag = b2PrismaticJoint_IsMotorEnabled(joint->id);
	return mrb_bool_value(flag);
}

/**
 * @overload Euler::Physics::PrismaticJoint#motor_speed=(value)
 *   Set the prismatic joint motor speed, usually in meters per second.
 *   @param value [Numeric] The motor speed in meters per second.
 *   @return [void]
 */
static mrb_value
prismatic_joint_set_motor_speed(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	mrb_float motor_speed;
	mrb_get_args(mrb, "f", &motor_speed);
	b2PrismaticJoint_SetMotorSpeed(joint->id, (float)motor_speed);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::PrismaticJoint#motor_speed
 *   Get the prismatic joint motor speed, usually in meters per second.
 *   @return [Numeric] The motor speed in meters per second.
 */
static mrb_value
prismatic_joint_motor_speed(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const float motor_speed = b2PrismaticJoint_GetMotorSpeed(joint->id);
	return mrb_float_value(mrb, motor_speed);
}

/**
 * @overload Euler::Physics::PrismaticJoint#max_motor_force=(value)
 *   Set the prismatic joint maximum motor force, usually in newtons.
 *   @param value [Numeric] The maximum motor force in newtons.
 *   @return [void]
 */
static mrb_value
prismatic_joint_set_max_motor_force(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	mrb_float force;
	mrb_get_args(mrb, "f", &force);
	b2PrismaticJoint_SetMaxMotorForce(joint->id, (float)force);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::PrismaticJoint#max_motor_force
 *   Get the prismatic joint maximum motor force, usually in newtons.
 *   @return [Numeric] The maximum motor force in newtons.
 */
static mrb_value
prismatic_joint_max_motor_force(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const float force = b2PrismaticJoint_GetMaxMotorForce(joint->id);
	return mrb_float_value(mrb, force);
}

/**
 * @overload Euler::Physics::PrismaticJoint#motor_force
 *   Get the prismatic joint current motor force, usually in newtons.
 *   @return [Numeric] The current motor force in newtons.
 */
static mrb_value
prismatic_joint_motor_force(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const float force = b2PrismaticJoint_GetMotorForce(joint->id);
	return mrb_float_value(mrb, force);
}

/**
 * @overload Euler::Physics::PrismaticJoint#translation
 *   Get the current joint translation, usually in meters.
 *   @return [Numeric] The current joint translation in meters.
 */
static mrb_value
prismatic_joint_translation(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const float translation = b2PrismaticJoint_GetTranslation(joint->id);
	return mrb_float_value(mrb, translation);
}

/**
 * @overload Euler::Physics::PrismaticJoint#speed
 *   Get the current joint translation speed, usually in meters per second.
 *   @return [Numeric] The current joint translation speed in meters per second.
 */
static mrb_value
prismatic_joint_speed(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, prismatic);
	const float speed = b2PrismaticJoint_GetSpeed(joint->id);
	return mrb_float_value(mrb, speed);
}

static struct RClass *
prismatic_joint_init(mrb_state *mrb, struct RClass *mod, struct RClass *super)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *joint
	    = mrb_define_class_under(mrb, mod, "PrismaticJoint", super);
	mrb_define_method(mrb, joint,
	    "spring_enabled=", prismatic_joint_enable_spring, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_enabled",
	    prismatic_joint_is_spring_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "spring_hertz=", prismatic_joint_set_spring_hertz, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_hertz",
	    prismatic_joint_spring_hertz, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "spring_damping_ratio=", prismatic_joint_set_spring_damping_ratio,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_damping_ratio",
	    prismatic_joint_spring_damping_ratio, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "target_translation=", prismatic_joint_set_target_translation,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "target_translation",
	    prismatic_joint_target_translation, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "limit_enabled=", prismatic_joint_enable_limit, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "limit_enabled",
	    prismatic_joint_is_limit_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "limits=", prismatic_joint_set_limits,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "limits", prismatic_joint_limits,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "motor_enabled=", prismatic_joint_enable_motor, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "motor_enabled",
	    prismatic_joint_is_motor_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "motor_speed=", prismatic_joint_set_motor_speed, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "motor_speed",
	    prismatic_joint_motor_speed, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "max_motor_force=", prismatic_joint_set_max_motor_force,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "max_motor_force",
	    prismatic_joint_max_motor_force, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "motor_force",
	    prismatic_joint_motor_force, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "translation",
	    prismatic_joint_translation, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "speed", prismatic_joint_speed,
	    MRB_ARGS_REQ(0));
	return joint;
}
