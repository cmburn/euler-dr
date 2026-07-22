/* SPDX-License-Identifier: ISC */

#include <cassert>

#include <SDL3/SDL.h>

#include "euler/app/native/logger.h"
#include "euler/util/logger.h"

using euler::app::native::Logger;
using Severity = Logger::Severity;

enum class Color {
	None,
	Black [[maybe_unused]],
	Red,
	Green,
	Yellow,
	Blue [[maybe_unused]],
	Magenta,
	Cyan,
	White,
};

using enum_t = std::underlying_type_t<Severity>;
static constexpr enum_t SEVERITY_COUNT
    = static_cast<enum_t>(Severity::Unknown) + 1;
static constexpr enum_t COLOR_COUNT = static_cast<enum_t>(Color::White) + 1;

using ColorList = std::array<Color, SEVERITY_COUNT>;

static constexpr ColorList SEVERITY_COLORS = {
	/* [Severity::Debug]   = */ Color::Green,
	/* [Severity::Info]    = */ Color::Green,
	/* [Severity::Warn]    = */ Color::Yellow,
	/* [Severity::Error]   = */ Color::Red,
	/* [Severity::Fatal]   = */ Color::Magenta,
	/* [Severity::Unknown] = */ Color::White,
};

static constexpr ColorList MESSAGE_COLORS = {
	/* [Severity::Debug]   = */ Color::None,
	/* [Severity::Info]    = */ Color::Cyan,
	/* [Severity::Warn]    = */ Color::Yellow,
	/* [Severity::Error]   = */ Color::Red,
	/* [Severity::Fatal]   = */ Color::Red,
	/* [Severity::Unknown] = */ Color::Cyan,
};

static constexpr std::array<std::string_view, COLOR_COUNT> COLORS = {
	/* [Color::Black]   = */ "\033[30m",
	/* [Color::Red]     = */ "\033[31m",
	/* [Color::Green]   = */ "\033[32m",
	/* [Color::Yellow]  = */ "\033[33m",
	/* [Color::Blue]    = */ "\033[34m",
	/* [Color::Magenta] = */ "\033[35m",
	/* [Color::Cyan]    = */ "\033[36m",
	/* [Color::White]   = */ "\033[37m",
};

static constexpr std::array<std::string_view, SEVERITY_COUNT> SEVERITY_NAMES = {
	/* [Severity::Debug]   = */ "debug",
	/* [Severity::Info]    = */ "info",
	/* [Severity::Warn]    = */ "warn",
	/* [Severity::Error]   = */ "error",
	/* [Severity::Fatal]   = */ "fatal",
	/* [Severity::Unknown] = */ "any",
};

static std::string_view
color_for(Color color)
{
	return COLORS.at(static_cast<size_t>(color));
}

static Severity
coerce_severity(enum_t level)
{
	switch (static_cast<Severity>(level)) {
	case Severity::Debug: return Severity::Debug;
	case Severity::Info: return Severity::Info;
	case Severity::Warn: return Severity::Warn;
	case Severity::Error: return Severity::Error;
	case Severity::Fatal: return Severity::Fatal;
	default: return Severity::Unknown;
	}
}

static constexpr const char *
severity_to_str(const Logger::Severity level)
{
	switch (level) {
	case Severity::Debug: return "debug";
	case Severity::Info: return "info";
	case Severity::Warn: return "warn";
	case Severity::Error: return "error";
	case Severity::Fatal: return "fatal";
	default: return "unknown";
	}
}

static FILE *
output_stream(const Logger::Severity level)
{
	switch (level) {
	case Severity::Warn:
	case Severity::Error: [[fallthrough]];
	case Severity::Fatal: return stderr;
	default: return stdout;
	}
}

Logger::Sink::Sink(FILE *output, Severity level)
    : _min_level(level)
    , _output(output)
{
	assert(output != nullptr);
	assert(output != stdout && "Use Logger::stdout_sink() instead");
	assert(output != stderr && "Use Logger::stderr_sink() instead");
}

Logger::Sink::Sink(const std::filesystem::path &path, Severity level)
    : Sink(std::fopen(path.string().c_str(), "a"), level)
{
	if (_output == nullptr) {
		throw std::runtime_error("Failed to open log file: "
		    + path.string() + ": " + std::strerror(errno));
	}
	_own_file = true;
}

Logger::Sink::~Sink()
{
	if (_own_file) fclose(_output);
}
void
Logger::Sink::write_to(Severity severity, const std::string &msg) const
{
	if (severity < _min_level) return;
	std::lock_guard lock(_mutex);
	fprintf(_output, "%s\n", msg.c_str());
	fflush(_output);
}

Logger::Sink::Sink(FILE *output, Severity level, PrivateStruct) { }

