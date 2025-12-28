/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_NONSCALAR_H
#define EULER_MATH_NONSCALAR_H

#include "euler/util/object.h"
#include "euler/util/state.h"
#include "euler/math/util.h"

namespace euler::math {
class Nonscalar : public util::Object {
public:
	enum class Type {
		Cube,
		Matrix,
		RowVector,
		SparseMatrix,
		Vector,
	};

	virtual Type type() const = 0;

	virtual mrb_value at(const util::Reference<util::State> &state) = 0;
	virtual mrb_value clamp(const util::Reference<util::State> &state) = 0;
	virtual mrb_value clean(const util::Reference<util::State> &state) = 0;
	virtual mrb_value copy_size(const util::Reference<util::State> &state) = 0;
	virtual mrb_value each(const util::Reference<util::State> &state) = 0;
	virtual mrb_value has_infinity(const util::Reference<util::State> &state) = 0;
	virtual mrb_value has_nan(const util::Reference<util::State> &state) = 0;
	virtual mrb_value imaginary(const util::Reference<util::State> &state) = 0;
	virtual mrb_value in_range(const util::Reference<util::State> &state) = 0;
	virtual mrb_value index_max(const util::Reference<util::State> &state) = 0;
	virtual mrb_value index_min(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_empty(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_finite(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_zero(const util::Reference<util::State> &state) = 0;
	virtual mrb_value max(const util::Reference<util::State> &state) = 0;
	virtual mrb_value min(const util::Reference<util::State> &state) = 0;
	virtual mrb_value real(const util::Reference<util::State> &state) = 0;
	virtual mrb_value replace(const util::Reference<util::State> &state) = 0;
	virtual mrb_value reset(const util::Reference<util::State> &state) = 0;
	virtual mrb_value resize(const util::Reference<util::State> &state) = 0;
	virtual mrb_value set_imaginary(const util::Reference<util::State> &state) = 0;
	virtual mrb_value set_real(const util::Reference<util::State> &state) = 0;
	virtual mrb_value set_size(const util::Reference<util::State> &state) = 0;
	virtual mrb_value size(const util::Reference<util::State> &state) = 0;
	virtual mrb_value to_string(const util::Reference<util::State> &state) = 0;
	virtual mrb_value transform(const util::Reference<util::State> &state) = 0;


	/* clang-format off */
	// m, v, r, c
	virtual mrb_value columns(const util::Reference<util::State> &state) = 0;
	// m, v, r, c
	virtual mrb_value rows(const util::Reference<util::State> &state) = 0;
	// c
	virtual mrb_value slices(const util::Reference<util::State> &state) = 0;
	// m, v, r, c
	virtual mrb_value column(const util::Reference<util::State> &state) = 0;
	// m, v, r, c
	virtual mrb_value row(const util::Reference<util::State> &state) = 0;
	// c
	virtual mrb_value slice(const util::Reference<util::State> &state) = 0;
	// c
	virtual mrb_value column_to_matrix(const util::Reference<util::State> &state) = 0;
	// m
	virtual mrb_value each_column(const util::Reference<util::State> &state) = 0;
	// m
	virtual mrb_value each_row(const util::Reference<util::State> &state) = 0;
	// c
	virtual mrb_value each_slice(const util::Reference<util::State> &state) = 0;
	// m, sm
	virtual mrb_value eye(const util::Reference<util::State> &state) = 0;
	// m, v, r, c
	virtual mrb_value fill(const util::Reference<util::State> &state) = 0;
	// m, v, r, c
	virtual mrb_value imbue(const util::Reference<util::State> &state) = 0;
	// m, v, c
	virtual mrb_value insert_columns(const util::Reference<util::State> &state) = 0;
	// m, r, c
	virtual mrb_value insert_rows(const util::Reference<util::State> &state) = 0;
	// c
	virtual mrb_value insert_slices(const util::Reference<util::State> &state) = 0;
	// m
	virtual mrb_value inverse(const util::Reference<util::State> &state) = 0;
	// m, sm
	virtual mrb_value is_diagonal(const util::Reference<util::State> &state) = 0;
	// m, sm
	virtual mrb_value is_hermitian(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_sorted(const util::Reference<util::State> &state) = 0;
	// m, sm
	virtual mrb_value is_square(const util::Reference<util::State> &state) = 0;
	// m, sm
	virtual mrb_value is_symmetric(const util::Reference<util::State> &state) = 0;
	// m, sm
	virtual mrb_value is_symmetric_positive_definite(const util::Reference<util::State> &state) = 0;
	// m, sm
	virtual mrb_value is_triangular_lower(const util::Reference<util::State> &state) = 0;
	// m, sm
	virtual mrb_value is_triangular_upper(const util::Reference<util::State> &state) = 0;
	// m, sm
	virtual mrb_value is_vector(const util::Reference<util::State> &state) = 0;
	// m, v, r, c
	virtual mrb_value ones(const util::Reference<util::State> &state) = 0;
	// m, v, r, c
	virtual mrb_value randn(const util::Reference<util::State> &state) = 0;
	// m, v, r, c
	virtual mrb_value randu(const util::Reference<util::State> &state) = 0;
	// m, c, sm
	virtual mrb_value reshape(const util::Reference<util::State> &state) = 0;
	// c
	virtual mrb_value row_to_matrix(const util::Reference<util::State> &state) = 0;
	// m, r, c, sm
	virtual mrb_value shed_columns(const util::Reference<util::State> &state) = 0;
	// m, v, c, sm
	virtual mrb_value shed_rows(const util::Reference<util::State> &state) = 0;
	// c
	virtual mrb_value shed_slices(const util::Reference<util::State> &state) = 0;
	// m, v, c
	virtual mrb_value simple_transpose(const util::Reference<util::State> &state) = 0;
	// m, v, r, c
	virtual mrb_value swap(const util::Reference<util::State> &state) = 0;
	// m, v, r, sm
	virtual mrb_value swap_columns(const util::Reference<util::State> &state) = 0;
	// m, v, c, sm
	virtual mrb_value swap_rows(const util::Reference<util::State> &state) = 0;
	// m
	virtual mrb_value to_column(const util::Reference<util::State> &state) = 0;
	// sm
	virtual mrb_value to_dense(const util::Reference<util::State> &state) = 0;
	// m
	virtual mrb_value to_row(const util::Reference<util::State> &state) = 0;
	// m, v, r
	virtual mrb_value transpose(const util::Reference<util::State> &state) = 0;
	virtual mrb_value zeros(const util::Reference<util::State> &state) = 0;
	/* clang-format on */
};
} /* namespace euler::math */

#endif /* EULER_MATH_NONSCALAR_H */
