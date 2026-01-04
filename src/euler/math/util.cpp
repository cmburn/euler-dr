/* SPDX-License-Identifier: ISC */

#include "euler/math/util.h"

#include "euler/math/size.h"

euler::math::FillForm
euler::math::to_fill_type(const util::Reference<util::State> &state,
    const mrb_sym sym)
{
	const auto str = state->mrb()->sym_name(sym);
	if (strcmp(str, "none") == 0) return FillForm::None;
	if (strcmp(str, "zeros") == 0) return FillForm::Zeros;
	if (strcmp(str, "ones") == 0) return FillForm::Ones;
	if (strcmp(str, "eye") == 0) return FillForm::Eye;
	if (strcmp(str, "randu") == 0) return FillForm::Randu;
	if (strcmp(str, "randn") == 0) return FillForm::Randn;
	if (strcmp(str, "value") == 0) return FillForm::Value;
	auto msg = std::format("unrecognized initialization symbol :{}", str);
	throw std::invalid_argument(std::move(msg));
}

std::array<euler::math::size_type, 1>
euler::math::unwrap_size_vector(const util::Reference<util::State> &state,
    mrb_value arg)
{
	auto size_obj = state->unwrap<Size>(arg);
	return size_obj->dims<1>();
}

std::array<euler::math::size_type, 2>
euler::math::unwrap_size_matrix(const util::Reference<util::State> &state,
    mrb_value arg)
{
	auto size_obj = state->unwrap<Size>(arg);
	return size_obj->dims<2>();
}

std::array<euler::math::size_type, 3>
euler::math::unwrap_size_cube(const util::Reference<util::State> &state,
    mrb_value arg)
{
	auto size_obj = state->unwrap<Size>(arg);
	return size_obj->dims<3>();
}