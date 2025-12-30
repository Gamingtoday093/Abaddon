#pragma once
#include <iostream>
#include <windows.h>

#if defined(DEBUG) || defined(RELEASE)
#include <cassert>
#endif

enum class eLogType : uint8_t
{
	Standard,
	Success,
	Warning,
	Error
};

namespace Log
{
	constexpr WORD COLOR_WHITE = 15;
	constexpr WORD COLOR_GREEN = 10;
	constexpr WORD COLOR_YELLOW = 14;
	constexpr WORD COLOR_RED = 12;

	static void Print(const std::string& aString, eLogType aLogType = eLogType::Standard)
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

		switch (aLogType)
		{
			case eLogType::Standard:
				std::cout << aString << std::endl;
				break;

			case eLogType::Success:
				SetConsoleTextAttribute(hConsole, COLOR_GREEN);
				std::cout << aString << std::endl;
				break;

			case eLogType::Warning:
				SetConsoleTextAttribute(hConsole, COLOR_YELLOW);
				std::cout << aString << std::endl;
				break;

			case eLogType::Error:
				SetConsoleTextAttribute(hConsole, COLOR_RED);
				std::cout << aString << std::endl;
				break;

			default:
				break;

		}

		SetConsoleTextAttribute(hConsole, COLOR_WHITE);
	}
}

#if defined(DEBUG) || defined(RELEASE)
#define LOG(string) Log::Print(string, eLogType::Standard)
#define LOG_SUCCESS(string) Log::Print(string, eLogType::Success)
#define LOG_WARNING(string) Log::Print(string, eLogType::Warning)
#define LOG_ERROR(string) Log::Print(string, eLogType::Error)
#define Assert(bool) assert(bool)

#else
#define LOG(string)
#define LOG_SUCCESS(string) 
#define LOG_WARNING(string) 
#define LOG_ERROR(string) 
#define Assert(bool)
#endif

