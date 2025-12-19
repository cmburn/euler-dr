/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_EXT_H
#define EULER_MATH_EXT_H

#include <armadillo>

#include "euler/math/util.h"
#include "euler/util/object.h"

namespace euler::math {

template <typename T, size_type N = dynamic_size>
mrb_value
read_column_vector(mrb_state *mrb, mrb_value value)
{

}


template <typename T, size_type N = dynamic_size>
mrb_value
read_vector(mrb_state *mrb, mrb_value value)
{
	return read_column_vector<T, N>(mrb, value);
}

template <typename T, size_type N = dynamic_size>
mrb_value
read_row_vector(mrb_state *mrb, mrb_value value)
{
}

template <typename T, size_type Rows = dynamic_size,
    size_type Cols = dynamic_size>
mrb_value
read_matrix(mrb_state *mrb, mrb_value value)
{
}

template <typename T, size_type Rows = dynamic_size,
    size_type Cols = dynamic_size, size_type Slices = dynamic_size>
mrb_value
read_cube(mrb_state *mrb, mrb_value value)
{
}

} /* namespace euler::math */

#endif /* EULER_MATH_EXT_H */
