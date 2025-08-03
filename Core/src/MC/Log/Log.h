#pragma once
#include "LogLevel.h"
#include "Logger.h"
#include "LogMessage.h"

#define RD_LOG(x)      MC::LogMessage(x, RELPATH, __LINE__, 0)
#define RD_LOG_(x)     MC::LogMessage(x, __FILE__, __LINE__, 0)
#define RD_LOG_CHK(lev, cond)  if (!(cond)) RD_LOG(lev)
#define RD_LOG_CHK_(lev, cond)  if (!(cond)) RD_LOG_(lev)

/* Info level */
#define RD_INFO       RD_LOG(MC::LogLevel::INFO)
#define RD_INFO_      RD_LOG_(MC::LogLevel::INFO)
#define RD_INFO_CHK(cond)   RD_LOG_CHK(MC::LogLevel::INFO, cond)
#define RD_INFO_CHK_(cond)  RD_LOG_CHK_(MC::LogLevel::INFO, cond)

/* Warn level */
#define RD_WARN     RD_LOG(MC::LogLevel::WARN)
#define RD_WARN_    RD_LOG_(MC::LogLevel::WARN)
#define RD_WARN_CHK(cond)   RD_LOG_CHK(MC::LogLevel::WARN, cond)
#define RD_WARN_CHK_(cond)  RD_LOG_CHK_(MC::LogLevel::WARN, cond)

/* Error level */
#define RD_ERROR    RD_LOG(MC::LogLevel::ERROR)
#define RD_ERROR_   RD_LOG_(MC::LogLevel::ERROR)
#define RD_ERROR_CHK(cond)   RD_LOG_CHK(MC::LogLevel::ERROR, cond)
#define RD_ERROR_CHK_(cond)  RD_LOG_CHK_(MC::LogLevel::ERROR, cond)

/* Fatal level */
#define RD_FATAL    RD_LOG(MC::LogLevel::FATAL)
#define RD_FATAL_   RD_LOG_(MC::LogLevel::FATAL)
#define RD_FATAL_CHK(cond)   RD_LOG_CHK(MC::LogLevel::FATAL, cond)
#define RD_FATAL_CHK_(cond)  RD_LOG_CHK_(MC::LogLevel::FATAL, cond)

/* Debug level */
#define RD_DEBUG    RD_LOG(MC::LogLevel::DEBUG)
#define RD_DEBUG_   RD_LOG_(MC::LogLevel::DEBUG)
#define RD_DEBUG_CHK(cond)   RD_LOG_CHK(MC::LogLevel::DEBUG, cond)
#define RD_DEBUG_CHK_(cond)  RD_LOG_CHK_(MC::LogLevel::DEBUG, cond)

/* Trace level */
#define RD_TRACE    RD_LOG(MC::LogLevel::TRACE)
#define RD_TRACE_   RD_LOG_(MC::LogLevel::TRACE)
#define RD_TRACE_CHK(cond)  RD_LOG_CHK(MC::LogLevel::TRACE, cond)
#define RD_TRACE_CHK_(cond)  RD_LOG_CHK_(MC::LogLevel::TRACE, cond)

