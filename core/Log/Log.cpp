#include "Log/Log.h"

namespace MC 
{
	struct log {
		FILE* fp;
		time_t now;
		int flags;
		int level;
	};
	
	static const char* level_str[] = {
		"", "info", "warn", "ERROR", "FATAL", // basic levels
		"debug", "trace"
	};
	
	static struct log log;

	/* write to buffer and print string (using vfprintf) */
	static void internal_vfprintf(const char* fmt, va_list list)
	{
		if (log.flags & MC_LOG_FILE) {
			vfprintf(log.fp, fmt, list);
			fflush(log.fp);
		}

		if (log.flags & MC_LOG_STDOUT)
			vfprintf(stderr, fmt, list);
	}

	static void internal_write(const char* fmt, ...) 
	{
		va_list args;
		va_start(args, fmt);
		internal_vfprintf(fmt, args);
		va_end(args);
	}
	
	static void set_prefix_output(const char* prefix) 
	{
		//TODO: Implement colors
		internal_write("[%s] ", prefix);
	}

	static void get_time_buff(const char* fmt, char* buffer, size_t size) 
	{
		strftime(buffer, size, fmt, localtime(&log.now));
	}

	int log_init(int flags)
	{
		char tb[30], fb[30];
		log.now = time(NULL);
		get_time_buff("%Y-%m-%d_%H-%M-%S", tb, sizeof(tb));
		snprintf(fb, sizeof(fb), "%s.log", tb);
		log.flags = flags;
		if (log.flags & MC_LOG_FILE) {
			log.fp = fopen(fb, "a");
			if (!log.fp) {
				fprintf(stderr, "[ERROR] could not find log file \"%s\"", fb);
				return 0;
			}
		}
		return 1;
	}

	void log_fini() 
	{
		fclose(log.fp);
	}

	void log_print(int level, const char* file, int line, const char* fmt, ...)
	{
		if (level > log.level)
			return; 
		
		char tb[30]; // time buffer
		get_time_buff("[%Y-%m-%d %H-%M-%S]", tb, sizeof(tb));
		internal_write("%s [%s:%i] ", tb, file, line);
		set_prefix_output(level_str[level]);
		va_list args;
		va_start(args, fmt);
		internal_vfprintf(fmt, args);
		va_end(args);
	}

	int log_get_level()
	{
		return log.level;
	}

	void log_set_level(int level)
	{
		log.level = level;
	}

}