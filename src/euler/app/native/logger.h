/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_NATIVE_LOGGER_H
#define EULER_APP_NATIVE_LOGGER_H

#include <filesystem>
#include <vector>
#include <memory>

#include "euler/util/logger.h"

namespace euler::app::native {
class Logger final : public util::Logger {
public:

	class Sink {
	public:
		Sink(FILE *output, Severity level = Severity::Info);
		Sink(const std::filesystem::path &path,
		    Severity level = Severity::Info);
		Sink(const std::string &path,
		    const Severity level = Severity::Info)
		    : Sink(std::filesystem::path(path), level)
		{
		}

		~Sink();

	private:
		void write_to(Severity severity, const std::string &msg) const;

		friend class Logger;
		struct PrivateStruct { };
		Sink(FILE *output, Severity level, PrivateStruct);

		Severity _min_level = Severity::Info;
		bool _own_file = false;
		FILE *_output = nullptr;
		mutable std::mutex _mutex;
	};

	static std::shared_ptr<Sink> stdout_sink();
	static std::shared_ptr<Sink> stderr_sink();
	static std::vector<std::shared_ptr<Sink>>
	default_sinks()
	{
		return { stdout_sink(), stderr_sink() };
	}

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

	std::string format_message(Severity level,
	    const std::string &message) const;
	// Color message_color(Severity level) const;
	// Color severity_color(Severity level) const;
	// static std::string_view color_for(Color color);
	static std::string_view severity_name(Severity level);

	std::string _progname;
	mutable std::mutex _progname_mutex;
	std::string _subsystem;
	mutable std::mutex _subsystem_mutex;
	std::atomic<Severity> _severity = Severity::Info;
	std::vector<std::shared_ptr<Sink>> _sinks;
	mutable std::mutex _sinks_mutex;
};
} /* namespace euler::app::native */


#endif /* EULER_APP_NATIVE_LOGGER_H */

