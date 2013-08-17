#ifndef UTILS_H
#define UTILS_H

#include <string.h>
const char *convert(const char *s, const char *from_cp, const char *to_cp);
const char *configread(const char *conf, const char *val);
int dodaemon(int nochdir, int noclose);
int fork2();
const char *currentTime();



#endif
