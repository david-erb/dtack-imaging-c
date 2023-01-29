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
| main program
| 
| END
 *..........................................................................*/

#include <tabrobot.h>
DT_RCSID("app/tabrobot $RCSfile: gui_qnx.c,v $ $Revision: 1.8 $");

#if DTPRJ_QW4 == DT_1

#ifndef DTDEPEND
#	include <windows/Qwindows.h>
#	include <sys/kernel.h>
#endif

static int VTPP = 0;
static int HTPP = 0;
#define XSCALE(X) ((X))
#define YSCALE(Y) ((Y))
#endif

#if DTPRJ_QW4 == DT_1
/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
DTCONFIG_API1
tabrobot_gui_qnx_send(
  dtparam_t *param,
  dtipc_t *ipc,
  char *tag,
  int first,
  long *turnaround)
{
  DT_F("tabrobot_gui_qnx_eventloop");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;
  
# define IS_BUTTON(TAG) (!strcmp(TAG, tag))
# define SEND(COMMAND, N) 				\
   {									\
     if (first || (N) == -1)			\
       DT_C(tabrobot_client_send,(		\
         param, ipc, COMMAND, 			\
         turnaround));					\
   }
  
										// ................................
	if (IS_BUTTON("panleft"))
	  SEND("ptz,<x", 1)
	else
	if (IS_BUTTON("panright"))
	  SEND("ptz,>x", 1)
	else
	if (IS_BUTTON("tiltup"))
	  SEND("ptz,>y", 1)
	else
	if (IS_BUTTON("tiltdown"))
	  SEND("ptz,<y", 1)
	else
										// ................................
	if (IS_BUTTON("panleft1"))
	  SEND("ptz,-x", -1)
	else
	if (IS_BUTTON("panright1"))
	  SEND("ptz,+x", -1)
	else
	if (IS_BUTTON("tiltup1"))
	  SEND("ptz,+y", -1)
	else
	if (IS_BUTTON("tiltdown1"))
	  SEND("ptz,-y", -1)
	else
										// ................................
	if (IS_BUTTON("upleft"))
	  SEND("ptz,<U", 1)
	else
	if (IS_BUTTON("upright"))
	  SEND("ptz,>U", 1)
	else
	if (IS_BUTTON("downleft"))
	  SEND("ptz,<D", 1)
	else
	if (IS_BUTTON("downright"))
	  SEND("ptz,>D", 1)
	else
										// ................................
	if (IS_BUTTON("upleft1"))
	  SEND("ptz,-U", -1)
	else
	if (IS_BUTTON("upright1"))
	  SEND("ptz,+U", -1)
	else
	if (IS_BUTTON("downleft1"))
	  SEND("ptz,-D", -1)
	else
	if (IS_BUTTON("downright1"))
	  SEND("ptz,+D", -1)
	else
										// ................................
	if (IS_BUTTON("zoomout"))
	  SEND("ptz,<z", 1)
	else
	if (IS_BUTTON("zoomin"))
	  SEND("ptz,>z", 1)
	else
	if (IS_BUTTON("zoomout1"))
	  SEND("ptz,-z", -1)
	else
	if (IS_BUTTON("zoomin1"))
	  SEND("ptz,+z", -1)
	else
										// ................................
	if (IS_BUTTON("focusnear"))
	  SEND("ptz,<f", 1)
	else
	if (IS_BUTTON("focusfar"))
	  SEND("ptz,>f", 1)
	else
	if (IS_BUTTON("focusnear1"))
	  SEND("ptz,-f", -1)
	else
	if (IS_BUTTON("focusfar1"))
	  SEND("ptz,+f", -1)
  
cleanup:
  return rc;
}
/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
DTCONFIG_API1
tabrobot_gui_qnx_eventloop(
  dtparam_t *param,
  dtipc_t *ipc,
  int win_id)
{
  DT_F("tabrobot_gui_qnx_eventloop");
  dt_ctl_t *ctl = &param->ctl;
  int quit = 0;
  int first = 1;
  dt_rc_e rc = DT_RC_GOOD;

  do 
  {
    QW_EVENT_MSG event_msg;
    pid_t pid;
    int action;
	char *tag;
	long turnaround;

    pid = GetEvent(0,					/* wait for event to happen 		*/
      (void *)&event_msg,
      sizeof(event_msg));
    if (pid == 0)
      continue;
    else
    if (pid == -1)
	{
      rc = dt_err(ctl, F,
        "GetEvent failed");
	  goto cleanup;
	}

    action = Event(&event_msg);

	if (action == 0)					/* non-qw event						*/
	  continue;

	if (DT_DBG(ctl, DT_DBG_MASK_ASYNC))
	  EventPrint(F, &event_msg);		/* print info about the report	    */
	
	tag = event_msg.hdr.key;
	if (tag-(char *)&event_msg == 38)	/* old version of windows? 			*/
	  tag -= 2;

	switch(action)
	{
  	  case QW_CLICK:
		DT_C(tabrobot_gui_qnx_send,(param, ipc, tag, first, &turnaround));
		first = 0;
	  break;
	  case QW_RELEASE:
        DT_C(tabrobot_client_send,(param, ipc, "<>", &turnaround));
		first = 1;
	  break;
	  case QW_QUIT:
        goto cleanup;
	  break;
	}

	dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "key=\"%s\"",
      event_msg.hdr.key);
  } while(!quit);
  
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
DTCONFIG_API1
tabrobot_gui_qnx_pushing(
  dtparam_t *param,
  const char *name,
  const char *label,
  int x,
  int y, 
  int w,
  int h)
{
  DT_F("tabrobot_gui_qnx_pushing");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DrawAt(YSCALE(y), XSCALE(x));

  DrawButton(label, "*", NULL, name);

  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS, 
    "button name=\"%s\" label=\"%s\" at %d,%d",
    name, label, YSCALE(y), XSCALE(x));
  
  Draw();
  return rc;
}

