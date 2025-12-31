/* SPDX-License-Identifier: ISC */

#include "euler/physics/motor_joint.h"

/* SPDX-License-Identifier: ISC */

#include "euler/physics/motor_joint.h"

#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/hash.h>

#include "euler/physics/body.h"
#include "euler/physics/util.h"
#include "euler/physics/world.h"
#include "euler/physics/joint.h"

/**
 * @overload Euler::Physics::MotorJoint#linear_velocity=(value)
 *   Set the target linear velocity of the motor joint.
 *   @param value [Array<Numeric>] The target linear velocity as [x, y].
 *   @return [void]
 */
static mrb_value
motor_joint_set_linear_velocity(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	mrb_value value;
	mrb_get_args(mrb, "A", &value);
	const b2Vec2 linear_velocity = value_to_b2_vec(mrb, value);
	b2MotorJoint_SetLinearVelocity(joint->id, linear_velocity);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::MotorJoint#linear_velocity
 *   Get the target linear velocity of the motor joint.
 *   @return [Array<Numeric>] The target linear velocity as [x, y].
 */
static mrb_value
motor_joint_linear_velocity(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	const b2Vec2 linear_velocity
	    = b2MotorJoint_GetLinearVelocity(joint->id);
	return b2_vec_to_value(mrb, linear_velocity);
}

/**
 * @overload Euler::Physics::MotorJoint#angular_velocity=(value)
 *   Set the target angular velocity of the motor joint.
 *   @param value [Numeric] The target angular velocity in radians per second.
 *   @return [void]
 */
static mrb_value
motor_joint_set_angular_velocity(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	mrb_float angular_velocity;
	mrb_get_args(mrb, "f", &angular_velocity);
	b2MotorJoint_SetAngularVelocity(joint->id, (float)angular_velocity);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::MotorJoint#angular_velocity
 *   Get the target angular velocity of the motor joint.
 *   @return [Numeric] The target angular velocity in radians per second.
 */
static mrb_value
motor_joint_angular_velocity(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	const float angular_velocity
	    = b2MotorJoint_GetAngularVelocity(joint->id);
	return mrb_float_value(mrb, angular_velocity);
}

/**
 * @overload Euler::Physics::MotorJoint#max_velocity_force=(value)
 *   Set the maximum force the motor joint can apply to achieve the target
 *   linear velocity.
 *   @param value [Numeric] The maximum force in newtons.
 *   @return [void]
 */
static mrb_value
motor_joint_set_max_velocity_force(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	mrb_float max_force;
	mrb_get_args(mrb, "f", &max_force);
	b2MotorJoint_SetMaxVelocityForce(joint->id, (float)max_force);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::MotorJoint#max_velocity_force
 *   Get the maximum force the motor joint can apply to achieve the target
 *   linear velocity.
 *   @return [Numeric] The maximum force in newtons.
 */
static mrb_value
motor_joint_max_velocity_force(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	const float max_force = b2MotorJoint_GetMaxVelocityForce(joint->id);
	return mrb_float_value(mrb, max_force);
}

/**
 * @overload Euler::Physics::MotorJoint#max_velocity_torque=(value)
 *   Set the maximum torque the motor joint can apply to achieve the target
 *   angular velocity.
 *   @param value [Numeric] The maximum torque in newton-meters.
 *   @return [void]
 */
static mrb_value
motor_joint_set_max_velocity_torque(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	mrb_float max_torque;
	mrb_get_args(mrb, "f", &max_torque);
	b2MotorJoint_SetMaxVelocityTorque(joint->id, (float)max_torque);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::MotorJoint#max_velocity_torque
 *   Get the maximum torque the motor joint can apply to achieve the target
 *   angular velocity.
 *   @return [Numeric] The maximum torque in newton-meters
 */
static mrb_value
motor_joint_max_velocity_torque(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	const float max_torque = b2MotorJoint_GetMaxVelocityTorque(joint->id);
	return mrb_float_value(mrb, max_torque);
}

/**
 * @overload Euler::Physics::MotorJoint#linear_hertz=(value)
 *   Set the linear spring stiffness in Hertz.
 *   @param value [Numeric] The linear spring stiffness in Hertz.
 *   @return [void]
 */
static mrb_value
motor_joint_set_linear_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	mrb_float hertz;
	mrb_get_args(mrb, "f", &hertz);
	b2MotorJoint_SetLinearHertz(joint->id, (float)hertz);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::MotorJoint#linear_hertz
 *   Get the linear spring stiffness in Hertz.
 *   @return [Numeric] The linear spring stiffness in Hertz.
 */
static mrb_value
motor_joint_linear_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	const float hertz = b2MotorJoint_GetLinearHertz(joint->id);
	return mrb_float_value(mrb, hertz);
}

/**
 * @overload Euler::Physics::MotorJoint#linear_damping_ratio=(value)
 *   Set the linear spring damping ratio (non-dimensional).
 *   @param value [Numeric] The linear spring damping ratio.
 *   @return [void]
 */
static mrb_value
motor_joint_set_linear_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	mrb_float damping_ratio;
	mrb_get_args(mrb, "f", &damping_ratio);
	b2MotorJoint_SetLinearDampingRatio(joint->id, (float)damping_ratio);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::MotorJoint#linear_damping_ratio
 *   Get the linear spring damping ratio (non-dimensional).
 *   @return [Numeric] The linear spring damping ratio.
 */
static mrb_value
motor_joint_linear_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	const float damping_ratio
	    = b2MotorJoint_GetLinearDampingRatio(joint->id);
	return mrb_float_value(mrb, damping_ratio);
}

/**
 * @overload Euler::Physics::MotorJoint#angular_hertz=(value)
 *   Set the angular spring stiffness in Hertz.
 *   @param value [Numeric] The angular spring stiffness in Hertz.
 *   @return [void]
 */
static mrb_value
motor_joint_set_angular_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	mrb_float hertz;
	mrb_get_args(mrb, "f", &hertz);
	b2MotorJoint_SetAngularHertz(joint->id, (float)hertz);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::MotorJoint#angular_hertz
 *   Get the angular spring stiffness in Hertz.
 *   @return [Numeric] The angular spring stiffness in Hertz.
 */
static mrb_value
motor_joint_angular_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	const float hertz = b2MotorJoint_GetAngularHertz(joint->id);
	return mrb_float_value(mrb, hertz);
}

/**
 * @overload Euler::Physics::MotorJoint#angular_damping_ratio=(value)
 *   Set the angular spring damping ratio (non-dimensional).
 *   @param value [Numeric] The angular spring damping ratio.
 *   @return [void]
 */
static mrb_value
motor_joint_set_angular_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	mrb_float damping_ratio;
	mrb_get_args(mrb, "f", &damping_ratio);
	b2MotorJoint_SetAngularDampingRatio(joint->id, (float)damping_ratio);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::MotorJoint#angular_damping_ratio
 *   Get the angular spring damping ratio (non-dimensional).
 *   @return [Numeric] The angular spring damping ratio.
 */
static mrb_value
motor_joint_angular_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	const float damping_ratio
	    = b2MotorJoint_GetAngularDampingRatio(joint->id);
	return mrb_float_value(mrb, damping_ratio);
}

