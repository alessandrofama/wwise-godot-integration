#include "wwise_logger.h"

WwiseLogger* WwiseLogger::singleton = nullptr;

WwiseLogger* WwiseLogger::get_singleton() { return singleton; }

WwiseLogger::WwiseLogger()
{
	ERR_FAIL_COND(singleton != nullptr);
	singleton = this;

	refresh_log_level();
}

WwiseLogger::~WwiseLogger()
{
	ERR_FAIL_COND(singleton != this);
	singleton = nullptr;
}

void WwiseLogger::refresh_log_level()
{
	if (WwiseSettings::get_singleton())
	{
		int level = WwiseSettings::get_singleton()->get_setting(
				WwiseSettings::get_singleton()->wwise_logger_settings.log_level, (int)WwiseLogLevel::Log);
		log_level = static_cast<WwiseLogLevel>(level);
	}
}

void WwiseLogger::error(const String& p_message) { log_message(WwiseLogLevel::Error, p_message); }

void WwiseLogger::warning(const String& p_message) { log_message(WwiseLogLevel::Warning, p_message); }

void WwiseLogger::log(const String& p_message) { log_message(WwiseLogLevel::Log, p_message); }

void WwiseLogger::verbose(const String& p_message) { log_message(WwiseLogLevel::Verbose, p_message); }

void WwiseLogger::very_verbose(const String& p_message) { log_message(WwiseLogLevel::VeryVerbose, p_message); }

void WwiseLogger::log_message(WwiseLogLevel p_level, const String& p_message)
{
	if (get_singleton()->log_level >= p_level)
	{
		String prefix = "WwiseGodot: ";

		switch (p_level)
		{
			case WwiseLogLevel::None:
				break;
			case WwiseLogLevel::Error:
				UtilityFunctions::push_error(prefix + "(ERROR) " + p_message);
				break;
			case WwiseLogLevel::Warning:
				UtilityFunctions::push_warning(prefix + "(WARNING) " + p_message);
				break;
			case WwiseLogLevel::Log:
				UtilityFunctions::print(prefix + "(LOG) " + p_message);
				break;
			case WwiseLogLevel::Verbose:
				UtilityFunctions::print(prefix + "(VERBOSE) " + p_message);
				break;
			case WwiseLogLevel::VeryVerbose:
				UtilityFunctions::print(prefix + "(VERYVERBOSE) " + p_message);
				break;
		}
	}
}