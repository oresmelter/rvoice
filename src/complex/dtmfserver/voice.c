#include <syslog.h>
#include <stdlib.h>
#include <string.h>

#include "voice.h"
#include "play.h"
#include "misc.h"


const char* V_PATH="../share/rvoice/rc/";

const char* V_SEKUND = "sekund.wav";
const char* V_MINUT = "minut.wav";
const char* V_CHASOV =  "chasov.wav";
const char* V_SEKUNDA = "sekunda.wav";
const char* V_MINUTA = "minuta.wav";
const char* V_CHAS =  "chas.wav";
const char* V_SEKUNDY = "sekundy.wav";
const char* V_MINUTY = "minuty.wav";
const char* V_CHASA =  "chasa.wav";

const char* V_ZERO = "nolj.wav";
const char* V_ODIN = "odin.wav";
const char* V_ODNA = "odna.wav";
const char* V_DVA = "dva.wav";
const char* V_DVE = "dve.wav";
const char* V_THREE = "tri.wav";
const char* V_FOUR = "chetyre.wav";
const char* V_FIVE = "pyat.wav";
const char* V_SIX = "shestj.wav";
const char* V_SEVEN = "semj.wav";
const char* V_EIGHT = "vosemj.wav";
const char* V_NINE = "devyatj.wav";

const char* V_TEN = "desyatj.wav";
const char* V_ELEVEN = "odinnadcatj.wav";
const char* V_TWELVE = "dvenadcatj.wav";
const char* V_THIRTEEN = "trinadcatj.wav";
const char* V_FOURTEEN = "chetyrnadcatj.wav";
const char* V_FIFTEEN = "pyatnadcatj.wav";
const char* V_SIXTEEN = "shestnadcatj.wav";
const char* V_SEVENTEEN = "semnadcatj.wav";
const char* V_EIGHTEEN = "vosemnadcatj.wav";
const char* V_NINETEEN = "devyatnadcatj.wav";

const char* V_TWENTY = "dvadcatj.wav";
const char* V_THIRTY = "tridcatj.wav";
const char* V_FOURTY = "sorok.wav";
const char* V_FIFTY = "pyatdesyat.wav";


voices voice={NULL, NULL};

/**
 * создает и добавляет в список следующее
 * имя файла для воспроизведения.
 */
void allocate_voice(char* f)
{
voices* tmp=&voice;
voices* v;

v=(voices*)malloc(sizeof(voices));
if(v==NULL)
  {
  syslog(LOG_ERR, "Cannot allocate memory for voice\n");
  go_out();
  };

while(tmp->next!=NULL)
     {
     tmp=tmp->next;
     };

tmp->next=v;
v->file=f;
v->next=NULL;
}


