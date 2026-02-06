/* SPDX-License-Identifier: ISC */

#include <cassert>
#include <utility>

#include "euler/physics/shape.h"

#include <box2d/box2d.h>

#include "euler/physics/body.h"
#include "euler/physics/chain.h"
#include "euler/physics/contact.h"
#include "euler/physics/util.h"
#include "euler/physics/world.h"
#include "euler/util/state.h"

using euler::physics::Shape;

/* Capsule, Circle, Polygon, and Segment are all purely ruby, defined via C
 * for convenience */

/* Euler::Physics::Capsule */

/**
 * @overload Euler::Physics::Capsule#radius
 *   @return [Float] the radius of the capsule
 */
static mrb_value
capsule_radius(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	return state->mrb()->iv_get(self, state->mrb()->intern_cstr("@radius"));
}

/**
 * @overload Euler::Physics::Capsule#radius=(radius)
 *   @param radius [Float] the radius to set for the capsule
 *   @return [void]
 */
static mrb_value
capsule_set_radius(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_float radius;
	state->mrb()->get_args("f", &radius);
	const mrb_value value = state->mrb()->float_value(radius);
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@radius"), value);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Capsule#centers
 *   @return [Array&lt;Array&lt;Float&gt;&gt;] The center points of the capsule.
 * Is an array of two points, i.e. [[x1, y1], [x2, y2]]
 */
static mrb_value
capsule_centers(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	return state->mrb()->iv_get(self, state->mrb()->intern_cstr("@center"));
}

/**
 * @overload Euler::Physics::Capsule#centers=(centers)
 *   @param centers [Array&lt;Array&lt;Float&gt;&gt;] The center points to set
 * for the capsule. Must be an array of two points, i.e. [[x1, y1], [x2, y2]].
 *   @return [void]
 */
static mrb_value
capsule_set_centers(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	/* @center is in this case an array of two points,
	 * i.e. [[x1, y1], [x2, y2]] */
	mrb_value center;
	state->mrb()->get_args("A", &center);
	if (RARRAY_LEN(center) != 2) {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    "center must be an array of two points");
	}
	const b2Vec2 center1
	    = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(center, 0));
	const b2Vec2 center2
	    = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(center, 1));
	const mrb_value center_val = state->mrb()->ary_new_capa(2);
	state->mrb()->ary_push(center_val,
	    euler::physics::b2_vec_to_value(mrb, center1));
	state->mrb()->ary_push(center_val,
	    euler::physics::b2_vec_to_value(mrb, center2));
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@center"),
	    center_val);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Capsule#initialize(centers, radius)
 *   @param centers [Array&lt;Array&lt;Float&gt;&gt;] The center points of the
 * capsule. Must be an array of two points, i.e. [[x1, y1], [x2, y2]].
 *   @param radius [Float] The radius of the capsule
 *   @return [void]
 */
static mrb_value
capsule_initialize(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value center;
	mrb_float radius;
	state->mrb()->get_args("Af", &center, &radius);
	if (RARRAY_LEN(center) != 2) {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    "center must be an array of two points");
	}
	const b2Vec2 center1
	    = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(center, 0));
	const b2Vec2 center2
	    = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(center, 1));
	const mrb_value center_val = state->mrb()->ary_new_capa(2);
	state->mrb()->ary_push(center_val,
	    euler::physics::b2_vec_to_value(mrb, center1));
	state->mrb()->ary_push(center_val,
	    euler::physics::b2_vec_to_value(mrb, center2));
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@center"),
	    center_val);
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@radius"),
	    state->mrb()->float_value(radius));
	return mrb_nil_value();
}

