#include <syslog.h>
#include <alsa/asoundlib.h>

#include "opts.h"
#include "misc.h"

snd_pcm_t* playback_handle;
snd_pcm_hw_params_t* hw_params;
snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
//char* device_name="dmix";
char* device_name="tee:dmix,'/proc/self/fd/1',raw";

/**
 *
 * open device for play
 *
 */
void open_device()
{
int err;

/* Allocate the snd_pcm_hw_params_t structure on the stack. */
snd_pcm_hw_params_alloca(&hw_params);

if((err = snd_pcm_open(&playback_handle, device_name, stream, 0)) < 0) 
  {
  syslog(LOG_ERR, "Audio device error: cannot open %s (%s)\n", device_name, snd_strerror(err));
  go_out();
  };

if(snd_pcm_hw_params_any(playback_handle, hw_params) < 0)
  {
  syslog(LOG_ERR, "Can not configure this PCM device.\n");
  go_out();
  };

if(snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
  {
  syslog(LOG_ERR, "Error setting access.\n");
  go_out();
  };

syslog(LOG_WARNING, "Device successfully opened for playback\n");
}

/**
 *
 * set up device parameters
 *
 */
void setup_device()
{
// как бы перейти от битов к формату

/* Sample rate */
int rate = current_params.freq;

/* Sample rate returned by */
/* snd_pcm_hw_params_set_rate_near */ 
int exact_rate;

/* exact_rate == rate --> dir = 0 */
/* exact_rate < rate  --> dir = -1 */
/* exact_rate > rate  --> dir = 1 */
int dir;
/* Number of periods */
int periods = 2;
/* Periodsize (bytes) */
snd_pcm_uframes_t periodsize = 8192;

/* Set sample format */
if(snd_pcm_hw_params_set_format(playback_handle, hw_params, SND_PCM_FORMAT_S16_LE) < 0)
  {
  syslog(LOG_ERR, "Error setting format.\n");
  go_out();
  };

/* Set sample rate. If the exact rate is not supported */
/* by the hardware, use nearest possible rate.         */ 
exact_rate = rate;
if(snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &exact_rate, 0) < 0)
  {
  syslog(LOG_ERR, "Error setting rate.\n");
  go_out();
  };

if(rate != exact_rate)
  {
  syslog(LOG_INFO, "The rate %d Hz is not supported by your hardware.\n ==> Using %d Hz instead.\n", rate, exact_rate);
  };

/* Set number of channels */
if(snd_pcm_hw_params_set_channels(playback_handle, hw_params, 2) < 0)
  {
  syslog(LOG_ERR, "Error setting channels.\n");
  go_out();
  };

/* Set number of periods. Periods used to be called fragments. */ 
if(snd_pcm_hw_params_set_periods(playback_handle, hw_params, periods, 0) < 0)
  {
  syslog(LOG_ERR, "Error setting periods.\n");
  go_out();
  };

}

/**
 *
 * close opened device
 *
 */
void close_device()
{
snd_pcm_drop(playback_handle);
}
