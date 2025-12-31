/* SPDX-License-Identifier: ISC */

#include "euler/physics/shape.h"

/**
 * @overload Euler::Physics::Shape#valid?
 *   @return [Boolean] true if the shape is valid
 */
static mrb_value
shape_is_valid(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const bool valid = b2Shape_IsValid(shape->id);
	return mrb_bool_value(valid);
}

/**
 * @overload Euler::Physics::Shape#type
 *   @return [Symbol] the shape type (:circle, :capsule, :segment, :polygon,
 *           :chain_segment)
 */
static mrb_value
shape_type(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	switch (b2Shape_GetType(shape->id)) {
	case b2_circleShape: return B2_SYM(circle);
	case b2_capsuleShape: return B2_SYM(capsule);
	case b2_segmentShape: return B2_SYM(segment);
	case b2_polygonShape: return B2_SYM(polygon);
	case b2_chainSegmentShape: return B2_SYM(chain_segment);
	default: mrb_raise(mrb, E_ARGUMENT_ERROR, "unknown shape type");
	}
}

/**
 * @overload Euler::Physics::Shape#body
 *   @return [Euler::Physics::Body] the body this shape is attached to
 */
static mrb_value
shape_body(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const b2BodyId body_id = b2Shape_GetBody(shape->id);
	return box2d_body_wrap(mrb, body_id);
}

/**
 * @overload Euler::Physics::Shape#world
 *   @return [Euler::Physics::World] the world this shape belongs to
 */
static mrb_value
shape_world(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const b2WorldId world_id = b2Shape_GetWorld(shape->id);
	return box2d_world_wrap(mrb, world_id);
}

/**
 * @overload Euler::Physics::Shape#sensor?
 *   @return [Boolean] true if the shape is a sensor
 */
static mrb_value
shape_is_sensor(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const bool is_sensor = b2Shape_IsSensor(shape->id);
	return mrb_bool_value(is_sensor);
}

/**
 * @overload Euler::Physics::Shape#density=(density)
 *   @param [Float] density the density in kg/m^2
 *   @return [void]
 */