RClass *
capsule_init(mrb_state *mrb, RClass *mod)
{
	const auto state = euler::util::State::get(mrb);
	RClass *capsule = state->mrb()->define_class_under(mod, "Capsule",
	    mrb->object_class);
	state->mrb()->define_method(capsule, "radius", capsule_radius,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(capsule, "radius=", capsule_set_radius,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(capsule, "centers", capsule_centers,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(capsule, "centers=", capsule_set_centers,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(capsule, "initialize", capsule_initialize,
	    MRB_ARGS_REQ(2));
	return capsule;
}

mrb_value
capsule_wrap(mrb_state *mrb, b2Capsule capsule)
{
	const auto state = euler::util::State::get(mrb);
	RClass *capsule_class = state->mrb()->class_get_under(
	    state->mrb()->module_get("Euler::Physics"), "Capsule");
	const mrb_value center_val = state->mrb()->ary_new_capa(2);
	state->mrb()->ary_push(center_val,
	    euler::physics::b2_vec_to_value(mrb, capsule.center1));
	state->mrb()->ary_push(center_val,
	    euler::physics::b2_vec_to_value(mrb, capsule.center2));
	const mrb_value radius_val = state->mrb()->float_value(capsule.radius);
	const mrb_value args[] = { center_val, radius_val };
	const mrb_value obj = state->mrb()->obj_new(capsule_class, 2, args);
	return obj;
}

b2Capsule
capsule_shape(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_sym rad_sym = state->mrb()->intern_cstr("@radius");
	const mrb_sym center_sym = state->mrb()->intern_cstr("@center");
	const mrb_value radius_val = state->mrb()->iv_get(self, rad_sym);
	const mrb_value center_val = state->mrb()->iv_get(self, center_sym);
	b2Capsule capsule;
	capsule.radius = (float)mrb_float(radius_val);
	if (RARRAY_LEN(center_val) != 2) {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    "center must be an array of two points");
	}
	capsule.center1 = euler::physics::value_to_b2_vec(mrb,
	    mrb_ary_entry(center_val, 0));
	capsule.center2 = euler::physics::value_to_b2_vec(mrb,
	    mrb_ary_entry(center_val, 1));
	return capsule;
}

/* Euler::Physics::Circle */

/**
 * @overload Box2D::Circle#radius
 *   @return [Float] the radius of the circle
 */
static mrb_value
circle_radius(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	return state->mrb()->iv_get(self, state->mrb()->intern_cstr("@radius"));
}

/**
 * @overload Box2D::Circle#radius=(radius)
 *   @param radius [Float] the radius to set for the circle
 *   @return [void]
 */
static mrb_value
circle_set_radius(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_float radius;
	state->mrb()->get_args("f", &radius);
	const mrb_value value = state->mrb()->float_value(radius);
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@radius"), value);
	return mrb_nil_value();
}

/**
 * @overload Box2D::Circle#center
 *   @return [Array<Numeric>] the center point of the circle
 */
static mrb_value
circle_center(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	return state->mrb()->iv_get(self, state->mrb()->intern_cstr("@center"));
}

/**
 * @overload Box2D::Circle#center=(center)
 *   @param center [Array<Numeric>] the center point to set for the circle
 *   @return [void]
 */
static mrb_value
circle_set_center(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value center;
	state->mrb()->get_args("o", &center);
	/* coerce to/from b2 vec to ensure we're valid */
	const b2Vec2 vec = euler::physics::value_to_b2_vec(mrb, center);
	const mrb_value value = euler::physics::b2_vec_to_value(mrb, vec);
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@center"), value);
	return mrb_nil_value();
}

/**
 * @overload Box2D::Circle#initialize(center, radius)
 *   @param center [Array<Numeric>] the center point of the circle
 *   @param radius [Float] the radius of the circle
 *   @return [void]
 */
static mrb_value
circle_initialize(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value center;
	mrb_float radius;
	state->mrb()->get_args("of", &center, &radius);
	const mrb_value radius_val = state->mrb()->float_value(radius);
	/* coerce to/from b2 vec to ensure we're valid */
	const b2Vec2 vec = euler::physics::value_to_b2_vec(mrb, center);
	const mrb_value center_val = euler::physics::b2_vec_to_value(mrb, vec);
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@radius"),
	    radius_val);
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@center"),
	    center_val);
	return mrb_nil_value();
}

RClass *
circle_init(mrb_state *mrb, RClass *mod)
{
	const auto state = euler::util::State::get(mrb);
	RClass *circle = state->mrb()->define_class_under(mod, "Circle",
	    mrb->object_class);
	state->mrb()->define_method(circle, "radius", circle_radius,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(circle, "radius=", circle_set_radius,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(circle, "center", circle_center,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(circle, "center=", circle_set_center,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(circle, "initialize", circle_initialize,
	    MRB_ARGS_REQ(2));
	return circle;
}

mrb_value
circle_wrap(mrb_state *mrb, b2Circle circle)
{
	const auto state = euler::util::State::get(mrb);
	RClass *circle_class = state->mrb()->class_get_under(
	    state->mrb()->module_get("Euler::Physics"), "Circle");
	const mrb_value radius_val = state->mrb()->float_value(circle.radius);
	const mrb_value center_val
	    = euler::physics::b2_vec_to_value(mrb, circle.center);
	const mrb_value args[] = { radius_val, center_val };
	const mrb_value obj = state->mrb()->obj_new(circle_class, 2, args);
	return obj;
}

b2Circle
circle_shape(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_sym rad_sym = state->mrb()->intern_cstr("@radius");
	const mrb_sym center_sym = state->mrb()->intern_cstr("@center");
	const mrb_value radius_val = state->mrb()->iv_get(self, rad_sym);
	const mrb_value center_val = state->mrb()->iv_get(self, center_sym);
	return b2Circle {
		.center = euler::physics::value_to_b2_vec(mrb, center_val),
		.radius = (float)mrb_float(radius_val),
	};
}

/* Euler::Physics::Polygon */

/**
 * @overload Euler::Physics::Polygon#vertices
 *   Gets the vertices of the polygon.
 *   @return [Array<Array<Numeric>>] Array of vertices, each vertex is an array
 *           of two numeric values [x, y].
 */
static mrb_value
polygon_vertices(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	return state->mrb()->iv_get(self,
	    state->mrb()->intern_cstr("@vertices"));
}

/**
 * @overload Euler::Physics::Polygon#vertices=(vertices)
 *   Sets the vertices of the polygon.
 *   @param [Array<Array<Numeric>>] vertices Array of vertices, each vertex is
 *          an array of two numeric values [x, y]. May not have more than
 *          Euler::Physics::Polygon::MAX_VERTICES entries.
 *   @return [void]
 */
static mrb_value
polygon_set_vertices(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value vertices;
	state->mrb()->get_args("A", &vertices);
	const mrb_int len = RARRAY_LEN(vertices);
	if (len > B2_MAX_POLYGON_VERTICES) {
		state->mrb()->raisef(E_ARGUMENT_ERROR,
		    "Number of vertices exceeds maximum (%d)",
		    B2_MAX_POLYGON_VERTICES);
	}

	const mrb_value arr = state->mrb()->ary_new_capa(len);
	for (mrb_int i = 0; i < len; ++i) {
		const mrb_value value = mrb_ary_entry(vertices, i);
		const b2Vec2 vec = euler::physics::value_to_b2_vec(mrb, value);
		state->mrb()->ary_push(value,
		    euler::physics::b2_vec_to_value(mrb, vec));
	}
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@vertices"), arr);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Polygon::normals
 *   Gets the normal vectors of the polygon sides.
 *   @return [Array<Array<Numeric>>] Array of normal vectors, each normal is
 *           an array of two numeric values [x, y].
 */
static mrb_value
polygon_normals(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	return state->mrb()->iv_get(self,
	    state->mrb()->intern_cstr("@normals"));
}

/**
 * @overload Euler::Physics::Polygon::normals=(normals)
 *   Sets the normal vectors of the polygon sides.
 *   @param [Array<Array<Numeric>>] normals Array of normal vectors, each normal
 *          is an array of two numeric values [x, y]. Must have the same number
 *          of entries as vertices.
 *   @return [void]
 */
static mrb_value
polygon_set_normals(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value normals;
	state->mrb()->get_args("A", &normals);
	const mrb_int len = RARRAY_LEN(normals);
	const mrb_value arr = state->mrb()->ary_new_capa(len);
	for (mrb_int i = 0; i < len; ++i) {
		const mrb_value value = mrb_ary_entry(normals, i);
		const b2Vec2 vec = euler::physics::value_to_b2_vec(mrb, value);
		state->mrb()->ary_push(value,
		    euler::physics::b2_vec_to_value(mrb, vec));
	}
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@normals"), arr);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Polygon#centroid
 *   Gets the centroid of the polygon.
 *   @return [Array<Numeric>] The centroid as an array of two numeric values
 *           [x, y].
 */
static mrb_value
polygon_centroid(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	return state->mrb()->iv_get(self,
	    state->mrb()->intern_cstr("@centroid"));
}

/**
 * @overload Euler::Physics::Polygon#centroid=(centroid)
 *   Sets the centroid of the polygon.
 *   @param [Array<Numeric>] centroid The centroid as an array of two numeric
 *          values [x, y].
 *   @return [void]
 */
static mrb_value
polygon_set_centroid(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value centroid;
	state->mrb()->get_args("A", &centroid);
	const b2Vec2 vec = euler::physics::value_to_b2_vec(mrb, centroid);
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@centroid"),
	    euler::physics::b2_vec_to_value(mrb, vec));
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Polygon#radius
 *   Gets the radius of the polygon for rounded polygons.
 *   @return [Numeric] The radius.
 */
static mrb_value
polygon_radius(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	return state->mrb()->iv_get(self, state->mrb()->intern_cstr("@radius"));
}

/**
 * @overload Euler::Physics::Polygon#radius=(radius)
 *   Sets the radius of the polygon for rounded polygons.
 *   @param [Numeric] radius The radius.
 *   @return [void]
 */
static mrb_value
polygon_set_radius(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_float radius;
	state->mrb()->get_args("f", &radius);
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@radius"),
	    state->mrb()->float_value(radius));
	return mrb_nil_value();
}

static void
read_offset(mrb_state *mrb, mrb_value offset, b2Vec2 *position, b2Rot *rotation)
{
	const auto state = euler::util::State::get(mrb);
	if (!mrb_hash_p(offset) || RARRAY_LEN(offset) != 2) {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    ":offset must be a hash with :position and :rotation keys");
	}
	const auto pos_value = state->mrb()->hash_get(offset, B2_SYM(position));
	const auto rot_value = state->mrb()->hash_get(offset, B2_SYM(rotation));
	*position = euler::physics::value_to_b2_vec(mrb, pos_value);
	*rotation = euler::physics::value_to_b2_rot(mrb, rot_value);
}

static b2Polygon
polygon_initialize_box(mrb_state *mrb, mrb_value box, mrb_value radius,
    mrb_value offset)
{
	const auto state = euler::util::State::get(mrb);
	if (!mrb_array_p(box) || RARRAY_LEN(box) != 2) {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    ":box must be an array of two elements [width, height]");
	}
	const float width_value
	    = euler::physics::coerce_float(mrb, mrb_ary_entry(box, 0));
	const float height_value
	    = euler::physics::coerce_float(mrb, mrb_ary_entry(box, 1));
	const float half_width = width_value / 2.0f;
	const float half_height = height_value / 2.0f;
	if (mrb_nil_p(radius)) {
		if (mrb_nil_p(offset))
			return b2MakeBox(half_width, half_height);
		b2Vec2 position;
		b2Rot rotation;
		read_offset(mrb, offset, &position, &rotation);
		return b2MakeOffsetBox(half_width, half_height, position,
		    rotation);
	}
	const float rad = euler::physics::coerce_float(mrb, radius);
	if (mrb_nil_p(offset))
		return b2MakeRoundedBox(half_width, half_height, rad);
	b2Vec2 position;
	b2Rot rotation;
	read_offset(mrb, offset, &position, &rotation);
	return b2MakeOffsetRoundedBox(half_width, half_height, position,
	    rotation, rad);
}

static b2Hull
value_to_hull(mrb_state *mrb, mrb_value hull)
{
	const auto state = euler::util::State::get(mrb);
	if (mrb_nil_p(hull) || !mrb_array_p(hull)) {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    "Expected an array of points as hull argument");
	}
	const mrb_int len = RARRAY_LEN(hull);
	std::vector<b2Vec2> points;
	points.reserve(len);
	for (mrb_int i = 0; i < len; ++i) {
		const auto entry = mrb_ary_entry(hull, i);
		const auto value = euler::physics::value_to_b2_vec(mrb, entry);
		points.push_back(value);
	}
	return b2ComputeHull(points.data(), points.size());
}

static b2Polygon
polygon_initialize_hull(mrb_state *mrb, mrb_value hull_value, mrb_value radius,
    mrb_value offset)
{
	const auto state = euler::util::State::get(mrb);
	const b2Hull hull = value_to_hull(mrb, hull_value);
	if (mrb_nil_p(radius)) {
		if (mrb_nil_p(offset)) {
			state->mrb()->raise(E_ARGUMENT_ERROR,
			    "For non-box polygons, either :offset or :radius "
			    "must be passed");
		}
		b2Vec2 position;
		b2Rot rotation;
		read_offset(mrb, offset, &position, &rotation);
		return b2MakeOffsetPolygon(&hull, position, rotation);
	}
	if (mrb_nil_p(offset)) {
		return b2MakePolygon(&hull,
		    euler::physics::coerce_float(mrb, radius));
	}
	b2Vec2 position;
	b2Rot rotation;
	read_offset(mrb, offset, &position, &rotation);
	const float rad = euler::physics::coerce_float(mrb, radius);
	return b2MakeOffsetRoundedPolygon(&hull, position, rotation, rad);
}

static b2Polygon
polygon_initialize_raw(mrb_state *mrb, mrb_value hash)
{
	const auto state = euler::util::State::get(mrb);
	b2Polygon polygon;
	auto verts = state->mrb()->hash_get(hash, B2_SYM(vertices));
	assert(RARRAY_LEN(verts) <= B2_MAX_POLYGON_VERTICES);
	for (mrb_int i = 0; i < RARRAY_LEN(verts); ++i) {
		const auto entry = state->mrb()->ary_entry(verts, i);
		const auto vec = euler::physics::value_to_b2_vec(mrb, entry);
		polygon.vertices[i] = vec;
	}
	auto norms = state->mrb()->hash_get(hash, B2_SYM(normals));
	assert(RARRAY_LEN(norms) <= B2_MAX_POLYGON_VERTICES);
	for (mrb_int i = 0; i < RARRAY_LEN(norms); ++i) {
		const auto entry = state->mrb()->ary_entry(norms, i);
		const auto vec = euler::physics::value_to_b2_vec(mrb, entry);
		polygon.normals[i] = vec;
	}
	auto centroid = state->mrb()->hash_get(hash, B2_SYM(centroid));
	polygon.centroid = euler::physics::value_to_b2_vec(mrb, centroid);
	auto radius = state->mrb()->hash_get(hash, B2_SYM(radius));
	polygon.radius = euler::physics::coerce_float(mrb, radius);
	auto count = state->mrb()->hash_get(hash, B2_SYM(count));
	polygon.count = mrb_integer(count);
	return polygon;
}

static b2Polygon
polygon_read_args(mrb_state *mrb)
{
	const auto state = euler::util::State::get(mrb);
	static const char *KW_NAMES[] = {
		"points",
		"box",
		"rounding",
		"offset",
	};
	enum {
		POINTS = 0,
		BOX,
		RADIUS,
		OFFSET,
		KW_COUNT = std::size(KW_NAMES),
	};
	mrb_sym kw_syms[KW_COUNT];
	for (size_t i = 0; i < KW_COUNT; ++i)
		kw_syms[i] = state->mrb()->intern_cstr(KW_NAMES[i]);
	mrb_value kw_values[KW_COUNT];
	const mrb_kwargs kwargs = {
		.num = KW_COUNT,
		.required = 0,
		.table = kw_syms,
		.values = kw_values,
		.rest = NULL,
	};
	mrb_value hash = mrb_nil_value();
	state->mrb()->get_args("|H:", &hash, &kwargs);
	if (!mrb_undef_p(hash)) return polygon_initialize_raw(mrb, hash);
	auto points = mrb_nil_value();
	auto box = mrb_nil_value();
	auto radius = mrb_nil_value();
	auto offset = mrb_nil_value();
	if (!mrb_undef_p(kw_values[POINTS]) && !mrb_undef_p(kw_values[BOX])) {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    "Cannot specify both :points and :box");
	}
	if (!mrb_undef_p(kw_values[POINTS])) {
		points = kw_values[POINTS];
	} else if (!mrb_undef_p(kw_values[BOX])) {
		box = kw_values[BOX];
	} else {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    "Must specify :points or :box");
	}

	if (!mrb_undef_p(kw_values[RADIUS])) radius = kw_values[RADIUS];
	if (!mrb_undef_p(kw_values[OFFSET])) offset = kw_values[OFFSET];
	if (mrb_undef_p(points))
		return polygon_initialize_box(mrb, box, radius, offset);
	return polygon_initialize_hull(mrb, points, radius, offset);
}

/*
 */

static mrb_value
polygon_initialize(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto pg = polygon_read_args(mrb);
	const mrb_value verts = state->mrb()->ary_new_capa(pg.count);
	for (int i = 0; i < pg.count; ++i)
		state->mrb()->ary_push(verts,
		    euler::physics::b2_vec_to_value(mrb, pg.vertices[i]));
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@vertices"),
	    verts);
	return mrb_nil_value();
}

