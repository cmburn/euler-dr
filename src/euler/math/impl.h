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
		using elem_type = T::elem_type;
		auto cube_impl
		    = util::make_reference<CubeImpl<elem_type>>(state);
		cube_impl->value() = obj;
		return state->wrap<Nonscalar>(cube_impl);
	} else if constexpr (is_matrix_v<T>) {
		using elem_type = T::elem_type;
		auto matrix_impl
		    = util::make_reference<MatrixImpl<elem_type>>(state);
		matrix_impl->value() = obj;
		return state->wrap<Nonscalar>(matrix_impl);
	} else if constexpr (is_row_vector_v<T>) {
		using elem_type = T::elem_type;
		auto row_vector_impl
		    = util::make_reference<RowVectorImpl<elem_type>>(state);
		row_vector_impl->value() = obj;
		return state->wrap<Nonscalar>(row_vector_impl);
	} else if constexpr (is_sparse_matrix_v<T>) {
		using elem_type = T::elem_type;
		auto sparse_matrix_impl
		    = util::make_reference<SparseMatrixImpl<elem_type>>(state);
		sparse_matrix_impl->value() = obj;
		return state->wrap<Nonscalar>(sparse_matrix_impl);
	} else if constexpr (is_vector_v<T>) {
		using elem_type = T::elem_type;
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
			const auto impl = base.cast_to<CubeImpl<int16_t>>();
			fn(impl);
		} break;
		case ValueType::Int32: {
			const auto base = state->unwrap<Cube>(arg);
			const auto impl = base.cast_to<CubeImpl<int32_t>>();
			fn(impl);
		} break;
		case ValueType::Int64: {
			const auto base = state->unwrap<Cube>(arg);
			const auto impl = base.cast_to<CubeImpl<int64_t>>();
			fn(impl);
		} break;
		case ValueType::UInt16: {
			const auto base = state->unwrap<Cube>(arg);
			const auto impl = base.cast_to<CubeImpl<uint16_t>>();
			fn(impl);
		} break;
		case ValueType::UInt32: {
			const auto base = state->unwrap<Cube>(arg);
			const auto impl = base.cast_to<CubeImpl<uint32_t>>();
			fn(impl);
		} break;
		case ValueType::UInt64: {
			const auto base = state->unwrap<Cube>(arg);
			const auto impl = base.cast_to<CubeImpl<uint64_t>>();
			fn(impl);
		} break;
		case ValueType::Float32: {
			const auto base = state->unwrap<Cube>(arg);
			const auto impl = base.cast_to<CubeImpl<float>>();
			fn(impl);
		} break;
		case ValueType::Float64: {
			const auto base = state->unwrap<Cube>(arg);
			const auto impl = base.cast_to<CubeImpl<double>>();
			fn(impl);
		} break;
		case ValueType::Complex32: {
			const auto base = state->unwrap<Cube>(arg);
			const auto impl
			    = base.cast_to<CubeImpl<std::complex<float>>>();
			fn(impl);
		}
		case ValueType::Complex64: {
			const auto base = state->unwrap<Cube>(arg);
			const auto impl
			    = base.cast_to<CubeImpl<std::complex<double>>>();
			fn(impl);
		}
		default: std::unreachable();
		}
		break;
	case Type::Matrix:
		switch (read_nonscalar_value_type(state, arg)) {
		case ValueType::Int16: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto impl = base.cast_to<MatrixImpl<int16_t>>();
			fn(impl);
		} break;
		case ValueType::Int32: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto impl = base.cast_to<MatrixImpl<int32_t>>();
			fn(impl);
		} break;
		case ValueType::Int64: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto impl = base.cast_to<MatrixImpl<int64_t>>();
			fn(impl);
		} break;
		case ValueType::UInt16: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto impl = base.cast_to<MatrixImpl<uint16_t>>();
			fn(impl);
		} break;
		case ValueType::UInt32: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto impl = base.cast_to<MatrixImpl<uint32_t>>();
			fn(impl);
		} break;
		case ValueType::UInt64: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto impl = base.cast_to<MatrixImpl<uint64_t>>();
			fn(impl);
		} break;
		case ValueType::Float32: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto impl = base.cast_to<MatrixImpl<float>>();
			fn(impl);
		} break;
		case ValueType::Float64: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto impl = base.cast_to<MatrixImpl<double>>();
			fn(impl);
		} break;
		case ValueType::Complex32: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto impl
			    = base.cast_to<MatrixImpl<std::complex<float>>>();
			fn(impl);
		} break;
		case ValueType::Complex64: {
			const auto base = state->unwrap<Matrix>(arg);
			const auto impl
			    = base.cast_to<MatrixImpl<std::complex<double>>>();
			fn(impl);
		}
		default: std::unreachable();
		}
		break;
	case Type::RowVector: {
		switch (read_nonscalar_value_type(state, arg)) {
		case ValueType::Int16: {
			const auto base = state->unwrap<RowVector>(arg);
			const auto impl
			    = base.cast_to<RowVectorImpl<int16_t>>();
			fn(impl);
		} break;
		case ValueType::Int32: {
			const auto base = state->unwrap<RowVector>(arg);
			const auto impl
			    = base.cast_to<RowVectorImpl<int32_t>>();
			fn(impl);
		} break;
		case ValueType::Int64: {
			const auto base = state->unwrap<RowVector>(arg);
			const auto impl
			    = base.cast_to<RowVectorImpl<int64_t>>();
			fn(impl);
		} break;
		case ValueType::UInt16: {
			const auto base = state->unwrap<RowVector>(arg);
			const auto impl
			    = base.cast_to<RowVectorImpl<uint16_t>>();
			fn(impl);
		} break;
		case ValueType::UInt32: {
			const auto base = state->unwrap<RowVector>(arg);
			const auto impl
			    = base.cast_to<RowVectorImpl<uint32_t>>();
			fn(impl);
		} break;
		case ValueType::UInt64: {
			const auto base = state->unwrap<RowVector>(arg);
			const auto impl
			    = base.cast_to<RowVectorImpl<uint64_t>>();
			fn(impl);
		} break;
		case ValueType::Float32: {
			const auto base = state->unwrap<RowVector>(arg);
			const auto impl = base.cast_to<RowVectorImpl<float>>();
			fn(impl);
		} break;
		case ValueType::Float64: {
			const auto base = state->unwrap<RowVector>(arg);
			const auto impl = base.cast_to<RowVectorImpl<double>>();
			fn(impl);
		} break;
		case ValueType::Complex32: {
			const auto base = state->unwrap<RowVector>(arg);
			const auto impl = base.cast_to<
			    RowVectorImpl<std::complex<float>>>();
			fn(impl);
		} break;
		case ValueType::Complex64: {
			const auto base = state->unwrap<RowVector>(arg);
			const auto impl = base.cast_to<
			    RowVectorImpl<std::complex<double>>>();
			fn(impl);
		} break;
		default: std::unreachable();
		}
	} break;
	case Type::SparseMatrix:
		switch (read_nonscalar_value_type(state, arg)) {
		case ValueType::Int16: {
			const auto base = state->unwrap<SparseMatrix>(arg);
			const auto impl
			    = base.cast_to<SparseMatrixImpl<int16_t>>();
			fn(impl);
		} break;
		case ValueType::Int32: {
			const auto base = state->unwrap<SparseMatrix>(arg);
			const auto impl
			    = base.cast_to<SparseMatrixImpl<int32_t>>();
			fn(impl);
		} break;
		case ValueType::Int64: {
			const auto base = state->unwrap<SparseMatrix>(arg);
			const auto impl
			    = base.cast_to<SparseMatrixImpl<int64_t>>();
			fn(impl);
		} break;
		case ValueType::UInt16: {
			const auto base = state->unwrap<SparseMatrix>(arg);
			const auto impl
			    = base.cast_to<SparseMatrixImpl<uint16_t>>();
			fn(impl);
		} break;
		case ValueType::UInt32: {
			const auto base = state->unwrap<SparseMatrix>(arg);
			const auto impl
			    = base.cast_to<SparseMatrixImpl<uint32_t>>();
			fn(impl);
		} break;
		case ValueType::UInt64: {
			const auto base = state->unwrap<SparseMatrix>(arg);
			const auto impl
			    = base.cast_to<SparseMatrixImpl<uint64_t>>();
			fn(impl);
		} break;
		case ValueType::Float32: {
			const auto base = state->unwrap<SparseMatrix>(arg);
			const auto impl
			    = base.cast_to<SparseMatrixImpl<float>>();
			fn(impl);
		} break;
		case ValueType::Float64: {
			const auto base = state->unwrap<SparseMatrix>(arg);
			const auto impl
			    = base.cast_to<SparseMatrixImpl<double>>();
			fn(impl);
		} break;
		case ValueType::Complex32: {
			const auto base = state->unwrap<SparseMatrix>(arg);
			const auto impl = base.cast_to<
			    SparseMatrixImpl<std::complex<float>>>();
			fn(impl);
		} break;
		case ValueType::Complex64: {
			const auto base = state->unwrap<SparseMatrix>(arg);
			const auto impl = base.cast_to<
			    SparseMatrixImpl<std::complex<double>>>();
			fn(impl);
		} break;
		}
		break;
	case Type::Vector:
		switch (read_nonscalar_value_type(state, arg)) {
		case ValueType::Int16: {
			const auto base = state->unwrap<Vector>(arg);
			const auto impl = base.cast_to<VectorImpl<int16_t>>();
			fn(impl);
		} break;
		case ValueType::Int32: {
			const auto base = state->unwrap<Vector>(arg);
			const auto impl = base.cast_to<VectorImpl<int32_t>>();
			fn(impl);
		} break;
		case ValueType::Int64: {
			const auto base = state->unwrap<Vector>(arg);
			const auto impl = base.cast_to<VectorImpl<int64_t>>();
			fn(impl);
		} break;
		case ValueType::UInt16: {
			const auto base = state->unwrap<Vector>(arg);
			const auto impl = base.cast_to<VectorImpl<uint16_t>>();
			fn(impl);
		} break;
		case ValueType::UInt32: {
			const auto base = state->unwrap<Vector>(arg);
			const auto impl = base.cast_to<VectorImpl<uint32_t>>();
			fn(impl);
		} break;
		case ValueType::UInt64: {
			const auto base = state->unwrap<Vector>(arg);
			const auto impl = base.cast_to<VectorImpl<uint64_t>>();
			fn(impl);
		} break;
		case ValueType::Float32: {
			const auto base = state->unwrap<Vector>(arg);
			const auto impl = base.cast_to<VectorImpl<float>>();
			fn(impl);
		} break;
		case ValueType::Float64: {
			const auto base = state->unwrap<Vector>(arg);
			const auto impl = base.cast_to<VectorImpl<double>>();
			fn(impl);
		} break;
		case ValueType::Complex32: {
			const auto base = state->unwrap<Vector>(arg);
			const auto impl
			    = base.cast_to<VectorImpl<std::complex<float>>>();
			fn(impl);
		} break;
		case ValueType::Complex64: {
			const auto base = state->unwrap<Vector>(arg);
			const auto impl
			    = base.cast_to<VectorImpl<std::complex<double>>>();
			fn(impl);
		} break;
		}
		break;
	default: std::unreachable();
	}
}

