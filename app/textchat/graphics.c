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
| textchat_* - textchat internal routine
| 
|.DESCRIPTION
| Undocumented.
| 
| END
 *..........................................................................*/

#include <textchat.h>
DT_RCSID("app/textchat $RCSfile: request.c,v $ $Revision: 1.8 $");

#ifdef DTCONFIG_IMPLIED_QNX4

#ifndef DTDEPEND
#include <ncierr.h>
#include <ncisys.h>
#include <nciwin.h>
#include <tokm/tokm.h>
#endif

#define CD_MAX (16)
#define TITLE "NetChat"
#define NAMELEN_MAX (32)

#define NOPARTNER "Select node first."
#define UNREACHABLE "Node unreachable."
#define BADSELECT "Invalid selection."
#define CONTACTED "Node contacted."

static
struct {
  int toggle;
  TOKM cd[CD_MAX];
  int is_selectable;
  char menu[CD_MAX*(NAMELEN_MAX+6)];
} global;

/*..........................................................................
 *..........................................................................*/
static
void
graphics_center(void)
{
  DialogAt(250,250,"sSVH",NULL);
}

/*..........................................................................
 * initialize the menu string which lists all possible partners
 * the list of possible partners is gotten from the token manager
 *..........................................................................*/
static
dt_rc_e 
textchat_graphics_init_menu(
  textchat_t *textchat)
{
  DT_F("textchat_graphics_init_menu");
  dt_ctl_t *ctl = textchat->ctl;
  TOKM tok,*tp = &tok;
  TOKM *cdp;
  int n = 0;
  int i;
  
  while (1) 
  {
	int trc;
	trc = tokm_get_cindx("AUD", n, tp); 
    if (trc == -1)
	{
      return dt_err(ctl, F,
        "token manager is not running");
	}
	if (trc != 0) 
      break;
	memmove(global.cd + n,tp,sizeof(TOKM));
	n++;
  }

  for (i = 0; i < n; i++)				/* find ourselves first 			*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_ALIMITS,
      "%d. %s", i, global.cd[i].name);
	if (!strcmp(global.cd[i].name,		/* this is us?						*/
		   textchat->ident))
	  break;
  }
  
  if (i == n)							/* we're not in the list? 			*/
  {
	graphics_center();
	return dt_err(ctl, F,
      "Sorry, no audio is"
      " configured for %s.", 
      textchat->ident);
  }
  if (n == 1)							/* nobody to talk to? 				*/
  {
	graphics_center();
	return dt_err(ctl, F,
      "Sorry, no other node"
      " has audio configured.");
  }
  else									/* there are some partners? 		*/
  {
	char *mr,tm[64];
	int nc = 0,fo;
	*global.menu = 0;
	for (i=0; i<n; i++) 
	{
	  if (strcmp(global.cd[i].name,		/* this partner is not us? 			*/
			textchat->ident))
	  {
		sprintf(tm,"%s|%s;",			/* add partner's name to list 		*/
          global.cd[i].name,
          global.cd[i].name);
		strcat(global.menu,tm);
		if (!nc) fo = i;
		nc++;
	  }
	}
	if (nc == 1) 
	{
	  strcpy(textchat->partner_contents, global.cd[fo].name);
	} 
    else
	{
	  strcpy(textchat->partner_contents, "");
	}
  }
  return DT_RC_GOOD;
}

/*..........................................................................*
 *..........................................................................*/

static
void
update_face(
  char *name,
  int mouth,
  int col) 
{
  int other = (mouth) ? 0 : 1;
  char tb[25];
  sprintf(tb,"%sF%d",name,other);
  ChangeOptions(tb,"X");
  ChangeColor(tb,col);
  sprintf(tb,"%sF%d",name,mouth);
  ChangeOptions(tb,"-X");
  ChangeColor(tb,col);
}

/*..........................................................................
 * make partner-selection button active or not
 * it should not be active when we are in active conversation
 * it should be active when we are idle
 *..........................................................................*/