std::shared_ptr<Logger::Sink>
Logger::stdout_sink()
{
	static std::weak_ptr<Sink> instance;
	static std::mutex instance_mutex;
	std::lock_guard lock(instance_mutex);
	std::shared_ptr<Sink> shared = nullptr;
	if (!instance.expired()) {
		shared = instance.lock();
		if (shared != nullptr) return shared;
	}
	Sink *sink = nullptr;
	try {
		/* ReSharper disable CppDFAMemoryLeak */
		sink
		    = new Sink(stdout, Severity::Debug, Sink::PrivateStruct {});
		shared = std::shared_ptr<Sink>(sink);
		instance = shared;
		return shared;
		/* ReSharper enable CppDFAMemoryLeak */
		/* ReSharper disable once CppDFAUnreachableCode */
	} catch (...) {
		delete sink;
		throw;
	}
}

std::shared_ptr<Logger::Sink>
Logger::stderr_sink()
{
	static std::weak_ptr<Sink> instance;
	static std::mutex instance_mutex;
	std::lock_guard lock(instance_mutex);
	std::shared_ptr<Sink> shared = nullptr;
	if (!instance.expired()) {
		shared = instance.lock();
		if (shared != nullptr) return shared;
	}
	Sink *sink = nullptr;
	try {
		/* ReSharper disable once CppDFAMemoryLeak */
		sink = new Sink(stderr, Severity::Warn, Sink::PrivateStruct {});
		shared = std::shared_ptr<Sink>(sink);
		instance = shared;
		/* ReSharper disable once CppDFAMemoryLeak */
		return shared;
		/* ReSharper disable once CppDFAUnreachableCode */
	} catch (...) {
		delete sink;
		throw;
	}
}

std::string_view
Logger::severity_name(Severity level)
{
}

const std::string &
Logger::subsystem() const
{
	std::lock_guard lock(_subsystem_mutex);
	return _subsystem;
}

void
Logger::set_subsystem(std::string_view name)
{
	std::lock_guard lock(_subsystem_mutex);
	_subsystem = name;
}

Severity
Logger::severity() const
{
	return _severity;
}

void
Logger::set_severity(Severity level)
{
	_severity = level;
}

euler::util::Reference<euler::util::Logger>
Logger::copy(std::optional<std::string_view> subsystem) const
{
}

void
Logger::write_log(Severity level, const std::string &message) const
{
	static constexpr const char *ANSI_RESET_COLOR = "\033[0m";
	const char *color = nullptr;
	switch (level) {
	case Severity::Debug: color = "\033[36m"; break; /* cyan */
	case Severity::Info: color = "\033[32m"; break;  /* green */
	case Severity::Warn: color = "\033[33m"; break;  /* yellow */
	case Severity::Error: color = "\033[31m"; break; /* red */
	case Severity::Fatal: color = "\033[41m"; break; /* red bg */
	default: color = ANSI_RESET_COLOR; break;        /* reset */
	}
	const auto out = output_stream(level);
	fprintf(out, "%s[%s] %s: %s%s\n", color, subsystem().c_str(),
	    severity_to_str(level), message.c_str(), ANSI_RESET_COLOR);
}

/* ReSharper disable once CppDFAUnreachableFunctionCall */
static SDL_DateTime
current_time()
{
	SDL_Time t;
	SDL_GetCurrentTime(&t);
	SDL_DateTime dt;
	SDL_TimeToDateTime(t, &dt, true);
	return dt;
}

static constexpr size_t MAX_TIME_STRING_SIZE = 26;

/* ReSharper disable once CppDFAUnreachableFunctionCall */
static void
write_time(std::stringstream &ss)
{
	static constexpr const char *WEEK_DAYS[] = {
		"Sun",
		"Mon",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat",
	};
	static constexpr const char *MONTHS[] = {
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec",
	};

	const auto dt = current_time();
	char time_string[MAX_TIME_STRING_SIZE] = { 0 };
	snprintf(time_string, MAX_TIME_STRING_SIZE,
	    "%s %s %i %02d:%02d:%02d %i", WEEK_DAYS[dt.day_of_week],
	    MONTHS[dt.month - 1], dt.day, dt.hour, dt.minute, dt.second,
	    dt.year);
	ss << time_string;
}

std::string
Logger::format_message(Severity level, const std::string &message) const
{
	const auto message_color = this->message_color(level);
	const auto severity_color = this->severity_color(level);
	std::stringstream ss;

	ss << color_for(message_color) << "[" << color_for(severity_color);
	write_time(ss);
	const auto sev_str = severity_name(level);
	ss << color_for(message_color) << "] " << color_for(severity_color);
	const auto padding = MAX_SEVERITY_LENGTH - sev_str.size();
	for (size_t i = 0; i < padding; i++) ss << ' ';
	ss << color_for(message_color) << "[" << color_for(severity_color)
	   << sev_str << color_for(message_color) << "]";
	{
		std::lock_guard lock(_progname_mutex);
		ss << " [" << color_for(severity_color) << _progname
		   << color_for(message_color) << "::";
	}
	{
		std::lock_guard lock(_subsystem_mutex);
		ss << color_for(severity_color) << _subsystem;
	}
	ss << color_for(message_color) << "] -- " << color_for(message_color)
	   << message << std::endl;
	return ss.str();
}
