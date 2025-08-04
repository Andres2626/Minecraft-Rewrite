#pragma once
#include "LogLevel.h"

#include "common.h"

#include <string>
#include <sstream>
#include <iostream>

#define RELPATH strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__

namespace MC 
{

	class MC_API Logger 
	{
	private:
		static LogLevel m_Level; /* global level */
		static LogLevel m_LineLevel; /* on line level */
		std::stringstream m_Buff;
	private:
		Logger();
		~Logger();
	private:
		void Print(int bG, int fG, const char* str);
	public:
		static Logger& Get();
		static Logger& Get(const LogLevel& level);

		/* Set global level */
		static void SetLogLevel(const LogLevel& level);

		void End(const char* file, int line, const char* func);
	private:
		std::string GetTimeStr();
	public:
		template <typename T>
		Logger& operator<<(const T& t);
	};

	template<typename T>
	inline Logger& Logger::operator<<(const T& t)
	{
		m_Buff << t;
		return *this;
	}

}
