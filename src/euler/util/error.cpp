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
euler::util::ArgumentError::exception_class()
{ return _state->mrb()->mrb()->eException_class; }
