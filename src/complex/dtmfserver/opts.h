#ifndef OPTS_H
#define OPTS_H

/**
 * "plughw:0,0";
 * "dmix";
 * "default";
 * "tee:dmix,'/proc/self/fd/1',raw";
 * "tee:default,'/proc/self/fd/1',raw";
 */

extern char* device_name;

struct sound_params
       {
       unsigned int freq;
       unsigned int bits;
       unsigned char mono;
       unsigned char head;
       };

typedef struct sound_params spars;

struct work_unit
       {
       unsigned char type;
       void* param;
       void* next_unit;
       };

struct work_algo
       {
       spars params;
       struct work_unit* head;
       struct work_unit* tail;
       };

typedef struct work_algo algorithm;

extern algorithm algo;
extern char demon;


int analyze_opts(int, char**);


#endif
