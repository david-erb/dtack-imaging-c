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
 * This is the complete list of all xmslb routines used by dt.
 */

#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMS_Setup;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMS_FreeMem;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMS_Version;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMS_AllocEMB;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMS_FreeEMB;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMS_LockEMB;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMS_UnlockEMB;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMS_MoveEMB;
#  pragma aux (DTCONFIG_WATCOM_DOS_MICROSOFT) XMS_ReallocEMB;

#endif

#include <dtack/nofar.h>
#ifndef DTDEPEND
#  include <xmslib.h>
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
