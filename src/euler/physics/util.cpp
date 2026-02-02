/* SPDX-License-Identifier: ISC */

#include <cassert>
#include <complex>
#include <utility>

#include "euler/physics/contact.h"
#include "euler/physics/util.h"
#include "euler/util/math.h"

mrb_value
euler::physics::b2_vec_to_value(mrb_state *mrb, b2Vec2 vec)
{
	/* TODO: allow dictation of output format at compile time */
	const auto state = util::State::get(mrb);
	const auto arr = state->mrb()->ary_new_capa(2);
	state->mrb()->ary_push(arr, state->mrb()->float_value(vec.x));
	state->mrb()->ary_push(arr, state->mrb()->float_value(vec.y));
	return arr;
}

b2Vec2
euler::physics::value_to_b2_vec(mrb_state *mrb, mrb_value val)
{
	const auto state = util::State::get(mrb);
	/*
	 * This can be one of a few formats:
	 * - A hash with an :x and :y key
	 * - A 2x1 array
	 * - A complex number
	 * - (if enabled in build) a 2x1 vector from euler::math
	 */
	if (mrb_array_p(val)) {
		if (RARRAY_LEN(val) != 2) {
			state->mrb()->raise(E_ARGUMENT_ERROR,
			    "Array must have exactly two elements");
		}
		const auto x_val = state->mrb()->ensure_float_type(
		    state->mrb()->ary_entry(val, 0));
		const auto y_val = state->mrb()->ensure_float_type(
		    state->mrb()->ary_entry(val, 1));
		return b2Vec2 {
			.x = static_cast<float>(mrb_float(x_val)),
			.y = static_cast<float>(mrb_float(y_val)),
		};
	}
	if (mrb_hash_p(val)) {
		const auto x_key = B2_SYM(x);
		const auto y_key = B2_SYM(y);
		const auto x_val = state->mrb()->hash_get(val, x_key);
		const auto y_val = state->mrb()->hash_get(val, y_key);
		if (mrb_nil_p(x_val) || mrb_nil_p(y_val)) {
			state->mrb()->raise(E_ARGUMENT_ERROR,
			    "Hash must have :x and :y keys");
		}
		const auto x_float = state->mrb()->ensure_float_type(x_val);
		const auto y_float = state->mrb()->ensure_float_type(y_val);
		return b2Vec2 {
			.x = static_cast<float>(mrb_float(x_float)),
			.y = static_cast<float>(mrb_float(y_float)),
		};
	}
	if (const auto complex_class = state->mrb()->class_get("Complex");
	    state->mrb()->obj_is_instance_of(val, complex_class)) {
		const auto complex
		    = util::unwrap_complex<float>(state->mrb(), val);
		return b2Vec2 {
			.x = (complex.real()),
			.y = (complex.imag()),
		};
	}
#ifdef EULER_MATH
	// TODO
#endif
	state->mrb()->raise(E_ARGUMENT_ERROR,
	    "Invalid argument type for vector");
	std::unreachable();
}

b2Rot
euler::physics::value_to_b2_rot(mrb_state *mrb, mrb_value val)
{
	/*
	 * This can be one of a few formats:
	 * - A hash with a :sin and :cos key
	 * - A 2x1 array (cos, sin)
	 * - A complex number
	 * - (if enabled in build) a 2x1 vector from euler::math
	 * - a numeric angle in radians
	 */
	const auto state = util::State::get(mrb);
	if (mrb_array_p(val)) {
		if (RARRAY_LEN(val) != 2) {
			state->mrb()->raise(E_ARGUMENT_ERROR,
			    "Array must have exactly two elements");
		}
		const auto c_val = state->mrb()->ensure_float_type(
		    state->mrb()->ary_entry(val, 0));
		const auto s_val = state->mrb()->ensure_float_type(
		    state->mrb()->ary_entry(val, 1));
		return b2Rot {
			.c = static_cast<float>(mrb_float(c_val)),
			.s = static_cast<float>(mrb_float(s_val)),
		};
	}
	if (mrb_hash_p(val)) {
		const auto c_key = B2_SYM(cos);
		const auto s_key = B2_SYM(sin);
		const auto c_val = state->mrb()->hash_get(val, c_key);
		const auto s_val = state->mrb()->hash_get(val, s_key);
		if (mrb_nil_p(s_val) || mrb_nil_p(c_val)) {
			state->mrb()->raise(E_ARGUMENT_ERROR,
			    "Hash must have :sin and :cos keys");
		}
		const auto c_float = state->mrb()->ensure_float_type(c_val);
		const auto s_float = state->mrb()->ensure_float_type(s_val);
		return b2Rot {
			.c = static_cast<float>(mrb_float(c_float)),
			.s = static_cast<float>(mrb_float(s_float)),
		};
	}
	if (util::is_complex(mrb, val)) {
		const auto s_val = state->mrb()->ensure_float_type(
		    state->mrb()->funcall(val, "imag", 0));
		const auto c_val = state->mrb()->ensure_float_type(
		    state->mrb()->funcall(val, "real", 0));
		return b2Rot {
			.c = static_cast<float>(mrb_float(c_val)),
			.s = static_cast<float>(mrb_float(s_val)),
		};
	}
#ifdef EULER_MATH
	// TODO
#endif
	if (util::is_numeric(mrb, val)) {
		assert(!util::is_complex(mrb, val));
		const float angle = util::unwrap_num<float>(state, val);
		return b2Rot {
			.c = std::cos(angle),
			.s = std::sin(angle),
		};
	}
	state->mrb()->raise(E_ARGUMENT_ERROR,
	    "Invalid argument type for rotation");
	std::unreachable();
}

