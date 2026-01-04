/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_UTIL_H
#define EULER_MATH_UTIL_H

#include <cassert>
#include <cstdint>

#include <armadillo>

#include "euler/util/ruby_state.h"
#include "euler/util/state.h"

namespace euler::math {

typedef decltype(arma::size(arma::vec())(0)) size_type;

static constexpr size_type size_type_size = sizeof(size_type);

static constexpr size_type dynamic_size = std::numeric_limits<size_type>::max();

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
	if constexpr (std::is_same_v<T, size_type>) return true;
	if constexpr (std::is_same_v<T, float>) return true;
	if constexpr (std::is_same_v<T, double>) return true;
	if constexpr (std::is_same_v<T, std::complex<float>>) return true;
	if constexpr (std::is_same_v<T, std::complex<double>>) return true;
	return false;
}

static_assert(is_supported_numeric<size_type>());

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
unwrap_complex(const util::Reference<util::RubyState> &mrb, mrb_value arg)
{
	const auto real_value = mrb->funcall(arg, "real", 0);
	const auto imag_value = mrb->funcall(arg, "imag", 0);
	const auto real = static_cast<T>(mrb->to_flo(real_value));
	const auto imag = static_cast<T>(mrb->to_flo(imag_value));
	return std::complex<T>(real, imag);
}

template <typename T>
static mrb_value
wrap_complex(const util::Reference<util::RubyState> &mrb,
    const std::complex<T> &v)
{
	const auto real = mrb->float_value(static_cast<mrb_float>(v.real()));
	const auto imag = mrb->float_value(static_cast<mrb_float>(v.imag()));
	const auto cls = mrb->class_get("Complex");
	const std::array args = { real, imag };
	return mrb->obj_new(cls, args.size(), args.data());
}

template <typename T>
size_type
coerce_size(T n)
{
	static_assert(!is_complex<T>(), "size cannot be complex");
	if constexpr (std::is_floating_point_v<T>) n = arma::round(n);
	return static_cast<size_type>(n);
}

template <typename T> struct numeric_info : std::false_type {
	static_assert(is_supported_numeric<T>());
};

template <> struct numeric_info<int16_t> {
	typedef mrb_int mrb_type;
	static constexpr auto abbreviation = "I16";
	static constexpr auto can_decompose = false;
	static constexpr bool is_complex = false;
};

template <> struct numeric_info<int32_t> {
	typedef mrb_int mrb_type;
	static constexpr auto abbreviation = "I32";
	static constexpr auto can_decompose = false;
	static constexpr bool is_complex = false;
};

template <> struct numeric_info<int64_t> {
	typedef mrb_int mrb_type;
	static constexpr auto abbreviation = "I64";
	static constexpr auto can_decompose = false;
	static constexpr bool is_complex = false;
};

template <> struct numeric_info<uint16_t> {
	typedef mrb_uint mrb_type;
	static constexpr auto abbreviation = "U16";
	static constexpr auto can_decompose = false;
	static constexpr bool is_complex = false;
};

template <> struct numeric_info<uint32_t> {
	typedef mrb_uint mrb_type;
	static constexpr auto abbreviation = "U32";
	static constexpr auto can_decompose = false;
	static constexpr bool is_complex = false;
};

template <> struct numeric_info<uint64_t> {
	typedef mrb_uint mrb_type;
	static constexpr auto abbreviation = "U64";
	static constexpr auto can_decompose = false;
	static constexpr bool is_complex = false;
};

template <> struct numeric_info<float> {
	typedef mrb_float mrb_type;
	static constexpr auto abbreviation = "F32";
	static constexpr auto can_decompose = true;
	static constexpr bool is_complex = false;
};

template <> struct numeric_info<double> {
	typedef mrb_float mrb_type;
	static constexpr auto abbreviation = "F64";
	static constexpr auto can_decompose = true;
};

template <> struct numeric_info<std::complex<float>> {
	typedef mrb_value mrb_type;
	static constexpr auto abbreviation = "C32";
	static constexpr auto can_decompose = true;
};

template <> struct numeric_info<std::complex<double>> {
	typedef mrb_value mrb_type;
	static constexpr auto abbreviation = "C64";
	static constexpr auto can_decompose = true;
};

template <typename T>
static T
unwrap_num(const util::Reference<util::State> &state, mrb_value arg)
{

	static_assert(is_supported_numeric<T>());
	/* ReSharper disable CppRedundantCastExpression */
	if constexpr (std::is_same_v<T, int16_t>) {
		return static_cast<int16_t>(
		    mrb_integer(state->mrb()->to_int(arg)));
	}
	if constexpr (std::is_same_v<T, int32_t>) {
		return static_cast<int32_t>(
		    mrb_integer(state->mrb()->to_int(arg)));
	}
	if constexpr (std::is_same_v<T, int64_t>) {
		return static_cast<int64_t>(
		    mrb_integer(state->mrb()->to_int(arg)));
	}
	if constexpr (std::is_same_v<T, uint16_t>) {
		return static_cast<uint16_t>(
		    mrb_integer(state->mrb()->to_int(arg)));
	}
	if constexpr (std::is_same_v<T, uint32_t>) {
		return static_cast<uint32_t>(
		    mrb_integer(state->mrb()->to_int(arg)));
	}
	if constexpr (std::is_same_v<T, uint64_t>
	    || std::is_same_v<T, size_type>) {
		return static_cast<uint64_t>(
		    mrb_integer(state->mrb()->to_int(arg)));
	}
	if constexpr (std::is_same_v<T, float>) {
		return static_cast<float>(state->mrb()->to_flo(arg));
	}
	if constexpr (std::is_same_v<T, double>) {
		return static_cast<double>(state->mrb()->to_flo(arg));
	}
	if constexpr (std::is_same_v<T, std::complex<float>>) {
		return unwrap_complex<float>(state->mrb(), arg);
	}
	if constexpr (std::is_same_v<T, std::complex<double>>) {
		return unwrap_complex<double>(state->mrb(), arg);
	}
	/* ReSharper restore CppRedundantCastExpression */
	return {};
}

