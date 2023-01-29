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
| Keymap for termcap.
|
|.TERMCAP and XTERM
| The standard termcap for xterm contains a ks= capability.
| This ks= capability, among other things, turns on Application Cursor Mode.
| In this mode, xterm will send ESC-OA when it sees XK_Up.
| This puts xterm in agreement with its termcap ku= capability.
| When not in Application Cursor Mode, xterm sends the string ESC-[A
| when it sees XK_Up.
| This is not in agreement with the termcap.
| 
| Bash 1.14.0(2) on SunOS 4.1.3 does not issue the ke= capability 
| when forking.
| This means that xterm is left in Application Cursor Mode when an
| application or sub-shell is run.
| Bash 1.14.2(1) on FreeBSD 2.0 does issue the ke= capability
| when forking.
| This means that xterm is returned to its default state when running
| an application.
| 
| Dt must protect itself from these vagaries by issuing the ks=
| capability when enabling the keymap.
| This is also in accordance with dt's stated goal of being completely
| termcap dependent.
| Note that bash (i.e. readline) tries to be smart and will interpret 
| ESC-OA as an up arrow even if there is no termcap at all.
| 
| Some systems' termcaps for xterm contain a k1=ESC-OP capability.
| However, xterm on these systems may invoke this capability 
| only when it sees XK_KP_F1.
| If it sees XK_F1, it sends the string ESC-[11~.
| This string is not mentioned in the standard termcap anywhere.
| To get dt to recognize F1, you have two choices.
| First, you can edit the termcap file to change the k1= capability.
| Second, you can figure out which key on your keymap generates XK_KP_F1.
| The standard X client xev is an excellent tool to help in this.
| 
| The standard termcap or xterm contains no kP= or kN= capability.
| However, xterm will send the string ESC-[5~ when it sees XK_Prior
| and ESC-[6~ when it sees XK_Next.
| Typically XK_Prior is generated when you hit the key labeled PageUp.
| The only way for dt to recognize the PageUp key is to put the
| kP= capability into the termcap.
|
| Xterm sends something unintelligible when it sees XK_Home or XK_End.
| Anyway, there is no well accepted termcap capability corresponding
| to these keys.
| This could be due to the fact that there is no well accepted 
| meaning for these keys either.
| 
| You may use the command "dttlow key -op diag" to help determine
| how character sequences coming from xterm are being interpreted by dt.
| 
|            xev            FBSD2.1 dttkey diag
| native F1  F1             ESC [ 1 1 ~
| kt F1      KP_F1          ESC O P
| kt SF1     Shift_L KP_F1  ESC O P
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
DT_RCSID("source/keybd $RCSfile: keymap.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/keymap.h>
#include <dtack/str.h>

#ifndef DTDEPEND
#  if DTPRJ_TERMCAP == DT_1
#    include <termcap.h>
#  endif
#endif

#if defined(DTCONFIG_IMPLIED_SUNOS) &&	/* not in this guy's header files!	*/\
    defined(DTCONFIG_IMPLIED_GNU)
  extern int fputc(char, FILE *);
#endif

#if DTPRJ_TERMCAP == DT_1

static dtkeymap_uninit_f dtkeymap_termcap_uninit;

/*..........................................................................
 * output routine for tputs()
 *..........................................................................*/
static
int
dtkeymap_tputs_outc(int c)
{
  int rc = fputc(c, stdout);
  return rc;
}
#endif

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeymap_termcap_init(					/* init keymap structure			*/
  dt_ctl_t *ctl,
  dtkeymap_t *keymap,
  const char *spec,					  
  unsigned long flags)
{
  DT_F("dtkeymap_termcap_init");
#if DTPRJ_TERMCAP == DT_1
  const char *termname;
  char *string, *end;
  int count = 0;
  char *ks_space = NULL;
  char *ke_space = NULL;
  void *buffer = NULL;
  void *space = NULL;
    int sysrc;
  dt_rc_e rc = DT_RC_GOOD;

  if (keymap->uninit_desc != NULL)		/* somebody has uninit in place?	*/
    return dt_err(ctl, F,
      "cannot coreside"
      " with %s",
      keymap->uninit_desc);

  keymap->uninit_desc = F;

  keymap->uninit =						/* our uninit method 				*/
    dtkeymap_termcap_uninit;
  
  termname = getenv("TERM");			/* get TERM environment				*/
  if (termname == NULL)
    return dt_err(ctl, F,
      "Sorry, no TERM"
      " environment variable is defined.");
  else
  if (strlen(termname) == 0)
    return dt_err(ctl, F,
      "Sorry, the TERM environment"
      " variable is empty.");

    DT_C(dtos_malloc2,(ctl,
      &buffer, 2048,
      F, "termcap buffer"));
      
    sysrc = tgetent((char *)buffer,
      (char *)termname);				/* cast: proto may not be const 	*/
    if (sysrc == -1)
      rc = dt_err(ctl, F,
        "cannot access termcap database for \"%s\"",
        termname);
    else
    if (sysrc == 0)
      rc = dt_err(ctl, F,
        "cannot find \"%s\" in termcap",
        termname);
    if (sysrc <= 0)
      goto cleanup;

  DT_C(dtos_malloc2,(ctl,
    &space, strlen(buffer)+1,			/* can't be longer than buffer 		*/
    F, "termcap inquiry space"));

#define MAPCAP(KEY, CAP) \
  end = space; \
  string = tgetstr(CAP, &end); \
  if (string)  							/* got the string?					*/\
  { \
    DT_C(dtkeymap_map,(ctl,				/* map the key					    */\
      keymap, KEY, space, \
      DTKEYMAP_FLAG_ADD)); \
	count++; \
  } \
  else \
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, \
      "%s (%s) capability missing from termcap", \
      #KEY, CAP);

  MAPCAP(DT_KEY_UPARROW,     "ku"); 
  MAPCAP(DT_KEY_DOWNARROW,   "kd"); 
  MAPCAP(DT_KEY_LEFTARROW,   "kl");
  MAPCAP(DT_KEY_RIGHTARROW,  "kr");
  MAPCAP(DT_KEY_PAGEDOWN,    "kN"); 
  MAPCAP(DT_KEY_PAGEUP,      "kP");
  MAPCAP(DT_KEY_F1,          "k1");
  MAPCAP(DT_KEY_F2,          "k2");
  MAPCAP(DT_KEY_F3,          "k3");
  MAPCAP(DT_KEY_F4,          "k4");
  MAPCAP(DT_KEY_F5,          "k5");
  MAPCAP(DT_KEY_DELETE,      "kD");
  MAPCAP(DT_KEY_BACKSPACE,   "kb");

  DT_C(dtkeymap_builtin,(ctl,
    keymap, "dumb", 
    DTKEYMAP_FLAG_ADD));
  
  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, 
    "%d termcap keys and %d control keys"
    " successfully added",
    count, keymap->n - count);

										/* ................................ */
  ks_space = space;						/* reuse for efficiency 			*/
  space = NULL;
  end = ks_space; 
  string = tgetstr("ks", &end);			/* get ks capability string			*/
  if (string)
  {
	*end = '\0';
    DT_C(dtos_realloc3,(ctl,			/* shrink to fit 					*/
      ks_space, (void **)&ks_space,
      strlen(ks_space)+1,
      F, "ks capability string"));
  } 
  else
  {
    DT_C(dtos_free2,(ctl, ks_space,
      F, "ks capability string"));
    ks_space = NULL;
  }
										/* ................................ */
  DT_C(dtos_malloc2,(ctl,				/* space for ke capability string 	*/
    (void **)&ke_space, strlen(buffer),
    F, "ke capability string"));
  end = ke_space; 
  string = tgetstr("ke", &end);			/* get ke capability string			*/
  if (string)
  {
	*end = '\0';
    DT_C(dtos_realloc3,(ctl,			/* shrink to fit 					*/
      ke_space, (void **)&ke_space,
      strlen(ke_space)+1,
      F, "ke capability string"));
  }
  else
  {
    DT_C(dtos_free2,(ctl, ke_space,
      F, "ke capability string"));
    ke_space = NULL;
  }

  if (ks_space)
  {
	char string[80];
    DT_Q(dtstr_printable,(ctl,	
      ks_space,
      strlen(ks_space),
      string, sizeof(string)));
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, 
      "outputting termcap ks `%s'",
	  string);
    tputs(ks_space, 1,					/* output the ks capability 		*/
      dtkeymap_tputs_outc);
	fflush(stdout);
	keymap->ks_done = 1;
  }

  if (ke_space)
  {
	char string[80];
    DT_Q(dtstr_printable,(ctl,	
      ke_space,
      strlen(ke_space),
      string, sizeof(string)));
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, 
      "will save termcap ke `%s'", 
      string);
  }

cleanup:
  if (rc == DT_RC_GOOD)
  {
    keymap->ks_string = ks_space;
    keymap->ke_string = ke_space;
  }

  if (ks_space && rc != DT_RC_GOOD)
    DT_I(dtos_free2,(ctl, ks_space,
      F, "ks capability string"));

  if (ke_space && rc != DT_RC_GOOD)
    DT_I(dtos_free2,(ctl, ke_space,
      F, "ke capability string"));

  if (space)
    DT_I(dtos_free2,(ctl, space,
      F, "termcap inquiry space"));
  if (buffer)
    DT_I(dtos_free2,(ctl, buffer,
      F, "termcap buffer"));

  return rc;
#else
  return dt_err_notlinked(ctl, F,
    "termcap library");
#endif
}

/*..........................................................................*/

#if DTPRJ_TERMCAP == DT_1

static
dt_rc_e
dtkeymap_termcap_uninit(				/* uninit keymap structure		*/
  dt_ctl_t *ctl,
  dtkeymap_t *keymap)
{
  DT_F("dtkeymap_termcap_uninit");
  dt_rc_e rc;
  if (keymap->ks_done &&				/* ks capability has been output? 	*/
	  keymap->ke_string != NULL)		/* and we have a ke capability?		*/
  {
	char string[80];
    DT_Q(dtstr_printable,(ctl,	
      keymap->ke_string,
      strlen(keymap->ke_string),
      string, sizeof(string)));
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, 
      "outputting termcap ke `%s'", 
      string);
    tputs(keymap->ke_string, 1,		/* output the ke capability 		*/
      dtkeymap_tputs_outc);
	fflush(stdout);
  }

  if (keymap->ke_string)
    DT_G(dtos_free2,(ctl,
	  keymap->ke_string,
      F, "ke capability string"));

  if (keymap->ks_string)
    DT_I(dtos_free2,(ctl,
	  keymap->ks_string,
      F, "ks capability string"));

  return rc;
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
