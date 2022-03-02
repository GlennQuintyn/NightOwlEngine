#pragma once
#include "Singleton.h"

namespace dae
{
	class Logger : public Singleton<Logger>
	{
	public:
		enum class LogLevel
		{
			Info,
			Warning,
			Error
		};

		~Logger() = default;

		void Log(LogLevel level, const std::string_view msg);
		void LogInfo(const std::string_view msg);
		void LogWarning(const std::string_view msg);
		void LogError(const std::string_view msg);

		Logger(const Logger& other) = delete;
		Logger(Logger&& other) = delete;
		Logger& operator=(const Logger& other) = delete;
		Logger& operator=(Logger&& other) = delete;
	private:
		friend class Singleton<Logger>;
		Logger();

		HANDLE m_ConsoleHandle;
	};
}

