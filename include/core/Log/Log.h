#pragma once

#ifndef LOG_H
#define LOG_H

#include "common.h"

#include "LogLevel.h"

#define MC_LOG_STDOUT BIT_L(1)
#define MC_LOG_FILE BIT_L(2)

/* print MC message */
#define mc_print(level, fmt, ...) MC::log_print(level, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define mc_info(fmt, ...) mc_print(MC_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define mc_warn(fmt, ...) mc_print(MC_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define mc_error(fmt, ...) mc_print(MC_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define mc_fatal(fmt, ...) mc_print(MC_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#define mc_trace(fmt, ...) mc_print(MC_LEVEL_TRACE, fmt, ##__VA_ARGS__)

#if defined MC_USE_DEBUG
#define mc_debug(fmt, ...) mc_print(MC_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#else
#define mc_debug(...)
#endif

#define MC_LOG_USE_RELPATH

namespace MC {

	/*
	 * The function to start the log system accepts two flags:
	 * 
	 * MC_LOG_STDOUT and MC_LOG_FILE.
	 * 
     * Use both flags to log messages to the console and the log file
	 * simultaneously.
	 */
	int MC_API log_init(int flags);

	/* If you set FILE flag, this function only close log file. */
	void MC_API log_fini();

	/*
	 * Main MC logging function
	 * The log system support the following levels:
	 * 
	 * - MC_NONE_LEVEL  (0)
	 * - MC_LEVEL_INFO  (1)
	 * - MC_LEVEL_WARN  (2)
	 * - MC_LEVEL_ERROR (3)
	 * - MC_LEVEL_FATAL (4)
	 * - MC_LEVEL_DEBUG (5)
	 * - MC_LEVEL_TRACE (6)
	 * 
	 * Note: 
	 * - Before writing to the log, ensure the log level is set
	 *   appropriately. If the given level is lower than the current
	 *   threshold, the message will not be printed.
	 */
	void MC_API log_print(int level, const char* file, int line, const char* fmt, ...);

	/* Obtain global level */
	int MC_API log_get_level();

	/* Set global level */
	void MC_API log_set_level(int level);
}

#endif