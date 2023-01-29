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
| TITLE dtkeymap()
|dxinclude(liblink1.dx)
|
|!INTRODUCTION
| This Function Group is supposed to convert raw bytes
| into key values such as :DT_KEY_F1:.
| It helps present a unified keyboard
| response across all operating systems.
| It does not read from the keyboard hardware itself.
|
| It is important to understand the difference between raw bytes and key values.
| Raw bytes are returned when reading the keyboard.
| Key values usually correspond one-to-one with they phsysical button.
| This is usually what is silkscreened on the keyboard itself.
| When you hit X, or any other simple character,
| the key value is the same as the raw byte.
| However, when you hit F1
| the keyboard may, for example, deliver the three raw bytes ESC, O and P.
| This Function Group's job is to combine the three raw bytes and return a single
| key value such as :DT_KEY_F1:.
| 
| This Function Group is not meant for application-level keyboard remapping.
| In other words, the key values delivered are always
| ASCII letters or constants such as :DT_KEY_F1:.
| The universe of key values is fixed.
| The keyboard maps should be initialized one time by an application
| then remain constant as long as the keyboard itself does not change.
| 
|.TERMCAP, BASH and XTERM
| The standard termcap for xterm contains a ks= capability.
| This ks= capability, among other things, turns on Application Cursor Mode.
| In this mode, xterm will send ESC-OA when it sees XK_Up.
| This puts xterm in agreement with its termcap ku= capability.
| When not in Application Cursor Mode, xterm sends the string ESC-[A
| when it sees XK_Up.
| This is not in agreement with the termcap.
| 
| Bash 1.14.0(2) on SunOS 4.1.3 does not issue the ke= capability when forking.
| This means that xterm is left in Application Cursor Mode when an
| application or sub-shell is run.
| Bash 1.14.2(1) on FreeBSD 2.0 does issue the ke= capability when forking.
| This means that xterm is returned to its default state when running
| an application.
| 
| This Function Group must protect itself from these vagaries by issuing the ks=
| capability when enabling the keyboard.
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
| Second, you can figure out which key on your keyboard generates XK_KP_F1.
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
| You may use the command "dttenter key -op diag" to help determine
| how character sequences coming from xterm are being interpreted by dt.
| 
|!SUMMARY
| Maintains a list of mappings from raw byte sequences to 
| discrete integer key values.
| Has a function which returns a single integer key value
| given a sequence of raw bytes.
| 
| Maps multi-byte sequences typically returned by
| function keys and arrow keys
| into the set of key values such as :DT_KEY_F1: and :DT_KEY_UPARROW:.
| Raw bytes which are not in a configured sequence are returned unmapped.
| That is, the discrete key value for a simple ASCII character
| the same as the corresponding raw byte.
| 
| Obtains the mapping list either from a compiled-in in set, 
| a caller-specified file, or an operating system interface such as termcap.
| The names of the compiled-in mapping sets are:
| :none:	empty mapping set
| :termcap:	mappings for keys derived from the :termcap: interface
| :qnxterm:	standard QNX mappings for values returned by term_key()
| :vt100:	mappings for well-known ANSI sequences, plus the control keys
| :dos:		mappings for keys returned by getch() under DOS
| :dumb:	mappings for keys with the control key held down
| :ctlkeys:	mappings for keys with the control key held down (same as :dumb:)
|
|.Salient Concepts
| You create the mapping to use from a compiled-in set, from termcap,
| or from a file which you edit.
|
| You give raw byte sequences and get back discrete key values.
| The discrete key values are all predefined.
| 
|.Specification Syntax
| The ~spec~ argument to dtkeymap_init() shall contain
| either the name of a readable file,
| one of the compiled-in mapping set names from the list above,
| or a single key mapping.
| If a file name is given, each line in the file
| is parsed recursively exactly the same as the ~spec~ argument.
| 
| A single key mapping consists of
| a discrete key name followed by a list of tokens.
| The key name and each of the tokens are separated by spaces or tabs.
| The list of key names is given in the next section.
| A token may be:
| - a single character
| - a single character preceded by a ^ (circumflex) character
| - a number from 0 to 255 or a hex value from 0x00 to 0xff
| - one of the strings :SPACE:, :ENTER:, :BACKSPACE:, :TAB:, :ESCAPE:
| 
| For example, the following ~spec~ reads the file :myfile::
|:   myfile
| 
| Another ~spec~ says to use the :termcap: interface mappings.
|:   termcap
|
| The following ~spec~ in :myfile: might contain:
|:   termcap
|:   UPARROW ^x ^x
| In this example, the standard termcap mappings are loaded.
| In addition, an alias is defined which maps two consecutive control-X bytes
| into the up arrow discrete key value.
| 
|.List of Discrete Key Values
| Here is the list of discrete key values returned for various special keys
| which typically deliver multi-byte sequences when pressed.
| The second column illustrates the :ctlkeys: compiled-in mapping set.
| DT_KEY_UPARROW	control-P	
| DT_KEY_DOWNARROW	control-N
| DT_KEY_LEFTARROW	control-B
| DT_KEY_RIGHTARROW	control-F
| DT_KEY_HOME		control-A
| DT_KEY_END		control-E
| DT_KEY_PAGEUP		control-R
| DT_KEY_PAGEDOWN	control-V
| DT_KEY_F1         control-X 1 acts like F1
| DT_KEY_F2     	control-X 2 acts like F2
| DT_KEY_F3     	control-X 3 acts like F3
| DT_KEY_F4     	control-X 4 acts like F4
| DT_KEY_F5     	control-X 5 acts like F5
| DT_KEY_F6			no control key mapping
| DT_KEY_F7			no control key mapping
| DT_KEY_F8			no control key mapping
| DT_KEY_F9			no control key mapping
| DT_KEY_F10		no control key mapping
| DT_KEY_ENTER		normal ASCII character 0x0a or 0x0d
| DT_KEY_SPACE		normal ASCII character 0x20
| DT_KEY_BACKSPACE	normal ASCII character 0x08
| DT_KEY_INSERT		no control key mapping
| DT_KEY_TAB		normal ASCII character 0x09
| DT_KEY_ERASEEOL	control-K
| DT_KEY_BACKTAB	no control key mapping
| DT_KEY_DELETE 	control-D
| DT_KEY_DELETE 	0x7f, the ANSI DEL key	
| DT_KEY_STOP		control-C stops program
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/keybd $RCSfile: keymap.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/keymap.h>
#include <dtack/str.h>

										/* key name translation should be 	*/
										/* done by dt_key object functions 	*/
#define KEYS \
    KEY(UPARROW) \
    KEY(DOWNARROW) \
    KEY(LEFTARROW) \
    KEY(RIGHTARROW) \
    KEY(PAGEDOWN) \
    KEY(PAGEUP) \
    KEY(F1) \
    KEY(F2) \
    KEY(F3) \
    KEY(F4) \
    KEY(F5) \
    KEY(F6) \
    KEY(F7) \
    KEY(F8) \
    KEY(F9) \
    KEY(F10) \
    KEY(ENTER) \
    KEY(SPACE) \
    KEY(BACKSPACE) \
    KEY(DELETE) \
    KEY(INSERT) \
    KEY(TAB) \
    KEY(HOME) \
    KEY(END) \
    KEY(ERASEEOL) \
    KEY(BACKTAB) \
    KEY(STOP)

static struct {
  dt_key_t key;
  char *name;
} namelist[56] = {
#   define KEY(NAME) {DT_KEY_ ## NAME, #NAME}, {DT_KEY_ ## NAME, "DT_KEY_" #NAME},
	KEYS
#   undef KEY
    {0, NULL}
  };

static const char *choices[] = {
  "none",
  "termcap",
  "qnxterm",
  "ctlkeys",
  NULL
};

static dtkeymap_init_f *methods[] = {
  NULL,									/* no method for "none" 			*/
  dtkeymap_termcap_init,
  dtkeymap_qnxterm_init,
  dtkeymap_ctlkeys_init			
};

/*..........................................................................
| NAME
| dtkeymap_init() - initialize keymap
| 
| SUMMARY
| Initialize the keymap object descriptor.
| Create an initial mapping list by parsing ~spec~ with dtkeymap_parse().
| If ~spec~ is NULL or empty, create an empty keymap list.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeymap_init(							/* initialize keymap 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  const char *spec,						/* initial keymap spec 				*/
  unsigned long flags)					/* control flags 					*/
{
  DT_F("dtkeymap_init");
  char message[128];
  dt_rc_e rc;

  if (spec == NULL)
    spec = "";

  memset(keymap, 0, sizeof(*keymap));	/* clear structure				    */

  keymap->maxstring =					/* set up for key mapping 			*/
    sizeof(keymap->s);

  keymap->m = DTKEYMAP_MAXENTRIES;

  keymap->spec = spec;

  DT_C(dtos_malloc2,(ctl,				/* space for the mapping list 		*/
    (void **)&keymap->entry,
    keymap->m * 
    sizeof(*keymap->entry),
    F, "keymap entries"));

  DT_C(dtkeymap_parse,(ctl,				/* see if we can parse it 			*/
    keymap, spec,
    message, sizeof(message)));
  if (message[0])						/* parse failed? 					*/
  {
	rc = dt_err(ctl, F,
      "%s", message);
	goto cleanup;
  }

  return DT_RC_GOOD;
  
cleanup:
  if (keymap->entry != NULL)
  {
    DT_I(dtos_free2,(ctl, keymap->entry,
      F, "keymap entries"));
	keymap->entry = NULL;
  }

  return rc;
}

