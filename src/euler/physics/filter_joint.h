/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_FILTER_JOINT_H
#define EULER_PHYSICS_FILTER_JOINT_H

#include "euler/physics/joint.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"

namespace euler::physics {
/*
 * has no special properties, but is a concrete instance of Joint
 */
class FilterJoint final : public Joint {
	friend class Joint;
	BIND_MRUBY("Euler::Physics::FilterJoint", FilterJoint,
	    physics.filter_joint);
	FilterJoint(b2JointId id)
	    : Joint(id)
	{
	}

public:
	Type
	type() const override
	{
		return Type::Filter;
	}
	mrb_value wrap(const util::Reference<util::State> &state) override;
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_FILTER_JOINT_H */