static RClass *
polygon_init(mrb_state *mrb, RClass *mod)
{
	const auto state = euler::util::State::get(mrb);
	RClass *polygon = state->mrb()->define_class_under(mod, "Polygon",
	    mrb->object_class);
	state->mrb()->define_method(polygon, "vertices", polygon_vertices,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(polygon, "vertices=", polygon_set_vertices,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(polygon, "normals", polygon_normals,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(polygon, "normals=", polygon_set_normals,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(polygon, "centroid", polygon_centroid,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(polygon, "centroid=", polygon_set_centroid,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(polygon, "radius", polygon_radius,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(polygon, "radius=", polygon_set_radius,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(polygon, "initialize", polygon_initialize,
	    MRB_ARGS_KEY(1, 0));
	return polygon;
}

static mrb_value
polygon_wrap(mrb_state *mrb, b2Polygon polygon)
{
	const auto state = euler::util::State::get(mrb);
	RClass *polygon_class = state->mrb()->class_get_under(
	    state->mrb()->module_get("Euler::Physics"), "Polygon");
	const mrb_value verts = state->mrb()->ary_new_capa(polygon.count);
	const mrb_value normals = state->mrb()->ary_new_capa(polygon.count);
	for (int i = 0; i < polygon.count; ++i) {
		const auto vert
		    = euler::physics::b2_vec_to_value(mrb, polygon.vertices[i]);
		state->mrb()->ary_push(verts, vert);
		const auto normal
		    = euler::physics::b2_vec_to_value(mrb, polygon.normals[i]);
		state->mrb()->ary_push(normals, normal);
	}
	const mrb_value centroid
	    = euler::physics::b2_vec_to_value(mrb, polygon.centroid);
	const mrb_value radius = state->mrb()->float_value(polygon.radius);
	const mrb_value count = state->mrb()->int_value(polygon.count);
	const mrb_value hash = state->mrb()->hash_new_capa(4);
	state->mrb()->hash_set(hash, B2_SYM(vertices), verts);
	state->mrb()->hash_set(hash, B2_SYM(normals), normals);
	state->mrb()->hash_set(hash, B2_SYM(centroid), centroid);
	state->mrb()->hash_set(hash, B2_SYM(radius), radius);
	state->mrb()->hash_set(hash, B2_SYM(count), count);
	const mrb_value args[] = { hash };
	const mrb_value obj = state->mrb()->obj_new(polygon_class, 1, args);
	return obj;
}

static b2Polygon
polygon_shape(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto hash = state->mrb()->hash_new_capa(4);
	const auto verts = state->mrb()->iv_get(self,
	    state->mrb()->intern_cstr("@vertices"));
	state->mrb()->hash_set(hash, B2_SYM(vertices), verts);
	const auto norms
	    = state->mrb()->iv_get(self, state->mrb()->intern_cstr("@normals"));
	state->mrb()->hash_set(hash, B2_SYM(normals), norms);
	const auto centroid = state->mrb()->iv_get(self,
	    state->mrb()->intern_cstr("@centroid"));
	state->mrb()->hash_set(hash, B2_SYM(centroid), centroid);
	const auto radius
	    = state->mrb()->iv_get(self, state->mrb()->intern_cstr("@radius"));
	state->mrb()->hash_set(hash, B2_SYM(radius), radius);
	const auto count = state->mrb()->int_value(RARRAY_LEN(verts));
	state->mrb()->hash_set(hash, B2_SYM(count), count);
	return polygon_initialize_raw(mrb, hash);
}

static mrb_value
segment_points(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	return state->mrb()->iv_get(self, state->mrb()->intern_cstr("@points"));
}

/**
 * @overload Euler::Physics::Segment#points=(points)
 *   Set the segment's two endpoints.
 *   @param points [Array<Euler::Physics::Vec2>] An array containing two
 * Box2D::Vec2 points.
 *   @return [void]
 */
static mrb_value
segment_set_points(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value points;
	state->mrb()->get_args("A", &points);
	if (RARRAY_LEN(points) != 2) {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    "points must be an array of two points");
	}
	const b2Vec2 p1
	    = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 0));
	const b2Vec2 p2
	    = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 1));
	const mrb_value points_val = state->mrb()->ary_new_capa(2);
	state->mrb()->ary_push(points_val,
	    euler::physics::b2_vec_to_value(mrb, p1));
	state->mrb()->ary_push(points_val,
	    euler::physics::b2_vec_to_value(mrb, p2));
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@points"),
	    points_val);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Segment#initialize(points)
 *   Initialize a new segment with two endpoints.
 *   @param points [Array<Euler::Physics::Vec2>] An array containing two
 * Box2D::Vec2 points.
 */
static mrb_value
segment_initialize(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value points;
	state->mrb()->get_args("A", &points);
	if (RARRAY_LEN(points) != 2) {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    "points must be an array of two points");
	}
	const b2Vec2 p1
	    = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 0));
	const b2Vec2 p2
	    = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 1));
	const mrb_value points_val = state->mrb()->ary_new_capa(2);
	state->mrb()->ary_push(points_val,
	    euler::physics::b2_vec_to_value(mrb, p1));
	state->mrb()->ary_push(points_val,
	    euler::physics::b2_vec_to_value(mrb, p2));
	state->mrb()->iv_set(self, state->mrb()->intern_cstr("@points"),
	    points_val);
	return self;
}