static mrb_value
shape_set_density(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_float density;
	mrb_get_args(mrb, "f", &density);
	b2Shape_SetDensity(shape->id, (float)density, true);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Shape#density
 *   @return [Float] the density in kg/m^2
 */
static mrb_value
shape_density(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const float density = b2Shape_GetDensity(shape->id);
	return mrb_float_value(mrb, density);
}

/**
 * @overload Euler::Physics::Shape#friction=(friction)
 *   @param [Float] friction the friction coefficient
 *   @return [void]
 */
static mrb_value
shape_set_friction(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_float friction;
	mrb_get_args(mrb, "f", &friction);
	b2Shape_SetFriction(shape->id, (float)friction);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Shape#friction
 *   @return [Float] the friction coefficient
 */
static mrb_value
shape_friction(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const float friction = b2Shape_GetFriction(shape->id);
	return mrb_float_value(mrb, friction);
}

/**
 * @overload Euler::Physics::Shape#restitution=(restitution)
 *   @param [Float] restitution the restitution (bounciness) coefficient
 *   @return [void]
 */
static mrb_value
shape_set_restitution(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_float restitution;
	mrb_get_args(mrb, "f", &restitution);
	b2Shape_SetRestitution(shape->id, (float)restitution);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Shape#restitution
 *   @return [Float] the restitution (bounciness) coefficient
 */
static mrb_value
shape_restitution(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const float restitution = b2Shape_GetRestitution(shape->id);
	return mrb_float_value(mrb, restitution);
}

/**
 * @overload Euler::Physics::Shape#user_material=(material)
 *   @param [Integer] material the user material identifier
 *   @return [void]
 */
static mrb_value
shape_set_user_material(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_int material;
	mrb_get_args(mrb, "i", &material);
	b2Shape_SetUserMaterial(shape->id, (uint64_t)material);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Shape#user_material
 *   @return [Integer] the user material identifier
 */
static mrb_value
shape_user_material(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const uint64_t material = b2Shape_GetUserMaterial(shape->id);
	return mrb_fixnum_value((mrb_int)material);
}

/**
 * @overload Euler::Physics::Shape#surface_material
 *   @return [Hash] the surface material
 */
static mrb_value
shape_surface_material(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const b2SurfaceMaterial material
	    = b2Shape_GetSurfaceMaterial(shape->id);
	return surface_material_to_value(mrb, &material);
}

/**
 * @overload Euler::Physics::Shape#surface_material=(material)
 *   @param [Hash] material the surface material
 *   @return [void]
 */
static mrb_value
shape_set_surface_material(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_value hash;
	mrb_get_args(mrb, "H", &hash);
	const b2SurfaceMaterial material = value_to_surface_material(mrb, hash);
	b2Shape_SetSurfaceMaterial(shape->id, &material);
	return mrb_nil_value();
}

static b2Filter
hash_to_filter(mrb_state *mrb, mrb_value hash)
{
	const auto state = euler::util::State::get(mrb);
	b2Filter filter = b2DefaultFilter();
	if (mrb_hash_key_p(mrb, hash, B2_SYM(category_bits))) {
		filter.categoryBits = (uint16_t)hash_read_int(mrb, hash,
		    B2_SYM(category_bits), filter.categoryBits);
	}
	if (mrb_hash_key_p(mrb, hash, B2_SYM(mask_bits))) {
		filter.maskBits = (uint16_t)hash_read_int(mrb, hash,
		    B2_SYM(mask_bits), filter.maskBits);
	}
	if (mrb_hash_key_p(mrb, hash, B2_SYM(group_index))) {
		filter.groupIndex = (int16_t)hash_read_int(mrb, hash,
		    B2_SYM(group_index), filter.groupIndex);
	}
	return filter;
}

static mrb_value
filter_to_hash(mrb_state *mrb, const b2Filter *filter)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_value hash = mrb_hash_new_capa(mrb, 3);
	mrb_hash_set(mrb, hash, B2_SYM(category_bits),
	    mrb_fixnum_value((mrb_int)filter->categoryBits));
	mrb_hash_set(mrb, hash, B2_SYM(mask_bits),
	    mrb_fixnum_value((mrb_int)filter->maskBits));
	mrb_hash_set(mrb, hash, B2_SYM(group_index),
	    mrb_fixnum_value((mrb_int)filter->groupIndex));
	return hash;
}

/**
 * @overload Euler::Physics::Shape#filter
 *   @return [Hash] the shape filter
 */
static mrb_value
shape_filter(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const b2Filter filter = b2Shape_GetFilter(shape->id);
	return filter_to_hash(mrb, &filter);
}

static mrb_value
shape_set_filter(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_value hash;
	mrb_get_args(mrb, "H", &hash);
	const b2Filter filter = hash_to_filter(mrb, hash);
	b2Shape_SetFilter(shape->id, filter);
	return mrb_nil_value();
}

static mrb_value
shape_enable_sensor_events(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_bool enable;
	mrb_get_args(mrb, "b", &enable);
	b2Shape_EnableSensorEvents(shape->id, enable);
	return mrb_nil_value();
}

static mrb_value
shape_are_sensor_events_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const bool enabled = b2Shape_AreSensorEventsEnabled(shape->id);
	return mrb_bool_value(enabled);
}

static mrb_value
shape_enable_contact_events(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_bool enable;
	mrb_get_args(mrb, "b", &enable);
	b2Shape_EnableContactEvents(shape->id, enable);
	return mrb_nil_value();
}

static mrb_value
shape_are_contact_events_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const bool enabled = b2Shape_AreContactEventsEnabled(shape->id);
	return mrb_bool_value(enabled);
}

static mrb_value
shape_enable_pre_solve_events(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_bool enable;
	mrb_get_args(mrb, "b", &enable);
	b2Shape_EnablePreSolveEvents(shape->id, enable);
	return mrb_nil_value();
}

static mrb_value
shape_are_pre_solve_events_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const bool enabled = b2Shape_ArePreSolveEventsEnabled(shape->id);
	return mrb_bool_value(enabled);
}

