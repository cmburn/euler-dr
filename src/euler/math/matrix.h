/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_MATRIX_H
#define EULER_MATH_MATRIX_H

#include <armadillo>

#include "euler/math/util.h"
#include "euler/util/ext.h"
#include "euler/util/object.h"
#include "euler/util/state.h"
#include "euler/math/nonscalar.h"

namespace euler::math {

class Matrix : public Nonscalar {
public:
	~Matrix() override = default;
	static constexpr mrb_data_type TYPE = {
		.struct_name = "Matrix",
		.dfree =
		    [](mrb_state *, void *ptr) {
			    const auto ref
				= util::Reference<Matrix>::unwrap(ptr);
			    ref.decrement();
		    },
	};
	virtual mrb_value columns(const util::Reference<util::State> &state) = 0;
	virtual mrb_value rows(const util::Reference<util::State> &state) = 0;
	virtual mrb_value column(const util::Reference<util::State> &state) = 0;
	virtual mrb_value row(const util::Reference<util::State> &state) = 0;
	virtual mrb_value each_column(const util::Reference<util::State> &state) = 0;
	virtual mrb_value each_row(const util::Reference<util::State> &state) = 0;
	virtual mrb_value eye(const util::Reference<util::State> &state) = 0;
	virtual mrb_value fill(const util::Reference<util::State> &state) = 0;
	virtual mrb_value imbue(const util::Reference<util::State> &state) = 0;
	virtual mrb_value insert_columns(const util::Reference<util::State> &state) = 0;
	virtual mrb_value insert_rows(const util::Reference<util::State> &state) = 0;
};

template <typename T> class MatrixImpl final : public Matrix {
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
	MatrixImpl() = default;

	static constexpr auto NAME = NAME_BUF.data();
	static constexpr mrb_data_type TYPE = {
		.struct_name = NAME,
		.dfree =
		    [](mrb_state *mrb, void *ptr) {
			    auto ref = util::Reference<MatrixImpl>::unwrap(ptr);
			    ref.decrement();
		    },
	};
	// static RClass *
	// get_class(const util::Reference<util::State> &state)
	// {
	// }

	// static RClass *
	// fetch_class(const util::Reference<util::State> &state)
	// { return state->modules().math.matrix<T>(); }
	//
	// static RClass *
	// fetch_class(mrb_state *mrb)
	// { return fetch_class(util::State::get(mrb)); }

	// static mrb_value
	// allocate(mrb_state *mrb, mrb_value self)
	// {
	// 	printf("%s::allocate: %p\n", NAME, mrb);
	// 	printf("sizeof(mrb_state) = %zu\n", sizeof(mrb_state));
	// 	const void *ud_addr = &mrb->ud;
	// 	printf("&mrb->ud = %p\n", ud_addr);
	// 	printf("mrb - &mrb->ud = %p\n",
	// 	    (void *)((uintptr_t)mrb - (uintptr_t)ud_addr));
	// 	auto instance = util::make_reference<MatrixImpl>();
	// 	auto name = std::format("Euler::Math::{}", NAME);
	// 	printf("sizeof(mrb_state) = %zu\n", sizeof(mrb_state));
	// 	auto cls = fetch_class(util::State::get(mrb));
	// 	return instance.wrap(mrb, cls, &TYPE);
	// }

	mrb_value
	initialize(const util::Reference<util::State> &state)
	{
		auto mrb = state->mrb();
		printf("in init\n");
		mrb_value arg1, arg2;
		mrb_sym fill_sym = mrb->intern_cstr("fill");
		mrb_value fill_value = mrb_nil_value();
		printf("value nil? %s\n", mrb_nil_p(fill_value) ? "yes" : "no");
		const mrb_kwargs kw_args = {
			.num = 1,
			.required = 0,
			.table = &fill_sym,
			.values = &fill_value,
			.rest = nullptr,
		};
		mrb->get_args("o|o:", &arg1, &arg2, &kw_args);
		if (mrb_undef_p(fill_value)) fill_value = mrb_nil_value();
		printf("value nil? %s\n", mrb_nil_p(fill_value) ? "yes" : "no");
		printf("%zu\n", fill_value.w);
		printf("Matrix::initialize: got args\n");
		if (is_numeric(arg1)) {
			if (!is_numeric(arg2)) {
				throw std::invalid_argument(
				    "second argument must be numeric if first "
				    "is numeric");
			}
			const auto r = unwrap_num<size_type>(state, arg1);
			const auto c = unwrap_num<size_type>(state, arg2);
			printf("Matrix::initialize: creating %zux%zu matrix\n",
			    r, c);
			_matrix = fill_dynamic<matrix_type>(state, r, c,
			    fill_value);
			return mrb_nil_value();
		}
		printf("Matrix::initialize: done\n");
		return mrb_nil_value();
	}

	mrb_int
	coerce_row(mrb_int r) const
	{
		if (r < 0) r += static_cast<mrb_int>(_matrix.n_rows);
		return r;
	}

	mrb_int
	coerce_col(mrb_int c) const
	{
		if (c < 0) c += static_cast<mrb_int>(_matrix.n_cols);
		return c;
	}

	mrb_value
	at(const util::Reference<util::State> &state)
	{
		auto mrb = state->mrb();
		const auto argc = mrb->get_argc();
		switch (argc) {
		case 2: {
			mrb_int row = 0, col = 0;
			mrb->get_args("ii", &row, &col);
			const auto value = _matrix(static_cast<size_type>(row),
			    static_cast<size_type>(col));
			return wrap_num<T>(mrb, value);
		}
		case 1: {
			mrb_int elem = 0;
			mrb->get_args("i", &elem);
			const auto value
			    = _matrix(static_cast<size_type>(elem));
			return wrap_num<T>(mrb, value);
		}
		default:
			throw std::invalid_argument(
			    "wrong number of arguments");
		}
	}

	// static RClass *
	// init(const util::Reference<util::State> &state, RClass *mod)
	// {
	// 	printf("Initializing %s\n", NAME);
	// 	printf("in module %s\n", state->mrb()->class_name(mod));
	// 	auto mrb = state->mrb()->mrb();
	// 	const void *ud_addr = &mrb->ud;
	// 	printf("sizeof(mrb_state) = %zu\n", sizeof(mrb_state));
	// 	printf("&mrb->ud = %p\n", ud_addr);
	// 	printf("mrb - &mrb->ud = %p\n",
	// 	    (void *)((uintptr_t)mrb - (uintptr_t)ud_addr));
	// 	const auto cls = state->mrb()->define_class_under(mod, NAME,
	// 	    state->object_class());
	// 	MRB_SET_INSTANCE_TT(cls, MRB_TT_DATA);
	// 	state->mrb()->define_class_method(cls, "allocate", allocate,
	// 	    MRB_ARGS_NONE());
	// 	state->mrb()->define_method(cls, "initialize",
	// 	    util::wrap_method<&MatrixImpl::initialize>(),
	// 	    MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1) | MRB_ARGS_KEY(1, 0));
	// 	state->mrb()->define_method(cls, "[]",
	// 	    util::wrap_method<&MatrixImpl::at>(),
	// 	    MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	// 	return cls;
	// }

private:
	matrix_type _matrix;
};
} /* namespace euler::math */

#endif /* EULER_MATH_MATRIX_H */
