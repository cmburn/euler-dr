/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_SIZE_H
#define EULER_MATH_SIZE_H

#include "euler/math/nonscalar.h"
#include "euler/math/util.h"
#include "euler/math/vector.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"

namespace euler::math {
/*
 * There's a few functions that don't make sense for Size, but are here for
 * completeness.
 */
class Size final : public Vector {
	BIND_MRUBY("Euler::Math::Size", Size, math.size);

	static bool is_size(const util::Reference<util::State> &state,
	    mrb_value arg);

public:
	typedef size_type elem_type;
	typedef arma::Col<size_type>::fixed<3> vector_type;

	Size(size_type elems);
	Size(size_type rows, size_type cols);
	Size(size_type slices, size_type rows, size_type cols);
	Size(const vector_type &vec);

	template <typename... Args>
	static mrb_value
	wrap_size(const util::Reference<util::State> &state, Args &&...args)
	{
		auto self
		    = util::make_reference<Size>(std::forward<Args>(args)...);
		return state->wrap(self);
	}

	template <size_type N = 3>
	std::array<size_type, N>
	dims()
	{
		static_assert(N > 0 && N <= 3);
		std::array<size_type, N> arr {};
		for (size_type i = 0; i < N; ++i) {
			const auto idx = 3 - N + i;
			auto dim = _vector(idx);
			if (dim == 0) dim = 1;
			arr[i] = dim;
		}
		return arr;
	}

	template <typename T>
	mrb_value
	set_real_from(const util::Reference<Vector> &vec_obj)
	{
		assert(!vec_obj->is_size());
		auto vec = vec_obj.cast_to<VectorImpl<T>>();
		_vector(0) = static_cast<size_type>(vec->_vector(0));
		_vector(1) = static_cast<size_type>(vec->_vector(1));
		_vector(2) = static_cast<size_type>(vec->_vector(2));
		return mrb_nil_value();
	}

	Type type() const override;

	ValueType value_type() const override;

	mrb_value at(const util::Reference<util::State> &state) override;
	mrb_value set_at(const util::Reference<util::State> &state) override;
	mrb_value clamp(const util::Reference<util::State> &state) override;
	mrb_value clean(const util::Reference<util::State> &state) override;
	/* this should be a no-op, but it's here for completeness' sake. */
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
	mrb_value reset(const util::Reference<util::State> &) override;
	mrb_value resize(const util::Reference<util::State> &state) override;
	mrb_value set_imaginary(
	    const util::Reference<util::State> &state) override;

	mrb_value set_real(const util::Reference<util::State> &state) override;
	mrb_value set_size(const util::Reference<util::State> &state) override;
	mrb_value size(const util::Reference<util::State> &state) override;
	mrb_value to_string(const util::Reference<util::State> &state) override;
	mrb_value transform(const util::Reference<util::State> &state) override;
	mrb_value fill(const util::Reference<util::State> &state);
	mrb_value imbue(const util::Reference<util::State> &state) override;
	mrb_value insert_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value is_sorted(const util::Reference<util::State> &state) override;
	mrb_value ones(const util::Reference<util::State> &state) override;
	mrb_value randn(const util::Reference<util::State> &state) override;
	mrb_value randu(const util::Reference<util::State> &state) override;
	mrb_value shed_rows(const util::Reference<util::State> &state) override;
	mrb_value simple_transpose(
	    const util::Reference<util::State> &state) override;
	mrb_value swap(const util::Reference<util::State> &state) override;
	mrb_value swap_cols(const util::Reference<util::State> &state) override;
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

	bool is_cube() const;

	bool is_matrix() const;

	bool is_vector() const;

	bool is_size() const override;
	mrb_value is_size(
	    const util::Reference<util::State> &state) const override;
	size_type n_cols() const override;
	size_type n_rows() const override;
	mrb_value rows(const util::Reference<util::State> &state) override;
	mrb_value row(const util::Reference<util::State> &state) override;

	size_type row_count();

	mrb_value insert_rows(
	    const util::Reference<util::State> &state) override;

protected:
	util::Reference<Vector> as_vector_impl() override;

private:
	/* note that if an element == 0, this means it was not supplied but is
	 * treated as a dimension of 1.  */
	/* [ #rows, #cols, #slices ]*/
	vector_type _vector;
};

template <size_type N>
std::array<size_type, N>
unwrap_size(const util::Reference<util::State> &state)
{
	mrb_value arg;
	const auto mrb = state->mrb();
	mrb->get_args("o", &arg);
	const auto size_obj = state->unwrap<Size>(arg);
	return size_obj->dims<N>();
}

} /* namespace euler::math */

#endif /* EULER_MATH_SIZE_H */
