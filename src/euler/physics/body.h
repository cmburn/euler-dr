/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_BODY_H
#define EULER_PHYSICS_BODY_H

#include <vector>

#include <box2d/box2d.h>
#include <box2d/id.h>

#include "euler/util/ext.h"
#include "euler/util/object.h"

namespace euler::physics {
class Joint;
class Shape;
class World;

class Body : public util::Object {
	BIND_MRUBY("Euler::Physics::Body", Body, physics.body);
	Body(b2BodyId id)
	    : _id(id)
	{
	}
public:
	static util::Reference<Body> wrap(b2BodyId id);
	static b2BodyType parse_type(mrb_state *mrb, mrb_value);


	b2BodyId
	id() const
	{
		return _id;
	}

	struct MoveEvent {
		b2Transform transform;
		util::Reference<Body> body;
		bool fell_asleep;
		mrb_value wrap(mrb_state *mrb);
		static MoveEvent from_b2(const b2BodyMoveEvent &event);
	};

	float angular_damping();
	float angular_velocity();
	void apply_angular_impulse(float value, bool wake = false);
	void apply_force(b2Vec2 force, b2Vec2 point, bool wake = false);
	void apply_force_to_center(b2Vec2 force, bool wake = false);
	void apply_linear_impulse(b2Vec2 impulse, b2Vec2 point,
	    bool wake = false);
	void apply_linear_impulse_to_center(b2Vec2 impulse, bool wake = false);
	void apply_mass_from_shapes();
	void apply_torque(float torque, bool wake = false);
	bool is_awake();
	void clear_forces();
	b2AABB compute_aabb();
	std::vector<b2ContactData> contact_data();
	void disable();
	void enable();
	void enable_contact_events(bool enable);
	void enable_hit_events(bool enable);
	float gravity_scale();
	bool is_bullet();
	bool is_enabled();
	bool is_sleep_enabled();
	bool is_valid();
	std::vector<util::Reference<Joint>> joints();
	float linear_damping();
	b2Vec2 linear_velocity();
	b2Vec2 local_center_of_mass();
	b2Vec2 local_point(b2Vec2 point);
	b2Vec2 local_point_velocity(b2Vec2 point);
	b2Vec2 local_vector(b2Vec2 vector);
	float mass();
	b2MassData mass_data();
	b2MotionLocks motion_locks();
	const char *name();
	b2Vec2 position();
	b2Rot rotation();
	float rotational_inertia();
	void set_angular_damping(float value);
	void set_angular_velocity(float value);
	void set_awake(bool value);
	void set_bullet(bool value);
	void set_gravity_scale(float value);
	void set_linear_damping(float value);
	void set_linear_velocity(b2Vec2 v);
	void set_mass_data(b2MassData md);
	void set_motion_locks(b2MotionLocks locks);
	void set_name(const char *name);
	void set_sleep_threshold(float value);
	void set_transform(b2Vec2 position, b2Rot rotation);
	void set_type(b2BodyType type);
	std::vector<util::Reference<Shape>> shapes();
	void set_sleep_enabled(bool enabled);
	float sleep_threshold();
	void target_transform(b2Transform tform, float step = 1.0,
	    bool wake = false);
	b2Transform transform();
	b2BodyType type();
	void wake_touching();
	util::Reference<World> world();
	b2Vec2 world_center_of_mass();
	b2Vec2 world_point(b2Vec2);
	b2Vec2 world_point_velocity(b2Vec2 point);
	b2Vec2 world_vector(b2Vec2 vector);

	static util::Reference<Body> unwrap(mrb_state *mrb, mrb_value self);

private:
	b2BodyId _id;
};

} /* namespace euler::physics */

#endif /* EULER_PHYSICS_BODY_H */
