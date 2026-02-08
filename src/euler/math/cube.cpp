/* SPDX-License-Identifier: ISC */

#include "euler/math/cube.h"

#include "euler/math/impl.h"

using namespace euler::math;

static mrb_value
cube_at(mrb_state *mrb, mrb_value self)
{

	auto state = euler::util::State::get(mrb);
	auto cube = state->unwrap<Cube>(self);
	return cube->at(state);
}

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
	const auto mrb = state->mrb();
	switch (const auto argc = mrb->get_argc()) {
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

RClass *
Cube::init(const util::Reference<util::State> &state, RClass *mod,
    RClass *super)
{
	auto mrb = state->mrb();
	auto cls = mrb->define_class_under(mod, "Cube", super);
	mrb->define_method(cls, "[]", WRAP_MATH_METHOD(Cube, at),
	    MRB_ARGS_ANY());
	mrb->define_method(cls, "[]=", WRAP_MATH_METHOD(Cube, set_at),
	    MRB_ARGS_ANY());
}
