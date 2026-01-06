/* SPDX-License-Identifier: ISC */

#include "euler/math/size.h"

#include "euler/math/impl.h"

euler::math::Nonscalar::Type
euler::math::Size::type() const
{
	return Type::Vector;
}

euler::math::Nonscalar::ValueType
euler::math::Size::value_type() const
{
	return nonscalar_value_type_v<size_type>;
}

mrb_value
euler::math::Size::at(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_int index = 0;
	mrb->get_args("i", &index);
	if (index < 0) index += static_cast<mrb_int>(3);
	if (index < 0 || index >= 3)
		throw std::out_of_range("index out of range");
	const auto value = _vector(static_cast<size_type>(index));
	return wrap_num(mrb, value);
}

mrb_value
euler::math::Size::set_at(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_int index = 0;
	mrb_value value;
	mrb->get_args("io", &index, &value);
	if (index < 0) index += static_cast<mrb_int>(3);
	if (index < 0 || index >= 3)
		throw std::out_of_range("index out of range");
}

mrb_value
euler::math::Size::clamp(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value min_value, max_value;
	mrb->get_args("oo", &min_value, &max_value);
	const auto min_v = unwrap_num<size_type>(state, min_value);
	const auto max_v = unwrap_num<size_type>(state, max_value);
	_vector = arma::clamp(_vector, min_v, max_v);
	return mrb_nil_value();
}

mrb_value
euler::math::Size::clean(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_float tol = 0.0;
	mrb->get_args("f", &tol);
	_vector.clean(static_cast<size_type>(tol));
	return mrb_nil_value();
}

mrb_value
euler::math::Size::copy_size(const util::Reference<util::State> &state)
{
	throw std::runtime_error("cannot change dimensions of Size object");
}

mrb_value
euler::math::Size::map(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	mrb_value arr = mrb->ary_new_capa(_vector.n_elem);
	_vector.for_each([&](size_type &value) {
		auto val = wrap_num(mrb, value);
		auto result = mrb->yield(blk, val);
		mrb->ary_push(arr, result);
	});
	return arr;
}

mrb_value
euler::math::Size::has_infinity(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_vector.has_inf());
}

mrb_value
euler::math::Size::has_nan(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_vector.has_nan());
}

mrb_value
euler::math::Size::imaginary(const util::Reference<util::State> &state)
{
	vector_type imag = arma::imag(_vector);
	assert(imag.n_elem == 3);
	return wrap_size(state, imag);
}

mrb_value
euler::math::Size::in_range(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value min_value, max_value;
	mrb->get_args("oo", &min_value, &max_value);
	const auto min_v = unwrap_num<size_type>(state, min_value);
	const auto max_v = unwrap_num<size_type>(state, max_value);
	return mrb_bool_value(_vector.in_range(min_v, max_v));
}

mrb_value
euler::math::Size::index_max(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto index = _vector.index_max();
	return mrb->int_value(static_cast<mrb_int>(index));
}

mrb_value
euler::math::Size::index_min(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	const auto index = _vector.index_min();
	return mrb->int_value(static_cast<mrb_int>(index));
}

mrb_value
euler::math::Size::is_complex(const util::Reference<util::State> &state)
{
	return mrb_bool_value(math::is_complex<size_type>());
}

mrb_value
euler::math::Size::is_empty(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_vector.is_empty());
}

mrb_value
euler::math::Size::is_finite(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_vector.is_finite());
}

mrb_value
euler::math::Size::is_zero(const util::Reference<util::State> &state)
{
	return mrb_bool_value(_vector.is_zero());
}

mrb_value
euler::math::Size::max(const util::Reference<util::State> &state)
{
	return wrap_num(state->mrb(), _vector.max());
}

mrb_value
euler::math::Size::min(const util::Reference<util::State> &state)
{
	return wrap_num(state->mrb(), _vector.min());
}

mrb_value
euler::math::Size::real(const util::Reference<util::State> &state)
{
	auto real = arma::real(_vector);
	assert(real.n_rows == 3);
	return wrap_size(state, real);
}

mrb_value
euler::math::Size::replace(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value old_value, new_value;
	mrb->get_args("oo", &old_value, &new_value);
	const auto old_v = unwrap_num<size_type>(state, old_value);
	const auto new_v = unwrap_num<size_type>(state, new_value);
	_vector.replace(old_v, new_v);
	return mrb_nil_value();
}

mrb_value
euler::math::Size::reset(const util::Reference<util::State> &)
{
	_vector.reset();
	return mrb_nil_value();
}

mrb_value
euler::math::Size::resize(const util::Reference<util::State> &state)
{
	throw std::runtime_error("cannot resize Size object");
}

mrb_value
euler::math::Size::set_imaginary(const util::Reference<util::State> &state)
{
	throw std::runtime_error("cannot set imaginary part of Size");
}

