/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_UTIL_H
#define EULER_PHYSICS_UTIL_H

#include <box2d/box2d.h>

#include "euler/util/state.h"

namespace euler::physics {

#define B2_SYM(SYM)                                                            \
	(mrb_symbol_value(                                                     \
	    (::euler::util::State::get(mrb)->mrb()->intern_static(#SYM,        \
		sizeof(#SYM) - 1))))

mrb_value b2_vec_to_value(mrb_state *mrb, b2Vec2 vec);
b2Vec2 value_to_b2_vec(mrb_state *mrb, mrb_value val);
void *tmpbuf(mrb_state *, size_t);

bool try_call_block(mrb_state *mrb, mrb_value block, mrb_value argv[],
    mrb_int argc, mrb_value *out);

b2Vec2 value_to_b2_vec(mrb_state *, mrb_value);
b2Rot value_to_b2_rot(mrb_state *, mrb_value);
mrb_value b2_vec_to_value(mrb_state *, b2Vec2);
mrb_value b2_rot_to_value(mrb_state *, b2Rot);
mrb_value b2_transform_to_value(mrb_state *, b2Transform);
mrb_value b2_manifold_to_value(mrb_state *, const b2Manifold *);
b2Transform value_to_b2_transform(mrb_state *, mrb_value);
float hash_read_float(mrb_state *, mrb_value, mrb_value, float);
mrb_int hash_read_int(mrb_state *, mrb_value, mrb_value, mrb_int);
mrb_value contact_data_to_value(mrb_state *, const b2ContactData *);
mrb_value surface_material_to_value(mrb_state *, const b2SurfaceMaterial *);
b2SurfaceMaterial value_to_surface_material(mrb_state *, mrb_value);
float coerce_float(mrb_state *, mrb_value);

} /* namespace euler::physics */

#endif /* EULER_PHYSICS_UTIL_H */
