/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_VECTOR_H
#define EULER_MATH_VECTOR_H

#include <sstream>

#include "euler/math/nonscalar.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"

namespace euler::math {
class Vector : public Nonscalar {
	BIND_MRUBY("Euler::Math::Vector", Vector, math.vector);

protected:
	static bool
	is_vector(const util::Reference<util::State> &state,
	    const mrb_value arg)
	{
		return Nonscalar::type(state, arg) == Type::Vector;
	}

public:
	virtual bool is_size() const = 0;
	virtual size_type n_cols() const = 0;
	virtual size_type n_rows() const = 0;

	virtual mrb_value rows(const util::Reference<util::State> &state) = 0;
	virtual mrb_value row(const util::Reference<util::State> &state) = 0;
	virtual mrb_value fill(const util::Reference<util::State> &state) = 0;
	virtual mrb_value imbue(const util::Reference<util::State> &state) = 0;
	virtual mrb_value insert_columns(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value insert_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_sorted(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value ones(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randn(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randu(const util::Reference<util::State> &state) = 0;
	virtual mrb_value shed_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value simple_transpose(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value swap(const util::Reference<util::State> &state) = 0;
	virtual mrb_value swap_cols(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value swap_rows(const util::Reference<util::State> &state)
	    = 0;

	virtual mrb_value transpose(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value zeros(const util::Reference<util::State> &state) = 0;
	virtual mrb_value is_size(
	    const util::Reference<util::State> &state) const
	    = 0;

protected:
	virtual util::Reference<Vector> as_vector_impl() = 0;
};

std::tuple<size_type, size_type, size_type> size_dims(
    const util::Reference<util::State> &state, mrb_value arg);
mrb_value make_size(const util::Reference<util::State> &state, size_type rows,
    size_type cols = 0, size_type slices = 0);

template <typename T, size_type Rows = dynamic_size>
class VectorImpl final : public Vector {
public:
	typedef T elem_type;
	typedef arma::Col<T> vector_type;
	typedef detail::pod_type_t<T> pod_type;
	typedef arma::Col<pod_type> pod_vector_type;
	friend class Size;
	template <typename , size_type> friend class VectorImpl;
	template <typename , size_type, size_type > friend class MatrixImpl;

	VectorImpl(const vector_type &vec);

	template <typename U = T>
	mrb_value
	wrap_vector(const util::Reference<util::State> &state,
	    const typename VectorImpl<U>::vector_type &vector)
	{
		auto mrb = state->mrb();
		// TODO
		throw std::runtime_error("Not implemented");
	}

	size_type
	n_cols() const
	{
		return _vector.n_cols;
	}

	Nonscalar::Type
	type() const
	{
		return Type::Vector;
	}

	Nonscalar::ValueType
	value_type() const
	{
		return nonscalar_value_type_v<T>;
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
	mrb_value rows(const util::Reference<util::State> &state) override;
	mrb_value row(const util::Reference<util::State> &state) override;
	mrb_value fill(const util::Reference<util::State> &state) override;
	mrb_value imbue(const util::Reference<util::State> &state) override;
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

	mrb_value is_size(const util::Reference<util::State> &) const override;

	bool is_size() const override;
	size_type n_rows() const override;
	mrb_value insert_columns(
	    const util::Reference<util::State> &state) override;
	mrb_value insert_rows(
	    const util::Reference<util::State> &state) override;

	template <typename U>
	util::Reference<VectorImpl<U>>
	coerce_type()
	{
	}

protected:
	util::Reference<Vector>
	as_vector_impl() override
	{
		return util::Reference<Vector>(this);
	}

private:
	vector_type _vector;
};

template <typename T>
util::Reference<VectorImpl<T>>
cast_vector(const util::Reference<Vector> &vec)
{
	switch (vec->value_type()) {
	case Nonscalar::ValueType::Int16:
		return vec.cast_to<VectorImpl<int16_t>>()->coerce_type<T>();
	case Nonscalar::ValueType::Int32:
		return vec.cast_to<VectorImpl<int32_t>>()->coerce_type<T>();
	case Nonscalar::ValueType::Int64:
		return vec.cast_to<VectorImpl<int64_t>>()->coerce_type<T>();
	case Nonscalar::ValueType::UInt16:
		return vec.cast_to<VectorImpl<uint16_t>>()->coerce_type<T>();
	case Nonscalar::ValueType::UInt32:
		return vec.cast_to<VectorImpl<uint32_t>>()->coerce_type<T>();
	case Nonscalar::ValueType::UInt64:
		return vec.cast_to<VectorImpl<uint64_t>>()->coerce_type<T>();
	case Nonscalar::ValueType::Float32:
		return vec.cast_to<VectorImpl<float>>()->coerce_type<T>();
	case Nonscalar::ValueType::Float64:
		return vec.cast_to<VectorImpl<double>>()->coerce_type<T>();
	case Nonscalar::ValueType::Complex64:
		return vec.cast_to<VectorImpl<std::complex<float>>>()
		    ->coerce_type<T>();
	case Nonscalar::ValueType::Complex128:
		return vec.cast_to<VectorImpl<std::complex<double>>>()
		    ->coerce_type<T>();
	default: std::unreachable();
	}
}

} /* namespace euler::math */

#endif /* EULER_MATH_VECTOR_H */
