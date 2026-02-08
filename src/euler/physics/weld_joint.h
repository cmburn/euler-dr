/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_WELD_JOINT_H
#define EULER_PHYSICS_WELD_JOINT_H

#include "box2d/box2d.h"
#include "euler/physics/joint.h"
#include "euler/util/object.h"

namespace euler::physics {
class WeldJoint final : public Joint {
	BIND_MRUBY("Euler::Physics::WeldJoint", WeldJoint, physics.weld_joint);
	friend class Joint;
	WeldJoint(b2JointId id)
	    : Joint(id)
	{
	}

public:
	Type
	type() const override
	{
		return Type::Weld;
	}

	void set_linear_hertz(float hertz);
	float linear_hertz() const;
	void set_linear_damping_ratio(float ratio);
	float linear_damping_ratio() const;
	void set_angular_hertz(float hertz);
	float angular_hertz() const;
	void set_angular_damping_ratio(float ratio);
	float angular_damping_ratio() const;
	mrb_value wrap(const util::Reference<util::State> &state) override;
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_WELD_JOINT_H */
