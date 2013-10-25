/*
 * hw_params.c - print hardware capabilities
 *
 * compile with: gcc -o hw_params hw_params.c -lasound
 */

#include <stdio.h>
#include <syslog.h>
#include <alsa/asoundlib.h>

#include "opts.h"
#include "sound.h"
#include "test_hw.h"

#define ARRAY_SIZE(a) (sizeof(a) / sizeof *(a))

const snd_pcm_access_t accesses[] = {
	SND_PCM_ACCESS_MMAP_INTERLEAVED,
	SND_PCM_ACCESS_MMAP_NONINTERLEAVED,
	SND_PCM_ACCESS_MMAP_COMPLEX,
	SND_PCM_ACCESS_RW_INTERLEAVED,
	SND_PCM_ACCESS_RW_NONINTERLEAVED,
};

const snd_pcm_format_t formats[] = {
	SND_PCM_FORMAT_S8,
	SND_PCM_FORMAT_U8,
	SND_PCM_FORMAT_S16_LE,
	SND_PCM_FORMAT_S16_BE,
	SND_PCM_FORMAT_U16_LE,
	SND_PCM_FORMAT_U16_BE,
	SND_PCM_FORMAT_S24_LE,
	SND_PCM_FORMAT_S24_BE,
	SND_PCM_FORMAT_U24_LE,
	SND_PCM_FORMAT_U24_BE,
	SND_PCM_FORMAT_S32_LE,
	SND_PCM_FORMAT_S32_BE,
	SND_PCM_FORMAT_U32_LE,
	SND_PCM_FORMAT_U32_BE,
	SND_PCM_FORMAT_FLOAT_LE,
	SND_PCM_FORMAT_FLOAT_BE,
	SND_PCM_FORMAT_FLOAT64_LE,
	SND_PCM_FORMAT_FLOAT64_BE,
	SND_PCM_FORMAT_IEC958_SUBFRAME_LE,
	SND_PCM_FORMAT_IEC958_SUBFRAME_BE,
	SND_PCM_FORMAT_MU_LAW,
	SND_PCM_FORMAT_A_LAW,
	SND_PCM_FORMAT_IMA_ADPCM,
	SND_PCM_FORMAT_MPEG,
	SND_PCM_FORMAT_GSM,
	SND_PCM_FORMAT_SPECIAL,
	SND_PCM_FORMAT_S24_3LE,
	SND_PCM_FORMAT_S24_3BE,
	SND_PCM_FORMAT_U24_3LE,
	SND_PCM_FORMAT_U24_3BE,
	SND_PCM_FORMAT_S20_3LE,
	SND_PCM_FORMAT_S20_3BE,
	SND_PCM_FORMAT_U20_3LE,
	SND_PCM_FORMAT_U20_3BE,
	SND_PCM_FORMAT_S18_3LE,
	SND_PCM_FORMAT_S18_3BE,
	SND_PCM_FORMAT_U18_3LE,
	SND_PCM_FORMAT_U18_3BE,
};

const unsigned int rates[] = {
	5512,
	8000,
	11025,
	16000,
	22050,
	32000,
	44100,
	48000,
	64000,
	88200,
	96000,
	176400,
	192000,
};

