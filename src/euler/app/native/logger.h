/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_NATIVE_LOGGER_H
#define EULER_APP_NATIVE_LOGGER_H

#include "euler/util/logger.h"

namespace euler::app::native {
class Logger final : public util::Logger {
public:
	[[nodiscard]] const std::string &subsystem() const override;
	void set_subsystem(std::string_view name) override;
	[[nodiscard]] Severity severity() const override;
	void set_severity(Severity level) override;
	[[nodiscard]] util::Reference<util::Logger> copy(
	    std::optional<std::string_view> subsystem) const override;

protected:
	void write_log(Severity level,
	    const std::string &message) const override;

private:
};
} /* namespace euler::app::native */


#endif /* EULER_APP_NATIVE_LOGGER_H */

