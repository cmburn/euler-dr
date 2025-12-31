/* SPDX-License-Identifier: ISC */

#include "euler/physics/distance_joint.h"

#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/hash.h>

#include "euler/physics/body.h"
#include "euler/physics/joint.h"
#include "euler/physics/util.h"
#include "euler/physics/world.h"

/**
 * @overload Euler::Physics::DistanceJoint#length=(length)
 *   Set the natural length between the two anchor points.
 *   @param length [Numeric] The new length.
 *   @return [void]
 */
static mrb_value
distance_joint_set_length(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	mrb_float length;
	mrb_get_args(mrb, "f", &length);
	joint->set_length(static_cast<float>(length));
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::DistanceJoint#length
 *   Get the natural length between the two anchor points.
 *   @return [Numeric] The length.
 */
static mrb_value
distance_joint_length(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	const float length = joint->length();
	return mrb_float_value(mrb, length);
}

/**
 * @overload Euler::Physics::DistanceJoint#spring_enabled=(flag)
 *  Enable or disable the distance joint's spring.
 *  @param flag [Boolean] true to enable the spring.
 *  @return [void]
 */
static mrb_value
distance_joint_enable_spring(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	joint->enable_spring(flag);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::DistanceJoint#spring_enabled?
 *   @see Euler::Physics::DistanceJoint#spring_enabled
 * @overload Euler::Physics::DistanceJoint#spring_enabled
 *   Check if the distance joint's spring is enabled.
 *   @return [Boolean] true if the spring is enabled.
 */
static mrb_value
distance_joint_is_spring_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	const bool flag = joint->is_spring_enabled();
	return mrb_bool_value(flag);
}

/**
 * @overload Euler::Physics::DistanceJoint#spring_force_range=(range)
 *   Set the force range for the distance joint's spring.
 *   @param range [Array<Numeric>] The lower and upper force limits.
 *   @return [void]
 */
static mrb_value
distance_joint_set_spring_force_range(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	mrb_value arr;
	mrb_get_args(mrb, "A", &arr);
	const b2Vec2 range = euler::physics::value_to_b2_vec(mrb, arr);
	joint->set_spring_force_range(range.x, range.y);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::DistanceJoint#spring_force_range
 *   Get the force range for the distance joint's spring.
 *   @return [Array<Numeric>] The lower and upper force limits.
 */
static mrb_value
distance_joint_spring_force_range(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	auto [lower_force, upper_force] = joint->spring_force_range();
	const mrb_value out = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, out, mrb_float_value(mrb, lower_force));
	mrb_ary_push(mrb, out, mrb_float_value(mrb, upper_force));
	return out;
}

/**
 * @overload Euler::Physics::DistanceJoint#spring_hertz=(hertz)
 *   Set the spring stiffness in Hertz.
 *   @param hertz [Numeric] The spring hertz.
 *   @return [void]
 */
static mrb_value
distance_joint_set_spring_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	mrb_float hertz;
	mrb_get_args(mrb, "f", &hertz);
	joint->set_spring_hertz(static_cast<float>(hertz));
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::DistanceJoint#spring_hertz
 *   Get the spring stiffness in Hertz.
 *   @return [Numeric] The spring hertz.
 */
static mrb_value
distance_joint_spring_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	const float hertz = joint->spring_hertz();
	return mrb_float_value(mrb, hertz);
}

/**
 * @overload Euler::Physics::DistanceJoint#spring_damping_ratio=(damping_ratio)
 *   Set the spring damping ratio, non-dimensional.
 *   @param damping_ratio [Numeric] The damping ratio.
 *   @return [void]
 */
static mrb_value
distance_joint_set_spring_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	mrb_float damping_ratio;
	mrb_get_args(mrb, "f", &damping_ratio);
	joint->set_spring_damping_ratio(static_cast<float>(damping_ratio));
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::DistanceJoint#spring_damping_ratio
 *   Get the spring damping ratio, non-dimensional.
 *   @return [Numeric] The damping ratio.
 */
static mrb_value
distance_joint_spring_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	const float damping_ratio
	    = joint->spring_damping_ratio();
	return mrb_float_value(mrb, damping_ratio);
}

/**
 * @overload Euler::Physics::DistanceJoint#limit_enabled=(flag)
 *   Enable or disable the distance joint's limit.
 *   @param flag [Boolean] true to enable the limit.
 *   @return [void]
 */
static mrb_value
distance_joint_enable_limit(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	joint->enable_limit(flag);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::DistanceJoint#limit_enabled?
 *   @see Euler::Physics::DistanceJoint#limit_enabled
 * @overload Euler::Physics::DistanceJoint#limit_enabled
 *   Check if the distance joint's limit is enabled.
 *   @return [Boolean] true if the limit is enabled.
 */
static mrb_value
distance_joint_is_limit_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	const bool flag = joint->is_limit_enabled();
	return mrb_bool_value(flag);
}

/**
 * @overload Euler::Physics::DistanceJoint#length_range=(range)
 *   Set the minimum and maximum length parameters of the distance joint.
 *   @param range [Array<Numeric>] The minimum and maximum lengths.
 *   @return [void]
 */
