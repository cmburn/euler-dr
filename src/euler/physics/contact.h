/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_CONTACT_H
#define EULER_PHYSICS_CONTACT_H

#include <vector>
#include <box2d/id.h>
#include <box2d/types.h>

#include "euler/util/ext.h"
#include "euler/util/object.h"

namespace euler::physics {
class Shape;
class World;

class Contact final : public util::Object {
	BIND_MRUBY("Euler::Physics::Contact", Contact, physics.contact);
	friend class World;
	Contact(b2ContactId id)
	    : _id(id)
	{
	}
public:
	struct Data {
		~Data();
		util::Reference<Contact> contact;
		util::Reference<Shape> shape_a;
		util::Reference<Shape> shape_b;
		b2Manifold manifold;
		mrb_value wrap(mrb_state *mrb);
		static Data from_b2(const b2ContactData &data);
	};


	struct Event {
		~Event();
		util::Reference<Shape> shape_a;
		util::Reference<Shape> shape_b;
		util::Reference<Contact> contact;
		mrb_value wrap(mrb_state *mrb);
	};

	struct HitEvent {
		~HitEvent();
		util::Reference<Shape> shape_a;
		util::Reference<Shape> shape_b;
		util::Reference<Contact> contact;
		b2Vec2 point;
		b2Vec2 normal;
		float approach_speed;
		mrb_value wrap(mrb_state *mrb);
	};

	struct Events {
		~Events();
		std::vector<Event> start_events;
		std::vector<Event> end_events;
		std::vector<HitEvent> hit_events;
		static Events from_b2(const b2ContactEvents &events);
		mrb_value wrap(mrb_state *mrb);
	};

	struct Key {
		b2ContactId id;
		std::array<uint32_t, 3> to_ints() const
		{
			std::array<uint32_t, 3> triplet;
			b2StoreContactId(id, triplet.data());
			return triplet;
		}
		bool operator==(const Key &other) const
		{
			return to_ints() == other.to_ints();
		}
	};

	~Contact();

	static util::Reference<Contact> wrap(b2ContactId id);
	bool is_valid() const;
	Data data() const;

private:
	util::Reference<World> world();
	static util::Reference<World> world(b2ContactId id);
	b2ContactId _id;
};
} /* namespace euler::physics */

template <> struct std::hash<euler::physics::Contact::Key> {
	size_t
	operator()(const euler::physics::Contact::Key &key) const noexcept
	{
		const auto ints = key.to_ints();
		// TODO: better hash algorithm
		size_t h1 = std::hash<uint32_t>()(ints[0]);
		size_t h2 = std::hash<uint32_t>()(ints[1]);
		size_t h3 = std::hash<uint32_t>()(ints[2]);
		// Combine the hashes
		return h1 ^ (h2 << 1) ^ (h3 << 2);
	}
};

#endif /* EULER_PHYSICS_CONTACT_H */
