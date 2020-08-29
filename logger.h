#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

typedef enum
{
	LOGTYPE_LOG, LOGTYPE_INFO, LOGTYPE_ERROR
} LogType;

void logger_setLevel(LogType t);
void logger_setOutput(const char *filename);
void logger_log(LogType t, const char *fmt, ...);

#endif // LOGGER_H

