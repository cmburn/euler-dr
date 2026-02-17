/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_NATIVE_LOGGER_H
#define EULER_APP_NATIVE_LOGGER_H

#include <filesystem>

#include <spdlog/spdlog.h>

#include "euler/util/logger.h"
#include "euler/util/object.h"

namespace euler::app::native {
class Logger final : public util::Logger {
public:
	explicit Logger(std::string_view subsystem,
	    const std::shared_ptr<spdlog::sinks::sink> &error = nullptr,
	    const std::shared_ptr<spdlog::sinks::sink> &output = nullptr);
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
	std::string _subsystem;
	mutable std::mutex _subsystem_mutex;
	std::shared_ptr<spdlog::sinks::sink> _error_sink;
	std::shared_ptr<spdlog::sinks::sink> _output_sink;
	std::unique_ptr<spdlog::logger> _logger;
	std::atomic<Severity> _severity = Severity::Info;
};
} /* namespace euler::app::native */

#endif /* EULER_APP_NATIVE_LOGGER_H */
