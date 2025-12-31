/* SPDX-License-Identifier: ISC */

#include "euler/physics/world.h"

#include <box2d/types.h>

static struct b2WorldDef
parse_world_new_args(mrb_state *mrb)
{
	const auto state = euler::util::State::get(mrb);
	static const char *KW_NAMES[] = {
		"gravity",
		"restitution_threshold",
		"hit_event_threshold",
		"contact_hertz",
		"contact_damping_ratio",
		"contact_speed",
		"maximum_linear_speed",
		"enable_sleep",
		"enable_continuous",
		"enable_contact_softening",
	};
	enum {
		GRAVITY = 0,
		RESTITUTION_THRESHOLD,
		HIT_EVENT_THRESHOLD,
		CONTACT_HERTZ,
		CONTACT_DAMPING_RATIO,
		CONTACT_SPEED,
		MAXIMUM_LINEAR_SPEED,
		ENABLE_SLEEP,
		ENABLE_CONTINUOUS,
		ENABLE_CONTACT_SOFTENING,
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
	struct b2WorldDef world_def = b2DefaultWorldDef();
	struct b2Vec2 gravity = world_def.gravity;
	if (!mrb_undef_p(kw_values[GRAVITY]))
		gravity = value_to_b2_vec(mrb, kw_values[GRAVITY]);
	world_def.gravity = gravity;
	if (!mrb_undef_p(kw_values[RESTITUTION_THRESHOLD])) {
		world_def.restitutionThreshold
		    = (float)mrb_float(kw_values[RESTITUTION_THRESHOLD]);
	}
	if (!mrb_undef_p(kw_values[HIT_EVENT_THRESHOLD])) {
		world_def.hitEventThreshold
		    = (float)mrb_float(kw_values[HIT_EVENT_THRESHOLD]);
	}
	if (!mrb_undef_p(kw_values[CONTACT_HERTZ])) {
		world_def.contactHertz
		    = (float)mrb_float(kw_values[CONTACT_HERTZ]);
	}
	if (!mrb_undef_p(kw_values[CONTACT_DAMPING_RATIO])) {
		world_def.contactDampingRatio
		    = (float)mrb_float(kw_values[CONTACT_DAMPING_RATIO]);
	}
	if (!mrb_undef_p(kw_values[CONTACT_SPEED])) {
		world_def.contactSpeed
		    = (float)mrb_float(kw_values[CONTACT_SPEED]);
	}
	if (!mrb_undef_p(kw_values[MAXIMUM_LINEAR_SPEED])) {
		world_def.maximumLinearSpeed
		    = (float)mrb_float(kw_values[MAXIMUM_LINEAR_SPEED]);
	}
	if (!mrb_undef_p(kw_values[ENABLE_SLEEP]))
		world_def.enableSleep = mrb_bool(kw_values[ENABLE_SLEEP]);
	if (!mrb_undef_p(kw_values[ENABLE_CONTINUOUS])) {
		world_def.enableContinuous
		    = mrb_bool(kw_values[ENABLE_CONTINUOUS]);
	}
	if (!mrb_undef_p(kw_values[ENABLE_CONTACT_SOFTENING])) {
		world_def.enableContactSoftening
		    = mrb_bool(kw_values[ENABLE_CONTACT_SOFTENING]);
	}
	return world_def;
}

static mrb_value
world_new(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const b2WorldDef def = parse_world_new_args(mrb);
	(void)self;
	const b2WorldId id = b2CreateWorld(&def);
	struct box2d_world *data = (struct box2d_world *)tmpbuf(mrb,
	    sizeof(struct box2d_world));
	assert(data != NULL);
	data->id = id;
	data->mrb = mrb;
	data->custom_filter_fn = mrb_nil_value();
	data->pre_solve_fn = mrb_nil_value();
	mrb_gc_register(mrb, data->custom_filter_fn);
	mrb_gc_register(mrb, data->pre_solve_fn);
	struct RClass *cls = read_box2d_class(mrb);
	struct RData *obj = Data_Wrap_Struct(mrb, cls, &BOX2D_WORLD_TYPE, data);
	data->self = mrb_obj_value(obj);
	b2World_SetUserData(id, &data);
	return data->self;
}

static mrb_value
world_destroy(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	struct box2d_world *world = box2d_world_unwrap(mrb, self);
	destroy_world(mrb, world);
	mrb_data_init(self, NULL, NULL);
	return mrb_nil_value();
}

/**
 * Tests whether the world is valid according to Box2D.
 *
 * @return [Boolean] true if the world is valid, false otherwise.
 */
static mrb_value
world_is_valid(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const bool valid = b2World_IsValid(world->id);
	return mrb_bool_value(valid);
}

/**
 * Simulates a world for one time step. This performs collision detection,
 * integration, and constraint solving.
 * @param time_step [Float] The amount of time to simulate.
 * @param substep_count [Integer] The number of sub-steps, increasing the
 *   sub-step count can increase accuracy.
 */
static mrb_value
world_step(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	static const char *KW_NAMES[] = {
		"substeps",
	};
	enum {
		SUBSTEPS = 0,
		KW_COUNT = sizeof(KW_NAMES) / sizeof(KW_NAMES[0]),
	};
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_float time_step = 1.0f / 60.0f;
	int substep_count = 4;
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
	mrb_get_args(mrb, "|f:", &time_step, &kwargs);
	if (!mrb_undef_p(kw_values[SUBSTEPS])) {
		substep_count = (int)mrb_integer(kw_values[SUBSTEPS]);
	}
	b2World_Step(world->id, time_step, substep_count);
	return mrb_nil_value();
}

static mrb_value
body_move_event_to_hash(mrb_state *mrb, const struct box2d_world *world,
    const b2BodyMoveEvent *event)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_value hash = mrb_hash_new(mrb);
	const mrb_value body = box2d_body_wrap(mrb, event->bodyId);
	mrb_hash_set(mrb, hash, B2_SYM(transform),
	    b2_transform_to_value(mrb, event->transform));
	mrb_hash_set(mrb, hash, B2_SYM(body), body);
	mrb_hash_set(mrb, hash, B2_SYM(fell_asleep),
	    mrb_bool_value(event->fellAsleep));
	return hash;
}

