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
| This test has several operational modes:
| diag		say the key map table then wait for 5 keys, printing
| 			the interpretation of each one
| create	ask for each important key in turn,
| 			then write the resulting map in parsable form to -output
| dump		simply write current map in parsable form to -output
| say		simply say the current map to -output
| wait		ask for each important key in turn, 
| 			then ask again and check
| read		ask for each important key in turn, 
| 			using non-polling input function, check against map 
| scan		ask for each important key in turn, 
| 			using polling input function, check against map 
| 
| If your keyboard is not being mapped correctly by your termcap,
| consider creating a custom map.
| Use :dttinter key -op create -out mymap.out: to create the map.
| Set the environment variable :set DT_KEYBOARD=,,mymap.out: before
| running the full-screen program.
| 
|.QNX Notes
| Under QNX, the scan and read operations use the dumb maps only.
| This is because QNX, though termcap support is present,
| cannot be relied upon to have a proper termcap entry at all sites.
|
| When in qnxterm mode, any termcap entries would be superfluous
| since all keyboard mappings are taken care of using term_key(),
| which uses :terminfo:.  
| Currently, qnxterm mode is enabled in only two places:
| here in this test program, and in :textqnx1.c:.
|
| The disadvantage of qnxterm mode is that anything output to
| the console by printf does not look right.
| Specifically, a '\n' in an output string does not do a carriage return.
|
| When not in qnxterm mode, TERMIOS input is used just like on unix.
| However, since termcap is disabled, there are no known keyboard 
| mappings.  For example, we won't know what crazy escape sequence
| the "up arrow" key inputs to a read on the computer.
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

#include <dtack/t.h>
DT_RCSID("test $RCSfile: dttkey.c,v $ $Revision: 1.7 $");
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/prj.h>

static char *USAGE = "usage: [params]\n";

static dt_key_t keylist[] = {
  DT_KEY_STOP,
  DT_KEY_F1,
  DT_KEY_F2,
  DT_KEY_F3,
  DT_KEY_F4,
  DT_KEY_F5,
  DT_KEY_LEFTARROW,
  DT_KEY_RIGHTARROW,
  DT_KEY_UPARROW,
  DT_KEY_DOWNARROW,
  DT_KEY_PAGEUP,
  DT_KEY_PAGEDOWN,
  DT_KEY_HOME,
  DT_KEY_END,
  DT_KEY_BACKSPACE,
  DT_KEY_DELETE,
  DT_KEY_ENTER,
  DT_KEY_IGNORE};

