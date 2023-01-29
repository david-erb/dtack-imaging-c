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
 * This is the complete list of all xmsif routines used by dt.
 */

#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMMlibinit;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMMcoreleft;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMMallcoreleft;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMMgetversion;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMMalloc;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMMrealloc;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMMfree;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) _XMMcopy;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) xmsif_vers_vers;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) xmsif_vers_date;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) xmsif_vers_time;

#endif

#include <dtack/nofar.h>
#ifndef DTDEPEND
#  include <xmsif.h>
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