RClass *
segment_init(mrb_state *mrb, RClass *mod)
{
	const auto state = euler::util::State::get(mrb);
	RClass *segment = state->mrb()->define_class_under(mod, "Segment",
	    mrb->object_class);
	state->mrb()->define_method(segment, "points", segment_points,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(segment, "points=", segment_set_points,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(segment, "initialize", segment_initialize,
	    MRB_ARGS_REQ(1));
	return segment;
}

mrb_value
segment_wrap(mrb_state *mrb, const b2Segment segment)
{
	const auto state = euler::util::State::get(mrb);
	RClass *cls = state->mrb()->class_get("Euler::Physics::Segment");
	const mrb_value points = state->mrb()->ary_new_capa(2);
	state->mrb()->ary_push(points,
	    euler::physics::b2_vec_to_value(mrb, segment.point1));
	state->mrb()->ary_push(points,
	    euler::physics::b2_vec_to_value(mrb, segment.point2));
	const mrb_value obj = state->mrb()->obj_new(cls, 1, &points);
	return obj;
}

b2Segment
segment_shape(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_value points
	    = state->mrb()->iv_get(self, state->mrb()->intern_cstr("@points"));
	if (RARRAY_LEN(points) != 2) {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    "points must be an array of two points");
	}
	const b2Vec2 p1
	    = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 0));
	const b2Vec2 p2
	    = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 1));
	return (b2Segment) {
		.point1 = p1,
		.point2 = p2,
	};
}

/* Euler::Physics::Segment */

/**
 * @overload Euler::Physics::Shape#valid?
 *   @return [Boolean] true if the shape is valid
 */
static mrb_value
shape_is_valid(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	const bool valid = shape->is_valid();
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
	const auto shape = state->unwrap<Shape>(self);
	return Shape::type_to_symbol(mrb, shape->type());
}

/**
 * @overload Euler::Physics::Shape#body
 *   @return [Euler::Physics::Body] the body this shape is attached to
 */
static mrb_value
shape_body(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	auto body = shape->body();
	return state->wrap<euler::physics::Body>(body);
}

/**
 * @overload Euler::Physics::Shape#world
 *   @return [Euler::Physics::World] the world this shape belongs to
 */
static mrb_value
shape_world(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	auto world = shape->world();
	return state->wrap<euler::physics::World>(world);
}

