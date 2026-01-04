/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_CONTACT_H
#define EULER_PHYSICS_CONTACT_H

#include <box2d/id.h>
#include <box2d/types.h>

#include "euler/util/object.h"

namespace euler::physics {
class Contact final : public util::Object {
public:
	bool is_valid() const;
	b2ContactData data() const;
	static util::Reference<Contact> unwrap(mrb_state *mrb, mrb_value self);

private:
	b2ContactId _id;
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_CONTACT_H */
