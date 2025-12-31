/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_DISTANCE_JOINT_H
#define EULER_PHYSICS_DISTANCE_JOINT_H

#include "euler/physics/joint.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"

namespace euler::physics {
class DistanceJoint final : public Joint {
public:
	static constexpr auto TYPE
	    = util::datatype<DistanceJoint>("Euler::Physics::DistanceJoint");

	void set_length(float length);
	float length() const;
	void enable_spring(bool flag);
	bool is_spring_enabled() const;
	void set_spring_force_range(float min, float max);
	std::pair<float, float> spring_force_range() const;
	void set_spring_hertz(float hertz);
	float spring_hertz() const;
	void set_spring_damping_ratio(float ratio);
	float spring_damping_ratio() const;
	void enable_limit(bool flag);
	bool is_limit_enabled() const;
	void set_length_range(float min, float max);
	std::pair<float, float> length_range() const;
	float current_length() const;
	void enable_motor(bool flag);
	bool is_motor_enabled() const;
	void set_motor_speed(float speed);
	float motor_speed() const;
	void set_max_motor_force(float force);
	float max_motor_force() const;
	float motor_force() const;
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_DISTANCE_JOINT_H */
