#include "plog.h"
/* --------------------------------------------------------------------- */
int open_logfile(const char *logfile)
{
    FPlog = fopen(logfile, "a+t");
    if (!FPlog)
	return 0;
    return 1;
}
/* --------------------------------------------------------------------- */
int close_logfile()
{
    return fclose(FPlog);
}
/* --------------------------------------------------------------------- */
void LOG(const char *format, ...)
{
    time_t T;
    char t[16];
    va_list ap;

    if (format[0] == '\n') {
	format++;
	fprintf(FPlog, "\n");
    }

    if (format[0]) {
	T = time(0);
	strftime(t, 16, "\n%H:%M:%S: ", localtime(&T));
	fprintf(FPlog, t);

	va_start(ap, format);
	vfprintf(FPlog, format, ap);
	va_end(ap);
    }

    fflush(FPlog);
}
/* --------------------------------------------------------------------- */
void PLOG(const char *format, ...)
{
    time_t T;
    char t[16];
    va_list ap;

    if (format[0] == '\n') {
	format++;
	printf("\n");
	fprintf(FPlog, "\n");
    }

    if (format[0]) {
	T = time(0);
	strftime(t, 16, "\n%H:%M:%S: ", localtime(&T));
	fprintf(FPlog, t);

	va_start(ap, format);
	//  printf("\n");

	vfprintf(FPlog, format, ap);
	printf(t);
	vprintf(format, ap);
	va_end(ap);
    }
    fflush(FPlog);
    fflush(stdout);
}
/* --------------------------------------------------------------------- */
