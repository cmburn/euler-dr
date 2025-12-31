/* SPDX-License-Identifier: ISC */

#include "euler/physics/ext.h"

#include <box2d/box2d.h>

#include "euler/physics/util.h"
#include "euler/util/state.h"


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
	return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@radius"));
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
	mrb_get_args(mrb, "f", &radius);
	const mrb_value value = mrb_float_value(mrb, radius);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@radius"), value);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Capsule#centers
 *   @return [Array&lt;Array&lt;Float&gt;&gt;] The center points of the capsule. Is an array
 *           of two points, i.e. [[x1, y1], [x2, y2]]
 */
static mrb_value
capsule_centers(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@center"));
}

/**
 * @overload Euler::Physics::Capsule#centers=(centers)
 *   @param centers [Array&lt;Array&lt;Float&gt;&gt;] The center points to set for the
 *          capsule. Must be an array of two points, i.e. [[x1, y1], [x2, y2]].
 *   @return [void]
 */
static mrb_value
capsule_set_centers(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	/* @center is in this case an array of two points,
	 * i.e. [[x1, y1], [x2, y2]] */
	mrb_value center;
	mrb_get_args(mrb, "A", &center);
	if (RARRAY_LEN(center) != 2) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "center must be an array of two points");
	}
	const b2Vec2 center1 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(center, 0));
	const b2Vec2 center2 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(center, 1));
	const mrb_value center_val = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, center_val, euler::physics::b2_vec_to_value(mrb, center1));
	mrb_ary_push(mrb, center_val, euler::physics::b2_vec_to_value(mrb, center2));
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@center"), center_val);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Capsule#initialize(centers, radius)
 *   @param centers [Array&lt;Array&lt;Float&gt;&gt;] The center points of the capsule. Must
 *          be an array of two points, i.e. [[x1, y1], [x2, y2]].
 *   @param radius [Float] The radius of the capsule
 *   @return [void]
 */
static mrb_value
capsule_initialize(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value center;
	mrb_float radius;
	mrb_get_args(mrb, "Af", &center, &radius);
	if (RARRAY_LEN(center) != 2) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "center must be an array of two points");
	}
	const b2Vec2 center1 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(center, 0));
	const b2Vec2 center2 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(center, 1));
	const mrb_value center_val = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, center_val, euler::physics::b2_vec_to_value(mrb, center1));
	mrb_ary_push(mrb, center_val, euler::physics::b2_vec_to_value(mrb, center2));
	mrb_iv_set(mrb, self,
	    mrb_intern_lit(mrb, "@center"), center_val);
	mrb_iv_set(mrb, self,
	    mrb_intern_lit(mrb, "@radius"), mrb_float_value(mrb, radius));
	return mrb_nil_value();
}

RClass *
capsule_init(mrb_state *mrb, struct RClass *mod)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *capsule
	    = mrb_define_class_under(mrb, mod, "Capsule", mrb->object_class);
	mrb_define_method(mrb, capsule, "radius", capsule_radius,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, capsule, "radius=", capsule_set_radius,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, capsule, "centers", capsule_centers,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, capsule, "centers=", capsule_set_centers,
	    MRB_ARGS_REQ(1));
	return capsule;
}

mrb_value
capsule_wrap(mrb_state *mrb, b2Capsule capsule)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *capsule_class
	    = mrb_class_get_under(mrb, mrb_module_get(mrb, "Euler::Physics"), "Capsule");
	const mrb_value center_val = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, center_val, euler::physics::b2_vec_to_value(mrb, capsule.center1));
	mrb_ary_push(mrb, center_val, euler::physics::b2_vec_to_value(mrb, capsule.center2));
	const mrb_value radius_val = mrb_float_value(mrb, capsule.radius);
	const mrb_value args[] = { center_val, radius_val };
	const mrb_value obj = mrb_obj_new(mrb, capsule_class, 2, args);
	return obj;
}

b2Capsule
capsule_shape(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_sym rad_sym = mrb_intern_lit(mrb, "@radius");
	const mrb_sym center_sym = mrb_intern_lit(mrb, "@center");
	const mrb_value radius_val = mrb_iv_get(mrb, self, rad_sym);
	const mrb_value center_val = mrb_iv_get(mrb, self, center_sym);
	b2Capsule capsule;
	capsule.radius = (float)mrb_float(radius_val);
	if (RARRAY_LEN(center_val) != 2) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "center must be an array of two points");
	}
	capsule.center1 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(center_val, 0));
	capsule.center2 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(center_val, 1));
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
	return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@radius"));
}

