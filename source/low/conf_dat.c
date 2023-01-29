/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/config.h>

const char *dtconfig = DTCONFIG;
const char *dtconfig_version = "1.0.0";
/* ........................................................................	*/
const int dtconfig_implied_sunos =
#ifdef DTCONFIG_IMPLIED_SUNOS
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_implied_linux =
#ifdef DTCONFIG_IMPLIED_LINUX
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_implied_qnx4 =
#ifdef DTCONFIG_IMPLIED_QNX4
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_implied_qnx416 =
#ifdef DTCONFIG_IMPLIED_QNX416
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_implied_qnx432 =
#ifdef DTCONFIG_IMPLIED_QNX432
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_implied_dos =
#ifdef DTCONFIG_IMPLIED_DOS
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_implied_dos16 =
#ifdef DTCONFIG_IMPLIED_DOS16
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_implied_dos32 =
#ifdef DTCONFIG_IMPLIED_DOS32
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_implied_msw =
#if defined(DTCONFIG_IMPLIED_MSW) || defined(DTCONFIG_IMPLIED_MSW32)
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_implied_msw16 =
#ifdef DTCONFIG_IMPLIED_MSW16
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_implied_msw32 =
#ifdef DTCONFIG_IMPLIED_MSW32
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_implied_unixlike =
#ifdef DTCONFIG_IMPLIED_UNIXLIKE
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_watcom =
#ifdef DTCONFIG_WATCOM
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_borland =
#ifdef DTCONFIG_BORLAND
  1;
#else
  0;
#endif
/* ........................................................................	*/
const int dtconfig_microsoft =
#ifdef DTCONFIG_MICROSOFT
  1;
#else
  0;
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