/**
 * Get the body events for the current time step.
 *
 * Returned hashes have the following keys::
 * - :transform - [Euler::Physics::Transform] The new transform of the body.
 * - :body - [Euler::Physics::Body] The body that moved.
 * - :fell_asleep - [Boolean] true if the body fell asleep this step.

 * @return [Array<Hash>] An array of body move event hashes.
 */
static mrb_value
world_body_events(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const b2BodyEvents body_events = b2World_GetBodyEvents(world->id);
	const mrb_value ary = mrb_ary_new_capa(mrb, body_events.moveCount);
	for (int i = 0; i < body_events.moveCount; ++i) {
		const mrb_value event = body_move_event_to_hash(mrb, world,
		    &body_events.moveEvents[i]);
		mrb_ary_push(mrb, ary, event);
	}
	return ary;
}

static mrb_value
sensor_touch_event_to_hash(mrb_state *mrb, const struct box2d_world *world,
    const b2ShapeId sensor, const b2ShapeId visitor)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_value hash = mrb_hash_new(mrb);
	const mrb_value sensor_shape = box2d_shape_wrap(mrb, sensor);
	const mrb_value visitor_shape = box2d_shape_wrap(mrb, visitor);
	mrb_hash_set(mrb, hash, B2_SYM(sensor), sensor_shape);
	mrb_hash_set(mrb, hash, B2_SYM(visitor), visitor_shape);
	return hash;
}

/**
 *
 * @return [Hash] A hash with :begin and :end keys containing arrays of sensor
 *                touch event hashes.
 */
static mrb_value
world_sensor_events(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const b2SensorEvents sensor_events = b2World_GetSensorEvents(world->id);
	const mrb_value begin_ary
	    = mrb_ary_new_capa(mrb, sensor_events.beginCount);
	for (int i = 0; i < sensor_events.beginCount; ++i) {
		const b2SensorBeginTouchEvent *event
		    = &sensor_events.beginEvents[i];
		const mrb_value event_hash = sensor_touch_event_to_hash(mrb,
		    world, event->sensorShapeId, event->visitorShapeId);
		mrb_ary_push(mrb, begin_ary, event_hash);
	}
	const mrb_value end_ary = mrb_ary_new_capa(mrb, sensor_events.endCount);
	for (int i = 0; i < sensor_events.endCount; ++i) {
		const b2SensorEndTouchEvent *event
		    = &sensor_events.endEvents[i];
		const mrb_value event_hash = sensor_touch_event_to_hash(mrb,
		    world, event->sensorShapeId, event->visitorShapeId);
		mrb_ary_push(mrb, end_ary, event_hash);
	}
	const mrb_value result = mrb_hash_new(mrb);
	mrb_hash_set(mrb, result, B2_SYM(begin), begin_ary);
	mrb_hash_set(mrb, result, B2_SYM(end), end_ary);
	return result;
}

static mrb_value
contact_bound_event_to_hash(mrb_state *mrb, const struct box2d_world *world,
    const b2ShapeId shape_a, const b2ShapeId shape_b)
{
	const auto state = euler::util::State::get(mrb);
	const mrb_value hash = mrb_hash_new(mrb);
	const mrb_value a = box2d_shape_wrap(mrb, shape_a);
	const mrb_value b = box2d_shape_wrap(mrb, shape_b);
	mrb_hash_set(mrb, hash, B2_SYM(a), a);
	mrb_hash_set(mrb, hash, B2_SYM(b), b);
	return hash;
}

