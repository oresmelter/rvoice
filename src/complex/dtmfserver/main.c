#include <stdio.h>
#include <unistd.h>
#Include <syslog.h>
#include <alsa/asoundlib.h>

#include "engine.h"
#include "misc.h"
#include "main.h"

algorythm algo;

int main(int argc, char** argv)
{
int res;

res=openlog("", LOG_PID, LOG_USER);

printf("dtmf client started\n");
res=analize_opts(argc, argv);

if((algo.params.head!=0)&&(demon!=0))
  {
  //wtf
  exit(-1);
  };

// демонизация
if(demon!=0)
  {
  if(!dodaemon())
    {
    exit(-1);
    };
  };

return(0);
}