mrb_value
euler::math::Size::set_real(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value value;
	mrb->get_args("o", &value);
	// value can be either a Size or a Vector of size 3
	if (is_size(state, value)) {
		auto size_obj = state->unwrap<Size>(value);
		_vector.set_real(size_obj->_vector);
		return mrb_nil_value();
	}

	if (Vector::is_vector(state, value)) {
		auto vec_obj = state->unwrap<Vector>(value);
		assert(!vec_obj->is_size());
		switch (vec_obj->value_type()) {
		case ValueType::Int16: return set_real_from<int16_t>(vec_obj);
		case ValueType::Int32: return set_real_from<int32_t>(vec_obj);
		case ValueType::Int64: return set_real_from<int64_t>(vec_obj);
		case ValueType::UInt16: return set_real_from<uint16_t>(vec_obj);
		case ValueType::UInt32: return set_real_from<uint32_t>(vec_obj);
		case ValueType::UInt64: return set_real_from<uint64_t>(vec_obj);
		case ValueType::Float32: return set_real_from<float>(vec_obj);
		case ValueType::Float64: return set_real_from<double>(vec_obj);
		case ValueType::Complex32: {
			auto complex_vec
			    = vec_obj
				  .cast_to<VectorImpl<std::complex<float>>>();
			arma::Col real = arma::real(complex_vec->_vector);
			_vector(0) = static_cast<size_type>(real(0));
			_vector(1) = static_cast<size_type>(real(1));
			_vector(2) = static_cast<size_type>(real(2));
			return mrb_nil_value();
		}

		case ValueType::Complex64: {
			auto complex_vec
			    = vec_obj
				  .cast_to<VectorImpl<std::complex<double>>>();
			arma::Col real = arma::real(complex_vec->_vector);
			_vector(0) = static_cast<size_type>(real(0));
			_vector(1) = static_cast<size_type>(real(1));
			_vector(2) = static_cast<size_type>(real(2));
			return mrb_nil_value();
		}
		}
	}
}

mrb_value
euler::math::Size::set_size(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value arg;
	mrb->get_args("o", &arg);
	if (!is_size(state, arg)) {
		throw std::invalid_argument("argument must be a Size");
	}

	auto size_obj = state->unwrap<Size>(arg);
	_vector = size_obj->_vector;
	return mrb_nil_value();
}

mrb_value
euler::math::Size::size(const util::Reference<util::State> &state)
{
	return make_size(state, 3);
}

mrb_value
euler::math::Size::to_string(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	std::stringstream ss;
	_vector.print(ss);
	return mrb->str_new_cstr(ss.str().c_str());
}

mrb_value
euler::math::Size::transform(const util::Reference<util::State> &state)
{
	auto mrb = state->mrb();
	mrb_value blk;
	mrb->get_args("&!", &blk);
	_vector.transform([&](auto &value) {
		auto val = wrap_num(mrb, value);
		auto result = mrb->yield(blk, val);
		auto new_value = unwrap_num<size_type>(state, result);
		value = new_value;
		return new_value;
	});
	return mrb_nil_value();
}

mrb_value
euler::math::Size::fill(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::imbue(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::insert_columns(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::is_sorted(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::ones(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::randn(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::randu(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::shed_rows(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::simple_transpose(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::swap(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::swap_cols(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::swap_rows(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::transpose(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::zeros(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::add(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::sub(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::matmul(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::mul(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::div(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::eq(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::ne(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::lt(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::le(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::gt(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::ge(const util::Reference<util::State> &state)
{
}

bool
euler::math::Size::is_cube() const
{
	return _vector(0) != 0 && _vector(1) != 0 && _vector(2) != 0;
}

bool
euler::math::Size::is_matrix() const
{
	return _vector(0) == 0 && _vector(1) != 0 && _vector(2) != 0;
}

bool
euler::math::Size::is_vector() const
{
	return _vector(0) == 0 && _vector(1) == 0 && _vector(2) != 0;
}

bool
euler::math::Size::is_size() const
{
	return true;
}

mrb_value
euler::math::Size::is_size(const util::Reference<util::State> &state) const
{
	return mrb_true_value();
}

euler::math::size_type
euler::math::Size::n_cols() const
{
	return 3;
}

euler::math::size_type
euler::math::Size::n_rows() const
{
	return 1;
}

mrb_value
euler::math::Size::rows(const util::Reference<util::State> &state)
{
}

mrb_value
euler::math::Size::row(const util::Reference<util::State> &state)
{
}

euler::math::size_type
euler::math::Size::row_count()
{
	return _vector(0) == 0 ? 1 : _vector(0);
}

mrb_value
euler::math::Size::insert_rows(const util::Reference<util::State> &state)
{
}

euler::util::Reference<euler::math::Vector>
euler::math::Size::as_vector_impl()
{
	arma::Col<size_type> cols;
	cols.set_size(3);
	cols(0) = _vector(0);
	cols(1) = _vector(1);
	cols(2) = _vector(2);
	return util::make_reference<VectorImpl<size_type>>(cols);
}