/**
 * @overload Euler::Physics::Shape#sensor?
 *   @return [Boolean] true if the shape is a sensor
 */
static mrb_value
shape_is_sensor(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	const bool is_sensor = shape->is_sensor();
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
	const auto shape = state->unwrap<Shape>(self);
	mrb_float density;
	state->mrb()->get_args("f", &density);
	shape->set_density((float)density);
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
	const auto shape = state->unwrap<Shape>(self);
	const float density = shape->density();
	return state->mrb()->float_value(density);
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
	const auto shape = state->unwrap<Shape>(self);
	mrb_float friction;
	state->mrb()->get_args("f", &friction);
	shape->set_friction((float)friction);
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
	const auto shape = state->unwrap<Shape>(self);
	const float friction = shape->friction();
	return state->mrb()->float_value(friction);
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
	const auto shape = state->unwrap<Shape>(self);
	mrb_float restitution;
	state->mrb()->get_args("f", &restitution);
	shape->set_restitution((float)restitution);
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
	const auto shape = state->unwrap<Shape>(self);
	const float restitution = shape->restitution();
	return state->mrb()->float_value(restitution);
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
	const auto shape = state->unwrap<Shape>(self);
	mrb_int material;
	state->mrb()->get_args("i", &material);
	shape->set_user_material((uint64_t)material);
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
	const auto shape = state->unwrap<Shape>(self);
	const uint64_t material = shape->user_material();
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
	const auto shape = state->unwrap<Shape>(self);
	const b2SurfaceMaterial material = shape->surface_material();
	return euler::physics::surface_material_to_value(mrb, &material);
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
	const auto shape = state->unwrap<Shape>(self);
	mrb_value hash;
	state->mrb()->get_args("H", &hash);
	const b2SurfaceMaterial material
	    = euler::physics::value_to_surface_material(mrb, hash);
	shape->set_surface_material(&material);
	return mrb_nil_value();
}

static b2Filter
hash_to_filter(mrb_state *mrb, mrb_value hash)
{
	const auto state = euler::util::State::get(mrb);
	b2Filter filter = b2DefaultFilter();
	if (state->mrb()->hash_key_p(hash, B2_SYM(category_bits))) {
		filter.categoryBits = (uint16_t)euler::physics::hash_read_int(
		    mrb, hash, B2_SYM(category_bits), filter.categoryBits);
	}
	if (state->mrb()->hash_key_p(hash, B2_SYM(mask_bits))) {
		filter.maskBits = (uint16_t)euler::physics::hash_read_int(mrb,
		    hash, B2_SYM(mask_bits), filter.maskBits);
	}
	if (state->mrb()->hash_key_p(hash, B2_SYM(group_index))) {
		filter.groupIndex = (int16_t)euler::physics::hash_read_int(mrb,
		    hash, B2_SYM(group_index), filter.groupIndex);
	}
	return filter;
}

static mrb_value
filter_to_hash(mrb_state *mrb, const b2Filter *filter)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_value hash = state->mrb()->hash_new_capa(3);
	state->mrb()->hash_set(hash, B2_SYM(category_bits),
	    mrb_fixnum_value((mrb_int)filter->categoryBits));
	state->mrb()->hash_set(hash, B2_SYM(mask_bits),
	    mrb_fixnum_value((mrb_int)filter->maskBits));
	state->mrb()->hash_set(hash, B2_SYM(group_index),
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
	const auto shape = state->unwrap<Shape>(self);
	const b2Filter filter = shape->filter();
	return filter_to_hash(mrb, &filter);
}

static mrb_value
shape_set_filter(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	mrb_value hash;
	state->mrb()->get_args("H", &hash);
	const b2Filter filter = hash_to_filter(mrb, hash);
	shape->set_filter(filter);
	return mrb_nil_value();
}

static mrb_value
shape_enable_sensor_events(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	mrb_bool enable;
	state->mrb()->get_args("b", &enable);
	shape->enable_sensor_events(enable);
	return mrb_nil_value();
}

static mrb_value
shape_are_sensor_events_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	const bool enabled = shape->are_sensor_events_enabled();
	return mrb_bool_value(enabled);
}

static mrb_value
shape_enable_contact_events(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	mrb_bool enable;
	state->mrb()->get_args("b", &enable);
	shape->enable_contact_events(enable);
	return mrb_nil_value();
}

static mrb_value
shape_are_contact_events_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	const bool enabled = shape->are_contact_events_enabled();
	return mrb_bool_value(enabled);
}

static mrb_value
shape_enable_pre_solve_events(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	mrb_bool enable;
	state->mrb()->get_args("b", &enable);
	shape->enable_pre_solve_events(enable);
	return mrb_nil_value();
}

static mrb_value
shape_are_pre_solve_events_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	const bool enabled = shape->are_pre_solve_events_enabled();
	return mrb_bool_value(enabled);
}

static mrb_value
shape_enable_hit_events(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	mrb_bool enable;
	state->mrb()->get_args("b", &enable);
	shape->enable_hit_events(enable);
	return mrb_nil_value();
}

static mrb_value
shape_are_hit_events_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	const bool enabled = shape->are_hit_events_enabled();
	return mrb_bool_value(enabled);
}

static mrb_value
shape_test_point(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	mrb_value point_val;
	state->mrb()->get_args("o", &point_val);
	const b2Vec2 point = euler::physics::value_to_b2_vec(mrb, point_val);
	const bool inside = shape->test_point(point);
	return mrb_bool_value(inside);
}

static mrb_value
shape_ray_cast(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	mrb_value origin, translation;
	mrb_float max_fraction;
	state->mrb()->get_args("oof", &origin, &translation, &max_fraction);
	const b2RayCastInput input = {
		.origin = euler::physics::value_to_b2_vec(mrb, origin),
		.translation
		= euler::physics::value_to_b2_vec(mrb, translation),
		.maxFraction = (float)max_fraction,
	};
	const b2CastOutput output = shape->ray_cast(input);
	const mrb_value result = state->mrb()->hash_new_capa(4);
	state->mrb()->hash_set(result, B2_SYM(normal),
	    euler::physics::b2_vec_to_value(mrb, output.normal));
	state->mrb()->hash_set(result, B2_SYM(point),
	    euler::physics::b2_vec_to_value(mrb, output.point));
	state->mrb()->hash_set(result, B2_SYM(fraction),
	    state->mrb()->float_value(output.fraction));
	state->mrb()->hash_set(result, B2_SYM(hit), mrb_bool_value(output.hit));
	return result;
}
//
// static mrb_value
// wrap_chain_segment(mrb_state *mrb, const b2ChainSegment *cs)
// {
// 	const auto state = euler::util::State::get(mrb);
// 	const mrb_value out = state->mrb()->hash_new_capa(2);
// 	const mrb_value ghost = state->mrb()->ary_new_capa(2);
// 	state->mrb()->ary_push(ghost,
// 	    euler::physics::b2_vec_to_value(mrb, cs->ghost1));
// 	state->mrb()->ary_push(ghost,
// 	    euler::physics::b2_vec_to_value(mrb, cs->ghost2));
// 	state->mrb()->hash_set(out, B2_SYM(ghosts), ghost);
// 	const mrb_value segment = Shape::wrap_shape_data(mrb, cs->segment);
// 	state->mrb()->hash_set(out, B2_SYM(segment), segment);
// 	return out;
// }

