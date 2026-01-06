/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_IMPL_H
#define EULER_MATH_IMPL_H

#include <functional>

#include "euler/math/cube.h"
#include "euler/math/matrix.h"
#include "euler/math/row_vector.h"
#include "euler/math/size.h"
#include "euler/math/sparse_matrix.h"
#include "euler/math/vector.h"

namespace euler::math {

static Nonscalar::Type
read_nonscalar_type(const util::Reference<util::State> &state, mrb_value value)
{
	if (is_nonscalar(state, value))
		return state->unwrap<Nonscalar>(value)->type();
	const auto cls = state->mrb()->obj_class(value);
	const char *name = state->mrb()->class_name(cls);
	throw util::make_error<util::ArgumentError>(state,
	    "Value is not a nonscalar value, has type {}", name);
}

static Nonscalar::ValueType
read_nonscalar_value_type(const util::Reference<util::State> &state,
    mrb_value value)
{
	if (is_nonscalar(state, value))
		return state->unwrap<Nonscalar>(value)->value_type();
	const auto cls = state->mrb()->obj_class(value);
	const char *name = state->mrb()->class_name(cls);
	throw util::make_error<util::ArgumentError>(state,
	    "Value is not a nonscalar value, has type {}", name);
}

template <typename T> struct is_cube : std::false_type { };

template <typename U> struct is_cube<arma::Cube<U>> : std::true_type { };

template <typename T> static constexpr bool is_cube_v = is_cube<T>::value;

template <typename T> struct is_matrix : std::false_type { };

template <typename U> struct is_matrix<arma::Mat<U>> : std::true_type { };

template <typename T> static constexpr bool is_matrix_v = is_matrix<T>::value;

template <typename T> struct is_row_vector : std::false_type { };

template <typename U> struct is_row_vector<arma::Row<U>> : std::true_type { };

template <typename T>
static constexpr bool is_row_vector_v = is_row_vector<T>::value;

template <typename T> struct is_sparse_matrix : std::false_type { };

template <typename U>
struct is_sparse_matrix<arma::SpMat<U>> : std::true_type { };

template <typename T>
static constexpr bool is_sparse_matrix_v = is_sparse_matrix<T>::value;

template <typename T> struct is_vector : std::false_type { };

template <typename U> struct is_vector<arma::Col<U>> : std::true_type { };

template <typename T> static constexpr bool is_vector_v = is_vector<T>::value;

template <typename T>
mrb_value
wrap_math_value(const util::Reference<util::State> &state, const T &obj)
{
	static_assert(is_supported_numeric<T>()
		|| std::is_base_of_v<Nonscalar, T>,
	    "T must be a supported numeric type or a Nonscalar type");
	if (is_supported_numeric<T>()) return wrap_num(state, obj);
	if constexpr (is_cube_v<T>) {
		using elem_type = typename T::elem_type;
		auto cube_impl
		    = util::make_reference<CubeImpl<elem_type>>(state);
		cube_impl->value() = obj;
		return state->wrap<Nonscalar>(cube_impl);
	} else if constexpr (is_matrix_v<T>) {
		using elem_type = typename T::elem_type;
		auto matrix_impl
		    = util::make_reference<MatrixImpl<elem_type>>(state);
		matrix_impl->value() = obj;
		return state->wrap<Nonscalar>(matrix_impl);
	} else if constexpr (is_row_vector_v<T>) {
		using elem_type = typename T::elem_type;
		auto row_vector_impl
		    = util::make_reference<RowVectorImpl<elem_type>>(state);
		row_vector_impl->value() = obj;
		return state->wrap<Nonscalar>(row_vector_impl);
	} else if constexpr (is_sparse_matrix_v<T>) {
		using elem_type = typename T::elem_type;
		auto sparse_matrix_impl
		    = util::make_reference<SparseMatrixImpl<elem_type>>(state);
		sparse_matrix_impl->value() = obj;
		return state->wrap<Nonscalar>(sparse_matrix_impl);
	} else if constexpr (is_vector_v<T>) {
		using elem_type = typename T::elem_type;
		auto vector_impl
		    = util::make_reference<VectorImpl<elem_type>>(state);
		vector_impl->value() = obj;
		return state->wrap<Nonscalar>(vector_impl);
	} else {
		return mrb_nil_value();
	}
}

template <typename F>
void
apply_concrete(const util::Reference<util::State> &state, mrb_value arg, F fn)
{
	using Type = Nonscalar::Type;
	using ValueType = Nonscalar::ValueType;
	switch (read_nonscalar_type(state, arg)) {
	case Type::Cube:
		switch (read_nonscalar_value_type(state, arg)) {
		case ValueType::Int16: {
			const auto base = state->unwrap<Cube>(arg);
			const auto other = base.cast_to<CubeImpl<int16_t>>();
			fn(other);
		} break;
		case ValueType::Int32: {
			const auto base = state->unwrap<Cube>(arg);
			const auto other = base.cast_to<CubeImpl<int32_t>>();
			fn(other);
		} break;
		case ValueType::Int64: {
			const auto base = state->unwrap<Cube>(arg);
			const auto other = base.cast_to<CubeImpl<int64_t>>();
			fn(other);
		} break;
		case ValueType::UInt16: {
			const auto base = state->unwrap<Cube>(arg);
			const auto other = base.cast_to<CubeImpl<uint16_t>>();
			fn(other);
		} break;
		case ValueType::UInt32: {
			const auto base = state->unwrap<Cube>(arg);
			const auto other = base.cast_to<CubeImpl<uint32_t>>();
			fn(other);
		} break;
		case ValueType::UInt64: {
			const auto base = state->unwrap<Cube>(arg);
			const auto other = base.cast_to<CubeImpl<uint64_t>>();
			fn(other);
		} break;
		case ValueType::Float32: {
			const auto base = state->unwrap<Cube>(arg);
			const auto other = base.cast_to<CubeImpl<float>>();
			fn(other);
		} break;
		case ValueType::Float64: {
			const auto base = state->unwrap<Cube>(arg);
			const auto other = base.cast_to<CubeImpl<double>>();
			fn(other);
		} break;
		case ValueType::Complex32: {
			const auto base  = state->unwrap<Cube>(arg);
			const auto other
			    = base.cast_to<CubeImpl<std::complex<float>>>();
			fn(other);
		}
		case ValueType::Complex64: {
			const auto base  = state->unwrap<Cube>(arg);
			const auto other
			    = base.cast_to<CubeImpl<std::complex<double>>>();
			fn(other);
		}
		default: std::unreachable();
		}
		break;
	case Type::Matrix:
		switch (read_nonscalar_value_type(state, arg)) {
		case ValueType::Int16: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto other = base.cast_to<MatrixImpl<int16_t>>();
			fn(other);
		} break;
		case ValueType::Int32: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto other = base.cast_to<MatrixImpl<int32_t>>();
			fn(other);
		} break;
		case ValueType::Int64: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto other = base.cast_to<MatrixImpl<int64_t>>();
			fn(other);
		} break;
		case ValueType::UInt16: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto other = base.cast_to<MatrixImpl<uint16_t>>();
			fn(other);
		} break;
		case ValueType::UInt32: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto other = base.cast_to<MatrixImpl<uint32_t>>();
			fn(other);
		} break;
		case ValueType::UInt64: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto other = base.cast_to<MatrixImpl<uint64_t>>();
			fn(other);
		} break;
		case ValueType::Float32: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto other = base.cast_to<MatrixImpl<float>>();
			fn(other);
		} break;
		case ValueType::Float64: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto other = base.cast_to<MatrixImpl<double>>();
			fn(other);
		} break;
		case ValueType::Complex32: {
			const auto base  = state->unwrap<Matrix>(arg);
			const auto other
			    = base.cast_to<MatrixImpl<std::complex<float>>>();
			fn(other);
		} break;
		case ValueType::Complex64: {
			const auto base  = state->unwrap<Matrix>(arg);
			const auto other
			    = base.cast_to<MatrixImpl<std::complex<double>>>();
			fn(other);
		}
		default: std::unreachable();
		}
		break;
	case Type::RowVector: {
		switch (read_nonscalar_value_type(state, arg)) {
		case ValueType::Int16: {
			const auto base  = state->unwrap<RowVector>(arg);
			const auto other
			    = base.cast_to<RowVectorImpl<int16_t>>();
			fn(other);
		} break;
		case ValueType::Int32: {
			const auto base  = state->unwrap<RowVector>(arg);
			const auto other
			    = base.cast_to<RowVectorImpl<int32_t>>();
			fn(other);
		} break;
		case ValueType::Int64: {
			const auto base  = state->unwrap<RowVector>(arg);
			const auto other
			    = base.cast_to<RowVectorImpl<int64_t>>();
			fn(other);
		} break;
		case ValueType::UInt16: {
			const auto base  = state->unwrap<RowVector>(arg);
			const auto other
			    = base.cast_to<RowVectorImpl<uint16_t>>();
			fn(other);
		} break;
		case ValueType::UInt32: {
			const auto base  = state->unwrap<RowVector>(arg);
			const auto other
			    = base.cast_to<RowVectorImpl<uint32_t>>();
			fn(other);
		} break;
		case ValueType::UInt64: {
			const auto base  = state->unwrap<RowVector>(arg);
			const auto other
			    = base.cast_to<RowVectorImpl<uint64_t>>();
			fn(other);
		} break;
		case ValueType::Float32: {
			const auto base = state->unwrap<RowVector>(arg);
			const auto other = base.cast_to<RowVectorImpl<float>>();
			fn(other);
		} break;
		case ValueType::Float64: {
			const auto base  = state->unwrap<RowVector>(arg);
			const auto other
			    = base.cast_to<RowVectorImpl<double>>();
			fn(other);
		} break;
		case ValueType::Complex32: {
			const auto base  = state->unwrap<RowVector>(arg);
			const auto other = base.cast_to<
			    RowVectorImpl<std::complex<float>>>();
			fn(other);
		} break;
		case ValueType::Complex64: {
			const auto base  = state->unwrap<RowVector>(arg);
			const auto other = base.cast_to<
			    RowVectorImpl<std::complex<double>>>();
			fn(other);
		} break;
		default: std::unreachable();
		}
	} break;
	case Type::SparseMatrix:
		switch (read_nonscalar_value_type(state, arg)) {
		case ValueType::Int16: {
			const auto base  = state->unwrap<SparseMatrix>(arg);
			const auto other
			    = base.cast_to<SparseMatrixImpl<int16_t>>();
			fn(other);
		} break;
		case ValueType::Int32: {
			const auto base  = state->unwrap<SparseMatrix>(arg);
			const auto other
			    = base.cast_to<SparseMatrixImpl<int32_t>>();
			fn(other);
		} break;
		case ValueType::Int64: {
			const auto base  = state->unwrap<SparseMatrix>(arg);
			const auto other
			    = base.cast_to<SparseMatrixImpl<int64_t>>();
			fn(other);
		} break;
		case ValueType::UInt16: {
			const auto base  = state->unwrap<SparseMatrix>(arg);
			const auto other
			    = base.cast_to<SparseMatrixImpl<uint16_t>>();
			fn(other);
		} break;
		case ValueType::UInt32: {
			const auto base  = state->unwrap<SparseMatrix>(arg);
			const auto other
			    = base.cast_to<SparseMatrixImpl<uint32_t>>();
			fn(other);
		} break;
		case ValueType::UInt64: {
			const auto base  = state->unwrap<SparseMatrix>(arg);
			const auto other
			    = base.cast_to<SparseMatrixImpl<uint64_t>>();
			fn(other);
		} break;
		case ValueType::Float32: {
			const auto base  = state->unwrap<SparseMatrix>(arg);
			const auto other
			    = base.cast_to<SparseMatrixImpl<float>>();
			fn(other);
		} break;
		case ValueType::Float64: {
			const auto base  = state->unwrap<SparseMatrix>(arg);
			const auto other
			    = base.cast_to<SparseMatrixImpl<double>>();
			fn(other);
		} break;
		case ValueType::Complex32: {
			const auto base  = state->unwrap<SparseMatrix>(arg);
			const auto other = base.cast_to<
			    SparseMatrixImpl<std::complex<float>>>();
			fn(other);
		} break;
		case ValueType::Complex64: {
			const auto base  = state->unwrap<SparseMatrix>(arg);
			const auto other = base.cast_to<
			    SparseMatrixImpl<std::complex<double>>>();
			fn(other);
		} break;
		}
		break;
	case Type::Vector:
		switch (read_nonscalar_value_type(state, arg)) {
		case ValueType::Int16: {
			const auto base = state->unwrap<Vector>(arg);
			const auto other = base.cast_to<VectorImpl<int16_t>>();
			fn(other);
		} break;
		case ValueType::Int32: {
			const auto base = state->unwrap<Vector>(arg);
			const auto other = base.cast_to<VectorImpl<int32_t>>();
			fn(other);
		} break;
		case ValueType::Int64: {
			const auto base = state->unwrap<Vector>(arg);
			const auto other = base.cast_to<VectorImpl<int64_t>>();
			fn(other);
		} break;
		case ValueType::UInt16: {
			const auto base = state->unwrap<Vector>(arg);
			const auto other = base.cast_to<VectorImpl<uint16_t>>();
			fn(other);
		} break;
		case ValueType::UInt32: {
			const auto base = state->unwrap<Vector>(arg);
			const auto other = base.cast_to<VectorImpl<uint32_t>>();
			fn(other);
		} break;
		case ValueType::UInt64: {
			const auto base = state->unwrap<Vector>(arg);
			const auto other = base.cast_to<VectorImpl<uint64_t>>();
			fn(other);
		} break;
		case ValueType::Float32: {
			const auto base = state->unwrap<Vector>(arg);
			const auto other = base.cast_to<VectorImpl<float>>();
			fn(other);
		} break;
		case ValueType::Float64: {
			const auto base = state->unwrap<Vector>(arg);
			const auto other = base.cast_to<VectorImpl<double>>();
			fn(other);
		} break;
		case ValueType::Complex32: {
			const auto base  = state->unwrap<Vector>(arg);
			const auto other
			    = base.cast_to<VectorImpl<std::complex<float>>>();
			fn(other);
		} break;
		case ValueType::Complex64: {
			const auto base  = state->unwrap<Vector>(arg);
			const auto other
			    = base.cast_to<VectorImpl<std::complex<double>>>();
			fn(other);
		} break;
		}
		break;
	default: std::unreachable();
	}
}

