#include "NightOwlEnginePCH.h"
#include "Logger.h"

using namespace dae;

Logger::Logger()
	: m_ConsoleHandle{ GetStdHandle(STD_OUTPUT_HANDLE) }
{
}

void Logger::Log(LogLevel level, const std::string_view msg)
{
	/*FOREGROUND_BLUE
	FOREGROUND_GREEN
	FOREGROUND_RED
	FOREGROUND_INTENSITY*/

	//using std::cerr instead of std::cout incase someone uses cout for file writing atleast cerr will still goto the console
	switch (level)
	{
	case Logger::LogLevel::Info:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE);
		std::cerr << "[INFO]\n" << msg << "\n\n";
		break;
	case Logger::LogLevel::Warning:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
		std::cerr << "[WARNING]\n" << msg << "\n\n";
		break;
	case Logger::LogLevel::Error:
		SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED);
		std::cerr << "[ERROR]\n" << msg << "\n\n";
		break;
	}

	//reseting console color incase anyone wants to print anything to the console
	SetConsoleTextAttribute(m_ConsoleHandle, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void Logger::LogInfo(const std::string_view msg)
{
	Log(LogLevel::Info, msg);
}

void Logger::LogWarning(const std::string_view msg)
{
	Log(LogLevel::Warning, msg);
}

void Logger::LogError(const std::string_view msg)
{
	Log(LogLevel::Error, msg);
}
