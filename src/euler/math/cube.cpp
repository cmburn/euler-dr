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
template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::at(const util::Reference<util::State> &state)
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
		return wrap_num<T, Rows, Cols, Slices>(state, value);
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
template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::set_at(const util::Reference<util::State> &state)
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
		auto value = unwrap_num<T, Rows, Cols, Slices>(state, value_val);
		_cube(r, c, s) = value;
		return mrb_nil_value();
	}
	case 2: {
		mrb_value index_val, value_val;
		mrb->get_args("oo", &index_val, &value_val);
		auto index = unwrap_num<size_type>(state, index_val);
		auto value = unwrap_num<T, Rows, Cols, Slices>(state, value_val);
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
template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::clamp(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value min_val, max_val;
	mrb->get_args("oo", &min_val, &max_val);
	auto min = unwrap_num<T, Rows, Cols, Slices>(state, min_val);
	auto max = unwrap_num<T, Rows, Cols, Slices>(state, max_val);
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
template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::clean(const util::Reference<util::State> &state)
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
template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::copy_size(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value other_val;
	mrb->get_args("o", &other_val);
	auto other = state->unwrap<Cube>(other_val);
	return mrb_nil_value();
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::map(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::has_infinity(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::has_nan(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::imaginary(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::in_range(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::index_max(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::index_min(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::is_complex(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::is_empty(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::is_finite(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::is_zero(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::max(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::min(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::real(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::replace(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::reset(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::resize(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::set_imaginary(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::set_real(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::set_size(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::size(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::to_string(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::transform(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::columns(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::rows(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::slices(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::column(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::row(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::slice(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::column_to_matrix(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::each_slice(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::fill(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::imbue(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::insert_columns(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::insert_rows(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::insert_slices(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::ones(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::randn(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::randu(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::reshape(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::row_to_matrix(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::shed_columns(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::shed_rows(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::shed_slices(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::swap(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows, size_type Cols, size_type Slices>
mrb_value
CubeImpl<T, Rows, Cols, Slices>::zeros(const util::Reference<util::State> &state)
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
