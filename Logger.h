#pragma once
#include <string>
#include <iostream>
#include <cassert>
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
	static LoggingLevel logLevel = LoggingLevel::All;
	static HANDLE hOut;
	inline void SetupLogging(LoggingLevel level)
	{
		logLevel = level;
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	inline void LogDebug(const std::string& lineInfo, const std::string& debug)
	{
		SetConsoleTextAttribute(hOut, DEBUG_STYLE);
		if(logLevel == LoggingLevel::Debug || logLevel == LoggingLevel::All)
		std::cout << "ERROR: " << lineInfo << " ~ " << debug << std::endl;
		SetConsoleTextAttribute(hOut, DEFAULT_STYLE);
	}

	inline void LogInfo(const std::string& lineInfo, const std::string& info)
	{
		if (logLevel == LoggingLevel::Info || logLevel == LoggingLevel::All)
			std::cout << "INFO: " << lineInfo << " ~ " << info << std::endl;
	}

	inline void SetLogLevel(const LoggingLevel newLevel)
	{
		logLevel = newLevel;
	}
	
}