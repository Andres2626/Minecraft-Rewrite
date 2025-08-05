#include "Log.h"

#include "Host/console.h"

namespace MC 
{
	static FILE* fp;
	static int log_flags = 0;
	static int log_level = MC_LEVEL_NONE;

	/* write to buffer and print string (using vfprintf) */
	static void internal_vfprintf(FILE* fp, const char* fmt, va_list list)
	{
		/* write to file */
		if (log_flags & MC_LOG_FILE) {
			vfprintf(fp, fmt, list);
			fflush(fp);
		}

		/* write to console */
		if (log_flags & MC_LOG_STDOUT)
			vfprintf(stderr, fmt, list);
	}

	static void internal_write(FILE* fp, const char* fmt, ...) 
	{
		va_list args;

		va_start(args, fmt);
		internal_vfprintf(fp, fmt, args);
		va_end(args);
	}

	/* Set level string before message */
	static void set_prefix_output(int fG, int bG, const char* prefix) 
	{
		internal_write(fp, "[");
		Console::SetTextColor(fG, bG);
		internal_write(fp, prefix);
		Console::ResetTextColor();
		internal_write(fp, "] ");
	}

	/* convert current time to char* */
	static void get_time_buff(const char* fmt, char* buffer, size_t size) 
	{
		time_t now = time(NULL);
		strftime(buffer, size, fmt, localtime(&now));
	}

	int log_init(int flags)
	{
		char time_buff[30], file_buff[30];
		get_time_buff("%F_%H-%M-%S", time_buff, sizeof(time_buff));
		snprintf(file_buff, sizeof(file_buff), "%s.log", time_buff);

		log_flags = flags;

		/* open file */
		if (log_flags & MC_LOG_FILE) {
			fp = fopen(file_buff, "a");
			if (!fp) {
				fprintf(stderr, "[ERROR] could not find log file \"%s\"", file_buff);
				return 0;
			}
		}

		return 1;
	}

	void log_fini() 
	{
		fclose(fp);
	}

	void log_print(int level, const char* file, int line, const char* fmt, ...)
	{
		char time_buff[30];
		va_list args;

		if (level > log_level)
			return; 

		get_time_buff("[%F %T]", time_buff, sizeof(time_buff));
		internal_write(fp, "%s [%s:%i] ", time_buff, file, line);

		switch (level) 
		{
		case MC_LEVEL_INFO:
			set_prefix_output(MC_CONSOLE_BLACK, MC_CONSOLE_BLUE, "info");
			break;
		case MC_LEVEL_WARN:
			set_prefix_output(MC_CONSOLE_BLACK, MC_CONSOLE_YELLOW, "warn");
			break;
		case MC_LEVEL_ERROR:
			set_prefix_output(MC_CONSOLE_BLACK, MC_CONSOLE_RED, "ERROR");
			break;
		case MC_LEVEL_FATAL:
			set_prefix_output(MC_CONSOLE_RED, MC_CONSOLE_INTENSITY, "FATAL");
			break;
		case MC_LEVEL_DEBUG:
			set_prefix_output(MC_CONSOLE_BLACK, MC_CONSOLE_GREEN, "debug");
			break;
		case MC_LEVEL_TRACE:
			set_prefix_output(MC_CONSOLE_CYAN + 8, MC_CONSOLE_INTENSITY, "trace");
			break;
		}

		/* pass arguments */
		va_start(args, fmt);
		internal_vfprintf(fp, fmt, args);
		va_end(args);
	}

	int log_get_level()
	{
		return log_level;
	}

	void log_set_level(int level)
	{
		log_level = level;
	}

}