static mrb_value
world_contact_events(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const b2ContactEvents contact_events
	    = b2World_GetContactEvents(world->id);
	const mrb_value begin_ary
	    = mrb_ary_new_capa(mrb, contact_events.beginCount);
	for (int i = 0; i < contact_events.beginCount; ++i) {
		const b2ContactBeginTouchEvent *event
		    = &contact_events.beginEvents[i];
		const mrb_value event_hash = contact_bound_event_to_hash(mrb,
		    world, event->shapeIdA, event->shapeIdB);
		mrb_ary_push(mrb, begin_ary, event_hash);
	}
	const mrb_value end_ary
	    = mrb_ary_new_capa(mrb, contact_events.endCount);
	for (int i = 0; i < contact_events.endCount; ++i) {
		const b2ContactEndTouchEvent *event
		    = &contact_events.endEvents[i];
		const mrb_value event_hash = contact_bound_event_to_hash(mrb,
		    world, event->shapeIdA, event->shapeIdB);
		mrb_ary_push(mrb, end_ary, event_hash);
	}
	const mrb_value hit_ary
	    = mrb_ary_new_capa(mrb, contact_events.hitCount);
	for (int i = 0; i < contact_events.hitCount; ++i) {
		const b2ContactHitEvent *event = &contact_events.hitEvents[i];
		const mrb_value a = box2d_shape_wrap(mrb, event->shapeIdA);
		const mrb_value b = box2d_shape_wrap(mrb, event->shapeIdB);
		const mrb_value point = b2_vec_to_value(mrb, event->point);
		const mrb_value normal = b2_vec_to_value(mrb, event->normal);
		const mrb_value hash = mrb_hash_new(mrb);
		mrb_hash_set(mrb, hash, B2_SYM(a), a);
		mrb_hash_set(mrb, hash, B2_SYM(b), b);
		mrb_hash_set(mrb, hash, B2_SYM(point), point);
		mrb_hash_set(mrb, hash, B2_SYM(normal), normal);
		const mrb_value as = mrb_float_value(mrb, event->approachSpeed);
		mrb_hash_set(mrb, hash, B2_SYM(approach_speed), as);
		mrb_ary_push(mrb, hit_ary, hash);
	}
	const mrb_value result = mrb_hash_new(mrb);
	mrb_hash_set(mrb, result, B2_SYM(begin), begin_ary);
	mrb_hash_set(mrb, result, B2_SYM(end), end_ary);
	mrb_hash_set(mrb, result, B2_SYM(hit), hit_ary);
	return result;
}

static mrb_value
world_joint_events(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const b2JointEvents joint_events = b2World_GetJointEvents(world->id);
	const mrb_value ary = mrb_ary_new_capa(mrb, joint_events.count);
	for (int i = 0; i < joint_events.count; ++i) {
		const b2JointEvent *event = &joint_events.jointEvents[i];
		const mrb_value joint = box2d_joint_wrap(mrb, event->jointId);
		mrb_ary_push(mrb, ary, joint);
	}
	return ary;
}

struct result_context {
	mrb_state *mrb;
	const struct box2d_world *world;
	mrb_value block;
	mrb_value result;
};

/*
 * if there's a block, yield each shape to the block
 * if the block is nil, return an array of all shapes found
 */
static bool
overlap_result_fn(const b2ShapeId shape_id, void *user_data)
{
	const auto state = euler::util::State::get(mrb);
	const struct result_context *ctx = (struct result_context *)user_data;
	mrb_state *mrb = ctx->mrb;
	const mrb_value shape = box2d_shape_wrap(mrb, shape_id);
	if (mrb_nil_p(ctx->block)) {
		mrb_ary_push(ctx->mrb, ctx->result, shape);
		return true;
	}
	const mrb_value result = mrb_yield(ctx->mrb, ctx->block, shape);
	if (!mrb_bool(result)) return false;
	mrb_ary_push(ctx->mrb, ctx->result, shape);
	return true;
}

/* TODO: arg cleanup */
static mrb_value
world_overlap_aabb(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	static const char *KW_NAMES[] = {
		"lower_bound",
		"upper_bound",
	};
	enum {
		LOWER_BOUND = 0,
		UPPER_BOUND,
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
		.required = KW_COUNT,
		.table = kw_syms,
		.values = kw_values,
		.rest = NULL,
	};
	mrb_value block;
	mrb_get_args(mrb, "&:", &block, &kwargs);
	const struct b2Vec2 lower_bound
	    = value_to_b2_vec(mrb, kw_values[LOWER_BOUND]);
	const struct b2Vec2 upper_bound
	    = value_to_b2_vec(mrb, kw_values[UPPER_BOUND]);
	const struct b2AABB aabb = {
		.lowerBound = lower_bound,
		.upperBound = upper_bound,
	};
	struct result_context ctx = {
		.mrb = mrb,
		.world = world,
		.block = block,
		.result = mrb_nil_p(block) ? mrb_ary_new(mrb) : mrb_nil_value(),
	};
	const b2QueryFilter filter = b2DefaultQueryFilter();
	b2World_OverlapAABB(world->id, aabb, filter, overlap_result_fn, &ctx);
	return ctx.result;
}

static b2ShapeProxy
read_shape_proxy(mrb_state *mrb, const mrb_value value)
{
	const auto state = euler::util::State::get(mrb);
	/* should be a hash with two keys: points and radius */
	if (!mrb_hash_p(value))
		mrb_raise(mrb, E_TYPE_ERROR, "Expected hash for shape proxy");
	const mrb_value points_value = mrb_hash_get(mrb, value, B2_SYM(points));
	if (!mrb_array_p(points_value)) {
		mrb_raise(mrb, E_TYPE_ERROR,
		    "Expected array for shape proxy points");
	}
	if (RARRAY_LEN(points_value) == 0) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "Shape proxy points array cannot be empty");
	}
	const int n_points = (int)RARRAY_LEN(points_value);
	if (n_points > B2_MAX_POLYGON_VERTICES) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "Shape proxy points array exceeds maximum vertices");
	}
	b2Vec2 points[B2_MAX_POLYGON_VERTICES] = { 0 };
	for (int i = 0; i < n_points; ++i) {
		const mrb_value point_value = mrb_ary_entry(points_value, i);
		points[i] = value_to_b2_vec(mrb, point_value);
	}
	// radius is optional, defaults to 0
	float radius = 0.0f;
	if (mrb_hash_key_p(mrb, value, B2_SYM(radius))) {
		const mrb_value radius_value
		    = mrb_hash_get(mrb, value, B2_SYM(radius));
		radius = (float)mrb_float(radius_value);
	}
	return b2MakeProxy(points, n_points, radius);
}

