/* SPDX-License-Identifier: ISC */

#include "euler/physics/joint.h"

/* SPDX-License-Identifier: ISC */

#include "euler/physics/joint.h"

#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/hash.h>

#include "box2d/types.h"
#include "euler/physics/body.h"
#include "euler/physics/util.h"
#include "euler/physics/world.h"

static mrb_value
joint_type(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	switch (b2Joint_GetType(joint->id)) {
	case b2_distanceJoint: return B2_SYM(distance);
	case b2_filterJoint: return B2_SYM(filter);
	case b2_motorJoint: return B2_SYM(motor);
	case b2_prismaticJoint: return B2_SYM(prismatic);
	case b2_revoluteJoint: return B2_SYM(revolute);
	case b2_weldJoint: return B2_SYM(weld);
	case b2_wheelJoint: return B2_SYM(wheel);
	default: mrb_raise(mrb, E_TYPE_ERROR, "Unknown joint type");
	}
}

static mrb_value
joint_body_a(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	const b2BodyId body_id = b2Joint_GetBodyA(joint->id);
	return box2d_body_wrap(mrb, body_id);
}

static mrb_value
joint_body_b(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	const b2BodyId body_id = b2Joint_GetBodyB(joint->id);
	return box2d_body_wrap(mrb, body_id);
}

static mrb_value
joint_world(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	const b2WorldId world_id = b2Joint_GetWorld(joint->id);
	return box2d_world_wrap(mrb, world_id);
}

static mrb_value
joint_set_local_frame_a(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	mrb_value value;
	mrb_get_args(mrb, "H", &value);
	const b2Transform local_frame_a = value_to_b2_transform(mrb, value);
	b2Joint_SetLocalFrameA(joint->id, local_frame_a);
	return mrb_nil_value();
}

static mrb_value
joint_set_local_frame_b(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	mrb_value value;
	mrb_get_args(mrb, "H", &value);
	const b2Transform local_frame_b = value_to_b2_transform(mrb, value);
	b2Joint_SetLocalFrameB(joint->id, local_frame_b);
	return mrb_nil_value();
}

static mrb_value
joint_local_frame_a(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	const b2Transform local_frame_a = b2Joint_GetLocalFrameA(joint->id);
	return b2_transform_to_value(mrb, local_frame_a);
}

static mrb_value
joint_local_frame_b(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	const b2Transform local_frame_b = b2Joint_GetLocalFrameB(joint->id);
	return b2_transform_to_value(mrb, local_frame_b);
}

static mrb_value
joint_set_collide_connected(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	mrb_bool flag;
	mrb_get_args(mrb, "b", &flag);
	b2Joint_SetCollideConnected(joint->id, flag);
	return mrb_nil_value();
}

static mrb_value
joint_collide_connected(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	const bool flag = b2Joint_GetCollideConnected(joint->id);
	return mrb_bool_value(flag);
}

static mrb_value
joint_wake_bodies(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	b2Joint_WakeBodies(joint->id);
	return mrb_nil_value();
}

static mrb_value
joint_constraint_force(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	const b2Vec2 force = b2Joint_GetConstraintForce(joint->id);
	return b2_vec_to_value(mrb, force);
}

static mrb_value
joint_linear_separation(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	const float separation = b2Joint_GetLinearSeparation(joint->id);
	return mrb_float_value(mrb, separation);
}

static mrb_value
joint_angular_separation(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	const float separation = b2Joint_GetAngularSeparation(joint->id);
	return mrb_float_value(mrb, separation);
}

static mrb_value
joint_set_constraint_tuning(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	/* should be a hash with keys `hertz` and `damping_ratio` */
	mrb_value tuning_value;
	mrb_get_args(mrb, "H", &tuning_value);
	const mrb_value hertz_value
	    = mrb_hash_get(mrb, tuning_value, B2_SYM(hertz));
	const mrb_value damping_ratio_value
	    = mrb_hash_get(mrb, tuning_value, B2_SYM(damping_ratio));
	const float hertz = (float)mrb_float(hertz_value);
	const float damping_ratio = (float)mrb_float(damping_ratio_value);
	b2Joint_SetConstraintTuning(joint->id, hertz, damping_ratio);
	return mrb_nil_value();
}

static mrb_value
joint_constraint_tuning(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	float hertz = 0.0f;
	float damping_ratio = 0.0f;
	b2Joint_GetConstraintTuning(joint->id, &hertz, &damping_ratio);
	const mrb_value out = mrb_hash_new_capa(mrb, 2);
	mrb_hash_set(mrb, out, B2_SYM(hertz), mrb_float_value(mrb, hertz));
	mrb_hash_set(mrb, out, B2_SYM(damping_ratio),
	    mrb_float_value(mrb, damping_ratio));
	return out;
}

static mrb_value
joint_set_force_threshold(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	mrb_float value;
	mrb_get_args(mrb, "f", &value);
	b2Joint_SetForceThreshold(joint->id, (float)value);
	return mrb_nil_value();
}

static mrb_value
joint_force_threshold(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	const float threshold = b2Joint_GetForceThreshold(joint->id);
	return mrb_float_value(mrb, threshold);
}

static mrb_value
joint_set_torque_threshold(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	mrb_float value;
	mrb_get_args(mrb, "f", &value);
	b2Joint_SetTorqueThreshold(joint->id, (float)value);
	return mrb_nil_value();
}

static mrb_value
joint_torque_threshold(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_joint *joint = box2d_joint_unwrap(mrb, self);
	const float threshold = b2Joint_GetTorqueThreshold(joint->id);
	return mrb_float_value(mrb, threshold);
}

struct RClass *
box2d_joint_init(mrb_state *mrb, struct RClass *mod)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *joint
	    = mrb_define_class_under(mrb, mod, "Joint", mrb->object_class);
	MRB_SET_INSTANCE_TT(joint, MRB_TT_CDATA);
	mrb_define_method(mrb, joint, "type", joint_type, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "body_a", joint_body_a, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "body_b", joint_body_b, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "world", joint_world, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "local_frame_a=", joint_set_local_frame_a,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "local_frame_b=", joint_set_local_frame_b,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "local_frame_a", joint_local_frame_a,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "local_frame_b", joint_local_frame_b,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "collide_connected=", joint_set_collide_connected, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "collide_connected",
	    joint_collide_connected, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "wake_bodies", joint_wake_bodies,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "constraint_force",
	    joint_constraint_force, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "linear_separation",
	    joint_linear_separation, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint, "angular_separation",
	    joint_angular_separation, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "constraint_tuning=", joint_set_constraint_tuning, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "constraint_tuning",
	    joint_constraint_tuning, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "force_threshold=", joint_set_force_threshold, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "force_threshold", joint_force_threshold,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, joint,
	    "torque_threshold=", joint_set_torque_threshold, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, joint, "torque_threshold",
	    joint_torque_threshold, MRB_ARGS_REQ(0));
	return joint;
}