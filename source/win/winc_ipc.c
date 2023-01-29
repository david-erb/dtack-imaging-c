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
  19990130 DE timeout 5 seconds looking for dtview server
 *..........................................................................*/


/*..........................................................................
| NAME
| Introduction - intro
|
| DESCRIPTION
| Called by dtwin_export_data and the others.
| Gets a client handle then calls the IPC-generated client-side functions.
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
DT_RCSID("base $RCSfile: winc_ipc.c,v $ $Revision: 1.1 $");

#include <dtack/win_r.h>

#include <dtack/winc.h>

#include <dtack/ipc.h>
#include <dtack/os.h>

/*..........................................................................*/
/*..........................................................................*/

#define PROLOG \
  unsigned long cmd; \
  char ipc_space[sizeof(dtipc_t)]; \
  dtipc_t *ipc = (dtipc_t *)ipc_space; \
  dt_dbg(ctl, F, DT_DBG_MASK_IPC, \
    "winc->server \"%s\"," \
    " argp->name \"%s\"", \
    winc && winc->server? \
    winc->server: "NULL", \
    argp && argp->name? \
    argp->name: "NULL"); \
  DT_Q(dtipc_find,(ctl, ipc,			/* connect to server			    */\
    winc->server, 0, 0, 5000));

#define EPILOG \
  DT_Q(dtipc_unfind,(ctl, ipc));		/* disconnect from server			*/\
  return DT_RC_GOOD

/*..........................................................................*/
static
dt_rc_e
dtwinc_ipc_create_window(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_create_window_arg_t *argp,
  dtwin_r_create_window_ret_t **res)
{
  DT_F("dtwinc_ipc_create_window");
  PROLOG;

  cmd = DTWIN_R_CREATE_WINDOW;
  DT_Q(dtipc_send,(ctl,					/* create window command		    */
	ipc, &cmd, sizeof(cmd)));
  DT_Q(dtipc_send,(ctl,					/* create window args			    */
 	ipc, argp, sizeof(*argp)));
  DT_Q(dtipc_recv,(ctl, ipc,			/* get reply					    */
 	(void **)res, sizeof(**res)));

  EPILOG;
}

/*..........................................................................*/
static
dt_rc_e
dtwinc_ipc_export_data(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_export_data_arg_t *argp,
  dtwin_r_export_data_ret_t **res)
{
  DT_F("dtwinc_ipc_export_data");
  PROLOG;

  cmd = DTWIN_R_EXPORT_DATA;
  DT_Q(dtipc_send,(ctl, ipc,			/* export data command		    	*/
	&cmd, sizeof(cmd)));
  DT_Q(dtipc_send,(ctl,					/* export data args			    	*/
 	ipc, argp, sizeof(*argp)));
  DT_Q(dtipc_send,(ctl, ipc,			/* export data itself		    	*/
 	argp->d.d_val,
    argp->d.d_len));
  DT_Q(dtipc_recv,(ctl, ipc,			/* get reply					    */
 	(void **)res, sizeof(**res)));

  EPILOG;
}

/*..........................................................................*/
static
dt_rc_e
dtwinc_ipc_export_color(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_export_color_arg_t *argp,
  dtwin_r_export_color_ret_t **res)
{
  DT_F("dtwinc_ipc_export_color");
  PROLOG;

  cmd = DTWIN_R_EXPORT_COLOR;
  DT_Q(dtipc_send,(ctl, ipc,			/* export color command		    	*/
	&cmd, sizeof(cmd)));
  DT_Q(dtipc_send,(ctl, ipc, argp,		/* export color args		    	*/
 	sizeof(*argp)));
  DT_Q(dtipc_send,(ctl, ipc,			/* export color itself		    	*/
 	argp->d.d_val,
    argp->d.d_len));
  DT_Q(dtipc_recv,(ctl, ipc,			/* get reply					    */
 	(void **)res, sizeof(**res)));

  EPILOG;
}
/*..........................................................................*/
static
dt_rc_e
dtwinc_ipc_import_data(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_import_data_arg_t *argp,
  dtwin_r_import_data_ret_t **res)
{
  DT_F("dtwinc_ipc_import_data");
  PROLOG;

  cmd = DTWIN_R_IMPORT_DATA;
  DT_Q(dtipc_send,(ctl,					/* send "import data" command	   	*/
	ipc, &cmd, sizeof(cmd)));
  DT_Q(dtipc_send,(ctl, ipc, argp,	 	/* send "import data" cmd's args   	*/
    sizeof(*argp)));
  DT_Q(dtipc_recv,(ctl, ipc,			/* get reply					    */
 	(void **)res, sizeof(**res)));
  DT_Q(dtipc_recv,(ctl, ipc,			/* get "import data" data		    */
    (void **)&(*res)->d.d_val, 
    (*res)->d.d_len));

  EPILOG;
}

/*..........................................................................*/
static
dt_rc_e
dtwinc_ipc_import_color(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_import_color_arg_t *argp,
  dtwin_r_import_color_ret_t **res)
{
  DT_F("dtwinc_ipc_import_color");
  PROLOG;

  cmd = DTWIN_R_IMPORT_COLOR;
  DT_Q(dtipc_send,(ctl,					/* send "import color" command	   	*/
	ipc, &cmd, sizeof(cmd)));
  DT_Q(dtipc_send,(ctl, ipc, argp,		/* send "import color" cmd's args  	*/
    sizeof(*argp)));
  DT_Q(dtipc_recv,(ctl, ipc,			/* get reply					    */
 	(void **)res, sizeof(**res)));
  DT_Q(dtipc_recv,(ctl, ipc,			/* get "import color" data		    */
    (void **)&(*res)->d.d_val, 
    (*res)->d.d_len));

  EPILOG;
}

/*..........................................................................*/
static
dt_rc_e
dtwinc_ipc_query(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  dtwin_r_query_arg_t *argp,
  dtwin_r_query_ret_t **res)
{
  DT_F("dtwinc_ipc_query");
  PROLOG;

  cmd = DTWIN_R_QUERY;
  DT_Q(dtipc_send,(ctl,					/* query command		    		*/
	ipc, &cmd, sizeof(cmd)));
  DT_Q(dtipc_send,(ctl, ipc, argp,		/* send query cmd's args		    */
    sizeof(*argp)));
  DT_Q(dtipc_recv,(ctl, ipc,			/* get reply					    */
 	(void **)res, sizeof(**res)));
  
  EPILOG;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtwinc_ipc_init(
  dt_ctl_t *ctl,
  dtwinc_t *winc)
{
  DT_F("dtwinc_ipc_init");
  if (!strlen(winc->server))			/* server name not specified?	  	*/
    return dt_err(ctl, F,
      "empty server name");
  dt_dbg(ctl, F, DT_DBG_MASK_WIN, 
    "initializing client for \"%s\"", 
    winc->server);
  winc->create_window = dtwinc_ipc_create_window;
  winc->export_data = dtwinc_ipc_export_data;
  winc->export_color = dtwinc_ipc_export_color;
  winc->import_data = dtwinc_ipc_import_data;
  winc->import_color = dtwinc_ipc_import_color;
  winc->query = dtwinc_ipc_query;
  winc->recv_max = DTCONFIG_MALLOC_MAX;
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
