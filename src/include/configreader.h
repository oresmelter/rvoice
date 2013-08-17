/* ------------------------------------------------------------------------- *
 * Header life for configreader                                              *
 *                                                            Цветков АН ©   *
 * ------------------------------------------------------------------------- */

typedef struct plist {
    struct plist *next;
    unsigned char *name;
    unsigned char *value;
    int type;
} PLIST;
#define  CONFIG_TOKEN_STRING    1
#define  CONFIG_TOKEN_DEC   	2

int configreader(const char *filename);
char *config_getval(char *name);
PLIST *config_get(char *name);
void config_die(int i);		//N error from configreader
void readgateconfig();