// template <typename T, typename U>
// static U
// apply_to(const util::Reference<util::State> &state, mrb_value value,
// 	const std::function<U()> &fn
// 	)
// {
// 	auto mrb = state->mrb();
// 	auto ns_cls = state->modules().math.nonscalar;
// 	if (!mrb->obj_is_kind_of(value, ns_cls) && !is_numeric(value)) {
// 		throw util::make_error<util::ArgumentError>(state,
// 		    "Argument must be a nonscalar value or a numeric value");
// 	}
//
// }

/*
 * given an object can be one of 6 container types,
 * and each container can have one of 10 types:
 * need a function that will take two values, cast them to their concrete types,
 * and attempt to apply a function to the two.
 */

/**
 * @overload Euler::Math::Cube#[](i)
 *   Access the element stored at index `i`.n
 *   @param i [Integer] The index.
 *   @return [Numeric] The value stored at index `i`.
 */
/**
 * @overload Euler::Math::Cube#[](r, c, s)
 *   Access the element stored at row `r`, column `c`, slice `s`.
 *   @param r [Integer] The row index.
 *   @param c [Integer] The column index.
 *   @param s [Integer] The slice index.
 *   @return [Numeric] The value stored at index `i`.
 */
template <typename T>
mrb_value
CubeImpl<T>::at(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	auto argc = mrb->get_argc();
	switch (argc) {
	case 3: {
		mrb_value rval, cval, sval;
		mrb->get_args("ooo", &rval, &cval, &sval);
		auto r = unwrap_num<size_type>(state, rval);
		auto c = unwrap_num<size_type>(state, cval);
		auto s = unwrap_num<size_type>(state, sval);
		auto value = _cube(r, c, s);
	}
	case 1: {
		mrb_value index_val;
		mrb->get_args("o", &index_val);
		auto index = unwrap_num<size_type>(state, index_val);
		auto value = _cube(index);
		return wrap_num<T>(state, value);
	}
	default:
		throw util::make_error<util::ArgumentError>(state,
		    "wrong number of arguments (given {}, expected 1 or 3)",
		    argc);
	}
}

