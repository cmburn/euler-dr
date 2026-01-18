/* SPDX-License-Identifier: ISC */

#include "euler/math/row_vector.h"

#include "euler/math/impl.h"

using namespace euler::math;

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

