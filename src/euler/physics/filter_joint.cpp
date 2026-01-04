/* SPDX-License-Identifier: ISC */

#include "euler/physics/filter_joint.h"

#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/hash.h>

#include "euler/physics/body.h"
#include "euler/physics/joint.h"
#include "euler/physics/util.h"
#include "euler/physics/world.h"

struct RClass *
box2d_filter_joint_init(mrb_state *mrb, struct RClass *mod,
    struct RClass *super)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *joint
	    = mrb_define_class_under(mrb, mod, "FilterJoint", super);
	return joint;
}
