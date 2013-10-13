#include <syslog.h>
#include <stdlib.h>

#include "main.h"
#include "misc.h"
#include "engine.h"
#include "dtmf.h"
#include "voice.h"
#include "pause.h"
#include "sound.h"
#include "opts.h"
#include "play.h"
#include "timebuf.h"


algorithm algo;


/**
 *
 * initialization
 *
 */
void Prepare(int argc, char** argv)
{
openlog("dtmfserver ", LOG_NDELAY | LOG_PID, LOG_USER);
syslog(LOG_INFO, "Старт службы dtmf\n");

// смотрим чего от нас требуется
analize_opts(argc, argv);

// простая проверка
if((algo.params.head!=0)&&(demon!=0))
  {
  //wtf
  syslog(LOG_ERR, "Невозможно выводить заголовок в режиме демона\n");
  go_out();
  };

// демонизация
if(demon!=0)
  {
  if(!dodaemon())
    {
    go_out();
    };
  };

//для паузы
init_buffer();

// для звука
open_device();
setup_device();
//simple_setup();
// dtmf
dtmf_prepare();
}


/**
 *
 * main cycle of following algorithm
 *
 */
int MainCycle()
{
struct work_unit* w=algo.head;
do
  {
  if(w==NULL)
    {
    syslog(LOG_ERR, "NULL алгоритм\n");
    go_out();
    };
  fill_time();
  switch(w->type)
        {
        case 't': play_dtmf(time_buffer);
                  break;
        case 'v': play_voice(time_buffer);
                  break;
        case 's': play_sound(w->param);
                  break;
        case 'p': play_pause(w->param);
                  break;
        default:  syslog(LOG_ERR, "Ошибка в структуре алгоритма работы: %c\n", w->type);
                  go_out();
                  break;
        };
  w=w->next_unit;
  } while(w!=NULL);
return(0);
}
