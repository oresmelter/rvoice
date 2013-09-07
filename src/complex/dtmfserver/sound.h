#ifndef SOUND_H
#define SOUND_H

#include <alsa/asoundlib.h>

extern snd_pcm_t* playback_handle;
extern snd_pcm_hw_params_t* hw_params;
extern snd_pcm_stream_t stream;


void open_device();
void setup_device();
void simple_setup();

#endif
