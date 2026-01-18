/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_MATRIX_H
#define EULER_MATH_MATRIX_H

#include <armadillo>

#include "euler/math/nonscalar.h"
#include "euler/math/row_vector.h"
#include "euler/math/size.h"
#include "euler/math/util.h"
#include "euler/math/vector.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"
#include "euler/util/state.h"

namespace euler::math {

class Matrix : public Nonscalar {
	BIND_MRUBY("Euler::Math::Matrix", Matrix, math.matrix);

public:
	~Matrix() override = default;

	Type type() const override;

	virtual mrb_value columns(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value rows(const util::Reference<util::State> &state) = 0;
	virtual mrb_value column_count(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value row_count(const util::Reference<util::State> &state)
	    = 0;
	virtual size_type n_cols() const = 0;
	virtual size_type n_rows() const = 0;
	virtual mrb_value column(const util::Reference<util::State> &state) = 0;
	virtual mrb_value row(const util::Reference<util::State> &state) = 0;
	virtual mrb_value each_column(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value each_row(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value eye(const util::Reference<util::State> &state) = 0;
	virtual mrb_value fill(const util::Reference<util::State> &state) = 0;
	virtual mrb_value imbue(const util::Reference<util::State> &state) = 0;
	virtual mrb_value insert_columns(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value insert_rows(const util::Reference<util::State> &state)
	    = 0;
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
	virtual mrb_value is_row_vector(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_column_vector(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value ones(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randn(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randu(const util::Reference<util::State> &state) = 0;
	virtual mrb_value reshape(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_columns(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_column(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_row(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value simple_transpose(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value swap(const util::Reference<util::State> &state) = 0;
	virtual mrb_value swap_columns(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value swap_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value to_array(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value to_column(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value to_row(const util::Reference<util::State> &state) = 0;
	virtual mrb_value transpose(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value zeros(const util::Reference<util::State> &state) = 0;
	virtual mrb_value add(const util::Reference<util::State> &state) = 0;
	virtual mrb_value sub(const util::Reference<util::State> &state) = 0;
	virtual mrb_value matmul(const util::Reference<util::State> &state) = 0;
	virtual mrb_value mul(const util::Reference<util::State> &state) = 0;
	virtual mrb_value div(const util::Reference<util::State> &state) = 0;
	virtual mrb_value eq(const util::Reference<util::State> &state) = 0;
	virtual mrb_value ne(const util::Reference<util::State> &state) = 0;
	virtual mrb_value lt(const util::Reference<util::State> &state) = 0;
	virtual mrb_value le(const util::Reference<util::State> &state) = 0;
	virtual mrb_value gt(const util::Reference<util::State> &state) = 0;
	virtual mrb_value ge(const util::Reference<util::State> &state) = 0;
};

template <typename T> class MatrixImpl final : public Matrix {
public:
	typedef T elem_type;
	typedef arma::Mat<T> matrix_type;
	typedef detail::pod_type_t<T> pod_type;
	typedef arma::Mat<pod_type> pod_matrix_type;

private:
	static bool
	is_matrix(const util::Reference<util::State> &state,
	    const mrb_value arg)
	{
		return Nonscalar::type(state, arg) == Type::Matrix;
	}

	mrb_int coerce_row(mrb_int r) const;
	mrb_int coerce_col(mrb_int c) const;
	mrb_int coerce_row(const util::Reference<util::State> &state, mrb_value r) const;
	mrb_int coerce_col(const util::Reference<util::State> &state, mrb_value c) const;

	template <typename U> friend class MatrixImpl;

public:
	static mrb_value wrap_matrix(const util::Reference<util::State> &state,
	    const matrix_type &matrix);

	MatrixImpl();

	/* These are defined in ops.h */
	matrix_type &
	value()
	{
		return _matrix;
	}
	const matrix_type &
	value() const
	{
		return _matrix;
	}
	ValueType
	value_type() const override
	{
		return nonscalar_value_type_v<T>;
	}
	mrb_value initialize(const util::Reference<util::State> &state);
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
	mrb_value is_empty(const util::Reference<util::State> &) override;
	mrb_value is_finite(const util::Reference<util::State> &) override;
	mrb_value is_zero(const util::Reference<util::State> &) override;
	mrb_value max(const util::Reference<util::State> &state) override;
	mrb_value min(const util::Reference<util::State> &state) override;
	mrb_value real(const util::Reference<util::State> &state) override;
	mrb_value replace(const util::Reference<util::State> &state) override;
	mrb_value reset(const util::Reference<util::State> &) override;
	mrb_value resize(const util::Reference<util::State> &state) override;
	mrb_value set_imaginary(
	    const util::Reference<util::State> &state) override;
	mrb_value set_real(const util::Reference<util::State> &state) override;
	mrb_value set_size(const util::Reference<util::State> &state) override;
	mrb_value size(const util::Reference<util::State> &state) override;
	mrb_value to_string(const util::Reference<util::State> &state) override;
	mrb_value transform(const util::Reference<util::State> &state) override;
	mrb_value columns(const util::Reference<util::State> &state) override;
	mrb_value rows(const util::Reference<util::State> &state) override;
	mrb_value column_count(
	    const util::Reference<util::State> &state) override;
	mrb_value row_count(const util::Reference<util::State> &state) override;
	size_type n_rows() const override;
	size_type n_cols() const override;
	mrb_value column(const util::Reference<util::State> &state) override;
	mrb_value row(const util::Reference<util::State> &state) override;
	mrb_value each_column(
	    const util::Reference<util::State> &state) override;
	mrb_value each_row(const util::Reference<util::State> &state) override;
	mrb_value eye(const util::Reference<util::State> &state) override;
	mrb_value fill(const util::Reference<util::State> &state) override;
	mrb_value imbue(const util::Reference<util::State> &state) override;
	mrb_value insert_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value insert_rows(
	    const util::Reference<util::State> &state) override;
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
	mrb_value is_row_vector(
	    const util::Reference<util::State> &state) override;
	mrb_value is_column_vector(
	    const util::Reference<util::State> &state) override;
	mrb_value ones(const util::Reference<util::State> &state) override;
	mrb_value randn(const util::Reference<util::State> &state) override;
	mrb_value randu(const util::Reference<util::State> &state) override;
	mrb_value reshape(const util::Reference<util::State> &state) override;
	mrb_value shed_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value shed_column(
	    const util::Reference<util::State> &state) override;
	mrb_value shed_rows(const util::Reference<util::State> &state) override;
	mrb_value shed_row(const util::Reference<util::State> &state) override;
	mrb_value simple_transpose(
	    const util::Reference<util::State> &state) override;
	mrb_value swap(const util::Reference<util::State> &state) override;
	mrb_value swap_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value swap_rows(const util::Reference<util::State> &state) override;
	mrb_value to_array(const util::Reference<util::State> &state) override;
	mrb_value to_column(const util::Reference<util::State> &state) override;
	mrb_value to_row(const util::Reference<util::State> &state) override;
	mrb_value transpose(const util::Reference<util::State> &state) override;
	mrb_value zeros(const util::Reference<util::State> &state) override;

private:
	matrix_type _matrix;
};
} /* namespace euler::math */

#endif /* EULER_MATH_MATRIX_H */
