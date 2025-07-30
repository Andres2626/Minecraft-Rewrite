#pragma once
#include "common.h"

/* Logging utils */
#define UNUSED_MODE 0
#define CONSOLE_MODE 1
#define FILE_MODE 2
#define BOTH_MODE 3

/* 
  Initialize LOG system. 
  This function have three work modes the CONSOLE, FILEMODE and BOTH mode
  UNUSED = 0
  CONSOLE = 1
  FILE = 2
  BOTH = 3

  When verbose flag is set the log print all messages provided with
  mc_info function, if verbose = 0 only prints warnings, errors and 
  fatal messages

  returns false if call fails
*/
bool mc_init_log(int verbose, int mode, const char *file = NULL);

/* finalize log system. */
void mc_fini_log();

/* This print if verbose > 0 */
void mc_info(const char *file, int line, const char *fmt, ...);
void mc_warn(const char *file, int line, const char *fmt, ...);
void mc_error(const char *file, int line, const char *fmt, ...);
void mc_fatal(const char *file, int line, const char *fmt, ...);
void mc_debug(const char *file, int line, const char *fmt, ...);
void mc_trace(const char *file, int line, const char *fmt, ...);

/* Log macros. Use this for log! */

#define MC_INFO(fmt, ...) mc_info(__FILE__, __LINE__, fmt, __VA_ARGS__);
#define MC_WARN(fmt, ...) mc_warn(__FILE__, __LINE__, fmt, __VA_ARGS__);
#define MC_ERROR(fmt, ...) mc_error(__FILE__, __LINE__, fmt, __VA_ARGS__);
#define MC_FATAL(fmt, ...) mc_fatal(__FILE__, __LINE__, fmt, __VA_ARGS__);
#define MC_DEBUG(fmt, ...) mc_debug(__FILE__, __LINE__, fmt, __VA_ARGS__);
#define MC_TRACE(fmt, ...) mc_trace(__FILE__, __LINE__, fmt, __VA_ARGS__);

/* Check log macros */

#define MC_INFOCHK(x, fmt, ...) if (!(x)) { MC_INFO(fmt, __VA_ARGS__) }
#define MC_WARNCHK(x, fmt, ...) if (!(x)) { MC_WARN(fmt, __VA_ARGS__) }
#define MC_ERRORCHK(x, fmt, ...) if (!(x)) { MC_ERROR(fmt, __VA_ARGS__) }
#define MC_FATALCHK(x, fmt, ...) if (!(x)) { MC_FATAL(fmt, __VA_ARGS__) }
#define MC_DEBUGCHK(x, fmt, ...) if (!(x)) { MC_DEBUG(fmt, __VA_ARGS__) }
#define MC_TRACECHK(x, fmt, ...) if (!(x)) { MC_TRACE(fmt, __VA_ARGS__) }