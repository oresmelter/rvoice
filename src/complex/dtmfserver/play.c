#include <string.h>
#include <syslog.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "sound.h"
#include "misc.h"
#include "opts.h"
#include "wave.h"


/**
 * verify wav format and return 
 */
char validate_wav(char* buf)
{
int res;
int channels;
Wav* wf=(Wav*)buf;

channels=2;
if(algo.params.mono!=0)
  {
  channels=1;
  };

res=strncmp(wf->ChunkID, "RIFF", 4);
if(res!=0)
  {
  //syslog(LOG_INFO, "no RIFF!");
  return(1);
  };

res=strncmp(wf->Format, "WAVE", 4);
if(res!=0)
  {
  //syslog(LOG_INFO, "no WAVE!");
  return(1);
  };

res=strncmp(wf->SubChunk1ID, "fmt ", 4);
if(res!=0)
  {
  //syslog(LOG_INFO, "no fmt!");
  return(1);
  };

res=strncmp(wf->SubChunk2ID, "data", 4);
if(res!=0)
  {
  //syslog(LOG_INFO, "no data!");
  return(1);
  };


if(wf->AudioFormat!=1)
  {
  syslog(LOG_INFO, "Unknown Wav format");
  return(1);
  };

if(wf->NumChannels!=channels)
  {
  syslog(LOG_INFO, "wrong channels: %d instead of %d", wf->NumChannels, channels);
  //return(1);
  };

if(wf->SampleRate!=algo.params.freq)
  {
  syslog(LOG_INFO, "wrong sample rate: %d instead of %d", wf->SampleRate, algo.params.freq);
  //return(1);
  };

/*
syslog(LOG_INFO, "Audio Format: %d", wf->AudioFormat);
syslog(LOG_INFO, "Number of channels: %d", wf->NumChannels);
syslog(LOG_INFO, "Sample rate: %d", wf->SampleRate);
syslog(LOG_INFO, "Byte Rate: %d", wf->ByteRate);
syslog(LOG_INFO, "Block Align: %d", wf->BlockAlign);
syslog(LOG_INFO, "Bits Per Sample: %d", wf->BitsPerSample);
 */

return(0);
}

/**
 *
 */
char* get_wav(char* filename, int sz)
{
int res;
int fd;
char* tmp;

fd=open(filename, O_RDONLY);
if(fd==-1)
  {
  syslog(LOG_ERR, "Cannot open file %s: error %d (%s)", filename, errno, strerror(errno));
  return(NULL);
  };
tmp=mmap(NULL, sz, PROT_READ, MAP_PRIVATE, fd, 0);
close(fd);
return(tmp);
}


/**
 *
 * playing sound from file
 *
 */
int play_wav(char* filename)
{
int fd;
int len=0;
int res;
char* buf;
unsigned long size;
Wav* wf;
struct stat st;

syslog(LOG_INFO, "файл %s\n", filename);

res=stat(filename, &st);
if(res<0)
  {
  syslog(LOG_ERR, "file %s not exist", filename);
  go_out();
  };

len=st.st_size;
buf=get_wav(filename, len);
if(buf==NULL)
  {
  syslog(LOG_ERR, "Error getting file %s", filename);
  go_out();
  };

if(validate_wav(buf)!=0)
  {
  syslog(LOG_ERR, "Inappropriate sound file %s", filename);
  go_out();
  };

wf=(Wav*) buf;

buf=(char*)&(wf->Data);
size=(wf->SubChunk2Size)*(wf->SampleRate)/(wf->ByteRate);
syslog(LOG_INFO, "samples: %d", size);

play_buffer(buf, size);
//drain_sound();
if(munmap(buf, len)!=0)
  {
  syslog(LOG_ERR, "Cannot unmap memory used for file %s", filename);
  //go_out();
  };

return(0);
}

