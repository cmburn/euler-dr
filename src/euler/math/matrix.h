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
		return nonscalar_kind_v<T>;
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
	{
		_matrix.zeros();
	}

	void
	zeros(const size_type n_rows, const size_type n_cols)
	{
		_matrix.zeros(n_rows, n_cols);
	}

	template <typename U, size_type S = dynamic_size>
	void
	zeros(const util::Reference<Vector<U, S>> &size)
	{
		assert_size_vector(size);
		_matrix.zeros(size->at(0), size->at(1));
	}

	void
	ones()
	{
		_matrix.zeros();
	}

	void
	ones(const size_type n_rows, const size_type n_cols)
	{
		_matrix.ones(n_rows, n_cols);
	}

	template <typename U, size_type S = dynamic_size>
	void
	ones(const util::Reference<Vector<U, S>> &size)
	{
		assert_size_vector(size);
		_matrix.ones(size->at(0), size->at(1));
	}

	void
	eye()
	{
		_matrix.eye();
	}

	void
	eye(const size_type n_rows, const size_type n_cols)
	{
		_matrix.eye(n_rows, n_cols);
	}

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
	{
		_matrix.randu();
	}

	void
	randu(const size_type n_rows, const size_type n_cols)
	{
		_matrix.randu(n_rows, n_cols);
	}

	template <typename U, size_type S = dynamic_size>
	void
	randu(const util::Reference<Vector<U, S>> &size)
	{
		assert_size_vector(size);
		_matrix.randu(size->at(0), size->at(1));
	}

	void
	randn()
	{
		_matrix.randn();
	}

	void
	randn(const size_type n_rows, const size_type n_cols)
	{
		_matrix.randn(n_rows, n_cols);
	}

	template <typename U, size_type S = dynamic_size>
	void
	randn(const util::Reference<Vector<U, S>> &size)
	{
		assert_size_vector(size);
		_matrix.randn(size->at(0), size->at(1));
	}

	void
	fill(const value_type v)
	{
		_matrix.fill(v);
	}

	void
	imbue(const std::function<value_type()> &fn)
	{
		_matrix.imbue(fn);
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

	static RClass *
	fetch_class(mrb_state *mrb)
	{
		return nullptr;
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

	[[nodiscard]] static util::Reference<Matrix>
	from_mrb(const util::Reference<util::MRubyState> &mrb,
	    const mrb_value v)
	{
	}

	static void
	mruby_init(const util::Reference<util::MRubyState> &mrb, RClass *mod,
	    RClass *super)
	{
		static const auto class_name = Matrix::class_name();
		const auto cls
		    = mrb->define_class_under(mod, class_name.c_str(), super);
		MRB_SET_INSTANCE_TT(cls, MRB_TT_DATA);
	}

private:
	matrix_type _matrix;
};

} /* namespace euler::math */

#endif /* EULER_MATH_MATRIX_H */
