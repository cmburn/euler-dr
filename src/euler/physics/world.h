/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_WORLD_H
#define EULER_PHYSICS_WORLD_H

#include <functional>
#include <vector>
#include <box2d/box2d.h>
#include <box2d/id.h>
#include <box2d/types.h>

#include "euler/physics/body.h"
#include "euler/physics/chain.h"
#include "euler/physics/contact.h"
#include "euler/physics/joint.h"
#include "euler/physics/shape.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"

mrb_value world_allocate(mrb_state *mrb, mrb_value);
mrb_value world_initialize(mrb_state *mrb, mrb_value);

namespace euler::physics {
class World final : public util::Object {
	friend mrb_value(::world_allocate)(mrb_state *mrb, mrb_value);
	friend mrb_value(::world_initialize)(mrb_state *mrb, mrb_value);
	BIND_MRUBY("Euler::Physics::World", World, physics.world);
	static bool overlap_result_fn(b2ShapeId shape_id, void *user_data);
	static float cast_result_fn(b2ShapeId shape_id, b2Vec2 point,
	    b2Vec2 normal, float fraction, void *context);
	static bool plane_result_fn(b2ShapeId shape_id,
	    const b2PlaneResult *plane, void *context);
	static bool custom_filter_fn(b2ShapeId shape_a_id, b2ShapeId shape_b_id,
	    void *context);
	static bool pre_solve_fn(b2ShapeId shape_a_id, b2ShapeId shape_b_id,
	    b2Vec2 point, b2Vec2 normal, void *context);
	friend class Chain;
	friend class Contact;

	World(b2WorldId id)
	    : _id(id)
	{
	}

	World()
	    : _id(B2_NULL_ID)
	{
	}

	void initialize(const b2WorldDef &);

public:
	typedef std::function<bool(util::Reference<Shape> &)> OverlapFn;
	typedef std::function<float(util::Reference<Shape> &, b2Vec2, b2Vec2,
	    float)>
	    CastResultFn;
	typedef std::function<bool(util::Reference<Shape> &,
	    const b2PlaneResult *)>
	    PlaneResultFn;
	typedef std::function<bool(util::Reference<Shape> &,
	    util::Reference<Shape> &)>
	    CustomFilterFn;
	typedef std::function<bool(util::Reference<Shape> &,
	    util::Reference<Shape> &, b2Vec2, b2Vec2)>
	    PreSolveFn;

	static util::Reference<World> wrap(b2WorldId id);
	static util::Reference<util::State> fetch_state(b2WorldId id);

	mrb_value wrap(const util::Reference<util::State> &state);
	~World() override;

	[[nodiscard]] b2WorldId
	id() const
	{
		return _id;
	}

	bool is_valid() const;

	void step(float dt, int substep_count = 4);

	[[nodiscard]] std::vector<Body::MoveEvent> body_events() const;

	Shape::SensorEvents sensor_events() const;

	Contact::Events contact_events() const;
	Joint::Events joint_events() const;

	b2TreeStats overlap_aabb(const b2AABB &aabb, const b2QueryFilter filter,
	    OverlapFn callback) const;

	b2TreeStats overlap_aabb(const b2AABB &aabb, OverlapFn callback) const;

	b2TreeStats overlap_shape(const b2ShapeProxy &proxy,
	    const b2QueryFilter filter, OverlapFn callback) const;

	b2TreeStats overlap_shape(const b2ShapeProxy &proxy,
	    OverlapFn callback) const;

	b2TreeStats cast_ray(b2Vec2 point, b2Vec2 normal, b2QueryFilter filter,
	    CastResultFn callback) const;

	b2TreeStats cast_ray(b2Vec2 point, b2Vec2 normal,
	    CastResultFn callback) const;

	b2RayResult cast_ray_closest(b2Vec2 origin, b2Vec2 translation,
	    b2QueryFilter filter = b2DefaultQueryFilter());

	b2TreeStats cast_shape(const b2ShapeProxy &proxy, b2Vec2 translation,
	    b2QueryFilter filter, CastResultFn callback) const;

	b2TreeStats cast_shape(const b2ShapeProxy &proxy, b2Vec2 translation,
	    CastResultFn callback) const;

	float cast_mover(const b2Capsule *mover, b2Vec2 translation,
	    const b2QueryFilter filter = b2DefaultQueryFilter()) const;

	void collide_mover(const b2Capsule *mover, const b2QueryFilter filter,
	    PlaneResultFn callback) const;

	void collide_mover(const b2Capsule *mover,
	    PlaneResultFn callback) const;

	void enable_sleeping(bool enable);
	bool is_sleeping_enabled() const;
	void enable_continuous(bool enable);
	bool is_continuous_enabled() const;
	void set_restitution_threshold(float threshold);
	float restitution_threshold();
	void set_hit_event_threshold(float threshold);
	float hit_event_threshold();
	void set_custom_filter(CustomFilterFn callback);
	void set_pre_solve(PreSolveFn callback);
	void set_gravity(const b2Vec2 &gravity);
	b2Vec2 gravity() const;
	void explode(const b2ExplosionDef &def);
	void set_contact_tuning(float hertz, float damping_ratio,
	    float push_speed);
	void set_maximum_linear_speed(float speed);
	float maximum_linear_speed() const;
	int awake_body_count() const;
	util::Reference<Body> create_body(const b2BodyDef &def) const;
	void set_custom_filter(mrb_state *mrb, mrb_value block);
	void set_pre_solve(mrb_state *mrb, mrb_value block);
	util::Reference<util::State> state() const;

private:
	util::Reference<Chain> wrap_chain(const b2ChainId &id);
	util::Reference<Contact> wrap_contact(const b2ContactId &id);
	void drop_chain(const b2ChainId &id);
	void drop_contact(const b2ContactId &id);

	b2WorldId _id;
	util::WeakReference<util::State> _state;

	/*
	 * To avoid creating multiple ruby values per object, the Box2D objects
	 * that don't have a user data field have to be cached here.
	 */
	std::unordered_map<Chain::Key, util::WeakReference<Chain>> _chains;
	std::unordered_map<Contact::Key, util::WeakReference<Contact>>
	    _contacts;
	mrb_value _custom_filter_block = mrb_nil_value();
	mrb_value _pre_solve_block = mrb_nil_value();
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_WORLD_H */
