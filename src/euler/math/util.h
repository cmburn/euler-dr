/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_UTIL_H
#define EULER_MATH_UTIL_H

#include <cassert>
#include <cstdint>

#include <armadillo>

#include "euler/util/mruby_state.h"

namespace euler::math {

typedef arma::size_t size_type;

static constexpr size_type dynamic_size = std::numeric_limits<size_type>::max();

namespace detail {
template <typename T>
static constexpr bool
is_supported_numeric()
{
	if constexpr (std::is_same_v<T, int16_t>) return true;
	if constexpr (std::is_same_v<T, int32_t>) return true;
	if constexpr (std::is_same_v<T, int64_t>) return true;
	if constexpr (std::is_same_v<T, uint16_t>) return true;
	if constexpr (std::is_same_v<T, uint32_t>) return true;
	if constexpr (std::is_same_v<T, uint64_t>) return true;
	if constexpr (std::is_same_v<T, float>) return true;
	if constexpr (std::is_same_v<T, double>) return true;
	if constexpr (std::is_same_v<T, std::complex<float>>) return true;
	if constexpr (std::is_same_v<T, std::complex<double>>) return true;
	return false;
}

template <typename T>
static constexpr bool
is_complex()
{
	if constexpr (std::is_same_v<T, std::complex<float>>) return true;
	if constexpr (std::is_same_v<T, std::complex<double>>) return true;
	return false;
}

template <typename T>
static constexpr bool
can_decompose()
{
	if constexpr (std::is_same_v<T, float>) return true;
	if constexpr (std::is_same_v<T, double>) return true;
	if constexpr (std::is_same_v<T, std::complex<float>>) return true;
	if constexpr (std::is_same_v<T, std::complex<double>>) return true;
	return false;
}

/* ReSharper disable once CppDFAUnreachableFunctionCall */
template <typename T>
static std::complex<T>
unwrap_complex(const util::Reference<util::MRubyState> &mrb, mrb_value arg)
{
	const auto real_value = mrb->funcall(arg, "real", 0);
	const auto imag_value = mrb->funcall(arg, "imag", 0);
	const auto real = static_cast<T>(mrb->to_flo(real_value));
	const auto imag = static_cast<T>(mrb->to_flo(imag_value));
	return std::complex<T>(real, imag);
}

template <typename T>
static mrb_value
wrap_complex(const util::Reference<util::MRubyState> &mrb, const std::complex<T> &v)
{
	auto real = mrb->float_value(static_cast<mrb_float>(v.real()));
	auto imag = mrb->float_value(static_cast<mrb_float>(v.imag()));
	auto cls = mrb->class_get("Complex");
	mrb_value obj = mrb->instance_new()

}

template <typename T>
size_type
coerce_size(T n)
{
	static_assert(!detail::is_complex<T>(), "size cannot be complex");
	if constexpr (std::is_floating_point_v<T>) n = arma::round(n);
	return static_cast<size_type>(n);
}

template <typename T> struct numeric_info : std::false_type {
	static_assert(detail::is_supported_numeric<T>());
};

template <> struct numeric_info<int16_t> {
	typedef mrb_int mrb_type;
	static constexpr auto abbreviation = "i16";
	static constexpr auto can_decompose = false;
};

template <> struct numeric_info<int32_t> {
	typedef mrb_int mrb_type;
	static constexpr auto abbreviation = "i32";
	static constexpr auto can_decompose = false;
};

template <> struct numeric_info<int64_t> {
	typedef mrb_int mrb_type;
	static constexpr auto abbreviation = "i64";
	static constexpr auto can_decompose = false;
};

template <> struct numeric_info<uint16_t> {
	typedef mrb_uint mrb_type;
	static constexpr auto abbreviation = "u16";
	static constexpr auto can_decompose = false;
};

template <> struct numeric_info<uint32_t> {
	typedef mrb_uint mrb_type;
	static constexpr auto abbreviation = "u32";
	static constexpr auto can_decompose = false;
};

template <> struct numeric_info<uint64_t> {
	typedef mrb_uint mrb_type;
	static constexpr auto abbreviation = "u64";
	static constexpr auto can_decompose = false;
};

template <> struct numeric_info<float> {
	typedef mrb_float mrb_type;
	static constexpr auto abbreviation = "";
	static constexpr auto can_decompose = true;
};

template <> struct numeric_info<double> {
	typedef mrb_float mrb_type;
	static constexpr auto abbreviation = "f64";
	static constexpr auto can_decompose = true;
};

template <> struct numeric_info<std::complex<float>> {
	typedef mrb_value mrb_type;
	static constexpr auto abbreviation = "c32";
	static constexpr auto can_decompose = true;
};

template <> struct numeric_info<std::complex<double>> {
	typedef mrb_value mrb_type;
	static constexpr auto abbreviation = "c64";
	static constexpr auto can_decompose = true;
};

template <typename T>
static T
unwrap_numeric(const util::Reference<util::MRubyState> &mrb, mrb_value arg)
{
	static_assert(detail::is_supported_numeric<T>());
	/* ReSharper disable CppRedundantCastExpression */
	if constexpr (std::is_same_v<T, int16_t>)
		return static_cast<int16_t>(mrb_integer(mrb->to_int(arg)));
	if constexpr (std::is_same_v<T, int32_t>)
		return static_cast<int32_t>(mrb_integer(mrb->to_int(arg)));
	if constexpr (std::is_same_v<T, int64_t>)
		return static_cast<int64_t>(mrb_integer(mrb->to_int(arg)));
	if constexpr (std::is_same_v<T, uint16_t>)
		return static_cast<uint16_t>(mrb_integer(mrb->to_int(arg)));
	if constexpr (std::is_same_v<T, uint32_t>)
		return static_cast<uint32_t>(mrb_integer(mrb->to_int(arg)));
	if constexpr (std::is_same_v<T, uint64_t>)
		return static_cast<uint64_t>(mrb_integer(mrb->to_int(arg)));
	if constexpr (std::is_same_v<T, float>)
		return static_cast<float>(mrb->to_flo(arg));
	if constexpr (std::is_same_v<T, double>)
		return static_cast<double>(mrb->to_flo(arg));
	if constexpr (std::is_same_v<T, std::complex<float>>)
		return detail::unwrap_complex<float>(mrb, arg);
	if constexpr (std::is_same_v<T, std::complex<double>>)
		return detail::unwrap_complex<double>(mrb, arg);
	/* ReSharper restore CppRedundantCastExpression */
	return {};
}

template <typename T>
mrb_value
value_to_mruby(const util::Reference<util::MRubyState> &mrb, T v)
{
	static_assert(detail::is_supported_numeric<T>());
	mrb->obj_value()
	if constexpr (std::is_same_v<T, int16_t>) return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<T, int32_t>) return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<T, int64_t>) return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<T, uint16_t>) return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<T, uint32_t>) return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<T, uint64_t>) return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<T, float>) return mrb->float_value(static_cast<mrb_float>(v));
	if constexpr (std::is_same_v<T, double>) return mrb->float_value(static_cast<mrb_float>(v));
	if constexpr (std::is_same_v<T, std::complex<float>>) {
		auto real = mrb->float_value(static_cast<mrb_float>(v.real()));
		auto imag = mrb->float_value(static_cast<mrb_float>(v.imag()));
		return mrb->funcall(mrb->module_get("Complex"), "new", 2, real, imag);
	}
	if constexpr (std::is_same_v<T, std::complex<double>>) {
		auto real = mrb->float_value(static_cast<mrb_float>(v.real()));
		auto imag = mrb->float_value(static_cast<mrb_float>(v.imag()));
		return mrb->funcall(mrb->module_get("Complex"), "new", 2, real, imag);
	}
	return mrb->nil_value();

}

} /* namespace euler::math */

#endif /* EULER_MATH_UTIL_H */
