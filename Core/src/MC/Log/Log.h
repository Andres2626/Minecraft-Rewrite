#pragma once
#include "LogLevel.h"
#include "Logger.h"
#include "LogMessage.h"

#define MC_LOG(x)      MC::LogMessage(x, RELPATH, __LINE__, 0)
#define MC_LOG_(x)     MC::LogMessage(x, __FILE__, __LINE__, 0)
#define MC_LOG_CHK(lev, cond)  if (!(cond)) MC_LOG(lev)
#define MC_LOG_CHK_(lev, cond)  if (!(cond)) MC_LOG_(lev)

/* Info level */
#define MC_INFO       MC_LOG(MC::LogLevel::INFO)
#define MC_INFO_      MC_LOG_(MC::LogLevel::INFO)
#define MC_INFO_CHK(cond)   MC_LOG_CHK(MC::LogLevel::INFO, cond)
#define MC_INFO_CHK_(cond)  MC_LOG_CHK_(MC::LogLevel::INFO, cond)

/* Warn level */
#define MC_WARN     MC_LOG(MC::LogLevel::WARN)
#define MC_WARN_    MC_LOG_(MC::LogLevel::WARN)
#define MC_WARN_CHK(cond)   MC_LOG_CHK(MC::LogLevel::WARN, cond)
#define MC_WARN_CHK_(cond)  MC_LOG_CHK_(MC::LogLevel::WARN, cond)

/* Error level */
#define MC_ERROR    MC_LOG(MC::LogLevel::ERROR)
#define MC_ERROR_   MC_LOG_(MC::LogLevel::ERROR)
#define MC_ERROR_CHK(cond)   MC_LOG_CHK(MC::LogLevel::ERROR, cond)
#define MC_ERROR_CHK_(cond)  MC_LOG_CHK_(MC::LogLevel::ERROR, cond)

/* Fatal level */
#define MC_FATAL    MC_LOG(MC::LogLevel::FATAL)
#define MC_FATAL_   MC_LOG_(MC::LogLevel::FATAL)
#define MC_FATAL_CHK(cond)   MC_LOG_CHK(MC::LogLevel::FATAL, cond)
#define MC_FATAL_CHK_(cond)  MC_LOG_CHK_(MC::LogLevel::FATAL, cond)

/* Debug level */
#define MC_DEBUG    MC_LOG(MC::LogLevel::DEBUG)
#define MC_DEBUG_   MC_LOG_(MC::LogLevel::DEBUG)
#define MC_DEBUG_CHK(cond)   MC_LOG_CHK(MC::LogLevel::DEBUG, cond)
#define MC_DEBUG_CHK_(cond)  MC_LOG_CHK_(MC::LogLevel::DEBUG, cond)

/* Trace level */
#define MC_TRACE    MC_LOG(MC::LogLevel::TRACE)
#define MC_TRACE_   MC_LOG_(MC::LogLevel::TRACE)
#define MC_TRACE_CHK(cond)  MC_LOG_CHK(MC::LogLevel::TRACE, cond)
#define MC_TRACE_CHK_(cond)  MC_LOG_CHK_(MC::LogLevel::TRACE, cond)

