#include "project.h"

/* ------------------------------------------------------------------------------------------ */
int best_sleep(double sleep_time)
// принимает дробное число секунд и возобновляет операцию ожидания 
// в случае прерывания по сигналу
{
    struct timespec tv;
    // заполнение структуры  на основании указанного числа
    tv.tv_sec = (time_t) sleep_time;
    // добавление неучтенных наносекунд
    tv.tv_nsec = (long) ((sleep_time - tv.tv_sec) * 1e+9);	//+ без пробелов

    while (1) {
	int rval = nanosleep(&tv, &tv);
	if (rval == 0)
	    return 0;
	else if (errno == EINTR)
	    continue;
	else
	    return rval;
    }

    return 0;
}
/* ------------------------------------------------------------------------------------------ */
const char *currentTime(void)
{
    char t[32];
    time_t T = time(0);
    strftime(t, 32, "%G.%m.%d %H.%M.%S", localtime(&T));	// pLOG(L_DEBUG,tdb);
    return strdup((const char *)&t);
}
/* ------------------------------------------------------------------------------------------ */