template <typename T>
mrb_value
wrap_num(const util::Reference<util::RubyState> &mrb, T v)
{
	using value_type = std::remove_cv_t<std::remove_reference_t<T>>;
	static_assert(is_supported_numeric<value_type>());
	if constexpr (std::is_same_v<value_type, int16_t>)
		return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<value_type, int32_t>)
		return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<value_type, int64_t>)
		return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<value_type, uint16_t>)
		return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<value_type, uint32_t>)
		return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<value_type, uint64_t>)
		return mrb->int_value(static_cast<mrb_int>(v));
	if constexpr (std::is_same_v<value_type, float>)
		return mrb->float_value(static_cast<mrb_float>(v));
	if constexpr (std::is_same_v<value_type, double>)
		return mrb->float_value(static_cast<mrb_float>(v));
	if constexpr (std::is_same_v<value_type, std::complex<float>>)
		return wrap_complex<float>(mrb, v);
	if constexpr (std::is_same_v<value_type, std::complex<double>>)
		return wrap_complex<double>(mrb, v);
	return mrb_nil_value();
}

namespace detail {
template <size_type... digits> struct to_chars {
	static const char value[];
};

template <size_type... digits>
constexpr char to_chars<digits...>::value[] = { ('0' + digits)..., 0 };

template <size_type rem, size_type... digits>
struct explode : explode<rem / 10, rem % 10, digits...> { };

template <size_type... digits>
struct explode<0, digits...> : to_chars<digits...> { };

template <size_type num> struct num_to_string : detail::explode<num> { };

template <size_type num>
static inline constexpr auto num_str = num_to_string<num>::value;
}

template <size_type N>
static constexpr const char *
dim_str_value()
{
	if constexpr (N == dynamic_size) return "X";
	return detail::num_str<N>;
}

template <size_type N>
static inline constexpr const char *dim_str = dim_str_value<N>();

enum class FillForm {
	None,
	Zeros,
	Ones,
	Eye,
	Randu,
	Randn,
	Value,
};

FillForm to_fill_type(const util::Reference<util::State> &state,
    const mrb_sym sym);

template <typename T>
static T
fill_dynamic(const util::Reference<util::State> &state, size_type rows,
    size_type cols, mrb_value value)
{
	printf("Matrix::fill_dynamic()\n");
	printf("mrb_state: %p\n", state->mrb()->mrb());
	printf("value nil? %s\n", mrb_nil_p(value) ? "yes" : "no");
	auto cls = state->mrb()->obj_class(value);

	printf("cls: %p\n", cls);
	const auto cls_name = state->mrb()->class_name(cls);
	printf("arg type: %s\n", cls_name);
	if (mrb_nil_p(value))
		value = mrb_symbol_value(state->mrb()->intern_cstr("zeros"));
	printf("arg type: %s\n",
	    state->mrb()->class_name(state->mrb()->obj_class(value)));
	if (!mrb_symbol_p(value)) {
		printf("Matrix::fill_dynamic: non-sym\n");
		const auto v = unwrap_num<typename T::elem_type>(state, value);
		return T(rows, cols, arma::fill::value(v));
	}
	printf("Matrix::fill_dynamic: sym");
	const auto sym = mrb_symbol(value);
	switch (to_fill_type(state, sym)) {
	case FillForm::None: return T(rows, cols, arma::fill::none);
	case FillForm::Zeros: return T(rows, cols, arma::fill::zeros);
	case FillForm::Ones: return T(rows, cols, arma::fill::ones);
	case FillForm::Eye: return T(rows, cols, arma::fill::eye);
	case FillForm::Randu: return T(rows, cols, arma::fill::randu);
	case FillForm::Randn: return T(rows, cols, arma::fill::randn);
	case FillForm::Value:
		throw std::invalid_argument(
		    "Must provide value for 'value' fill form");
	default: std::unreachable();
	}
}

static inline bool
is_numeric(const mrb_value v)
{
	return mrb_float_p(v) || mrb_fixnum_p(v);
}

std::array<size_type, 1> unwrap_size_vector(
    const util::Reference<util::State> &state, mrb_value arg);
std::array<size_type, 2> unwrap_size_matrix(
    const util::Reference<util::State> &state, mrb_value arg);
std::array<size_type, 3> unwrap_size_cube(
    const util::Reference<util::State> &state, mrb_value arg);

template <size_type N>
std::array<size_type, N>
unwrap_size(const util::Reference<util::State> &state, const mrb_value arg)
{
	static_assert(N > 0, "N must be positive");
	static_assert(N <= 3, "N must be at most 3");
	if constexpr (N == 1) return unwrap_size_vector(state, arg);
	if constexpr (N == 2) return unwrap_size_matrix(state, arg);
	if constexpr (N == 3) return unwrap_size_cube(state, arg);
	return {};
}

} /* namespace euler::math */

#endif /* EULER_MATH_UTIL_H */
