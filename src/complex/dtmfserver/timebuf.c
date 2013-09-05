#include <time.h>
#include <syslog.h>

#include "misc.h"


char time_buffer[200];

/**
 *
 */
void fill_time()
{
time_t t;
struct tm *tmp;

t = time(NULL);
tmp = localtime(&t);
if(tmp == NULL)
  {
  syslog(LOG_ERR, "ошибка localtime\n");
  go_out();
  };

if(strftime(time_buffer, sizeof(time_buffer), "%H:%M:%S", tmp) == 0)
  {
  syslog(LOG_ERR, "Ошибка при возврате из функции strftime\n");
  go_out();
  };

}

