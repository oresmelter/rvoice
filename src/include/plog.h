/* @(#)log.c 1.2	 xxx
 * Функции для ведения логов
 */
#include <stdio.h>
#include <time.h>
#include <stdarg.h>
// static int logmode  = 0;
int open_logfile(const char *logfile);
int close_logfile();
void LOG(const char *format, ...);
void PLOG(const char *format, ...);
FILE *FPlog;