/*..........................................................................
 *................................ up..........................................*/

static
dt_rc_e
DTCONFIG_API1
tabrobot_gui_qnx_redraw(
  dtparam_t *param)
{
  DT_F("tabrobot_gui_qnx_redraw");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

#define TAG(S)
#define PUSHING(N, L, X, Y, W, H) \
  DT_C(tabrobot_gui_qnx_pushing,(param, N, L, X, Y, W, H));
#define LABEL(N, L, X, Y, W, H)
#include <operate.i>
  
cleanup:
  return rc;
}

#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabrobot_gui_qnx(
  dtparam_t *param)
{
  DT_F("tabrobot_gui_qnx");
  dt_ctl_t *ctl = &param->ctl;
#if DTPRJ_QW4 == DT_1
  dtipc_t _ipc, *ipc = &_ipc;
  int ipc_initialized = 0;
  int sid;
  int graphics_initialized = 0;
  QW_BOX size;
  QW_BOX tpp;
  unsigned n_colors;
  char *windowname = "tabrobot";
  int xe = 300;
  int ye = 300;
  int pic_id, win_id;
  char *options, *actions;
  dt_rc_e rc;

  DT_C(dtipc_find,(ctl, ipc,			/* find tabrobot server 			*/
    param->connect, 0, 0, 1000));
  ipc_initialized = 1;

  sid = GraphicsOpen(NULL);				/* open graphics server        		*/
  if (sid == 0)
  {
    rc = dt_err(ctl, F,
      "GraphicsOpen(\"%s\") failed",
      "NULL");
	goto cleanup;
  }
  graphics_initialized = 1;

  {
    unsigned version;
    char name[QW_MAX_NAME_LEN];
    int n_pat;
    QW_BOX ppi;
    int owner;
    char model[QW_MAX_NAME_LEN];

    version = ScreenInfo(name,
      &n_colors, &n_pat, model,
      &size, &ppi, &tpp, &owner);

	VTPP = tpp.height;
	HTPP = tpp.width;

    dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
      "QNX4 Windows version %-u.%-u.%-u",
      (version / 1000),
      (version - version / 1000 * 1000)/100,
      (version %100));
    dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
      "screen server name \"%s\", n_colors %u",
      name, n_colors);
    dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
      "screen size %dx%d (VTPP=%d, HTPP=%d)",
      size.height/tpp.height,
      size.width/tpp.width,
	  VTPP, HTPP);
  }

  pic_id = Picture(						/* make a picture					*/
    windowname, NULL);
  if (pic_id == 0)
  {
    rc = dt_err(ctl, F,
      "error in Picture(\"%s\", NULL)",
      windowname);
	goto cleanup;
  }

/* -w takes triangular "close" button out of title bar
   -r means window cannot be resized
   -O removes outline from the frame
   -C means don't center the title
   -o means don't try to keep entire window on screen
*/
/*  options = "D-C-o;auBSMYxcN", */
/*  actions = "taXMRI", */

  WindowAt(size.height/2, size.width/2, "sSVH", NULL);
  win_id = WindowOpen(
    windowname,
    ye * VTPP,
    xe * HTPP,
    options = "D-C-o;uSN",
    actions = "",
    windowname, 0);

  if (win_id == 0)
  {
    rc = dt_err(ctl, F,
      "error in WindowOpen(\"%s\", %d, %d, %s, %s, %s, 0)",
      windowname,
      ye, xe,
      options, actions, windowname);
	goto cleanup;
  }

  {
	int i, n;
	int ids[32];
	ScreenFontList(&n, ids);
	for (i=0; i<n; i++)
	{
	  char face[QW_MAX_NAME_LEN];
	  char style[QW_MAX_NAME_LEN];
	  QW_REAL_EXTENT size;
	  ScreenFontInfo(ids[i], face, style, &size, NULL, NULL, NULL);
	  dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
        "%2d. id=0x%08lx face=\"%s\" style=\"%s\" size=%dx%d,%dx%d",  
        i, ids[i], face, style,
        size.x, size.y, size.w, size.h);
	}
  }

  SetFont("Tb", NULL, NULL, 0, 55);

  DT_C(tabrobot_gui_qnx_redraw,(		/* redraw screen 					*/
    param));

  DT_C(tabrobot_gui_qnx_eventloop,(		/* wait for button events 			*/
    param, ipc, win_id));
  
cleanup:
  if (ipc_initialized)
	DT_I(dtipc_unfind,(ctl, ipc));
  if (graphics_initialized)
	GraphicsClose(sid);					/* close connection to server		*/
  return rc;
#else
  return dt_err_notlinked(ctl, F, "QNX4 Windows");
#endif
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
