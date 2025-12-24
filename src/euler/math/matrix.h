/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_MATRIX_H
#define EULER_MATH_MATRIX_H

#include <armadillo>

#include "euler/math/util.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"
#include "euler/util/state.h"

namespace euler::math {
template <typename T> class Matrix final : public util::Object {
public:
	typedef T value_type;
	typedef arma::Mat<T> matrix_type;

private:
	static constexpr size_type NAME_LEN = sizeof("Matrix") + 3;
	static constexpr std::array<char, NAME_LEN> NAME_BUF = [] {
		std::array<char, NAME_LEN> buf = { };
		constexpr auto base = "Matrix";
		constexpr auto type_abbr = numeric_info<T>::abbreviation;
		size_t offset = 0;
		for (const char *p = base; *p != '\0'; ++p, ++offset)
			buf[offset] = *p;
		for (const char *p = type_abbr; *p != '\0'; ++p, ++offset)
			buf[offset] = *p;
		buf[offset] = '\0';
		return buf;
	}();

public:
	Matrix() = default;

	static constexpr auto NAME = NAME_BUF.data();
	static constexpr mrb_data_type TYPE = {
		.struct_name = NAME,
		.dfree =
		    [](mrb_state *mrb, void *ptr) {
			    auto ref = util::Reference<Matrix>::unwrap(ptr);
			    ref.decrement();
		    },
	};
	static RClass *
	get_class(const util::Reference<util::State> &state)
	{
	}

	static mrb_value
	allocate(mrb_state *mrb, mrb_value self)
	{
		printf("Matrix::allocate: %p\n", mrb);
		auto instance = util::make_reference<Matrix>();
		auto name = std::format("Euler::Math::{}", NAME);
		printf("sizeof(mrb_state) = %zu\n", sizeof(mrb_state));
		auto cls
		    = util::State::get(mrb)->mrb()->class_get(name.c_str());
		return instance.wrap(mrb, cls, &TYPE);
	}

	mrb_value
	initialize(const util::Reference<util::State> &state)
	{
		auto mrb = state->mrb();
		printf("in init\n");
		mrb_value arg1, arg2;
		mrb_sym fill_sym = mrb->intern_cstr("fill");
		mrb_value fill_value = mrb_nil_value();
		const mrb_kwargs kw_args = {
			.num = 1,
			.required = 0,
			.table = &fill_sym,
			.values = &fill_value,
			.rest = nullptr,
		};
		mrb->get_args("o|o:", &arg1, &arg2, &kw_args);
		if (is_numeric(arg1)) {
			if (!is_numeric(arg2)) {
				throw std::invalid_argument(
				    "second argument must be numeric if first "
				    "is numeric");
			}
			const auto r = unwrap_num<size_type>(state, arg1);
			const auto c = unwrap_num<size_type>(state, arg2);
			_matrix = fill_dynamic<matrix_type>(state, r, c,
			    fill_value);
			return mrb_nil_value();
		}
		return mrb_nil_value();
	}

	mrb_value
	at(const util::Reference<util::State> &state)
	{
		auto mrb = state->mrb();
		mrb_int row, col;
		mrb->get_args("ii", &row, &col);
		if (row < 0 || static_cast<size_type>(row) >= _matrix.n_rows
		    || col < 0
		    || static_cast<size_type>(col) >= _matrix.n_cols) {
			throw std::out_of_range("index out of range");
		}
		const auto value = _matrix(static_cast<size_type>(row),
		    static_cast<size_type>(col));
		return wrap_num<T>(mrb, value);
	}

	static RClass *
	init(const util::Reference<util::State> &state, RClass *mod)
	{
		printf("Initializing %s\n", NAME);
		printf("in module %s\n", state->mrb()->class_name(mod));
		const auto cls = state->mrb()->define_class_under(mod, NAME,
		    state->object_class());
		MRB_SET_INSTANCE_TT(cls, MRB_TT_DATA);
		state->mrb()->define_class_method(cls, "allocate", allocate,
		    MRB_ARGS_NONE());
		state->mrb()->define_method(cls, "initialize",
		    util::wrap_method<&Matrix::initialize>(),
		    MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1) | MRB_ARGS_KEY(1, 0));
		state->mrb()->define_method(cls, "at",
		    util::wrap_method<&Matrix::at>(), MRB_ARGS_REQ(2));
		return cls;
	}

private:
	matrix_type _matrix;
};
} /* namespace euler::math */

#endif /* EULER_MATH_MATRIX_H */
