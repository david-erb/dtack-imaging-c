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
| USAGE
| dtview - server executive
| 
|.SYNOPSIS
|:dtview [parameters]
| 
|.SUMMARY
| Run as server waiting for client IPC requests and for mouse/keyboard events.
|
|.PARAMETERS
| :-screen:		screen to use as display
| :-output:		directory to save images in
| :-variant:	options string
|dtinclude(params1.dx)
|
|.DESCRIPTION
| As an IPC (interprocess communication) server,
| dtview receives images from client programs.
| It also will send images to client programs upon their requests.
|
| As an interactive window manager, dtview allows a human operator
| to move the images around on the screen.
| Other minimal interactions are also possible.
|
| For a list of commands, hit the '?' (question mark key)
| in any of dtview's windows.
|
| If ~variant~ contains the string "nointro",
| then the welcome screen will not be displayed.
| 
|dtinclude(params2.dx)
|
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("server $RCSfile: view.c,v $ $Revision: 1.4 $");

#include <dtack/os.h>

#include <dtack/lut8.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>

#include <dtack/win_r.h>					/* needed inside dtwins.h only 		*/
#include <dtack/wins.h>						/* we are an svc-on-scr server 		*/

#include <dtack/view.h>
#include <dtview.use>

#include <dtack/mem.h>
#include <dtack/str.h>

static dtview_loop_f dtview_svc_loop;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtview_svc_loop(
  dtparam_t *param,
  dtwins_t *wins,
  dtsvc_t *svc,
  const char *output,
  const char *help_string)
{
  DT_F("dtview_svc_loop");
  dt_ctl_t *ctl = &param->ctl;

  while (1)
  {
    DT_Q(dtscr_wait_event,(ctl,			/* wait for quit event				*/
      wins->scr, svc, 3));

	DT_Q(dtview_keys,(ctl,				/* handle keys if any 				*/
      wins->scr, output, help_string));
	
	DT_Q(dtview_resize,(ctl,			/* handle resize if any 			*/
      wins->scr));
	
    if (wins->scr->
          last_event.keys[0] == 		/* user hit quit key?			    */
        DTSCR_KEY_QUIT)
      break;							/* must be to quit the program		*/
    else			
    if (wins->scr->last_event.type ==	/* it was an svc event?			    */
        DT_EVENT_IPC)
	{
      DT_Q(dtwins_dispatch,(ctl,		/* handle incoming svc				*/
 	    wins, svc));
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtview_arg_check(						/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dtview_arg_check");

  return DT_RC_GOOD;
}

/*..........................................................................
 * dtview_work() is also called by other servers besides this ipc one
 *..........................................................................*/

dt_rc_e
dtview_ipc(
  int argc,
  char *argv[],
  dtparam_t *param)
{
  DT_F("dtview_ipc");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  dtsvc_t svc;
  dt_rc_e rc;

  svc.priv = NULL;

  DT_Q(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_C(dtparam_init,(param));           /* init param structure             */

  DT_C(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->screen, NULL, NULL, NULL,
    &param->output, NULL, NULL, NULL,
    &param->variant, NULL, NULL, "",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  DT_C(dt_enter_parse,(					/* go parse params and args			*/
    argc, argv, F, usage, param,
    arg, dtview_arg_check, NULL));

  DT_C(dtsvc_register,(ctl,				/* register public name			    */
    &svc, 
    strncmp(param->win, "win,", 4)?		/* in case of common environment 	*/
      param->win: param->win+4,
    0));								/* don't go background 				*/
  
  DT_C(dtview_work,(argc, argv, param,	/* go do the work 					*/
    &svc, dtview_svc_loop));

cleanup:
  if (svc.priv != NULL)
    DT_I(dtsvc_unregister,(ctl, &svc));

  DT_I(dt_leave,(param));				/* clean up before leaving			*/

  {
    int assert_fail = 0;
    DT_I(dt_heapmon_uninit,(ctl,		/* check all memory is freed	    */
      &assert_fail));
    if (assert_fail)
      rc = DT_RC_BAD;
  }

  return DT_RC_GOOD;
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
