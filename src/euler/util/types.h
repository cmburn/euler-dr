/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_TYPES_H
#define EULER_UTIL_TYPES_H

#include <Eigen/Eigen>

namespace euler::util {

typedef Eigen::Index size_type;
static constexpr size_type Dynamic = Eigen::Dynamic;

typedef Eigen::Matrix<float, 2, 1> Vec2;
typedef Eigen::Matrix<float, 3, 1> Vec3;
typedef Eigen::Matrix<float, 4, 1> Vec4;
typedef Eigen::Matrix<uint32_t, 2, 1> UVec2;

} /* namespace euler::util */

#endif /* EULER_UTIL_TYPES_H */
