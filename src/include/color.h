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
#ifndef COLOR_H
#define COLOR_H

#ifdef __cplusplus
extern "C" {
#endif

#define COL_POINTER char*
//#define COL_POINTER char*

    extern COL_POINTER stzSETCOLOR_GREY;
    extern COL_POINTER stzSETCOLOR_RED;
    extern COL_POINTER stzSETCOLOR_GREEN;
    extern COL_POINTER stzSETCOLOR_YELLOW;
    extern COL_POINTER stzSETCOLOR_CIAN;
    extern COL_POINTER stzSETCOLOR_MAGENTA;
    extern COL_POINTER stzSETCOLOR_BLUE;
    extern COL_POINTER stzSETCOLOR_WHITE;
    extern COL_POINTER stzSETCOLOR_WHITE2;
    extern COL_POINTER stzSETCOLOR_NORMAL;
// terminal background to set color : black)
    extern COL_POINTER stzBG_BLACK;
// terminal background color : red)
    extern COL_POINTER stzBG_RED;
// terminal background to set color : green)
    extern COL_POINTER stzBG_GREEN;
// terminal background to set color : brown)
    extern COL_POINTER stzBG_BROWN;
// terminal background to set color : blue)
    extern COL_POINTER stzBG_CIAN;
// terminal background to set color : magenta)
    extern COL_POINTER stzBG_MAGENTA;
// terminal background to set color : cian)
    extern COL_POINTER stzBG_BLUE;
// terminal background to set color : white)
    extern COL_POINTER stzBG_WHITE;
/* ------------------------------------------------------------------------- */
    void set_term_color(COL_POINTER stzColor);
/* ------------------------------------------------------------------------- */
    void clrscr();
/* ------------------------------------------------------------------------- */
#endif

#ifdef __cplusplus
}
#endif