/**
 * @overload Box2D::Circle#radius=(radius)
 *   @param radius [Float] the radius to set for the circle
 *   @return [void]
 */
static mrb_value
circle_set_radius(mrb_state *mrb, mrb_value self)
{
	mrb_float radius;
	mrb_get_args(mrb, "f", &radius);
	const mrb_value value = mrb_float_value(mrb, radius);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@radius"), value);
	return mrb_nil_value();
}

/**
 * @overload Box2D::Circle#center
 *   @return [Array<Numeric>] the center point of the circle
 */
static mrb_value
circle_center(mrb_state *mrb, mrb_value self)
{
	return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@center"));
}

/**
 * @overload Box2D::Circle#center=(center)
 *   @param center [Array<Numeric>] the center point to set for the circle
 *   @return [void]
 */
static mrb_value
circle_set_center(mrb_state *mrb, mrb_value self)
{
	mrb_value center;
	mrb_get_args(mrb, "o", &center);
	/* coerce to/from b2 vec to ensure we're valid */
	const b2Vec2 vec = euler::physics::value_to_b2_vec(mrb, center);
	const mrb_value value = euler::physics::b2_vec_to_value(mrb, vec);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@center"), value);
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
	mrb_value center;
	mrb_float radius;
	mrb_get_args(mrb, "of", &center, &radius);
	const mrb_value radius_val = mrb_float_value(mrb, radius);
	/* coerce to/from b2 vec to ensure we're valid */
	const b2Vec2 vec = euler::physics::value_to_b2_vec(mrb, center);
	const mrb_value center_val = euler::physics::b2_vec_to_value(mrb, vec);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@radius"), radius_val);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@center"), center_val);
	return mrb_nil_value();
}

RClass *
circle_init(mrb_state *mrb, struct RClass *mod)
{
	struct RClass *circle
	    = mrb_define_class_under(mrb, mod, "Circle", mrb->object_class);
	mrb_define_method(mrb, circle, "radius", circle_radius,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, circle, "radius=", circle_set_radius,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, circle, "center", circle_center,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, circle, "center=", circle_set_center,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, circle, "initialize", circle_initialize,
	    MRB_ARGS_REQ(2));
	return circle;
}

mrb_value
circle_wrap(mrb_state *mrb, b2Circle circle)
{
	struct RClass *circle_class
	    = mrb_class_get_under(mrb, mrb_module_get(mrb, "Box2D"), "Circle");
	const mrb_value radius_val = mrb_float_value(mrb, circle.radius);
	const mrb_value center_val = euler::physics::b2_vec_to_value(mrb, circle.center);
	const mrb_value args[] = { radius_val, center_val };
	const mrb_value obj = mrb_obj_new(mrb, circle_class, 2, args);
	return obj;
}

b2Circle
circle_shape(mrb_state *mrb, mrb_value self)
{
	const mrb_sym rad_sym = mrb_intern_lit(mrb, "@radius");
	const mrb_sym center_sym = mrb_intern_lit(mrb, "@center");
	const mrb_value radius_val = mrb_iv_get(mrb, self, rad_sym);
	const mrb_value center_val = mrb_iv_get(mrb, self, center_sym);
	return (b2Circle) {
		.center = euler::physics::value_to_b2_vec(mrb, center_val),
		.radius = (float)mrb_float(radius_val),
	};
}


