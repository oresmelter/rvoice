/********************************************************************
* Description: Определения и функции для работы с терминальным цветом
* Author: Tsvetkov A.N. <oresmelter@mail.ru>
* Created at: Sun Oct  2 10:55:28 MSD 2005
* Computer: iru 
* System: Linux 2.4.22-1.2154.nptl.asp on i686
*    
* Copyright (c) 2005 root  All rights reserved.
*
********************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

#include "color.h"

COL_POINTER stzSETCOLOR_GREY = "\033[1;30m";
COL_POINTER stzSETCOLOR_RED = "\033[1;31m";
COL_POINTER stzSETCOLOR_GREEN = "\033[1;32m";
COL_POINTER stzSETCOLOR_YELLOW = "\033[1;33m";
COL_POINTER stzSETCOLOR_BLUE = "\033[1;34m";
COL_POINTER stzSETCOLOR_MAGENTA = "\033[1;35m";
COL_POINTER stzSETCOLOR_CIAN = "\033[1;36m";
COL_POINTER stzSETCOLOR_WHITE = "\033[1;37m";
COL_POINTER stzSETCOLOR_WHITE2 = "\033[1;38m";
COL_POINTER stzSETCOLOR_NORMAL = "\033[0;39m";
// terminal background to set color : black)
COL_POINTER stzBG_BLACK = "\033[1;40m";
// terminal background color : red)
COL_POINTER stzBG_RED = "\033[1;41m";
// terminal background to set color : green)
COL_POINTER stzBG_GREEN = "\033[1;42m";
// terminal background to set color : brown)
COL_POINTER stzBG_BROWN = "\033[1;43m";
// terminal background to set color : blue)
COL_POINTER stzBG_CIAN = "\033[1;44m";
// terminal background to set color : magenta)
COL_POINTER stzBG_MAGENTA = "\033[1;45m";
// terminal background to set color : cian)
COL_POINTER stzBG_BLUE = "\033[1;46m";
// terminal background to set color : white)
COL_POINTER stzBG_WHITE = "\033[1;47m";

/* ------------------------------------------------------------------------- */
void set_term_color(COL_POINTER stzColor)
{
    if (isatty(fileno(stderr)))
	fprintf(stderr, "%s", stzColor);
}

/* ------------------------------------------------------------------------- */
void clrscr()
{
    set_term_color("\033[2J");
    set_term_color("\033[0;0f");
}

/* ------------------------------------------------------------------------- */
