/* SPDX-License-Identifier: ISC */

#include "euler/physics/ext.h"

#include <box2d/box2d.h>

#include "euler/physics/body.h"
#include "euler/physics/chain.h"
#include "euler/physics/contact.h"
#include "euler/physics/distance_joint.h"
#include "euler/physics/filter_joint.h"
#include "euler/physics/joint.h"
#include "euler/physics/motor_joint.h"
#include "euler/physics/prismatic_joint.h"
#include "euler/physics/revolute_joint.h"
#include "euler/physics/shape.h"
#include "euler/physics/util.h"
#include "euler/physics/weld_joint.h"
#include "euler/physics/wheel_joint.h"
#include "euler/physics/world.h"
#include "euler/util/state.h"

RClass *
euler::physics::init(const util::Reference<util::State> &state, RClass *outer)
{
	const auto mod = state->mrb()->define_module_under(outer, "Physics");
	auto &physics = state->modules().physics;
	physics.body = Body::init(state, mod);
	physics.chain = Chain::init(state, mod);
	physics.contact = Contact::init(state, mod);
	const auto joint = Joint::init(state, mod);
	physics.joint = joint;
	physics.distance_joint = DistanceJoint::init(state, mod, joint);
	physics.filter_joint = FilterJoint::init(state, mod, joint);
	physics.motor_joint = MotorJoint::init(state, mod, joint);
	physics.prismatic_joint = PrismaticJoint::init(state, mod, joint);
	physics.revolute_joint = RevoluteJoint::init(state, mod, joint);
	physics.weld_joint = WeldJoint::init(state, mod, joint);
	physics.wheel_joint = WheelJoint::init(state, mod, joint);
	physics.world = World::init(state, mod);
	return mod;
}