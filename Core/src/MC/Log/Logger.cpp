#include "Logger.h"
#include "Host/console.h"

#include <ctime>

namespace MC {
	
	LogLevel Logger::m_Level;
	LogLevel Logger::m_LineLevel;

	Logger::Logger()
	{

	}

	Logger::~Logger()
	{

	}

	void Logger::Print(int bG, int fG, const char* str)
	{
		std::cout << "[";
		Console::SetTextColor(bG, fG);
		std::cout << str;
		Console::ResetTextColor();
		std::cout << "]: ";
	}

	Logger& Logger::Get()
	{
		static Logger instance;
		return instance;
	}

	Logger& Logger::Get(const LogLevel& level)
	{
		m_LineLevel = level;
		return Get();
	}

	void Logger::SetLogLevel(const LogLevel& level)
	{
		m_Level = level;
	}

	void Logger::End(const char* file, int line, const char* func)
	{
		/* check line level and global level */
		if (m_LineLevel > m_Level)
			return;

		/* convert buffer to string */
		std::string str = m_Buff.str();
		std::string date = GetTimeStr();

		if (func != NULL)
			std::cout << func << "() ";

		std::cout << "[" << file << ":" << line << "] [" << date << "] ";

		switch (m_LineLevel)
		{
		case LogLevel::INFO:
			Print(RD_CONSOLE_BLACK, RD_CONSOLE_BLUE, "Info");
			break;
		case LogLevel::WARN:
			Print(RD_CONSOLE_BLACK, RD_CONSOLE_YELLOW, "WARN");
			break;
		case LogLevel::ERROR:
			Print(RD_CONSOLE_BLACK, RD_CONSOLE_RED, "ERROR");
			break;
		case LogLevel::FATAL:
			Print(RD_CONSOLE_RED, RD_CONSOLE_INTENSITY, "FATAL");
			break;
		case LogLevel::DEBUG:
			Print(RD_CONSOLE_BLACK, RD_CONSOLE_GREEN, "Debug");
			break;
		case LogLevel::TRACE:
			Print(RD_CONSOLE_CYAN + 8, RD_CONSOLE_INTENSITY, "TRACE");
			break;
		}

		/* print string */
		std::cout << str << std::endl;

		/* restore buffer */
		m_Buff.str("");
		m_Buff.clear();

		m_LineLevel = LogLevel::NONE;
	}

	std::string Logger::GetTimeStr()
	{
		std::time_t now = std::time(nullptr);
		char buff[30];
		std::strftime(buff, 30, "%F %T", std::localtime(&now));
		return buff;
	}
}