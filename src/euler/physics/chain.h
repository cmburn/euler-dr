/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_CHAIN_H
#define EULER_PHYSICS_CHAIN_H

#include <vector>

#include <box2d/id.h>

#include "euler/physics/util.h"
#include "euler/util/object.h"

namespace euler::physics {
class World;

class Chain final : public util::Object {
public:
	static constexpr mrb_data_type TYPE = {
		.struct_name = "Euler::Physics::Chain",
		.dfree =
		    [](mrb_state *, void *ptr) {
			    const auto ref
				= util::Reference<Chain>::unwrap(ptr);
			    ref.decrement();
		    },
	};
	static RClass *init(const util::Reference<util::State> &state,
	    RClass *mod);

	Chain(b2ChainId id)
	    : _id(id)
	{
	}

	b2ChainId
	id() const
	{
		return _id;
	}

	static util::Reference<Chain> unwrap(mrb_state *mrb, mrb_value self);

	std::vector<b2SurfaceMaterial> surface_materials() const;
	size_t surface_material_count() const;
	void set_surface_materials(
	    const std::vector<b2SurfaceMaterial> &materials);
	void set_surface_material(const b2SurfaceMaterial &material, int index);
	util::Reference<World> world() const;
	std::vector<b2Segment> segments() const;
	bool is_valid() const;

private:
	b2ChainId _id;
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_CHAIN_H */
