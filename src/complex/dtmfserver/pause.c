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

// 8 * 1024 * 1024
#define MAX_SIZE 8388608

unsigned char* ZEROES;
int zeroes_fd=-1;

const char* device="/dev/zero";

/**
 * инициализация пустого буффера для использования 
 * при воспроизведении паузы
 */
void init_buffer()
{
char* tmp=NULL;

zeroes_fd=open(device, O_RDONLY);
if(zeroes_fd==-1)
  {
  syslog(LOG_ERR, "cannot open %s\n", device);
  exit(-1);
  };
tmp=mmap(NULL, MAX_SIZE, PROT_READ, MAP_SHARED, zeroes_fd, 0);
if(tmp==NULL)
  {
  syslog(LOG_ERR, "cannot map %s\n", device);
  exit(-1);
  };

/*
tmp=(char*)calloc(1, MAX_SIZE);
if(tmp==NULL)
  {
  syslog(LOG_ERR, "Not enough memory for filling pause buffer\n");
  exit(-1);
  };
*/
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
//syslog(LOG_INFO, "пауза %d\n", *time);
res=algo.params.freq;
//syslog(LOG_INFO, "размер буфера %d\n", res);
for(i=0; i<*time; i++)
   {
   syslog(LOG_INFO, "second #%d\n",i);
   play_buffer(ZEROES, res);
   };
return(0);
}

