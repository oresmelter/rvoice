#include <syslog.h>

#include "dtmf.h"

/**
 *
 * playing dtmf tone
 *
 */
int play_dtmf(char* text)
{
syslog(LOG_INFO, "DTMF %s\n", text);
return(0);
}

