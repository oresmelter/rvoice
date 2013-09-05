#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <syslog.h>
#include <stdlib.h>

#include <sys/time.h>

#include "sound.h"
#include "pause.h"
#include "misc.h"
#include "opts.h"

// 1024 * 1024 * 1024
#define MAX_SIZE 1073741824

char* ZEROES;

/**
 * инициализация пустого буффера для использования 
 * при воспроизведении паузы
 */
void init_buffer()
{
int fd;
char* tmp=NULL;
fd=open("/dev/zero", O_RDONLY);
if(fd==-1)
  {
  syslog(LOG_ERR, "cannot open /dev/zero\n");
  exit(-1);
  };
tmp=mmap(tmp, MAX_SIZE, PROT_READ, MAP_PRIVATE, fd, 0);
if(tmp==NULL)
  {
  syslog(LOG_ERR, "cannot map /dev/zero\n");
  exit(-1);
  };
ZEROES=tmp;
}

/**
 *
 * making a pause
 *
 */
int play_pause(int* time)
{
int i;
int res;
snd_pcm_sframes_t frames;

syslog(LOG_INFO, "пауза %d\n", *time);
res=calculate_size();
syslog(LOG_INFO, "размер буфера %d\n", res);
return(0);
for(i=0; i<*time; i++)
   {
   while((frames = snd_pcm_writei(playback_handle, ZEROES, res)) < 0) 
        {
        snd_pcm_prepare(playback_handle);
        syslog(LOG_WARNING, "<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>\n");
        };
   };
return(0);
}

