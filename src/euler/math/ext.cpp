/* SPDX-License-Identifier: ISC */

#include "euler/math/ext.h"

#include "euler/math/cube.h"
#include "euler/math/matrix.h"
#include "euler/math/row_vector.h"
#include "euler/math/util.h"
#include "euler/math/vector.h"

namespace euler::math {

#define DECLARE_COLUMN_VECTOR(TYPE, SUFFIX)                                    \
	typedef Vector<TYPE, 2> ColumnVector2##SUFFIX;                         \
	typedef Vector<TYPE, 2> Vector2##SUFFIX;                               \
	typedef Vector<TYPE, 3> ColumnVector3##SUFFIX;                         \
	typedef Vector<TYPE, 3> Vector3##SUFFIX;                               \
	typedef Vector<TYPE, 4> ColumnVector4##SUFFIX;                         \
	typedef Vector<TYPE, 4> Vector4##SUFFIX;                               \
	typedef Vector<TYPE, 5> ColumnVector5##SUFFIX;                         \
	typedef Vector<TYPE, 5> Vector5##SUFFIX;                               \
	typedef Vector<TYPE, 6> ColumnVector6##SUFFIX;                         \
	typedef Vector<TYPE, 6> Vector6##SUFFIX;                               \
	typedef Vector<TYPE, 7> ColumnVector7##SUFFIX;                         \
	typedef Vector<TYPE, 7> Vector7##SUFFIX;                               \
	typedef Vector<TYPE, 8> ColumnVector8##SUFFIX;                         \
	typedef Vector<TYPE, 8> Vector8##SUFFIX;                               \
	typedef Vector<TYPE, 9> ColumnVector9##SUFFIX;                         \
	typedef Vector<TYPE, 9> Vector9##SUFFIX;                               \
	typedef Vector<TYPE, dynamic_size> ColumnVector##SUFFIX;               \
	typedef Vector<TYPE, dynamic_size> Vector##SUFFIX

#define DECLARE_ROW_VECTOR(TYPE, SUFFIX)                                       \
	typedef RowVector<TYPE, 2> RowVector2##SUFFIX;                         \
	typedef RowVector<TYPE, 3> RowVector3##SUFFIX;                         \
	typedef RowVector<TYPE, 4> RowVector4##SUFFIX;                         \
	typedef RowVector<TYPE, 5> RowVector5##SUFFIX;                         \
	typedef RowVector<TYPE, 6> RowVector6##SUFFIX;                         \
	typedef RowVector<TYPE, 7> RowVector7##SUFFIX;                         \
	typedef RowVector<TYPE, 8> RowVector8##SUFFIX;                         \
	typedef RowVector<TYPE, 9> RowVector9##SUFFIX;                         \
	typedef RowVector<TYPE, dynamic_size> RowVector##SUFFIX

#define DECLARE_MATRIX(TYPE, SUFFIX)                                           \
	typedef Matrix<TYPE, 2, 2> Matrix2x2##SUFFIX;                          \
	typedef Matrix<TYPE, 3, 3> Matrix3x3##SUFFIX;                          \
	typedef Matrix<TYPE, 4, 4> Matrix4x4##SUFFIX;                          \
	typedef Matrix<TYPE, 5, 5> Matrix5x5##SUFFIX;                          \
	typedef Matrix<TYPE, 6, 6> Matrix6x6##SUFFIX;                          \
	typedef Matrix<TYPE, 7, 7> Matrix7x7##SUFFIX;                          \
	typedef Matrix<TYPE, 8, 8> Matrix8x8##SUFFIX;                          \
	typedef Matrix<TYPE, 9, 9> Matrix9x9##SUFFIX;                          \
	typedef Matrix<TYPE, dynamic_size, dynamic_size> Matrix##SUFFIX

#define DECLARE_CUBE(TYPE, SUFFIX)                                             \
	typedef Cube<TYPE, 2, 2, 2> Cube2x2x2##SUFFIX;                         \
	typedef Cube<TYPE, 3, 3, 3> Cube3x3x3##SUFFIX;                         \
	typedef Cube<TYPE, 4, 4, 4> Cube4x4x4##SUFFIX;                         \
	typedef Cube<TYPE, 5, 5, 5> Cube5x5x5##SUFFIX;                         \
	typedef Cube<TYPE, 6, 6, 6> Cube6x6x6##SUFFIX;                         \
	typedef Cube<TYPE, 7, 7, 7> Cube7x7x7##SUFFIX;                         \
	typedef Cube<TYPE, 8, 8, 8> Cube8x8x8##SUFFIX;                         \
	typedef Cube<TYPE, 9, 9, 9> Cube9x9x9##SUFFIX;                         \
	typedef Cube<TYPE, dynamic_size, dynamic_size, dynamic_size>           \
	    Cube##SUFFIX

#define DECLARE_NONSCALAR_TYPES(TYPE, SUFFIX)                                  \
	DECLARE_COLUMN_VECTOR(TYPE, SUFFIX);                                   \
	DECLARE_ROW_VECTOR(TYPE, SUFFIX);                                      \
	DECLARE_MATRIX(TYPE, SUFFIX);                                          \
	DECLARE_CUBE(TYPE, SUFFIX)

// DECLARE_NONSCALAR_TYPES(float, f32);
// DECLARE_NONSCALAR_TYPES(double, f64);
// DECLARE_NONSCALAR_TYPES(std::complex<float>, c32);
// DECLARE_NONSCALAR_TYPES(std::complex<double>, c64);
// DECLARE_NONSCALAR_TYPES(int16_t, i16);
// DECLARE_NONSCALAR_TYPES(int32_t, i32);
// DECLARE_NONSCALAR_TYPES(int64_t, i64);
// DECLARE_NONSCALAR_TYPES(uint16_t, u16);
// DECLARE_NONSCALAR_TYPES(uint32_t, u32);
// DECLARE_NONSCALAR_TYPES(uint64_t, u64);

void
init(const util::Reference<util::State> &state, RClass *euler_mod)
{
	auto mod = state->mrb()->define_module_under(euler_mod, "Math");
	auto &mods = state->modules();
	mods.math.mod = mod;
	// mods.math.matrix_f32 = MatrixImpl<float>::init(state, mod);
}

} /* namespace euler::math */