static mrb_value
world_overlap_shape(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	/*
	 * three scenarios for arguments (ignoring block):
	 * 1. Array of shape proxy hashes
	 * 2. Single shape proxy hash
	 * 3. Arguments of shape proxy hash directly as keywords
	 */
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const mrb_value block = mrb_nil_value();
	static const char *KW_NAMES[] = {
		"points",
		"radius",
	};
	enum {
		POINTS = 0,
		RADIUS,
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
		.required = 1,
		.table = kw_syms,
		.values = kw_values,
		.rest = NULL,
	};
	mrb_get_args(mrb, "&:", &block, &kwargs);
	mrb_value hash = mrb_hash_new(mrb);
	mrb_value points = kw_values[POINTS];
	if (!mrb_array_p(points)) {
		mrb_raise(mrb, E_TYPE_ERROR,
		    "Expected array for shape proxy points");
	}
	if (RARRAY_LEN(points) == 0) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "Shape proxy points array cannot be empty");
	}
	if (RARRAY_LEN(points) > B2_MAX_POLYGON_VERTICES) {
		mrb_raisef(mrb, E_ARGUMENT_ERROR,
		    "Shape proxy points array exceeds maximum vertices (%d)",
		    B2_MAX_POLYGON_VERTICES);
	}
	mrb_hash_set(mrb, hash, B2_SYM(points), kw_values[POINTS]);
	if (!mrb_undef_p(kw_values[RADIUS]))
		mrb_hash_set(mrb, hash, B2_SYM(radius), kw_values[RADIUS]);
	const b2ShapeProxy sp = read_shape_proxy(mrb, hash);
	struct result_context ctx = {
		.mrb = mrb,
		.world = world,
		.block = block,
		.result = mrb_nil_p(block) ? mrb_ary_new(mrb) : mrb_nil_value(),
	};
	const b2QueryFilter filter = b2DefaultQueryFilter();
	b2World_OverlapShape(world->id, &sp, filter, overlap_result_fn, &ctx);
	return ctx.result;
}

static float
cast_result_fn(b2ShapeId id, b2Vec2 point, b2Vec2 normal, float fraction,
    void *context)
{
	const auto state = euler::util::State::get(mrb);
	const struct result_context *ctx = (struct result_context *)context;
	const mrb_value shape = box2d_shape_wrap(ctx->mrb, id);
	const mrb_value point_hash = b2_vec_to_value(ctx->mrb, point);
	const mrb_value normal_hash = b2_vec_to_value(ctx->mrb, normal);
	const mrb_value fraction_value = mrb_float_value(ctx->mrb, fraction);
	mrb_value args[] = { shape, point_hash, normal_hash, fraction_value };
	mrb_value result = mrb_nil_value();
	if (!try_call_block(ctx->mrb, ctx->block, args, 4, &result))
		return 0.0f; /* stop the ray cast on error */
	if (mrb_nil_p(result)) return 0.0f;
	const mrb_value float_value = mrb_ensure_float_type(ctx->mrb, result);
	return (float)mrb_float(float_value);
}

static void
read_cast_ray_args(mrb_state *mrb, struct b2Vec2 *origin,
    struct b2Vec2 *translation, b2QueryFilter *filter, mrb_value *block)
{
	const auto state = euler::util::State::get(mrb);
	static const char *KW_NAMES[] = {
		"origin",
		"translation",
	};
	enum {
		ORIGIN = 0,
		TRANSLATION,
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
		.required = KW_COUNT,
		.table = kw_syms,
		.values = kw_values,
		.rest = NULL,
	};
	if (block == NULL) mrb_get_args(mrb, ":", &kwargs);
	else mrb_get_args(mrb, "&:", block, &kwargs);
	*origin = value_to_b2_vec(mrb, kw_values[ORIGIN]);
	*translation = value_to_b2_vec(mrb, kw_values[TRANSLATION]);
	*filter = b2DefaultQueryFilter();
}

static mrb_value
world_cast_ray(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	b2Vec2 origin, translation;
	b2QueryFilter filter;
	mrb_value block = mrb_nil_value();
	read_cast_ray_args(mrb, &origin, &translation, &filter, &block);
	struct result_context ctx = {
		.mrb = mrb,
		.world = world,
		.block = block,
		.result = mrb_nil_p(block) ? mrb_ary_new(mrb) : mrb_nil_value(),
	};
	b2World_CastRay(world->id, origin, translation, filter, cast_result_fn,
	    &ctx);
	return ctx.result;
}

static mrb_value
world_cast_ray_closest(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	b2Vec2 origin, translation;
	b2QueryFilter filter;
	read_cast_ray_args(mrb, &origin, &translation, &filter, NULL);
	b2RayResult result
	    = b2World_CastRayClosest(world->id, origin, translation, filter);
	mrb_value shape = mrb_nil_value();
	if (B2_IS_NON_NULL(result.shapeId))
		shape = box2d_shape_wrap(mrb, result.shapeId);
	const mrb_value out = mrb_hash_new_capa(mrb, 5);
	mrb_hash_set(mrb, out, B2_SYM(shape), shape);
	mrb_hash_set(mrb, out, B2_SYM(point),
	    b2_vec_to_value(mrb, result.point));
	mrb_hash_set(mrb, out, B2_SYM(normal),
	    b2_vec_to_value(mrb, result.normal));
	mrb_hash_set(mrb, out, B2_SYM(fraction),
	    mrb_float_value(mrb, result.fraction));
	mrb_hash_set(mrb, out, B2_SYM(hit), mrb_bool_value(result.hit));
	return out;
}