/* Euler::Physics::Polygon */
static mrb_ssize
polygon_vertex_count(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value vertices
	    = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@vertices"));
	return RARRAY_LEN(vertices);
}

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
	return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@vertices"));
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
	mrb_get_args(mrb, "A", &vertices);
	const size_t len = RARRAY_LEN(vertices);
	if (len > B2_MAX_POLYGON_VERTICES) {
		mrb_raisef(mrb, E_ARGUMENT_ERROR,
		    "Number of vertices exceeds maximum (%d)",
		    B2_MAX_POLYGON_VERTICES);
	}

	const mrb_value arr = mrb_ary_new_capa(mrb, len);
	for (mrb_int i = 0; i < len; ++i) {
		const mrb_value value = mrb_ary_entry(vertices, i);
		const b2Vec2 vec = euler::physics::value_to_b2_vec(mrb, value);
		mrb_ary_push(mrb, value, euler::physics::b2_vec_to_value(mrb, vec));
	}
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@vertices"), arr);
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
	return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@normals"));
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
	mrb_get_args(mrb, "A", &normals);
	const size_t len = RARRAY_LEN(normals);
	const mrb_value arr = mrb_ary_new_capa(mrb, len);
	for (mrb_int i = 0; i < len; ++i) {
		const mrb_value value = mrb_ary_entry(normals, i);
		const b2Vec2 vec = euler::physics::value_to_b2_vec(mrb, value);
		mrb_ary_push(mrb, value, euler::physics::b2_vec_to_value(mrb, vec));
	}
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@normals"), arr);
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
	return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@centroid"));
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
	mrb_get_args(mrb, "A", &centroid);
	const b2Vec2 vec = euler::physics::value_to_b2_vec(mrb, centroid);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@centroid"),
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
	return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@radius"));
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
	mrb_get_args(mrb, "f", &radius);
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@radius"),
	    mrb_float_value(mrb, radius));
	return mrb_nil_value();
}

static void
read_offset(mrb_state *mrb, mrb_value offset, b2Vec2 *position, b2Rot *rotation)
{
	const auto state = euler::util::State::get(mrb);
}

static b2Polygon
polygon_initialize_box(mrb_state *mrb, mrb_value box, mrb_value radius,
    mrb_value offset)
{
	const auto state = euler::util::State::get(mrb);
	if (!mrb_array_p(box) || RARRAY_LEN(box) != 2) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    ":box must be an array of two elements [width, height]");
	}
	const float width_value = euler::physics::coerce_float(mrb, mrb_ary_entry(box, 0));
	const float height_value = euler::physics::coerce_float(mrb, mrb_ary_entry(box, 1));
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
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "Expected an array of points as hull argument");
	}
	const size_t len = RARRAY_LEN(hull);
	b2Vec2 *points = (b2Vec2 *)euler::physics::tmpbuf(mrb, len * sizeof(b2Vec2));
	for (mrb_int i = 0; i < len; ++i)
		points[i] = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(hull, i));
	return b2ComputeHull(points, len);
}

static b2Polygon
polygon_initialize_hull(mrb_state *mrb, mrb_value hull_value, mrb_value radius,
    mrb_value offset)
{
	const auto state = euler::util::State::get(mrb);
	const b2Hull hull = value_to_hull(mrb, hull_value);
	if (mrb_nil_p(radius)) {
		if (mrb_nil_p(offset)) {
			mrb_raise(mrb, E_ARGUMENT_ERROR,
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

/*
 */

static mrb_value
polygon_initialize(mrb_state *mrb, mrb_value self)
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
		KW_COUNT = sizeof(KW_NAMES) / sizeof(KW_NAMES[0]),
	};
	mrb_sym kw_syms[KW_COUNT];
	for (size_t i = 0; i < KW_COUNT; ++i) {
		const size_t len = strlen(KW_NAMES[i]);
		kw_syms[i] = mrb_intern_static(mrb, KW_NAMES[i], len);
	}
	mrb_value kw_values[KW_COUNT];
	const mrb_kwargs kwargs = {
		.num = KW_COUNT,
		.required = 0,
		.table = kw_syms,
		.values = kw_values,
		.rest = NULL,
	};
	mrb_get_args(mrb, ":", &kwargs);
	mrb_value points, box, radius, offset;
	if (!mrb_undef_p(kw_values[POINTS]) && !mrb_undef_p(kw_values[BOX])) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "Cannot specify both :points and :box");
	}
	if (!mrb_undef_p(kw_values[POINTS])) points = kw_values[POINTS];
	else if (!mrb_undef_p(kw_values[BOX])) box = kw_values[BOX];
	else mrb_raise(mrb, E_ARGUMENT_ERROR, "Must specify :points or :box");
	if (!mrb_undef_p(kw_values[RADIUS])) radius = kw_values[RADIUS];
	if (!mrb_undef_p(kw_values[OFFSET])) offset = kw_values[OFFSET];
	b2Polygon pg;

	if (mrb_undef_p(points))
		pg = polygon_initialize_box(mrb, box, radius, offset);
	else pg = polygon_initialize_hull(mrb, points, radius, offset);
	const mrb_value verts = mrb_ary_new_capa(mrb, pg.count);
	for (int i = 0; i < pg.count; ++i)
		mrb_ary_push(mrb, verts, euler::physics::b2_vec_to_value(mrb, pg.vertices[i]));
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@vertices"), verts);
}

