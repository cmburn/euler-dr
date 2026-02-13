/* SPDX-License-Identifier: ISC */

#include "euler/util/error.h"

#include "euler/util/state.h"

using euler::util::Error;

Error::~Error() = default;
Error::Error(const Reference<State> &state, const std::string &message)
    : _message(message)
    , _state(state)
{
}

RClass *
Error::exception_class() const
{
	return  _state->mrb()->mrb()->eException_class;
}

mrb_value
Error::to_mrb() const
{
	const auto exc_class = exception_class();
	const auto message = _state->mrb()->str_new_cstr(_message.c_str());
	return _state->mrb()->exc_new_str(exc_class, message);
}
RClass *
euler::util::StandardError::exception_class() const
{
	return _state->mrb()->mrb()->eStandardError_class;
}

mrb_value
euler::util::StandardError::to_mrb() const
{
	const auto exc_class = exception_class();
	const auto message = _state->mrb()->str_new_cstr(_message.c_str());
	return _state->mrb()->exc_new_str(exc_class, message);
}

const char *
euler::util::StandardError::what() const noexcept
{
	return Error::what();
}

RClass *
euler::util::ArgumentError::exception_class() const
{
	return _state->mrb()->mrb()->eException_class;
}
