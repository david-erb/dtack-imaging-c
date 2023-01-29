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
  CHANGES
  20000630 DE changed wrong MAJOR to MINOR
  20000411 DE added dtrev_base function
  19990102 DE added dbg calls
 *..........................................................................*/


/*..........................................................................
| TITLE dtrev()
|dxinclude(liblink1.dx)
| 
|!SUMMARY
| Utility functions for printing program and library rev levels.
|
|.Salient Concepts
| Each program and library has a rev method.
| You pass one or more function pointers to these utility functions.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/rev $RCSfile: rev.c,v $ $Revision: 1.2 $");
#include <dtack/rev.h>

/*..........................................................................
| NAME
| dtrev_base - base library revision
| 
| SUMMARY
| Return base library revision.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtrev_base(
  dt_ctl_t *ctl,
  const char **rev_string,
  int *rev_major,
  int *rev_minor,
  long *rev_date)
{
  if (rev_string != NULL)
    *rev_string = "Dtack base library";
  if (rev_major != NULL)
    *rev_major = (int)atol(DT_VERSION_MAJOR);
  if (rev_minor != NULL)
    *rev_minor = (int)atol(DT_VERSION_MINOR);
  if (rev_date != NULL)
    *rev_date = atol(DT_VERSION_YEAR DT_VERSION_MONTH DT_VERSION_DAY);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtrev_say1 - say one rev
| 
| SUMMARY
| Call the given rev function.
| Format the result and call dt_say().
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtrev_say1(								/* say one rev						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtrev_f *rev)
{
  DT_F("dtrev_say1");
  const char *rev_string;
  int rev_major;
  int rev_minor;
  long rev_date;

  DT_Q((*rev),(ctl,
    &rev_string,
    &rev_major, &rev_minor,
    &rev_date));

  dt_say(ctl, NULL,
    "%-20s %02d.%02d %d",
    rev_string,
    rev_major, rev_minor,
    rev_date);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtrev_say - say revs in list
| 
| SUMMARY
| Call the given rev functions.
| The list should be NULL terminated.
| Format the results and call dt_say() for each one.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtrev_say(								/* say one rev						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtrev_f **rev)
{
  DT_F("dtrev_say");
  while (*rev != NULL)
    DT_Q(dtrev_say1,(ctl, *rev++));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtrev_dbg1 - dbg one rev
| 
| SUMMARY
| Call the given rev function.
| Format the result and call dt_dbg().
| 
| END
 *..........................................................................*/

void
DTCONFIG_API1
dtrev_dbg1(								/* dbg one rev						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  unsigned long mask,
  dtrev_f *rev)
{
  DT_F("dtrev_dbg1");
  const char *rev_string;
  int rev_major;
  int rev_minor;
  long rev_date;
  dt_rc_e rc;

  DT_C((*rev),(ctl,
    &rev_string,
    &rev_major, &rev_minor,
    &rev_date));

  dt_dbg(ctl, NULL, mask,
    "%-20s %02d.%02d %d",
    rev_string,
    rev_major, rev_minor,
    rev_date);

cleanup:
  return;
}

/*..........................................................................
| NAME
| dtrev_dbg - dbg revs in list
| 
| SUMMARY
| Call the given rev functions.
| The list should be NULL terminated.
| Format the results and call dt_dbg() for each one.
| 
| END
 *..........................................................................*/

void
DTCONFIG_API1
dtrev_dbg(								/* dbg list of revs					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  unsigned long mask,
  dtrev_f **rev)
{
  DT_F("dtrev_dbg");
  if (DT_DBG(ctl, mask))				/* debug enabled? 					*/
  while (*rev != NULL)
    dtrev_dbg1(ctl, mask, *rev++);
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
