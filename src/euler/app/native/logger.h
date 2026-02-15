/* SPDX-License-Identifier: ISC */

#ifndef EULER_APP_NATIVE_LOGGER_H
#define EULER_APP_NATIVE_LOGGER_H

#include <filesystem>

#include "euler/util/object.h"
#include "euler/util/logger.h"

namespace euler::app::native {
class Logger final : public util::Logger {
private:

	class Sink {
	public:
		Sink(FILE *output);
		Sink(const std::filesystem::path &path);
		Sink(const std::string &path)
		    : Sink(std::filesystem::path(path))
		{
		}

		~Sink();

	private:
		void write_to(Severity severity, const std::string &msg) const;

		friend class Logger;
		struct PrivateStruct { };
		Sink(FILE *output, Severity level, PrivateStruct);

		bool _own_file = false;
		FILE *_output = nullptr;
		mutable std::mutex _mutex;
	};

public:
	[[nodiscard]] const std::string &subsystem() const override
	{
		return _subsystem;
	}
	void set_subsystem(std::string_view name) override {
		_subsystem = name;
	}
	[[nodiscard]] Severity severity() const override {
		return _severity;
	}
	void set_severity(Severity level) override {
		_severity = level;
	}
	[[nodiscard]] util::Reference<util::Logger> copy(
	    std::optional<std::string_view> subsystem) const override;

protected:
	void write_log(Severity level,
	    const std::string &message) const override;
private:
	std::string _subsystem;
	Severity _severity = Severity::Info;
};
} /* namespace euler::app::native */

#endif /* EULER_APP_NATIVE_LOGGER_H */
