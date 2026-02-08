/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_MOTOR_JOINT_H
#define EULER_PHYSICS_MOTOR_JOINT_H

#include "box2d/box2d.h"
#include "euler/physics/joint.h"
#include "euler/util/object.h"

namespace euler::physics {
class MotorJoint final : public Joint {
	friend class Joint;
	BIND_MRUBY("Euler::Physics::MotorJoint", MotorJoint,
	    physics.motor_joint);
	MotorJoint(b2JointId id)
	    : Joint(id)
	{
	}

public:
	Type
	type() const override
	{
		return Type::Motor;
	}

	b2Vec2 linear_velocity() const;
	void set_linear_velocity(b2Vec2 velocity);
	float angular_velocity() const;
	void set_angular_velocity(float velocity);
	float max_velocity_force() const;
	void set_max_velocity_force(float force);
	float max_velocity_torque() const;
	void set_max_velocity_torque(float torque);
	float linear_hertz() const;
	void set_linear_hertz(float hertz);
	float linear_damping_ratio() const;
	void set_linear_damping_ratio(float ratio);
	float angular_hertz() const;
	void set_angular_hertz(float hertz);
	float angular_damping_ratio() const;
	void set_angular_damping_ratio(float ratio);
	float max_spring_force() const;
	void set_max_spring_force(float force);
	float max_spring_torque() const;
	void set_max_spring_torque(float torque);
	mrb_value wrap(const util::Reference<util::State> &state) override;
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_MOTOR_JOINT_H */
