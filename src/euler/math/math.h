/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_MATH_H
#define EULER_MATH_MATH_H

#include "euler/util/object.h"
#include "euler/util/math.h"
#include "euler/util/ext.h"

namespace euler::math {

template <typename T> using Vec2 = Eigen::Matrix<T, 1, 2>;

template <typename T>
static inline float angle(Vec2<T> v)
{
	return std::atan2f(v.y(), v.x());
}

template <typename T>
static inline float length(Vec2<T> v)
{
	return std::sqrtf(v.x() * v.x() + v.y() * v.y());
}

static inline float deg2rad(const float deg)
{
	static constexpr float factor = (M_PI / 180.f);
	return deg * factor;
}

static inline float rad2deg(const float rad)
{
	static constexpr float factor = (M_PI / 180.f);
	return rad * factor;
}

static inline Vec2<float> deg2vec(const float deg)
{
	const auto rad = deg2rad(deg);
	return Vec2<float>{std::cosf(rad), std::sinf(rad)};
}

RClass *init(const util::Reference<util::State> &state, RClass *mod,
	RClass *super = nullptr);

} /* namespace euler::math */


#endif /* EULER_MATH_MATH_H */

