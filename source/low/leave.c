/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>

DT_RCSID("source/low $RCSfile: leave.c,v $ $Revision: 1.2 $");

#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/msw.h>

/* ........................................................................ *
| NAME
| dt_leave() - release program environment resources
| 
| DESCRIPTION
| dt_leave() releases program environment resources allocated
| by the :enter: Function Group and the :param: Function Group.
|
| All program resources
|_aside Frees program environment resources.
| allocated by the program environment setup functions
| dt_enter_parse() and dtparam_init()
| and stored in ~*param~ and ~*param->ctl~ are freed.
|
| This function is a convenient epilogue function for a program.
|_aside Epilogue function.
|
| This function does not release resources allocated by dt_heapmon_init().
| For this, you must call dt_heapmon_uninit() explicitly.
| 
| It is good form to call this function before your program exits.
|_aside Program exit frees resources anyway.
| It is not absolutely required because
| all the resources it frees would be freed anyway when your program exits.
| However, if dt_heapmon_init() has activated heap monitoring,
| you might get error messages relating to unfreed memory blocks.
|
| RETURN VALUES
| Resources stored in ~*param~ are freed.
| Various pointers in ~*param~ are cleared to zero.
|dxinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to free malloc() memory
| 
| CAVEATS
| This function does not release resources allocated by dt_heapmon_init().
|
| SEE ALSO
| Please refer to the
|_hyper Overview, "Overview"
| and
|_hyper The leave Function Group, "The :leave: Function Group"
| and
|_hyper sections, The :enter: Function Group
| 
| The following functions are also related:
| dtparam_init()			initialize parameter structure
| dt_enter_parse()			parse command line
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv3.c:
| 
| The following code snippet highlights this function:
|
|:  dt_leave(&param);					// program epilogue
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dt_leave(								/* clean up before leaving 			*/
  dtparam_t *param)
{
  DT_F("dt_leave");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  if (ctl != NULL)
  {
    if (ctl->feedbackwin != NULL)		/* notify launcher we are quitting 	*/
    {
	  dtmsw_t *msw =					/* reference special msw structure 	*/
        (dtmsw_t *)ctl->msw;
#define DBG(N) \
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, \
        "%5ld " #N "'s", msw->N)
      DBG(n_outstring);
      DBG(n_peekmessage);
      DBG(n_getmessage);
      DBG(n_yield);
      DBG(n_kick);
      DBG(n_status);
      DBG(n_feedback);
      DT_I(dtmsw_kick,(ctl,	
        DTMSW_KICK_FINISH));
    }

	if (ctl->mem != NULL)
	{
	  DT_I(dtos_free2,(ctl,
        (char *)ctl->mem,
        F, "big memory source specs"));
	  ctl->mem = NULL;
	}
	if (ctl->win != NULL)
	{
	  DT_I(dtos_free2,(ctl,
        (char *)ctl->win,
        F, "default windowing server name"));
	  ctl->win = NULL;
	}
  }

  DT_I(dtparam_free,(param));			/* free param space				    */

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
