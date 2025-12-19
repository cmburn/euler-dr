/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_CUBE_H
#define EULER_MATH_CUBE_H

#include <armadillo>

#include "euler/math/nonscalar.h"
#include "euler/util/object.h"

namespace euler::math {
template <typename T, size_type Rows, size_type Cols, size_type Slices>
class Cube final : public Nonscalar {
public:
	static constexpr size_type row_count = Rows;
	static constexpr size_type column_count = Cols;
	static constexpr bool is_dynamic = (Rows == dynamic_size)
	    || (Cols == dynamic_size)
	    || (Slices == dynamic_size);
	typedef arma::Cube<T>::template fixed<row_count, column_count, Slices>
	    cube_type;

	size_type
	n_slices() const
	{
		if constexpr (Slices == dynamic_size) return _cube.n_slices;
		else return Slices;
	}



private:
	cube_type _cube;
};
} /* namespace euler::math */

#endif /* EULER_MATH_CUBE_H */
