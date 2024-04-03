#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#include "Helpers/logger.h"
#include "stm32f4xx.h"

/** \brief Redirects `printf()` output to the serial wire out (SWO).
 * This function overrides a weak function symbol and is not to be used directly.
 */
int _write(int file, char *ptr, int len)
{
  int i=0;
  for(i=0 ; i<len ; i++)
    ITM_SendChar((*ptr++));

  return len;
}

char const * const _get_log_level_string(LogLevel const log_level)
{
    switch(log_level)
    {
        case LOG_LEVEL_ERROR:
            return "ERROR";
        case LOG_LEVEL_INFORMATION:
            return "INFO";
        case LOG_LEVEL_DEBUG:
            return "DEBUG";
    }
}

static void _log(LogLevel const log_level, char const * const format, va_list args)
{
    if (log_level > system_log_level)
        return;

	printf("[%s] ", _get_log_level_string(log_level));
	vfprintf(stdout, format, args);
	printf("\n");
}

void log_error(char const * const format, ...)
{
    va_list args;
	va_start(args, format);
    _log(LOG_LEVEL_ERROR, format, args);
    va_end(args);
}

void log_info(char const * const format, ...)
{
    va_list args;
	va_start(args, format);
    _log(LOG_LEVEL_INFORMATION, format, args);
    va_end(args);
}

void log_debug(char const * const format, ...)
{
    va_list args;
	va_start(args, format);
    _log(LOG_LEVEL_DEBUG, format, args);
    va_end(args);
}


