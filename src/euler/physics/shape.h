/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_SHAPE_H
#define EULER_PHYSICS_SHAPE_H

#include <box2d/id.h>

#include "euler/util/object.h"

namespace euler::physics {
class Shape final : public util::Object {
public:
	static constexpr mrb_data_type TYPE = {
		.struct_name = "Euler::Physics::Shape",
		.dfree =
		    [](mrb_state *, void *ptr) {
			    const auto ref
				= util::Reference<Shape>::unwrap(ptr);
			    ref.decrement();
		    },
	};

	mrb_value wrap(const util::Reference<util::State> &state);
	Shape(b2ShapeId id)
	    : _id(id)
	{
	}
private:
	b2ShapeId _id;
};
} /* namespace euler::physics */


#endif /* EULER_PHYSICS_SHAPE_H */

