/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_VECTOR_H
#define EULER_MATH_VECTOR_H

#include <cassert>
#include <format>

#include <armadillo>
#include <mruby.h>

#include "euler/math/nonscalar.h"
#include "euler/math/util.h"
#include "euler/util/state.h"

namespace euler::math {

template <typename T, size_t N> class Vector final : public Nonscalar {
public:
	static constexpr size_type column_count = N;
	static constexpr bool is_dynamic = column_count == dynamic_size;
	typedef T value_type;

	[[nodiscard]] Kind
	kind() const override
	{
		return nonscalar_kind_v<value_type>;
	}

	[[nodiscard]] Type
	type() const override
	{
		return Type::Vector;
	}

	[[nodiscard]] size_type
	n_cols() const override
	{
		if constexpr (is_dynamic) return _data.n_cols;
		else return column_count;
	}

	[[nodiscard]] size_type
	n_rows() const override
	{
		assert(_data.n_rows == 1);
		return 1;
	}

	[[nodiscard]] size_type
	n_elem() const override
	{
		if constexpr (is_dynamic) return _data.n_elem;
		else return column_count;
	}

	[[nodiscard]] bool
	is_fixed_size() const override
	{
		return !is_dynamic;
	}
	[[nodiscard]] mrb_value to_mrb(
	    const util::Reference<util::MRubyState> &mrb) const override;
	[[nodiscard]] mrb_value to_array(
	    const util::Reference<util::MRubyState> &mrb) const override;

	static constexpr size_t size = N;
	typedef std::conditional_t<size == dynamic_size, arma::Col<value_type>,
	    typename arma::Col<value_type>::template fixed<size>>
	    vector_type;

	template <typename U>
	static util::Reference<Vector<size_type, N>>
	to_size()
	{
		if constexpr (is_dynamic) { }
	}

private:
	vector_type _data;
};

template <typename T, size_t N> using ColumnVector = Vector<T, N>;

} /* namespace euler::math */

#endif /* EULER_MATH_VECTOR_H */
