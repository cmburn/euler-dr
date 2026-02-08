/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_CHAIN_H
#define EULER_PHYSICS_CHAIN_H

#include <vector>

#include <box2d/id.h>

#include "euler/physics/util.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"

namespace euler::physics {
class World;
class Shape;

class Chain final : public util::Object {
	BIND_MRUBY("Euler::Physics::Chain", Chain, physics.chain);
	friend class World;
	Chain(b2ChainId id)
	    : _id(id)
	{
	}

public:
	~Chain();

	b2ChainId
	id() const
	{
		return _id;
	}

	static util::Reference<Chain> unwrap(mrb_state *mrb, mrb_value self);
	static util::Reference<Chain> wrap(b2ChainId id);

	std::vector<b2SurfaceMaterial> surface_materials() const;
	size_t surface_material_count() const;
	void set_surface_materials(
	    const std::vector<b2SurfaceMaterial> &materials);
	void set_surface_material(const b2SurfaceMaterial &material, int index);
	util::Reference<World> world() const;
	std::vector<util::Reference<Shape>> segments() const;
	bool is_valid() const;

	struct Key {
		b2ChainId id;
		uint64_t
		to_uint64() const
		{
			return b2StoreChainId(id);
		}
		bool
		operator==(const Key &other) const
		{
			return to_uint64() == other.to_uint64();
		}
	};

	Key
	key() const
	{
		return Key { _id };
	}

private:
	b2ChainId _id;
};

} /* namespace euler::physics */

template <> struct std::hash<euler::physics::Chain::Key> {
	size_t
	operator()(const euler::physics::Chain::Key &key) const noexcept
	{
		return std::hash<uint64_t>()(key.to_uint64());
	}
};

#endif /* EULER_PHYSICS_CHAIN_H */