static
void
selectable(
  int flag)
{
  if (flag == global.is_selectable)
    return;
  if (flag == 0)
    ChangeOptions("partner", ";-D");
  else
    ChangeOptions("partner", ";D");
  global.is_selectable = flag;
}

/*..........................................................................
 *..........................................................................*/
static
const char *
partner_select()
{
  const char *selection;
  if (!global.is_selectable)			/* we are in active conversation?	*/
    return NULL;

  DialogAt(0, 0, "eE", "partner");		/* align menu with partner button 	*/
  
  selection = Menu(NULL,
    "Select who to chat with", 
     1, global.menu, NULL);
  
  return selection;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_graphics_breathe(
  textchat_t *textchat)
{
  DT_F("textchat_graphics_breathe");
  dt_ctl_t *ctl = textchat->ctl;

  dt_dbg(ctl, F, DT_DBG_MASK_ANY, "breath state %c", textchat->state);

  switch (textchat->state)				/* depending on program state 		*/
  {
	case TEXTCHAT_STATE_STARTING:		/* starting? 						*/
	case TEXTCHAT_STATE_IDLING:			/* idle? 							*/
	{
	  update_face("left",0,BLUE);
	  update_face("right",0,BLUE);
	  ChangeState("talk", 0);
	  ChangeState("listen", 0);
	  global.toggle = 1;
      selectable(1);					/* now allow partner selection		*/
	}
	break;
	case TEXTCHAT_STATE_TALKING:		/* talking? 						*/
	{
	  update_face("left",0,BLUE);
	  ChangeState("talk", 1);
	  ChangeState("listen", 0);
	  update_face("right",
        global.toggle&1, BLACK);
	  global.toggle++;
      selectable(0);					/* don't allow partner selection	*/
	}
	break;
	case TEXTCHAT_STATE_LISTENING:		/* listening? 						*/
	{
	  update_face("right",0,BLUE);
	  ChangeState("talk", 0);
	  ChangeState("listen", 1);
	  update_face("left",
        global.toggle&1,BLACK);
	  global.toggle++;
      selectable(0);					/* don't allow partner selection	*/
	}
	break;
  }

  return DT_RC_GOOD;
}
#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_graphics_update(
  textchat_t *textchat)
{
  DT_F("textchat_graphics_update");
  dt_ctl_t *ctl = textchat->ctl;

  if (!TEXTCHAT_HAVE_GRAPHICS(textchat))
    return DT_RC_GOOD;

# ifdef DTCONFIG_IMPLIED_QNX4
  if (textchat->update)					/* screen needs redrawing? 			*/
  {
	ChangeText("message",
      textchat->message_contents,
      0, -1, 0);
    ChangeText("partner",				/* change partner button text 		*/
	  textchat->partner_contents,
	  0, -1, 0);
  }
  else									/* screen doesn't need redrawing? 	*/
  {
	DT_Q(textchat_graphics_breathe,(	/* draw breathing faces 			*/
      textchat));
  }
  
  Draw();

# endif

  return DT_RC_GOOD;
}

/*..........................................................................
 * handle incoming request from ipc
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_graphics_poll(
  textchat_t *textchat,
  char **request)
{
  DT_F("textchat_graphics_poll");
  dt_ctl_t *ctl = textchat->ctl;
# ifdef DTCONFIG_IMPLIED_QNX4
  pid_t pid;
  EVENT_MSG msg;
  int action;
  const char *command = NULL;
  char *s = NULL;

  if (!TEXTCHAT_HAVE_GRAPHICS(textchat))
    return DT_RC_GOOD;

  if (!EventWaiting())					/* no graphics events pending? 		*/
     return DT_RC_GOOD;

  pid = GetEvent(0, &msg, sizeof(msg));

  action = Event(&msg);					/* assume windows event 			*/
  if (action != 0)						/* action is a windows event? 		*/
  {
	if (action == CLICK)
	{
	  command = msg.hdr.key;
	  if (command-(char *)&msg == 38)	/* old version of windows? 			*/
	    command -= 2;
	}
  }

  dt_dbg(ctl, F, DT_DBG_MASK_ANY,
    "pid=%d, action=0x%02x, command=%s offset=%d",
    pid, action, command? command: "NULL",
    command-(char *)&msg);

