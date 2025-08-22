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

	static struct log log = { nullptr, 0, 0, 0 };

	/* write to buffer and print string (using vfprintf) */
	static void internal_vprintf(const char* fmt, va_list list)
	{
		if (log.flags & MC_LOG_STDOUT) {
			va_list copy;
			va_copy(copy, list);
			vfprintf(stderr, fmt, copy);
			va_end(copy);
		}

		if ((log.flags & MC_LOG_FILE) && log.fp) {
			va_list copy;
			va_copy(copy, list);
			vfprintf(log.fp, fmt, copy);
			fflush(log.fp);
			va_end(copy);
		}
	}

	static void internal_write(const char* fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		internal_vprintf(fmt, args);
		va_end(args);
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

	static const char* convert_to_relpath(const char* path)
	{
#if defined (MC_PLATFORM_WINDOWS)
		return strrchr(path, '\\') ? strrchr(path, '\\') + 1 : path;
#else
		return strrchr(path, '/') ? strrchr(path, '/') + 1 : path;
#endif
	}

	void log_print(int level, const char* file, int line, const char* fmt, ...)
	{
		if (level > log.level)
			return;
		
		char tb[30], buff[512];
		const char* file_ = NULL;
		
#if defined (MC_LOG_USE_RELPATH)
		file_ = convert_to_relpath(file);
#else
		file_ = file;
#endif
		get_time_buff("[%Y-%m-%d %H-%M-%S]", tb, sizeof(tb));
		
		int n = snprintf(buff, sizeof(buff), "%s [%s:%i] [%s] ", tb, file_, line, level_str[level]);
		
		va_list args;
		va_start(args, fmt);
		vsnprintf(buff+n, sizeof(buff)-n, fmt, args);
		va_end(args);
		
		internal_write(buff);
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