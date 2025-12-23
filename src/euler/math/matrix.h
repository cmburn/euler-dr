/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_MATRIX_H
#define EULER_MATH_MATRIX_H

#include <cassert>
#include <format>

#include <armadillo>
#include <mruby.h>

#include <mruby/value.h>

#include "euler/math/nonscalar.h"
#include "euler/math/util.h"
#include "euler/math/vector.h"
#include "euler/util/ext.h"
#include "euler/util/state.h"

namespace euler::math {

template <typename T, size_type Rows = dynamic_size,
    size_type Cols = dynamic_size>
class Matrix final : public Nonscalar {
	typedef numeric_info<T> numeric_info;
	typedef numeric_info::mrb_type mrb_type;
	static constexpr auto type_abbreviation = numeric_info::abbreviation;
	static constexpr bool can_decompose = numeric_info::can_decompose;

public:
	static constexpr size_type row_count = Rows;
	static constexpr size_type column_count = Cols;
	static constexpr bool is_dynamic_rows = Rows == dynamic_size;
	static constexpr bool is_dynamic_cols = Cols == dynamic_size;
	static_assert(is_dynamic_cols == is_dynamic_rows,
	    "Matrix must be either fully dynamic or fully static");
	static constexpr bool is_dynamic = is_dynamic_cols && is_dynamic_rows;
	typedef T value_type;

	typedef std::conditional_t<is_dynamic, arma::Mat<value_type>,
	    typename arma::Mat<value_type>::template fixed<row_count,
		column_count>>
	    matrix_type;
	typedef std::conditional_t<is_dynamic, arma::Col<value_type>,
	    typename arma::Col<value_type>::template fixed<row_count>>
	    column_type;
	typedef std::conditional_t<is_dynamic, arma::Row<value_type>,
	    typename arma::Row<value_type>::template fixed<column_count>>
	    row_type;
	typedef util::Reference<Matrix> (*class_initializer_type)();

private:
	static std::string
	type_name()
	{
		static const std::string str = []() -> std::string {
			if constexpr (is_dynamic) {
				return std::format("Matrix{}",
				    type_abbreviation);
			}
			return std::format("Matrix{}x{}{}", dim_str<Rows>,
			    dim_str<Cols>, type_abbreviation);
		}();
		return str;
	}

	static const mrb_data_type *
	datatype()
	{
		static const auto name = type_name();
		static const mrb_data_type data_type = {
			.struct_name = name.c_str(),
			.dfree =
			    [](mrb_state *, void *ptr) {
				    auto ref
					= util::Reference<Matrix>::unwrap(ptr);
				    ref.decrement();
			    },
		};
		return &data_type;
	}

	std::enable_if_t<!is_dynamic, void>
	standard_init(const util::Reference<util::State> &state, mrb_value v)
	{
		// we're in column-major order, but ruby is row-major
		v = state->mrb()->ensure_array_type(v);
		auto cols = RARRAY_LEN(v);
		if (cols != Cols) {
			auto str = std::format("Matrix{}x{}: passed {} columns",
			    Rows, Cols, cols);
			throw std::out_of_range(std::move(str));
		}

		for (size_type c = 0; c < cols; ++c) {
			auto col_val = state->mrb()->ary_ref(v, c);
			col_val = state->mrb()->ensure_array_type(col_val);
			const auto rows = RARRAY_LEN(col_val);
			if (rows != Rows) {
				auto str = std::format(
				    "Matrix{}x{}: column {} has {} rows", Rows,
				    Cols, c, rows);
				throw std::out_of_range(std::move(str));
			}
			for (size_type r = 0; r < rows; ++r) {
				const auto cell
				    = state->mrb()->ary_ref(col_val, r);
				_matrix(r, c) = unwrap_num<T>(state, cell);
			}
		}
	}