/**
 * @overload Euler::Physics::MotorJoint#max_spring_force=(value)
 *   Set the maximum spring force the motor joint can apply.
 *   @param value [Numeric] The maximum spring force in newtons.
 *   @return [void]
 */
static mrb_value
motor_joint_set_max_spring_force(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	mrb_float max_force;
	mrb_get_args(mrb, "f", &max_force);
	b2MotorJoint_SetMaxSpringForce(joint->id, (float)max_force);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::MotorJoint#max_spring_force
 *   Get the maximum spring force the motor joint can apply.
 *   @return [Numeric] The maximum spring force in newtons.
 */
static mrb_value
motor_joint_max_spring_force(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	const float max_force = b2MotorJoint_GetMaxSpringForce(joint->id);
	return mrb_float_value(mrb, max_force);
}

/**
 * @overload Euler::Physics::MotorJoint#max_spring_torque=(value)
 *   Set the maximum spring torque the motor joint can apply.
 *   @param value [Numeric] The maximum spring torque in newton-meters.
 *   @return [void]
 */
static mrb_value
motor_joint_set_max_spring_torque(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	mrb_float max_torque;
	mrb_get_args(mrb, "f", &max_torque);
	b2MotorJoint_SetMaxSpringTorque(joint->id, (float)max_torque);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::MotorJoint#max_spring_torque
 *   Get the maximum spring torque the motor joint can apply.
 *   @return [Numeric] The maximum spring torque in newton-meters.
 */
static mrb_value
motor_joint_max_spring_torque(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, motor);
	const float max_torque = b2MotorJoint_GetMaxSpringTorque(joint->id);
	return mrb_float_value(mrb, max_torque);
}

static struct RClass *
motor_joint_init(mrb_state *mrb, struct RClass *mod, struct RClass *super)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *joint
	    = mrb_define_class_under(mrb, mod, "MotorJoint", super);
	mrb_define_method(mrb, joint,
	    "linear_velocity=", motor_joint_set_linear_velocity,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "linear_velocity",
	    motor_joint_linear_velocity, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "angular_velocity=", motor_joint_set_angular_velocity,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "angular_velocity",
	    motor_joint_angular_velocity, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "max_velocity_force=", motor_joint_set_max_velocity_force,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "max_velocity_force",
	    motor_joint_max_velocity_force, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "max_velocity_torque=", motor_joint_set_max_velocity_torque,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "max_velocity_torque",
	    motor_joint_max_velocity_torque, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "linear_hertz=", motor_joint_set_linear_hertz, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "linear_hertz", motor_joint_linear_hertz,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "linear_damping_ratio=", motor_joint_set_linear_damping_ratio,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "linear_damping_ratio",
	    motor_joint_linear_damping_ratio, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "angular_hertz=", motor_joint_set_angular_hertz, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "angular_hertz",
	    motor_joint_angular_hertz, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "angular_damping_ratio=", motor_joint_set_angular_damping_ratio,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "angular_damping_ratio",
	    motor_joint_angular_damping_ratio, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "max_spring_force=", motor_joint_set_max_spring_force,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "max_spring_force",
	    motor_joint_max_spring_force, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "max_spring_torque=", motor_joint_set_max_spring_torque,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "max_spring_torque",
	    motor_joint_max_spring_torque, MRB_ARGS_REQ(0));
	return joint;
}
