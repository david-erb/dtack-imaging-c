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
| Called by ipc server.
| Inputs args from dtipc_recv and deserializes them.
| Calls dtwin_svc function with argp and res structures.
| Serializes results out to dtipc_send.
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

DT_RCSID("base $RCSfile: wins_ipc.c,v $ $Revision: 1.1 $");

#include <dtack/win_r.h>
#include <dtack/ipc.h>
#include <dtack/list.h>
#include <dtack/lut8.h>						/* to satisfy dtscr.h only		    */
#include <dtack/svc.h>
#include <dtack/scr.h>

#include <dtack/wins.h>

/*..........................................................................*/
/*..........................................................................*/

#define Q DT_Q
#define G DT_G
#define I DT_I

#define RECV_STRUCT						/* receive arg structure			*/\
  {Q(ipc->recv,(ctl,                                                          \
 	 ipc, (void **)&argp, sizeof(*argp)));                                    \
   *f++ = (void *)argp;}

#define FIX_ENDIAN(L, TYPE)				/* fix long byte endianing			*/\
  L = (TYPE)DTIPC_FIX(DT_Q, ctl, ipc, L)

#define V(VAR) 							/* receive variable length array	*/\
  if (rc == DT_RC_GOOD)                                                       \
  {G(ipc->recv,(ctl, ipc,                                                     \
     (void **)&argp->VAR. ## VAR ## _val,                                     \
     argp->VAR. ## VAR ## _len));                                             \
   *f++ = (void *)argp->VAR. ## VAR ## _val;}
#define CALL(FUNC)						/* call service function			*/\
  if (rc == DT_RC_GOOD)                                                       \
  {G(wins-> ## FUNC,(ctl, wins,                                               \
     argp, &res));}

#define RESPOND 						/* send response structure			*/\
  {I(ipc->send,(                                                              \
 	 ctl, ipc, &res, sizeof(res)));} 

#define W(VAR, LEN)						/* send variable length array		*/\
  if (rc == DT_RC_GOOD &&                                                     \
      res.VAR. ## VAR ## _val != NULL &&                                      \
      LEN != 0)                                                               \
  {G(ipc->send,(ctl, ipc,                                                     \
     res.VAR. ## VAR ## _val, LEN));                                          \
   dtos_free2(ctl,                                                            \
     res.VAR. ## VAR ## _val,                                                 \
     F, "res." #VAR "." #VAR "_val");}

#define Z								/* free stuff we allocated			*/\
  while(--f >= ff) dtos_free2(ctl, *f,                                        \
    F, *f == argp? "received args":                                           \
    "received variable length data");                                         \
  return rc;

/*..........................................................................*/
static
dt_rc_e
dtwins_ipc_create_window(dt_ctl_t *ctl, dtipc_t *ipc, dtwins_t *wins)
{
	DT_F("dtwins_ipc_create_window");
	dtwin_r_create_window_arg_t *argp;
	dtwin_r_create_window_ret_t res;
	void *ff[10]; void **f = ff;
    dt_rc_e rc = DT_RC_GOOD;
	RECV_STRUCT;
	FIX_ENDIAN(argp->x0, int);
	FIX_ENDIAN(argp->y0, int);
	FIX_ENDIAN(argp->xe, int);
	FIX_ENDIAN(argp->ye, int);
	FIX_ENDIAN(argp->bits, int);
	CALL(create_window);
	RESPOND;
    Z;
}

/*..........................................................................*/
static
dt_rc_e
dtwins_ipc_export_data(dt_ctl_t *ctl, dtipc_t *ipc, dtwins_t *wins)
{
	DT_F("dtwins_ipc_export_data");
	dtwin_r_export_data_arg_t *argp;
	dtwin_r_export_data_ret_t res;
	void *ff[10]; void **f = ff;
    dt_rc_e rc = DT_RC_GOOD;
	RECV_STRUCT;
	FIX_ENDIAN(argp->x0, int);
	FIX_ENDIAN(argp->y0, int);
	FIX_ENDIAN(argp->xe, int);
	FIX_ENDIAN(argp->ye, int);
	FIX_ENDIAN(argp->bits, int);
	FIX_ENDIAN(argp->d.d_len, u_int);
	V(d);
	CALL(export_data);
	RESPOND;
    Z;
}

/*..........................................................................*/
static
dt_rc_e
dtwins_ipc_export_color(dt_ctl_t *ctl, dtipc_t *ipc, dtwins_t *wins)
{
	DT_F("dtwins_ipc_export_color");
	dtwin_r_export_color_arg_t *argp;
	dtwin_r_export_color_ret_t res;
	void *ff[10]; void **f = ff;
    dt_rc_e rc = DT_RC_GOOD;
	RECV_STRUCT;
	FIX_ENDIAN(argp->start, int);
	FIX_ENDIAN(argp->count, int);
	FIX_ENDIAN(argp->d.d_len, u_int);
	V(d);
	CALL(export_color);
	RESPOND;
	Z;
}

/*..........................................................................*/
static
dt_rc_e
dtwins_ipc_import_data(dt_ctl_t *ctl, dtipc_t *ipc, dtwins_t *wins)
{
	DT_F("dtwins_ipc_import_data");
	dtwin_r_import_data_arg_t *argp;
	dtwin_r_import_data_ret_t res;
	void *ff[10]; void **f = ff;
	u_int len;
    dt_rc_e rc = DT_RC_GOOD;
	RECV_STRUCT;
	FIX_ENDIAN(argp->x0, int);
	FIX_ENDIAN(argp->y0, int);
	FIX_ENDIAN(argp->xe, int);
	FIX_ENDIAN(argp->ye, int);
	CALL(import_data);
	len = res.d.d_len;					/* save length before "fixing" 		*/
	FIX_ENDIAN(res.d.d_len, u_int);
	RESPOND;
	W(d, len);
	Z;
}

/*..........................................................................*/
static
dt_rc_e
dtwins_ipc_import_color(dt_ctl_t *ctl, dtipc_t *ipc, dtwins_t *wins)
{
	DT_F("dtwins_ipc_import_color");
	dtwin_r_import_color_arg_t *argp;
	dtwin_r_import_color_ret_t res;
	void *ff[10]; void **f = ff;
	u_int len;
    dt_rc_e rc = DT_RC_GOOD;
	RECV_STRUCT;
	FIX_ENDIAN(argp->start, int);
	FIX_ENDIAN(argp->count, int);
	CALL(import_color);
	len = res.d.d_len;					/* save length before "fixing" 		*/
	FIX_ENDIAN(res.d.d_len, u_int);
	RESPOND;
	W(d, len);
	Z;
}

/*..........................................................................*/
static
dt_rc_e
dtwins_ipc_query(dt_ctl_t *ctl, dtipc_t *ipc, dtwins_t *wins)
{
	DT_F("dtwins_ipc_query");
	dtwin_r_query_arg_t *argp;
	dtwin_r_query_ret_t res;
	void *ff[10]; void **f = ff;
    dt_rc_e rc = DT_RC_GOOD;
	RECV_STRUCT;
	CALL(query);
    if (rc == DT_RC_GOOD && 
        res.rc == DT_RC_GOOD)
      res.state.recv_max = ipc->recv_max;
	FIX_ENDIAN(res.state.xe, int);
	FIX_ENDIAN(res.state.ye, int);
	FIX_ENDIAN(res.state.bits, int);
	FIX_ENDIAN(res.state.recv_max, u_int);
    RESPOND;
	Z;
}

/*..........................................................................
| Get pending ipc request and honor it with dtscr.
| The ipc protocol says that each ipc interaction begins with
| a command sent twice.
| This routine expects that the first command was already
| gobbled up by by some other mechanism, probably dtscr_wait_event().
| It is an error to call this function with an ~*svc~ structure which
| was not initialized by dtsvc_ipc_register().
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtwins_ipc_dispatch(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtsvc_t *svc)
{
  DT_F("dtwins_ipc_dispatch");
# define SERVE(A) DT_C(dtwins_ipc_ ## A,(ctl, ipc, wins)); break;
  dtipc_t *ipc;
  unsigned long *cmd = NULL;
  dt_rc_e rc;

  DT_C(dtsvc_ipc_getipc,(ctl, svc,		/* fetch ipc structure 				*/
    (void **)&ipc));

  DT_C(dtipc_recv,(ctl,					/* command available on ipc 		*/
    ipc, (void **)&cmd, 
    sizeof(*cmd)));

  FIX_ENDIAN((*cmd), unsigned long);
  switch(*cmd)
  {
	case DTWIN_R_CREATE_WINDOW: SERVE(create_window);
	case DTWIN_R_EXPORT_DATA:   SERVE(export_data);
	case DTWIN_R_EXPORT_COLOR:  SERVE(export_color);
	case DTWIN_R_IMPORT_DATA:   SERVE(import_data);
	case DTWIN_R_IMPORT_COLOR:  SERVE(import_color);
	case DTWIN_R_QUERY:         SERVE(query);
    default:
      rc = dt_err(ctl, F, 
        "unknown ipc command 0x%08lx",
        (long)cmd);
  }
cleanup:
  if (cmd)
    DT_I(dtos_free,(ctl, cmd));
  DT_I(dtipc_done,(ctl, ipc));			/* server is done with connection 	*/
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
