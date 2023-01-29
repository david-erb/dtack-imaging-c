/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*..........................................................................
| NAME
| Introduction - intro
|
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/config.h>

#if DTPRJ_FASTGRAPH_FAKE == DT_1
#  include <dtack/nofar.h>
#  ifndef DTDEPEND
#    include <fastgraf.h>
#  endif

#  define EMPTY_INT {return 0;}
#  define EMPTY_VOID {}

int    fg_bestmode(int a, int b, int c) EMPTY_INT
void   fg_erase(void) EMPTY_VOID
void   fg_fillpage(void) EMPTY_VOID
void   fg_getblock(char DTCONFIG_FAR *a, int b, int c, int d, int e) EMPTY_VOID
void   fg_getdacs(int a, int b, char *c) EMPTY_VOID
void   fg_getimage(char *a, int b, int c) EMPTY_VOID
int    fg_getmaxx(void) EMPTY_INT
int    fg_getmaxy(void) EMPTY_INT
int    fg_getmode(void) EMPTY_INT
void   fg_mousebut(int a, int *b, int *c, int *d) EMPTY_VOID
void   fg_mousecur(int a, int b) EMPTY_VOID
void   fg_mousefin(void) EMPTY_VOID
int    fg_mouseini(void) EMPTY_INT
void   fg_mouselim(int a, int b, int c, int d) EMPTY_VOID
void   fg_mousemov(int a, int b) EMPTY_VOID
void   fg_mousepos(int *a, int *b, int *c) EMPTY_VOID
void   fg_mouseptr(int *a, int b, int c) EMPTY_VOID
void   fg_mousespd(int a, int b) EMPTY_VOID
void   fg_mousevis(int a) EMPTY_VOID
void   fg_move(int a, int b) EMPTY_VOID
void   fg_moverel(int a, int b) EMPTY_VOID
void   fg_putblock(char DTCONFIG_FAR *a, int b, int c, int d, int e) EMPTY_VOID
void   fg_putimage(char * a, int b, int c) EMPTY_VOID
void   fg_reset(void) EMPTY_VOID
void   fg_setdacs(int a, int b, char *c) EMPTY_VOID
void   fg_setmode(int a) EMPTY_VOID
void   fg_setcolor(int a) EMPTY_VOID
int    fg_svgainit(int b) EMPTY_INT
int    fg_svgastat(void) EMPTY_INT
void   fg_svgaver(int *a, int *b) EMPTY_VOID
int    fg_testmode(int a, int b) EMPTY_VOID
void   fg_waitkey(void) EMPTY_VOID
#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
