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

#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/ipc.h>
#include <dtack/qnx4.h>

DT_RCSID("source/low $RCSfile: qnx4.c,v $ $Revision: 1.1 $");

#ifdef DTCONFIG_IMPLIED_QNX4

#ifndef DTDEPEND
#include <sys/qnxterm.h>
#endif

#define MSG_MAX (16384)

/*..........................................................................
| NAME
| dtqnx4_out_string
| 
| DESCRIPTION
| Put string into window.
| 
| RETURN VALUES
| None.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

void
DTCONFIG_API1
dtqnx4_out_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
{
  int i;
  if (string == NULL)
    return;
  if (length == -1)						/* caller has not given length?		*/
    length = strlen(string);
  for (i=0; i<length; i++)
  {
	if (string[i] == '\n')
      term_scroll_up();					/* puts cursor at lower left 		*/
	else
      term_type(-1, -1, string+i, 1,	/* output the string here		 	*/
        TERM_NORMAL);
  }
}

/*..........................................................................*/
void
DTCONFIG_API1
dtqnx4_dbg_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
{
  if (string == NULL)
    return;
  if (length == -1)						/* caller has not given length?		*/
    length = strlen(string);
  if (DTQNX4_TERM(ctl))
    dtqnx4_out_string(ctl, string, length);
  else
  {
	fwrite(string, 1, length, stderr);
	fflush(stderr);
  }
}

/*..........................................................................*/
void
DTCONFIG_API1
dtqnx4_err_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
{
  if (string == NULL)
    return;
  if (length == -1)						/* caller has not given length?		*/
    length = strlen(string);
  if (DTQNX4_TERM(ctl))
    dtqnx4_out_string(ctl, string, length);
  else
  {
	fwrite(string, 1, length, stderr);
	fflush(stderr);
  }
}

/*..........................................................................*/
void
DTCONFIG_API1
dtqnx4_say_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
{
  if (string == NULL)
    return;
  if (length == -1)						/* caller has not given length?		*/
    length = strlen(string);
  if (DTQNX4_TERM(ctl))					/* qnx is in TERM mode? 			*/
    dtqnx4_out_string(ctl, string, length);
  else
  {
	fwrite(string, 1, length, stdout);
	fflush(stdout);
  }
}
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
