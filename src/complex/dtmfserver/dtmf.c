#include <syslog.h>
#include <math.h>
#include <limits.h>
#include <stdint.h>

#include "dtmf.h"
#include "sound.h"
#include "opts.h"

const int freq_low[4]={697, 770, 852, 941};
const int freq_high[4]={1209, 1336, 1477, 1633};

/* 0 1 2 3 4 5 6 7 8 9 A B C D * # */
const int freq_digit[16]={0x13, 0x00, 0x10, 0x20, 0x01, 0x11, 0x21, 0x02, 0x12, 0x22, 0x30, 0x31, 0x32, 0x33, 0x03, 0x23};

const int digit_duration_ms=200;
const int digit_pause_ms=200;

char* digits_buffers[16]={NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};

unsigned long digit_size=0;
unsigned long digit_frames=0;

/**
 * digit code
 */
static int dtmf_get_digit_index(char k)
{
    if((k>='0')&&(k<='9')) return(k-'0');
    else if((k>='a')&&(k<='d')) return(k-'a'+10);
    else if((k>='A')&&(k<='D')) return(k-'A'+10);
    else if(k=='*') return(14);
    else if(k=='#') return(15);
    else return(-1);
}


/**
 * prepare sound buffers for each digit (using signed model)
 */
int dtmf_prepare()
{
int i;
int frame;
int frames_digit_duration_count=algo.params.freq*digit_duration_ms/1000.0;
int frames_per_full_digit_count=algo.params.freq*(digit_duration_ms+digit_pause_ms)/1000.0;

int digit;
double freq_1, freq_2;
double tmp;
int n;
int b;
int res;

// dtmf frequencies
double f1;
double f2;

char* current_digit;

char bytes=algo.params.bits/8;
unsigned int offset;

int channel;
char channels=2;
if(algo.params.mono==1)
  {
  channels=1;
  };

unsigned int amplitude=((unsigned int)1<<(algo.params.bits-1));

digit_size=frames_per_full_digit_count*bytes*channels;
digit_frames=frames_per_full_digit_count;
syslog(LOG_INFO, "dtmf digit size: %d\n", digit_size);

// digits
for(i=0;i<16;i++)
   {
   current_digit=(char*)malloc(digit_size);
   syslog(LOG_INFO, "current buffer %d: %x", i, current_digit);
   if(current_digit==NULL)
     {
     syslog(LOG_ERR, "not enough memory to prepare dtmf buffers\n");
     go_out();
     };
   digits_buffers[i]=current_digit;
   digit=i;
   freq_1=freq_low[freq_digit[digit]&0x0F];
   freq_2=freq_high[freq_digit[digit]>>4];
   // frames
   for(frame=0; frame<frames_per_full_digit_count; frame++)
      {
      offset=frame*channels*bytes;
      if(frame < frames_digit_duration_count)
        {
        // from -1 to 1
        f1=sin(2*M_PI*frame*freq_1/algo.params.freq);
        f2=sin(2*M_PI*frame*freq_2/algo.params.freq);
        }
      else
        {
        f1=0;
        f2=0;
        };

      // interference
      tmp=(f1+f2)/2;
      res=tmp*amplitude;
      // channels
      for(channel=0; channel<channels; channel++)
         {
         // filling
         for(b=0; b<bytes; b++)
            {
            current_digit[offset+channel*bytes+b]=(res>>(b*8))%0xff;
            };
         };
      };
   };
}


/**
 * playing dtmf tone
 */
int play_dtmf(char* text)
{
int i, frame;
int digits_count = strlen(text);
int digit;

if(digits_count == 0)
  {
  return 0;
  }

syslog(LOG_INFO, "DTMF %s\n", text);

for(i=0; i<digits_count; i++)
   {
   digit=dtmf_get_digit_index(text[i]);
   if(digit!=-1)
     {
     syslog(LOG_INFO, "playing digit %d (%x)", digit, digits_buffers[digit]);
     play_buffer(digits_buffers[digit], digit_frames);
     //exit(0);
     };
   };
syslog(LOG_INFO, "playing digit #");
play_buffer(digits_buffers[15], digit_frames);

syslog(LOG_INFO, "DTMF done\n");
return(0);
}

/**
 * debug write to file
 */
int append_buffer(char* buf, int sz)
{
int fd;
int res;
fd=open("/tmp/debugdtmf.dat", O_WRONLY | O_CREAT | O_TRUNC, 0600);
if(fd==-1)
  {
  syslog(LOG_INFO, "cannot open file for debug write");
  return(-1);
  };

res=write(fd, buf, sz);
if(res!=sz)
  {
  syslog(LOG_INFO, "written not all buffer");
  };
close(fd);
return(0);
}
