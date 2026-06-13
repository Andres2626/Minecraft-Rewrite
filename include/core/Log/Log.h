#pragma once

#ifndef LOG_H
#define LOG_H

#include "common.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#define MC_LOG_STDOUT BIT_L(1)
#define MC_LOG_FILE BIT_L(2)

#if defined (MC_LOG_PREFIX)
#define mc_fmt(fmt) MC_LOG_PREFIX ": " fmt
#else
#define mc_fmt(fmt) fmt
#endif

#define mc_info(fmt, ...) MC::Log::GetInstance()->info(mc_fmt(fmt), ##__VA_ARGS__)
#define mc_warn(fmt, ...)  MC::Log::GetInstance()->warn(mc_fmt(fmt), ##__VA_ARGS__)
#define mc_error(fmt, ...) MC::Log::GetInstance()->error(mc_fmt(fmt), ##__VA_ARGS__)
#define mc_fatal(fmt, ...) MC::Log::GetInstance()->critical(mc_fmt(fmt), ##__VA_ARGS__);
#define mc_trace(fmt, ...) MC::Log::GetInstance()->trace(mc_fmt(fmt), ##__VA_ARGS__)

#if defined(MC_USE_DEBUG)
#define mc_debug(fmt, ...) MC::Log::GetInstance()->debug(mc_fmt(fmt), ##__VA_ARGS__)
#else
#define mc_debug(fmt, ...) 
#endif


namespace MC 
{
	using level_enum = spdlog::level::level_enum;

	class MC_API Log
	{
	public:
		static int Init(mc_u32 flags, level_enum lv);
		static int Fini();
	public:
		static std::shared_ptr<spdlog::logger>& GetInstance();
	};
}

#endif