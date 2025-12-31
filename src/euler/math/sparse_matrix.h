/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_SPARSE_MATRIX_H
#define EULER_MATH_SPARSE_MATRIX_H

#include "euler/math/nonscalar.h"
#include "euler/util/object.h"

namespace euler::math {
class SparseMatrix : public Nonscalar {
	virtual mrb_value eye(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_diagonal(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_hermitian(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_square(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_symmetric(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_symmetric_positive_definite(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_triangular_lower(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_triangular_upper(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_vector(const util::Reference<util::State> &state) = 0;
	virtual mrb_value shed_columns(const util::Reference<util::State> &state) = 0;
	virtual mrb_value shed_rows(const util::Reference<util::State> &state) = 0;
	virtual mrb_value swap_columns(const util::Reference<util::State> &state) = 0;
	virtual mrb_value swap_rows(const util::Reference<util::State> &state) = 0;
	virtual mrb_value to_dense(const util::Reference<util::State> &state) = 0;
};
} /* namespace euler::math */

#endif /* EULER_MATH_SPARSE_MATRIX_H */

