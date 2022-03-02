#include "NightOwlEnginePCH.h"
#include "Logger.h"
#include <iostream>
#include <windows.h>

dae::Logger::Logger()
	: m_ConsoleHandle{ GetStdHandle(STD_OUTPUT_HANDLE) }
{
}

void dae::Logger::Log(LogLevel level, const std::string_view msg)
{
	/*FOREGROUND_BLUE
	FOREGROUND_GREEN
	FOREGROUND_RED
	FOREGROUND_INTENSITY*/

	//using std::cerr instead of std::cout incase someone uses cout for file writing atleast cerr will still goto the console
	switch (level)
	{
	case dae::Logger::LogLevel::Info:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cerr << "[INFO]\t" << msg << "\n";
		break;
	case dae::Logger::LogLevel::Warning:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		std::cerr << "[WARNING]\t" << msg << "\n";
		break;
	case dae::Logger::LogLevel::Error:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cerr << "[ERROR]\t" << msg << "\n";
		break;
	}
}

void dae::Logger::LogInfo(const std::string_view msg)
{
	Log(LogLevel::Info, msg);
}

void dae::Logger::LogWarning(const std::string_view msg)
{
	Log(LogLevel::Warning, msg);
}

void dae::Logger::LogError(const std::string_view msg)
{
	Log(LogLevel::Error, msg);
}