/*..........................................................................
| NAME
| dtkeymap_uninit() - release keymap
| 
| SUMMARY
| Release resources used by keymap object descriptor and
| clear the object descriptor structure to zero.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeymap_uninit(						/* release keymap 					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap)					/* keymap object descriptor 		*/
{
  DT_F("dtkeymap_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (keymap->uninit)
    DT_Q((keymap->uninit),(ctl, 
      keymap));
  if (keymap->entry != NULL)
    DT_I(dtos_free2,(ctl, keymap->entry,
      F, "keymap entries"));
  memset(keymap, 0, sizeof(*keymap));	/* clear structure to zero 			*/
  return rc;
}

/*..........................................................................
| NAME
| dtkeymap_map() - add single key mapping
| 
| SUMMARY
| Add a single entry to the keymap list which associates
| the raw bytes in ~string~ with the ~key~ value.
| If :DTKEYMAP_FLAG_NEW: is set, ignore the new entry if there is an
| existing entry for ~key~.
| If :DTKEYMAP_FLAG_REPLACE: is set, replace the first existing entry
| for ~key~ with the new mapping.
| Otherwise add the new mapping to the end of the keymap list.
|
| Have an error if the keymap list cannot be expanded.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeymap_map(							/* add single key mapping			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  dt_key_t key,							/* key value 						*/
  const char *string,					/* raw bytes 						*/
  long flags)							/* control flags 					*/
{
  DT_F("dtkeymap_map");
  int i;
  int n = keymap->n;

  for (i=0; i<n; i++)					/* see if we already have the key   */
    if (keymap->entry[i].k == key &&
		!strncmp(keymap->entry[i].s, 
          string, keymap->entry[i].l))
        break;
  if (i < n)
    return DT_RC_GOOD;

  if ((flags &
       DTKEYMAP_FLAG_NEW) ||			/* only supposed to add if new?		*/
      (flags &
       DTKEYMAP_FLAG_REPLACE))			/* or supposed to replace?			*/
  {
    for (i=0; i<n; i++)					/* see if we already have the key   */
      if (keymap->entry[i].k == key)
        break;
    if ((i < n) &&						/* found it?					    */
        (flags &
         DTKEYMAP_FLAG_NEW))			/* only supposed to add if new?		*/
      return DT_RC_GOOD;
    n = i;								/* replace this one					*/
  }

  if (n == keymap->m)					/* new addition would overfill?		*/
    return dt_err(ctl, F,
      "map table full (%d)",
      keymap->m);

  if (strlen(keymap->s) +				/* check total string length	    */
      strlen(string) + 1 >
      (unsigned int)keymap->maxstring)
    return dt_err(ctl, F,
      "map string full (%d)",
      keymap->maxstring);

  keymap->entry[n].k = key;
  keymap->entry[n].s = keymap->s +
    strlen(keymap->s);
  keymap->entry[n].l = strlen(string);
  strcat(keymap->s, string);

  if (n == keymap->n)					/* we added one at the end?		    */
    keymap->n++;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtkeymap_builtin() - add predefined set of key mappings
| 
| SUMMARY
| Add a predefined set of entries to the keymap list.
| Expect ~name~ to be one of:
| dos	standard DOS raw bytes presented by getch()
| ansi	standard ANSI key sequences (aliases: sun vt100 xterm)
| dumb	control keys only, should work with virtually all keyboards
| Pass ~flags~ unchanged to dtkeymap_map() for each mapping added.
| 
| Take no action and have no error if ~name~ is unknown.
| Have an error if the keymap list becomes full while adding new entries.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeymap_builtin(						/* add predefined key mappings		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  const char *name,						/* name of predefined mapping set 	*/
  long flags)							/* control flags 					*/
{
  DT_F("dtkeymap_builtin");
  typedef struct {
    dt_key_t key;
    char string[8];
  } map_t;
  map_t *maps[4];
  int i, n;
  dt_rc_e rc = DT_RC_GOOD;

  static map_t ansi_maps[9] = {
    {DT_KEY_UPARROW,    {0xff, '[', 'A', 0}},
    {DT_KEY_DOWNARROW,  {0xff, '[', 'B', 0}},
    {DT_KEY_LEFTARROW,  {0xff, '[', 'D', 0}},
    {DT_KEY_RIGHTARROW, {0xff, '[', 'C', 0}},
    {DT_KEY_BACKSPACE,  {0x08, 0}},
    {DT_KEY_DELETE,     {0x7f, 0}},
    {DT_KEY_TAB,        {0x09, 0}},
    {DT_KEY_STOP,       {'C'-'@', 0}},	/* control-C stops program 			*/
    {0}
  };
  static map_t dos_maps[31] = {
    {DT_KEY_UPARROW,    {0xff, 0x48, 0}},
    {DT_KEY_DOWNARROW,  {0xff, 0x50, 0}},
    {DT_KEY_LEFTARROW,  {0xff, 0x4b, 0}},
    {DT_KEY_RIGHTARROW, {0xff, 0x4d, 0}},
    {DT_KEY_PAGEUP,     {0xff, 0x49, 0}},
    {DT_KEY_PAGEDOWN,   {0xff, 0x51, 0}},
    {DT_KEY_HOME,       {0xff, 0x47, 0}},
    {DT_KEY_END,        {0xff, 0x4f, 0}},
										/* win98 mappings??? 				*/
    {DT_KEY_UPARROW,    {0xe0, 0x48, 0}},
    {DT_KEY_DOWNARROW,  {0xe0, 0x50, 0}},
    {DT_KEY_LEFTARROW,  {0xe0, 0x4b, 0}},
    {DT_KEY_RIGHTARROW, {0xe0, 0x4d, 0}},
    {DT_KEY_PAGEUP,     {0xe0, 0x49, 0}},
    {DT_KEY_PAGEDOWN,   {0xe0, 0x51, 0}},
    {DT_KEY_HOME,       {0xe0, 0x47, 0}},
    {DT_KEY_END,        {0xe0, 0x4f, 0}},

    {DT_KEY_F1,         {0xff, 0x3b, 0}},
    {DT_KEY_F2,         {0xff, 0x3c, 0}},
    {DT_KEY_F3,         {0xff, 0x3d, 0}},
    {DT_KEY_F4,         {0xff, 0x3e, 0}},
    {DT_KEY_F5,         {0xff, 0x3f, 0}},
    {DT_KEY_F6,         {0xff, 0x40, 0}},
    {DT_KEY_F7,         {0xff, 0x41, 0}},
    {DT_KEY_F8,         {0xff, 0x42, 0}},
    {DT_KEY_F9,         {0xff, 0x43, 0}},
    {DT_KEY_BACKSPACE,  {0x08, 0}},
    {DT_KEY_DELETE,     {0xff, 0x53, 0}},
    {DT_KEY_TAB,        {0x09, 0}},
    {DT_KEY_ENTER,      {'\r', 0}},
    {DT_KEY_STOP,       {'C'-'@', 0}},	/* control-C stops program 			*/
    {0}
  };
  static map_t ctl_maps[21] = {
    {DT_KEY_UPARROW,    {'P'-'@', 0}},	/* control-P					   	*/
    {DT_KEY_DOWNARROW,  {'N'-'@', 0}},
    {DT_KEY_LEFTARROW,  {'B'-'@', 0}},
    {DT_KEY_RIGHTARROW, {'F'-'@', 0}},
    {DT_KEY_HOME,       {'A'-'@', 0}},
    {DT_KEY_END,        {'E'-'@', 0}},
    {DT_KEY_PAGEUP,     {'R'-'@', 0}},
    {DT_KEY_PAGEDOWN,   {'V'-'@', 0}},
    {DT_KEY_ERASEEOL,   {'K'-'@', 0}},
    {DT_KEY_F1,         {'X'-'@', '1', 0}},	/* control-X 1 acts like F1 	*/
    {DT_KEY_F2,         {'X'-'@', '2', 0}},	/* control-X 2 acts like F2 	*/
    {DT_KEY_F3,         {'X'-'@', '3', 0}},	/* control-X 3 acts like F3 	*/
    {DT_KEY_F4,         {'X'-'@', '4', 0}},	/* control-X 4 acts like F4 	*/
    {DT_KEY_F5,         {'X'-'@', '5', 0}},	/* control-X 5 acts like F5 	*/
    {DT_KEY_BACKSPACE,  {'\b', 0}},
    {DT_KEY_DELETE,     {'D'-'@', 0}},	/* control-D						*/
    {DT_KEY_DELETE,     {0x7f, 0}},		/* ANSI DEL key						*/
    {DT_KEY_TAB,        {'\t', 0}},
    {DT_KEY_ENTER,      {'\n', 0}},
    {DT_KEY_STOP,       {'C'-'@', 0}},	/* control-C stops program 			*/
    {0}
  };


  n = 0;
  if (!strcmp(name, "dos"))
    maps[n++] = dos_maps;

  if (!strcmp(name, "xterm") ||
      !strcmp(name, "ansi") ||
      !strcmp(name, "vt100") ||
      !strcmp(name, "sun"))
    maps[n++] = ansi_maps;

  if (!strcmp(name, "dumb"))			/* caller wants control keys?	    */
    maps[n++] = ctl_maps;

  for (i=0; i<n; i++)
  {
    map_t *map;
    for (map=maps[i]; map->key; map++)	/* for each map in list			    */
      DT_C(dtkeymap_map,(ctl,			/* map the key					    */
        keymap,
        map->key, map->string,
        flags));
  }

cleanup:
  
  return rc;
}


/*..........................................................................
| NAME
| dtkeymap_match() - look up key value using raw bytes
| 
| SUMMARY
| Search the keymap list for a discrete key value whose associated raw bytes
| match the given raw buffer.
| Expect ~n~ to be at least 1.
| If the raw bytes make an incomplete match, 
| set ~*ispartial~ to be true and leave ~*key~ undefined.
| If there is a complete match, return the key value from the keymap list.
| If there is no match at all, 
| return the key value set equal to the first raw byte.
| 
| Shift unused raw bytes to the start of the buffer before returning.
| Return the number of raw bytes remaining in the buffer after the match.
| 
| Emit debug under the :DT_DBG_MASK_ASYNC: mask.
| Set function return code to :DT_RC_STOP:
| if the matched key value is :DT_KEY_STOP:.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeymap_match(							/* look up key value 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  char *b,								/* raw bytes buffer					*/
  int n,								/* number of raw bytes 				*/
  int *r,								/* returned characters remaining	*/
  dt_key_t *key,						/* returned key value 				*/
  int *ispartial)						/* returned true if partial match 	*/
{
  DT_F("dtkeymap_match");
  int i;
  int l;

  for (i=0; i<keymap->n; i++)			/* search all entries				*/
  {
    l = keymap->entry[i].l;				/* length of map entry 				*/
	if (!strncmp(keymap->entry[i].s, b,	/* we have a match?					*/
          DT_MIN(n, l)))
	{
	  if (n < l)						/* partial match only? 				*/
	  {
		l = 0;							/* don't match any 					*/
	  }
	  else
	  {
        *key = keymap->entry[i].k;		/* use the mapped key			    */
        if (DT_DBG(ctl,					/* armed for this debug? 			*/
              DT_DBG_MASK_ASYNC))
		{
          char tmp1[48];
          char tmp2[16];
          DT_Q(dtstr_printable,(ctl, b, n, 
            tmp1, sizeof(tmp1)));
          DT_Q(dtkeymap_name_str,(ctl,
            keymap, *key, 
            tmp2, sizeof(tmp2)));
          dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
            "have map for <%s>, returning key <%s>",
            tmp1, tmp2);
		}
	  }
	  break;
	}
  }
  if (i == keymap->n)					/* no match?				   		*/
  {
    char tmp1[48];
    char tmp2[16];
    DT_Q(dtstr_printable,(ctl, b, n,
      tmp1, sizeof(tmp1)));
    DT_Q(dtstr_printable,(ctl, b, 1,
      tmp2, sizeof(tmp2)));
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "no map for <%s>,"
      " returning char <%s>",
      tmp1, tmp2);
    *key = b[0];						/* use the first key			    */
    l = 1;
  }

  if (l != 0)							/* some characters matched? 		*/
  for (i=l; i<n; i++)					/* remove the matched chars			*/
    b[i-l] = b[i];

  *r = n - l;							/* return characters left 			*/

  *ispartial = l == 0? 1: 0;			/* partial match? 					*/

  if (*key == DT_KEY_STOP)
    return DT_RC_STOP; 
  else
    return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtkeymap_name_str() - make string for key value
