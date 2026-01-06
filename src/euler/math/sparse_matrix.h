/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_SPARSE_MATRIX_H
#define EULER_MATH_SPARSE_MATRIX_H

#include "euler/math/nonscalar.h"
#include "euler/util/object.h"

namespace euler::math {
class SparseMatrix : public Nonscalar {
public:
	Type type() const override { return Type::SparseMatrix; }
	virtual mrb_value eye(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_diagonal(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_hermitian(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_square(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_symmetric(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_symmetric_positive_definite(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_triangular_lower(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_triangular_upper(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_vector(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_columns(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value swap_columns(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value swap_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value to_dense(const util::Reference<util::State> &state)
	    = 0;
};

template <typename T> class SparseMatrixImpl final : public SparseMatrix {
public:
	ValueType value_type() const override;
	mrb_value at(const util::Reference<util::State> &state) override;
	mrb_value set_at(const util::Reference<util::State> &state) override;
	mrb_value clamp(const util::Reference<util::State> &state) override;
	mrb_value clean(const util::Reference<util::State> &state) override;
	mrb_value copy_size(const util::Reference<util::State> &state) override;
	mrb_value map(const util::Reference<util::State> &state) override;
	mrb_value has_infinity(
	    const util::Reference<util::State> &state) override;
	mrb_value has_nan(const util::Reference<util::State> &state) override;
	mrb_value imaginary(const util::Reference<util::State> &state) override;
	mrb_value in_range(const util::Reference<util::State> &state) override;
	mrb_value index_max(const util::Reference<util::State> &state) override;
	mrb_value index_min(const util::Reference<util::State> &state) override;
	mrb_value is_complex(
	    const util::Reference<util::State> &state) override;
	mrb_value is_empty(const util::Reference<util::State> &state) override;
	mrb_value is_finite(const util::Reference<util::State> &state) override;
	mrb_value is_zero(const util::Reference<util::State> &state) override;
	mrb_value max(const util::Reference<util::State> &state) override;
	mrb_value min(const util::Reference<util::State> &state) override;
	mrb_value real(const util::Reference<util::State> &state) override;
	mrb_value replace(const util::Reference<util::State> &state) override;
	mrb_value reset(const util::Reference<util::State> &state) override;
	mrb_value resize(const util::Reference<util::State> &state) override;
	mrb_value set_imaginary(
	    const util::Reference<util::State> &state) override;
	mrb_value set_real(const util::Reference<util::State> &state) override;
	mrb_value set_size(const util::Reference<util::State> &state) override;
	mrb_value size(const util::Reference<util::State> &state) override;
	mrb_value to_string(const util::Reference<util::State> &state) override;
	mrb_value transform(const util::Reference<util::State> &state) override;
	mrb_value eye(const util::Reference<util::State> &state) override;
	mrb_value is_diagonal(
	    const util::Reference<util::State> &state) override;
	mrb_value is_hermitian(
	    const util::Reference<util::State> &state) override;
	mrb_value is_square(const util::Reference<util::State> &state) override;
	mrb_value is_symmetric(
	    const util::Reference<util::State> &state) override;
	mrb_value is_symmetric_positive_definite(
	    const util::Reference<util::State> &state) override;
	mrb_value is_triangular_lower(
	    const util::Reference<util::State> &state) override;
	mrb_value is_triangular_upper(
	    const util::Reference<util::State> &state) override;
	mrb_value is_vector(const util::Reference<util::State> &state) override;
	mrb_value shed_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value shed_rows(const util::Reference<util::State> &state) override;
	mrb_value swap_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value swap_rows(const util::Reference<util::State> &state) override;
	mrb_value to_dense(const util::Reference<util::State> &state) override;
};

} /* namespace euler::math */

#endif /* EULER_MATH_SPARSE_MATRIX_H */
