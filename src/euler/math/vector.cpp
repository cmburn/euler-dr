/* SPDX-License-Identifier: ISC */

#include "euler/math/vector.h"

#include "euler/math/impl.h"

using namespace euler::math;

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::at(const util::Reference<util::State> &state)
{
	mrb_int index = 0;
	state->mrb()->get_args("i", &index);
	if (index < 0) index += static_cast<mrb_int>(_vector.n_elem);
	const auto value = _vector(static_cast<size_type>(index));
	return wrap_num(state->mrb(), value);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::set_at(const util::Reference<util::State> &state)
{
	mrb_int index = 0;
	mrb_value value;
	state->mrb()->get_args("io", &index, &value);
	if (index < 0) index += static_cast<mrb_int>(_vector.n_elem);
	const auto v = unwrap_num<T>(state, value);
	_vector(static_cast<size_type>(index)) = v;
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::clamp(const util::Reference<util::State> &state)
{
	mrb_value min_value, max_value;
	state->mrb()->get_args("oo", &min_value, &max_value);
	const auto min_v = unwrap_num<T>(state, min_value);
	const auto max_v = unwrap_num<T>(state, max_value);
	_vector = arma::clamp(_vector, min_v, max_v);
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::clean(const util::Reference<util::State> &state)
{
	mrb_value value;
	state->mrb()->get_args("o", &value);
	const auto tol = unwrap_num<pod_type>(state, value);
	_vector.clean(tol);
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::copy_size(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_vector(state, arg)) {
		throw std::invalid_argument("argument must be a Vector");
	}
	auto other = state->unwrap<Vector>(arg);
	_vector.set_size(other->n_cols());
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::map(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	auto arr = mrb->ary_new_capa(n_cols());
	_vector.for_each([&](T &value) {
		auto val = wrap_num(mrb, value);
		auto result = mrb->yield(blk, val);
		mrb->ary_push(arr, result);
	});
	return arr;
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::has_infinity(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_vector.has_inf());
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::has_nan(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_vector.has_nan());
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::imaginary(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	pod_vector_type imag = arma::imag(_vector);
	return wrap_vector<pod_type>(state, imag);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::in_range(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value value;
	mrb_value aux;
	mrb->get_args("o|o", &value, &aux);
	size_type aux_v = 0;
	if (!mrb_undef_p(aux)) aux_v = unwrap_num<size_type>(state, aux);
	const auto v = unwrap_num<size_type>(state, value);
	const auto result = _vector.in_range(v, aux_v);
	return mrb_bool_value(result);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::index_max(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto index = _vector.index_max();
	return mrb->int_value(static_cast<mrb_int>(index));
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::index_min(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto index = _vector.index_min();
	return mrb->int_value(static_cast<mrb_int>(index));
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::is_complex(const util::Reference<util::State> &state)
{
	static constexpr bool result = math::is_complex<T>();
	return mrb_bool_value(result);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::is_empty(const util::Reference<util::State> &state)
{
	const bool result = _vector.is_empty();
	return mrb_bool_value(result);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::is_finite(const util::Reference<util::State> &state)
{
	const bool result = _vector.is_finite();
	return mrb_bool_value(result);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::is_zero(const util::Reference<util::State> &state)
{
	const bool result = _vector.is_zero();
	return mrb_bool_value(result);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::max(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto value = _vector.max();
	return wrap_num(mrb, value);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::min(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto value = _vector.min();
	return wrap_num(mrb, value);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::real(const util::Reference<util::State> &state)
{
	pod_vector_type real = arma::real(_vector);
	return wrap_vector<pod_type>(state, real);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::replace(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value old_value, new_value;
	mrb->get_args("oo", &old_value, &new_value);
	const auto old_v = unwrap_num<T>(state, old_value);
	const auto new_v = unwrap_num<T>(state, new_value);
	_vector.replace(old_v, new_v);
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::reset(const util::Reference<util::State> &state)
{
	_vector.reset();
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::resize(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	const auto [r, _c, _s] = size_dims(state, arg);
	_vector.resize(r);
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::set_imaginary(const util::Reference<util::State> &state)
{
	if (!math::is_complex<T>()) {
		throw std::runtime_error(
		    "cannot set imaginary part of real vector");
	}
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_vector(state, arg)) {
		throw std::invalid_argument("argument must be a Vector");
	}
	auto other = state->unwrap<Vector>(arg);
	auto vec_obj = other.cast_to<VectorImpl<pod_type>>();
	_vector.set_imag(vec_obj->_vector);
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::set_real(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_vector(state, arg)) {
		throw std::invalid_argument("argument must be a Vector");
	}
	auto other = state->unwrap<Vector>(arg);
	if (other->value_type() != value_type()) {
		throw std::invalid_argument(
		    "argument must be a Vector of the same type");
	}
	auto vec = other.cast_to<VectorImpl<pod_type>>();
	_vector.set_real(vec->_vector);
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::set_size(const util::Reference<util::State> &state)
{
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::size(const util::Reference<util::State> &state)
{
	return make_size(state, n_rows());
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::to_string(const util::Reference<util::State> &state)
{
	const auto mrb = state->mrb();
	std::stringstream ss;
	_vector.print(ss);
	return mrb->str_new_cstr(ss.str().c_str());
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::transform(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	auto arr = mrb->ary_new_capa(n_cols());
	_vector.transform([&](T &value) {
		auto val = wrap_num(mrb, value);
		auto result = mrb->yield(blk, val);
		mrb->ary_push(arr, result);
		auto new_value = unwrap_num<T>(state, result);
		value = new_value;
		return new_value;
	});
	return arr;
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::rows(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arr = mrb->ary_new_capa(_vector.n_rows);
	for (size_type i = 0; i < _vector.n_rows; ++i) {
		auto row_vec = _vector.row(i);
		auto row_obj = wrap_vector(state, row_vec);
		mrb->ary_push(arr, row_obj);
	}
	return arr;
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::row(const util::Reference<util::State> &state)
{
	mrb_int index = 0;
	state->mrb()->get_args("i", &index);
	if (index < 0) index += static_cast<mrb_int>(_vector.n_rows);
	auto row_vec = _vector.row(static_cast<size_type>(index));
	return wrap_vector(state, row_vec);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::fill(const util::Reference<util::State> &state)
{
	mrb_value arg;
	state->mrb()->get_args("o", &arg);
	const auto v = unwrap_num<T>(state, arg);
	_vector.fill(v);
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::imbue(const util::Reference<util::State> &state)
{
	mrb_value blk;
	state->mrb()->get_args("&!", &blk);
	_vector.imbue([&]() -> T {
		const auto mrb = state->mrb();
		const auto result = mrb->yield(blk, mrb_nil_value());
		return unwrap_num<T>(state, result);
	});
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::is_sorted(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_vector.is_sorted());
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::ones(const util::Reference<util::State> &state)
{
	mrb_value arg;
	state->mrb()->get_args("o", &arg);
	if (mrb_undef_p(arg)) {
		_vector.ones();
	} else {
		const auto n = unwrap_num<size_type>(state, arg);
		_vector.ones(n);
	}
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::randn(const util::Reference<util::State> &state)
{
	mrb_value arg;
	state->mrb()->get_args("o", &arg);
	if (mrb_undef_p(arg)) {
		_vector.randn();
	} else {
		const auto n = unwrap_num<size_type>(state, arg);
		_vector.randn(n);
	}
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::randu(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (mrb_undef_p(arg)) {
		_vector.randu();
	} else {
		const auto n = unwrap_num<size_type>(state, arg);
		_vector.randu(n);
	}
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::shed_rows(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg1, arg2;
	mrb->get_args("o|o", &arg1, &arg2);
	if (!mrb_undef_p(arg2)) {
		auto low = unwrap_num<size_type>(state, arg1);
		auto high = unwrap_num<size_type>(state, arg2);
		_vector.shed_rows(low, high);
		return mrb_nil_value();
	}
	if (!mrb_array_p(arg1)) {
		auto row = unwrap_num<size_type>(state, arg1);
		_vector.shed_row(row);
		return mrb_nil_value();
	}
	arma::uvec rows;
	const auto len = RARRAY_LEN(arg1);
	rows.set_size(len);
	for (size_type i = 0; i < len; ++i) {
		const auto item = mrb->ary_ref(arg1, i);
		const auto row = unwrap_num<arma::uword>(state, item);
		rows(i) = row;
	}
	_vector.shed_rows(rows);
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::simple_transpose(const util::Reference<util::State> &state)
{
	vector_type transposed = _vector.st();
	return wrap_vector(state, transposed);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::swap(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value other;
	mrb->get_args("o", &other);
	if (!is_vector(state, other)) {
		throw std::invalid_argument("argument must be a Vector");
	}
	auto other_vec = state->unwrap<Vector>(other);
	if (other_vec->value_type() != value_type()) {
		throw std::invalid_argument(
		    "argument must be a Vector of the same type");
	}
	auto vec = other_vec.cast_to<VectorImpl<T, Rows>>();
	_vector.swap(vec->_vector);
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::swap_cols(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value src, dst;
	mrb->get_args("oo", &src, &dst);
	mrb_int src_idx = unwrap_num<mrb_int>(state, src);
	mrb_int dst_idx = unwrap_num<mrb_int>(state, dst);
	_vector.swap_cols(static_cast<size_type>(src_idx),
	    static_cast<size_type>(dst_idx));
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::swap_rows(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value src, dst;
	mrb->get_args("oo", &src, &dst);
	mrb_int src_idx = unwrap_num<mrb_int>(state, src);
	mrb_int dst_idx = unwrap_num<mrb_int>(state, dst);
	_vector.swap_rows(static_cast<size_type>(src_idx),
	    static_cast<size_type>(dst_idx));
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::transpose(const util::Reference<util::State> &state)
{
	auto transposed = _vector.t();
	return wrap_vector(state, transposed);
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::zeros(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("|o", &arg);
	if (mrb_undef_p(arg)) {
		_vector.zeros();
	} else {
		const auto n = unwrap_num<size_type>(state, arg);
		_vector.zeros(n);
	}
	return mrb_nil_value();
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::add(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	mrb_value out;
	apply_concrete(state, arg, [&]<typename U>(U &v) {
		if constexpr (std::is_base_of_v<Vector, std::decay_t<U>>) {
			auto result = this->_vector + v->value();
			out = wrap_math_value(state, result);
		}
	});
	return out;
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::sub(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::matmul(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::mul(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::div(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::eq(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::ne(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::lt(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::le(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::gt(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::ge(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::is_size(const util::Reference<util::State> &) const
{
	return mrb_false_value();
}

template <typename T, size_type Rows>
bool
VectorImpl<T, Rows>::is_size() const
{
	return false;
}

template <typename T, size_type Rows>
size_type
VectorImpl<T, Rows>::n_rows() const
{
	return _vector.n_rows;
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::insert_columns(const util::Reference<util::State> &state)
{
}

template <typename T, size_type Rows>
mrb_value
VectorImpl<T, Rows>::insert_rows(const util::Reference<util::State> &state)
{
}