int test_hardware()
{
unsigned int i;
unsigned int min, max;
int any_rate;
int err;

open_device();

syslog(LOG_INFO, "Device: %s (type: %s)\n", device_name, snd_pcm_type_name(snd_pcm_type(playback_handle)));

syslog(LOG_INFO, "Access types:");
for(i = 0; i < ARRAY_SIZE(accesses); ++i)
   {
   if(!snd_pcm_hw_params_test_access(playback_handle, hw_params, accesses[i]))
     {
     syslog(LOG_INFO, " %s", snd_pcm_access_name(accesses[i]));
     };
   };

syslog(LOG_INFO, "Formats:");
for(i = 0; i < ARRAY_SIZE(formats); ++i)
   {
   if(!snd_pcm_hw_params_test_format(playback_handle, hw_params, formats[i]))
     {
     syslog(LOG_INFO, " %s", snd_pcm_format_name(formats[i]));
     };
   };

err=snd_pcm_hw_params_get_channels_min(hw_params, &min);
if(err<0)
  {
  syslog(LOG_ERR, "cannot get minimum channels count: %s\n", snd_strerror(err));
  close_device();
  return 1;
  };

err=snd_pcm_hw_params_get_channels_max(hw_params, &max);
if(err<0)
  {
  syslog(LOG_ERR, "cannot get maximum channels count: %s\n", snd_strerror(err));
  close_device();
  return 1;
  };

syslog(LOG_INFO, "Min channels: %d\n", min);
syslog(LOG_INFO, "Max channels: %d\n", max);

syslog(LOG_INFO, "Channels:");
for(i=min; i<=max; ++i)
   {
   if(!snd_pcm_hw_params_test_channels(playback_handle, hw_params, i))
     {
     syslog(LOG_INFO, " %u", i);
     };
   };

err=snd_pcm_hw_params_get_rate_min(hw_params, &min, NULL);
if(err<0)
  {
  syslog(LOG_ERR, "cannot get minimum rate: %s\n", snd_strerror(err));
  close_device();
  return 1;
  };
syslog(LOG_INFO, "min rate: %ld\n", min);

err=snd_pcm_hw_params_get_rate_max(hw_params, &max, NULL);
if(err<0)
  {
  syslog(LOG_ERR, "cannot get maximum rate: %s\n", snd_strerror(err));
  close_device();
  return 1;
  };
syslog(LOG_INFO, "max rate: %ld\n", max);

syslog(LOG_INFO, "Sample rates:");
if(min==max)
  {
  syslog(LOG_INFO, " %u", min);
  }
else if(!snd_pcm_hw_params_test_rate(playback_handle, hw_params, min+1, 0))
       {
       syslog(LOG_INFO, " %u-%u", min, max);
       }
     else
       {
       any_rate = 0;
       };
/*
for (i = 0; i < ARRAY_SIZE(rates); ++i) {
if (!snd_pcm_hw_params_test_rate(pcm, hw_params, rates[i], 0)) {
any_rate = 1;
printf(" %u", rates[i]);
}
}
*/
for(i=0; i<(max-min)+1; ++i)
   {
   if(!snd_pcm_hw_params_test_rate(playback_handle, hw_params, min+i, 0))
     {
     any_rate=1;
     syslog(LOG_INFO, " %u", min+i);
     };
   };

if(!any_rate)
  {
  syslog(LOG_INFO, " %u-%u", min, max);
  };

err=snd_pcm_hw_params_get_period_time_min(hw_params, &min, NULL);
if(err<0)
  {
  syslog(LOG_ERR, "cannot get minimum period time: %s\n", snd_strerror(err));
  close_device();
  return 1;
  };

err=snd_pcm_hw_params_get_period_time_max(hw_params, &max, NULL);
if(err<0)
  {
  syslog(LOG_ERR, "cannot get maximum period time: %s\n", snd_strerror(err));
  close_device();
  return 1;
  };
syslog(LOG_INFO, "Interrupt interval: %u-%u us\n", min, max);

err=snd_pcm_hw_params_get_buffer_time_min(hw_params, &min, NULL);
if(err<0)
  {
  syslog(LOG_ERR, "cannot get minimum buffer time: %s\n", snd_strerror(err));
  close_device();
  return 1;
  };

err=snd_pcm_hw_params_get_buffer_time_max(hw_params, &max, NULL);
if(err<0)
  {
  syslog(LOG_ERR, "cannot get maximum buffer time: %s\n", snd_strerror(err));
  close_device();
  return 1;
  };
syslog(LOG_INFO, "Buffer size: %u-%u us\n", min, max);

close_device();
return 0;
}
