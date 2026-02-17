/* SPDX-License-Identifier: ISC */

#include "euler/app/native/logger.h"

euler::app::native::Logger::Logger(std::string_view subsystem,
    const std::shared_ptr<spdlog::sinks::sink> &error,
    const std::shared_ptr<spdlog::sinks::sink> &output)
{
	std::lock_guard lock(_subsystem_mutex);
	_subsystem = subsystem;
	_error_sink = error;
	_output_sink = output;
	std::vector<spdlog::sink_ptr> sinks;
	if (_error_sink != nullptr) sinks.push_back(_error_sink);
	if (_output_sink != nullptr) sinks.push_back(_output_sink);
	_logger = std::make_unique<spdlog::logger>(subsystem.data(),
	    sinks.begin(), sinks.end());
}
const std::string &
euler::app::native::Logger::subsystem() const
{
	std::lock_guard lock(_subsystem_mutex);
	return _subsystem;
}

void
euler::app::native::Logger::set_subsystem(std::string_view name)
{
	_subsystem = name;
}

euler::util::Logger::Severity
euler::app::native::Logger::severity() const
{
	return _severity;
}

void
euler::app::native::Logger::set_severity(Severity level)
{
	_severity = level;
}

euler::util::Reference<euler::util::Logger>
euler::app::native::Logger::copy(
    std::optional<std::string_view> subsystem) const
{
	auto other = util::make_reference<Logger>(
	    subsystem.value_or(_subsystem), _error_sink, _output_sink);
	other->set_severity(severity());
	return other;
}

void
euler::app::native::Logger::write_log(Severity level,
    const std::string &message) const
{
	if (level < severity()) return;
	switch (level) {
	case Severity::Debug: _logger->debug(message); break;
	case Severity::Info: _logger->info(message); break;
	case Severity::Warn: _logger->warn(message); break;
	case Severity::Error: _logger->error(message); break;
	case Severity::Fatal: _logger->critical(message); std::abort();
	case Severity::Unknown: _logger->info(message); break;
	}
}