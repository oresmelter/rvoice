#include <stdlib.h>
#include <syslog.h>
#include <errno.h>
#include <limits.h>


#include "misc.h"
#include "main.h"
#include "opts.h"

/**
 * проверка строки содержащей число
 * -1 - отсутствует входная строка
 *  0 - проблемы преобразования
 *  1 - все в порядке
 */
int check_integer(char* st)
{
char* endp=NULL;
long l;
errno=0;
if(st==NULL)
  {
  return(-1);
  };
l=strtol(st, &endp, 10);
if((l==LONG_MIN)||(l==LONG_MAX))
  {
  return(0);
  };
if(errno!=0)
  {
  return(0);
  };
if((*st!=0x00)&&(*endp==0x00))
  {
  return(1);
  }
else
  {
  syslog(LOG_ERR, "результат конверсии %p\n", endp);
  };
return(0);
}

/**
 * парсер целых чисел
 */
int try_parse(char param, char* arg)
{
if(!check_integer(arg))
  {
  syslog(LOG_ERR, "Ошибка при разборе параметра <-%c %s>\n", param, arg);
  go_out();
  };
return(atoi(arg));
}

/**
 * размер пустого буффера равный
 * секунде воспроизведения
 */
int calculate_size()
{
int res;
res=current_params.freq*current_params.bits;
if(current_params.mono!=1)
  {
  res=res*2;
  };
return(res);
}

/**
 * завершение работы
 */
void go_out()
{
closelog();
exit(-1);
}