static mrb_value
world_cast_shape(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_value block = mrb_nil_value();
	static const char *KW_NAMES[] = {
		"shape",
		"translation",
	};
	enum {
		SHAPE = 0,
		TRANSLATION,
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
		.required = KW_COUNT,
		.table = kw_syms,
		.values = kw_values,
		.rest = NULL,
	};
	mrb_get_args(mrb, "&:", &block, &kwargs);
	const b2ShapeProxy sp = read_shape_proxy(mrb, kw_values[SHAPE]);
	const b2Vec2 translation = value_to_b2_vec(mrb, kw_values[TRANSLATION]);
	struct result_context ctx = {
		.mrb = mrb,
		.world = world,
		.block = block,
		.result = mrb_nil_p(block) ? mrb_ary_new(mrb) : mrb_nil_value(),
	};
	const struct b2QueryFilter filter = b2DefaultQueryFilter();
	b2World_CastShape(world->id, &sp, translation, filter, cast_result_fn,
	    &ctx);
	return ctx.result;
}

static b2Capsule
read_capsule_value(mrb_state *mrb, const mrb_value value)
{
	const auto state = euler::util::State::get(mrb);
	if (!mrb_hash_p(value)) {
		mrb_raise(mrb, E_TYPE_ERROR, "Expected hash for capsule");
	}

	const mrb_value points = mrb_hash_get(mrb, value, B2_SYM(points));
	if (!mrb_array_p(points)) {
		mrb_raise(mrb, E_TYPE_ERROR,
		    "Expected array for capsule points");
	}
	if (RARRAY_LEN(points) != 2) {
		mrb_raise(mrb, E_ARGUMENT_ERROR,
		    "Expected array of length 2 for capsule points");
	}
	const struct b2Vec2 center1
	    = value_to_b2_vec(mrb, mrb_ary_entry(points, 0));
	const struct b2Vec2 center2
	    = value_to_b2_vec(mrb, mrb_ary_entry(points, 1));
	const mrb_value radius_value = mrb_hash_get(mrb, value, B2_SYM(radius));
	if (!mrb_float_p(radius_value) && !mrb_integer_p(radius_value)) {
		mrb_raise(mrb, E_TYPE_ERROR,
		    "Expected float for capsule radius");
	}
	const float radius = (float)mrb_float(radius_value);
	return (b2Capsule) {
		.center1 = center1,
		.center2 = center2,
		.radius = radius,
	};
}

static mrb_value
world_cast_mover(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_value block = mrb_nil_value();
	static const char *KW_NAMES[] = {
		"mover",
		"translation",
	};
	enum {
		MOVER = 0,
		TRANSLATION,
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
		.required = KW_COUNT,
		.table = kw_syms,
		.values = kw_values,
		.rest = NULL,
	};
	mrb_get_args(mrb, "&:", &block, &kwargs);
	const b2Capsule mover = read_capsule_value(mrb, kw_values[MOVER]);
	const b2Vec2 translation = value_to_b2_vec(mrb, kw_values[TRANSLATION]);
	const b2QueryFilter filter = b2DefaultQueryFilter();
	const float out
	    = b2World_CastMover(world->id, &mover, translation, filter);
	return mrb_float_value(mrb, out);
}

static bool
plane_result_fn(const b2ShapeId shape_id, const b2PlaneResult *plane,
    void *context)
{
	const auto state = euler::util::State::get(mrb);
	struct result_context *ctx = (struct result_context *)context;
	mrb_state *mrb = ctx->mrb;
	const mrb_value shape = box2d_shape_wrap(ctx->mrb, shape_id);
	if (mrb_nil_p(ctx->block)) {
		mrb_ary_push(ctx->mrb, ctx->result, shape);
		return true;
	}
	const mrb_value plane_result = mrb_hash_new_capa(mrb, 3);
	const mrb_value col_plane = mrb_hash_new_capa(mrb, 2);
	mrb_hash_set(mrb, col_plane, B2_SYM(normal),
	    b2_vec_to_value(mrb, plane->plane.normal));
	mrb_hash_set(mrb, col_plane, B2_SYM(offset),
	    mrb_float_value(mrb, plane->plane.offset));
	mrb_hash_set(mrb, plane_result, B2_SYM(plane), col_plane);
	mrb_hash_set(mrb, plane_result, B2_SYM(point),
	    b2_vec_to_value(mrb, plane->point));
	mrb_hash_set(mrb, plane_result, B2_SYM(hit),
	    mrb_bool_value(plane->hit));
	// const mrb_value result = mrb_yield(ctx->mrb, ctx->block, shape);
	mrb_value args[2] = { shape, plane_result };
	mrb_value result;
	if (!try_call_block(mrb, ctx->block, args, 2, &result)) return false;
	if (mrb_false_p(result)) return false;
	if (!mrb_nil_p(result)) mrb_ary_push(ctx->mrb, ctx->result, shape);
	return true;
}

static mrb_value
world_collide_mover(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_value block = mrb_nil_value();
	mrb_value mover_value = mrb_nil_value();
	mrb_get_args(mrb, "H&", &mover_value, &block);
	const b2Capsule mover = read_capsule_value(mrb, mover_value);
	struct result_context ctx = {
		.mrb = mrb,
		.world = world,
		.block = block,
		.result = mrb_nil_p(block) ? mrb_ary_new(mrb) : mrb_nil_value(),
	};
	const b2QueryFilter filter = b2DefaultQueryFilter();
	b2World_CollideMover(world->id, &mover, filter, plane_result_fn, &ctx);
	return ctx.result;
}

