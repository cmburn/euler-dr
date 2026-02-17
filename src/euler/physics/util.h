/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_UTIL_H
#define EULER_PHYSICS_UTIL_H

#include <box2d/box2d.h>

#include "euler/util/state.h"

namespace euler::physics {

mrb_value b2_vec_to_value(mrb_state *mrb, b2Vec2 vec);
b2Vec2 value_to_b2_vec(mrb_state *mrb, mrb_value val);
b2Rot value_to_b2_rot(mrb_state *, mrb_value);
mrb_value b2_rot_to_value(mrb_state *, b2Rot);
mrb_value b2_transform_to_value(mrb_state *, b2Transform);
b2Transform value_to_b2_transform(mrb_state *, mrb_value);
float hash_read_float(mrb_state *, mrb_value, mrb_value, float);
mrb_int hash_read_int(mrb_state *, mrb_value, mrb_value, mrb_int);
mrb_value contact_data_to_value(mrb_state *, const b2ContactData *);
mrb_value surface_material_to_value(mrb_state *, const b2SurfaceMaterial *);
b2SurfaceMaterial value_to_surface_material(mrb_state *, mrb_value);
float coerce_float(mrb_state *, mrb_value);

template <size_t N>
void init_kw_values(mrb_value (&kw_values)[N])
{
	for (size_t i = 0; i < N; ++i) kw_values[i] = mrb_undef_value();
}

} /* namespace euler::physics */

#endif /* EULER_PHYSICS_UTIL_H */
