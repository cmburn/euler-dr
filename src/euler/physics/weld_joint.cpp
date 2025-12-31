/* SPDX-License-Identifier: ISC */

#include "euler/physics/weld_joint.h"

/* SPDX-License-Identifier: ISC */

#include "euler/physics/weld_joint.h"

#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/hash.h>

#include "euler/physics/body.h"
#include "euler/physics/util.h"
#include "euler/physics/world.h"
#include "euler/physics/joint.h"

/**
 * @overload Euler::Physics::WeldJoint#linear_hertz=(value)
 *   Set the linear spring frequency in Hertz.
 *   @param value [Numeric] The linear spring frequency in Hertz.
 *   @return [void]
 */
static mrb_value
weld_joint_set_linear_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, weld);
	mrb_float hertz;
	mrb_get_args(mrb, "f", &hertz);
	b2WeldJoint_SetLinearHertz(joint->id, (float)hertz);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::WeldJoint#linear_hertz
 *   Get the linear spring frequency in Hertz.
 *   @return [Numeric] The linear spring frequency in Hertz.
 */
static mrb_value
weld_joint_linear_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, weld);
	const float hertz = b2WeldJoint_GetLinearHertz(joint->id);
	return mrb_float_value(mrb, hertz);
}

static mrb_value
weld_joint_set_linear_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, weld);
	mrb_float damping_ratio;
	mrb_get_args(mrb, "f", &damping_ratio);
	b2WeldJoint_SetLinearDampingRatio(joint->id, (float)damping_ratio);
	return mrb_nil_value();
}

static mrb_value
weld_joint_linear_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, weld);
	const float damping_ratio
	    = b2WeldJoint_GetLinearDampingRatio(joint->id);
	return mrb_float_value(mrb, damping_ratio);
}

static mrb_value
weld_joint_set_angular_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, weld);
	mrb_float hertz;
	mrb_get_args(mrb, "f", &hertz);
	b2WeldJoint_SetAngularHertz(joint->id, (float)hertz);
	return mrb_nil_value();
}

static mrb_value
weld_joint_angular_hertz(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, weld);
	const float hertz = b2WeldJoint_GetAngularHertz(joint->id);
	return mrb_float_value(mrb, hertz);
}

static mrb_value
weld_joint_set_angular_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, weld);
	mrb_float damping_ratio;
	mrb_get_args(mrb, "f", &damping_ratio);
	b2WeldJoint_SetAngularDampingRatio(joint->id, (float)damping_ratio);
	return mrb_nil_value();
}

static mrb_value
weld_joint_angular_damping_ratio(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	ASSERT_JOINT_TYPE(joint, weld);
	const float damping_ratio
	    = b2WeldJoint_GetAngularDampingRatio(joint->id);
	return mrb_float_value(mrb, damping_ratio);
}

static struct RClass *
weld_joint_init(mrb_state *mrb, struct RClass *mod, struct RClass *super)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *joint
	    = mrb_define_class_under(mrb, mod, "WeldJoint", super);
	mrb_define_method(mrb, joint,
	    "linear_hertz=", weld_joint_set_linear_hertz, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "linear_hertz", weld_joint_linear_hertz,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "linear_damping_ratio=", weld_joint_set_linear_damping_ratio,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "linear_damping_ratio",
	    weld_joint_linear_damping_ratio, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "angular_hertz=", weld_joint_set_angular_hertz, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "angular_hertz", weld_joint_angular_hertz,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "angular_damping_ratio=", weld_joint_set_angular_damping_ratio,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "angular_damping_ratio",
	    weld_joint_angular_damping_ratio, MRB_ARGS_REQ(0));
	return joint;
}