static mrb_value
shape_underlying_shape(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	const auto data = shape->underlying_shape();
	return Shape::wrap_shape_data(mrb, data);
}

static mrb_value
shape_set_underlying_shape(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	mrb_value outline;
	state->mrb()->get_args("o", &outline);
	const auto data = Shape::shape_data(mrb, outline);
	shape->set_underlying_shape(data);
	return mrb_nil_value();
}

static mrb_value
shape_parent_chain(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	auto chain = shape->parent_chain();
	return state->wrap<euler::physics::Chain>(chain);
}

static mrb_value
shape_contact_data(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	const auto contacts = shape->contact_data();
	const mrb_value result = state->mrb()->ary_new_capa(contacts.size());
	for (auto contact : contacts)
		state->mrb()->ary_push(result, contact.wrap(mrb));
	return result;
}

static mrb_value
shape_sensors(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	const auto sensors = shape->sensors();
	const mrb_value result = state->mrb()->ary_new_capa(sensors.size());
	for (auto sensor : sensors) {
		const mrb_value sensor_val = state->wrap<Shape>(sensor);
		state->mrb()->ary_push(result, sensor_val);
	}
	return result;
}

static mrb_value
shape_aabb(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	const b2AABB aabb = shape->aabb();
	const mrb_value upper
	    = euler::physics::b2_vec_to_value(mrb, aabb.upperBound);
	const mrb_value lower
	    = euler::physics::b2_vec_to_value(mrb, aabb.lowerBound);
	const mrb_value out = state->mrb()->hash_new_capa(2);
	state->mrb()->hash_set(out, B2_SYM(upper_bound), upper);
	state->mrb()->hash_set(out, B2_SYM(lower_bound), lower);
	return out;
}

static mrb_value
shape_compute_mass_data(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	const b2MassData mass_data = shape->compute_mass_data();
	const mrb_value out = state->mrb()->hash_new_capa(3);
	state->mrb()->hash_set(out, B2_SYM(mass),
	    state->mrb()->float_value(mass_data.mass));
	state->mrb()->hash_set(out, B2_SYM(center),
	    euler::physics::b2_vec_to_value(mrb, mass_data.center));
	state->mrb()->hash_set(out, B2_SYM(rotational_inertia),
	    state->mrb()->float_value(mass_data.rotationalInertia));
	return out;
}

static mrb_value
shape_closest_point(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	mrb_value point_val;
	state->mrb()->get_args("o", &point_val);
	const b2Vec2 point = euler::physics::value_to_b2_vec(mrb, point_val);
	const b2Vec2 closest = shape->closest_point(point);
	return euler::physics::b2_vec_to_value(mrb, closest);
}

static mrb_value
shape_apply_wind(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto shape = state->unwrap<Shape>(self);
	mrb_value wind_val;
	float drag, lift;
	bool wake;
	state->mrb()->get_args("off|b", &wind_val, &drag, &lift, &wake);
	const b2Vec2 wind = euler::physics::value_to_b2_vec(mrb, wind_val);
	shape->apply_wind(wind, (float)drag, (float)lift, wake);
	return mrb_nil_value();
}

RClass *
Shape::init(const util::Reference<util::State> &state, RClass *mod, RClass *)
{
	RClass *body = state->mrb()->define_class_under(mod, "Shape",
	    state->mrb()->mrb()->object_class);
	MRB_SET_INSTANCE_TT(body, MRB_TT_DATA);
	state->mrb()->define_method(body, "valid?", shape_is_valid,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "type", shape_type, MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "body", shape_body, MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "world", shape_world,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "sensor?", shape_is_sensor,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "density=", shape_set_density,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "density", shape_density,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "friction=", shape_set_friction,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "friction", shape_friction,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "restitution=", shape_set_restitution,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "restitution", shape_restitution,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body,
	    "user_material=", shape_set_user_material, MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "user_material", shape_user_material,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "surface_material",
	    shape_surface_material, MRB_ARGS_REQ(0));
	state->mrb()->define_method(body,
	    "surface_material=", shape_set_surface_material, MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "filter", shape_filter,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "filter=", shape_set_filter,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body,
	    "sensor_events_enabled=", shape_enable_sensor_events,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "sensor_events_enabled",
	    shape_are_sensor_events_enabled, MRB_ARGS_REQ(0));
	state->mrb()->alias_method(body,
	    state->mrb()->intern_cstr("sensor_events_enabled?"),
	    state->mrb()->intern_cstr("sensor_events_enabled"));
	state->mrb()->define_method(body,
	    "contact_events_enabled=", shape_enable_contact_events,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "contact_events_enabled",
	    shape_are_contact_events_enabled, MRB_ARGS_REQ(0));
	state->mrb()->alias_method(body,
	    state->mrb()->intern_cstr("contact_events_enabled?"),
	    state->mrb()->intern_cstr("contact_events_enabled"));
	state->mrb()->define_method(body,
	    "pre_solve_events_enabled=", shape_enable_pre_solve_events,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "pre_solve_events_enabled",
	    shape_are_pre_solve_events_enabled, MRB_ARGS_REQ(0));
	state->mrb()->alias_method(body,
	    state->mrb()->intern_cstr("pre_solve_events_enabled?"),
	    state->mrb()->intern_cstr("pre_solve_events_enabled"));
	state->mrb()->define_method(body,
	    "hit_events_enabled=", shape_enable_hit_events, MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "hit_events_enabled",
	    shape_are_hit_events_enabled, MRB_ARGS_REQ(0));
	state->mrb()->alias_method(body,
	    state->mrb()->intern_cstr("hit_events_enabled?"),
	    state->mrb()->intern_cstr("hit_events_enabled"));
	state->mrb()->define_method(body, "test_point", shape_test_point,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "ray_cast", shape_ray_cast,
	    MRB_ARGS_REQ(1));
	// state->mrb()->define_method(body, "copy", shape_copy,
	// MRB_ARGS_REQ(0)); state->mrb()->define_method(body, "reshape",
	// shape_reshape, MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "parent_chain", shape_parent_chain,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "contacts", shape_contact_data,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "sensors", shape_sensors,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "aabb", shape_aabb, MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "compute_mass_data",
	    shape_compute_mass_data, MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "closest_point", shape_closest_point,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "apply_wind", shape_apply_wind,
	    MRB_ARGS_REQ(1));
	auto ms = state->mrb()->mrb();
	auto &mods = state->modules().physics;
	mods.capsule = capsule_init(ms, mod);
	mods.circle = circle_init(ms, mod);
	mods.polygon = polygon_init(ms, mod);
	mods.segment = segment_init(ms, mod);
	return body;
}

euler::util::Reference<Shape>
Shape::wrap(b2ShapeId id)
{
	auto user_data = b2Shape_GetUserData(id);
	if (user_data != nullptr)
		return util::Reference(static_cast<Shape *>(user_data));
	auto self = util::Reference(new Shape(id));
	b2Shape_SetUserData(id, self.get());
	return self;
}

