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
  19981014 DE handle mouse event mappings different in hws systems
 *..........................................................................*/


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

/*..........................................................................
  CHANGES
  19980716 DE remove sunpro1 compiler warnings
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/screen $RCSfile: scr_act.c,v $ $Revision: 1.2 $");
#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/svc.h>
#include <dtack/scr.h>

#include <dtack/str.h>

static char NOKEYSTRING[2] = {(char)0xff, 0x00};

/*..........................................................................
| state1 is the template
| state2 is the current
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtscr_state_match(						/* determine if states match		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t *state1,
  dtscr_state_t *state2,
  int *match)
{
  *match = 1;							/* presume a match 					*/

  if (*match &&
      strlen(state1->windowname) &&
      strlen(state2->windowname) &&
      strcmp(state1->windowname, state2->windowname))
    *match = 0;

  if (*match &&
      strcmp(state1->keystring, NOKEYSTRING) &&
      strcmp(state2->keystring, NOKEYSTRING) &&
      strcmp(state1->keystring, state2->keystring))
    *match = 0;

  if (*match &&
      state1->left != -1 &&
      state2->left != -1 &&
      state1->left != state2->left)
    *match = 0;

  if (*match &&
      state1->middle != -1 &&
      state2->middle != -1 &&
      state1->middle != state2->middle)
    *match = 0;

  if (*match &&
      state1->right != -1 &&
      state2->right != -1 &&
      state1->right != state2->right)
    *match = 0;

#ifdef NONO
  if (*match &&
      state1->x != 0 &&		
      state2->x == 0 &&
      state1->y == 0)			
    *match = 0;

  if (*match &&
      state1->y != 0 &&
      state2->y == 0 &&
      state1->x == 0)
    *match = 0;
