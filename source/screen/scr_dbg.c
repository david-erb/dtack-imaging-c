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
DT_RCSID("source/screen $RCSfile: scr_dbg.c,v $ $Revision: 1.3 $");
#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/lut8.h>
#include <dtack/svc.h>
#include <dtack/scr.h>

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1
dtscr_state_dbg(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_state_t *state,
  char *F,
  long mask,
  char *prefix)
{
  char state_string[64];
  char action_string[DTSCR_ACTION_STRING_MAX];

  dtscr_state_string(ctl, scr,
    state, 
    state_string, 
    sizeof(state_string));

  dtscr_action_string(ctl, scr,
    state->action, action_string);

  dt_dbg(ctl, F, mask,
    "%s%s: %s",
    prefix? prefix: "",
    state_string,
    action_string);      
}

/*..........................................................................
 *..........................................................................*/
void
DTCONFIG_API1
dtscr_states_dbg(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  dtscr_states_t *states,
  char *F,
  long mask)
{
  dtscr_state_t *s;

  for (s=states->head; s; s=s->next)
    dtscr_state_dbg(ctl, scr,
      s, F, mask, NULL);
}

/*..........................................................................
 *..........................................................................*/
void
DTCONFIG_API1
dtscr_list_dbg(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *F,
  long mask)
{
  int i = 0;
  dtlist_elem_t *elem;

  elem = DTLIST_ELEM_HEAD;
  while (elem)
  {
    dtlist_next(ctl, &scr->windowlist, elem, &elem);
    if (elem)
	{
      dt_dbg(ctl, F, mask, "%2d.%c%s", i,
        i == scr->cycle? '>': ' ',
        elem->name);
      i++;
	}
  }
  dt_dbg(ctl, F, mask, "%d windows", i);
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1
dtscr_all_dbg(
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *F,
  long mask)
{
  dt_dbg(ctl, F, mask, "default states/actions:");
  dtscr_states_dbg(ctl, scr,
    &scr->states_default, F, mask);
  dt_dbg(ctl, F, mask, "limits:");
  dtscr_limits_dbg(ctl, scr,
    F, mask);
  dt_dbg(ctl, F, mask, "list:");
  dtscr_list_dbg(ctl, scr,
    F, mask);
}

/*..........................................................................
 *..........................................................................*/
void
DTCONFIG_API1
dtscr_limits_dbg(						/* debug limits stuff				*/
  dt_ctl_t *ctl,
  dtscr_t *scr,
  char *F,
  long mask)
{

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