void add_time_part(char numb[2], char type, char* unit0, char* unit1, char* unit2)
{
char* unit;
// десятки
switch(numb[0])
      {
      case '0': 
                switch(numb[1])
                      {
                      case '0':
                               allocate_voice((char*)V_ZERO);
                               unit=unit0;
                               break;
                      case '1':
                               if(type)
                                 {
                                 allocate_voice((char*)V_ODIN);
                                 }
                               else
                                 {
                                 allocate_voice((char*)V_ODNA);
                                 };
                               unit=unit1;
                               break;
                      case '2':
                               if(type)
                                 {
                                 allocate_voice((char*)V_DVA);
                                 }
                               else
                                 {
                                 allocate_voice((char*)V_DVE);
                                 };
                               unit=unit2;
                               break;
                      case '3':
                               allocate_voice((char*)V_THREE);
                               unit=unit2;
                               break;
                      case '4':
                               allocate_voice((char*)V_FOUR);
                               unit=unit2;
                               break;
                      case '5':
                               allocate_voice((char*)V_FIVE);
                               unit=unit0;
                               break;
                      case '6':
                               allocate_voice((char*)V_SIX);
                               unit=unit0;
                               break;
                      case '7':
                               allocate_voice((char*)V_SEVEN);
                               unit=unit0;
                               break;
                      case '8':
                               allocate_voice((char*)V_EIGHT);
                               unit=unit0;
                               break;
                      case '9':
                               allocate_voice((char*)V_NINE);
                               unit=unit0;
                               break;
                      default:
                               syslog(LOG_ERR, "Error parsing time\n");
                               go_out();
                               break;
                      };
                break;
      case '1':
                unit=unit0;
                switch(numb[1])
                      {
                      case '0':
                               allocate_voice((char*)V_TEN);
                               break;
                      case '1':
                               allocate_voice((char*)V_ELEVEN);
                               break;
                      case '2':
                               allocate_voice((char*)V_TWELVE);
                               break;
                      case '3':
                               allocate_voice((char*)V_THIRTEEN);
                               break;
                      case '4':
                               allocate_voice((char*)V_FOURTEEN);
                               break;
                      case '5':
                               allocate_voice((char*)V_FIFTEEN);
                               break;
                      case '6':
                               allocate_voice((char*)V_SIXTEEN);
                               break;
                      case '7':
                               allocate_voice((char*)V_SEVENTEEN);
                               break;
                      case '8':
                               allocate_voice((char*)V_EIGHTEEN);
                               break;
                      case '9':
                               allocate_voice((char*)V_NINETEEN);
                               break;
                      default:
                               syslog(LOG_ERR, "Error parsing time\n");
                               go_out();
                               break;
                      };
                break;
      default : 
                // десятки
                switch(numb[0])
                      {
                      case '2': 
                                allocate_voice((char*)V_TWENTY);
                                break;
                      case '3':
                                allocate_voice((char*)V_THIRTY);
                                break;
                      case '4':
                                allocate_voice((char*)V_FOURTY);
                                break;
                      case '5':
                                allocate_voice((char*)V_FIFTY);
                                break;
                      default :
                                syslog(LOG_ERR, "Error parsing time\n");
                                go_out();
                                break;
                      };
                // единицы
                switch(numb[1])
                      {
                      case '0':
                               unit=unit0;
                               break;
                      case '1':
                               if(type)
                                 {
                                 allocate_voice((char*)V_ODIN);
                                 }
                               else
                                 {
                                 allocate_voice((char*)V_ODNA);
                                 };
                               unit=unit1;
                               break;
                      case '2':
                               if(type)
                                 {
                                 allocate_voice((char*)V_DVA);
                                 }
                               else
                                 {
                                 allocate_voice((char*)V_DVE);
                                 };
                               unit=unit2;
                               break;
                      case '3':
                               allocate_voice((char*)V_THREE);
                               unit=unit2;
                               break;
                      case '4':
                               allocate_voice((char*)V_FOUR);
                               unit=unit2;
                               break;
                      case '5':
                               allocate_voice((char*)V_FIVE);
                               unit=unit0;
                               break;
                      case '6':
                               allocate_voice((char*)V_SIX);
                               unit=unit0;
                               break;
                      case '7':
                               allocate_voice((char*)V_SEVEN);
                               unit=unit0;
                               break;
                      case '8':
                               allocate_voice((char*)V_EIGHT);
                               unit=unit0;
                               break;
                      case '9':
                               allocate_voice((char*)V_NINE);
                               unit=unit0;
                               break;
                      default:
                               syslog(LOG_ERR, "Error parsing time\n");
                               go_out();
                               break;
                      };
                break;
      };

allocate_voice(unit);
//syslog(LOG_INFO, "Added %c %c %s", numb[0], numb[1], unit);
}


char* fill_unit(char* tm, char unit[2])
{
char* tmp;
int pos;
int i;

unit[0]=0x00;
unit[1]=0x00;

tmp=strchr(tm,':');
if(tmp==NULL)
  {
  tmp=tm+strlen(tm);
  };

pos=tmp-tm;

//syslog(LOG_WARNING, "digits: pos=%d, string=%s\n", pos, tm);
if(pos>2)
  {
  syslog(LOG_ERR, "Error while parsing time string: %s\n", tm);
  go_out();
  };

//strncpy(unit, tmp, len);
for(i=0;i<pos;i++)
   {
   unit[2-i-1]=tm[pos-i-1];
   };

//syslog(LOG_INFO, "result: %x %x\n", unit[1], unit[0]);
tmp=tmp+1;
return(tmp);
}

/**
 * потрошит строку времени
 */
void parse_time(char* time)
{
char unit[2];

char* pnt=time;

pnt=fill_unit(pnt, unit);
add_time_part(unit, 1, (char*)V_CHASOV, (char*)V_CHAS, (char*)V_CHASA);

pnt=fill_unit(pnt, unit);
add_time_part(unit, 0, (char*)V_MINUT, (char*)V_MINUTA, (char*)V_MINUTY);

pnt=fill_unit(pnt, unit);
add_time_part(unit, 0, (char*)V_SEKUND, (char*)V_SEKUNDA, (char*)V_SEKUNDY);

}


/**
 * очищает список файлов голоса
 */
void free_voices()
{
voices* it;
voices* nxt;

it=&voice;
nxt=it->next;
do
  {
  it=nxt;
  if(it!=NULL)
    {
    nxt=it->next;
    free(it);
    };
  } while(nxt!=NULL);

voice.file=NULL;
voice.next=NULL;
}


/**
 *
 * playing voice files
 *
 */
int play_voice(char* text)
{
char tmpbuf[256];
voices* tmp=&voice;
syslog(LOG_INFO, "Время %s\n", text);
parse_time(text);
tmp=tmp->next;
while(tmp!=NULL)
     {
     tmpbuf[0]=0x00;
     strncat(tmpbuf, V_PATH, 40);
     strncat(tmpbuf, tmp->file, 215);
     play_wav(tmpbuf);
     tmp=tmp->next;
     };
free_voices();
return(0);
}
