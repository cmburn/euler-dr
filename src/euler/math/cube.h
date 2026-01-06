/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_CUBE_H
#define EULER_MATH_CUBE_H

#include "euler/math/nonscalar.h"
#include "euler/math/util.h"
#include "euler/util/object.h"

namespace euler::math {
class Cube : public Nonscalar {
public:
	Type
	type() const override
	{
		return Type::Cube;
	}
	virtual size_type n_slices() const = 0;
	virtual size_type n_rows() const = 0;
	virtual size_type n_cols() const = 0;
	virtual mrb_value columns(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value rows(const util::Reference<util::State> &state) = 0;
	virtual mrb_value slices(const util::Reference<util::State> &state) = 0;
	virtual mrb_value column(const util::Reference<util::State> &state) = 0;
	virtual mrb_value row(const util::Reference<util::State> &state) = 0;
	virtual mrb_value slice(const util::Reference<util::State> &state) = 0;
	virtual mrb_value column_to_matrix(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value each_slice(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value fill(const util::Reference<util::State> &state) = 0;
	virtual mrb_value imbue(const util::Reference<util::State> &state) = 0;
	virtual mrb_value insert_columns(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value insert_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value insert_slices(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value ones(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randn(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randu(const util::Reference<util::State> &state) = 0;
	virtual mrb_value reshape(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value row_to_matrix(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_columns(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_slices(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value swap(const util::Reference<util::State> &state) = 0;
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

template <typename T> class CubeImpl final : Cube {
public:
	typedef T elem_type;
	typedef arma::Cube<T> cube_type;
	typedef detail::pod_type_t<T> pod_type;
	typedef arma::Cube<pod_type> pod_cube_type;

	cube_type &value() { return _cube; }
	const cube_type &value() const { return _cube; }

	ValueType
	value_type() const override
	{
		return nonscalar_value_type_v<T>;
	}
	size_type
	n_slices() const override
	{
		return static_cast<size_type>(_cube.n_slices);
	}

	size_type
	n_rows() const override
	{
		return static_cast<size_type>(_cube.n_rows);
	}

	size_type
	n_cols() const override
	{
		return static_cast<size_type>(_cube.n_cols);
	}

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
	mrb_value columns(const util::Reference<util::State> &state) override;
	mrb_value rows(const util::Reference<util::State> &state) override;
	mrb_value slices(const util::Reference<util::State> &state) override;
	mrb_value column(const util::Reference<util::State> &state) override;
	mrb_value row(const util::Reference<util::State> &state) override;
	mrb_value slice(const util::Reference<util::State> &state) override;
	mrb_value column_to_matrix(
	    const util::Reference<util::State> &state) override;
	mrb_value each_slice(
	    const util::Reference<util::State> &state) override;
	mrb_value fill(const util::Reference<util::State> &state) override;
	mrb_value imbue(const util::Reference<util::State> &state) override;
	mrb_value insert_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value insert_rows(
	    const util::Reference<util::State> &state) override;
	mrb_value insert_slices(
	    const util::Reference<util::State> &state) override;
	mrb_value ones(const util::Reference<util::State> &state) override;
	mrb_value randn(const util::Reference<util::State> &state) override;
	mrb_value randu(const util::Reference<util::State> &state) override;
	mrb_value reshape(const util::Reference<util::State> &state) override;
	mrb_value row_to_matrix(
	    const util::Reference<util::State> &state) override;
	mrb_value shed_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value shed_rows(const util::Reference<util::State> &state) override;
	mrb_value shed_slices(
	    const util::Reference<util::State> &state) override;
	mrb_value swap(const util::Reference<util::State> &state) override;
	mrb_value zeros(const util::Reference<util::State> &state) override;
	mrb_value add(const util::Reference<util::State> &state) override;
	mrb_value sub(const util::Reference<util::State> &state) override;
	mrb_value matmul(const util::Reference<util::State> &state) override;
	mrb_value mul(const util::Reference<util::State> &state) override;
	mrb_value div(const util::Reference<util::State> &state) override;
	mrb_value eq(const util::Reference<util::State> &state) override;
	mrb_value ne(const util::Reference<util::State> &state) override;
	mrb_value lt(const util::Reference<util::State> &state) override;
	mrb_value le(const util::Reference<util::State> &state) override;
	mrb_value gt(const util::Reference<util::State> &state) override;
	mrb_value ge(const util::Reference<util::State> &state) override;

private:
	cube_type _cube;
};

} /* namespace euler::math */

#endif /* EULER_MATH_CUBE_H */
