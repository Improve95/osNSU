#ifndef LOGGING_H
#define LOGGING_H

#include "stdio.h"
#include "stdarg.h"
#include "cache.h"
//#define LOGGING
#define ERROR
//#define INFO
#define WARN

struct LOGGER {

    void (*error)(const char *, ...);

    void (*warn)(const char *, ...);

    void (*info)(const char *, ...);

} typedef LOGGER;

void errorPrintf(const char *formatString, ...);

void warnPrintf(const char *formatString, ...);

void infoPrintf(const char *formatString, ...);

void loggerPrintf(char *level, char *color, const char *formatString, ...);

char *loggerGetTime();

LOGGER *closeLogger();

LOGGER *initLogger();
#endif //LOGGING_H