# define COMMAND(T) \
    (command != NULL && \
     !strncmp(command, #T, \
       strlen(#T)))

  if (COMMAND(quit))					/* user clicked quit? 				*/
  {
	s = "exit";
  }
  else
  if (COMMAND(talk))					/* user clicked talk? 				*/
  {
	if (textchat->state ==	
		TEXTCHAT_STATE_TALKING)
	  s = "idle";
	else
	if (textchat->state ==	
		TEXTCHAT_STATE_LISTENING)
	  s = "talk";
	else
	if (textchat->state ==	
		TEXTCHAT_STATE_IDLING)
	  s = "talk";
  }
  else
  if (COMMAND(listen))					/* user clicked listen? 			*/
  {
	if (textchat->state ==	
		TEXTCHAT_STATE_LISTENING)
	  s = "idle";
	else
	if (textchat->state ==	
		TEXTCHAT_STATE_TALKING)
	  s = "listen";
	else
	if (textchat->state ==	
		TEXTCHAT_STATE_IDLING)
	  s = "listen";
  }
  else
  if (COMMAND(partner))					/* user clicked partner button? 	*/
  {
	const char *partner = partner_select();
	if (partner != NULL)
      DT_Q(dtos_strdup3,(ctl, 
        "GET /partner?", partner, "", 
        request));
  }

  if (s != NULL)
  {
    DT_Q(dtos_strdup3,(ctl, 
      "GET /", s, "", request));
  }

  if (*request != NULL)
	dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, "request=%s", *request);
  else
	dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, "graphics event ignored");
  
# endif

  return DT_RC_GOOD;
}

/*..........................................................................
 * initialize textchat structure
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_graphics_init(
  textchat_t *textchat,
  dtparam_t *param)
{
  DT_F("textchat_graphics_init");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;
# ifdef DTCONFIG_IMPLIED_QNX4
  int epid, ewid;

  SetName("NETCHAT", NULL);
  if (GraphicsOpen(NULL) == NULL)
    return dt_err(ctl, F,
      "GraphicsOpen failed");

  DT_Q(textchat_graphics_init_menu,(
    textchat));

#  define PICTFILE "/user/local/netchat.pict"
  epid = PictureOpen(NULL, PICTFILE,
    NULL, 0, 0, NULL, "R");
  if (epid == 0)
    return dt_err(ctl, F,
	  "unable to open %s", PICTFILE);

  ChangeText("self",
    textchat->ident, 0, -1, 0);
  ChangeText("partner",
    textchat->partner_contents, 0, -1, 0);
  ChangeText("message",
    "", 0, -1, 0);

  WindowAt(param->x, param->y, "sSHV", NULL);
  ewid = WindowOpen("netchat",0,0,"-rTDltf;NmksStx:/user/local/netchat_icon.pict",
    "bMX",NULL, epid);
  if (ewid == 0)
    return dt_err(ctl, F,
	  "unable to open window");

  textchat->graphics_initialized = 1;
# endif

  return rc;
}

/*..........................................................................
 * uninitialize textchat structure
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
textchat_graphics_uninit(
  textchat_t *textchat)
{
  DT_F("textchat_graphics_uninit");
  dt_ctl_t *ctl = textchat->ctl;
  dt_rc_e rc = DT_RC_GOOD;

# ifdef DTCONFIG_IMPLIED_QNX4

  if (textchat->graphics_initialized)
  {
	textchat->graphics_initialized = 0;
  }
# endif

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
