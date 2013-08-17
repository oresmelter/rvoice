#include "project.h"
static int plen;		/* длина образца */
const char *pattern;			/* образец (что искать) */
static int d[256];		/* таблица сдвигов; в алфавите ASCII - * 256 букв. */
/* расстояние от конца образца до позиции i в нем */
#define DISTANCE(i)     ((plen-1) - (i))
/* -------------------------------------------------------------------------------------- */
// выводит число в двоичном виде
int printb(u_int32_t n)
{
    register int i;

    for (i = 8 * sizeof(n) - 1; i >= 0; --i)
	putchar(n & (1 << i) ? '1' : '0');

    return 0;
}
/* -------------------------------------------------------------------------------------- */
// выводит число с любым основанием (base)
int printi(int n, unsigned int base)
{
    register int i;

    if (n < 0) {
	putchar('-');
	n = -n;
    }
    if (i = n / base)
	printi(i, base);
    i = n % base;
    putchar(i >= 10 ? 'A' + i - 10 : '0' + i);
    
    return 0;
}
/* -------------------------------------------------------------------------------------- */
unsigned long ulReverse(unsigned long in)
{
    unsigned long out;
    unsigned char tmp[4];
//    int i;
//    printb(in);puts("->");
    tmp[0] = 0x000000ff & in;
    tmp[1] = (0x0000ff00 & in) >> 8;
    tmp[2] = (0x00ff0000 & in) >> 16;
    tmp[3] = (0xff000000 & in) >> 24;

    out = (tmp[0] << 24) | (tmp[1] << 16) | tmp[2] << 8 | tmp[3];
//    printb(out);puts(" \n");
    return out;
}
/*--------------------------------------------------------------------------*/
int indexBM(const char *str)
{				/* в чем искать */
    int slen = strlen(str);	/* длина строки */
    register int pindx;		/* индекс сравниваемой буквы в образце */
    register int cmppos;	/* индекс сравниваемой буквы в строке  */
    register int endpos;	/* позиция в строке, к которой "приставляется"
				 * последняя буква образца */

    /* пока образец помещается в остаток строки */
    for (endpos = plen - 1; endpos < slen;) {

	/* просмотр образца от конца к началу */
	for (cmppos = endpos, pindx = (plen - 1);
	     pindx >= 0; cmppos--, pindx--)

	    if (str[cmppos] != pattern[pindx]) {
		endpos += d[str[endpos] & 0377];
		break;		/* & 0377 подавляет расширение знака. Еще  */
	    }
	/* можно сделать все char -> unsigned char */
	if (pindx < 0)
	    return (endpos - (plen - 1));
	/* Нашел: весь образец вложился */
    }
    return (-1);		/* Не найдено */
}

/*--------------------------------------------------------------------------*/
/* Разметка таблицы сдвигов */
void compilePatternBM(const char *ptrn)
{
    register int c;

    pattern = ptrn;
    plen = strlen(ptrn);

    /* c - номер буквы алфавита */
    for (c = 0; c < 256; c++)
	d[c] = plen;
    /* сдвиг на длину всего образца */

    /* c - позиция в образце */
    for (c = 0; c < plen - 1; c++)
	d[pattern[c] & 0377] = DISTANCE(c);
}

/*--------------------------------------------------------------------------*/
int strindex(const char *s, const char *pat)
{
    compilePatternBM(pat);
    return indexBM(s);
}

/*------------------------------------------------------------------------*/
void squeeze(char s1[], char s2[])
{
    int i, j, k;
    for (i = k = 0; s1[i] != '\0'; i++) {
	for (j = 0; s2[j] != '\0' && s2[j] != s1[i]; j++);
	if (s2[j] == '\0')
	    s1[k++] = s1[i];
    }
    s1[k] = '\0';
}

/*-----------------------------------------------------------------------*/
void reverse(char *s)
{
    int c;
    char *t;
    for (t = s + (strlen(s) - 1); s < t; s++, t--) {
	c = *s;
	*s = *t;
	*t = c;
    }
}

