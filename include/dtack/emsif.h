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
 * This is the complete list of all emsif routines used by dt.
 */

#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) EMMlibinit;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) EMMcoreleft;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) EMMalloc;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) EMMrealloc;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) EMMfree;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) EMMcopyto;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) EMMcopyfrom;

#endif

#include <dtack/nofar.h>
#ifndef DTDEPEND
#  include <emsif.h>
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
