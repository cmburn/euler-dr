/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_SHAPE_H
#define EULER_PHYSICS_SHAPE_H

#include <variant>
#include <vector>

#include <box2d/box2d.h>
#include <box2d/id.h>

#include "euler/physics/contact.h"
#include "euler/physics/ext.h"
#include "euler/physics/util.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"

namespace euler::physics {
class Body;
class World;
class Chain;

class Shape final : public util::Object {
	friend class Chain;
	friend class Body;
	BIND_MRUBY("Euler::Physics::Shape", Shape, physics.shape);

	Shape(b2ShapeId shape)
	    : _id(shape)
	{
	}

public:
	typedef std::variant<b2Circle, b2Capsule, b2Segment, b2Polygon,
	    b2ChainSegment>
	    ShapeData;

	static util::Reference<Shape> wrap(b2ShapeId id);

	static mrb_value wrap_shape_data(mrb_state *mrb,
	    const ShapeData &shape);
	static ShapeData shape_data(mrb_state *mrb, mrb_value val);

	enum class Type {
		Circle,
		Capsule,
		Segment,
		Polygon,
		ChainSegment,
	};

	struct SensorEvent {
		util::Reference<Shape> sensor;
		util::Reference<Shape> visitor;
		mrb_value wrap(mrb_state *mrb);
	};

	struct SensorEvents {
		std::vector<SensorEvent> start;
		std::vector<SensorEvent> end;
		static SensorEvents from_b2(const b2SensorEvents &events);
		mrb_value wrap(mrb_state *mrb);
	};

	static mrb_value type_to_symbol(mrb_state *mrb, Type type);

	ShapeData underlying_shape() const;
	void set_underlying_shape(const ShapeData &data);
	util::Reference<Body> body() const;
	util::Reference<World> world() const;
	b2ShapeId id() const;
	bool is_valid() const;
	Type type() const;
	bool is_sensor() const;
	void set_density(float density);
	float density() const;
	void set_friction(float friction);
	float friction() const;
	void set_restitution(float restitution);
	float restitution() const;
	void set_user_material(uint64_t material_id);
	uint64_t user_material() const;
	b2SurfaceMaterial surface_material() const;
	void set_surface_material(const b2SurfaceMaterial *material);
	b2Filter filter() const;
	void set_filter(const b2Filter &filter);
	void enable_sensor_events(bool flag);
	bool are_sensor_events_enabled() const;
	void enable_contact_events(bool flag);
	bool are_contact_events_enabled() const;
	void enable_pre_solve_events(bool flag);
	bool are_pre_solve_events_enabled() const;
	void enable_hit_events(bool flag);
	bool are_hit_events_enabled() const;
	bool test_point(const b2Vec2 &point) const;
	b2CastOutput ray_cast(const b2RayCastInput &input) const;
	util::Reference<Chain> parent_chain() const;
	std::vector<Contact::Data> contact_data() const;
	std::vector<util::Reference<Shape>> sensors() const;
	b2AABB aabb() const;
	b2MassData compute_mass_data() const;
	b2Vec2 closest_point(const b2Vec2 &point) const;
	void apply_wind(const b2Vec2 &wind, float drag, float lift, bool wake);

private:
	b2ShapeId _id;
};

mrb_value wrap_segment(mrb_state *mrb, const b2Segment &segment);

} /* namespace euler::physics */

#endif /* EULER_PHYSICS_SHAPE_H */