static mrb_value
world_enable_sleeping(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_bool value;
	mrb_get_args(mrb, "b", &value);
	b2World_EnableSleeping(world->id, value);
	return mrb_nil_value();
}

static mrb_value
world_is_sleeping_enabled(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const bool enabled = b2World_IsSleepingEnabled(world->id);
	return mrb_bool_value(enabled);
}

static mrb_value
world_enable_continuous(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_bool value;
	mrb_get_args(mrb, "b", &value);
	b2World_EnableContinuous(world->id, value);
	return mrb_nil_value();
}

static mrb_value
world_is_continuous_enabled(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const bool enabled = b2World_IsContinuousEnabled(world->id);
	return mrb_bool_value(enabled);
}

static mrb_value
world_set_restitution_threshold(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_float value;
	mrb_get_args(mrb, "f", &value);
	b2World_SetRestitutionThreshold(world->id, value);
	return mrb_nil_value();
}

static mrb_value
world_restitution_threshold(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const float threshold = b2World_GetRestitutionThreshold(world->id);
	return mrb_float_value(mrb, threshold);
}

static mrb_value
world_set_hit_event_threshold(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_float value;
	mrb_get_args(mrb, "f", &value);
	b2World_SetHitEventThreshold(world->id, value);
	return mrb_nil_value();
}

static mrb_value
world_hit_event_threshold(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const float threshold = b2World_GetHitEventThreshold(world->id);
	return mrb_float_value(mrb, threshold);
}

static void
lock_check(mrb_state *mrb, const char *fn)
{
	const auto state = euler::util::State::get(mrb);
	if (box2d_can_lock_state(mrb)) return;
	mrb_raisef(mrb, E_ARGUMENT_ERROR,
	    "World#%s is not allowed unless locking is enabled", fn);
}

static bool
custom_filter_fn(b2ShapeId shapeA, b2ShapeId shapeB, void *context)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = (struct box2d_world *)context;
	mrb_state *mrb = world->mrb;
	lock_check(mrb, "on_custom_filter");
	box2d_lock_state(mrb);
	const mrb_value shape_a = box2d_shape_wrap(mrb, shapeA);
	const mrb_value shape_b = box2d_shape_wrap(mrb, shapeB);
	mrb_value args[2] = { shape_a, shape_b };
	mrb_value result;
	box2d_unlock_state(mrb);
	if (!try_call_block(mrb, world->custom_filter_fn, args, 2, &result))
		return false;
	return mrb_bool(result);
}

static mrb_value
world_on_custom_filter(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	lock_check(mrb, "on_custom_filter");
	struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_value block;
	mrb_get_args(mrb, "&!", &block);
	world->custom_filter_fn = block;
	b2World_SetCustomFilterCallback(world->id, custom_filter_fn,
	    (void *)world);
	return mrb_nil_value();
}

static bool
pre_solve_fn(b2ShapeId shapeIdA, b2ShapeId shapeIdB, b2Vec2 point,
    b2Vec2 normal, void *context)
{
	const auto state = euler::util::State::get(mrb);
	struct box2d_world *world = (struct box2d_world *)context;
	mrb_state *mrb = world->mrb;
	lock_check(mrb, "on_pre_solve");
	box2d_lock_state(mrb);
	const mrb_value shape_a = box2d_shape_wrap(mrb, shapeIdA);
	const mrb_value shape_b = box2d_shape_wrap(mrb, shapeIdB);
	const mrb_value point_vec = b2_vec_to_value(mrb, point);
	const mrb_value normal_vec = b2_vec_to_value(mrb, normal);
	mrb_value args[4] = {
		shape_a,
		shape_b,
		point_vec,
		normal_vec,
	};
	mrb_value result;
	box2d_unlock_state(mrb);
	if (!try_call_block(mrb, world->pre_solve_fn, args, 4, &result))
		return false;
	return mrb_bool(result);
}

static mrb_value
world_on_pre_solve(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_value block;
	mrb_get_args(mrb, "&!", &block);
	world->pre_solve_fn = block;
	b2World_SetPreSolveCallback(world->id, pre_solve_fn, world);
	return mrb_nil_value();
}

static mrb_value
world_set_gravity(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_value value;
	mrb_get_args(mrb, "H", &value);
	const struct b2Vec2 gravity = value_to_b2_vec(mrb, value);
	b2World_SetGravity(world->id, gravity);
	return mrb_nil_value();
}

static mrb_value
world_gravity(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const struct b2Vec2 gravity = b2World_GetGravity(world->id);
	return b2_vec_to_value(mrb, gravity);
}

