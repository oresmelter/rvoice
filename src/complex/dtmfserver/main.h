#ifndef MAIN_H
#define MAIN_H

struct sound_params
       {
       unsigned int freq;
       unsigned int bits;
       unsigned char mono;
       unsigned char head;
       };

struct work_unit
       {
       unsigned char type;
       void* param;
       };

struct work_algo
       {
       struct sound_params params;
       int units_quantity;
       struct work_unit** units;
       };

typedef struct work_algo algorithm;

extern algorithm algo;
extern char demon;
extern sound_params current_params;

#endif
