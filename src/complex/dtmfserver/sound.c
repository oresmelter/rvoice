#include <syslog.h>
#include <alsa/asoundlib.h>

#include "opts.h"
#include "misc.h"

snd_pcm_t* playback_handle;
snd_pcm_hw_params_t* hw_params;
snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
//char* device_name="dmix";
//char* device_name="default";
//char* device_name="tee:dmix,'/proc/self/fd/1',raw";
char* device_name="tee:default,'/proc/self/fd/1',raw";

/**
 * map between bits and proper sound format to play
 */
int get_proper_format(int bits)
{
int res;
switch(bits)
      {
      case 8: res=SND_PCM_FORMAT_S8;
              break;
      case 16:res=SND_PCM_FORMAT_S16;
              break;
      case 24:res=SND_PCM_FORMAT_S24;
              break;
      case 32:res=SND_PCM_FORMAT_S32;
              break;
      default:res=SND_PCM_FORMAT_UNKNOWN;
              break;
      };
return(res);
}


/**
 *
 * open device for play
 *
 */
void open_device()
{
int err;

// Allocate the snd_pcm_hw_params_t structure on the stack.

if((err = snd_pcm_hw_params_malloc(&hw_params)) < 0)
  {
  syslog(LOG_ERR, "Audio device error: cannot allocate hardware parameter structure (%s)\n", snd_strerror(err));
  go_out();
  };

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

syslog(LOG_WARNING, "Device successfully opened for playback\n");

}


/**
 *
 * set up device parameters
 *
 */
void simple_setup()
{
int err;
int format = get_proper_format(algo.params.bits);
int rate = algo.params.freq;

int channels=2;
if(algo.params.mono==1)
  {
  channels=1;
  };

// frame size
int frame=algo.params.bits*channels;
// bytes per second
int bps=frame*rate;

syslog(LOG_INFO, "preparing to play:\n");
// latency 1 sec
if((err = snd_pcm_set_params(playback_handle, format, SND_PCM_ACCESS_RW_INTERLEAVED, 1, rate, 1, 1000000)) < 0)
  {
  syslog(LOG_ERR, "Playback open error: %s\n", snd_strerror(err));
  go_out();
  };
syslog(LOG_INFO, "done\n");

syslog(LOG_INFO, "preparing to play:\n");
if((err=snd_pcm_prepare(playback_handle)) < 0)
  {
  syslog(LOG_ERR, "cannot prepare audio interface for use (%s)\n", snd_strerror(err));
  go_out();
  }
syslog(LOG_INFO, "done\n");

}


/**
 *
 * set up device parameters
 *
 */
void setup_device()
{
// как бы получше перейти от битов к формату
int format = get_proper_format(algo.params.bits);
/* Sample rate */
int rate = algo.params.freq;
/* Sample rate returned by snd_pcm_hw_params_set_rate_near */
int exact_rate;
int err;
/* exact_rate == rate --> dir = 0 */
/* exact_rate < rate  --> dir = -1 */
/* exact_rate > rate  --> dir = 1 */
int dir;
/* Number of periods in buffer */
int periods = 2;
/* Period size (bytes) */
snd_pcm_uframes_t period_size = 81920;

int channels=2;
if(algo.params.mono==1)
  {
  channels=1;
  };

// frame size
int frame=algo.params.bits*channels;
// bytes per second
int bps=frame*rate;


syslog(LOG_INFO, "setting access mode:\n");
if((err=snd_pcm_hw_params_set_access(playback_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0)
  {
  syslog(LOG_ERR, "Error setting access (%s).\n", snd_strerror(err));
  go_out();
  };
syslog(LOG_INFO, "done\n");

syslog(LOG_INFO, "setting format %ld (%s):\n", format, snd_pcm_format_name(format));
// Set sample format
if((err=snd_pcm_hw_params_set_format(playback_handle, hw_params, format)) < 0)
  {
  syslog(LOG_ERR, "Error setting format.\n", snd_strerror(err));
  go_out();
  };
syslog(LOG_INFO, "done\n");

// Set sample rate. If the exact rate is not supported
// by the hardware, use nearest possible rate.
syslog(LOG_INFO, "setting freq (%d):\n", rate);
exact_rate = rate;
if((err=snd_pcm_hw_params_set_rate_near(playback_handle, hw_params, &exact_rate, 0)) < 0)
  {
  syslog(LOG_ERR, "Error setting rate.\n", snd_strerror(err));
  go_out();
  };
if(rate != exact_rate)
  {
  syslog(LOG_INFO, "The rate %d Hz is not supported by your hardware.\n ==> Using %d Hz instead.\n", rate, exact_rate);
  };
syslog(LOG_INFO, "done\n");

// Set number of channels
syslog(LOG_INFO, "setting channels number:\n");
if((err=snd_pcm_hw_params_set_channels(playback_handle, hw_params, channels)) < 0)
  {
  syslog(LOG_ERR, "Error setting channels.\n", snd_strerror(err));
  go_out();
  };
syslog(LOG_INFO, "done\n");

// Set buffer size (in frames). The resulting latency is given by
// latency = periodsize * periods / (rate * bytes_per_frame)
if((err=snd_pcm_hw_params_set_buffer_size(playback_handle, hw_params, (period_size * periods)>>2)) < 0)
  {
  syslog(LOG_ERR, "Error setting buffersize.\n");
  go_out();
  };

// set hardware resampling
syslog(LOG_INFO, "setting resample:\n");
if((err = snd_pcm_hw_params_set_rate_resample(playback_handle, hw_params, 0)) < 0)
  {
  syslog(LOG_ERR, "Resampling setup failed for playback: %s\n", snd_strerror(err));
  };
syslog(LOG_INFO, "done\n");

syslog(LOG_INFO, "writting sound parameters:\n");
if((err=snd_pcm_hw_params(playback_handle, hw_params)) < 0)
  {
  syslog(LOG_ERR, "Audio device error: cannot set parameters (%s)\n", snd_strerror(err));
  go_out();
  };
syslog(LOG_INFO, "done\n");

snd_pcm_hw_params_free(hw_params);

syslog(LOG_INFO, "preparing to play:\n");
if((err=snd_pcm_prepare(playback_handle)) < 0)
  {
  syslog(LOG_ERR, "cannot prepare audio interface for use (%s)\n", snd_strerror(err));
  go_out();
  }
syslog(LOG_INFO, "done\n");
}

/**
 *
 * close opened device
 *
 */
void close_device()
{
snd_pcm_drop(playback_handle);
snd_pcm_close(playback_handle);
}
