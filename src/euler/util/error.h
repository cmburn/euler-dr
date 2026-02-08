/* SPDX-License-Identifier: ISC */

#ifndef EULER_UTIL_ERROR_H
#define EULER_UTIL_ERROR_H

#include <format>
#include <string>

#include "euler/util/object.h"


namespace euler::util {

class State;

/* ReSharper disable CppClassCanBeFinal */
class Error : public std::exception {
public:
	~Error() override;
	Error(const Reference<State> &state, const std::string &message);
	template <typename... Args>
	Error(const Reference<State> &state, std::format_args fmt, Args... args)
	    : Error(state, std::format(fmt, args...))
	{
	}

	enum class Kind {
		Exception,
		Standard,
		Runtime,
		Type,
		ZeroDivision,
		Argument,
		Index,
		Range,
		Name,
		NoMethod,
		Script,
		Syntax,
		LocalJump,
		Regexp,
		Frozen,
		NotImplemented,
		Key,
		FloatDomain,
		Custom,
	};

	[[nodiscard]] virtual Kind
	kind() const
	{
		return Kind::Exception;
	}
	[[nodiscard]] virtual RClass *exception_class();
	[[nodiscard]] virtual mrb_value to_mrb() const;
	[[nodiscard]] const char *
	what() const noexcept override
	{
		return _message.c_str();
	}

protected:
	std::string _message;
	Reference<State> _state;
};

class StandardError : public Error {
public:
	StandardError(const Reference<State> &state, const std::string &message)
	    : Error(state, message)
	{
	}
	~StandardError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Standard;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class RuntimeError : public StandardError {
public:
	RuntimeError(const Reference<State> &state, const std::string &message)
	    : StandardError(state, message)
	{
	}
	~RuntimeError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Runtime;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class TypeError : public Error {
public:
	TypeError(const Reference<State> &state, const std::string &message)
	    : Error(state, message)
	{
	}
	~TypeError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Type;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class ZeroDivisionError : public StandardError {
public:
	ZeroDivisionError(const Reference<State> &state,
	    const std::string &message)
	    : StandardError(state, message)
	{
	}
	~ZeroDivisionError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::ZeroDivision;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class ArgumentError final : public StandardError {
public:
	ArgumentError(const Reference<State> &state, const std::string &message)
	    : StandardError(state, message)
	{
	}
	~ArgumentError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Argument;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value
	to_mrb() const override
	{
		return mrb_nil_value();
	}
};

class IndexError : public StandardError {
public:
	IndexError(const Reference<State> &state, const std::string &message)
	    : StandardError(state, message)
	{
	}
	~IndexError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Index;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class RangeError : public StandardError {
public:
	RangeError(const Reference<State> &state, const std::string &message)
	    : StandardError(state, message)
	{
	}
	~RangeError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Range;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class NameError : public StandardError {
public:
	NameError(const Reference<State> &state, const std::string &message)
	    : StandardError(state, message)
	{
	}
	~NameError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Name;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class NoMethodError : public NameError {
public:
	NoMethodError(const Reference<State> &state, const std::string &message)
	    : NameError(state, message)
	{
	}
	~NoMethodError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::NoMethod;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class ScriptError : public Error {
public:
	ScriptError(const Reference<State> &state, const std::string &message)
	    : Error(state, message)
	{
	}
	~ScriptError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Script;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class SyntaxError : public ScriptError {
public:
	SyntaxError(const Reference<State> &state, const std::string &message)
	    : ScriptError(state, message)
	{
	}
	~SyntaxError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Syntax;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class LocalJumpError : public StandardError {
public:
	LocalJumpError(const Reference<State> &state,
	    const std::string &message)
	    : StandardError(state, message)
	{
	}
	~LocalJumpError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::LocalJump;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class RegexpError : public StandardError {
public:
	RegexpError(const Reference<State> &state, const std::string &message)
	    : StandardError(state, message)
	{
	}
	~RegexpError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Regexp;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class FrozenError : public RuntimeError {
public:
	FrozenError(const Reference<State> &state, const std::string &message)
	    : RuntimeError(state, message)
	{
	}
	~FrozenError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Frozen;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class NotImplementedError : public ScriptError {
public:
	NotImplementedError(const Reference<State> &state,
	    const std::string &message)
	    : ScriptError(state, message)
	{
	}
	~NotImplementedError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::NotImplemented;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class KeyError : public IndexError {
public:
	KeyError(const Reference<State> &state, const std::string &message)
	    : IndexError(state, message)
	{
	}
	~KeyError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::Key;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};

class FloatDomainError : public RangeError {
public:
	FloatDomainError(const Reference<State> &state,
	    const std::string &message)
	    : RangeError(state, message)
	{
	}
	~FloatDomainError() override = default;
	[[nodiscard]] Kind
	kind() const override
	{
		return Kind::FloatDomain;
	}
	[[nodiscard]] RClass *exception_class() override;
	[[nodiscard]] mrb_value to_mrb() const override;
	[[nodiscard]] const char *what() const noexcept override;
};
/* ReSharper restore CppClassCanBeFinal */

// Reference<Error> wrap_exception(const Reference<State> &state, const std::exception &e);
// Reference<Error> wrap_ruby_exception(const Reference<State> &state, RObject *exc);

template <typename T, typename... Args>
inline Reference<Error>
make_error(const Reference<State> &state,
    const std::format_string<Args...> &fmt, Args &&...args)
{
	auto message = std::format(fmt, args...);
	return util::make_reference<T>(state, message);
}

} /* namespace euler::util */

#endif /* EULER_UTIL_ERROR_H */