	void
	num_init(const util::Reference<util::State> &state, const mrb_value v)
	{ _matrix.fill(unwrap_num<T>(state, v)); }

public:
	static const std::string &
	class_name()
	{
		static const std::string str = []() -> std::string {
			if constexpr (Rows == dynamic_size
			    && Cols == dynamic_size) {
				return std::format("Matrix{}",
				    type_abbreviation);
			} else {
				static_assert(Rows != dynamic_size
				    && Cols != dynamic_size);
				return std::format("Matrix{}x{}{}", Rows, Cols,
				    type_abbreviation);
			}
		}();
		return str;
	}

	[[nodiscard]] Kind
	kind() const override
	{ return nonscalar_kind_v<T>; }

	[[nodiscard]] size_type
	n_cols() const override
	{
		if constexpr (Cols == dynamic_size) return _matrix.n_cols;
		else return Cols;
	}

	[[nodiscard]] size_type
	n_rows() const override
	{
		if constexpr (Rows == dynamic_size) return _matrix.n_rows;
		else return Rows;
	}

	[[nodiscard]] size_type
	n_elem() const override
	{
		if constexpr (is_dynamic) return _matrix.n_elem;
		else return Rows * Cols;
	}

	[[nodiscard]] Type
	type() const override
	{ return Type::Matrix; }

	value_type &
	at(const size_type row, const size_type col)
	{ return _matrix(row, col); }

	const value_type &
	at(const size_type row, const size_type col) const
	{ return _matrix(row, col); }

	const value_type &
	at(const size_type index) const
	{ return _matrix(index); }

	template <typename U, size_type S>
	static void
	assert_size_vector(const util::Reference<Vector<U, S>> &size)
	{
		static_assert(!is_complex<U>(),
		    "size vector cannot be complex");
		static_assert(S == 2 || S == dynamic_size,
		    "size vector must have 2 elements");
		if constexpr (S != dynamic_size) return;
		if (size->n_elem() != 2) {
			throw std::invalid_argument(
			    "size vector must have 2 elements");
		}
	}

	void
	zeros()
	{ _matrix.zeros(); }

	void
	zeros(const size_type n_rows, const size_type n_cols)
	{ _matrix.zeros(n_rows, n_cols); }

	template <typename U, size_type S = dynamic_size>
	void
	zeros(const util::Reference<Vector<U, S>> &size)
	{
		assert_size_vector(size);
		_matrix.zeros(size->at(0), size->at(1));
	}

	void
	ones()
	{ _matrix.zeros(); }

	void
	ones(const size_type n_rows, const size_type n_cols)
	{ _matrix.ones(n_rows, n_cols); }

	template <typename U, size_type S = dynamic_size>
	void
	ones(const util::Reference<Vector<U, S>> &size)
	{
		assert_size_vector(size);
		_matrix.ones(size->at(0), size->at(1));
	}

	void
	eye()
	{ _matrix.eye(); }

	void
	eye(const size_type n_rows, const size_type n_cols)
	{ _matrix.eye(n_rows, n_cols); }

	template <typename U, size_type S = dynamic_size>
	void
	eye(const util::Reference<Vector<U, S>> &size)
	{
		static_assert(!is_complex<U>(),
		    "size vector cannot be complex");
		static_assert(S == 2 || S == dynamic_size,
		    "size vector must have 2 elements");
		if constexpr (S == dynamic_size) {
			if (size->n_elem() != 2) {
				throw std::invalid_argument(
				    "Matrix::eye: size vector must have 2 "
				    "elements");
			}
		}
		_matrix.eye(size->at(0), size->at(1));
	}

	void
	randu()
	{ _matrix.randu(); }

	void
	randu(const size_type n_rows, const size_type n_cols)
	{ _matrix.randu(n_rows, n_cols); }

	template <typename U, size_type S = dynamic_size>
	void
	randu(const util::Reference<Vector<U, S>> &size)
	{
		assert_size_vector(size);
		_matrix.randu(size->at(0), size->at(1));
	}

	void
	randn()
	{ _matrix.randn(); }

	void
	randn(const size_type n_rows, const size_type n_cols)
	{ _matrix.randn(n_rows, n_cols); }

