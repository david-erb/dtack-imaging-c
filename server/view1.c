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
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("server $RCSfile: view.c,v $ $Revision: 1.4 $");
#include <dtack/cstack.h>

#include <dtack/lut8.h>

#include <dtack/os.h>
#include <dtack/heap.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>
#include <dtack/xchg.h>

#include <dtack/win_r.h>					/* needed inside dtwins.h only 		*/
#include <dtack/wins.h>						/* we are an svc-on-scr server 		*/

#include <dtack/str.h>
#include <dtack/mem.h>

#include <dtack/view.h>

#define INTRODUCE_X0 (0)
#define INTRODUCE_Y0 (scr->ye-1)
#define INTRODUCE_GRAVITY DTVIEW_GRAVITY_SW
#define INTRODUCE_STRING "\
     Dtack Image Viewer \n\
     Copyright (C) 1996 \n\
   Dtack Imaging Software \n\
 press the '?' key for help \
"

/*..........................................................................*/
static
dt_rc_e
dtview_introduce(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *title)
{
  DT_F("dtview_introduce");
  DT_Q(dtview_panel,(ctl, scr,
    title,
    INTRODUCE_STRING,
    INTRODUCE_X0,
    INTRODUCE_Y0,
    INTRODUCE_GRAVITY));
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
dtview_memory(
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtview_memory");
  char *s = NULL;
  dt_rc_e rc;
  
# define SMAX 2048
  DT_C(dtos_malloc2,(ctl,
    (void **)&s, SMAX,
    F, "panel string"));
  s[0] = '\0';							/* clear string 					*/

{
  dtmemsource_t *memsource = 
    (dtmemsource_t *)ctl->memsource;
  int i;
  for (i=0; i<DT_CTL_MEMSOURCES; i++)
  if (memsource[i].alloc)				/* this one is allocated? 			*/
    if (memsource[i].maxusage)
      DT_C(dtstr_printf,(ctl, 
        s+strlen(s), SMAX-strlen(s),
        "%8s used %d chunks,"
        " %ld bytes of %ld (%ld%%)\n",
        memsource[i].name,
        memsource[i].nallocs,
        memsource[i].usage,
        memsource[i].maxusage,
        (100L*memsource[i].usage) /
        memsource[i].maxusage))
	else
      DT_C(dtstr_printf,(ctl, 
        s+strlen(s), SMAX-strlen(s),
        "%8s used %d chunks,"
        " %ld bytes\n",
        memsource[i].name,
        memsource[i].nallocs,
        memsource[i].usage))
}

  if (ctl->heap != NULL)
  {
	long blocks, bytes;
    DT_C(dtheap_outstanding,(ctl,		/* query heap outstanding			*/
       ctl->heap, &blocks, &bytes));
    DT_C(dtstr_printf,(ctl, 
      s+strlen(s), SMAX-strlen(s),
      "%8s used %ld chunks,"
      " %ld bytes, highwater %ld\n",
      "heap", blocks, bytes));
  }

{
# define MEMAVAIL 1000000UL
  unsigned long memavail1;
  unsigned long memavail2;
  unsigned long nchunks1;
  unsigned long nchunks2;

  DT_C(dtheap_avail,(ctl, ctl->heap, 256,
    MEMAVAIL, &nchunks1));
  memavail1 = 256UL *
    (unsigned long)nchunks1;

  DT_C(dtheap_avail,(ctl, ctl->heap, 16384,
    MEMAVAIL, &nchunks2));
  memavail2 = 16384UL *
    (unsigned long)nchunks2;
  
  if (memavail1 < MEMAVAIL ||
	  memavail2 < MEMAVAIL)
  {
      DT_C(dtstr_printf,(ctl, 
        s+strlen(s), SMAX-strlen(s),
        "%8s has %ld %d-byte chunks"
        " (%lu bytes) available\n",
        "heap", nchunks1, 
        256, memavail1));
      DT_C(dtstr_printf,(ctl, 
        s+strlen(s), SMAX-strlen(s),
        "%8s has %ld %d-byte chunks"
        " (%lu bytes) available\n",
        "heap", nchunks2, 
        16384, memavail2));
  }
  else
    DT_C(dtstr_printf,(ctl, 
      s+strlen(s), SMAX-strlen(s),
	  "%8s has more than %lu bytes available\n",
      "heap", MEMAVAIL));
}

{
  unsigned long stack1;
  unsigned long stack_highwater;

  dtcstack_query(ctl,
    &stack1, &stack_highwater);
  DT_C(dtstr_printf,(ctl, 
    s+strlen(s), SMAX-strlen(s),
    "%8s used %lu bytes, highwater %lu\n",
    "stack", stack1, stack_highwater));
}  

  s[strlen(s)-1] = '\0';				/* nuke trailing newline 			*/

  DT_C(dtview_panel,(ctl, scr,
    DTVIEW_MEMORY_TITLE,
    s,
    DTVIEW_MEMORY_X0,
    DTVIEW_MEMORY_Y0,
    DTVIEW_MEMORY_GRAVITY));

cleanup:
  if (s != NULL)
    DT_I(dtos_free2,(ctl, s,
    F, "panel string"));
  return rc;
#undef SMAX
}

/*..........................................................................*/
static
dt_rc_e
dtview_help(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char **help_string)
{
  DT_F("dtview_help");
  dtscr_states_t *states = &scr->states_default;
  dtscr_state_t *state;
  char state_string[32];
  char action_string[32];
  int n;
  int l;
  char format[32];
  char *help;

  n = 0;
  l = 0;
  for (state=states->head; 
       state; 
       state=state->next)
  {
    DT_Q(dtscr_state_string,(ctl, scr,	/* fetch state string			    */
      state, 
      state_string, 
      sizeof(state_string)));
    DT_Q(dtscr_action_string,(ctl, scr,	/* fetch action string			    */
      state->action,
      action_string));
    n++;								/* count lines in help			    */
    l = DT_MAX(l,						/* remember longest state string    */
      (int)strlen(state_string));
  }

  n = (n+3) *							/* size of entire help string	    */
    (l+5+DTSCR_ACTION_STRING_MAX+1)+1;

  DT_Q(dtos_malloc2,(ctl,				/* space for entire help string	    */
    (void **)&help, n,
    F, "help window string"));
  *help_string = help;

  sprintf(format, " %%-%ds - %%s \n", l);

  *help = '\0';							/* clear help string			    */
  for (state=states->head;				/* pass over states again		    */
       state; 
       state=state->next)
  {
    DT_Q(dtscr_state_string,(ctl, scr,	/* fetch state string			    */
      state, 
      state_string, 
      sizeof(state_string)));
    DT_Q(dtscr_action_string,(ctl, scr,	/* fetch action string			    */
      state->action,
      action_string));
    sprintf(							/* marry state and action in help	*/
      help+strlen(help),
      format,
      state_string,
      action_string);
  }

  sprintf(help+strlen(help),
    format, DTVIEW_SAVE_KEY, "save windows");

  sprintf(help+strlen(help),
    format, DTVIEW_MEMORY_KEY, "memory report");

  sprintf(help+strlen(help),
    format, DTVIEW_HELP_KEY, "this help");

  help[strlen(help)-1] = '\0';			/* nuke trailing newline 			*/
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dtview_keys(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  const char *output,
  const char *help_string)
{
  DT_F("dtview_keys");

    if (scr->last_event.keys[0] == 		/* user hit save key?			    */
        DTVIEW_SAVE_KEY[0])
	{
      DT_Q(dtview_save,(ctl, scr,		/* display save window			    */
        output));
	}
    else								
    if (scr->last_event.keys[0] == 		/* user hit memory key?			    */
        DTVIEW_MEMORY_KEY[0])
	{
      DT_Q(dtview_memory,(ctl, scr));	/* display memory window		    */
	}
    else								
    if (scr->last_event.keys[0] == 		/* user hit help key?			    */
        DTVIEW_HELP_KEY[0])
	{
      DT_Q(dtview_panel,(ctl, scr,		/* display help window			    */
        DTVIEW_HELP_TITLE,
        help_string,
        DTVIEW_HELP_X0,
        DTVIEW_HELP_Y0,
        DTVIEW_HELP_GRAVITY));
	}
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dtview_resize(
  dt_ctl_t *ctl,
  dtscr_t *scr)
{
  DT_F("dtview_resize");
  dtlist_elem_t *elem;
  dtimage_t _image, *image = &_image;
  dtimage_t _resized, *resized = &_resized;
  char name[DT_WINDOWNAME_MAXLEN];

  double dx, dy, d;
  dt_rc_e rc = DT_RC_GOOD;

  image->xe = 0;
  resized->xe = 0;
  
  if (scr->last_event.type !=			/* not the right type of message? 	*/
	  DT_EVENT_RESIZED)
    goto cleanup;						/* ignore it 						*/

  elem = DTLIST_ELEM_HEAD;
  while (elem)
  {
	DT_C(dtlist_next,(ctl,				/* fetch next window in list        */
      &scr->windowlist, 
      elem, &elem));
	if (elem && 						/* not the last in the list?        */
		scr->last_event.id == 
		((dtscr_win_t *)				/* and this is the window?			*/
	      elem->user)->id)
	break;
  }

  if (elem == NULL)						/* not on a window?					*/
    goto cleanup;

  dtstr_printf(ctl, name, sizeof(name),	/* capture the window name 			*/
    "%s", elem->name);

  DT_C(dtxchg_scr_import_image,(ctl,	/* get window contents 				*/
    scr, name, image, 0, 2));

  dx = (double)image->xe / 
    (double)scr->last_event.xe;
  dy = (double)image->ye / 
    (double)scr->last_event.ye;

  d = DT_MIN(dx, dy);					/* keep same aspect ratio 			*/

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "dx %d->%d is scale %0.3f,"
    " dy %d->%d is scale %0.3f,"
    " choosing %d",
    image->xe, scr->last_event.xe,
    dx,
    image->ye, scr->last_event.ye,
    dy, d);

  DT_C(dtimage_resample,(ctl,			/* change window size 				*/
    image, d, d, "bilinear",
    resized));
  
  DT_C(dtxchg_scr_export_image,(ctl,	/* output on top of old window		*/
    scr, name, resized, 
    DTIMAGE_NOPOS, DTIMAGE_NOPOS));

cleanup:
  if (resized->xe != 0)
    DT_I(dtimage_free,(ctl, resized));
  
  if (image->xe != 0)
    DT_I(dtimage_free,(ctl, image));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dtview_work(
  int argc,
  char *argv[],
  dtparam_t *param,
  dtsvc_t *svc,			
  dtview_loop_f *loop)
{
  DT_F("dtview_work");
  dt_ctl_t *ctl = &param->ctl;
  char *help_string = NULL;
  dtscr_t *scr = NULL;
  dtwins_t wins;
  int have_wins = 0;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,				/* space for big screen structure 	*/
    (void **)&scr, sizeof(*scr),
    F, "scr structure"));

  DT_G(dtscr_create,(ctl, scr,			/* open screen						*/
    param->screen));
  if (rc != DT_RC_GOOD)					/* unable to open screen? 			*/
  {
    DT_I(dtos_free2,(ctl, scr,
      F, "scr structure"));
	scr = NULL;
	goto cleanup;
  }

  DT_C(dtview_help,(ctl, scr,			/* make the help string				*/
    &help_string));

  if (strstr(param->variant,			/* introduce panel not disabled? 	*/
		"nointro") == NULL)
  DT_C(dtview_introduce,(ctl, scr,		/* introduce the program			*/
    DTVIEW_INTRODUCE_TITLE));

  DT_C(dtscr_state_keystring_action,	/* return on help character 		*/
    (ctl, scr, &scr->states_default,
     DTVIEW_HELP_KEY,
     DTSCR_ACTION_QUIT));

  DT_C(dtscr_state_keystring_action,	/* return on memory character 		*/
    (ctl, scr, &scr->states_default,
     DTVIEW_MEMORY_KEY, 
     DTSCR_ACTION_QUIT));

  DT_C(dtscr_state_keystring_action,	/* return on save character 		*/
    (ctl, scr, &scr->states_default,
     DTVIEW_SAVE_KEY, 
     DTSCR_ACTION_QUIT));

  DT_C(dtwins_init,(ctl, &wins, scr));	/* prepare to serve 				*/
  have_wins = 1;

  DT_C(loop,(param, &wins, svc,			/* loop for input 					*/
    param->output, help_string));

cleanup:
  if (have_wins)
    DT_I(dtwins_uninit,(ctl, &wins));	/* quit serving						*/

  if (help_string)
    DT_I(dtos_free2,(ctl, 
      help_string,
      F, "help window string"));

  if (scr != NULL)
  {
    DT_I(dtscr_destroy,(ctl, scr));		/* close window system				*/
    DT_I(dtos_free2,(ctl, scr,
      F, "scr structure"));
  }

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