/**
 * @overload Euler::Math::Cube#[]=(i, value)
 *   Set the element stored at index `i`.
 *   @param i [Integer] The index.
 *   @param value [Numeric] The new value.
 *   @return [void]
 */
/**
 * @overload Euler::Math::Cube#[]=(r, c, s, value)
 *   Set the element stored at row `r`, column `c`, slice `s`.
 *   @param r [Integer] The row index.
 *   @param c [Integer] The column index.
 *   @param s [Integer] The slice index.
 *   @param value [Numeric] The new value.
 *   @return [void]
 */
template <typename T>
mrb_value
CubeImpl<T>::set_at(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	auto argc = mrb->get_argc();
	switch (argc) {
	case 4: {
		mrb_value rval, cval, sval, value_val;
		mrb->get_args("oooo", &rval, &cval, &sval, &value_val);
		auto r = unwrap_num<size_type>(state, rval);
		auto c = unwrap_num<size_type>(state, cval);
		auto s = unwrap_num<size_type>(state, sval);
		auto value = unwrap_num<T>(state, value_val);
		_cube(r, c, s) = value;
		return mrb_nil_value();
	}
	case 2: {
		mrb_value index_val, value_val;
		mrb->get_args("oo", &index_val, &value_val);
		auto index = unwrap_num<size_type>(state, index_val);
		auto value = unwrap_num<T>(state, value_val);
		_cube(index) = value;
		return mrb_nil_value();
	}
	default:
		throw util::make_error<util::ArgumentError>(state,
		    "wrong number of arguments (given {}, expected 2 or 4)",
		    argc);
	}
}

