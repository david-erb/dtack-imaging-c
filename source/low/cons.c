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
  19980716 DE remove sunpro1 compiler warnings
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

#ifndef DT_INMAIN
#include <dtack/base.h>
DT_RCSID("source/inter $RCSfile: cons.c,v $ $Revision: 1.2 $");

#include <dtack/cons.h>
#include <dtack/os.h>
#endif

#define INUSE(I) (cons->arg[I] != NULL)

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtcons_init(
  dt_ctl_t *ctl)
{
  DT_F("dtcons_init");
  dtcons_t *cons;

  DT_Q(dtos_malloc2,(ctl,				/* space for the cons structure		*/
    (void **)&cons,
    sizeof(*cons),
    F, "cons structure"));

  DT_MEMSET((void *)cons, 0, sizeof(*cons));

  cons->maxcons = DTCONS_MAXCONS;

  cons->update = dtcons_update;

  cons->curr = -1;						/* initially no current cons		*/

	cons->err_string = ctl->err_string;	/* grab environment console saved 	*/
	cons->dbg_string = ctl->dbg_string;
	cons->say_string = ctl->say_string;
	cons->errto = ctl->errto;
	cons->dbgto = ctl->dbgto;
	cons->sayto = ctl->sayto;

		dt_dbg_trace(ctl, F,
          DT_RCSID_STRING, __LINE__, 
          "saving pointers in console,"
          " dbg_string 0x%08lx, dbgto 0x%08lx",
          cons->dbg_string, cons->dbgto);

  ctl->cons = cons;						/* basic control					*/

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtcons_uninit(
  dt_ctl_t *ctl)
{
  DT_F("dtcons_uninit");
  dtcons_t *cons = (dtcons_t *)ctl->cons;
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  if (cons == NULL)
    return rc;

  for (i=0; i<cons->maxcons; i++)		/* run over all slots				*/
    if (INUSE(i))						/* this slot is in use?				*/
	{
      dt_dbg_trace(ctl, F,
        DT_RCSID_STRING, __LINE__, 
        "sending DTCONS_OP_CLOSE"
        " to 0x%08lx",
        cons->arg[i]);
      DT_I((cons->opfunc[i]),(ctl,		/* send it a close message			*/
        DTCONS_OP_CLOSE, 
        cons->arg[i], NULL));
	}

  DT_I(dtos_free2,(ctl,					/* free cons structure				*/
    cons,
    F, "cons structure"));

  ctl->cons = NULL;

  return rc;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtcons_add(
  dt_ctl_t *ctl,
  const char *tag,
  dtcons_opfunc_f *opfunc,
  void *arg)
{
  DT_F("dtcons_add");
  dtcons_t *cons = (dtcons_t *)ctl->cons;
  char pass[DTCONS_PASSMAX];
  int i;

  if (cons == NULL)						/* not ready for this?				*/
    return DT_RC_GOOD;

  if (strlen(tag) > DTCONS_TAG_MAXLEN)
    return dt_err(ctl, F,
      "tag \"%s\" too long (%d)",
      tag, DTCONS_TAG_MAXLEN);

  for (i=0; i<cons->maxcons; i++)		/* find an unused entry				*/
    if (!INUSE(i))
      break;
  if (i == cons->maxcons)				/* no unused entries				*/
    return dt_err(ctl, F,
      "too many cons (%d)",
      cons->maxcons);

  cons->opfunc[i] = opfunc;				/* remember stuff in slot			*/
  cons->arg[i] = arg;
  strcpy(cons->tag[i], tag);

  if (cons->curr != -1)					/* this is not the first one?		*/
  DT_Q((cons->opfunc[cons->curr]),		/* tell old one we are leaving		*/
    (ctl, DTCONS_OP_LEAVE,
     cons->arg[cons->curr], 
     pass));

  cons->curr = i;						/* set the new one					*/

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtcons_search(							/* search, no error if not found    */
  dt_ctl_t *ctl,
  const char *tag,
  dtcons_opfunc_f **opfunc,
  void **arg)
{
  dtcons_t *cons = (dtcons_t *)ctl->cons;
  int i;

  if (cons == NULL)						/* not ready for this?				*/
    return DT_RC_GOOD;

  for (i=0; i<cons->maxcons; i++)		/* find the tag						*/
    if (INUSE(i) &&
        strstr(cons->tag[i], tag))
	{
      if (opfunc)
        *opfunc = cons->opfunc[i];
      if (arg)
        *arg = cons->arg[i];
	}

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
int next(
  dtcons_t *cons,
  int curr,
  int dir)
{
  int i, j;
  for (i=1; i<=cons->maxcons; i++)		/* find next unused entry			*/
  {
    j = (curr+dir*i) %
      cons->maxcons;
    if (j < 0)
      j += cons->maxcons;
    if (INUSE(j))
      break;
  }
  return j;
}

/*..........................................................................
| Returns value of nth consecutive tag.
| 
| Used by dialog implementation to extract list of window tags.
| For example, dtdialog_text makes a menu bar at the top listing
| the tags of each open cons.
| As another example, dtdialog_msw has a pull-down menu in which
| each open cons has an entry, with a check-mark on the current one.
|
| There are cases where a dialog implementation's input method can
| detect a request to go to another dialog.  
| The cons must be notified and will actually cause the goto to take place.
| The implementation notifies the cons be returning a key value
| of DT_KEY_JUMP+~nth~.
| The value of ~nth~ will be passed to dtcons_jump().
|
| This routine may be safely be called by dtdialog implementations.
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtcons_tag(								/* get nth tag						*/
  dt_ctl_t *ctl,
  int nth,
  const char **tag,
  int *iscurr)
{
  dtcons_t *cons = (dtcons_t *)(ctl? ctl->cons: NULL);
  int i;
  int n = -1;

  *tag = NULL;
  if (iscurr != NULL)
    *iscurr = 0;

  if (cons == NULL)						/* not ready for this?				*/
    return DT_RC_GOOD;

  for (i=0; i<cons->maxcons; i++)		/* find the tag						*/
    if (INUSE(i))
    {
      n++;
      if (n == nth)
      {
        *tag = cons->tag[i];
        if (i == cons->curr && 
			iscurr != NULL)
          *iscurr = 1;
        break;
      }
    }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtcons_find(							/* find tag, error if not found	    */
  dt_ctl_t *ctl,
  const char *tag,
  dtcons_opfunc_f **opfunc,
  void **arg)
{
  DT_F("dtcons_find");
  dtcons_t *cons = (dtcons_t *)ctl->cons;
  int i;

  if (cons == NULL)						/* not ready for this?				*/
    return DT_RC_GOOD;

  for (i=0; i<cons->maxcons; i++)		/* find the tag						*/
    if (INUSE(i) &&
        strstr(cons->tag[i], tag))
      break;
  if (i == cons->maxcons)				/* tag not found?					*/
    return dt_err(ctl, F,
      "tag \"%s\" not found",
      tag);

  if (opfunc)
    *opfunc = cons->opfunc[i];
  if (arg)
    *arg = cons->arg[i];

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtcons_swap(
  dt_ctl_t *ctl,
  int i)
{
  DT_F("dtcons_swap");
  dtcons_t *cons = (dtcons_t *)ctl->cons;
  char pass[DTCONS_PASSMAX];

  if (cons == NULL || cons->curr < 0)
    return DT_RC_GOOD;

  if (i == cons->curr)					/* already on this one?			    */
    return DT_RC_GOOD;

  if (!INUSE(i))						/* this one not in use?			    */
    return dt_err(ctl, F,
      "cons %d not in use", i);

  DT_Q((cons->opfunc[cons->curr]),		/* tell old one we are leaving		*/
    (ctl, DTCONS_OP_LEAVE,
     cons->arg[cons->curr], 
     pass));

  cons->curr = i;						/* get next cons				    */

  DT_Q((cons->opfunc[cons->curr]),		/* tell new one we have arrived		*/
    (ctl, DTCONS_OP_ENTER,
     cons->arg[cons->curr],
     pass));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtcons_goto(
  dt_ctl_t *ctl,
  dtcons_opfunc_f *opfunc,
  void *arg)
{
  DT_F("dtcons_goto");
  dtcons_t *cons = (dtcons_t *)ctl->cons;
  int i;

  if (cons == NULL || cons->curr < 0)
    return DT_RC_GOOD;

  for (i=0; i<cons->maxcons; i++)		/* find the given entry				*/
    if (cons->opfunc[i] == opfunc &&
        cons->arg[i] == arg)
      break;

  if (i == cons->maxcons)				/* entry not found?					*/
    return dt_err(ctl, F,
      "cons with opfunc 0x%08lx and arg 0x%08lx not found",
      opfunc, arg);

  DT_Q(dtcons_swap,(ctl, i));			/* swap to new cons				    */
  
  return DT_RC_GOOD;
}

/*..........................................................................
| If you have the value of a cons tag string, then you may use
| dtcons_tag to elicit an ~nth~ suitable for this routine.
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtcons_jump(
  dt_ctl_t *ctl,
  int nth)
{
  DT_F("dtcons_jump");
  dtcons_t *cons = (dtcons_t *)ctl->cons;
  int i;
  int n = -1;

  if (cons == NULL || cons->curr < 0)
    return DT_RC_GOOD;

  for (i=0; i<cons->maxcons; i++)		/* find the given entry				*/
    if (INUSE(i))
	{
      n++;
      if (n == nth)
        break;
	}

  if (n != nth)							/* no nth entry found?				*/
    return dt_err(ctl, F,
      "cons nth %d not found (max %d)",
      nth, n);

  DT_Q(dtcons_swap,(ctl, i));			/* swap to new cons				    */
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtcons_del(
  dt_ctl_t *ctl,
  dtcons_opfunc_f *opfunc,
  void *arg)
{
  DT_F("dtcons_del");
  dtcons_t *cons = (dtcons_t *)ctl->cons;
  char pass[DTCONS_PASSMAX];
  int i;

  if (cons == NULL || cons->curr < 0)
    return DT_RC_GOOD;

  for (i=0; i<cons->maxcons; i++)		/* find the given entry				*/
    if (cons->opfunc[i] == opfunc &&
        cons->arg[i] == arg)
      break;

  if (i == cons->maxcons)				/* entry not found?					*/
    return dt_err(ctl, F,
      "cons not found");


  if (i == cons->curr)					/* deleting current one?		    */
  {
    DT_Q((cons->opfunc[cons->curr]),	/* tell old one we are leaving		*/
      (ctl, DTCONS_OP_LEAVE,
       cons->arg[cons->curr], 
       pass));
    cons->curr = next(cons,		 		/* get previous cons			    */
      cons->curr, -1);
  }

  cons->opfunc[i] = NULL;
  cons->arg[i] = NULL;

  if (i == cons->curr)					/* deleted the only one?		    */
    cons->curr = -1;
  else
    DT_Q((cons->opfunc[cons->curr]),	/* tell new one we have arrived		*/
      (ctl, DTCONS_OP_ENTER,
       cons->arg[cons->curr],
       pass));
  
  return DT_RC_GOOD;
}



/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtcons_input(
  dt_ctl_t *ctl,
  dt_key_t *key)
{
  DT_F("dtcons_input");
  dtcons_t *cons = (dtcons_t *)ctl->cons;
  dt_key_t inkey;
  dt_rc_e rc = DT_RC_GOOD;

  if (cons == NULL || cons->curr < 0)
    return DT_RC_GOOD;

  while(1)
  {
    inkey = 0;
    DT_C((cons->opfunc[cons->curr]),	/* get input from current cons		*/
      (ctl,	DTCONS_OP_INPUT,
       cons->arg[cons->curr], &inkey));

    if (inkey == DTCONS_KEY_RUNSTOP)	/* stop running?				    */
    {
      rc = DT_RC_STOP;
      break;
	}
    else
    if (inkey == DTCONS_KEY_EXIT)		/* exit program?				    */
	{
      rc = DT_RC_EXIT;
      break;
	}
    else
    if (inkey == DTCONS_KEY_SWAP)
    {
      int oldcurr = cons->curr;
      DT_C(dtcons_swap,(ctl, 			/* swap to next one?				*/
        next(cons, cons->curr, 1)));
      dt_dbg(ctl, F, DT_DBG_MASK_WIN,
        "swapped from cons #%d to #%d",
        oldcurr, cons->curr);
      continue;
    }
    else
    if (inkey >= DT_KEY_JUMP_MIN &&
        inkey <= DT_KEY_JUMP_MAX)
    {
      int oldcurr = cons->curr;
      DT_C(dtcons_jump,(ctl,	 		/* jump to another one?				*/
        inkey - DT_KEY_JUMP_MIN));
      dt_dbg(ctl, F, DT_DBG_MASK_WIN,
        "jumped from cons #%d to #%d",
        oldcurr, cons->curr);
      continue;
    }
    break;
  }

  if (key)
    *key = inkey;

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "key 0x%04x rc %d",
    inkey, rc);

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtcons_update(							/* update all consoles			    */
  dt_ctl_t *ctl,
  const char *status,
  dt_key_t *key)
{
  DT_F("dtcons_update");
  dtcons_t *cons = (dtcons_t *)ctl->cons;
  int i;
  dt_key_t inkey;
  dt_rc_e rc = DT_RC_GOOD;

  if (cons == NULL || cons->curr < 0)
    return DT_RC_GOOD;

  if (status)
  {
    char t[64];
    strncpy(t, status, sizeof(t)-1);
    t[sizeof(t)-1] = '\0';
    for (i=0; i<cons->maxcons; i++)		/* go through list				    */
      if (INUSE(i))						/* slot is in use?				    */
        DT_C((cons->opfunc[i]),(ctl,	/* give it the new status			*/
          DTCONS_OP_STATUS,
          cons->arg[i], t));
  }

  for (i=0; i<cons->maxcons; i++)		/* go through list				    */
  {
    if (INUSE(i))						/* slot is in use?				    */
    {
      inkey = 0;

      DT_C((cons->opfunc[i]),(ctl,		/* send update to slot				*/
        DTCONS_OP_UPDATE,
        cons->arg[i], &inkey));

      if (inkey == DT_KEY_ENTER)
	  {
        rc = DT_RC_STOP;
        break;
	  }
      else
      if (inkey == DT_KEY_F3)
	  {
        rc = DT_RC_EXIT;
        break;
	  }
      else
      if (inkey == DT_KEY_F2)
      {
        DT_C(dtcons_swap,(ctl,	 		/* swap to next one?				*/
          next(cons, cons->curr, 1)));
      }
      else
      if (inkey >= DT_KEY_JUMP_MIN &&
          inkey <= DT_KEY_JUMP_MAX)
      {
        DT_C(dtcons_jump,(ctl,	 		/* jump to another one?				*/
          inkey - DT_KEY_JUMP_MIN));
      }
    }
  }

  if (key)
    *key = inkey;

  if (key && inkey || rc != DT_RC_GOOD)
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "key 0x%04x rc %d",
      inkey, rc);

cleanup:
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