#endif

  if (*match &&
      state1->x != 0 &&					/* need both X and Y motion? 		*/
      state1->y != 0 &&
      (state2->x == 0 &&				/* but got neither motion? 			*/
       state2->y == 0))
    *match = 0;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_state_matches(					/* count matches for state in list	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states,
  dtscr_state_t *state,
  int *count)
{
  DT_F("dtscr_state_matches");
  dtscr_state_t *s;
  int match;
  int matches = 0;

  for (s=states->head; s; s=s->next)
  {
    DT_Q(dtscr_state_match,(ctl, scr,
      s, state, &match));
    matches += match;
  }
  
  *count = matches;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_state_free(	
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states)
{
  DT_F("dtscr_state_free");
  dtscr_state_t *s, *next;

  for (s=states->head; s; s=next)
  {
    next = s->next;
    DT_Q(dtos_free2,(ctl,
      s, F, "state structure"));
  }

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_state_add(						/* add new state					*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states,
  dtscr_state_t *state)
{
  state->next = states->head;
  states->head = state;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_state_dontcare(					/* make new don't-care state		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t *state)
{
  state->windowname[0] = '\0';
  strcpy(state->keystring, NOKEYSTRING);
  state->left = -1;
  state->middle = -1;
  state->right = -1;
  state->x = 0;
  state->y = 0;
  state->action = DTSCR_ACTION_UNSPECIFIED;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_state_make_dontcare(				/* make a new don't-care state		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t **pstate)
{
  DT_F("dtscr_state_make_dontcare");
  dtscr_state_t *state;

  DT_Q(dtos_malloc2,(ctl,				/* space for state					*/
    (void **)&state, sizeof(*state),
    F, "state structure"));

  DT_Q(dtscr_state_dontcare,(ctl,		/* init state to don't care			*/
    scr, state));

  *pstate = state;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_state_keystring_action(			/* add new state for keystring		*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states,
  char *keystring,
  dtscr_action_e action)
{
  DT_F("dtscr_state_keystring_action");
  dtscr_state_t *state;

  DT_Q(dtscr_state_make_dontcare,(		/* make a don't-care state			*/
    ctl, scr, &state));

  strcpy(state->keystring, keystring);	/* set the value					*/

  state->action = action;				/* action to be taken				*/

  DT_Q(dtscr_state_add,(ctl, scr,		/* add to list of states			*/
    states, state));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_state_default_actions(			/* make default states list			*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states)
{
  DT_F("dtscr_state_default_action");
  char s[2];

  if (!scr->have_window_manager)		/* window manager does mouse		*/
  {										/* mouse movement 					*/
    dtscr_state_t *state;
    DT_Q(dtscr_state_make_dontcare,(	/* make a don't-care state			*/
      ctl, scr, &state));
    state->x = 1;						/* take action on mouse movement	*/
    state->y = 1;				
    state->action = DTSCR_ACTION_MOVE;	/* action to be taken				*/
    DT_Q(dtscr_state_add,(ctl, scr,		/* add to list of states			*/
      states, state));
  }

  if (!scr->have_window_manager)		/* window manager does mouse		*/
  {										/* mouse double click 				*/
    dtscr_state_t *state;
    DT_Q(dtscr_state_make_dontcare,(	/* make a don't-care state			*/
      ctl, scr, &state));
    state->left = 2;					/* take action on button press		*/
    state->action = DTSCR_ACTION_LATCH;	/* action to be taken				*/
    DT_Q(dtscr_state_add,(ctl, scr,		/* add to list of states			*/
      states, state));
  }

  if (!scr->have_window_manager)		/* window manager does mouse		*/
  {										/* mouse button click 				*/
    dtscr_state_t *state;
    DT_Q(dtscr_state_make_dontcare,(	/* make a don't-care state			*/
      ctl, scr, &state));
    state->left = 1;					/* take action on button press		*/
    state->action =						/* action to be taken				*/
      DTSCR_ACTION_RAISE_ONE;
    DT_Q(dtscr_state_add,(ctl, scr,		/* add to list of states			*/
      states, state));
  }

  s[1] = '\0';

  s[0] = DTSCR_KEY_QUIT;
  DT_Q(dtscr_state_keystring_action,(	/* ^q - quit						*/
    ctl, scr, states, s, 
    DTSCR_ACTION_QUIT));

  s[0] = DTSCR_KEY_REFRESH;
  DT_Q(dtscr_state_keystring_action,(	/* ^l - refresh						*/
    ctl, scr, states, s, 
    DTSCR_ACTION_REFRESH));

  s[0] = DTSCR_KEY_DEBUG;
  DT_Q(dtscr_state_keystring_action,(	/* ^a - debug						*/
    ctl, scr, states, s, 
    DTSCR_ACTION_DEBUG));

  s[0] = DTSCR_KEY_CLOSE_ONE;
  DT_Q(dtscr_state_keystring_action,(	/* ^d - delete (close) one			*/
    ctl, scr, states, s, 
    DTSCR_ACTION_CLOSE_ONE));

  s[0] = DTSCR_KEY_ICONIFY_ONE;
  DT_Q(dtscr_state_keystring_action,(	/* ^i - iconify one					*/
    ctl, scr, states, s, 
    DTSCR_ACTION_ICONIFY_ONE));

  s[0] = DTSCR_KEY_COLORMAP_ONE;
  DT_Q(dtscr_state_keystring_action,(	/* ^v - colormap one				*/
    ctl, scr, states, s, 
    DTSCR_ACTION_COLORMAP_ONE));

  s[0] = DTSCR_KEY_ENTER;
  DT_Q(dtscr_state_keystring_action,(	/* enter - raise one				*/
    ctl, scr, states, s, 
    DTSCR_ACTION_RAISE_ONE));

  s[0] = DTSCR_KEY_UPARROW;
  DT_Q(dtscr_state_keystring_action,(
    ctl, scr, states, s, 
    DTSCR_ACTION_MOVE_UP_FAST));

  s[0] = DTSCR_KEY_DOWNARROW;
  DT_Q(dtscr_state_keystring_action,(
    ctl, scr, states, s, 
    DTSCR_ACTION_MOVE_DOWN_FAST));

  s[0] = DTSCR_KEY_LEFTARROW;
  DT_Q(dtscr_state_keystring_action,(
    ctl, scr, states, s, 
    DTSCR_ACTION_MOVE_LEFT_FAST));

  s[0] = DTSCR_KEY_RIGHTARROW;
  DT_Q(dtscr_state_keystring_action,(
    ctl, scr, states, s, 
    DTSCR_ACTION_MOVE_RIGHT_FAST));

  s[0] = DTSCR_KEY_UPARROW_FAST;
  DT_Q(dtscr_state_keystring_action,(
    ctl, scr, states, s, 
    DTSCR_ACTION_MOVE_UP));

  s[0] = DTSCR_KEY_DOWNARROW_FAST;
  DT_Q(dtscr_state_keystring_action,(
    ctl, scr, states, s, 
    DTSCR_ACTION_MOVE_DOWN));

  s[0] = DTSCR_KEY_LEFTARROW_FAST;
  DT_Q(dtscr_state_keystring_action,(
    ctl, scr, states, s, 
    DTSCR_ACTION_MOVE_LEFT));

  s[0] = DTSCR_KEY_RIGHTARROW_FAST;
  DT_Q(dtscr_state_keystring_action,(
    ctl, scr, states, s, 
    DTSCR_ACTION_MOVE_RIGHT));

  s[0] = DTSCR_KEY_LATCH;
  DT_Q(dtscr_state_keystring_action,(
    ctl, scr, states, s, 
    DTSCR_ACTION_LATCH));

  s[0] = DTSCR_KEY_CYCLE_NEXT;
  DT_Q(dtscr_state_keystring_action,(
    ctl, scr, states, s, 
    DTSCR_ACTION_CYCLE_NEXT));

  s[0] = DTSCR_KEY_CYCLE_PREV;
  DT_Q(dtscr_state_keystring_action,(
    ctl, scr, states, s, 
    DTSCR_ACTION_CYCLE_PREV));

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_state_action(						/* take action in state				*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t *state,
  dtscr_action_e action,
  int *quit)
{
  DT_F("dtscr_state_action");
  dt_rc_e rc = DT_RC_GOOD;

  switch(action)
  {
    case DTSCR_ACTION_UNSPECIFIED:
      rc = dt_err(ctl, F, "UNSPECIFIED action for event");
    break;
    case DTSCR_ACTION_NOOP:
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, "noop event detected");
    break;
    case DTSCR_ACTION_CLOSE_ONE:
      if (strlen(state->windowname))
	  {
        DT_GI(dtscr_window_destroy,
          (ctl, scr, 
           state->windowname));
        DT_GI(dtscr_refresh,(ctl,		/* refresh all remaining windows  	*/
          scr));
  	  }
    break;
    case DTSCR_ACTION_ICONIFY_ONE:
      if (strlen(state->windowname))
	  {
        DT_GI(dtscr_window_iconify,
          (ctl, scr,
           state->windowname));
        DT_GI(dtscr_refresh,(ctl,		/* refresh all remaining windows  	*/
          scr));
  	  }
    break;
    case DTSCR_ACTION_RAISE_ONE:
      if (strlen(state->windowname))
	  {
		if (scr->latchbuf)				/* something latched? 				*/
          DT_GI(dtscr_latch,(ctl, scr,	/* unlatch it 						*/
            state->windowname))
  	    else							/* nothing latched? 				*/
          DT_GI(dtscr_window_raise,(ctl,
            scr, state->windowname));
	  }
    break;
    case DTSCR_ACTION_RAISE_ALL:
      DT_GI(dtscr_raise_all,(ctl, scr));
    break;
    case DTSCR_ACTION_LOWER_ONE:
      if (strlen(state->windowname))
      DT_GI(dtscr_window_lower,(ctl, scr, state->windowname));
    break;
    case DTSCR_ACTION_LATCH:
      if (strlen(state->windowname))
      DT_GI(dtscr_latch,(ctl, scr, state->windowname));
    break;
    case DTSCR_ACTION_CYCLE_NEXT:
      DT_GI(dtscr_cycle,(ctl, scr, 1));
    break;
    case DTSCR_ACTION_CYCLE_PREV:
      DT_GI(dtscr_cycle,(ctl, scr, -1));
    break;
    case DTSCR_ACTION_MOVE:
      DT_GI(dtscr_cursor_move,(ctl, scr, state->windowname, state->x, state->y));
    break;
    case DTSCR_ACTION_MOVE_LEFT:
      DT_GI(dtscr_cursor_move,(ctl, scr, state->windowname, -1,  0));
    break;
    case DTSCR_ACTION_MOVE_RIGHT:
      DT_GI(dtscr_cursor_move,(ctl, scr, state->windowname,  1,  0));
    break;
    case DTSCR_ACTION_MOVE_DOWN:
      DT_GI(dtscr_cursor_move,(ctl, scr, state->windowname,  0,  1));
    break;
    case DTSCR_ACTION_MOVE_UP:
      DT_GI(dtscr_cursor_move,(ctl, scr, state->windowname,  0, -1));
    break;
    case DTSCR_ACTION_MOVE_LEFT_FAST:
      DT_GI(dtscr_cursor_move,(ctl, scr, state->windowname, -10,  0));
    break;
    case DTSCR_ACTION_MOVE_RIGHT_FAST:
      DT_GI(dtscr_cursor_move,(ctl, scr, state->windowname,  10,  0));
    break;
    case DTSCR_ACTION_MOVE_DOWN_FAST:
      DT_GI(dtscr_cursor_move,(ctl, scr, state->windowname,  0,  10));
    break;
    case DTSCR_ACTION_MOVE_UP_FAST:
      DT_GI(dtscr_cursor_move,(ctl, scr, state->windowname,  0, -10));
    break;
    case DTSCR_ACTION_COLORMAP_ONE:
      if (strlen(state->windowname))
	  {
        DT_GI(dtscr_window_colormap,(ctl, scr, state->windowname));
	  }
    break;
    case DTSCR_ACTION_DEBUG:
      dtscr_all_dbg(ctl, scr, "",		/* dump debug on console 			*/
        DT_DBG_MASK_ANY);
	break;
    case DTSCR_ACTION_REFRESH:
      DT_Q(dtscr_refresh,(ctl, scr));
    break;
    case DTSCR_ACTION_QUIT:
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, "quit event detected");
      *quit = *quit | DTSCR_QUIT1;
    break;
    default:
    {
      char action_string[DTSCR_ACTION_STRING_MAX];
      dtscr_action_string(ctl, scr,
        action, action_string);
      rc = dt_err(ctl, F, 
        "don't know what do to for action %s",
        action_string);
    }
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_state_actions(					/* take action in matching states	*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states,
  dtscr_state_t *state,
  int *quit)
{
  DT_F("dtscr_state_actions");
  dtscr_state_t *s;
  int match;

  for (s=states->head;
       s && !(*quit);
       s=s->next)
  {
    DT_Q(dtscr_state_match,(ctl,		/* see if state in list matches us	*/
      scr, s, state, &match));
    if (match)
    {
      dtscr_state_dbg(ctl, scr, s,
        F, DT_DBG_MASK_ASYNC,
        "matches: ");
      DT_Q(dtscr_state_action,(			/* take action said by list			*/
        ctl, scr, state,
        s->action, quit));
    }
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_action_string(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_action_e action,
  char *string)
{
  switch(action)
  {
#   define SET(A) case DTSCR_ACTION_ ## A: strcpy(string, #A); break
#   define SET2(A, D) case DTSCR_ACTION_ ## A: strcpy(string, D); break
    SET2(CLOSE_ONE, "close image");
    SET2(COLORMAP_ONE, "match image colormap");
    SET2(RAISE_ONE, "raise image");
    SET(RAISE_FAMILY);
    SET(RAISE_ALL);
    SET2(LOWER_ONE, "lower image");
    SET(LOWER_FAMILY);
    SET(LOWER_ALL);
    SET2(ICONIFY_ONE, "iconify image");
    SET(ICONIFY_FAMILY);
    SET(ICONIFY_ALL);
    SET2(MOVE, "move image");
    SET2(MOVE_UP, "move image up");
    SET2(MOVE_DOWN, "move image down");
    SET2(MOVE_LEFT, "move image left");
    SET2(MOVE_RIGHT, "move image right");
    SET2(MOVE_UP_FAST, "move image up fast");
    SET2(MOVE_DOWN_FAST, "move image down fast");
    SET2(MOVE_LEFT_FAST, "move image left fast");
    SET2(MOVE_RIGHT_FAST, "move image right fast");
    SET2(DEBUG, "print debug on console");
    SET2(REFRESH, "raise all images");
    SET2(LATCH, "pick up/set down");
    SET2(CYCLE_NEXT, "go to next image");

#ifdef NONO
  DT_Q(dtscr_state_make_dontcare,(		/* make a don't-care state			*/
    ctl, scr, &state));
  state->action = DTSCR_ACTION_NOOP;	/* ignore all actions				*/

  DT_Q(dtscr_state_add,(ctl, scr,		/* add to list of states			*/
    states, state));
#endif

#ifdef NONO
  DT_Q(dtscr_state_make_dontcare,(		/* make a don't-care state			*/
    ctl, scr, &state));
  state->action = DTSCR_ACTION_NOOP;	/* ignore all actions				*/

  DT_Q(dtscr_state_add,(ctl, scr,		/* add to list of states			*/
    states, state));
#endif
    SET2(CYCLE_PREV, "go to previous image");
    SET2(QUIT, "quit viewing");
    case DTSCR_ACTION_UNSPECIFIED: strcpy(string, ""); break;
    SET(NOOP);
#   undef SET
#   undef SET2
    default:
      sprintf(string, "0x%04x", action);
  }
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtscr_state_string(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t *state,
  char *string,
  int lmax)
{
  DT_F("dtscr_state_string");
  int l;

  string[0] = '\0';

  l = strlen(string);
  if (strlen(state->windowname))		/* state is specific to a window?	*/
    DT_Q(dtstr_printf,(ctl, 
      string+l,
      lmax-l,
      "\"%s\" ",
      state->windowname));

  l = strlen(string);
  if (strcmp(state->keystring,			/* state involves on a keystring?   */
         NOKEYSTRING))
  {
    char *extra;
    DT_Q(dtstr_printable,(ctl,
      state->keystring, -1,
      string+l,
      lmax-l));
    if (state->keystring[0] ==
        DTSCR_KEY_LEFTARROW)
      extra = " or left arrow ";
    else
    if (state->keystring[0] ==
        DTSCR_KEY_RIGHTARROW)
      extra = " or right arrow ";
    else
    if (state->keystring[0] ==
        DTSCR_KEY_UPARROW)
      extra = " or up arrow ";
    else
    if (state->keystring[0] ==
        DTSCR_KEY_DOWNARROW)
      extra = " or down arrow ";
    else
      extra = " ";
    l = strlen(string);
    DT_Q(dtstr_printf,(ctl, 
      string+l,
      lmax-l,
      extra));
  }

  l = strlen(string);
  if (state->left == 1 &&				/* state is left button down? 		*/
	  state->middle == -1 &&
	  state->right == -1)
    DT_Q(dtstr_printf,(ctl, 
      string+l, lmax-l,
      "mouse click"))
  else
  if (state->left == 2 &&				/* state is left double click? 		*/
	  state->middle == -1 &&
	  state->right == -1)
    DT_Q(dtstr_printf,(ctl, 
      string+l, lmax-l,
      "mouse double click"))
  else
  if (state->left != -1 ||				/* state involves mouse buttons?	*/
      state->middle != -1 ||
      state->right != -1)
    DT_Q(dtstr_printf,(ctl, 
      string+l,
      lmax-l,
      "mouse buttons=%d,%d,%d ",
      state->left, 
      state->middle,
      state->right));

  l = strlen(string);
  if (state->x != 0 &&					/* state is any mouse movement?		*/
      state->y != 0)
    DT_Q(dtstr_printf,(ctl, 
      string+l,
      lmax-l,
      "mouse movement "))
  else
  if (state->x != 0 ||					/* state involves a mouse location?	*/
      state->y != 0)
    DT_Q(dtstr_printf,(ctl, 
      string+l,
      lmax-l,
      "(x=%d,y=%d) ",
      state->x, 
      state->y));

  l = strlen(string);
  if (l && string[l-1] == ' ')			/* nuke a trailing space		    */
    string[l-1] = '\0';

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