/**
 * @overload Euler::Math::Cube#clamp(min, max)
 *   Clamp the values in the cube to be within the given range.
 *   @param min [Numeric] The minimum value.
 *   @param max [Numeric] The maximum value.
 *   @return [void]
 */
template <typename T>
mrb_value
CubeImpl<T>::clamp(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value min_val, max_val;
	mrb->get_args("oo", &min_val, &max_val);
	auto min = unwrap_num<T>(state, min_val);
	auto max = unwrap_num<T>(state, max_val);
	_cube.clamp(min, max);
	return mrb_nil_value();
}

/**
 * @overload Euler::Math::Cube#clean(threshold)
 *   Clean the cube by setting elements with absolute value less than
 *   `threshold` to zero.
 *   @param threshold [Numeric] The threshold value.
 *   @return [void]
 */
template <typename T>
mrb_value
CubeImpl<T>::clean(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value threshold_val;
	mrb->get_args("o", &threshold_val);
	auto threshold = unwrap_num<pod_type>(state, threshold_val);
	_cube.clean(threshold);
	return mrb_nil_value();
}

/**
 * @overload Euler::Math::Cube#copy_size(other)
 *   Copy the size of another cube.
 *   @param other [Euler::Math::Cube] The other cube.
 *   @return [void]
 */
template <typename T>
mrb_value
CubeImpl<T>::copy_size(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value other_val;
	mrb->get_args("o", &other_val);
	auto other = state->unwrap<Cube>(other_val);
	return mrb_nil_value();
}

