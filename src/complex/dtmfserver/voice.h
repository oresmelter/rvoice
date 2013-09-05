#ifndef VOICE_H
#define VOICE_H

/**
 * PREFIX
 */
extern const char* V_PATH;

struct voice_list
       {
       const char* file;
       struct voice* next;
       };

typedef struct voice_list voices;

extern voices voice;

#endif
