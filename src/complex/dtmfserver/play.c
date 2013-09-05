#include <syslog.h>

#include "sound.h"
#include "misc.h"

/**
 *
 * playing sound from file
 *
 */
int play_sound(char* filename)
{
syslog(LOG_INFO, "файл %s\n", filename);

return(0);
}

