/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_CUBE_H
#define EULER_MATH_CUBE_H

#include "euler/math/nonscalar.h"
#include "euler/util/object.h"

namespace euler::math {
class Cube : public util::Object {
public:
	virtual mrb_value columns(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value rows(const util::Reference<util::State> &state) = 0;
	virtual mrb_value slices(const util::Reference<util::State> &state) = 0;
	virtual mrb_value column(const util::Reference<util::State> &state) = 0;
	virtual mrb_value row(const util::Reference<util::State> &state) = 0;
	virtual mrb_value slice(const util::Reference<util::State> &state) = 0;
	virtual mrb_value column_to_matrix(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value each_slice(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value fill(const util::Reference<util::State> &state) = 0;
	virtual mrb_value imbue(const util::Reference<util::State> &state) = 0;
	virtual mrb_value insert_columns(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value insert_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value insert_slices(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value ones(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randn(const util::Reference<util::State> &state) = 0;
	virtual mrb_value randu(const util::Reference<util::State> &state) = 0;
	virtual mrb_value reshape(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value row_to_matrix(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_columns(
	    const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_rows(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value shed_slices(const util::Reference<util::State> &state)
	    = 0;
	virtual mrb_value swap(const util::Reference<util::State> &state) = 0;
	virtual mrb_value zeros(const util::Reference<util::State> &state) = 0;
	virtual mrb_value add(const util::Reference<util::State> &state) = 0;
	virtual mrb_value sub(const util::Reference<util::State> &state) = 0;
	virtual mrb_value matmul(const util::Reference<util::State> &state) = 0;
	virtual mrb_value mul(const util::Reference<util::State> &state) = 0;
	virtual mrb_value div(const util::Reference<util::State> &state) = 0;
	virtual mrb_value eq(const util::Reference<util::State> &state) = 0;
	virtual mrb_value ne(const util::Reference<util::State> &state) = 0;
	virtual mrb_value lt(const util::Reference<util::State> &state) = 0;
	virtual mrb_value le(const util::Reference<util::State> &state) = 0;
	virtual mrb_value gt(const util::Reference<util::State> &state) = 0;
	virtual mrb_value ge(const util::Reference<util::State> &state) = 0;
};
} /* namespace euler::math */

#endif /* EULER_MATH_CUBE_H */
