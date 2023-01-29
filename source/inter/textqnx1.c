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
| dttext_qnxterm - text using qnxterm services
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/inter $RCSfile: textqnx1.c,v $ $Revision: 1.1 $");
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/text.h>
#include <dtack/os.h>
#include <dtack/qnx4.h>

#if DTPRJ_QNXTERM == DT_1
  static dttext_end_f dttext_qnxterm_end;
  static dttext_out_f dttext_qnxterm_out;
  static dttext_clear_f dttext_qnxterm_clear;
  static dttext_key_f dttext_qnxterm_key;

  typedef struct {
    dtkeybd_t keyboard;
  } dttext_qnxterm_priv_t;

# ifndef DTDEPEND
#   include <sys/qnxterm.h>
# endif

#endif

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttext_qnxterm_beg(						/* begin operations					*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *spec)
{
  DT_F("dttext_qnxterm_beg");

# if DTPRJ_QNXTERM != DT_1
  return dt_err_notlinked(ctl, F,
    "QNX term");
# endif

# if DTPRJ_QNXTERM == DT_1
  dttext_qnxterm_priv_t *priv = NULL;
  int have_keyboard = 0;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,               /* space for priv structure         */
    (void **)&priv,
    sizeof(*priv),
    F, "priv"));
  memset(priv, 0, sizeof(*priv));

  DT_C(dtkeybd_init,(ctl,				/* make us a keyboard				*/
    &priv->keyboard, 
    text->keyboard_name, 0));
  have_keyboard = 1;
  
  if (!DTQNX4_TERM(ctl))				/* keyboard didn't enter term mode? */
  {
	rc = dt_err(ctl, F,
      "keyboard \"%s\" is not compatible"
	  " with full screen text mode \"%s\"",
      text->keyboard_name, spec);
	goto cleanup;
  }

  text->priv = priv;
  text->keyboard = &priv->keyboard;		/* so upper levels in print maps 	*/

  text->end =   dttext_qnxterm_end;		/* set methods					    */
  text->out =   dttext_qnxterm_out;
  text->clear = dttext_qnxterm_clear;
  text->key =   dttext_qnxterm_key;

  text->xe = 80 - 1;					/* stuff we have to tell caller	    */
  text->ye = 25;

  DT_C(dttext_qnxterm_clear,(ctl, 		/* clear screen at init			    */
    text));

  return DT_RC_GOOD;
cleanup:
  if (have_keyboard)
    DT_I(dtkeybd_uninit,(ctl,			/* undo the keyboard  				*/
      &priv->keyboard));
  if (priv != NULL)
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  return rc;
# endif
}

#if DTPRJ_QNXTERM == DT_1

/*..........................................................................*/
static
dt_rc_e
dttext_qnxterm_end(						/* end operations					*/
  dt_ctl_t *ctl,
  dttext_t *text)
{
  DT_F("dttext_qnxterm_end");
  dttext_qnxterm_priv_t *priv =
    (dttext_qnxterm_priv_t *)text->priv;
  dt_rc_e rc = DT_RC_GOOD;

  dttext_qnxterm_clear(ctl, text);		/* clear screen at termination	    */

  DT_I(dtkeybd_uninit,(ctl,				/* get rid of our keyboard			*/
    &priv->keyboard));

  DT_I(dtos_free2,(ctl, priv,			/* free our priv structure	    	*/
    F, "priv"));  
  
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttext_qnxterm_out(						/* output text at position			*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *string,
  int x,
  int y,
  unsigned long flags)
{
  dttext_qnxterm_priv_t *priv =
    (dttext_qnxterm_priv_t *)text->priv;

  if (flags & DTTEXT_FLAG_NORMAL ||
	  flags & DTTEXT_FLAG_HIGHLIGHT)
  {
	unsigned attr;
	if (flags & DTTEXT_FLAG_HIGHLIGHT)
	  attr = TERM_HILIGHT;
	else
	  attr = TERM_NORMAL;
	term_type(y, x, string, 0, attr);	/* output the string				*/
  }
  else
  if (flags & DTTEXT_FLAG_CURSOR)		/* cursor placement only?			*/
  {
	term_cur(y, x);						/* place the cursor					*/
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttext_qnxterm_clear(					/* clear screen						*/
  dt_ctl_t *ctl,
  dttext_t *text)
{
  dttext_qnxterm_priv_t *priv =
    (dttext_qnxterm_priv_t *)text->priv;

  term_clear(TERM_CLS_SCR);				/* clear entire screen				*/

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttext_qnxterm_key(						/* take keyboard input				*/
  dt_ctl_t *ctl,
  dttext_t *text,
  int *iskey,
  dt_key_t *key)
{
  DT_F("dttext_qnxterm_key");
  dttext_qnxterm_priv_t *priv =
    (dttext_qnxterm_priv_t *)text->priv;

  if (iskey)
    DT_Q(dtkeybd_scan,(ctl,				/* get key with no waiting			*/
      &priv->keyboard, iskey, key))
  else
    DT_Q(dtkeybd_read,(ctl,				/* wait for key						*/
      &priv->keyboard, key));

  return DT_RC_GOOD;
}
#endif






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
