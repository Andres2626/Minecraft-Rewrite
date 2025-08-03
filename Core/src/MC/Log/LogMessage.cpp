#include "LogMessage.h"

namespace MC {

	LogMessage::LogMessage(const LogLevel& lev, const char* file, int line, const char* function)
		: m_Lev(lev), m_File(file), m_Line(line), m_Func(function)
	{
		Logger::Get(m_Lev);
	}

	LogMessage::~LogMessage()
	{
		Logger::Get().End(m_File, m_Line, m_Func);
	}

}