/*-----------------------------------------------------------------------*/
void itoa(char s[], int n)
{
    int sign;
    char *t = s;
    if ((sign = n) < 0)
	n = -n;
    do {
	*s++ = n % 10 + '0';
    } while ((n /= 10) != 0);
    if (sign < 0)
	*s++ = '-';
    *s = '\0';
    reverse(t);
}
/*-----------------------------------------------------------------------*/
void strNcpy(char *s, const char *t, int n)
{
    int i;
    for (i = n; --i >= 0;)
	if(*s != '\n') *s++ = *t++;
    *s++ = '\0';
}
/*-----------------------------------------------------------------------*/
void strMcpy(char *s, const char *t)
{
// убираем кавычки
    while(*t)
	if((*t != '\n') || (*t != 0x027)) {
	    *s++ = *t++;
	}
    *s++ = '\0';
}
/*-----------------------------------------------------------------------*/
const char *convert(const char *s, const char *from_cp, const char *to_cp)
{
    iconv_t ic = iconv_open(to_cp, from_cp);
    char *out_buf, *out, *in;
    size_t in_ln, out_ln, k;

    if (ic == (iconv_t) (-1)) {
	fprintf(stderr, "iconv: cannot convert from %s to %s\n", from_cp,
		to_cp);
	return "";
    }

    out_buf = (char *) calloc(strlen(s) + 1, 1);
    out = out_buf;
    in = (char *) malloc(strlen(s) + 1);
    strcpy(in, s);

    in_ln = strlen(s);
    out_ln = in_ln;
    k = iconv(ic, &in, &in_ln, &out, &out_ln);
    if (k == (size_t) - 1)
	fprintf(stderr, "iconv: %u of %d converted\n", (unsigned int)k, (unsigned int)strlen(s));

    if (errno != 0)
	fprintf(stderr, "iconv: %s\n", strerror(errno));

    iconv_close(ic);

    return out_buf;
}
/* --------------------------------------------------------------------- */
const char *eatblanks(const char *str)
{
    char *out;
    int len;
    reverse(str);
    while (isblank(*str))
//    while(!isalnum(*str))
	str++; 
    len = strlen(str);
    out = (char *) malloc(len + 1);// точки!!
    if(!isalnum(str[len])) {
	memmove(out, str, len-1);
	out[len-1] = '\0';
    } else { 
	memmove(out, str, len);
	out[len] = '\0';
    }
    reverse(out);
    return strdup(out);
}
/* ------------------------------------------------------------------------- */
int random_range(unsigned const low, unsigned const high) // хай-ло
{
    unsigned const range = high - low + 1;
    return low + (int) (((double) range) * rand() / (RAND_MAX + 1.0));
}
/* ------------------------------------------------------------------------- */
unsigned short checksum(void *b, int len)
{
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2)
	sum += *buf++;
    if (len == 1)
	sum += *(unsigned char *) buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}
/* ------------------------------------------------------------------------- */
const char *configread(const char *conf, const char *val)
{
    FILE *fp;
    char line[256];
    char RS[64];

    if ((fp = fopen(conf, "r")) == NULL) {
		printf("Не открывается %s \n", conf);
		return NULL;
    }

    while (fgets(line, 256, fp) != NULL) {
		char *pwnd;
		char buff[256];
		int len, k;
	
		if (line[0] == '#' || line[0] == '\n')
			continue;

		if (line[0] == '[' || line[0] == '\n')
			continue;
	
		len = strlen(line);//		printf("%s", line); fflush(stdout);
		k = strindex(line, "=");
		if(!k) continue;
		if(!strstr(line, val)) continue;
		strNcpy(buff, line, k);//printf("%d, %s\n", k, buff); fflush(stdout);
		if (strcmp(buff, val) == 0) {
			len = strlen(line + k + 1);
			memmove(RS, line + k + 1, len);//RS[len] = '\0'; printf("%s", RS); fflush(stdout);
			break;
		}
		pwnd[0] = '\0'; //	pwnd = '\0';		bzero(buff, sizeof(buff));
    }
    fclose(fp);
    bzero(line, sizeof(line));

    return strdup(RS);
}
/* ------------------------------------------------------------------------- *
 * Набор библиотечных функций для бифоркации и перевода приложения в фоновый * 
 * режим работы                                                              *
 *                                                            Цветков АН ©   *
 * ------------------------------------------------------------------------- */
#include "utils.h"
/* ------------------------------------------------------------------------ */
       /* closeall() -- close all FDs >= a specified value */
void closeall(int fd)
{
    int fdlimit = sysconf(_SC_OPEN_MAX);

    while (fd < fdlimit)
	close(fd++);
}

/* ------------------------------------------------------------------------ */
       /* daemon() - detach process from user and disappear into the
          background       * returns -1 on failure, but you can't do much except
          exit in that case       * since we may already have forked. This is
          based on the BSD version,       * so the caller is responsible for
          things like the umask, etc.       */

       /* believed to work on all Posix systems */

int dodaemon(int nochdir, int noclose)
{
    switch (fork()) {
    case 0:
	break;
    case -1:
	return -1;
    default:
	_exit(0);		/* exit the original process
				 */
    }

    if (setsid() < 0)		/* shoudn't fail */
	return -1;

    /* dyke out this switch if you want to acquire a control tty
       in *//* the future -- not normally advisable for daemons */

    switch (fork()) {
    case 0:
	break;
    case -1:
	return -1;
    default:
	_exit(0);
    }

    if (!nochdir)
	chdir("/");

    if (!noclose) {
	closeall(0);
	open("/dev/null", O_RDWR);
	dup(0);
	dup(0);
    }

    return 0;
}

/* ------------------------------------------------------------------------ */
       /* fork2() -- like fork, but the new process is immediately
          orphaned       *            (won't leave a zombie when it exits)
          * Returns 1 to the parent, not any meaningful pid.
          * The parent cannot wait() for the new process (it's unrelated).
        */

       /* This version assumes that you *haven't* caught or ignored
	         SIGCHLD. *//* If you have, then you should just be using fork()
	         instead anyway.  */

/* ------------------------------------------------------------------------ */
int fork2()
{
    pid_t pid;
    int status;

    if (!(pid = fork())) {
	switch (fork()) {
	case 0:
	    return 0;
	case -1:
	    _exit(errno);	/* assumes all errnos are <256
				 */
	default:
	    _exit(0);
	}
    }

    if (pid < 0 || waitpid(pid, &status, 0) < 0)
	return -1;

    if (WIFEXITED(status))
	if (WEXITSTATUS(status) == 0)
	    return 1;
	else
	    errno = WEXITSTATUS(status);
    else
	errno = EINTR;		/* well, sort of :-) */

    return -1;
}

/* ------------------------------------------------------------------------ */
