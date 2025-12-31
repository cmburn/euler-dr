/* SPDX-License-Identifier: ISC */

#include "euler/math/nonscalar.h"

euler::math::Nonscalar::Type
euler::math::Nonscalar::type(const util::Reference<util::State> &state,
    const mrb_value arg)
{
	const auto mrb = state->mrb();
	const auto type_value = mrb->funcall(arg, "type", 0);
	if (!mrb_symbol_p(type_value))
		throw std::invalid_argument("invalid type");
	const auto type_sym = mrb_symbol(type_value);
	const auto str = mrb->sym_name(type_sym);
	if (std::strcmp(str, "cube") == 0) return Type::Cube;
	if (std::strcmp(str, "matrix") == 0) return Type::Matrix;
	if (std::strcmp(str, "row_vector") == 0) return Type::RowVector;
	if (std::strcmp(str, "sparse_matrix") == 0) return Type::SparseMatrix;
	if (std::strcmp(str, "vector") == 0) return Type::Vector;
	throw std::invalid_argument("unknown type");
}

euler::math::Nonscalar::ValueType
euler::math::Nonscalar::value_type(const util::Reference<util::State> &state,
    const mrb_value arg)
{
	const auto mrb = state->mrb();
	const auto type_value = mrb->funcall(arg, "value_type", 0);
	if (!mrb_symbol_p(type_value))
		throw std::invalid_argument("invalid value type");
	const auto type_sym = mrb_symbol(type_value);
	const auto str = mrb->sym_name(type_sym);
	if (std::strcmp(str, "int16") == 0) return ValueType::Int16;
	if (std::strcmp(str, "int32") == 0) return ValueType::Int32;
	if (std::strcmp(str, "int64") == 0) return ValueType::Int64;
	if (std::strcmp(str, "uint16") == 0) return ValueType::UInt16;
	if (std::strcmp(str, "uint32") == 0) return ValueType::UInt32;
	if (std::strcmp(str, "uint64") == 0) return ValueType::UInt64;
	if (std::strcmp(str, "float32") == 0) return ValueType::Float32;
	if (std::strcmp(str, "float64") == 0) return ValueType::Float64;
	if (std::strcmp(str, "complex32") == 0) return ValueType::Complex32;
	if (std::strcmp(str, "complex64") == 0) return ValueType::Complex64;
	throw std::invalid_argument("unknown value type");
}