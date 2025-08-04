#pragma once
#include "Logger.h"
#include "LogLevel.h"

#include "common.h"

namespace MC {

	class MC_API LogMessage {
	public:
		LogLevel m_Lev;
		const char *m_File, *m_Func;
		int m_Line;
	public:
		LogMessage(const LogLevel& lev, const char* file, int line, const char* function);
		~LogMessage();
	public:
		template <typename T>
		LogMessage& operator<<(const T& t);
	};

	template<typename T>
	inline LogMessage& LogMessage::operator<<(const T& t)
	{
		Logger::Get(m_Lev) << t;
		return *this;
	}

}