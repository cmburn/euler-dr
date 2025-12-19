/* SPDX-License-Identifier: ISC */

#ifndef EULER_PHYSICS_BODY_H
#define EULER_PHYSICS_BODY_H

#include "euler/util/object.h"

namespace euler::physics {
class Body : public util::Object {
public:
	float angular_damping() const;
	void set_angular_damping(float value);
	float angular_velocity() const;
	void set_angular_velocity(float value);
	float angular_impulse() const;
	void set_angular_impulse(float value);
	void apply_angular_impulse(float impulse);
};
} /* namespace euler::physics */

#endif /* EULER_PHYSICS_BODY_H */