#define WRAP_MATH_METHOD(TYPE, METHOD)                                         \
	([](mrb_state *mrb, mrb_value self) -> mrb_value {                     \
		auto state = util::State::get(mrb);                            \
		auto impl = state->unwrap<TYPE>(self);                         \
		auto ptr = impl.get();                                         \
		return (ptr->*(&TYPE::METHOD))(state);                         \
	})

#define EULER_DEFINE_METHOD_ALT(NAME, SYM, ARGS)                               \
	do {                                                                   \
		mrb->define_method(cls, (#SYM), (WRAP_MATH_METHOD(T, NAME)),   \
		    (ARGS));                                                   \
	} while (0)

#define EULER_DEFINE_METHOD(NAME, ARGS)                                        \
	do {                                                                   \
		mrb->define_method(cls, (#NAME), (WRAP_MATH_METHOD(T, NAME)),  \
		    (ARGS));                                                   \
	} while (0)

template <typename T>
void
register_nonscalar_methods(const util::Reference<util::RubyState> &mrb,
    RClass *cls)
{
#define DEF(NAME, ARGS) EULER_DEFINE_METHOD(NAME, ARGS)
#define DEFQ(NAME, SYM, ARGS) EULER_DEFINE_METHOD_ALT(NAME, SYM, ARGS)
	// Register common nonscalar methods. Leave [] and []= to the specific
	// types, since they vary in argument count.
	// mrb->define_method(cls, "clamp", WRAP_MATH_METHOD(T, clamp),
	// MRB_ARGS_REQ(2)); mrb->define_method(cls, "clean",
	// WRAP_MATH_METHOD(T, clean), MRB_ARGS_REQ(1));
	DEF(clamp, MRB_ARGS_REQ(2));
	DEF(clean, MRB_ARGS_REQ(1));
	DEF(copy_size, MRB_ARGS_REQ(1));
	DEF(map, MRB_ARGS_BLOCK());
	DEFQ(has_infinity, infinity?, MRB_ARGS_NONE());
	DEFQ(has_nan, nan?, MRB_ARGS_NONE());
	DEF(imaginary, MRB_ARGS_NONE());
	DEFQ(in_range, in_range?, MRB_ARGS_REQ(2));
	DEF(index_max, MRB_ARGS_NONE());
	DEF(index_min, MRB_ARGS_NONE());
	DEFQ(is_complex, complex?, MRB_ARGS_NONE());
	DEFQ(is_empty, empty?, MRB_ARGS_NONE());
	DEFQ(is_finite, finite?, MRB_ARGS_NONE());
	DEFQ(is_zero, zero?, MRB_ARGS_NONE());
	DEF(max, MRB_ARGS_NONE());
	DEF(min, MRB_ARGS_NONE());
	DEF(real, MRB_ARGS_NONE());
	DEF(replace, MRB_ARGS_REQ(2));
	DEF(reset, MRB_ARGS_NONE());
	DEF(resize, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	DEFQ(set_imaginary, imaginary =, MRB_ARGS_REQ(1));
	DEFQ(set_real, real =, MRB_ARGS_REQ(1));
	DEFQ(set_size, size =, MRB_ARGS_REQ(1));
	DEF(size, MRB_ARGS_NONE());
	DEFQ(to_string, to_s, MRB_ARGS_NONE());
	DEF(transform, MRB_ARGS_BLOCK());
#undef DEFQ
#undef DEF
}

/*
 * given an object can be one of 6 container types,
 * and each container can have one of 10 types:
 * need a function that will take two values, cast them to their concrete types,
 * and attempt to apply a function to the two.
 */

} /* namespace euler::math */

#endif /* EULER_MATH_IMPL_H */