static mrb_value
shape_enable_hit_events(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_bool enable;
	mrb_get_args(mrb, "b", &enable);
	b2Shape_EnableHitEvents(shape->id, enable);
	return mrb_nil_value();
}

static mrb_value
shape_are_hit_events_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const bool enabled = b2Shape_AreHitEventsEnabled(shape->id);
	return mrb_bool_value(enabled);
}

static mrb_value
shape_test_point(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_value point_val;
	mrb_get_args(mrb, "o", &point_val);
	const b2Vec2 point = value_to_b2_vec(mrb, point_val);
	const bool inside = b2Shape_TestPoint(shape->id, point);
	return mrb_bool_value(inside);
}

static mrb_value
shape_ray_cast(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_value origin, translation;
	mrb_float max_fraction;
	mrb_get_args(mrb, "oof", &origin, &translation, &max_fraction);
	const b2RayCastInput input = {
		.origin = value_to_b2_vec(mrb, origin),
		.translation = value_to_b2_vec(mrb, translation),
		.maxFraction = (float)max_fraction,
	};
	const b2CastOutput output = b2Shape_RayCast(shape->id, &input);
	const mrb_value result = mrb_hash_new_capa(mrb, 4);
	mrb_hash_set(mrb, result, B2_SYM(normal),
	    b2_vec_to_value(mrb, output.normal));
	mrb_hash_set(mrb, result, B2_SYM(point),
	    b2_vec_to_value(mrb, output.point));
	mrb_hash_set(mrb, result, B2_SYM(fraction),
	    mrb_float_value(mrb, output.fraction));
	mrb_hash_set(mrb, result, B2_SYM(hit), mrb_bool_value(output.hit));
	return result;
}

static mrb_value
wrap_chain_segment(mrb_state *mrb, const b2ChainSegment *cs)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_value out = mrb_hash_new_capa(mrb, 2);
	const mrb_value ghost = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, ghost, b2_vec_to_value(mrb, cs->ghost1));
	mrb_ary_push(mrb, ghost, b2_vec_to_value(mrb, cs->ghost2));
	mrb_hash_set(mrb, out, B2_SYM(ghosts), ghost);
	const mrb_value segment = box2d_segment_wrap(mrb, cs->segment);
	mrb_hash_set(mrb, out, B2_SYM(segment), segment);
	return out;
}

/*
 * selects the correct shape subtype and calls the respective copy method
 * i.e. b2Shape_GetCircle for a circle, etc.
 */
static mrb_value
shape_copy(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	switch (b2Shape_GetType(shape->id)) {
	case b2_circleShape: {
		const b2Circle circle = b2Shape_GetCircle(shape->id);
		return box2d_circle_wrap(mrb, circle);
	}
	case b2_capsuleShape: {
		const b2Capsule capsule = b2Shape_GetCapsule(shape->id);
		return box2d_capsule_wrap(mrb, capsule);
	}
	case b2_segmentShape: {
		const b2Segment segment = b2Shape_GetSegment(shape->id);
		return box2d_segment_wrap(mrb, segment);
	}
	case b2_polygonShape: {
		const b2Polygon polygon = b2Shape_GetPolygon(shape->id);
		return box2d_polygon_wrap(mrb, polygon);
	}
	case b2_chainSegmentShape: {
		const b2ChainSegment chain_segment
		    = b2Shape_GetChainSegment(shape->id);
		return wrap_chain_segment(mrb, &chain_segment);
	}
	default: mrb_raise(mrb, E_ARGUMENT_ERROR, "unknown shape type");
	}
}

