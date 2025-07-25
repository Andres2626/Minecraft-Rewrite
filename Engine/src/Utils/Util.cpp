
#include "util.h"
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static bool init = false;
static FILE *fp = NULL;
static int verb = 0;
static int md = UNUSED_MODE;

#ifdef _MSC_VER
#define RELPATH(path) (strrchr(path, '\\') ? strrchr(path, '\\') + 1 : path)
#else
#define RELPATH(path) (strrchr(path, '/') ? strrchr(path, '/') + 1 : path)
#endif

/* ANSI Terminal Colors. NOTE: This is not supported in all consoles
TODO: improve this! */
#define RST  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[96m"
#define KWHT  "\x1B[37m"

#define FRED(x) KRED x RST
#define FGRN(x) KGRN x RST
#define FYEL(x) KYEL x RST
#define FBLU(x) KBLU x RST
#define FMAG(x) KMAG x RST
#define FCYN(x) KCYN x RST
#define FWHT(x) KWHT x RST

#define BOLD(x) "\x1B[1m" x RST
#define UNDL(x) "\x1B[4m" x RST

static char* get_date_buff() 
{
	char* buff = (char*)malloc(sizeof(char) * 64);
	if (!buff) {
		printf("alloc() error.");
		return nullptr;
	}

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);

	/* Write date and hour to buffer */
	sprintf(buff, "%i/%i/%i %i:%i:%i", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
		tm.tm_hour, tm.tm_min, tm.tm_sec);
	return buff;
}

bool mc_init_log(int verbose, int mode, const char *file) 
{
	verb = verbose;
	md = mode;
	
	init = true;
	if (mode == FILE_MODE || mode == BOTH_MODE) {
		if (!file) {
			printf("log_file == NULL.\n");
			return false;
		}
		fp = fopen(file, "wb");
		if (!fp) {
			printf("Error opening log file. %s\n", file);
			init = false;
		}
	}
	return init;
}

void mc_fini_log() 
{
	if (fp)
		fclose(fp);
}

void mc_info(const char *file, int line, const char* fmt, ...) 
{
	if (!init) {
		printf("Log system is not initialized.\n");
		return;
	}

	if (verb > 0) {
		char *date_buff = get_date_buff();
		char *log_buff = (char*)malloc(255 * sizeof(char));
		if (!log_buff) {
			printf("alloc() error.");
			return;
		}

		va_list ap;
		va_start(ap, fmt);

		char* cur = log_buff;
		const char* end = log_buff + 255;
		cur += snprintf(cur, end - cur, "[%s] [%s]: ", date_buff, FBLU("Info"));
		if (cur < end)
			cur += vsnprintf(cur, end - cur, fmt, ap);

		va_end(ap);

		switch (md) {
		case CONSOLE_MODE:
			fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
			break;
		case FILE_MODE:
			fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
			break;
		case BOTH_MODE:
			fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
			fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
			break;
		default:
			break;
		}

		if (log_buff)
			free(log_buff);
		if (date_buff)
			free(date_buff);
	}
}

void mc_warn(const char* file, int line, const char* fmt, ...) 
{
	if (!init) {
		printf("Log system is not initialized.\n");
		return;
	}

	char* date_buff = get_date_buff();
	char* log_buff = (char*)malloc(255 * sizeof(char));
	if (!log_buff) {
		printf("alloc() error.");
		return;
	}

	va_list ap;
	va_start(ap, fmt);

	char* cur = log_buff;
	const char* end = log_buff + 255;
	cur += snprintf(cur, end - cur, "[%s] [%s]: ", date_buff, FYEL("WARN"));
	if (cur < end)
		cur += vsnprintf(cur, end - cur, fmt, ap);

	va_end(ap);

	switch (md) {
	case CONSOLE_MODE:
		fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	case FILE_MODE:
		fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	case BOTH_MODE:
		fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
		fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	default:
		break;
	}

	if (log_buff)
		free(log_buff);
	if (date_buff)
		free(date_buff);
}