	template <typename U, size_type S = dynamic_size>
	void
	randn(const util::Reference<Vector<U, S>> &size)
	{
		assert_size_vector(size);
		_matrix.randn(size->at(0), size->at(1));
	}

	void
	fill(const value_type v)
	{ _matrix.fill(v); }

	void
	imbue(const std::function<value_type()> &fn)
	{ _matrix.imbue(fn); }

	void
	initialize(const util::Reference<util::State> &state,
	    const mrb_value self)
	{
		auto mrb = state->mrb();
		if constexpr (is_dynamic) {
			// const auto argc = state->mrb()->get_argc();
			// if (argc > 4) {
			// 	throw std::invalid_argument(
			// 	    "Matrix::initialize: too many arguments");
			// }
			mrb_sym fill_sym = mrb->intern_cstr("fill");
			mrb_value fill_value = mrb_nil_value();
			const mrb_kwargs kwargs = {
				.num = 1,
				.required = 0,
				.table = &fill_sym,
				.values = &fill_value,
				.rest = nullptr,
			};
			mrb_value arg1, arg2;
			mrb->get_args("o|o:", &arg1, &arg2, &kwargs);
			if (is_numeric(arg1)) {
				if (!is_numeric(arg2)) {
					throw std::invalid_argument(
					    "Matrix::initialize: both size "
					    "arguments must be numeric");
				}
				const auto n_rows
				    = unwrap_num<size_type>(state, arg1);
				const auto n_cols
				    = unwrap_num<size_type>(state, arg2);
				if (mrb_nil_p(fill_value)) {
					_matrix.set_size(n_rows, n_cols);
					return;
				}
				_matrix = fill_dynamic<matrix_type>(state,
				    n_rows, n_cols, fill_value);
				return;
			}
			if ()
		} else {
		}
	}

	void
	initialize(const util::Reference<util::State> &state,
	    const mrb_value self, std::enable_if_t<!is_dynamic, int> = 1)
	{
	}

	void
	initialize(const util::Reference<util::State> &state,
	    const mrb_value self, std::enable_if_t<is_dynamic, int> = 0)
	{
	}

	// void
	// initialize(const util::Reference<util::State> &state,
	//     const mrb_value self)
	// {
	// 	// if (state->mrb()->get_argc() == 3 && is_dynamic) {
	// 	// 	mrb_sym sym;
	// 	// 	mrb_float r, c;
	// 	// 	state->mrb()->get_args("nff", &sym, &r, &c);
	// 	// }
	// 	mrb_value v;
	// 	state->mrb()->get_args("o", &v);
	// 	if (mrb_array_p(v)) {
	// 		standard_init(state, v);
	// 		return;
	// 	}
	// 	if (mrb_symbol_p(v) && !is_dynamic) {
	// 		symbol_init(state, v);
	// 		return;
	// 	}
	// 	/* has to be a scalar int or float */
	// 	if (mrb_float_p(v) || mrb_fixnum_p(v)) {
	// 		num_init(state, v);
	// 		return;
	// 	}
	//
	// 	throw std::invalid_argument(std::format(
	// 	    "Matrix{}x{}: invalid initialization value", Rows, Cols));
	// }

	explicit Matrix(const util::Reference<util::State> &state,
	    const mrb_value v)
	{ initialize(state, v); }

	explicit Matrix(matrix_type &&matrix)
	    : _matrix(std::move(matrix))
	{
	}

	explicit Matrix(const matrix_type &matrix)
	    : _matrix(matrix)
	{
	}

	explicit Matrix()
	    : _matrix()
	{
	}

	static RClass *
	fetch_class(mrb_state *mrb)
	{ return nullptr; }

	[[nodiscard]] bool
	is_fixed_size() const override
	{ return (Rows != dynamic_size) && (Cols != dynamic_size); }

