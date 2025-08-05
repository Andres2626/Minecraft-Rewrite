#pragma once
#include "common.h"

#include "LogLevel.h"

#define MC_LOG_STDOUT BIT_L(1)
#define MC_LOG_FILE BIT_L(2)

#define MC_RELPATH strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__

/*
 * mc_fmt - Pass format string to log output.
 *
 * This macro passes the format string to the log functions without
 * modifications by default.
 *
 * To add a common prefix to log messages, you can `#undef` this macro
 * and redefine it to suit your needs.
 *
 * Example:
 *     #undef mc_fmt
 *     #define mc_fmt(fmt) "my_module: " fmt
 *
 * The above redefinition causes all mc_* log functions to print messages
 * prefixed with "my_module: ".
 */
#define mc_fmt(fmt) fmt

/* print MC message */
#define mc_print(level, fmt, ...) MC::log_print(level, MC_RELPATH, __LINE__, mc_fmt(fmt), ##__VA_ARGS__);

#define mc_info(fmt, ...) mc_print(MC_LEVEL_INFO, fmt, ##__VA_ARGS__)
#define mc_warn(fmt, ...) mc_print(MC_LEVEL_WARN, fmt, ##__VA_ARGS__)
#define mc_error(fmt, ...) mc_print(MC_LEVEL_ERROR, fmt, ##__VA_ARGS__)
#define mc_fatal(fmt, ...) mc_print(MC_LEVEL_FATAL, fmt, ##__VA_ARGS__)
#define mc_debug(fmt, ...) mc_print(MC_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#define mc_trace(fmt, ...) mc_print(MC_LEVEL_TRACE, fmt, ##__VA_ARGS__)

#if defined MC_USE_DEBUG
#define mc_debug(fmt, ...) mc_print(MC_LEVEL_DEBUG, fmt, ##__VA_ARGS__)
#else
#define mc_debug(...)
#endif

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