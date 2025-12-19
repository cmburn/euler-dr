/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_LOGGER_H
#define EULER_UTIL_LOGGER_H

#include <array>
#include <optional>
#include <string_view>

#include "euler/util/object.h"

namespace euler::util {
class Logger : public util::Object {

	enum class Severity {
		Debug,
		Info,
		Warn,
		Error,
		Fatal,
		Unknown,
	};

	virtual const std::string &subsystem() const = 0;
	virtual Severity severity() const = 0;
	virtual void set_severity(Severity level) = 0;

	virtual Reference<Logger> copy(std::optional<std::string_view> subsystem
	    = std::nullopt) const = 0;


protected:
	virtual void write_log(Severity level, const std::string &message) const = 0;
};
} /* namespace euler::util */


#endif /* EULER_UTIL_LOGGER_H */