static mrb_value
shape_reshape(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_value outline;
	mrb_get_args(mrb, "o", &outline);
	const char *type_str = mrb_obj_classname(mrb, outline);
	if (strcmp(type_str, "Euler::Physics::Circle")) {
		const b2Circle circle = box2d_circle_shape(mrb, outline);
		b2Shape_SetCircle(shape->id, &circle);
		return mrb_nil_value();
	}
	if (strcmp(type_str, "Euler::Physics::Capsule")) {
		const b2Capsule capsule = box2d_capsule_shape(mrb, outline);
		b2Shape_SetCapsule(shape->id, &capsule);
		return mrb_nil_value();
	}
	if (strcmp(type_str, "Euler::Physics::Segment")) {
		const b2Segment segment = box2d_segment_shape(mrb, outline);
		b2Shape_SetSegment(shape->id, &segment);
		return mrb_nil_value();
	}
	if (strcmp(type_str, "Euler::Physics::Polygon")) {
		const b2Polygon polygon = box2d_polygon_shape(mrb, outline);
		b2Shape_SetPolygon(shape->id, &polygon);
		return mrb_nil_value();
	}
	mrb_raisef(mrb, E_ARGUMENT_ERROR, "unsupported outline type: %S",
	    mrb_str_new_cstr(mrb, type_str));
}

static mrb_value
shape_parent_chain(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const b2ChainId chain_id = b2Shape_GetParentChain(shape->id);
	return box2d_chain_wrap(mrb, chain_id);
}

static mrb_value
shape_contacts(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const int capacity = b2Shape_GetContactCapacity(shape->id);
	b2ContactData *contact_data = (b2ContactData *)tmpbuf(mrb,
	    sizeof(b2ContactData) * capacity);
	const int count
	    = b2Shape_GetContactData(shape->id, contact_data, capacity);
	const mrb_value result = mrb_ary_new_capa(mrb, count);
	for (int i = 0; i < count; i++) {
		const mrb_value data
		    = contact_data_to_value(mrb, &contact_data[i]);
		mrb_ary_push(mrb, result, data);
	}
	mrb_free(mrb, contact_data);
	return result;
}

static mrb_value
shape_sensors(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const int capacity = b2Shape_GetSensorCapacity(shape->id);
	b2ShapeId *sensor_ids
	    = (b2ShapeId *)tmpbuf(mrb, sizeof(b2ShapeId) * capacity);
	const int count
	    = b2Shape_GetSensorData(shape->id, sensor_ids, capacity);
	const mrb_value result = mrb_ary_new_capa(mrb, count);
	for (int i = 0; i < count; i++) {
		const mrb_value sensor = box2d_shape_wrap(mrb, sensor_ids[i]);
		mrb_ary_push(mrb, result, sensor);
	}
	mrb_free(mrb, sensor_ids);
	return result;
}

static mrb_value
shape_aabb(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const b2AABB aabb = b2Shape_GetAABB(shape->id);
	const mrb_value upper = b2_vec_to_value(mrb, aabb.upperBound);
	const mrb_value lower = b2_vec_to_value(mrb, aabb.lowerBound);
	const mrb_value out = mrb_hash_new_capa(mrb, 2);
	mrb_hash_set(mrb, out, B2_SYM(upper_bound), upper);
	mrb_hash_set(mrb, out, B2_SYM(lower_bound), lower);
	return out;
}

static mrb_value
shape_compute_mass_data(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	const b2MassData mass_data = b2Shape_ComputeMassData(shape->id);
	const mrb_value out = mrb_hash_new_capa(mrb, 3);
	mrb_hash_set(mrb, out, B2_SYM(mass),
	    mrb_float_value(mrb, mass_data.mass));
	mrb_hash_set(mrb, out, B2_SYM(center),
	    b2_vec_to_value(mrb, mass_data.center));
	mrb_hash_set(mrb, out, B2_SYM(rotational_inertia),
	    mrb_float_value(mrb, mass_data.rotationalInertia));
	return out;
}