	[[nodiscard]] mrb_value
	to_array(const util::Reference<util::State> &state) const override
	{
		const auto arr = state->mrb()->ary_new_capa(n_rows());
		for (size_type c = 0; c < n_rows(); ++c) {
			const auto col_arr
			    = state->mrb()->ary_new_capa(n_rows());
			for (size_type r = 0; r < n_rows(); ++r) {
				const auto val = state->mrb()->float_value(
				    static_cast<double>(_matrix(r, c)));
				state->mrb()->ary_push(col_arr, val);
			}
			state->mrb()->ary_push(arr, col_arr);
		}
		for (size_type r = 0; r < n_rows(); ++r) {
			const auto row_arr
			    = state->mrb()->ary_new_capa(n_cols());
			for (size_type c = 0; c < n_cols(); ++c) {
				const auto val = state->mrb()->float_value(
				    static_cast<double>(_matrix(r, c)));
				state->mrb()->ary_push(row_arr, val);
			}
			state->mrb()->ary_push(arr, row_arr);
		}
		return arr;
	}

	[[nodiscard]] mrb_value
	at(const util::Reference<util::State> &state) const
	{
		if (state->mrb()->get_argc() == 1) {
			mrb_value index;
			state->mrb()->get_args("o", &index);
			const auto idx = unwrap_num<size_type>(state, index);
			return wrap_numeric(state, _matrix(idx));
		}
		mrb_value x, y;
		state->mrb()->get_args("oo", &y, &x);
		const auto row = unwrap_num<size_type>(state, y);
		const auto col = unwrap_num<size_type>(state, x);
		return wrap_numeric(state, _matrix(row, col));
	}

	[[nodiscard]] mrb_value
	set_at(const util::Reference<util::State> &state)
	{
		if (state->mrb()->get_argc() == 2) {
			mrb_value index, value;
			state->mrb()->get_args("oo", &index, &value);
			const auto idx = unwrap_num<size_type>(state, index);
			_matrix(idx) = unwrap_num<value_type>(state, value);
			return mrb_nil_value();
		}
		mrb_value x, y, value;
		state->mrb()->get_args("ooo", &y, &x, &value);
		const auto row = unwrap_num<size_type>(state, y);
		const auto col = unwrap_num<size_type>(state, x);
		_matrix(row, col) = unwrap_num<value_type>(state, value);
		return mrb_nil_value();
	}

	[[nodiscard]] mrb_value
	to_mrb(const util::Reference<util::State> &state) const override
	{ return mrb_nil_value(); }

	[[nodiscard]] static util::Reference<Matrix>
	from_mrb(const util::Reference<util::State> &state, const mrb_value v)
	{ return mrb_nil_value(); }

	static mrb_value
	allocate(mrb_state *mrb, mrb_value)
	{
		auto ref = util::make_reference<Matrix>();
		return ref.wrap(mrb, datatype());
	}

	static mrb_value
	initialize(mrb_state *mrb, mrb_value self)
	{
		auto state = util::State::get(mrb);
		auto ref = util::Reference<Matrix>::unwrap(self);
		ref->initialize(state, self);
		return mrb_nil_value();
	}

	static void
	mruby_init(const util::Reference<util::State> &state, RClass *mod,
	    RClass *super)
	{
		static const auto class_name = Matrix::class_name();
		const auto cls = state->mrb()->define_class_under(mod,
		    class_name.c_str(), super);
		MRB_SET_INSTANCE_TT(cls, MRB_TT_DATA);
		// state->mrb()->define_method(cls, "to_a",
		//     util::wrap_method<&Matrix::to_array>(), MRB_ARGS_NONE());
		util::define_method<&Matrix::to_array>(state, cls, "to_a",
		    MRB_ARGS_NONE());
		state->mrb()->define_class_method(cls, "allocate", allocate,
		    MRB_ARGS_NONE());
		state->mrb()->define_method(cls, "initialize", initialize,
		    MRB_ARGS_REQ(1));

		printf("class: %s\n", class_name.c_str());
	}

private:
	matrix_type _matrix;
};

} /* namespace euler::math */

#endif /* EULER_MATH_MATRIX_H */
