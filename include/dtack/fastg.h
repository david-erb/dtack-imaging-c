/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#ifdef NONO
#ifdef DTCONFIG_WATCOM_DOS
  
/* 
 * This library uses the Microsoft C calling conventions.
 * This is more than the complete list of all fastgraf routines used by dt.
 */

#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_bestmode;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_erase;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_fillpage;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_getblock;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_getdacs;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_getimage;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_getmaxx;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_getmaxy;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_getmode;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_mousebut;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_mousecur;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_mousefin;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_mouseini;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_mouselim;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_mousemov;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_mousepos;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_mouseptr;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_mousespd;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_mousevis;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_move;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_moverel;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_putblock;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_putimage;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_reset;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_setdacs;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_setmode;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_setcolor;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_svgainit;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_svgastat;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_svgaver;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_testmode;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT)   fg_waitkey;
#endif
#endif

#ifndef DTDEPEND
#include <fastgraf.h>
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