static mrb_value
shape_closest_point(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_value point_val;
	mrb_get_args(mrb, "o", &point_val);
	const b2Vec2 point = value_to_b2_vec(mrb, point_val);
	const b2Vec2 closest = b2Shape_GetClosestPoint(shape->id, point);
	return b2_vec_to_value(mrb, closest);
}

static mrb_value
shape_apply_wind(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_shape *shape = box2d_shape_unwrap(mrb, self);
	mrb_value wind_val;
	float drag, lift;
	bool wake;
	mrb_get_args(mrb, "off|b", &wind_val, &drag, &lift, &wake);
	const b2Vec2 wind = value_to_b2_vec(mrb, wind_val);
	b2Shape_ApplyWind(shape->id, wind, (float)drag, (float)lift, wake);
	return mrb_nil_value();
}

struct RClass *
box2d_shape_init(mrb_state *mrb, struct RClass *mod)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *body
	    = mrb_define_class_under(mrb, mod, "Shape", mrb->object_class);
	MRB_SET_INSTANCE_TT(body, MRB_TT_CDATA);
	mrb_define_method(mrb, body, "valid?", shape_is_valid, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "type", shape_type, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "body", shape_body, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "world", shape_world, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "sensor?", shape_is_sensor,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "density=", shape_set_density,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "density", shape_density, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "friction=", shape_set_friction,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "friction", shape_friction,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "restitution=", shape_set_restitution,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "restitution", shape_restitution,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "user_material=", shape_set_user_material,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "user_material", shape_user_material,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "surface_material", shape_surface_material,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body,
	    "surface_material=", shape_set_surface_material, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "filter", shape_filter, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "filter=", shape_set_filter,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body,
	    "sensor_events_enabled=", shape_enable_sensor_events,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "sensor_events_enabled",
	    shape_are_sensor_events_enabled, MRB_ARGS_REQ(0));
	mrb_alias_method(mrb, body,
	    mrb_intern_lit(mrb, "sensor_events_enabled?"),
	    mrb_intern_lit(mrb, "sensor_events_enabled"));
	mrb_define_method(mrb, body,
	    "contact_events_enabled=", shape_enable_contact_events,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "contact_events_enabled",
	    shape_are_contact_events_enabled, MRB_ARGS_REQ(0));
	mrb_alias_method(mrb, body,
	    mrb_intern_lit(mrb, "contact_events_enabled?"),
	    mrb_intern_lit(mrb, "contact_events_enabled"));
	mrb_define_method(mrb, body,
	    "pre_solve_events_enabled=", shape_enable_pre_solve_events,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "pre_solve_events_enabled",
	    shape_are_pre_solve_events_enabled, MRB_ARGS_REQ(0));
	mrb_alias_method(mrb, body,
	    mrb_intern_lit(mrb, "pre_solve_events_enabled?"),
	    mrb_intern_lit(mrb, "pre_solve_events_enabled"));
	mrb_define_method(mrb, body,
	    "hit_events_enabled=", shape_enable_hit_events, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "hit_events_enabled",
	    shape_are_hit_events_enabled, MRB_ARGS_REQ(0));
	mrb_alias_method(mrb, body, mrb_intern_lit(mrb, "hit_events_enabled?"),
	    mrb_intern_lit(mrb, "hit_events_enabled"));
	mrb_define_method(mrb, body, "test_point", shape_test_point,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "ray_cast", shape_ray_cast,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "copy", shape_copy, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "reshape", shape_reshape, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "parent_chain", shape_parent_chain,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "contacts", shape_contacts,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "sensors", shape_sensors, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "aabb", shape_aabb, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "compute_mass_data",
	    shape_compute_mass_data, MRB_ARGS_REQ(0));
	mrb_define_method(mrb, body, "closest_point", shape_closest_point,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, body, "apply_wind", shape_apply_wind,
	    MRB_ARGS_REQ(1));
	return body;
}
