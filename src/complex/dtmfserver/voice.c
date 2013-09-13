#include <syslog.h>
#include <stdlib.h>

#include "voice.h"
#include "misc.h"


const char* V_PATH="../share/rvoice/rc/";

const char* V_SECONDS = "sekund.wav";
const char* V_MINUTES = "minut.wav";
const char* V_HOURS =  "chasov.wav";

const char* V_ONE = "1.wav";
const char* V_TWO = "2.wav";
const char* V_THREE = "3.wav";
const char* V_FOUR = "4.wav";
const char* V_FIVE = "5.wav";
const char* V_SIX = "6.wav";
const char* V_SEVEN = "7.wav";
const char* V_EIGHT = "8.wav";
const char* V_NINE = "9.wav";

const char* V_TEN = "10.wav";
const char* V_ELEVEN = "11.wav";
const char* V_TWELVE = "12.wav";
const char* V_THIRTEEN = "13.wav";
const char* V_FOURTEEN = "14.wav";
const char* V_FIFTEEN = "15.wav";
const char* V_SIXTEEN = "16.wav";
const char* V_SEVENTEEN = "17.wav";
const char* V_EIGHTEEN = "18.wav";
const char* V_NINETEEN = "19.wav";

const char* V_TWENTY = "20.wav";
const char* V_THIRTY = "30.wav";
const char* V_FOURTY = "40.wav";
const char* V_FIFTY = "50.wav";


voices voice={NULL, NULL};

/**
 * создает и добавляет в список следующее
 * имя файла для воспроизведения.
 */
int allocate_voice(char* f)
{
voices* tmp;
voices* v;

if(v==NULL)
  {
  return(-1);
  };

v=(voices*)malloc(sizeof(voices));
if(v==NULL)
  {
  syslog(LOG_ERR, "Cannot allocate memory for voice\n");
  go_out();
  };

tmp=&voice;

while(tmp->next!=NULL)
     {
     tmp=tmp->next;
     };

tmp->next=v;
v->file=f;
v->next=NULL;
}


/**
 * потрошит строку времени
 */
int parse_time(char* time)
{



}


/**
 * очищает список файлов голоса
 */
void free_voices()
{
voices* it;
voices* nxt;

it=&voice;
while(it->next!=NULL)
     {
     nxt=it;
     free(it);
     it=nxt;
     };
}



/**
 *
 * playing voice files
 *
 */
int play_voice(char* text)
{
syslog(LOG_INFO, "Время %s\n", text);
return(0);
}

