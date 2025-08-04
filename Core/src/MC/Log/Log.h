#pragma once
#include "LogLevel.h"
#include "Logger.h"
#include "LogMessage.h"

#define MC_LOG(x)      MC::LogMessage(x, RELPATH, __LINE__, 0)
#define MC_LOG_(x)     MC::LogMessage(x, __FILE__, __LINE__, 0)

/* Info level */
#define MC_INFO       MC_LOG(MC::LogLevel::INFO)
#define MC_INFO_      MC_LOG_(MC::LogLevel::INFO)

/* Warn level */
#define MC_WARN     MC_LOG(MC::LogLevel::WARN)
#define MC_WARN_    MC_LOG_(MC::LogLevel::WARN)

/* Error level */
#define MC_ERROR    MC_LOG(MC::LogLevel::ERROR)
#define MC_ERROR_   MC_LOG_(MC::LogLevel::ERROR)

/* Fatal level */
#define MC_FATAL    MC_LOG(MC::LogLevel::FATAL)
#define MC_FATAL_   MC_LOG_(MC::LogLevel::FATAL)

/* Debug level */
#define MC_DEBUG    MC_LOG(MC::LogLevel::DEBUG)
#define MC_DEBUG_   MC_LOG_(MC::LogLevel::DEBUG)

/* Trace level */
#define MC_TRACE    MC_LOG(MC::LogLevel::TRACE)
#define MC_TRACE_   MC_LOG_(MC::LogLevel::TRACE)

