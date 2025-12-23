/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_NON_SCALAR_H
#define EULER_MATH_NON_SCALAR_H

#include <complex>

#include "euler/util/mruby_state.h"
#include "euler/util/object.h"
#include "euler/math/util.h"

namespace euler::math {
class Nonscalar : public util::Object {
public:
	enum class Kind {
		UInt16,
		UInt32,
		UInt64,
		Int16,
		Int32,
		Int64,
		Float32,
		Float64,
		Complex32,
		Complex64,
	};

	enum class Type {
		Vector,
		RowVector,
		Matrix,
		Cube,
	};

	[[nodiscard]] virtual Kind kind() const = 0;
	[[nodiscard]] virtual Type type() const = 0;
	[[nodiscard]] virtual size_type n_cols() const = 0;
	[[nodiscard]] virtual size_type n_rows() const = 0;
	[[nodiscard]] virtual size_type n_elem() const = 0;
	[[nodiscard]] virtual bool is_fixed_size() const = 0;
	[[nodiscard]] virtual mrb_value to_mrb(
	    const util::Reference<util::State> &mrb) const
	    = 0;
	[[nodiscard]] virtual mrb_value to_array(
	    const util::Reference<util::State> &mrb) const
	    = 0;
};

template <typename T> struct nonscalar_kind : std::false_type { };

template <> struct nonscalar_kind<uint16_t> {
	static constexpr auto value = Nonscalar::Kind::UInt16;
};
template <> struct nonscalar_kind<uint32_t> {
	static constexpr auto value = Nonscalar::Kind::UInt32;
};
template <> struct nonscalar_kind<uint64_t> {
	static constexpr auto value = Nonscalar::Kind::UInt64;
};
template <> struct nonscalar_kind<int16_t> {
	static constexpr auto value = Nonscalar::Kind::Int16;
};
template <> struct nonscalar_kind<int32_t> {
	static constexpr auto value = Nonscalar::Kind::Int32;
};
template <> struct nonscalar_kind<int64_t> {
	static constexpr auto value = Nonscalar::Kind::Int64;
};
template <> struct nonscalar_kind<float> {
	static constexpr auto value = Nonscalar::Kind::Float32;
};
template <> struct nonscalar_kind<double> {
	static constexpr auto value = Nonscalar::Kind::Float64;
};
template <> struct nonscalar_kind<std::complex<float>> {
	static constexpr auto value = Nonscalar::Kind::Complex32;
};
template <> struct nonscalar_kind<std::complex<double>> {
	static constexpr auto value = Nonscalar::Kind::Complex64;
};

template <typename T>
static inline Nonscalar::Kind nonscalar_kind_v = nonscalar_kind<T>::value;

} /* namespace euler::math */

#endif /* EULER_MATH_NON_SCALAR_H */
