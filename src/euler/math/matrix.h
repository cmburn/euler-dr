/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_MATRIX_H
#define EULER_MATH_MATRIX_H

#include <armadillo>

#include "euler/math/nonscalar.h"
#include "euler/math/util.h"
#include "euler/math/vector.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"
#include "euler/util/state.h"

namespace euler::math {

class Matrix : public Nonscalar {
public:
	~Matrix() override = default;
	static constexpr mrb_data_type TYPE = {
		.struct_name = "Matrix",
		.dfree =
		    [](mrb_state *, void *ptr) {
			    const auto ref
				= util::Reference<Matrix>::unwrap(ptr);
			    ref.decrement();
		    },
	};
	static void init(const util::Reference<util::State> &state);

	Type
	type() const override
	{ return Type::Matrix; }

	virtual mrb_value columns(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value rows(const util::Reference<util::State> &state) = 0;
	virtual mrb_value column_count(
	    const util::Reference<util::State> &state) = 0;
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
	    const util::Reference<util::State> &state) = 0;
	virtual mrb_value insert_rows(const util::Reference<util::State> &state)
	    = 0;

	virtual mrb_value is_diagonal(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_hermitian(
	    const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_square(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_symmetric(
	    const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_symmetric_positive_definite(
	    const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_triangular_lower(
	    const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_triangular_upper(
	    const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_vector(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value ones(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randn(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randu(const util::Reference<util::State> &state) = 0;
	virtual mrb_value reshape(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_columns(
	    const util::Reference<util::State> &state) = 0;
	virtual mrb_value shed_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value simple_transpose(
	    const util::Reference<util::State> &state) = 0;
	virtual mrb_value swap(const util::Reference<util::State> &state) = 0;
	virtual mrb_value swap_columns(
	    const util::Reference<util::State> &state) = 0;
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

private:
	static bool
	is_matrix(const util::Reference<util::State> &state,
	    const mrb_value arg)
	{ return Nonscalar::type(state, arg) == Type::Matrix; }

	template <typename U> friend class MatrixImpl;

	static mrb_value
	wrap_matrix(const util::Reference<util::State> &state,
	    const matrix_type &matrix)
	{
		auto mrb = state->mrb();
		// TODO
		throw std::runtime_error("Not implemented");
	}

public:
	MatrixImpl() = default;

	matrix_type &
	matrix()
	{ return _matrix; }

	ValueType
	value_type() const override
	{ return nonscalar_value_type_v<T>; }

	mrb_value
	initialize(const util::Reference<util::State> &state)
	{
		auto mrb = state->mrb();
		return mrb_nil_value();
	}

	mrb_int
	coerce_row(mrb_int r) const
	{
		if (r < 0) r += static_cast<mrb_int>(_matrix.n_rows);
		return r;
	}

	mrb_int
	coerce_col(mrb_int c) const
	{
		if (c < 0) c += static_cast<mrb_int>(_matrix.n_cols);
		return c;
	}

	mrb_value
	at(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		const auto argc = mrb->get_argc();
		switch (argc) {
		case 2: {
			mrb_int row = 0, col = 0;
			mrb->get_args("ii", &row, &col);
			const auto value = _matrix(static_cast<size_type>(row),
			    static_cast<size_type>(col));
			return wrap_num(mrb, value);
		}
		case 1: {
			mrb_int elem = 0;
			mrb->get_args("i", &elem);
			const auto value
			    = _matrix(static_cast<size_type>(elem));
			return wrap_num(mrb, value);
		}
		default:
			throw std::invalid_argument(
			    "wrong number of arguments");
		}
	}

	mrb_value
	set_at(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		const auto argc = mrb->get_argc();
		switch (argc) {
		case 3: {
			mrb_value value;
			mrb_int row = 0, col = 0;
			mrb->get_args("iio", &row, &col, &value);
			const auto v = unwrap_num<T>(state, value);
			_matrix(static_cast<size_type>(row),
			    static_cast<size_type>(col)) = v;
			return mrb_nil_value();
		}
		case 2: {
			mrb_value value;
			mrb_int elem = 0;
			mrb->get_args("io", &elem, &value);
			const auto v = unwrap_num<T>(state, value);
			_matrix(static_cast<size_type>(elem)) = v;
			return mrb_nil_value();
		}
		default:
			throw std::invalid_argument(
			    "wrong number of arguments");
		}
	}

	mrb_value
	clamp(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		mrb_value min_value, max_value;
		mrb->get_args("oo", &min_value, &max_value);
		const auto min = unwrap_num<T>(state, min_value);
		const auto max = unwrap_num<T>(state, max_value);
		_matrix.clamp(min, max);
		return mrb_nil_value();
	}

	mrb_value
	clean(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		mrb_value value;
		mrb->get_args("o", &value);
		const auto tol = unwrap_num<double>(state, value);
		_matrix.clean(tol);
		return mrb_nil_value();
	}

	mrb_value
	copy_size(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		mrb_value arg;
		mrb->get_args("o", &arg);
		if (!is_matrix(state, arg)) {
			throw std::invalid_argument(
			    "argument must be a Matrix");
		}
		const auto other = util::Reference<Matrix>::unwrap(mrb->mrb(), arg);
		// there's a few scenarios where we mimic Armadillo's behavior
		// here rather than call the appropriate Armadillo function
		// directly to avoid having to cast to concrete types
		_matrix.set_size(other->n_rows(), other->n_cols());
		return mrb_nil_value();
	}

	mrb_value
	map(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		mrb_value blk;
		mrb->get_args("&!", &blk);
		mrb_value arr = mrb->ary_new_capa(_matrix.n_elem);
		_matrix.for_each([&](T value) -> void {
			auto val = wrap_num(mrb, value);
			auto result = mrb->yield(blk, val);
			mrb->ary_push(arr, result);
		});
		return arr;
	}

	mrb_value
	has_infinity(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		const bool result = _matrix.has_inf();
		return mrb_bool_value(result);
	}

	mrb_value
	has_nan(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		const bool result = _matrix.has_nan();
		return mrb_bool_value(result);
	}

	/*
	 * Not an actual method in armadillo, but is wrapped as one for API
	 * consistency with `imaginary=`
	 */
	mrb_value
	imaginary(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		auto imag = arma::imag(_matrix);
		return wrap_matrix(state, imag);
	}

	mrb_value
	in_range(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		mrb_value value;
		mrb->get_args("o", &value);
		const auto v = unwrap_num<T>(state, value);
		const bool result = _matrix.in_range(v);
		return mrb_bool_value(result);
	}

	mrb_value
	index_max(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		const auto index = _matrix.index_max();
		return mrb->int_value(wrap_num(mrb, index));
	}

	mrb_value
	index_min(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		const auto index = _matrix.index_min();
		return mrb->int_value(wrap_num(mrb, index));
	}

	mrb_value
	is_complex(const util::Reference<util::State> &state) override
	{
		static constexpr bool result = math::is_complex<T>();
		return mrb_bool_value(result);
	}

	mrb_value
	is_empty(const util::Reference<util::State> &) override
	{
		const bool result = _matrix.is_empty();
		return mrb_bool_value(result);
	}

	mrb_value
	is_finite(const util::Reference<util::State> &) override
	{
		const bool result = _matrix.is_finite();
		return mrb_bool_value(result);
	}

	mrb_value
	is_zero(const util::Reference<util::State> &) override
	{
		const bool result = _matrix.is_zero();
		return mrb_bool_value(result);
	}

	mrb_value
	max(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		const auto value = _matrix.max();
		return wrap_num(mrb, value);
	}

	mrb_value
	min(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		const auto value = _matrix.min();
		return wrap_num(mrb, value);
	}

	mrb_value
	real(const util::Reference<util::State> &state) override
	{
		auto real = arma::real(_matrix);
		return wrap_matrix(state, real);
	}

	mrb_value
	replace(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		mrb_value old_value, new_value;
		mrb->get_args("oo", &old_value, &new_value);
		const auto old_v = unwrap_num<T>(state, old_value);
		const auto new_v = unwrap_num<T>(state, new_value);
		_matrix.replace(old_v, new_v);
		return mrb_nil_value();
	}

	mrb_value
	reset(const util::Reference<util::State> &) override
	{
		_matrix.reset();
		return mrb_nil_value();
	}

	mrb_value
	resize(const util::Reference<util::State> &state) override
	{
		auto [r, c] = unwrap_size<2>(state);
		_matrix.resize(r, c);
		return mrb_nil_value();
	}

	mrb_value
	set_imaginary(const util::Reference<util::State> &state) override
	{
		if (!is_complex<T>()) {
			throw std::runtime_error(
			    "cannot set imaginary part of real matrix");
		}
		auto mrb = state->mrb();
		mrb_value arg;
		mrb->get_args("o", &arg);
		if (!is_matrix(state, arg)) {
			throw std::invalid_argument(
			    "argument must be a Matrix");
		}
		auto other = state->unwrap<Matrix>(arg);
		switch (other->value_type()) {
		case ValueType::Int16: {
			auto mat = other.cast_to<MatrixImpl<int16_t>>();
			_matrix.set_imag(mat->_matrix);
			break;
		}
		case ValueType::Int32: {
			auto mat = other.cast_to<MatrixImpl<int32_t>>();
			_matrix.set_imag(mat->_matrix);
			break;
		}
		case ValueType::Int64: {
			auto mat = other.cast_to<MatrixImpl<int64_t>>();
			_matrix.set_imag(mat->_matrix);
			break;
		}
		case ValueType::UInt16: {
			auto mat = other.cast_to<MatrixImpl<uint16_t>>();
			_matrix.set_imag(mat->_matrix);
			break;
		}
		case ValueType::UInt32: {
			auto mat = other.cast_to<MatrixImpl<uint32_t>>();
			_matrix.set_imag(mat->_matrix);
			break;
		}
		case ValueType::UInt64: {
			auto mat = other.cast_to<MatrixImpl<uint64_t>>();
			_matrix.set_imag(mat->_matrix);
			break;
		}
		case ValueType::Float32: {
			auto mat = other.cast_to<MatrixImpl<float>>();
			_matrix.set_imag(mat->_matrix);
			break;
		}
		case ValueType::Float64: {
			auto mat = other.cast_to<MatrixImpl<double>>();
			_matrix.set_imag(mat->_matrix);
			break;
		}
		case ValueType::Complex32: [[fallthrough]];
		case ValueType::Complex64:
			throw std::invalid_argument(
			    "argument must be a real Matrix");
		}
		return mrb_nil_value();
	}

	mrb_value
	set_real(const util::Reference<util::State> &state) override
	{
		auto mrb = state->mrb();
		mrb_value arg;
		mrb->get_args("o", &arg);
		if (!is_matrix(state, arg)) {
			throw std::invalid_argument(
			    "argument must be a Matrix");
		}
		// auto other = util::Reference<Matrix>::unwrap(arg);
		auto other = state->unwrap<Matrix>(arg);
		switch (other->value_type()) {
		case ValueType::Int16: {
			auto mat = other.cast_to<MatrixImpl<int16_t>>();
			_matrix.set_real(mat->_matrix);
			break;
		}
		case ValueType::Int32: {
			auto mat = other.cast_to<MatrixImpl<int32_t>>();
			_matrix.set_real(mat->_matrix);
			break;
		}
		case ValueType::Int64: {
			auto mat = other.cast_to<MatrixImpl<int64_t>>();
			_matrix.set_real(mat->_matrix);
			break;
		}
		case ValueType::UInt16: {
			auto mat = other.cast_to<MatrixImpl<uint16_t>>();
			_matrix.set_real(mat->_matrix);
			break;
		}
		case ValueType::UInt32: {
			auto mat = other.cast_to<MatrixImpl<uint32_t>>();
			_matrix.set_real(mat->_matrix);
			break;
		}
		case ValueType::UInt64: {
			auto mat = other.cast_to<MatrixImpl<uint64_t>>();
			_matrix.set_real(mat->_matrix);
			break;
		}
		case ValueType::Float32: {
			auto mat = other.cast_to<MatrixImpl<float>>();
			_matrix.set_real(mat->_matrix);
			break;
		}
		case ValueType::Float64: {
			auto mat = other.cast_to<MatrixImpl<double>>();
			_matrix.set_real(mat->_matrix);
			break;
		}
		case ValueType::Complex32: [[fallthrough]];
		case ValueType::Complex64:
			throw std::invalid_argument(
			    "argument must be a real Matrix");
		}
		return mrb_nil_value();
	}

	mrb_value
	set_size(const util::Reference<util::State> &state) override
	{
		auto [r, c] = unwrap_size<2>(state);
		_matrix.set_size(r, c);
		return mrb_nil_value();
	}

	mrb_value
	size(const util::Reference<util::State> &state) override
	{
	}

	mrb_value to_string(const util::Reference<util::State> &state) override;
	mrb_value transform(const util::Reference<util::State> &state) override;
	mrb_value columns(const util::Reference<util::State> &state) override;
	mrb_value rows(const util::Reference<util::State> &state) override;
	mrb_value
	column_count(const util::Reference<util::State> &state) override
	{ return state->mrb()->int_value(static_cast<mrb_int>(n_cols())); }
	mrb_value
	row_count(const util::Reference<util::State> &state) override
	{ return state->mrb()->int_value(static_cast<mrb_int>(n_rows())); }
	size_type
	n_rows() const override
	{ return _matrix.n_rows; }
	size_type
	n_cols() const override
	{ return _matrix.n_cols; }
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
	mrb_value ones(const util::Reference<util::State> &state) override;
	mrb_value randn(const util::Reference<util::State> &state) override;
	mrb_value randu(const util::Reference<util::State> &state) override;
	mrb_value reshape(const util::Reference<util::State> &state) override;
	mrb_value shed_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value shed_rows(const util::Reference<util::State> &state) override;
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
	matrix_type _matrix;
};
} /* namespace euler::math */

#endif /* EULER_MATH_MATRIX_H */
