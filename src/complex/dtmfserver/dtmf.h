#ifndef DTMF_H
#define DTMF_H

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

struct audio_sound_tag 
       {
       int left;
       int right;
       };

typedef struct audio_sound_tag audio_sound_t;

int dtmf_prepare();
int play_dtmf(char*);

#endif
