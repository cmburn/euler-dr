/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_JOINT_H
#define EULER_PHYSICS_JOINT_H

#include <box2d/id.h>
#include <box2d/math_functions.h>

#include "euler/util/object.h"

namespace euler::physics {
class Body;
class World;

class Joint : public util::Object {
public:
	static constexpr mrb_data_type TYPE = {
		.struct_name = "Euler::Physics::Joint",
		.dfree =
		    [](mrb_state *, void *ptr) {
			    const auto ref
				= util::Reference<Joint>::unwrap(ptr);
			    ref.decrement();
		    },
	};

	enum class Type {
		Distance,
		Filter,
		Motor,
		Prismatic,
		Revolute,
		Weld,
		Wheel,
	};

	virtual Type type() const = 0;

	mrb_value wrap(const util::Reference<util::State> &state);

	Joint(b2JointId id)
	    : _id(id)
	{
	}

	util::Reference<Body> body_a() const;
	util::Reference<Body> body_b() const;
	util::Reference<World> world() const;
	void set_local_frame_a(b2Transform transform);
	void set_local_frame_b(b2Transform transform);
	b2Transform local_frame_a() const;
	b2Transform local_frame_b() const;
	void set_collide_connected(bool flag);
	bool collide_connected() const;
	void wake_bodies() const;
	b2Vec2 constraint_force() const;
	float linear_separation() const;
	float angular_separation() const;
	void set_constraint_tuning(float hertz, float damping_ratio);
	std::pair<float, float> constraint_tuning() const;
	void set_force_threshold(float threshold);
	float force_threshold() const;
	void set_torque_threshold(float threshold);
	float torque_threshold() const;

	static util::Reference<Joint> from_id(b2JointId id);

protected:
	b2JointId _id;
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_JOINT_H */
