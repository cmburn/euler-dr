/* SPDX-License-Identifier: ISC */

#include "euler/math/matrix.h"

#include "euler/math/impl.h"

using namespace euler::math;

Nonscalar::Type
Matrix::type() const
{
	return Type::Matrix;
}

template <typename T>
mrb_value
MatrixImpl<T>::wrap_matrix(const util::Reference<util::State> &state,
    const matrix_type &matrix)
{
	auto mrb = state->mrb();
	// TODO
	throw std::runtime_error("Not implemented");
}

template <typename T> MatrixImpl<T>::MatrixImpl() = default;

template <typename T>
mrb_value
MatrixImpl<T>::initialize(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	return mrb_nil_value();
}

template <typename T>
mrb_int
MatrixImpl<T>::coerce_row(mrb_int r) const
{
	if (r < 0) r += static_cast<mrb_int>(_matrix.n_rows);
	return r;
}

template <typename T>
mrb_int
MatrixImpl<T>::coerce_col(mrb_int c) const
{
	if (c < 0) c += static_cast<mrb_int>(_matrix.n_cols);
	return c;
}

template <typename T>
mrb_int
MatrixImpl<T>::coerce_row(const util::Reference<util::State> &state,
    mrb_value r) const
{
	const auto row = unwrap_num<mrb_int>(state, r);
	return coerce_row(row);
}

template <typename T>
mrb_int
MatrixImpl<T>::coerce_col(const util::Reference<util::State> &state,
    mrb_value c) const
{
	const auto col = unwrap_num<mrb_int>(state, c);
	return coerce_col(col);
}

template <typename T>
mrb_value
MatrixImpl<T>::at(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto argc = mrb->get_argc();
	switch (argc) {
	case 2: {
		mrb_int row = 0, col = 0;
		mrb->get_args("ii", &row, &col);
		const auto value = _matrix(static_cast<size_type>(row),
		    static_cast<size_type>(col));
		return wrap_num(mrb, value);
	}
	case 1: {
		mrb_int elem = 0;
		mrb->get_args("i", &elem);
		const auto value = _matrix(static_cast<size_type>(elem));
		return wrap_num(mrb, value);
	}
	default: throw std::invalid_argument("wrong number of arguments");
	}
}

template <typename T>
mrb_value
MatrixImpl<T>::set_at(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto argc = mrb->get_argc();
	switch (argc) {
	case 3: {
		mrb_value value;
		mrb_int row = 0, col = 0;
		mrb->get_args("iio", &row, &col, &value);
		const auto v = unwrap_num<T>(state, value);
		_matrix(static_cast<size_type>(row),
		    static_cast<size_type>(col))
		    = v;
		return mrb_nil_value();
	}
	case 2: {
		mrb_value value;
		mrb_int elem = 0;
		mrb->get_args("io", &elem, &value);
		const auto v = unwrap_num<T>(state, value);
		_matrix(static_cast<size_type>(elem)) = v;
		return mrb_nil_value();
	}
	default: throw std::invalid_argument("wrong number of arguments");
	}
}

