/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_WORLD_H
#define EULER_PHYSICS_WORLD_H

#include <box2d/id.h>

#include "euler/util/object.h"

namespace euler::physics {
class World final : public util::Object {
public:
	static constexpr mrb_data_type TYPE = {
		.struct_name = "Euler::Physics::World",
		.dfree =
		    [](mrb_state *, void *ptr) {
			    const auto ref
				= util::Reference<World>::unwrap(ptr);
			    ref.decrement();
		    },
	};
	void init(const util::Reference<util::State> &state);

	mrb_value wrap(const util::Reference<util::State> &state);
	World();
	World(b2WorldId id)
	    : _id(id)
	{
	}

private:
	b2WorldId _id;
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_WORLD_H */
