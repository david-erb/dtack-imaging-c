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
| dtwin_r server side
| uses generic dtscr interface for screen interaction
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

DT_RCSID("base $RCSfile: wins.c,v $ $Revision: 1.1 $");

#include <dtack/win_r.h>

#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>						/* to satisfy dtscr.h only		    */
#include <dtack/svc.h>
#include <dtack/scr.h>
#include <dtack/ipc.h>
#include <dtack/wins.h>

#define Z return DT_RC_GOOD;

/*..........................................................................*/
static
dt_rc_e
dtwins_create_window(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtwin_r_create_window_arg_t *argp,
  dtwin_r_create_window_ret_t *res)
{
  DT_F("dtwins_create_window");
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "\"%s\": (%ld,%ld) (%ld,%ld) bit %ld",
    argp->name,
    argp->x0, argp->y0,
    argp->xe, argp->ye,
    argp->bits);

  DT_G(dtscr_create_window,(
    ctl, wins->scr,
    argp->name,
    argp->x0, argp->y0,
    argp->xe, argp->ye,
    (int)argp->bits));

  res->rc = rc;
  Z;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtwins_export_data(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtwin_r_export_data_arg_t *argp,
  dtwin_r_export_data_ret_t *res)
{
  DT_F("dtwins_export_data");
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "\"%s\": (%ld,%ld) (%ld,%ld) bits %ld, d.d_len %ld",
    argp->name, 
    argp->x0, argp->y0,
    argp->xe, argp->ye, 
    argp->bits,
    argp->d.d_len);

  DT_G(dtscr_export_data,(
    ctl, wins->scr,
    argp->name,
    argp->x0, argp->y0,
    argp->xe, argp->ye, 
    (int)argp->bits, 
    argp->d.d_val));

  res->rc = rc;
  Z;
}


/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtwins_import_data(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtwin_r_import_data_arg_t *argp,
  dtwin_r_import_data_ret_t *res)
{
  DT_F("dtwins_import_data");
  dtscr_state_t state;
  char *d_val = NULL;
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "name \"%s\"", argp->name);

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "\"%s\": (%ld,%ld) (%ld,%ld)",
    argp->name,
    argp->x0, argp->y0,
    argp->xe, argp->ye);

  DT_GI(dtscr_query_state,(ctl,			/* get state of image				*/
    wins->scr,
    argp->name, &state));

  if (state.xe == 0)					/* window does not exist? 			*/
    rc = dt_err(ctl, F,
      "window \"%s\""
      " does not exist",
      argp->name);

  if (rc == DT_RC_GOOD)
  res->d.d_len = argp->xe * argp->ye;

  if (rc == DT_RC_GOOD)
  if (state.bits != 1 && state.bits != 8)
    rc = dt_err(ctl, F,
      "unable to handle %d-bit image"
      " from dtscr",
      state.bits);

  if (rc == DT_RC_GOOD)
  if (state.bits == 1)
    res->d.d_len /= 8;

  DT_GI(dtos_malloc2,(ctl, 				/* get space for the data			*/
    (void **)&d_val,
    res->d.d_len, 
    F, "imported data"));

  DT_GI(dtscr_import_data,(ctl,			/* go ask scr for the data		    */
    wins->scr,							
    argp->name,
    argp->x0, argp->y0,
    argp->xe, argp->ye,
    d_val));

  if (rc == DT_RC_GOOD)					/* successful import from scr?	    */
  {
    res->d.d_val = d_val;
    res->rc = DT_RC_GOOD;
  }
  else									/* scr import failed?			    */
  {
    if (d_val != NULL)					/* we had allocated something? 	    */
      DT_G(dtos_free2,(ctl,				/* free it						    */
        d_val, 
        F, "imported data"));
    res->d.d_val = NULL;
    res->d.d_len = 0;
    res->rc = DT_RC_BAD;
  }

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "returning size %d rc %s",
    res->d.d_len,
    res->rc == DT_RC_GOOD? "DT_RC_GOOD": "DT_RC_BAD");

  Z;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtwins_export_color(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtwin_r_export_color_arg_t *argp,
  dtwin_r_export_color_ret_t *res)
{
  DT_F("dtwins_export_color");
  dtlut8_t lut[256];
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "\"%s\" start %d, count %d, d.d_len %u",
    argp->name, argp->start, argp->count,
    argp->d.d_len);
  {
	unsigned char *r = (unsigned char *)argp->d.d_val;
	unsigned char *g = r + argp->count;
	unsigned char *b = g + argp->count;
	unsigned char *i = b + argp->count;
	int j;

    for (j=0; j<argp->count; j++)
    {
	  lut[j].r = r[j];
	  lut[j].g = g[j];
	  lut[j].b = b[j];
	  lut[j].i = i[j];
	}
  }

  DT_G(dtscr_export_color,(ctl, wins->scr,
    argp->name, lut,
    argp->start, argp->count));

  res->rc = rc;
  Z;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtwins_import_color(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtwin_r_import_color_arg_t *argp,
  dtwin_r_import_color_ret_t *res)
{
  DT_F("dtwins_import_color");
  char *d_val = NULL;
  dtlut8_t lut[256];
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "\"%s\" start %d, count %d",
    argp->name, argp->start, argp->count);

  if (rc == DT_RC_GOOD)
    res->d.d_len = argp->count * 4;

  DT_GI(dtos_malloc2,(ctl, 				/* get space for the data			*/
    (void **)&d_val,
    res->d.d_len, 
    F, "imported colors"));

  DT_GI(dtscr_import_color,(ctl,		/* go ask scr for the data		    */
    wins->scr,							
    argp->name,
    lut, argp->start, argp->count));

  if (rc == DT_RC_GOOD)					/* successful import from scr?	    */
  {
	unsigned char *r = (unsigned char *)d_val;
	unsigned char *g = r + argp->count;
	unsigned char *b = g + argp->count;
	unsigned char *i = b + argp->count;
	int j;

    for (j=0; j<argp->count; j++)
    {
	  r[j] = lut[j].r; 
	  g[j] = lut[j].g;
	  b[j] = lut[j].b;
	  i[j] = lut[j].i;
	}
    res->d.d_val = d_val;
    res->rc = DT_RC_GOOD;
  }
  else									/* scr import failed?			    */
  {
    if (d_val != NULL)					/* we had allocated something? 	    */
      DT_G(dtos_free2,(ctl,				/* free it						    */
        d_val, 
        F, "imported data"));
    res->d.d_val = NULL;
    res->d.d_len = 0;
    res->rc = DT_RC_BAD;
  }

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "returning size %d rc %s",
    res->d.d_len,
    res->rc == DT_RC_GOOD? "DT_RC_GOOD": "DT_RC_BAD");

  Z;
}

