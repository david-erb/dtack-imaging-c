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
| dtkeymap_qnxterm_init() - qnx key code mapping
| 
| SUMMARY
| Implementation function for keywords returning qnx key codes.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/keybd $RCSfile: keymapq.c,v $ $Revision: 1.1 $");

#include <dtack/os.h>
#include <dtack/keymap.h>

#ifndef DTDEPEND
#  if DTPRJ_QNXTERM == DT_1
#    include <sys/qnxterm.h>
#  endif
#endif

#if DTPRJ_QNXTERM == DT_1				/* we have qnxterm linked?		   */

/*..........................................................................*/

static
dt_rc_e
dtkeymap_qnxterm_builtin(				/* do built-in keymap mappings	*/
  dt_ctl_t *ctl,
  dtkeymap_t *keymap,
  unsigned long flags)
{
  DT_F("dtkeymap_qnxterm_builtin");
  typedef struct {
    dt_key_t key;
    char string[8];
  } map_t;
  map_t *map;

# define SPLIT(K) 0xff, ((K)&0xff)
  static map_t maps[23] = {
    {DT_KEY_UPARROW,    {SPLIT(K_UP), 0}},
    {DT_KEY_DOWNARROW,  {SPLIT(K_DOWN), 0}},
    {DT_KEY_LEFTARROW,  {SPLIT(K_LEFT), 0}},
    {DT_KEY_RIGHTARROW, {SPLIT(K_RIGHT), 0}},
    {DT_KEY_PAGEUP,     {SPLIT(K_PGUP), 0}},
    {DT_KEY_PAGEDOWN,   {SPLIT(K_PGDN), 0}},
    {DT_KEY_HOME,       {SPLIT(K_HOME), 0}},
    {DT_KEY_END,        {SPLIT(K_END), 0}},
    {DT_KEY_F1,         {SPLIT(K_F1), 0}},
    {DT_KEY_F2,         {SPLIT(K_F2), 0}},
    {DT_KEY_F3,         {SPLIT(K_F3), 0}},
    {DT_KEY_F4,         {SPLIT(K_F4), 0}},
    {DT_KEY_F5,         {SPLIT(K_F5), 0}},
    {DT_KEY_F6,         {SPLIT(K_F6), 0}},
    {DT_KEY_F7,         {SPLIT(K_F7), 0}},
    {DT_KEY_F8,         {SPLIT(K_F8), 0}},
    {DT_KEY_F9,         {SPLIT(K_F9), 0}},
    {DT_KEY_BACKSPACE,  {0x7f, 0}},				/* why not K_BACKSP???		*/
    {DT_KEY_DELETE,     {SPLIT(K_DELETE), 0}},
    {DT_KEY_TAB,        {K_TAB, 0}},
    {DT_KEY_ENTER,      {K_ENTER, 0}},
    {DT_KEY_STOP,       {'C'-'@', 0}},	/* control-C stops program 			*/
    {0}
  };

  for (map=maps; map->key; map++)		/* for each map in list			    */
    DT_Q(dtkeymap_map,(ctl,				/* map the key					    */
      keymap,
      map->key, map->string,
      flags));

  return DT_RC_GOOD;
}
#endif

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtkeymap_qnxterm_init(					
  dt_ctl_t *ctl,
  dtkeymap_t *keymap,
  const char *spec,					  
  unsigned long flags)
{
  DT_F("dtkeymap_qnxterm_init");

# if DTPRJ_QNXTERM == DT_1
  
  DT_Q(dtkeymap_qnxterm_builtin,(ctl,
    keymap, flags));

  return DT_RC_GOOD;

# else
  return dt_err_notlinked(ctl, F, "qnx keyboard");  
# endif
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