template <typename T>
mrb_value
CubeImpl<T>::map(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::has_infinity(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::has_nan(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::imaginary(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::in_range(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::index_max(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::index_min(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::is_complex(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::is_empty(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::is_finite(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::is_zero(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::max(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::min(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::real(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::replace(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::reset(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::resize(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::set_imaginary(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::set_real(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::set_size(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::size(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::to_string(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::transform(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::slices(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::column(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::row(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::slice(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::column_to_matrix(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::each_slice(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::fill(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::imbue(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::insert_columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::insert_rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::insert_slices(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::ones(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::randn(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::randu(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::reshape(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::row_to_matrix(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::shed_columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::shed_rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::shed_slices(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::swap(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::zeros(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::add(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	mrb_value out;
	apply_concrete(state, arg, [&]<typename U>(U &v) {
		if constexpr (std::is_base_of_v<Cube, std::decay_t<U>>) {
			auto result = this->_cube + v->value();
			out = wrap_math_value(state, result);
		}
	});
	return out;
}

template <typename T>
mrb_value
CubeImpl<T>::sub(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::matmul(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::mul(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::div(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::eq(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::ne(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::lt(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::le(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::gt(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
CubeImpl<T>::ge(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::wrap_matrix(const util::Reference<util::State> &state,
    const matrix_type &matrix)
{
	auto mrb = state->mrb();
	// TODO
	throw std::runtime_error("Not implemented");
}

template <typename T> MatrixImpl<T>::MatrixImpl() = default;

template <typename T>
mrb_value
MatrixImpl<T>::initialize(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	return mrb_nil_value();
}

template <typename T>
mrb_int
MatrixImpl<T>::coerce_row(mrb_int r) const
{
	if (r < 0) r += static_cast<mrb_int>(_matrix.n_rows);
	return r;
}

template <typename T>
mrb_int
MatrixImpl<T>::coerce_col(mrb_int c) const
{
	if (c < 0) c += static_cast<mrb_int>(_matrix.n_cols);
	return c;
}

template <typename T>
mrb_value
MatrixImpl<T>::at(const util::Reference<util::State> &state)
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
		const auto value = _matrix(static_cast<size_type>(elem));
		return wrap_num(mrb, value);
	}
	default: throw std::invalid_argument("wrong number of arguments");
	}
}

template <typename T>
mrb_value
MatrixImpl<T>::set_at(const util::Reference<util::State> &state)
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
		    static_cast<size_type>(col))
		    = v;
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
	default: throw std::invalid_argument("wrong number of arguments");
	}
}

template <typename T>
mrb_value
MatrixImpl<T>::clamp(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value min_value, max_value;
	mrb->get_args("oo", &min_value, &max_value);
	const auto min = unwrap_num<T>(state, min_value);
	const auto max = unwrap_num<T>(state, max_value);
	_matrix.clamp(min, max);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::clean(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value value;
	mrb->get_args("o", &value);
	const auto tol = unwrap_num<double>(state, value);
	_matrix.clean(tol);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::copy_size(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_matrix(state, arg)) {
		throw std::invalid_argument("argument must be a Matrix");
	}
	const auto other = util::Reference<Matrix>::unwrap(mrb->mrb(), arg);
	// there's a few scenarios where we mimic Armadillo's behavior
	// here rather than call the appropriate Armadillo function
	// directly to avoid having to cast to concrete types
	_matrix.set_size(other->n_rows(), other->n_cols());
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::map(const util::Reference<util::State> &state)
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

template <typename T>
mrb_value
MatrixImpl<T>::has_infinity(const util::Reference<util::State> &state)
{
	const bool result = _matrix.has_inf();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::has_nan(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const bool result = _matrix.has_nan();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::imaginary(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	pod_matrix_type imag = arma::imag(_matrix);
	return wrap_matrix(state, imag);
}

template <typename T>
mrb_value
MatrixImpl<T>::in_range(const util::Reference<util::State> &state)
{
	if constexpr (math::is_complex<T>()) {
		throw std::runtime_error(
		    "cannot check range of complex matrix");
	} else {
		auto mrb = state->mrb();
		mrb_value value;
		mrb->get_args("o", &value);
		const auto v = unwrap_num<T>(state, value);
		const bool result = _matrix.in_range(v);
		return mrb_bool_value(result);
	}
}

template <typename T>
mrb_value
MatrixImpl<T>::index_max(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto index = _matrix.index_max();
	return wrap_num(mrb, index);
}

template <typename T>
mrb_value
MatrixImpl<T>::index_min(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto index = _matrix.index_min();
	return wrap_num(mrb, index);
}

template <typename T>
mrb_value
MatrixImpl<T>::is_complex(const util::Reference<util::State> &state)
{
	static constexpr bool result = math::is_complex<T>();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::is_empty(const util::Reference<util::State> &)
{
	const bool result = _matrix.is_empty();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::is_finite(const util::Reference<util::State> &)
{
	const bool result = _matrix.is_finite();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::is_zero(const util::Reference<util::State> &)
{
	const bool result = _matrix.is_zero();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::max(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto value = _matrix.max();
	return wrap_num(mrb, value);
}

template <typename T>
mrb_value
MatrixImpl<T>::min(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto value = _matrix.min();
	return wrap_num(mrb, value);
}

template <typename T>
mrb_value
MatrixImpl<T>::real(const util::Reference<util::State> &state)
{
	pod_matrix_type real = arma::real(_matrix);
	return wrap_matrix(state, real);
}

template <typename T>
mrb_value
MatrixImpl<T>::replace(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value old_value, new_value;
	mrb->get_args("oo", &old_value, &new_value);
	const auto old_v = unwrap_num<T>(state, old_value);
	const auto new_v = unwrap_num<T>(state, new_value);
	_matrix.replace(old_v, new_v);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::reset(const util::Reference<util::State> &)
{
	_matrix.reset();
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::resize(const util::Reference<util::State> &state)
{
	auto [r, c] = unwrap_size<2>(state);
	_matrix.resize(r, c);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::set_imaginary(const util::Reference<util::State> &state)
{
	if (!math::is_complex<T>()) {
		throw std::runtime_error(
		    "cannot set imaginary part of real matrix");
	}
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_matrix(state, arg)) {
		throw std::invalid_argument("argument must be a Matrix");
	}
	auto other = state->unwrap<Matrix>(arg);
	if (other->value_type() != nonscalar_value_type_v<pod_type>) {
		throw std::invalid_argument(
		    "argument must be of the correct underlying type");
	}
	auto mat = other.cast_to<MatrixImpl<T>>();
	_matrix.set_imag(mat->_matrix);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::set_real(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_matrix(state, arg)) {
		throw std::invalid_argument("argument must be a Matrix");
	}
	// auto other = util::Reference<Matrix>::unwrap(arg);
	auto other = state->unwrap<Matrix>(arg);
	if (other->value_type() != nonscalar_value_type_v<pod_type>) {
		throw std::invalid_argument(
		    "argument must be of the correct underlying type");
	}
	auto mat = other.cast_to<MatrixImpl<T>>();
	_matrix.set_real(mat->_matrix);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::set_size(const util::Reference<util::State> &state)
{
	auto [r, c] = unwrap_size<2>(state);
	_matrix.set_size(r, c);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::size(const util::Reference<util::State> &state)
{
	return make_size(state, n_rows(), n_cols());
}

template <typename T>
mrb_value
MatrixImpl<T>::to_string(const util::Reference<util::State> &state)
{
	std::stringstream ss;
	_matrix.print(ss);
	auto mrb = state->mrb();
	return mrb->str_new_cstr(ss.str().c_str());
}

template <typename T>
mrb_value
MatrixImpl<T>::transform(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::column_count(const util::Reference<util::State> &state)
{
	return state->mrb()->int_value(static_cast<mrb_int>(n_cols()));
}

template <typename T>
mrb_value
MatrixImpl<T>::row_count(const util::Reference<util::State> &state)
{
	return state->mrb()->int_value(static_cast<mrb_int>(n_rows()));
}

template <typename T>
size_type
MatrixImpl<T>::n_rows() const
{
	return _matrix.n_rows;
}

template <typename T>
size_type
MatrixImpl<T>::n_cols() const
{
	return _matrix.n_cols;
}

template <typename T>
mrb_value
MatrixImpl<T>::column(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_int col = 0;
	mrb->get_args("i", &col);
	col = coerce_col(col);
	auto col_vec = _matrix.col(static_cast<size_type>(col));
	return VectorImpl<T>::wrap_vector(state, col_vec);
}

template <typename T>
mrb_value
MatrixImpl<T>::row(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_int row = 0;
	mrb->get_args("i", &row);
	row = coerce_row(row);
	auto row_vec = _matrix.row(static_cast<size_type>(row));
	return RowVectorImpl<T>::wrap_row_vector(state, row_vec);
}

template <typename T>
mrb_value
MatrixImpl<T>::each_column(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	_matrix.each_col([&](auto col) {
		auto col_obj = VectorImpl<T>::wrap_vector(state, col);
		mrb->yield(blk, col_obj);
	});
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::each_row(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	_matrix.each_row([&](auto row) {
		auto row_obj = RowVectorImpl<T>::wrap_row_vector(state, row);
		mrb->yield(blk, row_obj);
	});
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::eye(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::fill(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::imbue(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::insert_columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::insert_rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::is_diagonal(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::is_hermitian(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::is_square(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::is_symmetric(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::is_symmetric_positive_definite(
    const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::is_triangular_lower(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::is_triangular_upper(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::is_vector(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::ones(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::randn(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::randu(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::reshape(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::shed_columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::shed_rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::simple_transpose(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::swap(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::swap_columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::swap_rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::to_array(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::to_column(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::to_row(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::transpose(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::zeros(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::add(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::sub(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::matmul(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::mul(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::div(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::eq(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::ne(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::lt(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::le(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::gt(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::ge(const util::Reference<util::State> &state)
{
}

template <typename T>
Nonscalar::ValueType
RowVectorImpl<T>::value_type() const
{
	return nonscalar_value_type_v<T>;
}

template <typename T>
mrb_value
RowVectorImpl<T>::at(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::set_at(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::clamp(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::clean(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::copy_size(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::map(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::has_infinity(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::has_nan(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::imaginary(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::in_range(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::index_max(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::index_min(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::is_complex(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::is_empty(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::is_finite(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::is_zero(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::max(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::min(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::real(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::replace(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::reset(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::resize(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::set_imaginary(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::set_real(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::set_size(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::size(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::to_string(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::transform(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::column(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::row(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::fill(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::imbue(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::insert_rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::is_sorted(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::ones(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::randn(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::randu(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::shed_columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::shed_rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::simple_transpose(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::swap(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::swap_columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::swap_rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::transpose(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::zeros(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::add(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::sub(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::matmul(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::mul(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::div(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::eq(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::ne(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::lt(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::le(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::gt(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
RowVectorImpl<T>::ge(const util::Reference<util::State> &state)
{
}

template <typename T>
Nonscalar::ValueType
SparseMatrixImpl<T>::value_type() const
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::at(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::set_at(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::clamp(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::clean(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::copy_size(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::map(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::has_infinity(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::has_nan(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::imaginary(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::in_range(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::index_max(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::index_min(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_complex(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_empty(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_finite(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_zero(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::max(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::min(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::real(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::replace(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::reset(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::resize(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::set_imaginary(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::set_real(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::set_size(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::size(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::to_string(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::transform(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::eye(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_diagonal(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_hermitian(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_square(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_symmetric(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_symmetric_positive_definite(
    const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_triangular_lower(
    const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_triangular_upper(
    const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::is_vector(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::shed_columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::shed_rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::swap_columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::swap_rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
SparseMatrixImpl<T>::to_dense(const util::Reference<util::State> &state)
{
}

template <typename T>
VectorImpl<T>::VectorImpl(const vector_type &vec)
    : _vector(vec)
{
}

template <typename T>
template <typename U>
mrb_value
VectorImpl<T>::wrap_vector(const util::Reference<util::State> &state,
    const typename VectorImpl<U>::vector_type &vector)
{
	auto mrb = state->mrb();
	// TODO
	throw std::runtime_error("Not implemented");
}

template <typename T>
size_type
VectorImpl<T>::n_cols() const
{
	return _vector.n_cols;
}

template <typename T>
Nonscalar::Type
VectorImpl<T>::type() const
{
	return Type::Vector;
}

template <typename T>
Nonscalar::ValueType
VectorImpl<T>::value_type() const
{
	return nonscalar_value_type_v<T>;
}

template <typename T>
mrb_value
VectorImpl<T>::at(const util::Reference<util::State> &state)
{
	mrb_int index = 0;
	state->mrb()->get_args("i", &index);
	if (index < 0) index += static_cast<mrb_int>(_vector.n_elem);
	const auto value = _vector(static_cast<size_type>(index));
	return wrap_num(state->mrb(), value);
}

template <typename T>
mrb_value
VectorImpl<T>::set_at(const util::Reference<util::State> &state)
{
	mrb_int index = 0;
	mrb_value value;
	state->mrb()->get_args("io", &index, &value);
	if (index < 0) index += static_cast<mrb_int>(_vector.n_elem);
	const auto v = unwrap_num<T>(state, value);
	_vector(static_cast<size_type>(index)) = v;
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::clamp(const util::Reference<util::State> &state)
{
	mrb_value min_value, max_value;
	state->mrb()->get_args("oo", &min_value, &max_value);
	const auto min_v = unwrap_num<T>(state, min_value);
	const auto max_v = unwrap_num<T>(state, max_value);
	_vector = arma::clamp(_vector, min_v, max_v);
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::clean(const util::Reference<util::State> &state)
{
	mrb_value value;
	state->mrb()->get_args("o", &value);
	const auto tol = unwrap_num<pod_type>(state, value);
	_vector.clean(tol);
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::copy_size(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_vector(state, arg)) {
		throw std::invalid_argument("argument must be a Vector");
	}
	auto other = state->unwrap<Vector>(arg);
	_vector.set_size(other->n_cols());
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::map(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	auto arr = mrb->ary_new_capa(n_cols());
	_vector.for_each([&](T &value) {
		auto val = wrap_num(mrb, value);
		auto result = mrb->yield(blk, val);
		mrb->ary_push(arr, result);
	});
	return arr;
}

template <typename T>
mrb_value
VectorImpl<T>::has_infinity(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_vector.has_inf());
}

template <typename T>
mrb_value
VectorImpl<T>::has_nan(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_vector.has_nan());
}

template <typename T>
mrb_value
VectorImpl<T>::imaginary(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	pod_vector_type imag = arma::imag(_vector);
	return wrap_vector<pod_type>(state, imag);
}

template <typename T>
mrb_value
VectorImpl<T>::in_range(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value value;
	mrb_value aux;
	mrb->get_args("o|o", &value, &aux);
	size_type aux_v = 0;
	if (!mrb_undef_p(aux)) aux_v = unwrap_num<size_type>(state, aux);
	const auto v = unwrap_num<size_type>(state, value);
	const auto result = _vector.in_range(v, aux_v);
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
VectorImpl<T>::index_max(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto index = _vector.index_max();
	return mrb->int_value(static_cast<mrb_int>(index));
}

template <typename T>
mrb_value
VectorImpl<T>::index_min(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto index = _vector.index_min();
	return mrb->int_value(static_cast<mrb_int>(index));
}

template <typename T>
mrb_value
VectorImpl<T>::is_complex(const util::Reference<util::State> &state)
{
	static constexpr bool result = math::is_complex<T>();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
VectorImpl<T>::is_empty(const util::Reference<util::State> &state)
{
	const bool result = _vector.is_empty();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
VectorImpl<T>::is_finite(const util::Reference<util::State> &state)
{
	const bool result = _vector.is_finite();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
VectorImpl<T>::is_zero(const util::Reference<util::State> &state)
{
	const bool result = _vector.is_zero();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
VectorImpl<T>::max(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto value = _vector.max();
	return wrap_num(mrb, value);
}

template <typename T>
mrb_value
VectorImpl<T>::min(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto value = _vector.min();
	return wrap_num(mrb, value);
}

template <typename T>
mrb_value
VectorImpl<T>::real(const util::Reference<util::State> &state)
{
	pod_vector_type real = arma::real(_vector);
	return wrap_vector<pod_type>(state, real);
}

template <typename T>
mrb_value
VectorImpl<T>::replace(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value old_value, new_value;
	mrb->get_args("oo", &old_value, &new_value);
	const auto old_v = unwrap_num<T>(state, old_value);
	const auto new_v = unwrap_num<T>(state, new_value);
	_vector.replace(old_v, new_v);
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::reset(const util::Reference<util::State> &state)
{
	_vector.reset();
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::resize(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	const auto [r, _c, _s] = size_dims(state, arg);
	_vector.resize(r);
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::set_imaginary(const util::Reference<util::State> &state)
{
	if (!math::is_complex<T>()) {
		throw std::runtime_error(
		    "cannot set imaginary part of real vector");
	}
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_vector(state, arg)) {
		throw std::invalid_argument("argument must be a Vector");
	}
	auto other = state->unwrap<Vector>(arg);
	auto vec_obj = other.cast_to<VectorImpl<pod_type>>();
	_vector.set_imag(vec_obj->_vector);
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::set_real(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_vector(state, arg)) {
		throw std::invalid_argument("argument must be a Vector");
	}
	auto other = state->unwrap<Vector>(arg);
	if (other->value_type() != value_type()) {
		throw std::invalid_argument(
		    "argument must be a Vector of the same type");
	}
	auto vec = other.cast_to<VectorImpl<pod_type>>();
	_vector.set_real(vec->_vector);
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::set_size(const util::Reference<util::State> &state)
{
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::size(const util::Reference<util::State> &state)
{
	return make_size(state, n_rows());
}

template <typename T>
mrb_value
VectorImpl<T>::to_string(const util::Reference<util::State> &state)
{
	const auto mrb = state->mrb();
	std::stringstream ss;
	_vector.print(ss);
	return mrb->str_new_cstr(ss.str().c_str());
}

template <typename T>
mrb_value
VectorImpl<T>::transform(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	auto arr = mrb->ary_new_capa(n_cols());
	_vector.transform([&](T &value) {
		auto val = wrap_num(mrb, value);
		auto result = mrb->yield(blk, val);
		mrb->ary_push(arr, result);
		auto new_value = unwrap_num<T>(state, result);
		value = new_value;
		return new_value;
	});
	return arr;
}

template <typename T>
mrb_value
VectorImpl<T>::rows(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arr = mrb->ary_new_capa(_vector.n_rows);
	for (size_type i = 0; i < _vector.n_rows; ++i) {
		auto row_vec = _vector.row(i);
		auto row_obj = wrap_vector(state, row_vec);
		mrb->ary_push(arr, row_obj);
	}
	return arr;
}

template <typename T>
mrb_value
VectorImpl<T>::row(const util::Reference<util::State> &state)
{
	mrb_int index = 0;
	state->mrb()->get_args("i", &index);
	if (index < 0) index += static_cast<mrb_int>(_vector.n_rows);
	auto row_vec = _vector.row(static_cast<size_type>(index));
	return wrap_vector(state, row_vec);
}

template <typename T>
mrb_value
VectorImpl<T>::fill(const util::Reference<util::State> &state)
{
	mrb_value arg;
	state->mrb()->get_args("o", &arg);
	const auto v = unwrap_num<T>(state, arg);
	_vector.fill(v);
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::imbue(const util::Reference<util::State> &state)
{
	mrb_value blk;
	state->mrb()->get_args("&!", &blk);
	_vector.imbue([&]() -> T {
		const auto mrb = state->mrb();
		const auto result = mrb->yield(blk, mrb_nil_value());
		return unwrap_num<T>(state, result);
	});
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::is_sorted(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_vector.is_sorted());
}

template <typename T>
mrb_value
VectorImpl<T>::ones(const util::Reference<util::State> &state)
{
	mrb_value arg;
	state->mrb()->get_args("o", &arg);
	if (mrb_undef_p(arg)) {
		_vector.ones();
	} else {
		const auto n = unwrap_num<size_type>(state, arg);
		_vector.ones(n);
	}
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::randn(const util::Reference<util::State> &state)
{
	mrb_value arg;
	state->mrb()->get_args("o", &arg);
	if (mrb_undef_p(arg)) {
		_vector.randn();
	} else {
		const auto n = unwrap_num<size_type>(state, arg);
		_vector.randn(n);
	}
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::randu(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (mrb_undef_p(arg)) {
		_vector.randu();
	} else {
		const auto n = unwrap_num<size_type>(state, arg);
		_vector.randu(n);
	}
}

template <typename T>
mrb_value
VectorImpl<T>::shed_rows(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg1, arg2;
	mrb->get_args("o|o", &arg1, &arg2);
	if (!mrb_undef_p(arg2)) {
		auto low = unwrap_num<size_type>(state, arg1);
		auto high = unwrap_num<size_type>(state, arg2);
		_vector.shed_rows(low, high);
		return mrb_nil_value();
	}
	if (!mrb_array_p(arg1)) {
		auto row = unwrap_num<size_type>(state, arg1);
		_vector.shed_row(row);
		return mrb_nil_value();
	}
	arma::uvec rows;
	const auto len = RARRAY_LEN(arg1);
	rows.set_size(len);
	for (size_type i = 0; i < len; ++i) {
		const auto item = mrb->ary_ref(arg1, i);
		const auto row = unwrap_num<arma::uword>(state, item);
		rows(i) = row;
	}
	_vector.shed_rows(rows);
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::simple_transpose(const util::Reference<util::State> &state)
{
	vector_type transposed = _vector.st();
	return wrap_vector(state, transposed);
}

template <typename T>
mrb_value
VectorImpl<T>::swap(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value other;
	mrb->get_args("o", &other);
	if (!is_vector(state, other)) {
		throw std::invalid_argument("argument must be a Vector");
	}
	auto other_vec = state->unwrap<Vector>(other);
	if (other_vec->value_type() != value_type()) {
		throw std::invalid_argument(
		    "argument must be a Vector of the same type");
	}
	auto vec = other_vec.cast_to<VectorImpl<T>>();
	_vector.swap(vec->_vector);
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::swap_cols(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value src, dst;
	mrb->get_args("oo", &src, &dst);
	mrb_int src_idx = unwrap_num<mrb_int>(state, src);
	mrb_int dst_idx = unwrap_num<mrb_int>(state, dst);
	_vector.swap_cols(static_cast<size_type>(src_idx),
	    static_cast<size_type>(dst_idx));
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::swap_rows(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value src, dst;
	mrb->get_args("oo", &src, &dst);
	mrb_int src_idx = unwrap_num<mrb_int>(state, src);
	mrb_int dst_idx = unwrap_num<mrb_int>(state, dst);
	_vector.swap_rows(static_cast<size_type>(src_idx),
	    static_cast<size_type>(dst_idx));
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::transpose(const util::Reference<util::State> &state)
{
	auto transposed = _vector.t();
	return wrap_vector(state, transposed);
}

template <typename T>
mrb_value
VectorImpl<T>::zeros(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("|o", &arg);
	if (mrb_undef_p(arg)) {
		_vector.zeros();
	} else {
		const auto n = unwrap_num<size_type>(state, arg);
		_vector.zeros(n);
	}
	return mrb_nil_value();
}

template <typename T>
mrb_value
VectorImpl<T>::add(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	mrb_value out;
	apply_concrete(state, arg, [&]<typename U>(U &v) {
		if constexpr (std::is_base_of_v<Vector, std::decay_t<U>>) {
			auto result = this->_vector + v->value();
			out = wrap_math_value(state, result);
		}
	});
	return out;
}

template <typename T>
mrb_value
VectorImpl<T>::sub(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
VectorImpl<T>::matmul(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
VectorImpl<T>::mul(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
VectorImpl<T>::div(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
VectorImpl<T>::eq(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
VectorImpl<T>::ne(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
VectorImpl<T>::lt(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
VectorImpl<T>::le(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
VectorImpl<T>::gt(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
VectorImpl<T>::ge(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
VectorImpl<T>::is_size(const util::Reference<util::State> &) const
{
	return mrb_false_value();
}

template <typename T>
bool
VectorImpl<T>::is_size() const
{
	return false;
}

template <typename T>
size_type
VectorImpl<T>::n_rows() const
{
	return _vector.n_rows;
}

template <typename T>
mrb_value
VectorImpl<T>::insert_columns(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
VectorImpl<T>::insert_rows(const util::Reference<util::State> &state)
{
}
} /* namespace euler::math */

#endif /* EULER_MATH_IMPL_H */