/*..........................................................................
 *..........................................................................*/
static
dt_rc_e
dtwins_query(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtwin_r_query_arg_t *argp,
  dtwin_r_query_ret_t *res)
{
  DT_F("dtwins_query");
  dtscr_state_t state;
  dt_rc_e rc;

  if (((long *)argp->name)[0] == 0)		/* looks like a special command? 	*/
  {
	if (!strcmp(argp->name+sizeof(long),
        DTWIN_R_QUERY_STOP))
	{
	  dt_dbg(ctl, F,
        DT_DBG_MASK_TRACE |
        DT_DBG_MASK_WIN |
        DT_DBG_MASK_IPC,
        "stop command received");
      DT_MEMSET(&res->state, 0,
        sizeof(res->state));
      res->rc = DT_RC_GOOD;
      return DT_RC_STOP;
	}
  }

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "\"%s\":", argp->name);

  rc = dtscr_query_state(ctl,			/* ask scr for state of window		*/
    wins->scr,
    argp->name, &state);

  if (rc == DT_RC_GOOD)					/* successful query from scr?	    */
  {
    res->state.x0 = state.x0;
    res->state.y0 = state.y0;
    res->state.xe = state.xe;
    res->state.ye = state.ye;
    res->state.bits = state.bits;
    res->rc = DT_RC_GOOD;
  }
  else									/* scr query failed?			    */
  {
    DT_MEMSET(&res->state, 0,
      sizeof(res->state));
    res->rc = DT_RC_BAD;
  }

  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "\"%s\": size (%d,%d) bits %d",
    argp->name,
    res->state.xe, res->state.ye,
    res->state.bits);

  Z;
}

/*..........................................................................
 * run ipc server
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtwins_init(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtscr_t *scr)
{
  DT_MEMSET(wins, 0, sizeof(*wins));

  wins->scr = scr;
  wins->create_window = dtwins_create_window;
  wins->export_data = dtwins_export_data;
  wins->import_data = dtwins_import_data;
  wins->export_color = dtwins_export_color;
  wins->import_color = dtwins_import_color;
  wins->query = dtwins_query;

  return DT_RC_GOOD;
}

/*..........................................................................
 * run ipc server
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtwins_uninit(
  dt_ctl_t *ctl,
  dtwins_t *wins)
{
  DT_MEMSET(wins, 0, sizeof(*wins));
  return DT_RC_GOOD;
}

/*..........................................................................
 * run ipc server
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtwins_dispatch(
  dt_ctl_t *ctl,
  dtwins_t *wins,
  dtsvc_t *svc)
{
  DT_F("dtwins_dispatch");

  if (svc->type == DTSVC_TYPE_IPC)
    DT_Q(dtwins_ipc_dispatch,(ctl, wins, svc))
  else
  if (svc->type == DTSVC_TYPE_DSK)
    DT_Q(dtwins_dsk_dispatch,(ctl, wins, svc))
  else
    return dt_err(ctl, F, 
      "invalid svc type %d",
      svc->type);
  
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