void mc_error(const char* file, int line, const char* fmt, ...) 
{
	if (!init) {
		printf("Log system is not initialized.\n");
		return;
	}

	char* date_buff = get_date_buff();
	char* log_buff = (char*)malloc(255 * sizeof(char));
	if (!log_buff) {
		printf("alloc() error.");
		return;
	}

	va_list ap;
	va_start(ap, fmt);

	char* cur = log_buff;
	const char* end = log_buff + 255;
	cur += snprintf(cur, end - cur, "[%s] [%s]: ", date_buff, BOLD(FRED("ERROR")));
	if (cur < end)
		cur += vsnprintf(cur, end - cur, fmt, ap);

	va_end(ap);

	switch (md) {
	case CONSOLE_MODE:
		fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	case FILE_MODE:
		fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	case BOTH_MODE:
		fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
		fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	default:
		break;
	}

	if (log_buff)
		free(log_buff);
	if (date_buff)
		free(date_buff);

}

void mc_fatal(const char* file, int line, const char* fmt, ...) 
{
	if (!init) {
		printf("Log system is not initialized.\n");
		return;
	}

	char* date_buff = get_date_buff();
	char* log_buff = (char*)malloc(255 * sizeof(char));
	if (!log_buff) {
		printf("alloc() error.");
		return;
	}

	va_list ap;
	va_start(ap, fmt);

	char* cur = log_buff;
	const char* end = log_buff + 255;
	cur += snprintf(cur, end - cur, "[%s] [%s]: ", date_buff, FRED("FATAL"));
	if (cur < end)
		cur += vsnprintf(cur, end - cur, fmt, ap);

	va_end(ap);

	switch (md) {
	case CONSOLE_MODE:
		fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	case FILE_MODE:
		fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	case BOTH_MODE:
		fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
		fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	default:
		break;
	}

	if (log_buff)
		free(log_buff);
	if (date_buff)
		free(date_buff);

	mc_fini_log();
	exit(1);
}

void mc_debug(const char *file, int line, const char *fmt, ...) 
{
	if (!init) {
		printf("Log system is not initialized.\n");
		return;
	}

	char* date_buff = get_date_buff();
	char* log_buff = (char*)malloc(255 * sizeof(char));
	if (!log_buff) {
		printf("alloc() error.");
		return;
	}

	va_list ap;
	va_start(ap, fmt);

	char* cur = log_buff;
	const char* end = log_buff + 255;
	cur += snprintf(cur, end - cur, "[%s] [%s]: ", date_buff, FGRN("DEBUG"));
	if (cur < end)
		cur += vsnprintf(cur, end - cur, fmt, ap);

	va_end(ap);

	switch (md) {
	case CONSOLE_MODE:
		fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	case FILE_MODE:
		fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	case BOTH_MODE:
		fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
		fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	default:
		break;
	}

	if (log_buff)
		free(log_buff);
	if (date_buff)
		free(date_buff);
}

void mc_trace(const char *file, int line, const char *fmt, ...) 
{
	if (!init) {
		printf("Log system is not initialized.\n");
		return;
	}

	char* date_buff = get_date_buff();
	char* log_buff = (char*)malloc(255 * sizeof(char));
	if (!log_buff) {
		printf("alloc() error.");
		return;
	}

	va_list ap;
	va_start(ap, fmt);

	char* cur = log_buff;
	const char* end = log_buff + 255;
	cur += snprintf(cur, end - cur, "[%s] [%s]: ", date_buff, FCYN("TRACE"));
	if (cur < end)
		cur += vsnprintf(cur, end - cur, fmt, ap);

	va_end(ap);

	switch (md) {
	case CONSOLE_MODE:
		fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	case FILE_MODE:
		fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	case BOTH_MODE:
		fprintf(stderr, "%s:%i %s\n", RELPATH(file), line, log_buff);
		fprintf(fp, "%s:%i %s\n", RELPATH(file), line, log_buff);
		break;
	default:
		break;
	}

	if (log_buff)
		free(log_buff);
	if (date_buff)
		free(date_buff);

	mc_fini_log();
}