/*..........................................................................*/
static
dt_rc_e
dttkey_make_map(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  dt_key_t key)
{
  DT_F("dttkey_make_map");
  dtkeymap_t *keymap =
    &keyboard->keymap;
  dt_key_t inkey;
  char name[16];
  char map[32];
  int n = 0;
  dt_rc_e rc;

  DT_Q(dtkeymap_name_str,(ctl,			/* name for this key 				*/
    keymap, key, 
    name, sizeof(name)));

  if (key == DT_KEY_ENTER)
    dt_say(ctl, F, 
      "hit ENTER");
  else
    dt_say(ctl, F, 
      "hit keystroke(s) for %s"
      " then ENTER", name);

  do {
    int nmaps = keymap->n;				/* remember the keyboard map count 	*/
    keymap->n = 0;						/* so no mapping occurs in read 	*/
    DT_G(dtkeybd_read,(ctl,				/* read key from keyboard			*/
      keyboard, &inkey));
	keymap->n = nmaps;
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "got key 0x%04x", inkey);
    map[n++] = inkey;
  } while ((unsigned)n < sizeof(map) &&
           inkey != '\r' &&
           inkey != '\n' &&
           inkey != DT_KEY_ENTER);
  if ((unsigned)n == sizeof(map))
    return dt_err(ctl, F,
      "too many input characters (%d)",
      sizeof(map));

  if (key == DT_KEY_ENTER)
  {
    if (n < 1)							/* no map string for this one?		*/
      return DT_RC_GOOD;
    map[n] = '\0';						/* keep the enter					*/
  }
  else
  {
    if (n <= 1)							/* no map string for this one?		*/
      return DT_RC_GOOD;
    map[n-1] = '\0';					/* clobber the enter				*/
  }

  DT_Q(dtkeymap_map,(ctl,				/* map key sequence definition		*/
    keymap, key, map,
    DTKEYMAP_FLAG_ADD));

  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttkey_make_maps(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  DT_F("dttkey_make_maps");
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  for (i=0;
	   keylist[i] != DT_KEY_IGNORE;
	   i++)
    DT_GI(dttkey_make_map,(ctl,
      keyboard,
      keylist[i]));

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttkey_say_map(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  dt_key_t key)
{
  DT_F("dttkey_say_map");
  char name[16];
  char maps[80];

  DT_Q(dtkeymap_name_str,(ctl,
    &keyboard->keymap, key, 
    name, sizeof(name)));

  DT_Q(dtkeymap_maps_str,(ctl,
    &keyboard->keymap, key, 
    maps, sizeof(maps)));

  dt_say(ctl, NULL, "  %-11s %s",
    name, maps);
  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttkey_say_maps(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard)
{
  DT_F("dttkey_say_maps");
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  dt_say(ctl, F, "these are the keyboard mappings:");

  for (i=0;
	   keylist[i] != DT_KEY_IGNORE;
	   i++)
    DT_GI(dttkey_say_map,(ctl,
      keyboard, 
      keylist[i]));

  return rc;
}
/*..........................................................................*/
static
dt_rc_e
dttkey_dump_maps(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  dtfd_t *fd)
{
  DT_F("dttkey_dump_maps");
  int i;
  char string[80];
  int lmax = sizeof(string);
  dtkeymap_t *keymap =
    &keyboard->keymap;

  for (i=0; i<keymap->n; i++)			/* go through list of maps			*/
  {
    DT_Q(dtkeymap_name_str,(ctl,		/* make key name printable 			*/
      keymap, keymap->entry[i].k,
      string, lmax));
    DT_Q(dtstr_printf,(ctl,				/* separate with a space 			*/
      string+strlen(string), 
      lmax-strlen(string),
      " "));
    DT_Q(dtstr_printable,(ctl,			/* make map entry printable		    */
      keymap->entry[i].s, 
      keymap->entry[i].l, 
      string+strlen(string), 
      lmax-strlen(string)));
    DT_Q(dtstr_printf,(ctl,				/* separate with a space 			*/
      string+strlen(string), 
      lmax-strlen(string),
      "\n"));
	DT_Q(dtfd_write_all,(ctl, fd, 		/* output the string 				*/
      string, strlen(string)));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dttkey_check_map(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  int flag,
  dt_key_t key,
  int *assert_fail)
{
  DT_F("dttkey_check_map");
  dtkeymap_t *keymap =
    &keyboard->keymap;
  dt_key_t inkey;
  int iskey;
  char name[16];
  char inname[16];
  char maps[80];
  int i;

  DT_Q(dtkeymap_name_str,(ctl,			/* make string with key's name in it */
    keymap, key, 
    name, sizeof(name)));

  DT_Q(dtkeymap_maps_str,(ctl,			/* make string with maps in it 		*/
    keymap, key, 
    maps, sizeof(maps)));

  for (i=0; i<keymap->n; i++)
    if (keymap->entry[i].k == key)
      break;

  if (key == DT_KEY_STOP)
  {
    if (i == keymap->n)					/* no map for STOP?					*/
      dt_say(ctl, F, 
        "Please verify keys");
    else
      dt_say(ctl, F, 
        "Please verify keys,"
        " hit %s to quit",
        maps);
    return DT_RC_GOOD;
  }

  if (i == keymap->n)					/* no map for this key?				*/
  {
    dt_say(ctl, F, "%-11s %s, skipping",
      name, maps);
    return DT_RC_GOOD;
  }

  DT_Q(dtkeybd_flush,(ctl,				/* clear the input buffer 			*/
    keyboard));

  dt_say(ctl, F, "now hit %-10s %-22s (%s)", 
    name, maps, flag? "sleeping": "polling");
  if (flag)
    DT_Q(dtkeybd_read,(ctl,
      keyboard, &inkey))
  else
  do {
    DT_Q(dtkeybd_scan,(ctl,
      keyboard, &iskey, &inkey));
  } while(!iskey);

  DT_Q(dtkeymap_name_str,(ctl,	/* make string with key's name in it */
    &keyboard->keymap, inkey, 
    inname, sizeof(inname)));

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl,
      key == inkey,
      F, "got %s 0x%04x (expected %s 0x%04x)",
      inname, inkey, name, key); 

  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttkey_check_maps(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  int flag,
  int *assert_fail)
{
  DT_F("dttkey_check_maps");
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dttkey_say_maps,(ctl,			/* say all key maps 				*/
    keyboard));

  for (i=0;
	   keylist[i] != DT_KEY_IGNORE;
	   i++)
    DT_GI(dttkey_check_map,(ctl,
      keyboard, flag,
      keylist[i], assert_fail));

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttkey_diag_map(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  int left,
  dt_key_t *key)
{
  DT_F("diag");
  dtkeymap_t *keymap =
    &keyboard->keymap;
  char name[16];
  char string[80];
  int i;

  do {
    int nmaps = keymap->n;				/* remember the keyboard map count 	*/
    keymap->n = 0;						/* so no mapping occurs in read 	*/
    dt_say(ctl, F, 
      "(loop %d) please hit a key",
      left); 
    DT_Q(dtkeybd_read,(ctl,				/* read input from keyboard 		*/
      keyboard, key));
    keymap->n = nmaps;

    for (i=keyboard->n; i>0; i--)		/* shuffle buffer to right 			*/
      keyboard->buffer[i] =
        keyboard->buffer[i-1];
    keyboard->buffer[0] = *key;			/* replace key we read in buffer 	*/
    keyboard->n++;

    DT_Q(dtstr_printable,(ctl,			/* make map entry printable		    */
      keyboard->buffer,
      keyboard->n, 
      string, sizeof(string)));

	DT_Q(dtkeymap_match,(ctl,			/* match buffer to key maps		    */
      keymap, 
      keyboard->buffer, keyboard->n, 
      &keyboard->n, key, 
      &keyboard->ispartial));

    if (keyboard->ispartial)
      dt_say(ctl, F, "the key you hit"
        " is part of a partial"
        " sequence `%s'",
        string);

  } while(keyboard->ispartial);

  DT_Q(dtkeymap_name_str,(ctl,			/* make string with key's name in it */
    &keyboard->keymap, *key, 
    name, sizeof(name)));

    dt_say(ctl, F, "the key you hit"
      " produced the"
      " sequence `%s'",
      string);

  if (*key > 255)
    dt_say(ctl, F, 
      "this sequence maps to"
      " the special value %s",
      name);
  else
    dt_say(ctl, F, 
      "this sequence maps to"
      " the simple character `%s'",
      name);

  if (keyboard->n)						/* any keys left in buffer? 		*/
  {
    DT_Q(dtstr_printable,(ctl,			/* make map entry printable		    */
      keyboard->buffer,
      keyboard->n, 
      string, sizeof(string)));

    dt_say(ctl, F, "the characters `%s'"
      " were not used in the mapping",
      string);

	DT_Q(dtkeybd_flush,(ctl,			/* clear the input buffer 			*/
      keyboard));
  }
  return DT_RC_GOOD;
}
/*..........................................................................*/
static
dt_rc_e
dttkey_diag_maps(
  dt_ctl_t *ctl,
  dtkeybd_t *keyboard,
  int n)
{
  DT_F("dttkey_diag_maps");
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  DT_Q(dttkey_say_maps,(ctl,			/* say all key maps 				*/
    keyboard));

  for (i=0; i<n; i++)
  {
	dt_key_t key;
    DT_GI(dttkey_diag_map,(ctl,
      keyboard, n-i, &key));
  }

  return rc;
}

/*..........................................................................*/


DTT_TEST(dttkey)
{
  DT_F("dttkey");
  dt_ctl_t *ctl = &param->ctl;
  int assert_fail = 0;
  int n;
  dtkeybd_t keyboard;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

/* printf("backslash-r is 0x%02x, backslash-n is 0x%02x\n", '\r', '\n'); */

										/* ................................ */
  DT_GI(dtparam_init,(param));			/* init param structure             */

  DTPARAM_DEFAULT(dbg, 0);				/* set default for params we want	*/
  DTPARAM_DEFAULT(n, 5);
  DTPARAM_DEFAULT(op, "read");
  DTPARAM_DEFAULT(output, "dttkey.out");

  DT_GI(dt_enter_param,(				/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;

  if (argc != 0)
    return dt_usage_say(
      param, USAGE);


  if (!strcmp(param->op, "all") ||
      !strcmp(param->op, "diag"))
  {
    DT_GI(dtkeybd_init,(ctl,
      &keyboard, param->keyboard, 0));
    if (rc == DT_RC_GOOD)
	{
      DT_GI(dttkey_diag_maps,(ctl,
        &keyboard, (int)param->n));
      DT_I(dtkeybd_uninit,(ctl,
        &keyboard));
	}
  }
										/* ................................ */
  if (!strcmp(param->op, "create"))
  {
    DT_GI(dtkeybd_init,(ctl,			/* open keyboard with default maps  */
      &keyboard, param->keyboard, 0));
    if (rc == DT_RC_GOOD)
	{
	  dtfd_t fd;
      DT_GI(dttkey_make_maps,(ctl,		/* ask for each key in turn		    */
        &keyboard));
	  DT_GI(dtfd_open,(ctl, &fd,		/* open file for map output		    */
        param->output, "w"));
      DT_GI(dttkey_dump_maps,(ctl,		/* dump maps into file			    */
        &keyboard, &fd));
	  DT_I(dtfd_close,(ctl, &fd));		/* close map output file		    */
      dt_say(ctl, F, "wrote map to %s",
        param->output);
      DT_I(dtkeybd_uninit,(ctl,
        &keyboard));
    }
  }
										/* ................................ */
  if (!strcmp(param->op, "dump"))
  {
    DT_GI(dtkeybd_init,(ctl,			/* open keyboard with default maps  */
      &keyboard, param->keyboard, 0));
    if (rc == DT_RC_GOOD)
	{
	  dtfd_t fd;
	  DT_GI(dtfd_open,(ctl, &fd,		/* open file for map output		    */
        param->output, "w"));
      DT_GI(dttkey_dump_maps,(ctl,		/* dump maps into file			    */
        &keyboard, &fd));
	  DT_I(dtfd_close,(ctl, &fd));		/* close map output file		    */
      dt_say(ctl, F, "wrote map to %s",
        param->output);
      DT_I(dtkeybd_uninit,(ctl,
        &keyboard));
    }
  }
										/* ................................ */
  if (!strcmp(param->op, "say"))
  {
    DT_GI(dtkeybd_init,(ctl,			/* open keyboard with default maps  */
      &keyboard, param->keyboard, 0));
    if (rc == DT_RC_GOOD)
	{
      DT_GI(dttkey_say_maps,(ctl,		/* say maps							*/
        &keyboard));
      DT_I(dtkeybd_uninit,(ctl,
        &keyboard));
    }
  }
										/* ................................ */
  if (!strcmp(param->op, "all") ||
      !strcmp(param->op, "wait"))
  {
    DT_GI(dtkeybd_init,(ctl,
      &keyboard, param->keyboard,
      DTKEYBD_FLAG_INIT_NOMAP));
    if (rc == DT_RC_GOOD)
	{
      DT_GI(dttkey_make_maps,(ctl,
        &keyboard));
      DT_GI(dttkey_check_maps,(ctl,
        &keyboard, 1, &assert_fail));
      DT_I(dtkeybd_uninit,(ctl,
        &keyboard));
    }
  }
										/* ................................ */
  if (!strcmp(param->op, "all") ||
      !strcmp(param->op, "read"))
  {
    DT_GI(dtkeybd_init,(ctl,
      &keyboard, param->keyboard, 0));
    if (rc == DT_RC_GOOD)
	{
      DT_GI(dttkey_check_maps,(ctl,
        &keyboard, 1, &assert_fail));
      DT_I(dtkeybd_uninit,(ctl,
        &keyboard));
    }
  }
										/* ................................ */
  if (rc == DT_RC_GOOD)
  if (!strcmp(param->op, "all") ||
      !strcmp(param->op, "scan"))
  {
    DT_GI(dtkeybd_init,(ctl,
      &keyboard, param->keyboard, 0));
    if (rc == DT_RC_GOOD)
	{
      DT_GI(dttkey_check_maps,(ctl,
        &keyboard, 0, &assert_fail));
      DT_I(dtkeybd_uninit,(ctl,
        &keyboard));
	}
  }
										/* ................................ */

  DT_I(dt_leave,(param));				/* clean up before leaving			*/

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
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
