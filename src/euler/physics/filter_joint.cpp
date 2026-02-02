/* SPDX-License-Identifier: ISC */

#include "euler/physics/filter_joint.h"

#include <box2d/box2d.h>
#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/hash.h>

#include "euler/physics/body.h"
#include "euler/physics/joint.h"
#include "euler/physics/util.h"
#include "euler/physics/world.h"

using euler::physics::FilterJoint;

RClass *
FilterJoint::init(const euler::util::Reference<euler::util::State> &state,
    RClass *mod, RClass *super)
{
	const auto mrb = state->mrb();
	return mrb->define_class_under(mod, "FilterJoint", super);
}

mrb_value
FilterJoint::wrap(const util::Reference<util::State> &state)
{
	auto self = util::Reference(this);
	return state->wrap(self);
}