mrb_value
euler::physics::b2_rot_to_value(mrb_state *mrb, b2Rot rot)
{
	// return in radians by default
	const auto state = util::State::get(mrb);
	const float angle = std::atan2(rot.s, rot.c);
	return state->mrb()->float_value(angle);
}

mrb_value
euler::physics::b2_transform_to_value(mrb_state *mrb, b2Transform tform)
{
	const auto state = util::State::get(mrb);
	const auto hash = state->mrb()->hash_new_capa(2);
	const auto position = b2_vec_to_value(mrb, tform.p);
	const auto rotation = b2_rot_to_value(mrb, tform.q);
	state->mrb()->hash_set(hash, B2_SYM(position), position);
	state->mrb()->hash_set(hash, B2_SYM(rotation), rotation);
	return hash;
}

b2Transform
euler::physics::value_to_b2_transform(mrb_state *mrb, mrb_value val)
{
	const auto state = util::State::get(mrb);
	if (!mrb_hash_p(val)) {
		state->mrb()->raise(E_ARGUMENT_ERROR,
		    "Transform must be a hash with :position and :rotation "
		    "keys");
	}
	const auto pos_value = state->mrb()->hash_get(val, B2_SYM(position));
	const auto rot_value = state->mrb()->hash_get(val, B2_SYM(rotation));
	return b2Transform {
		.p = value_to_b2_vec(mrb, pos_value),
		.q = value_to_b2_rot(mrb, rot_value),
	};
}

float
euler::physics::hash_read_float(mrb_state *mrb, mrb_value hash, mrb_value key,
    float default_value)
{
	const auto state = util::State::get(mrb);
	const auto val = state->mrb()->hash_get(hash, key);
	if (mrb_nil_p(val)) { return default_value; }
	const auto float_val = state->mrb()->ensure_float_type(val);
	return static_cast<float>(mrb_float(float_val));
}

mrb_int
euler::physics::hash_read_int(mrb_state *mrb, mrb_value hash, mrb_value key,
    mrb_int default_value)
{
	const auto state = util::State::get(mrb);
	const auto val = state->mrb()->hash_get(hash, key);
	if (mrb_nil_p(val)) return default_value;
	const auto int_val = state->mrb()->ensure_integer_type(val);
	return mrb_int(mrb, int_val);
}

mrb_value
euler::physics::contact_data_to_value(mrb_state *mrb, const b2ContactData *data)
{
	auto contact = Contact::Data::from_b2(*data);
	return contact.wrap(mrb);
}

mrb_value
euler::physics::surface_material_to_value(mrb_state *mrb,
    const b2SurfaceMaterial *mat)
{
	const auto state = util::State::get(mrb);
	const auto hash = state->mrb()->hash_new_capa(5);
	state->mrb()->hash_set(hash, B2_SYM(friction),
	    state->mrb()->float_value(mat->friction));
	state->mrb()->hash_set(hash, B2_SYM(restitution),
	    state->mrb()->float_value(mat->restitution));
	state->mrb()->hash_set(hash, B2_SYM(rolling_resistance),
	    state->mrb()->float_value(mat->rollingResistance));
	state->mrb()->hash_set(hash, B2_SYM(tangent_speed),
	    state->mrb()->float_value(mat->tangentSpeed));
	state->mrb()->hash_set(hash, B2_SYM(user_material_id),
	    state->mrb()->int_value(static_cast<mrb_int>(mat->userMaterialId)));
	return hash;
}

b2SurfaceMaterial
euler::physics::value_to_surface_material(mrb_state *mrb, mrb_value hash)
{
	b2SurfaceMaterial sm = b2DefaultSurfaceMaterial();
	const auto state = util::State::get(mrb);
	if (state->mrb()->hash_key_p(hash, B2_SYM(surface_friction))) {
		sm.friction = hash_read_float(mrb, hash,
		    B2_SYM(surface_friction), sm.friction);
	}
	if (state->mrb()->hash_key_p(hash, B2_SYM(restitution))) {
		sm.restitution = hash_read_float(mrb, hash, B2_SYM(restitution),
		    sm.restitution);
	}
	if (state->mrb()->hash_key_p(hash, B2_SYM(rolling_resistance))) {
		sm.rollingResistance = hash_read_float(mrb, hash,
		    B2_SYM(rolling_resistance), sm.rollingResistance);
	}
	if (state->mrb()->hash_key_p(hash, B2_SYM(tangent_speed))) {
		sm.tangentSpeed = hash_read_float(mrb, hash,
		    B2_SYM(tangent_speed), sm.tangentSpeed);
	}
	if (state->mrb()->hash_key_p(hash, B2_SYM(user_material_id))) {
		sm.userMaterialId = static_cast<uint64_t>(
		    hash_read_int(mrb, hash, B2_SYM(user_material_id),
			static_cast<mrb_int>(sm.userMaterialId)));
	}
	return sm;
}

float
euler::physics::coerce_float(mrb_state *mrb, mrb_value value)
{
	const auto state = util::State::get(mrb);
	const auto float_val = state->mrb()->ensure_float_type(value);
	return static_cast<float>(mrb_float(float_val));
}