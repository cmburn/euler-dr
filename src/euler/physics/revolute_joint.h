/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_REVOLUTE_JOINT_H
#define EULER_PHYSICS_REVOLUTE_JOINT_H

#include "euler/physics/joint.h"
#include "euler/util/object.h"

namespace euler::physics {
class RevoluteJoint final : public Joint {
	BIND_MRUBY("Euler::Physics::RevoluteJoint", RevoluteJoint,
	    physics.revolute_joint);
	friend class Joint;
	RevoluteJoint(b2JointId id)
	    : Joint(id)
	{
	}

public:
	Type
	type() const override
	{
		return Type::Revolute;
	}
	void enable_spring(bool flag);
	bool is_spring_enabled() const;
	void set_spring_hertz(float hertz);
	float spring_hertz() const;
	void set_spring_damping_ratio(float ratio);
	float spring_damping_ratio() const;
	void set_target_angle(float angle);
	float target_angle() const;
	float angle() const;
	void enable_limit(bool flag);
	bool is_limit_enabled() const;
	void set_limits(float lower, float upper);
	std::pair<float, float> limits() const;
	void enable_motor(bool flag);
	bool is_motor_enabled() const;
	void set_motor_speed(float speed);
	float motor_speed() const;
	float motor_torque() const;
	void set_max_motor_torque(float torque);
	float max_motor_torque() const;
	mrb_value wrap(const util::Reference<util::State> &state) override;
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_REVOLUTE_JOINT_H */