mrb_value
Shape::wrap_shape_data(mrb_state *mrb, const ShapeData &shape)
{
	if (std::holds_alternative<b2Circle>(shape))
		return circle_wrap(mrb, std::get<b2Circle>(shape));
	if (std::holds_alternative<b2Capsule>(shape))
		return capsule_wrap(mrb, std::get<b2Capsule>(shape));
	if (std::holds_alternative<b2Polygon>(shape))
		return polygon_wrap(mrb, std::get<b2Polygon>(shape));
	if (std::holds_alternative<b2Segment>(shape))
		return segment_wrap(mrb, std::get<b2Segment>(shape));
	// if (std::holds_alternative<b2ChainSegment>(shape))
	// 	return wrap_chain_segment(mrb, std::get<b2ChainSegment>(shape));
	return mrb_nil_value();
}

Shape::ShapeData
Shape::shape_data(mrb_state *mrb, mrb_value val)
{
	const auto state = util::State::get(mrb);
	const auto cls_name = std::string(
	    state->mrb()->class_name(state->mrb()->obj_class(val)));
	if (cls_name == "Euler::Physics::Circle") return circle_shape(mrb, val);
	if (cls_name == "Euler::Physics::Capsule")
		return capsule_shape(mrb, val);
	if (cls_name == "Euler::Physics::Polygon")
		return polygon_shape(mrb, val);
	if (cls_name == "Euler::Physics::Segment")
		return segment_shape(mrb, val);
	// if (cls_name == "Euler::Physics::ChainSegment")
	// 	return chain_segment_shape(mrb, val);
	state->mrb()->raisef(E_ARGUMENT_ERROR,
	    "Expected a Shape data object, got %s", cls_name.c_str());
	std::unreachable();
}

mrb_value
Shape::SensorEvent::wrap(mrb_state *mrb)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_value out = state->mrb()->hash_new_capa(2);
	const auto sensor_val = state->wrap<Shape>(sensor);
	state->mrb()->hash_set(out, B2_SYM(sensor), sensor_val);
	const auto visitor_val = state->wrap<Shape>(visitor);
	state->mrb()->hash_set(out, B2_SYM(visitor), visitor_val);
	return out;
}

RClass *
box2d_shape_init(mrb_state *mrb, RClass *mod)
{
	const auto state = euler::util::State::get(mrb);
	RClass *body
	    = state->mrb()->define_class_under(mod, "Shape", mrb->object_class);
	MRB_SET_INSTANCE_TT(body, MRB_TT_DATA);
	state->mrb()->define_method(body, "valid?", shape_is_valid,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "type", shape_type, MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "body", shape_body, MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "world", shape_world,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "sensor?", shape_is_sensor,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "density=", shape_set_density,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "density", shape_density,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "friction=", shape_set_friction,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "friction", shape_friction,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "restitution=", shape_set_restitution,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "restitution", shape_restitution,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body,
	    "user_material=", shape_set_user_material, MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "user_material", shape_user_material,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "surface_material",
	    shape_surface_material, MRB_ARGS_REQ(0));
	state->mrb()->define_method(body,
	    "surface_material=", shape_set_surface_material, MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "filter", shape_filter,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "filter=", shape_set_filter,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body,
	    "sensor_events_enabled=", shape_enable_sensor_events,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "sensor_events_enabled",
	    shape_are_sensor_events_enabled, MRB_ARGS_REQ(0));
	state->mrb()->alias_method(body,
	    state->mrb()->intern_cstr("sensor_events_enabled?"),
	    state->mrb()->intern_cstr("sensor_events_enabled"));
	state->mrb()->define_method(body,
	    "contact_events_enabled=", shape_enable_contact_events,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "contact_events_enabled",
	    shape_are_contact_events_enabled, MRB_ARGS_REQ(0));
	state->mrb()->alias_method(body,
	    state->mrb()->intern_cstr("contact_events_enabled?"),
	    state->mrb()->intern_cstr("contact_events_enabled"));
	state->mrb()->define_method(body,
	    "pre_solve_events_enabled=", shape_enable_pre_solve_events,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "pre_solve_events_enabled",
	    shape_are_pre_solve_events_enabled, MRB_ARGS_REQ(0));
	state->mrb()->alias_method(body,
	    state->mrb()->intern_cstr("pre_solve_events_enabled?"),
	    state->mrb()->intern_cstr("pre_solve_events_enabled"));
	state->mrb()->define_method(body,
	    "hit_events_enabled=", shape_enable_hit_events, MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "hit_events_enabled",
	    shape_are_hit_events_enabled, MRB_ARGS_REQ(0));
	state->mrb()->alias_method(body,
	    state->mrb()->intern_cstr("hit_events_enabled?"),
	    state->mrb()->intern_cstr("hit_events_enabled"));
	state->mrb()->define_method(body, "test_point", shape_test_point,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "ray_cast", shape_ray_cast,
	    MRB_ARGS_REQ(1));
	// state->mrb()->define_method(body, "copy", shape_copy,
	// MRB_ARGS_REQ(0)); state->mrb()->define_method(body, "reshape",
	// shape_reshape, MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "underlying_shape",
	    shape_underlying_shape, MRB_ARGS_REQ(0));
	state->mrb()->define_method(body,
	    "underlying_shape=", shape_set_underlying_shape, MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "parent_chain", shape_parent_chain,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "contacts", shape_contact_data,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "sensors", shape_sensors,
	    MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "aabb", shape_aabb, MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "compute_mass_data",
	    shape_compute_mass_data, MRB_ARGS_REQ(0));
	state->mrb()->define_method(body, "closest_point", shape_closest_point,
	    MRB_ARGS_REQ(1));
	state->mrb()->define_method(body, "apply_wind", shape_apply_wind,
	    MRB_ARGS_REQ(1));
	return body;
}

Shape::SensorEvents
Shape::SensorEvents::from_b2(const b2SensorEvents &events)
{
	SensorEvents out;
	out.start.reserve(events.beginCount);
	out.end.reserve(events.endCount);
	for (int i = 0; i < events.beginCount; ++i) {
		const auto &event = events.beginEvents[i];
		auto sensor_ptr = new Shape(event.sensorShapeId);
		const auto sensor = util::Reference(sensor_ptr);
		auto visitor_ptr = new Shape(event.visitorShapeId);
		const auto visitor = util::Reference(visitor_ptr);
		out.start.emplace_back(SensorEvent {
		    .sensor = sensor,
		    .visitor = visitor,
		});
	}
	for (int i = 0; i < events.endCount; ++i) {
		const auto &[sensorShapeId, visitorShapeId]
		    = events.endEvents[i];
		const auto sensor_ptr = new Shape(sensorShapeId);
		const auto sensor = util::Reference(sensor_ptr);
		const auto visitor_ptr = new Shape(visitorShapeId);
		const auto visitor = util::Reference(visitor_ptr);
		out.end.emplace_back(SensorEvent {
		    .sensor = sensor,
		    .visitor = visitor,
		});
	}
	return out;
}

mrb_value
Shape::SensorEvents::wrap(mrb_state *mrb)
{
	const auto state = util::State::get(mrb);
	const mrb_value begin_ary = state->mrb()->ary_new_capa(start.size());
	for (auto &event : start)
		state->mrb()->ary_push(begin_ary, event.wrap(mrb));
	const mrb_value end_ary = state->mrb()->ary_new_capa(end.size());
	for (auto &event : end)
		state->mrb()->ary_push(end_ary, event.wrap(mrb));
	const mrb_value result = state->mrb()->hash_new_capa(2);
	state->mrb()->hash_set(result, B2_SYM(begin), begin_ary);
	state->mrb()->hash_set(result, B2_SYM(end), end_ary);
	return result;
}

