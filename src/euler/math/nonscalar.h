/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_NONSCALAR_H
#define EULER_MATH_NONSCALAR_H

#include "euler/math/util.h"
#include "euler/util/object.h"
#include "euler/util/state.h"

#include <mruby.h>

#include "euler/util/ext.h"

namespace euler::math {
class Nonscalar : public util::Object {
	BIND_MRUBY("Euler::Math::Nonscalar", Nonscalar, math.nonscalar);
public:
	enum class Type {
		Cube,
		Matrix,
		RowVector,
		SparseMatrix,
		Vector,
	};

	enum class ValueType {
		Int16,
		Int32,
		Int64,
		UInt16,
		UInt32,
		UInt64,
		Float32,
		Float64,
		Complex64,
		Complex128,
	};

	template <typename T>
	struct Range {
		T start;
		T step;
		T end;
	};

	virtual Type type() const = 0;
	static Type type(const util::Reference<util::State> &state,
	    mrb_value arg);
	virtual ValueType value_type() const = 0;
	static ValueType value_type(const util::Reference<util::State> &state,
	    mrb_value arg);

	virtual mrb_value at(const util::Reference<util::State> &state) = 0;
	virtual mrb_value set_at(const util::Reference<util::State> &state) = 0;
	virtual mrb_value clamp(const util::Reference<util::State> &state) = 0;
	virtual mrb_value clean(const util::Reference<util::State> &state) = 0;
	virtual mrb_value copy_size(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value map(const util::Reference<util::State> &state) = 0;
	virtual mrb_value has_infinity(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value has_nan(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value imaginary(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value in_range(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value index_max(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value index_min(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_complex(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_empty(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_finite(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value is_zero(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value max(const util::Reference<util::State> &state) = 0;
	virtual mrb_value min(const util::Reference<util::State> &state) = 0;
	virtual mrb_value real(const util::Reference<util::State> &state) = 0;
	virtual mrb_value replace(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value reset(const util::Reference<util::State> &state) = 0;
	virtual mrb_value resize(const util::Reference<util::State> &state) = 0;
	virtual mrb_value set_imaginary(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value set_real(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value set_size(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value size(const util::Reference<util::State> &state) = 0;
	virtual mrb_value to_string(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value transform(const util::Reference<util::State> &state)
	    = 0;
};

template <typename T> struct nonscalar_value_type : std::false_type { };

template <> struct nonscalar_value_type<int16_t> {
	static constexpr auto value = Nonscalar::ValueType::Int16;
};

template <> struct nonscalar_value_type<int32_t> {
	static constexpr auto value = Nonscalar::ValueType::Int32;
};

template <> struct nonscalar_value_type<int64_t> {
	static constexpr auto value = Nonscalar::ValueType::Int64;
};

template <> struct nonscalar_value_type<uint16_t> {
	static constexpr auto value = Nonscalar::ValueType::UInt16;
};

template <> struct nonscalar_value_type<uint32_t> {
	static constexpr auto value = Nonscalar::ValueType::UInt32;
};

template <> struct nonscalar_value_type<uint64_t> {
	static constexpr auto value = Nonscalar::ValueType::UInt64;
};

template <> struct nonscalar_value_type<size_type> {
	static constexpr auto value = Nonscalar::ValueType::UInt64;
};

template <> struct nonscalar_value_type<float> {
	static constexpr auto value = Nonscalar::ValueType::Float32;
};

template <> struct nonscalar_value_type<double> {
	static constexpr auto value = Nonscalar::ValueType::Float64;
};

template <> struct nonscalar_value_type<std::complex<float>> {
	static constexpr auto value = Nonscalar::ValueType::Complex64;
};

template <> struct nonscalar_value_type<std::complex<double>> {
	static constexpr auto value = Nonscalar::ValueType::Complex128;
};

template <typename T>
static constexpr auto nonscalar_value_type_v = nonscalar_value_type<T>::value;



} /* namespace euler::math */

#endif /* EULER_MATH_NONSCALAR_H */
