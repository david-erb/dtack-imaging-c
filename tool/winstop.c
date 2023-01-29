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
| dttool_winstop utility - tell win server to stop
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dttool.h>

DT_RCSID("tool $RCSfile: winstop.c,v $ $Revision: 1.9 $");

#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/os.h>

#define USAGE "[params], -win param specifies win server"

/*..........................................................................*/

DTTOOL_MAIN(dttool_winstop)
{
  dt_ctl_t *ctl = &param->ctl;
  int n;
  dtxchg_e which;
  char *server = NULL;
  char *id = NULL;
  char win[64];
  dt_rc_e rc;

  DT_Q(dtparam_init,(param));			/* init param structure             */

  DT_C(dt_enter_param,(					/* go parse params					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;

  if (rc == DT_RC_GOOD)
  if (argc != 0)						/* check args						*/
    DT_C(dt_usage_say,(
      param, USAGE));

  DT_C(dtstr_printf,(ctl,
    win, sizeof(win)-1, "%s",
    param->win));

  if (!(param->win_flag &				/* -win not given on command line? 	*/
		DTPARAM_FLAG_VALUE))
    strcat(win, ",");

#ifdef NONO
  DT_C(dtxchg_which,(ctl, win,			/* figure out which exchanger	    */
    &which, &server, &id, F));

  switch(which)
  { 
    case DTXCHG_WIN:					/* ................................	*/
    {
	  DT_C(dtxchg_win_stop,(ctl, 
        server));
	} break;
    default:
	{
      if (which < DTXCHG_END_OF_LIST)	/* some known one we can't handle?	*/
	  {
        rc = dt_err(ctl, F,
          "\"%s\" is type %s,"
          " which cannot be told to stop",
          param->win,
          dtxchg_choices[which]);
		goto cleanup;
	  }
	  else								/* totally unknown "which"		    */
	  {
        rc = dt_err(ctl, F,
          "unknown exchanger type %d"
          " for \"%s\"",
          which, param->win);
		goto cleanup;
	  }
	}
  }
#endif
  
cleanup:
  if (id != NULL)
    DT_I(dtos_free2,(ctl, id,
      F, "id"));
  if (server != NULL)
    DT_I(dtos_free2,(ctl, server,
      F, "server"));
  
  DT_Q(dtparam_free,(param));			/* free param space					*/

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