mrb_value
Shape::type_to_symbol(mrb_state *mrb, Type type)
{
	const auto state = util::State::get(mrb);
	switch (type) {
	case Type::Circle: return B2_SYM(circle);
	case Type::Capsule: return B2_SYM(capsule);
	case Type::Segment: return B2_SYM(segment);
	case Type::Polygon: return B2_SYM(polygon);
	case Type::ChainSegment: return B2_SYM(chain_segment);
	default:
		state->mrb()->raise(E_ARGUMENT_ERROR, "unknown shape type");
		std::unreachable();
	}
}

Shape::ShapeData
Shape::underlying_shape() const
{
	switch (type()) {
	case Type::Circle: return b2Shape_GetCircle(_id);
	case Type::Capsule: return b2Shape_GetCapsule(_id);
	case Type::Segment: return b2Shape_GetSegment(_id);
	case Type::Polygon: return b2Shape_GetPolygon(_id);
	case Type::ChainSegment: return b2Shape_GetChainSegment(_id);
	default: std::unreachable();
	}
}

void
Shape::set_underlying_shape(const ShapeData &data)
{
	if (std::holds_alternative<b2Circle>(data))
		b2Shape_SetCircle(_id, &std::get<b2Circle>(data));
	else if (std::holds_alternative<b2Capsule>(data))
		b2Shape_SetCapsule(_id, &std::get<b2Capsule>(data));
	else if (std::holds_alternative<b2Segment>(data))
		b2Shape_SetSegment(_id, &std::get<b2Segment>(data));
	else if (std::holds_alternative<b2Polygon>(data))
		b2Shape_SetPolygon(_id, &std::get<b2Polygon>(data));
	else if (std::holds_alternative<b2ChainSegment>(data))
		throw std::runtime_error("cannot reshape into a chain segment");
	else throw std::runtime_error("unknown shape data type");
}

euler::util::Reference<euler::physics::Body>
Shape::body() const
{
	const auto body_id = b2Shape_GetBody(_id);
	return Body::wrap(body_id);
}

euler::util::Reference<euler::physics::World>
Shape::world() const
{
	const auto world_id = b2Shape_GetWorld(_id);
	return World::wrap(world_id);
}

b2ShapeId
Shape::id() const
{
	return _id;
}

bool
Shape::is_valid() const
{
	return b2Shape_IsValid(_id);
}

Shape::Type
Shape::type() const
{
	switch (b2Shape_GetType(_id)) {
	case b2_circleShape: return Type::Circle;
	case b2_capsuleShape: return Type::Capsule;
	case b2_segmentShape: return Type::Segment;
	case b2_polygonShape: return Type::Polygon;
	case b2_chainSegmentShape: return Type::ChainSegment;
	default: throw std::runtime_error("unknown shape type");
	}
}

bool
Shape::is_sensor() const
{
	return b2Shape_IsSensor(_id);
}

void
Shape::set_density(float density)
{
	b2Shape_SetDensity(_id, density, true);
}

float
Shape::density() const
{
	return b2Shape_GetDensity(_id);
}

void
Shape::set_friction(float friction)
{
	b2Shape_SetFriction(_id, friction);
}

float
Shape::friction() const
{
	return b2Shape_GetFriction(_id);
}

void
Shape::set_restitution(float restitution)
{
	b2Shape_SetRestitution(_id, restitution);
}

float
Shape::restitution() const
{
	return b2Shape_GetRestitution(_id);
}

void
Shape::set_user_material(uint64_t material_id)
{
	b2Shape_SetUserMaterial(_id, material_id);
}

uint64_t
Shape::user_material() const
{
	return b2Shape_GetUserMaterial(_id);
}

b2SurfaceMaterial
Shape::surface_material() const
{
	return b2Shape_GetSurfaceMaterial(_id);
}

void
Shape::set_surface_material(const b2SurfaceMaterial *material)
{
	b2Shape_SetSurfaceMaterial(_id, material);
}

b2Filter
Shape::filter() const
{
	return b2Shape_GetFilter(_id);
}

void
Shape::set_filter(const b2Filter &filter)
{
	b2Shape_SetFilter(_id, filter);
}

void
Shape::enable_sensor_events(bool flag)
{
	b2Shape_EnableSensorEvents(_id, flag);
}

bool
Shape::are_sensor_events_enabled() const
{
	return b2Shape_AreSensorEventsEnabled(_id);
}

void
Shape::enable_contact_events(bool flag)
{
	b2Shape_EnableContactEvents(_id, flag);
}

bool
Shape::are_contact_events_enabled() const
{
	return b2Shape_AreContactEventsEnabled(_id);
}

void
Shape::enable_pre_solve_events(bool flag)
{
	b2Shape_EnablePreSolveEvents(_id, flag);
}

bool
Shape::are_pre_solve_events_enabled() const
{
	return b2Shape_ArePreSolveEventsEnabled(_id);
}

void
Shape::enable_hit_events(bool flag)
{
	b2Shape_EnableHitEvents(_id, flag);
}

bool
Shape::are_hit_events_enabled() const
{
	return b2Shape_AreHitEventsEnabled(_id);
}

bool
Shape::test_point(const b2Vec2 &point) const
{
	return b2Shape_TestPoint(_id, point);
}

b2CastOutput
Shape::ray_cast(const b2RayCastInput &input) const
{
	return b2Shape_RayCast(_id, &input);
}

euler::util::Reference<euler::physics::Chain>
Shape::parent_chain() const
{
	const auto chain_id = b2Shape_GetParentChain(_id);
	return Chain::wrap(chain_id);
}

std::vector<euler::physics::Contact::Data>
Shape::contact_data() const
{
	const auto count = b2Shape_GetContactCapacity(_id);
	std::vector<b2ContactData> contact_data;
	contact_data.reserve(count);
	b2Shape_GetContactData(_id, contact_data.data(), count);
	std::vector<Contact::Data> contacts;
	contacts.reserve(count);
	for (const auto &data : contact_data)
		contacts.push_back(Contact::Data::from_b2(data));
	return contacts;
}

std::vector<euler::util::Reference<Shape>>
Shape::sensors() const
{
	const auto count = b2Shape_GetSensorCapacity(_id);
	std::vector<b2ShapeId> sensor_ids;
	sensor_ids.reserve(count);
	b2Shape_GetSensorData(_id, sensor_ids.data(), count);
	std::vector<util::Reference<Shape>> sensors;
	sensors.reserve(count);
	for (const auto sensor_id : sensor_ids) {
		const auto ptr = new Shape(sensor_id);
		sensors.push_back(util::Reference(ptr));
	}
	return sensors;
}

b2AABB
Shape::aabb() const
{
	return b2Shape_GetAABB(_id);
}

b2MassData
Shape::compute_mass_data() const
{
	return b2Shape_ComputeMassData(_id);
}

b2Vec2
Shape::closest_point(const b2Vec2 &point) const
{
	return b2Shape_GetClosestPoint(_id, point);
}

void
Shape::apply_wind(const b2Vec2 &wind, float drag, float lift, bool wake)
{
	return b2Shape_ApplyWind(_id, wind, drag, lift, wake);
}
