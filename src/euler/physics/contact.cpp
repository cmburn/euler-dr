/* SPDX-License-Identifier: ISC */

#include "euler/physics/contact.h"

#include <mruby/class.h>

#include "euler/physics/util.h"

using euler::physics::Contact;

/**
 * @overload Euler::Physics::Contact#valid?
 *   @return [Boolean] whether the contact is valid
 */
static mrb_value
contact_is_valid(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto contact = Contact::unwrap(mrb, self);
	const bool valid = contact->is_valid();
	return mrb_bool_value(valid);
}

/**
 * @overload Euler::Physics::Contact#data
 *   @return [Array<Hash>] An array of contact data hashes. Each hash has the
 *           following keys:
 *           - :contact [Euler::Physics::Contact] The contact.
 *           - :shapes [Array<Euler::Physics::Shape>] The two shapes involved in
 * the contact.
 *           - :manifold [Hash] The contact manifold data. This contains the
 *             following keys:
 *             - :normal [Array<Numeric>] The unit normal vector in world
 *               space, points from shape A to body B.
 *             - :rolling_impulse [Numeric] Angular impulse applied for
 *               rolling resistance. N*m*s = kg*m^2/s
 *             - :points [Array<Hash>] An array of 0-2 contact points. Each
 *               point hash has the following keys:
 *               - :point [Array<Numeric>] Location of the contact point in
 *                 world space. Subject to precision loss at large
 *                 coordinates.
 *               - :anchors [Array<Array<Numeric>>] An array of two anchor
 *                 points, containing the contact point relative to each
 *                 shape's origin in world space.
 *               - :separation [Numeric] The separation of the contact point,
 *                 negative if penetrating.
 *               - :normal_impulse [Numeric] The impulse along the manifold
 *                 normal vector.
 *               - :tangent_impulse [Numeric] The friction impulse.
 *               - :total_normal_impulse [Numeric] The total normal impulse
 *                  applied across sub-stepping and restitution. This is
 *                  important to identify speculative contact points that had
 *                  an interaction in the time step.
 *               - :normal_velocity [Numeric] Relative normal velocity
 *                 pre-solve. Used for hit events. If the normal impulse is
 *                 zero then there was no hit. Negative means shapes are
 *                 approaching.
 *               - :id [Integer] Uniquely identifies a contact point between
 *                 two shapes.
 *               - :persisted [Boolean] Did this contact point exist the
 *                 previous step?
 */
static mrb_value
contact_data(mrb_state *mrb, mrb_value self)
{
	const auto state = euler::util::State::get(mrb);
	const auto contact = Contact::unwrap(mrb, self);
	const auto data = contact->data();
	return euler::physics::contact_data_to_value(mrb, &data);
}
