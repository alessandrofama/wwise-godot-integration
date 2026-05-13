#pragma once

#include "core/wwise_settings.h"

using namespace godot;

enum class WwiseLogLevel
{
	None = 0,
	Error = 1,
	Warning = 2,
	Log = 3,
	Verbose = 4,
	VeryVerbose = 5
};

class WwiseLogger : public RefCounted
{
	GDCLASS(WwiseLogger, RefCounted);

protected:
	static void _bind_methods() {};

private:
	static WwiseLogger* singleton;
	WwiseLogLevel log_level{ WwiseLogLevel::Log };

public:
	WwiseLogger();
	~WwiseLogger();
	static WwiseLogger* get_singleton();

	void refresh_log_level();

	static void error(const String& p_message);
	static void warning(const String& p_message);
	static void log(const String& p_message);
	static void verbose(const String& p_message);
	static void very_verbose(const String& p_message);

	template <typename... VarArgs> static void error_format(const String& p_format, const VarArgs... p_args)
	{
		error(vformat(p_format, p_args...));
	}

	template <typename... VarArgs> static void warning_format(const String& p_format, const VarArgs... p_args)
	{
		warning(vformat(p_format, p_args...));
	}

	template <typename... VarArgs> static void log_format(const String& p_format, const VarArgs... p_args)
	{
		log(vformat(p_format, p_args...));
	}

	template <typename... VarArgs> static void verbose_format(const String& p_format, const VarArgs... p_args)
	{
		verbose(vformat(p_format, p_args...));
	}

	template <typename... VarArgs> static void very_verbose_format(const String& p_format, const VarArgs... p_args)
	{
		very_verbose(vformat(p_format, p_args...));
	}

	static void log_message(WwiseLogLevel p_level, const String& p_message);
};