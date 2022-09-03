#pragma once
#include <string>
#include <iostream>
#include <cassert>
#include <mutex>
#include <Windows.h>


namespace Logger
{
	constexpr WORD DEBUG_STYLE = (BACKGROUND_RED | BACKGROUND_INTENSITY);
	constexpr WORD DEFAULT_STYLE= (FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	enum class LoggingLevel
	{
		None,
		Info,
		Debug,
		All
	};
	static std::mutex logMutex;
	static LoggingLevel logLevel = LoggingLevel::All;
	static HANDLE hOut;

	inline void SetupLogging(LoggingLevel level)
	{
#ifndef DISABLE_LOGGING
		logLevel = level;
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	}

	inline void LogDebug(const std::string& lineInfo, const std::string& debug)
	{
#ifndef DISABLE_LOGGING
		std::scoped_lock lock(logMutex);
		SetConsoleTextAttribute(hOut, DEBUG_STYLE);
		if(logLevel == LoggingLevel::Debug || logLevel == LoggingLevel::All)
			std::cout << "ERROR: " << lineInfo << " ~ " << debug << std::endl;
		SetConsoleTextAttribute(hOut, DEFAULT_STYLE);
#endif
	}

	inline void LogInfo(const std::string& lineInfo, const std::string& info)
	{
#ifndef DISABLE_LOGGING
		std::scoped_lock lock(logMutex);
		if (logLevel == LoggingLevel::Info || logLevel == LoggingLevel::All)
			std::cout << "INFO: " << lineInfo << " ~ " << info << std::endl;
#endif
	}

	inline void SetLogLevel(const LoggingLevel newLevel)
	{
#ifndef DISABLE_LOGGING
		std::scoped_lock lock(logMutex);
		logLevel = newLevel;
#endif
	}
	
}