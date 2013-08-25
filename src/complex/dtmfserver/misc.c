#include <stdio.h>
#include <unistd.h>

#include "misc.h"
#include "main.h"

/**
 * Формат:
 * d - запуск в режиме демона
 * m - режим моно
 * t - выводить DTMF
 * v - выводить время голосом
 * p <интервал> - пауза
 * s <файл> - вывод файла
 * f <частота> - частота дискретизации
 * b <разрядность> - разрядность в битах
 * h <> параметры вывода заголовка WAV
 */
const char* format="dmtvp:s:f:b:h:";

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
if(st==NULL)
  {
  return(-1);
  };
l=strtol(st, endp, 10);
if((l==LON
G_MIN)||(l==LONG_MAX))
  {
  return(0);
  };
if(*endp!=NULL)
  {
  return(0);
  };
return(1);
}

/**
 * добавляет запись в алгоритм работы
 */
int add_to_algo(struct work_unit w)
{
return(0);
}

/**
 * выделяет память под фрагмент алгоритма
 * и заполняет ее
 */
struct work_unit** allocate_unit(char p, char* v)
{
int n;
if(check_integer(optarg))
  {
  n=atoi(optarg);
  }
else
  {
  syslog(LOG_ERR, "Ошибка в параметре <%c %s>\n", p, v);
  exit(-1);
  };


return();
}

/**
 * Анализ параметров командной строки
 *
 */
int analize_opts(int argc, char** argv)
{
struct work_utit* wu;
opterr=0;
demon=0;
algo.params.mono=0;
while(res=getopt(argc, argv, format)!=-1)
     {
     switch(res)
           {
           case 'd': demon=0x01;
                     break;
           case 'm': algo.params.mono=1;
                     break;
           case 't': allocate_unit('t',optarg);
                     break;
           case 'v': allocate_unit('v',optarg);
                     break;
           case 'p': allocate_unit('p',optarg);
                     break;
           case 's': allocate_unit('s',optarg);
                     break;
           case 'f': 
                     if(check_integer(optarg))
                       {
                       algo.params.freq=atoi(optarg);
                       }
                     else
                       {
                       syslog(LOG_ERR, "Ошибка в параметре <-f %s>\n", optarg);
                       exit(-1);
                       };
                     break;
           case 'b': 
                     if(check_integer(optarg))
                       {
                       algo.params.freq=atoi(optarg);
                       }
                     else
                       {
                       syslog(LOG_ERR, "Ошибка в параметре <-f %s>\n", optarg);
                       exit(-1);
                       };
                     break;
           case 'h': 
                     if(check_integer(optarg))
                       {
                       algo.params.freq=atoi(optarg);
                       }
                     else
                       {
                       syslog(LOG_ERR, "Ошибка в параметре <-f %s>\n", optarg);
                       exit(-1);
                       };
                     break;
           case '?': return(-1);
                     break;
           };

     };

}

