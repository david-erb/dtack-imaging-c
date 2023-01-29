/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#ifdef DTCONFIG_WATCOM_DOS
  
/* 
 * This library uses the Microsoft C calling conventions.
 * This is the complete list of all svgacc20 routines used by dt.
 */

#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) whichcpu;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) whichmem;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) setcard;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) videomodeget;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) whichvga;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) res320;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) res640l;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) res640;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) res800;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) res1024;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) res1280;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) videomodeset;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) blkput;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) blkget;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) palset;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) drwcircle;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) fillarea;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) fillscreen;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) whichmouse;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) mouseenter; 
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) mousestatus;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) mouselocset;

#endif

#ifndef DTDEPEND
#include <svgacc.h>
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