static mrb_value
distance_joint_set_length_range(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	mrb_value arr;
	mrb_get_args(mrb, "A", &arr);
	const b2Vec2 range = euler::physics::value_to_b2_vec(mrb, arr);
	joint->set_length_range(range.x, range.y);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::DistanceJoint#length_range
 *   Get the minimum and maximum length parameters of the distance joint.
 *   @return [Array<Numeric>] The minimum and maximum lengths.
 */
static mrb_value
distance_joint_length_range(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	auto [min_length, max_length] = joint->length_range();
	const mrb_value out = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, out, mrb_float_value(mrb, min_length));
	mrb_ary_push(mrb, out, mrb_float_value(mrb, max_length));
	return out;
}

/**
 * @overload Euler::Physics::DistanceJoint#length_range=(range)
 *   Set the minimum and maximum length parameters of the distance joint.
 *   @param range [Array<Numeric>] The minimum and maximum lengths.
 *   @return [void]
 */
static mrb_value
distance_joint_current_length(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	const float current_length
	    = joint->current_length();
	return mrb_float_value(mrb, current_length);
}

/**
 * @overload Euler::Physics::DistanceJoint#motor_enabled=(flag)
 *   Enable or disable the distance joint's motor.
 *   @param flag [Boolean] true to enable the motor.
 *   @return [void]
 */
static mrb_value
distance_joint_enable_motor(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	joint->enable_motor(flag);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::DistanceJoint#motor_enabled?
 *   @see Euler::Physics::DistanceJoint#motor_enabled
 * @overload Euler::Physics::DistanceJoint#motor_enabled
 *   Check if the distance joint's motor is enabled.
 *   @return [Boolean] true if the motor is enabled.
 */
static mrb_value
distance_joint_is_motor_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	const bool flag = joint->is_motor_enabled();
	return mrb_bool_value(flag);
}

/**
 * @overload Euler::Physics::DistanceJoint#motor_speed=(speed)
 *   Set the motor speed in units per second.
 *   @param speed [Numeric] The motor speed.
 *   @return [void]
 */
static mrb_value
distance_joint_set_motor_speed(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	mrb_float motor_speed;
	mrb_get_args(mrb, "f", &motor_speed);
	joint->set_motor_speed(static_cast<float>(motor_speed));
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::DistanceJoint#motor_speed
 *   Get the motor speed in units per second.
 *   @return [Numeric] The motor speed.
 */
static mrb_value
distance_joint_motor_speed(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	const float motor_speed = joint->motor_speed();
	return mrb_float_value(mrb, motor_speed);
}

/**
 * @overload Euler::Physics::DistanceJoint#max_motor_force=(force)
 *   Set the maximum motor force in Newtons.
 *   @param force [Numeric] The maximum motor force.
 *   @return [void]
 */
static mrb_value
distance_joint_set_max_motor_force(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	mrb_float force;
	mrb_get_args(mrb, "f", &force);
	joint->set_max_motor_force(static_cast<float>(force));
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::DistanceJoint#max_motor_force
 *   Get the maximum motor force in Newtons.
 *   @return [Numeric] The maximum motor force.
 */
static mrb_value
distance_joint_max_motor_force(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	const float force = joint->max_motor_force();
	return mrb_float_value(mrb, force);
}

/**
 * @overload Euler::Physics::DistanceJoint#motor_force
 *   Get the current motor force in Newtons.
 *   @return [Numeric] The current motor force.
 */
static mrb_value
distance_joint_motor_force(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto joint = state->unwrap<euler::physics::DistanceJoint>(self);
	const float force = joint->motor_force();
	return mrb_float_value(mrb, force);
}

struct RClass *
box2d_distance_joint_init(mrb_state *mrb, struct RClass *mod, struct RClass *super)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *joint
	    = mrb_define_class_under(mrb, mod, "DistanceJoint", super);
	mrb_define_method(mrb, joint, "length=", distance_joint_set_length,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "length", distance_joint_length,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "spring_enabled=", distance_joint_enable_spring, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_enabled",
	    distance_joint_is_spring_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "spring_force_range=", distance_joint_set_spring_force_range,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_force_range",
	    distance_joint_spring_force_range, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "spring_hertz=", distance_joint_set_spring_hertz, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_hertz",
	    distance_joint_spring_hertz, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "spring_damping_ratio=", distance_joint_set_spring_damping_ratio,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "spring_damping_ratio",
	    distance_joint_spring_damping_ratio, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "limit_enabled=", distance_joint_enable_limit, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "limit_enabled",
	    distance_joint_is_limit_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "length_range=", distance_joint_set_length_range, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "length_range",
	    distance_joint_length_range, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "current_length",
	    distance_joint_current_length, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "motor_enabled=", distance_joint_enable_motor, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "motor_enabled",
	    distance_joint_is_motor_enabled, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "motor_speed=", distance_joint_set_motor_speed, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "motor_speed", distance_joint_motor_speed,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "max_motor_force=", distance_joint_set_max_motor_force,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "max_motor_force",
	    distance_joint_max_motor_force, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "motor_force", distance_joint_motor_force,
	    MRB_ARGS_REQ(0));
	return joint;
}
