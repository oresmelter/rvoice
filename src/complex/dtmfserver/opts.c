#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <limits.h>
#include <syslog.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "opts.h"

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
 * h параметры вывода заголовка WAV
 */
const char* format="dmtvp:s:f:b:h";
char demon;
spars current_params;

/**
 * добавляет запись в алгоритм работы
 */
void add_to_algo(struct work_unit* w)
{
if(algo.head!=NULL)
  {
  algo.tail->next_unit=(void*)w;
  algo.tail=w;
  }
else
  {
  algo.head=w;
  algo.tail=w;
  };
}

/**
 * выделяет память под фрагмент алгоритма
 * и заполняет ее
 */
void allocate_unit(char p, char* v)
{
int n;
struct work_unit* w;
int* pn;

syslog(LOG_INFO, "Создаем запись с параметрами %c, %p\n", p, v);

pn=(int*)malloc(sizeof(int));
if(pn==NULL)
  {
  syslog(LOG_ERR, "Ошибка выделения памяти %d (%s)\n", errno, strerror(errno));
  go_out();
  };

w=(struct work_unit*)malloc(sizeof(struct work_unit));
if(w==NULL)
  {
  syslog(LOG_ERR, "Ошибка выделения памяти %d (%s)\n", errno, strerror(errno));
  go_out();
  };

n=try_parse(p, v);
*pn=n;
w->type=p;
w->param=(void*)pn;
w->next_unit=NULL;
syslog(LOG_INFO, "Создана запись с параметрами %c, %p\n", p, pn);
add_to_algo(w);
}

/**
 * Анализ параметров командной строки
 *
 */
void init_algo()
{
algo.head=NULL;
algo.tail=NULL;
algo.params.mono=0;
algo.params.bits=16;
algo.params.freq=16000;
algo.params.head=0;
}

/**
 * Анализ параметров командной строки
 *
 */
void analize_opts(int argc, char** argv)
{
int res;
struct work_utit* wu;
opterr=0;
demon=0;

init_algo();
res=getopt(argc, argv, format);
syslog(LOG_INFO, "анализируем %d\n", res);
while(res!=-1)
     {
     switch(res)
           {
           case 'd': demon=0x01;
                     break;
           case 'm': algo.params.mono=1;
                     break;
           case 'h': algo.params.head=1;
                     break;
           case 't': allocate_unit('t', "1");
                     break;
           case 'v': allocate_unit('v', "1");
                     break;
           case 'p': allocate_unit('p',optarg);
                     break;
           case 's': allocate_unit('s',optarg);
                     break;
           case 'f': algo.params.freq=try_parse(res, optarg);
                     break;
           case 'b': algo.params.bits=try_parse(res,optarg);
                     break;
           case '?': syslog(LOG_ERR, "Ошибка в параметре <%s>\n", argv[optind]);
                     go_out();
                     break;
           default:  syslog(LOG_ERR, "Ошибка %d\n", res);
                     break;
           };
     res=getopt(argc, argv, format);
     syslog(LOG_INFO, "анализируем %d\n", res);
     };
}
