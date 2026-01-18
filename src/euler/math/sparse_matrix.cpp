/* SPDX-License-Identifier: ISC */

#include "euler/math/sparse_matrix.h"

using namespace euler::math;

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
