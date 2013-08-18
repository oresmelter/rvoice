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

int add_to_algo(struct work_unit w)
{
return(0);
}

struct work_unit** allocate_unit()
{
return();
}

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
           case 't': 
                     break;
           case 'v': 
                     break;
           case 'p': algo.=optarg;
                     break;
           case 's': 
                     break;
           case 'f': 
                     break;
           case 'b': 
                     break;
           case 'h': 
                     break;
           case '?': return(-1);
                     break;
           };

     };

}