| 
| SUMMARY
| Fill ~string~ with the printable name of the discrete key value ~key~.
| Silently truncate the name if longer than ~lmax~-1.
| In no case store more characters than ~lmax~ in string.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeymap_name_str(						/* make string for key value 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  dt_key_t key,							/* the key value 					*/
  char *string,							/* return string 					*/
  int lmax)								/* maximum length of string 		*/
{
  DT_F("dtkeymap_name_str");
  int i;

  for (i=0; namelist[i].key; i++)		/* look for key in known list	  	*/
    if (namelist[i].key == key)
	{
      DT_Q(dtstr_printf,(ctl, 
        string, lmax,
        "%s", namelist[i].name))
      break;
	}

  if (!namelist[i].key)					/* unknown key?					    */
  {
	char charname[32];
	char c = key;
    DT_Q(dtstr_printable,(ctl,			/* make key entry printable		    */
      &c, 1,
      charname, sizeof(charname)));

    DT_Q(dtstr_printf,(ctl, 
      string, lmax,
      "%s", charname))
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtkeymap_maps_str() - make printable string of all mappings of key value
| 
| SUMMARY
| Print all raw key sequences for the given discrete key value.
| Set off each raw sequence with apostrophe characters
| and separated by the word "or".
| Silently truncate the printable output if longer than ~lmax~-1.
| In no case store more characters than ~lmax~ in string.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeymap_maps_str(						/* make string of mappings 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  dt_key_t key,							/* key value 						*/
  char *string,							/* returned string 					*/
  int lmax)								/* maximum length of string 		*/
{
  DT_F("dtkeymap_maps_str");
  int i, n;

  string[0] = '\0';

  n = 0;
  for (i=0; i<keymap->n; i++)			/* search list of keymap maps	    */
  if (keymap->entry[i].k == key)		/* is a map for the key we want? 	*/
  {
	char tmp[32];
    DT_Q(dtstr_printable,(ctl,			/* make map entry printable		    */
      keymap->entry[i].s, 
      keymap->entry[i].l, 
      tmp, sizeof(tmp)));

    DT_Q(dtstr_printf,(ctl,				/* add to output string 			*/
      string+strlen(string), 
      lmax-strlen(string),
      "%s`%s'", 
      n?" or ": "", tmp));
    n++;
	if (strlen(string) ==				/* output string is full?			*/
		(unsigned int)(lmax-1))
	  break;
  }

  if (n == 0)							/* this key had no map? 			*/
    DT_Q(dtstr_printf,(ctl,
      string+strlen(string), 
      lmax-strlen(string),
      "no maps"));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtkeymap_parse() - parse keymap string
| 
| SUMMARY
| Parse given ~spec~ consisting of tokens separated by spaces or tabs.
| 
| If first token in ~spec~ is a filename,
| recursively call this function for each line in the file.
| 
| If first token in ~spec~ is one of the predefined builtin
| keymap sets such as "dos", "ansi" or "dumb",
| load the corresponding keymap entries using dtkeymap_builtin().
| 
| If first token in ~spec~ is one of the operating system
| mapping interfaces such as "termcap", "qnxterm" or "ctlkeys",
| call the corresponding implementation initialization function
| such as dtkeymap_termcap_init().
| Give the initialization function the second token, if any, as an argument.
| 
| If not a file, builtin, or mapping interface,
| expect the first token to be a key value name such as :F1:.
| Expect all subsequent tokens to be raw bytes specified
| as a single letter, a number, or one of the literals
| SPACE, TAB, ESCAPE or ENTER.
| Add an entry to the keymap list which maps the raw bytes
| to the discrete key value.
| 
| If ~spec~ is empty or its first non-blank character is '#', ignore it.
| 
| Do not have an error for parsing problems, instead return an error message.
| Have an error if an initialization function is called
| but the keymap object already has one.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeymap_parse(							/* parse keymap spec 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtkeymap_t *keymap,					/* keymap object descriptor 		*/
  const char *spec,						/* spec to parse 					*/
  char *message,						/* returned diagnostic message 		*/
  int message_max)						/* max length of message 			*/
{
  DT_F("dtkeymap_parse");
# define MAXARGV (32)					/* max tokens on a line 			*/
  char *argv[MAXARGV];
  char *s = NULL;
  int argc;
  dtfd_t fd;
  int isreadable;
  int choice;
  int i, l;
  dt_rc_e rc;

  message[0] = '\0';					/* presume everything goes well 	*/
  fd.fd = -1;

  DT_C(dtos_strdup,(ctl, spec, &s));	/* space to parse in 				*/

  DT_C(dtstr_argv,(ctl, s,				/* parse spec into tokens 			*/
    argv, MAXARGV, &argc));
										/* ................................ */
  if (argc == 0)						/* spec is empty?					*/
  {
	goto cleanup;
  }
										/* ................................ */
  if (argv[0][0] == '#')				/* spec is a comment? 				*/
  {
	goto cleanup;
  }
										/* ................................ */
  DT_Q(dtos_isreadable,(ctl,
    argv[0], &isreadable));
  if (isreadable)						/* spec is a readable file? 		*/
  {
	char line[128];
	int lineno = 0;
	int nkeys = keymap->n;
	
	if (argc > 1)						/* too many tokens on the line? 	*/
	{
	  dtstr_printf(ctl,
        message, message_max,
        "found %d extra tokens after filename %s",
        argc-1, argv[0]);
	  goto cleanup;
	}

	DT_C(dtfd_open,(ctl, &fd,			/* open the file 					*/
      argv[0], "r"));
	do {
	  lineno++;
	  DT_C(dtfd_read_line,(ctl, &fd,
        line, sizeof(line)));
	  if (strlen(line) ==				/* filled up our string? 			*/
		  sizeof(line)-1 &&
		  line[strlen(line)-1] != '\n')	/* but didn't get to end of line? 	*/
	  {
		dtstr_printf(ctl,
          message, message_max,
          "file %s line %d too long",
          fd.filename, lineno);
		goto cleanup;
	  }
	  if (line[0] == '\n')				/* blank line? 						*/
	    continue;
	  DT_C(dtkeymap_parse,(ctl,			/* parse this line 					*/
        keymap, line, 
        message, message_max));
	  if (message[0])					/* message was issued? 				*/
	  {
	    dtstr_printf(ctl,				/* add filename and line 			*/
          message+strlen(message),
          message_max-strlen(message),
          " (file %s line %d)",
          fd.filename, lineno);
		goto cleanup;
	  }
	} while(line[0]);					/* while bytes left in file 		*/
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "added %d keys from file %s",
       keymap->n-nkeys, fd.filename);
	goto cleanup;
  }
										/* ................................ */
  {
	int nkeys = keymap->n;
	DT_C(dtkeymap_builtin,(ctl,			/* try builtin map entries 			*/
      keymap, argv[0], 0));
	if (nkeys < keymap->n)				/* it was a builtin name? 			*/
	{
	  if (argc > 1)						/* too many tokens on the line? 	*/
	  {
	    dtstr_printf(ctl,
          message, message_max,
          "found %d extra tokens after"
          " builtin %s",
          argc-1, argv[0]);
	    goto cleanup;
	  }
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "added %d keys from builtin %s",
         keymap->n-nkeys, argv[0]);
	  goto cleanup;
	}
  }
										/* ................................ */

  DT_C(dtstr_choice,(ctl, choices,		/* look up keymap type			    */
    argv[0], &choice));

  if (choice >= 0)						/* it's one of the types 			*/
  {
	if (argc > 2)						/* too many tokens on the line? 	*/
	{
	  dtstr_printf(ctl,
        message, message_max,
        "found %d extra tokens"
        " after keymap type %s",
        argc-2, argv[0]);
	  goto cleanup;
	}
	if (methods[choice] != NULL)		/* there is a method to initialize? */
      DT_C((methods[choice]),(ctl,	
        keymap, argc>1? argv[1]: NULL,
        0))
    goto cleanup;
  }
										/* ................................ */
  l = strlen(argv[0]);
  for (i=0; i<l; i++)
    argv[0][i] = toupper(argv[0][i]);
										/* ................................ */
  for (i=0; namelist[i].key; i++)		/* search list of key names 		*/
    if (!strcmp(namelist[i].name,		/* name matches? 					*/
          argv[0]))
	  break;

  if (namelist[i].key)					/* spec is the name of a key? 	*/
  {
	dt_key_t key = namelist[i].key;
	char map[MAXARGV+1];
	if (argc > MAXARGV)					/* too many characters on the line? */
	{
	  dtstr_printf(ctl,
        message, message_max,
        "found %d tokens but max is %d",
        argc, MAXARGV);
	  goto cleanup;
	}
	for (i=1; i<argc; i++)				/* loop for all characters 			*/
	{
	  DT_C(dtstr_parsechar,(ctl,		/* turn from readable into binary 	*/
        argv[i], &map[i-1],
        message, message_max));
	  if (message[0])
	    goto cleanup;
	}
	map[i-1] = '\0';					/* finish off the map string 		*/
	DT_C(dtkeymap_map,(ctl, keymap,
      key, map, 0));
	goto cleanup;
  }
										/* ................................ */
										/* unknown spec 					*/
  dtstr_printf(ctl,
    message, message_max,
    "\"%s\" is not a file,"
    " builtin or key name",
    spec);
										/* ................................ */

cleanup:
  if (s != NULL)
    DT_I(dtos_free,(ctl, s));

  if (fd.fd != -1)
	DT_I(dtfd_close,(ctl, &fd));

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
