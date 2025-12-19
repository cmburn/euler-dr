/* SPDX-License-Identifier: ISC */

#include "euler/util/error.h"

#include "euler/util/state.h"

using euler::util::Error;

Error::~Error() = default;
Error::Error(const Reference<State> &state, const std::string &message)
    : _message(message)
    , _state(state)
{ }

