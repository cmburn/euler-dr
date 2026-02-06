/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_ROW_VECTOR_H
#define EULER_MATH_ROW_VECTOR_H

#include "euler/math/nonscalar.h"
#include "euler/util/object.h"

namespace euler::math {

class RowVector : public Nonscalar {
	BIND_MRUBY("Euler::Math::RowVector", RowVector, math.row_vector);
public:
	[[nodiscard]] Type type() const override { return Type::RowVector; }
	virtual mrb_value columns(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value rows(const util::Reference<util::State> &state) = 0;
	virtual mrb_value column(const util::Reference<util::State> &state) = 0;
	virtual mrb_value row(const util::Reference<util::State> &state) = 0;
	virtual mrb_value fill(const util::Reference<util::State> &state) = 0;
	virtual mrb_value imbue(const util::Reference<util::State> &state) = 0;
	virtual mrb_value insert_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_sorted(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value ones(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randn(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randu(const util::Reference<util::State> &state) = 0;
	virtual mrb_value shed_columns(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_rows(const util::Reference<util::State> &state)
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

template <typename T> class RowVectorImpl final : public RowVector {
public:
	typedef T elem_type;
	typedef arma::Row<T> row_vector_type;
	typedef detail::pod_type_t<T> pod_type;
	typedef arma::Row<pod_type> pod_row_vector_type;

	template <typename U = T>
	static mrb_value wrap_row_vector(
	    const util::Reference<util::State> &state,
	    const arma::Row<U> &row_vector)
	{
		auto obj = util::make_reference<RowVectorImpl<U>>(state);
		// auto rv_impl = static_cast<RowVectorImpl<U> *>(obj.get());
		obj->_row_vector = row_vector;
		return state->wrap(obj);
	}

	row_vector_type &value() { return _row_vector; }
	const row_vector_type &value() const { return _row_vector; }
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
	mrb_value columns(const util::Reference<util::State> &state) override;
	mrb_value rows(const util::Reference<util::State> &state) override;
	mrb_value column(const util::Reference<util::State> &state) override;
	mrb_value row(const util::Reference<util::State> &state) override;
	mrb_value fill(const util::Reference<util::State> &state) override;
	mrb_value imbue(const util::Reference<util::State> &state) override;
	mrb_value insert_rows(
	    const util::Reference<util::State> &state) override;
	mrb_value is_sorted(const util::Reference<util::State> &state) override;
	mrb_value ones(const util::Reference<util::State> &state) override;
	mrb_value randn(const util::Reference<util::State> &state) override;
	mrb_value randu(const util::Reference<util::State> &state) override;
	mrb_value shed_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value shed_rows(const util::Reference<util::State> &state) override;
	mrb_value simple_transpose(
	    const util::Reference<util::State> &state) override;
	mrb_value swap(const util::Reference<util::State> &state) override;
	mrb_value swap_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value swap_rows(const util::Reference<util::State> &state) override;
	mrb_value transpose(const util::Reference<util::State> &state) override;
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
	row_vector_type _row_vector;
};

} /* namespace euler::math */

#endif /* EULER_MATH_ROW_VECTOR_H */