static mrb_value
world_explode(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	static const char *KW_NAMES[] = {
		"position",
		"radius",
		"falloff",
		"impulse_per_length",
	};
	enum {
		POSITION = 0,
		RADIUS,
		FALLOFF,
		IMPULSE_PER_LENGTH,
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
	b2ExplosionDef def = b2DefaultExplosionDef();
	if (!mrb_undef_p(kw_values[POSITION]))
		def.position = value_to_b2_vec(mrb, kw_values[POSITION]);
	if (!mrb_undef_p(kw_values[RADIUS]))
		def.radius = (float)mrb_float(kw_values[RADIUS]);
	if (!mrb_undef_p(kw_values[FALLOFF]))
		def.falloff = (float)mrb_float(kw_values[FALLOFF]);
	if (!mrb_undef_p(kw_values[IMPULSE_PER_LENGTH])) {
		def.impulsePerLength
		    = (float)mrb_float(kw_values[IMPULSE_PER_LENGTH]);
	}
	b2World_Explode(world->id, &def);
	return mrb_nil_value();
}

static mrb_value
world_set_contact_tuning(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	static const char *KW_NAMES[] = {
		"hertz",
		"damping_ratio",
		"push_speed",
	};
	enum {
		HERTZ = 0,
		DAMPING_RATIO,
		PUSH_SPEED,
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
		.required = KW_COUNT,
		.table = kw_syms,
		.values = kw_values,
		.rest = NULL,
	};
	mrb_get_args(mrb, ":", &kwargs);
	const float hertz = (float)mrb_float(kw_values[HERTZ]);
	const float damping_ratio = (float)mrb_float(kw_values[DAMPING_RATIO]);
	const float push_speed = (float)mrb_float(kw_values[PUSH_SPEED]);
	b2World_SetContactTuning(world->id, hertz, damping_ratio, push_speed);
	return mrb_nil_value();
}

static mrb_value
world_set_maximum_linear_speed(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	mrb_float value;
	mrb_get_args(mrb, "f", &value);
	b2World_SetMaximumLinearSpeed(world->id, value);
	return mrb_nil_value();
}

static mrb_value
world_maximum_linear_speed(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const float speed = b2World_GetMaximumLinearSpeed(world->id);
	return mrb_float_value(mrb, speed);
}

static mrb_value
world_awake_body_count(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	const int count = b2World_GetAwakeBodyCount(world->id);
	return mrb_fixnum_value(count);
}

static b2MotionLocks
read_motion_locks_args(mrb_state *mrb, mrb_value locks_value)
{
	const auto state = euler::util::State::get(mrb);
	if (!mrb_array_p(locks_value)) {
		mrb_raise(mrb, E_TYPE_ERROR,
		    "Expected array of locked directions for "
		    "motion_locks");
	}
	bool x = false, y = false, z = false;
	for (mrb_int i = 0; i < RARRAY_LEN(locks_value); ++i) {
		const mrb_value a = mrb_ary_entry(locks_value, i);
		if (!mrb_symbol_p(a)) {
			mrb_raise(mrb, E_TYPE_ERROR,
			    "Expected symbols in motion_locks array");
		}
		const mrb_sym lock_sym = mrb_symbol(a);
		if (mrb_equal(mrb, a, B2_SYM(linear_x))) x = true;
		if (mrb_equal(mrb, a, B2_SYM(x))) x = true;
		if (mrb_equal(mrb, a, B2_SYM(linear_y))) y = true;
		if (mrb_equal(mrb, a, B2_SYM(y))) y = true;
		if (mrb_equal(mrb, a, B2_SYM(angular_z))) z = true;
		if (mrb_equal(mrb, a, B2_SYM(z))) z = true;
	}
	return (b2MotionLocks) {
		.linearX = x,
		.linearY = y,
		.angularZ = z,
	};
}

static mrb_value
world_create_body(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const struct box2d_world *world = box2d_world_unwrap(mrb, self);
	static const char *KW_NAMES[] = {
		"type",
		"rotation",
		"angle",
		"linear_velocity",
		"angular_velocity",
		"linear_damping",
		"angular_damping",
		"gravity_scale",
		"sleep_threshold",
		"motion_locks",
		"enable_sleep",
		"awake",
		"bullet",
		"enabled",
		"allow_fast_rotation",
	};
	enum {
		TYPE = 0,
		POSITION,
		ROTATION,
		LINEAR_VELOCITY,
		ANGULAR_VELOCITY,
		LINEAR_DAMPING,
		ANGULAR_DAMPING,
		GRAVITY_SCALE,
		SLEEP_THRESHOLD,
		MOTION_LOCKS,
		ENABLE_SLEEP,
		AWAKE,
		BULLET,
		ENABLED,
		ALLOW_FAST_ROTATION,

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
	b2BodyDef def = b2DefaultBodyDef();
	if (!mrb_nil_p(kw_values[TYPE])) {
		def.type = box2d_parse_body_type(mrb, kw_values[TYPE]);
	}
	if (!mrb_nil_p(kw_values[POSITION])) {
		def.position = value_to_b2_vec(mrb, kw_values[POSITION]);
	}
	if (!mrb_nil_p(kw_values[TYPE]))
		def.rotation = value_to_b2_rot(mrb, kw_values[ROTATION]);
	if (!mrb_nil_p(kw_values[LINEAR_VELOCITY])) {
		def.linearVelocity
		    = value_to_b2_vec(mrb, kw_values[LINEAR_VELOCITY]);
	}
	if (!mrb_nil_p(kw_values[ANGULAR_VELOCITY])) {
		def.angularVelocity
		    = (float)mrb_float(kw_values[ANGULAR_VELOCITY]);
	}
	if (!mrb_nil_p(kw_values[LINEAR_DAMPING])) {
		def.linearDamping = (float)mrb_float(kw_values[LINEAR_DAMPING]);
	}
	if (!mrb_nil_p(kw_values[ANGULAR_DAMPING])) {
		def.angularDamping
		    = (float)mrb_float(kw_values[ANGULAR_DAMPING]);
	}
	if (!mrb_nil_p(kw_values[GRAVITY_SCALE])) {
		def.gravityScale = (float)mrb_float(kw_values[GRAVITY_SCALE]);
	}
	if (!mrb_nil_p(kw_values[SLEEP_THRESHOLD])) {
		def.sleepThreshold
		    = (float)mrb_float(kw_values[SLEEP_THRESHOLD]);
	}
	if (!mrb_nil_p(kw_values[MOTION_LOCKS])) {
		// expect an array of symbols
		const mrb_value locks_value = kw_values[MOTION_LOCKS];
		def.motionLocks = read_motion_locks_args(mrb, locks_value);
	}
	if (!mrb_nil_p(kw_values[ENABLE_SLEEP])) {
		def.enableSleep = mrb_bool(kw_values[ENABLE_SLEEP]);
	}
	if (!mrb_nil_p(kw_values[AWAKE])) {
		def.isAwake = mrb_bool(kw_values[AWAKE]);
	}
	if (!mrb_nil_p(kw_values[BULLET])) {
		def.isBullet = mrb_bool(kw_values[BULLET]);
	}
	if (!mrb_nil_p(kw_values[ENABLED])) {
		def.isEnabled = mrb_bool(kw_values[ENABLED]);
	}
	if (!mrb_nil_p(kw_values[ALLOW_FAST_ROTATION])) {
		def.allowFastRotation
		    = mrb_bool(kw_values[ALLOW_FAST_ROTATION]);
	}
	const b2BodyId body_id = b2CreateBody(world->id, &def);
	return box2d_body_wrap(mrb, body_id);
}

struct RClass *
box2d_world_init(mrb_state *mrb, struct RClass *mod)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *world
	    = mrb_define_class_under(mrb, mod, "World", mrb->object_class);
	MRB_SET_INSTANCE_TT(world, MRB_TT_CDATA);
	mrb_define_method(mrb, world, "valid?", world_is_valid,
	    MRB_ARGS_NONE());
	mrb_define_method(mrb, world, "step", world_step, MRB_ARGS_KEY(2, 0));
	mrb_define_method(mrb, world, "body_events", world_body_events,
	    MRB_ARGS_NONE());
	mrb_define_method(mrb, world, "sensor_events", world_sensor_events,
	    MRB_ARGS_NONE());
	mrb_define_method(mrb, world, "contact_events", world_contact_events,
	    MRB_ARGS_NONE());
	mrb_define_method(mrb, world, "joint_events", world_joint_events,
	    MRB_ARGS_NONE());
	mrb_define_method(mrb, world, "overlap_aabb", world_overlap_aabb,
	    MRB_ARGS_KEY(2, 0));
	mrb_define_method(mrb, world, "overlap_shape", world_overlap_shape,
	    MRB_ARGS_KEY(2, 0));
	mrb_define_method(mrb, world, "cast_ray", world_cast_ray,
	    MRB_ARGS_KEY(2, 0));
	mrb_define_method(mrb, world, "cast_ray_closest",
	    world_cast_ray_closest, MRB_ARGS_KEY(2, 0));
	mrb_define_method(mrb, world, "cast_shape", world_cast_shape,
	    MRB_ARGS_KEY(2, 0));
	mrb_define_method(mrb, world, "cast_mover", world_cast_mover,
	    MRB_ARGS_KEY(2, 0));
	mrb_define_method(mrb, world, "collide_mover", world_collide_mover,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, world, "enable_sleeping", world_enable_sleeping,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, world, "sleeping_enabled?",
	    world_is_sleeping_enabled, MRB_ARGS_NONE());
	mrb_define_method(mrb, world,
	    "continuous_enabled=", world_enable_continuous, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, world, "continuous_enabled",
	    world_is_continuous_enabled, MRB_ARGS_NONE());
	mrb_alias_method(mrb, world, mrb_intern_lit(mrb, "enable_continuous"),
	    mrb_intern_lit(mrb, "continuous_enabled="));
	mrb_define_method(mrb, world,
	    "restitution_threshold=", world_set_restitution_threshold,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, world, "restitution_threshold",
	    world_restitution_threshold, MRB_ARGS_NONE());
	mrb_define_method(mrb, world,
	    "hit_event_threshold=", world_set_hit_event_threshold,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, world, "hit_event_threshold",
	    world_hit_event_threshold, MRB_ARGS_NONE());
	mrb_define_method(mrb, world, "on_custom_filter",
	    world_on_custom_filter, MRB_ARGS_BLOCK());
	mrb_define_method(mrb, world, "on_pre_solve", world_on_pre_solve,
	    MRB_ARGS_BLOCK());
	mrb_define_method(mrb, world, "gravity=", world_set_gravity,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, world, "gravity", world_gravity,
	    MRB_ARGS_NONE());
	mrb_define_method(mrb, world, "explode", world_explode,
	    MRB_ARGS_KEY(4, 0));
	mrb_define_method(mrb, world,
	    "contact_tuning=", world_set_contact_tuning, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, world,
	    "maximum_linear_speed=", world_set_maximum_linear_speed,
	    MRB_ARGS_REQ(1));
	mrb_define_method(mrb, world, "maximum_linear_speed",
	    world_maximum_linear_speed, MRB_ARGS_NONE());
	mrb_define_method(mrb, world, "awake_body_count",
	    world_awake_body_count, MRB_ARGS_NONE());
	mrb_define_method(mrb, world, "create_body", world_create_body,
	    MRB_ARGS_KEY(15, 0));
	return world;
}

void
euler::physics::World::init(const util::Reference<util::State> &state)
{
	const auto state = euler::util::State::get(mrb);

}