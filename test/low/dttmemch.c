/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*
 */

#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <memcheck.h>

#define DT_F(NAME) const char *F = NAME
#define DT_RC_GOOD 0
#define DT_RC_BAD  1
#define dt_rc_e int

/*..........................................................................
 *..........................................................................*/

void
dttmemcheck_debug(
  const char *caller,
  const char *format,
  ...)
{
  va_list arg;
  printf("%s: ", caller);
  va_start(arg, format);
  vprintf(format, arg);
  printf("\n");
  va_end(arg);
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttmemcheck_err(
  const char *f,
  const char *funcname,
  int error)
{
  dttmemcheck_debug(f,
    "dttmemcheck error 0x%02x in %s",
    error, funcname);

  return DT_RC_BAD;
}

/*..........................................................................
 *..........................................................................*/

int
main(
  int argc,
  char *argv[])
{
  free(malloc(2));
  {
	char upper[64];
	strcpy(upper, "DT_");
  }
  return 0;
}






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
