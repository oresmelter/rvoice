#ifndef PROJECT_H
#define PROJECT_H

#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <iconv.h>


#include "debug.h"
#include "plog.h"
//#include "configreader.h"
#include "wav_head.h"
#include "utils.h"


static int def_sample_rate;
static short int def_num_channels;
static short int def_bits_per_sample;
static int verbose, def_daemon, def_t;
static const char *def_out_fmt;

int best_sleep(double sleep_time);


#endif