static RClass *
polygon_init(mrb_state *mrb, RClass *mod)
{

}

static mrb_value
segment_points(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	return mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@points"));
}

/**
 * @overload Euler::Physics::Segment#points=(points)
 *   Set the segment's two endpoints.
 *   @param points [Array<Euler::Physics::Vec2>] An array containing two Box2D::Vec2
 *          points.
 *   @return [void]
 */
static mrb_value
segment_set_points(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value points;
	mrb_get_args(mrb, "A", &points);
	if (RARRAY_LEN(points) != 2) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "points must be an array of two points");
	}
	const b2Vec2 p1 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 0));
	const b2Vec2 p2 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 1));
	const mrb_value points_val = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, points_val, euler::physics::b2_vec_to_value(mrb, p1));
	mrb_ary_push(mrb, points_val, euler::physics::b2_vec_to_value(mrb, p2));
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@points"), points_val);
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Segment#initialize(points)
 *   Initialize a new segment with two endpoints.
 *   @param points [Array<Euler::Physics::Vec2>] An array containing two Box2D::Vec2
 *          points.
 */
static mrb_value
segment_initialize(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	mrb_value points;
	mrb_get_args(mrb, "A", &points);
	if (RARRAY_LEN(points) != 2) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "points must be an array of two points");
	}
	const b2Vec2 p1 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 0));
	const b2Vec2 p2 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 1));
	const mrb_value points_val = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, points_val, euler::physics::b2_vec_to_value(mrb, p1));
	mrb_ary_push(mrb, points_val, euler::physics::b2_vec_to_value(mrb, p2));
	mrb_iv_set(mrb, self, mrb_intern_lit(mrb, "@points"), points_val);
	return self;
}

struct RClass *
segment_init(mrb_state *mrb, struct RClass *mod)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *segment
	    = mrb_define_class_under(mrb, mod, "Segment", mrb->object_class);
	mrb_define_method(mrb, segment, "points", segment_points,
	    MRB_ARGS_REQ(0));
	mrb_define_method(mrb, segment, "points=", segment_set_points,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, segment, "initialize", segment_initialize,
	    MRB_ARGS_REQ(1));
	return segment;
}

mrb_value
segment_wrap(mrb_state *mrb, const b2Segment segment)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *cls = mrb_class_get(mrb, "Euler::Physics::Segment");
	const mrb_value points = mrb_ary_new_capa(mrb, 2);
	mrb_ary_push(mrb, points, euler::physics::b2_vec_to_value(mrb, segment.point1));
	mrb_ary_push(mrb, points, euler::physics::b2_vec_to_value(mrb, segment.point2));
	const mrb_value obj = mrb_obj_new(mrb, cls, 1, &points);
	return obj;
}

b2Segment
segment_shape(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_value points
	    = mrb_iv_get(mrb, self, mrb_intern_lit(mrb, "@points"));
	if (RARRAY_LEN(points) != 2) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "points must be an array of two points");
	}
	const b2Vec2 p1 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 0));
	const b2Vec2 p2 = euler::physics::value_to_b2_vec(mrb, mrb_ary_entry(points, 1));
	return (b2Segment) {
		.point1 = p1,
		.point2 = p2,
	};
}

/* Euler::Physics::Segment */

RClass *
euler::physics::init(const util::Reference<util::State> &state,
    RClass *outer)
{
	auto mod = state->mrb()->define_module_under(outer, "Physics");
	capsule_init(state->mrb()->mrb(), mod);
	circle_init(state->mrb()->mrb(), mod);
	polygon_init(state->mrb()->mrb(), mod);
	segment_init(state->mrb()->mrb(), mod);
}