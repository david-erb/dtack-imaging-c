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
| dttext_dos - text using dos int 10h services
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
DT_RCSID("source/inter $RCSfile: textdos1.c,v $ $Revision: 1.1 $");
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/text.h>
#include <dtack/os.h>

#if DTPRJ_INT10 == DT_1
  static dttext_end_f dttext_int10_end;
  static dttext_out_f dttext_int10_out;
  static dttext_clear_f dttext_int10_clear;
  static dttext_key_f dttext_int10_key;

  typedef struct {
    dtkeybd_t keyboard;
  } dttext_int10_priv_t;

# if defined(DTCONFIG_WATCOM) && defined(__386__) && !defined(__WINDOWS_386__)
#   define mycx ecx
# else
#   define mycx cx
# endif

# ifndef DTDEPEND
#   ifdef DTCONFIG_WATCOM
#     include <i86.h>
#   endif
# endif

# if defined(DTCONFIG_IMPLIED_DOS32) || defined(DTCONFIG_IMPLIED_QNX432)
#   define INT10 int386(0x10, &regs, &regs);
# else
#   define INT10 int86(0x10, &regs, &regs);
# endif

#endif

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttext_int10_beg(						/* begin operations					*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *init)
{
  DT_F("dttext_int10_beg");

# if DTPRJ_INT10 != DT_1
  return dt_err_notlinked(ctl, F,
    "DOS INT 10h");
# endif

# if DTPRJ_INT10 == DT_1
  dttext_int10_priv_t *priv;

  DT_Q(dtos_malloc2,(ctl,				/* space for priv structure			*/
    (void **)&priv,
    sizeof(*priv),
    F, "priv"));
  memset(priv, 0, sizeof(*priv));

  DT_Q(dtkeybd_init,(ctl,				/* make us a keyboard				*/
    &priv->keyboard, 
    text->keyboard_name, 0));

  text->priv = priv;

  text->end =   dttext_int10_end;		/* set methods					    */
  text->out =   dttext_int10_out;
  text->clear = dttext_int10_clear;
  text->key =   dttext_int10_key;

  text->xe = 80 - 1;					/* stuff we have to tell caller	    */
  text->ye = 25;

  dttext_int10_clear(ctl, text);		/* clear screen at init			    */

  return DT_RC_GOOD;
# endif
}

#if DTPRJ_INT10 == DT_1

/*..........................................................................*/
static
void
move(
  int x,
  int y)
{
  union REGS regs;
  regs.h.ah = 0x02;						/* set cursor position			  	*/
  regs.h.bh = 0;						/* page number						*/
  regs.h.dh = y;						/* position						    */
  regs.h.dl = x;
  INT10;
}

/*..........................................................................*/
static
dt_rc_e
dttext_int10_end(						/* end operations					*/
  dt_ctl_t *ctl,
  dttext_t *text)
{
  DT_F("dttext_int10_end");
  dttext_int10_priv_t *priv =
    (dttext_int10_priv_t *)text->priv;
  dt_rc_e rc = DT_RC_GOOD;

  dttext_int10_clear(ctl, text);		/* clear screen at termination	    */

  move(0, 0);							/* move cursor to upper left	    */

  DT_I(dtkeybd_uninit,(ctl,				/* get rid of our keyboard			*/
    &priv->keyboard));

  DT_I(dtos_free2,(ctl, priv,			/* free our priv structure	    	*/
    F, "priv"));  
  
  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttext_int10_out(						/* output text at position			*/
  dt_ctl_t *ctl,
  dttext_t *text,
  const char *string,
  int x,
  int y,
  unsigned long flags)
{
/*DT_F("dttext_int10_out");*/
  union REGS regs;

  if (flags & DTTEXT_FLAG_CURSOR)		/* cursor only?						*/
    move(x, y);
  else
  {
	int attrbyte;
    if (flags & DTTEXT_FLAG_HIGHLIGHT)
      attrbyte = 0x70;					/* attribute byte				    */
    else
      attrbyte = 0x07;

    while(*string)
    {    
      move(x++, y);						/* position cursor				    */
      regs.h.ah = 0x09;					/* code to write character		    */
      regs.h.bh = 0;					/* page number					    */
      regs.h.bl = attrbyte;				/* attribute byte				    */
      regs.x.mycx = 1;					/* number of characters			    */
      regs.h.al = *string++;			/* next char in string			    */
      INT10;							/* issue interrupt for char		    */
	}
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttext_int10_clear(						/* clear screen						*/
  dt_ctl_t *ctl,
  dttext_t *text)
{
  union REGS regs;

  regs.h.ah = 0x06;						/* scroll						    */
  regs.h.al = 0;						/* means blank window			    */
  regs.h.bh = 0x07;						/* white on black attributes		*/
  regs.h.ch = 0;						/* upper left of scroll window	    */
  regs.h.cl = 0;
  regs.h.dh = 24;						/* lower right of scroll window	    */
  regs.h.dl = 80;

  INT10;								/* issue interrupt				    */

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttext_int10_key(						/* take keyboard input				*/
  dt_ctl_t *ctl,
  dttext_t *text,
  int *iskey,
  dt_key_t *key)
{
  DT_F("dttext_int10_key");
  dttext_int10_priv_t *priv =
    (dttext_int10_priv_t *)text->priv;

  if (iskey)
    DT_Q(dtkeybd_scan,(ctl,		/* get key with no waiting			*/
      &priv->keyboard, iskey, key))
  else
    DT_Q(dtkeybd_read,(ctl,		/* wait for key						*/
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
