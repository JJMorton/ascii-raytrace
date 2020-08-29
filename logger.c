#include <errno.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>

#include "logger.h"

FILE *logger_getOutput(LogType t);
void logger_close(void);

FILE *logger_out = NULL;
LogType logger_loglevel = LOGTYPE_LOG;

void logger_close(void)
{
	if (logger_out)
	{
		logger_log(LOGTYPE_INFO, "Attempting to close log file...");
		if (fclose(logger_out) == EOF) logger_log(LOGTYPE_ERROR, "Failed to close log file, error code %i\n", errno);
	}
}

FILE *logger_getOutput(LogType t)
{
	FILE *std[] = { stdout, stdout, stderr };
	return (logger_out ? logger_out : std[(size_t) t]);
}

/* Functions declared in header */

void logger_setLevel(LogType t)
{
	logger_log(LOGTYPE_LOG, "Setting logging level to %i...", (int) t);
	logger_loglevel = t;
	logger_log(LOGTYPE_LOG, "Set logging level to %i", (int) t);
}

void logger_setOutput(const char *filename)
{
	logger_close();
	if ((logger_out = fopen(filename, "w")) == NULL)
	{
		logger_log(LOGTYPE_ERROR, "Failed to open log file \"%s\"", filename);
	}
	else
	{
		logger_log(LOGTYPE_LOG, "Opened log file \"%s\" successfully", filename);
	}
	if (atexit(logger_close) != 0) logger_log(LOGTYPE_ERROR, "Failed to register exit handler for logger");
}

void logger_log(LogType t, const char *fmt, ...)
{
	if (t < logger_loglevel) return;

	const char *desc[] = { "LOG  ", "INFO ", "ERROR" };
	FILE *out = logger_getOutput(t);
	va_list args;
	va_start(args, fmt);

	char timestr[9] = {0};
	time_t now = time(NULL);
	strftime(timestr, 9, "%H:%M:%S", localtime(&now));

	fprintf(out, "[%s] [%s] ", desc[(size_t) t], timestr);
	vfprintf(out, fmt, args);
	fprintf(out, "\n");

	va_end(args);
}

