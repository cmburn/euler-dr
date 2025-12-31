/* SPDX-License-Identifier: ISC */

#include "euler/physics/chain.h"

#include <mruby/array.h>
#include <mruby/class.h>
#include <mruby/hash.h>

#include "euler/physics/ext.h"
#include "euler/physics/shape.h"
#include "euler/physics/util.h"
#include "euler/physics/world.h"
#include "euler/util/error.h"

using euler::physics::Chain;

/**
 * @overload Euler::Physics::Chain#surface_materials
 *   @return [Array<Hash>] An array of surface material hashes for each segment
 *           in the chain. Each hash has the following keys:
 *           - :friction [Float]
 *           - :restitution [Float]
 *           - :rolling_resistance [Float]
 *           - :tangent_speed [Float]
 *           - :user_material_id [Integer]
 *   @note This returns a new array; modifying the returned array will not
 *         change the chain's surface materials. See
 *         {Euler::Physics::Chain#set_surface_materials} for changing an
 * individual segment's surface material, or
 * {Euler::Physics::Chain#surface_materials=} for changing all surface materials
 * at once.
 */
static mrb_value
chain_surface_materials(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto chain = Chain::unwrap(mrb, self);
	const auto materials = chain->surface_materials();
	const mrb_value out = mrb_ary_new_capa(mrb, materials.size());
	for (const auto &sm : materials) {
		const mrb_value mat
		    = euler::physics::surface_material_to_value(mrb, &sm);
		mrb_ary_push(mrb, out, mat);
	}
	return out;
}

/**
 * @overload Euler::Physics::Chain#surface_materials=(materials)
 *   @param materials [Array<Hash>] An array of surface material hashes to set
 *          for the chain. This array must either be of length 1, in which
 *          case the single material is applied to all segments, or of length
 *          equal to the number of segments in the chain. Each hash has the
 *          following keys:
 *          - :friction [Float]
 *          - :restitution [Float]
 *          - :rolling_resistance [Float]
 *          - :tangent_speed [Float]
 *          - :user_material_id [Integer]
 */
static mrb_value
chain_set_surface_materials(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto chain = Chain::unwrap(mrb, self);
	mrb_value array;
	mrb_get_args(mrb, "A", &array);
	const mrb_int len = RARRAY_LEN(array);
	const int chain_len = chain->surface_material_count();
	if (len != 1 && len != chain_len) {
		throw euler::util::make_error<euler::util::ArgumentError>(state,
		    "Expected assignment of array of length 1 or {} for chain "
		    "of length {}",
		    chain_len, chain_len);
	}
	const bool single = (len == 1);
	for (int i = 0; i < chain_len; ++i) {
		const mrb_value item = mrb_ary_ref(mrb, array, single ? 0 : i);
		const b2SurfaceMaterial sm
		    = euler::physics::value_to_surface_material(mrb, item);
		chain->set_surface_material(sm, i);
	}
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Chain#set_surface_material(material, index)
 *   @param material [Hash] A surface material hash to set for the segment at
 *          the specified index. The hash has the following keys:
 *          - :friction [Float]
 *          - :restitution [Float]
 *          - :rolling_resistance [Float]
 *          - :tangent_speed [Float]
 *          - :user_material_id [Integer]
 *   @param index [Integer] The index of the segment to set the material for.
 *   @return [void]
 */
static mrb_value
chain_set_surface_material(mrb_state *mrb, const mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto chain = Chain::unwrap(mrb, self);
	mrb_value material_value;
	mrb_int index;
	mrb_get_args(mrb, "oi", &material_value, &index);
	const b2SurfaceMaterial sm
	    = euler::physics::value_to_surface_material(mrb, material_value);
	chain->set_surface_material(sm, static_cast<int>(index));
	return mrb_nil_value();
}

/**
 * @overload Euler::Physics::Chain#world
 *   @return [Euler::Physics::World] The world that owns this chain shape.
 */
static mrb_value
chain_world(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto chain = Chain::unwrap(mrb, self);
	return chain->world()->wrap(state);
}

/**
 * @overload Euler::Physics::Chain#segments
 *   @return [Array<Euler::Physics::Shape>] An array of the segment shapes that
 * make up this chain.
 */
static mrb_value
chain_segments(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto chain = Chain::unwrap(mrb, self);
	const auto segments = chain->segments();
	const mrb_value out = mrb_ary_new_capa(mrb, segments.size());
	for (const auto &segment : segments) {
		const mrb_value segment_value
		    = euler::physics::segment_wrap(mrb, segment);
		mrb_ary_push(mrb, out, segment_value);
	}
	return out;
}

/**
 * @overload Euler::Physics::Chain#valid?
 *   Check if the chain is valid.
 *   @return [Boolean] true if the chain is valid, false otherwise.
 */
static mrb_value
chain_is_valid(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto chain = Chain::unwrap(mrb, self);
	const bool valid = chain->is_valid();
	return mrb_bool_value(valid);
}

RClass *
box2d_chain_init(mrb_state *mrb, struct RClass *mod)
{
	const auto state = euler::util::State::get(mrb);
	struct RClass *chain
	    = mrb_define_class_under(mrb, mod, "World", mrb->object_class);
	MRB_SET_INSTANCE_TT(chain, MRB_TT_DATA);
	mrb_define_method(mrb, chain, "surface_materials",
	    chain_surface_materials, MRB_ARGS_NONE());
	mrb_define_method(mrb, chain,
	    "surface_materials=", chain_set_surface_materials, MRB_ARGS_REQ(1));
	mrb_define_method(mrb, chain, "set_surface_material",
	    chain_set_surface_material, MRB_ARGS_REQ(2));
	mrb_define_method(mrb, chain, "world", chain_world, MRB_ARGS_NONE());
	mrb_define_method(mrb, chain, "segments", chain_segments,
	    MRB_ARGS_NONE());
	mrb_define_method(mrb, chain, "valid?", chain_is_valid,
	    MRB_ARGS_NONE());
	return chain;
}
