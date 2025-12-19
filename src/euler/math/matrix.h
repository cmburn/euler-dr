/* SPDX-License-Identifier: ISC */

#ifndef EULER_MATH_MATRIX_H
#define EULER_MATH_MATRIX_H

#include <cassert>
#include <format>

#include <armadillo>
#include <mruby.h>

#include "euler/math/nonscalar.h"
#include "euler/math/util.h"
#include "euler/util/state.h"
#include "vector.h"

namespace euler::math {

template <typename T, size_type Rows = dynamic_size,
    size_type Cols = dynamic_size>
class Matrix final : public Nonscalar {
	typedef numeric_info<T> numeric_info;
	typedef numeric_info::mrb_type mrb_type;
	static constexpr auto type_abbreviation = numeric_info::abbreviation;
	static constexpr bool can_decompose = numeric_info::can_decompose;

	static std::string
	type_name()
	{
		if constexpr (Rows == dynamic_size && Cols == dynamic_size)
			return std::format("Matrix{}", type_abbreviation);
		return std::format("Matrix{}x{}{}", Rows, Cols,
		    type_abbreviation);
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

	void
	standard_init(const util::Reference<util::MRubyState> &mrb, mrb_value v)
	{
		// we're in column-major order, but ruby is row-major
		v = mrb->ensure_array_type(v);
		auto rows = RARRAY_LEN(v);
		if (rows != Rows) {
			auto str = std::format("Matrix{}x{}: passed {} rows",
			    Rows, Cols, rows);
			throw std::out_of_range(std::move(str));
		}

		for (size_type r = 0; r < rows; ++r) {
			auto row_val = mrb->ary_ref(v, r);
			row_val = mrb->ensure_array_type(row_val);
			const auto cols = RARRAY_LEN(row_val);
			if (cols != Cols) {
				auto str = std::format(
				    "Matrix{}x{}: row {} has {} columns", Rows,
				    Cols, r, cols);
				throw std::out_of_range(std::move(str));
			}
			for (size_type c = 0; c < cols; ++c) {
				const auto cell = mrb->ary_ref(row_val, c);
				_matrix(r, c) = unwrap_numeric<T>(mrb, cell);
			}
		}
	}

	void
	symbol_init(const util::Reference<util::MRubyState> &mrb, mrb_value v)
	{
		auto sym = mrb_symbol(v);
		auto str = mrb->sym_name(sym);
		if (strcmp(str, "zeros") == 0) {
			_matrix = matrix_type(arma::fill::zeros);
			return;
		}
		if (strcmp(str, "ones") == 0) {
			_matrix = matrix_type(arma::fill::ones);
			return;
		}
		if (strcmp(str, "eye") == 0) {
			_matrix = matrix_type(arma::fill::eye);
			return;
		}
		if (strcmp(str, "randu") == 0) {
			_matrix = matrix_type(arma::fill::randu);
			return;
		}
		if (strcmp(str, "randn") == 0) {
			_matrix = matrix_type(arma::fill::randn);
			return;
		}
		if (strcmp(str, "none") == 0) {
			_matrix = matrix_type(arma::fill::none);
			return;
		}
		auto msg = std::format(
		    "Matrix{}x{}: unrecognized initialization symbol :{}", Rows,
		    Cols, mrb->sym_name(sym));
		throw std::invalid_argument(std::move(msg));
	}

	void
	float_init(const util::Reference<util::MRubyState> &mrb,
	    const mrb_value v)
	{
		auto f = mrb->to_flo(v);
		auto val = static_cast<T>(f);
		_matrix.fill(val);
	}

	void
	int_init(const util::Reference<util::MRubyState> &mrb,
	    const mrb_value v)
	{
		auto i = mrb->to_int(v);
		auto val = static_cast<T>(i);
		_matrix.fill(val);
	}

public:
	static constexpr size_type row_count = Rows;
	static constexpr size_type column_count = Cols;
	static constexpr bool is_dynamic = Rows == dynamic_size;
	typedef T value_type;

	typedef arma::Mat<value_type>::template fixed<row_count, column_count>
	    matrix_type;
	typedef arma::Col<value_type>::template fixed<column_count> column_type;
	typedef arma::Row<value_type>::template fixed<row_count> row_type;
	typedef util::Reference<Matrix> (*class_initializer_type)();

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
	{
		return nonscalar_kind_value<T>;
	}

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
	{
		return Type::Matrix;
	}

	value_type &
	at(const size_type row, const size_type col)
	{
		return _matrix(row, col);
	}

	const value_type &
	at(const size_type row, const size_type col) const
	{
		return _matrix(row, col);
	}

	const value_type &
	at(const size_type index) const
	{
		return _matrix(index);
	}

	void
	zeros()
	{
		_matrix.zeros();
	}

	void
	zeros(const size_type n_rows, const size_type n_cols,
	    std::enable_if_t<is_dynamic, int> = 0)
	{
		_matrix.zeros(n_rows, n_cols);
	}

	template <typename U>
	void
	zeros(const util::Reference<Vector<U, dynamic_size>> &size)
	{
		static_assert(!detail::is_complex<U>(),
		    "size vector cannot be complex");
	}

	explicit Matrix(const util::Reference<util::MRubyState> &mrb,
	    const mrb_value v)
	{
		if (mrb_array_p(v)) {
			standard_init(mrb, v);
			return;
		}
		if (mrb_symbol_p(v)) {
			symbol_init(mrb, v);
			return;
		}
		/* has to be a scalar int or float */
		if (mrb_float_p(v)) {
			float_init(mrb, v);
			return;
		}
		if (mrb_integer_p(v)) {
			int_init(mrb, v);
			return;
		}
		throw std::invalid_argument(std::format(
		    "Matrix{}x{}: invalid initialization value", Rows, Cols));
	}

	explicit Matrix(matrix_type &&matrix)
	    : _matrix(std::move(matrix))
	{
	}

	explicit Matrix(const matrix_type &matrix)
	    : _matrix(matrix)
	{
	}

	explicit Matrix()
	    : _matrix(Rows, Cols, arma::fill::zeros)
	{
	}

	static util::Reference<Matrix>
	zeros()
	{
		return util::make_reference<Matrix>();
	}

	static RClass *
	fetch_class(mrb_state *mrb)
	{
		return nullptr;
	}

	template <class_initializer_type Fn>
	static mrb_value
	wrap_initializer(mrb_state *mrb, const mrb_value self)
	{
		const auto state = util::MRubyState::unwrap(mrb);
		auto mat = Fn();
		auto ptr = mat.wrap();
		auto cls = fetch_class(mrb);
		return state->data_object_alloc(cls, ptr, datatype());
	}

	static util::Reference<Matrix>
	ones()
	{
		auto mat = util::make_reference<Matrix>();
		mat->_matrix.fill(arma::fill::ones);
		return mat;
	}

	static util::Reference<Matrix>
	eye()
	{
		auto mat = util::make_reference<Matrix>();
		mat->_matrix.fill(arma::fill::eye);
		return mat;
	}

	static util::Reference<Matrix>
	randu()
	{
		auto mat = util::make_reference<Matrix>();
		mat->_matrix.fill(arma::fill::randu);
		return mat;
	}

	static util::Reference<Matrix>
	randn()
	{
		auto mat = util::make_reference<Matrix>();
		mat->_matrix.fill(arma::fill::randn);
		return mat;
	}

	static util::Reference<Matrix>
	fill(T value)
	{
		auto mat = util::make_reference<Matrix>();
		mat->_matrix.fill(value);
		return mat;
	}

	static mrb_value
	class_fill(mrb_state *mrb, const mrb_value self_value)
	{
	}

	static util::Reference<Matrix>
	imbue(const std::function<T()> &fn)
	{
		auto mat = util::make_reference<Matrix>();
		mat->_matrix.imbue(fn);
		return mat;
	}

	[[nodiscard]] bool
	is_fixed_size() const override
	{
		return (Rows != dynamic_size) && (Cols != dynamic_size);
	}

	[[nodiscard]] mrb_value
	to_array(const util::Reference<util::MRubyState> &mrb) const override
	{
		const auto arr = mrb->ary_new_capa(n_rows());
		for (size_type r = 0; r < n_rows(); ++r) {
			const auto row_arr = mrb->ary_new_capa(n_cols());
			for (size_type c = 0; c < n_cols(); ++c) {
				const auto val = mrb->float_value(
				    static_cast<double>(_matrix(r, c)));
				mrb->ary_push(row_arr, val);
			}
			mrb->ary_push(arr, row_arr);
		}
		return arr;
	}

	[[nodiscard]] mrb_value
	to_mrb(const util::Reference<util::MRubyState> &mrb) const override
	{
	}

	static void
	mruby_init(const util::Reference<util::MRubyState> &mrb, RClass *mod,
	    RClass *super)
	{
		static const auto class_name = Matrix::class_name();
		const auto cls
		    = mrb->define_class_under(mod, class_name.c_str(), super);
		mrb->define_class_method(cls, "zeros", wrap_initializer<zeros>,
		    MRB_ARGS_NONE());
		mrb->define_class_method(cls, "ones", wrap_initializer<ones>,
		    MRB_ARGS_NONE());
		mrb->define_class_method(cls, "eye", wrap_initializer<eye>,
		    MRB_ARGS_NONE());
		mrb->define_class_method(cls, "randu", wrap_initializer<randu>,
		    MRB_ARGS_NONE());
		mrb->define_class_method(cls, "randn", wrap_initializer<randn>,
		    MRB_ARGS_NONE());
		mrb->define_class_method(cls, "fill", class_fill,
		    MRB_ARGS_REQ(1));
	}

private:
	matrix_type _matrix;
};

} /* namespace euler::math */

#endif /* EULER_MATH_MATRIX_H */
