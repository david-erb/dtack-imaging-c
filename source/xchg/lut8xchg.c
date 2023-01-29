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
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: lut8.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/ipc.h>
#include <dtack/lut8.h>

#include <dtack/xchg.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlut8_export(							/* export lut to file or window		*/
  dt_ctl_t *ctl,
  const dtlut8_t *lut,
  const char *spec,
  int start,
  int count)
{
  DT_F("dtlut8_export");
  dtxchg_e which;
  char *server = NULL;
  char *id = NULL;
  dt_rc_e rc;

  {
	char message[256];
	DT_Q(dtxchg_parse,(ctl, spec,		/* figure out which exchanger	    */
      &which, &server, &id,
      NULL, message, sizeof(message)));
	if (message[0] != '\0')
      return dt_err(ctl, F, "%s", 
        message);
  }

  switch(which)
  { 
    case DTXCHG_WIN:					/* ................................	*/
    {
      DT_C(dtxchg_win_export_lut8,		/* output to window server			*/
        (ctl, server, id, lut,
         start, count));
	} break;
    default:
	{
	  rc = dt_err(ctl, F,
        "cannot export lut"
        " to \"%s\" (type %s)",
        spec, 
        which < DTXCHG_END_OF_LIST?
          dtxchg_choices[which]:
          "unknown");
	}
  }
 
cleanup:
  if (server != NULL)
    DT_I(dtos_free2,(ctl, 
     server, F, "server"));
  if (id !=  NULL)
    DT_I(dtos_free2,(ctl, 
     id, F, "id"))

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlut8_import(							/* import lut to file or window		*/
  dt_ctl_t *ctl,
  dtlut8_t *lut,
  const char *spec,
  int start,
  int count)
{
  DT_F("dtlut8_export");
  dtxchg_e which;
  char *server = NULL;
  char *id = NULL;
  dt_rc_e rc;

  {
	char message[256];
	DT_Q(dtxchg_parse,(ctl, spec,		/* figure out which exchanger	    */
      &which, &server, &id,
      NULL, message, sizeof(message)));
	if (message[0] != '\0')
      return dt_err(ctl, F, "%s", 
        message);
  }

  switch(which)
  { 
    case DTXCHG_WIN:					/* ................................	*/
    {
      DT_C(dtxchg_win_import_lut8,		/* import from window server		*/
        (ctl, server, id, lut,
         start, count));
	} break;
    default:
	{
	  rc = dt_err(ctl, F,
        "cannot import lut"
        " from \"%s\" (type %s)",
        spec, 
        which < DTXCHG_END_OF_LIST?
          dtxchg_choices[which]:
          "unknown");
	}
  }
 
cleanup:
  if (server != NULL)
    DT_I(dtos_free2,(ctl, 
     server, F, "server"));
  if (id !=  NULL)
    DT_I(dtos_free2,(ctl, 
     id, F, "id"))

  return rc;
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
