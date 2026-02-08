/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_PRISMATIC_JOINT_H
#define EULER_PHYSICS_PRISMATIC_JOINT_H

#include "euler/physics/joint.h"
#include "euler/util/object.h"

namespace euler::physics {
class PrismaticJoint final : public Joint {
	BIND_MRUBY("Euler::Physics::PrismaticJoint", PrismaticJoint,
	    physics.prismatic_joint);
	friend class Joint;
	PrismaticJoint(b2JointId id)
	    : Joint(id)
	{
	}

public:
	Type
	type() const override
	{
		return Type::Prismatic;
	}
	void enable_spring(bool flag);
	bool is_spring_enabled();
	void set_spring_hertz(float hertz);
	float spring_hertz();
	void set_spring_damping_ratio(float ratio);
	float spring_damping_ratio();
	void set_target_translation(float translation);
	float target_translation();
	void enable_limit(bool flag);
	bool is_limit_enabled();
	void set_limits(float lower, float upper);
	std::pair<float, float> limits();
	void enable_motor(bool flag);
	bool is_motor_enabled();
	void set_motor_speed(float speed);
	float motor_speed();
	void set_max_motor_force(float force);
	float max_motor_force();
	float motor_force();
	float translation();
	float speed();
	mrb_value wrap(const util::Reference<util::State> &state) override;
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_PRISMATIC_JOINT_H */