template <typename T>
mrb_value
MatrixImpl<T>::clamp(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value min_value, max_value;
	mrb->get_args("oo", &min_value, &max_value);
	const auto min = unwrap_num<T>(state, min_value);
	const auto max = unwrap_num<T>(state, max_value);
	_matrix.clamp(min, max);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::clean(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value value;
	mrb->get_args("o", &value);
	const auto tol = unwrap_num<double>(state, value);
	_matrix.clean(tol);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::copy_size(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_matrix(state, arg)) {
		throw std::invalid_argument("argument must be a Matrix");
	}
	const auto other = util::Reference<Matrix>::unwrap(mrb->mrb(), arg);
	// there's a few scenarios where we mimic Armadillo's behavior
	// here rather than call the appropriate Armadillo function
	// directly to avoid having to cast to concrete types
	_matrix.set_size(other->n_rows(), other->n_cols());
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::map(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	auto arr = mrb->ary_new_capa(_matrix.n_cols);
	for (auto c = 0; c < _matrix.n_cols; ++c) {
		auto col_arr = mrb->ary_new_capa(_matrix.n_rows);
		for (auto r = 0; r < _matrix.n_rows; ++r) {
			const auto value = _matrix(r, c);
			auto val = wrap_num(mrb, value);
			auto result = mrb->yield(blk, val);
			mrb->ary_push(col_arr, result);
		}
		mrb->ary_push(arr, col_arr);
	}
	return arr;
}

template <typename T>
mrb_value
MatrixImpl<T>::has_infinity(const util::Reference<util::State> &state)
{
	const bool result = _matrix.has_inf();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::has_nan(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const bool result = _matrix.has_nan();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::imaginary(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	pod_matrix_type imag = arma::imag(_matrix);
	return wrap_matrix(state, imag);
}

template <typename T>
mrb_value
MatrixImpl<T>::in_range(const util::Reference<util::State> &state)
{
	if constexpr (math::is_complex<T>()) {
		throw std::runtime_error(
		    "cannot check range of complex matrix");
	} else {
		auto mrb = state->mrb();
		mrb_value value;
		mrb->get_args("o", &value);
		const auto v = unwrap_num<T>(state, value);
		const bool result = _matrix.in_range(v);
		return mrb_bool_value(result);
	}
}

template <typename T>
mrb_value
MatrixImpl<T>::index_max(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto index = _matrix.index_max();
	return wrap_num(mrb, index);
}

template <typename T>
mrb_value
MatrixImpl<T>::index_min(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto index = _matrix.index_min();
	return wrap_num(mrb, index);
}

template <typename T>
mrb_value
MatrixImpl<T>::is_complex(const util::Reference<util::State> &state)
{
	static constexpr bool result = math::is_complex<T>();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::is_empty(const util::Reference<util::State> &)
{
	const bool result = _matrix.is_empty();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::is_finite(const util::Reference<util::State> &)
{
	const bool result = _matrix.is_finite();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::is_zero(const util::Reference<util::State> &)
{
	const bool result = _matrix.is_zero();
	return mrb_bool_value(result);
}

template <typename T>
mrb_value
MatrixImpl<T>::max(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto value = _matrix.max();
	return wrap_num(mrb, value);
}

template <typename T>
mrb_value
MatrixImpl<T>::min(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto value = _matrix.min();
	return wrap_num(mrb, value);
}

template <typename T>
mrb_value
MatrixImpl<T>::real(const util::Reference<util::State> &state)
{
	pod_matrix_type real = arma::real(_matrix);
	return wrap_matrix(state, real);
}

template <typename T>
mrb_value
MatrixImpl<T>::replace(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value old_value, new_value;
	mrb->get_args("oo", &old_value, &new_value);
	const auto old_v = unwrap_num<T>(state, old_value);
	const auto new_v = unwrap_num<T>(state, new_value);
	_matrix.replace(old_v, new_v);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::reset(const util::Reference<util::State> &)
{
	_matrix.reset();
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::resize(const util::Reference<util::State> &state)
{
	auto [r, c] = unwrap_size<2>(state);
	_matrix.resize(r, c);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::set_imaginary(const util::Reference<util::State> &state)
{
	if (!math::is_complex<T>()) {
		throw std::runtime_error(
		    "cannot set imaginary part of real matrix");
	}
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_matrix(state, arg)) {
		throw std::invalid_argument("argument must be a Matrix");
	}
	auto other = state->unwrap<Matrix>(arg);
	if (other->value_type() != nonscalar_value_type_v<pod_type>) {
		throw std::invalid_argument(
		    "argument must be of the correct underlying type");
	}
	auto mat = other.cast_to<MatrixImpl<T>>();
	_matrix.set_imag(mat->_matrix);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::set_real(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_matrix(state, arg)) {
		throw std::invalid_argument("argument must be a Matrix");
	}
	// auto other = util::Reference<Matrix>::unwrap(arg);
	auto other = state->unwrap<Matrix>(arg);
	if (other->value_type() != nonscalar_value_type_v<pod_type>) {
		throw std::invalid_argument(
		    "argument must be of the correct underlying type");
	}
	auto mat = other.cast_to<MatrixImpl<T>>();
	_matrix.set_real(mat->_matrix);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::set_size(const util::Reference<util::State> &state)
{
	auto [r, c] = unwrap_size<2>(state);
	_matrix.set_size(r, c);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::size(const util::Reference<util::State> &state)
{
	return make_size(state, n_rows(), n_cols());
}

template <typename T>
mrb_value
MatrixImpl<T>::to_string(const util::Reference<util::State> &state)
{
	std::stringstream ss;
	_matrix.print(ss);
	const auto mrb = state->mrb();
	return mrb->str_new_cstr(ss.str().c_str());
}

template <typename T>
mrb_value
MatrixImpl<T>::transform(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	_matrix.transform([&](T value) -> void {
		auto val = wrap_num(mrb, value);
		auto result_value = mrb->yield(blk, val);
		return unwrap_num<T>(state, result_value);
	});
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::columns(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto argc = mrb->get_argc();
	switch (argc) {
	case 0: {
		mrb_value arr = mrb->ary_new_capa(_matrix.n_cols);
		_matrix.each_col([&](auto col) {
			auto col_obj = VectorImpl<T>::wrap_vector(state, col);
			mrb->ary_push(arr, col_obj);
		});
	}
	case 1: {
	}
	case 2: {
	}
	}
}

template <typename T>
mrb_value
MatrixImpl<T>::rows(const util::Reference<util::State> &state)
{
}

template <typename T>
mrb_value
MatrixImpl<T>::column_count(const util::Reference<util::State> &state)
{
	return state->mrb()->int_value(static_cast<mrb_int>(n_cols()));
}

template <typename T>
mrb_value
MatrixImpl<T>::row_count(const util::Reference<util::State> &state)
{
	return state->mrb()->int_value(static_cast<mrb_int>(n_rows()));
}

template <typename T>
size_type
MatrixImpl<T>::n_rows() const
{
	return _matrix.n_rows;
}

template <typename T>
size_type
MatrixImpl<T>::n_cols() const
{
	return _matrix.n_cols;
}

template <typename T>
mrb_value
MatrixImpl<T>::column(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_int col = 0;
	mrb->get_args("i", &col);
	col = coerce_col(col);
	auto col_vec = _matrix.col(static_cast<size_type>(col));
	return VectorImpl<T>::wrap_vector(state, col_vec);
}

template <typename T>
mrb_value
MatrixImpl<T>::row(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_int row = 0;
	mrb->get_args("i", &row);
	row = coerce_row(row);
	auto row_vec = _matrix.row(static_cast<size_type>(row));
	return RowVectorImpl<T>::wrap_row_vector(state, row_vec);
}

template <typename T>
mrb_value
MatrixImpl<T>::each_column(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	_matrix.each_col([&](auto col) {
		auto col_obj = VectorImpl<T>::wrap_vector(state, col);
		mrb->yield(blk, col_obj);
	});
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::each_row(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	_matrix.each_row([&](auto row) {
		auto row_obj = RowVectorImpl<T>::wrap_row_vector(state, row);
		mrb->yield(blk, row_obj);
	});
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::eye(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	auto [r, c] = unwrap_size<2>(state);
	_matrix = matrix_type::eye(r, c);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::fill(const util::Reference<util::State> &state)
{
	const auto mrb = state->mrb();
	mrb_value value;
	mrb->get_args("o", &value);
	const auto v = unwrap_num<T>(state, value);
	_matrix.fill(v);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::imbue(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	_matrix.imbue([&]() -> T {
		const auto result = mrb->yield(blk, mrb_nil_value());
		return unwrap_num<T>(state, result);
	});
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::insert_columns(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb_int col = 0;
	mrb->get_args("io", &col, &arg);
	col = coerce_col(col);
	// arg can be an int or a Matrix
	if (is_matrix(state, arg)) {
		auto other = state->unwrap<Matrix>(arg);
		if (other->value_type() != nonscalar_value_type_v<T>) {
			throw std::invalid_argument(
			    "argument must be of the same underlying type");
		}
		auto mat = other.cast_to<MatrixImpl<T>>();
		_matrix.insert_cols(static_cast<size_type>(col), mat->_matrix);
	} else {
		const auto n_cols = unwrap_num<mrb_int>(state, arg);
		_matrix.insert_cols(static_cast<size_type>(col),
		    static_cast<size_type>(n_cols));
	}
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::insert_rows(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb_int row = 0;
	mrb->get_args("io", &row, &arg);
	row = coerce_row(row);
	// arg can be an int or a Matrix
	if (is_matrix(state, arg)) {
		auto other = state->unwrap<Matrix>(arg);
		if (other->value_type() != nonscalar_value_type_v<T>) {
			throw std::invalid_argument(
			    "argument must be of the same underlying type");
		}
		auto mat = other.cast_to<MatrixImpl<T>>();
		_matrix.insert_rows(static_cast<size_type>(row), mat->_matrix);
	} else {
		const auto n_rows = unwrap_num<mrb_int>(state, arg);
		_matrix.insert_rows(static_cast<size_type>(row),
		    static_cast<size_type>(n_rows));
	}
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::is_diagonal(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_matrix.is_diag());
}

template <typename T>
mrb_value
MatrixImpl<T>::is_hermitian(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_matrix.is_hermitian());
}

template <typename T>
mrb_value
MatrixImpl<T>::is_square(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_matrix.is_square());
}

template <typename T>
mrb_value
MatrixImpl<T>::is_symmetric(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_matrix.is_symmetric());
}

template <typename T>
mrb_value
MatrixImpl<T>::is_symmetric_positive_definite(
    const util::Reference<util::State> &state)
{
	return mrb_bool_value(_matrix.is_sympd());
}

template <typename T>
mrb_value
MatrixImpl<T>::is_triangular_lower(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_matrix.is_tril());
}

template <typename T>
mrb_value
MatrixImpl<T>::is_triangular_upper(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_matrix.is_triu());
}

template <typename T>
mrb_value
MatrixImpl<T>::is_vector(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_matrix.is_vec());
}

template <typename T>
mrb_value
MatrixImpl<T>::is_column_vector(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_matrix.is_colvec());
}

template <typename T>
mrb_value
MatrixImpl<T>::is_row_vector(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_matrix.is_rowvec());
}

template <typename T>
mrb_value
MatrixImpl<T>::ones(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	auto [r, c] = unwrap_size<2>(state);
	_matrix.ones(r, c);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::randn(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	auto [r, c] = unwrap_size<2>(state);
	_matrix.randn(r, c);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::randu(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	auto [r, c] = unwrap_size<2>(state);
	_matrix.randu(r, c);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::reshape(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	auto [r, c] = unwrap_size<2>(state);
	_matrix.reshape(r, c);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::shed_columns(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto argc = mrb->get_argc();
	if (argc == 1) {
		// must be a vector of columns to remove
		mrb_value arg;
		mrb->get_args("o", &arg);
		if (Nonscalar::type(state, arg) != Type::Vector) {
			throw std::invalid_argument(
			    "argument must be a Vector");
		}
		auto vec = state->unwrap<Vector>(arg);
		if (vec->is_size()) {
			throw std::invalid_argument(
			    "argument must be an index Vector");
		}
		auto vec_impl = cast_vector<uint64_t>(vec);
		arma::uvec cols(vec_impl->_vector.n_cols, arma::fill::none);
		for (auto i = 0; i < vec_impl->_vector.n_cols; ++i) {
			auto col = vec_impl->_vector(i);
			col = coerce_col(static_cast<mrb_int>(col));
			cols(i) = static_cast<size_type>(col);
		}
		_matrix.shed_cols(cols);
		return mrb_nil_value();
	}
	if (argc == 2) {
		auto col_start = 0, col_end = 0;
		mrb->get_args("ii", &col_start, &col_end);
		col_start = coerce_col(col_start);
		col_end = coerce_col(col_end);
		_matrix.shed_cols(static_cast<size_type>(col_start),
		    static_cast<size_type>(col_end));
		return mrb_nil_value();
	}
	throw std::invalid_argument("wrong number of arguments");
}

template <typename T>
mrb_value
MatrixImpl<T>::shed_column(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_int col = 0;
	mrb->get_args("i", &col);
	col = coerce_col(col);
	_matrix.shed_col(static_cast<size_type>(col));
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::shed_rows(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	if (mrb->get_argc() == 1) {
		// must be a vector of rows to remove
		mrb_value arg;
		mrb->get_args("o", &arg);
		if (Nonscalar::type(state, arg) != Type::Vector) {
			throw std::invalid_argument(
			    "argument must be a Vector");
		}
		auto vec = state->unwrap<Vector>(arg);
		if (vec->is_size()) {
			throw std::invalid_argument(
			    "argument must be an index Vector");
		}
		auto vec_impl = cast_vector<uint64_t>(vec);
		arma::uvec rows(vec_impl->_vector.n_cols, arma::fill::none);
		for (auto i = 0; i < vec_impl->_vector.n_cols; ++i) {
			auto row = vec_impl->_vector(i);
			row = coerce_row(static_cast<mrb_int>(row));
			rows(i) = static_cast<size_type>(row);
		}
		_matrix.shed_rows(rows);
		return mrb_nil_value();
	}
	if (mrb->get_argc() == 2) {
		mrb_int row_start = 0, row_end = 0;
		mrb->get_args("ii", &row_start, &row_end);
		row_start = coerce_row(row_start);
		row_end = coerce_row(row_end);
		_matrix.shed_rows(static_cast<size_type>(row_start),
		    static_cast<size_type>(row_end));
		return mrb_nil_value();
	}
	throw std::invalid_argument("wrong number of arguments");
}

template <typename T>
mrb_value
MatrixImpl<T>::shed_row(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_int row = 0;
	mrb->get_args("i", &row);
	row = coerce_row(row);
	_matrix.shed_row(static_cast<size_type>(row));
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::simple_transpose(const util::Reference<util::State> &state)
{
	matrix_type transposed = _matrix.st();
	return wrap_matrix(state, transposed);
}

template <typename T>
mrb_value
MatrixImpl<T>::swap(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value other;
	mrb->get_args("o", &other);
	if (!is_matrix(state, other)) {
		throw std::invalid_argument("argument must be a Matrix");
	}
	auto other_mat = state->unwrap<Matrix>(other);
	if (other_mat->value_type() != value_type()) {
		throw std::invalid_argument(
		    "argument must be a Matrix of the same type");
	}
	auto mat = other_mat.cast_to<MatrixImpl<T>>();
	_matrix.swap(mat->_matrix);
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::swap_columns(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_int col1 = 0, col2 = 0;
	mrb->get_args("ii", &col1, &col2);
	col1 = coerce_col(col1);
	col2 = coerce_col(col2);
	_matrix.swap_cols(static_cast<size_type>(col1),
	    static_cast<size_type>(col2));
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::swap_rows(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_int row1 = 0, row2 = 0;
	mrb->get_args("ii", &row1, &row2);
	row1 = coerce_row(row1);
	row2 = coerce_row(row2);
	_matrix.swap_rows(static_cast<size_type>(row1),
	    static_cast<size_type>(row2));
	return mrb_nil_value();
}

template <typename T>
mrb_value
MatrixImpl<T>::to_array(const util::Reference<util::State> &state)
{
	mrb_value out = state->mrb()->ary_new_capa(_matrix.n_cols);
	for (auto c = 0; c < static_cast<mrb_int>(_matrix.n_cols); ++c) {
		mrb_value col_arr = state->mrb()->ary_new_capa(_matrix.n_rows);
		for (auto r = 0; r < static_cast<mrb_int>(_matrix.n_rows); ++r) {
			const auto value = _matrix(static_cast<size_type>(r),
			    static_cast<size_type>(c));
			mrb_value val = wrap_num<T>(state->mrb(), value);
			state->mrb()->ary_push(col_arr, val);
		}
		state->mrb()->ary_push(out, col_arr);
	}
	return out;
}

template <typename T>
mrb_value
MatrixImpl<T>::to_column(const util::Reference<util::State> &state)
{
	auto col = _matrix.as_col();
	return VectorImpl<T>::wrap_vector(state, col);
}

template <typename T>
mrb_value
MatrixImpl<T>::to_row(const util::Reference<util::State> &state)
{
	auto row = _matrix.as_row();
	return RowVectorImpl<T>::wrap_row_vector(state, row);
}

template <typename T>
mrb_value
MatrixImpl<T>::transpose(const util::Reference<util::State> &state)
{
	matrix_type transposed = _matrix.t();
	return wrap_matrix(state, transposed);
}

template <typename T>
mrb_value
MatrixImpl<T>::zeros(const util::Reference<util::State> &state)
{
	switch (state->mrb()->get_argc()) {
	case 0: {
		_matrix.zeros();
		return mrb_nil_value();
	}
	case 2 : {
		auto [r, c] = unwrap_size<2>(state);
		_matrix.zeros(r, c);
		return mrb_nil_value();
	}
	default: throw std::invalid_argument("wrong number of arguments");
	}
}

static mrb_value
matrix_type(mrb_state *mrb, mrb_value self)
{
	using namespace euler;
	auto state = util::State::get(mrb);
	auto matrix = util::Reference<Matrix>::unwrap(mrb, self);
	switch (matrix->value_type()) {
	case Nonscalar::ValueType::Int16:
		return mrb_symbol_value(state->mrb()->intern_cstr("int16"));
	case Nonscalar::ValueType::Int32:
		return mrb_symbol_value(state->mrb()->intern_cstr("int32"));
	case Nonscalar::ValueType::Int64:
		return mrb_symbol_value(state->mrb()->intern_cstr("int64"));
	case Nonscalar::ValueType::UInt16:
		return mrb_symbol_value(state->mrb()->intern_cstr("uint16"));
	case Nonscalar::ValueType::UInt32:
		return mrb_symbol_value(state->mrb()->intern_cstr("uint32"));
	case Nonscalar::ValueType::UInt64:
		return mrb_symbol_value(state->mrb()->intern_cstr("uint64"));
	case Nonscalar::ValueType::Float32:
		return mrb_symbol_value(state->mrb()->intern_cstr("float32"));
	case Nonscalar::ValueType::Float64:
		return mrb_symbol_value(state->mrb()->intern_cstr("float64"));
	case Nonscalar::ValueType::Complex32:
		return mrb_symbol_value(state->mrb()->intern_cstr("complex32"));
	case Nonscalar::ValueType::Complex64:
		return mrb_symbol_value(state->mrb()->intern_cstr("complex64"));
	default: throw std::runtime_error("unknown matrix value type");
	}
}

RClass *
Matrix::init(const util::Reference<util::State> &state, RClass *mod,
    RClass *super)
{
	using T = Matrix;
	auto mrb = state->mrb();
	RClass *cls = mrb->define_class_under(mod, "Matrix", super);
	MRB_SET_INSTANCE_TT(cls, MRB_TT_DATA);
	mrb->define_method(cls, "type", matrix_type, MRB_ARGS_NONE());
	register_nonscalar_methods<Matrix>(mrb, cls);
#define DEF(NAME, ARGS) EULER_DEFINE_METHOD(NAME, ARGS)
#define DEFQ(NAME, SYM, ARGS) EULER_DEFINE_METHOD_ALT(NAME, SYM, ARGS)
	DEFQ(at, [], MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	DEFQ(set_at, [] =, MRB_ARGS_REQ(2) | MRB_ARGS_OPT(1));
	DEF(columns, MRB_ARGS_OPT(2));
	DEF(rows, MRB_ARGS_OPT(2));
	DEF(column_count, MRB_ARGS_NONE());
	DEF(row_count, MRB_ARGS_NONE());
	DEF(column, MRB_ARGS_REQ(1));
	DEF(row, MRB_ARGS_REQ(1));
	DEF(each_column, MRB_ARGS_BLOCK());
	DEF(each_row, MRB_ARGS_BLOCK());
	DEF(eye, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	DEF(fill, MRB_ARGS_REQ(1));
	DEF(imbue, MRB_ARGS_BLOCK());
	DEF(insert_columns, MRB_ARGS_REQ(2));
	DEF(insert_rows, MRB_ARGS_REQ(2));
	DEFQ(is_diagonal, diagonal?, MRB_ARGS_NONE());
	DEFQ(is_hermitian, hermitian?, MRB_ARGS_NONE());
	DEFQ(is_square, square?, MRB_ARGS_NONE());
	DEFQ(is_symmetric, symmetric?, MRB_ARGS_NONE());
	DEFQ(is_symmetric_positive_definite, symmetric_positive_definite?,
	    MRB_ARGS_NONE());
	DEFQ(is_triangular_lower, triangular_lower?, MRB_ARGS_NONE());
	DEFQ(is_triangular_upper, triangular_upper?, MRB_ARGS_NONE());
	DEFQ(is_vector, vector?, MRB_ARGS_NONE());
	DEFQ(is_column_vector, column_vector?, MRB_ARGS_NONE());
	DEFQ(is_row_vector, row_vector?, MRB_ARGS_NONE());
	DEF(ones, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	DEF(randn, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	DEF(randu, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	DEF(reshape, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	DEF(shed_columns, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	DEF(shed_column, MRB_ARGS_REQ(1));
	DEF(shed_rows, MRB_ARGS_REQ(1) | MRB_ARGS_OPT(1));
	DEF(simple_transpose, MRB_ARGS_NONE());
	DEF(swap, MRB_ARGS_REQ(2) | MRB_ARGS_OPT(1));
	DEF(swap_columns, MRB_ARGS_REQ(2));
	DEF(swap_rows, MRB_ARGS_REQ(2));
	DEF(to_array, MRB_ARGS_NONE());
	DEF(to_column, MRB_ARGS_NONE());
	DEF(to_row, MRB_ARGS_NONE());
	DEF(transpose, MRB_ARGS_NONE());
 	DEF(zeros, MRB_ARGS_OPT(2));
#undef DEFQ
#undef DEF
	return cls;
}
