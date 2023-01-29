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
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/t.h>
DT_RCSID("test $RCSfile: dttw31.c,v $ $Revision: 1.15 $");

/*..........................................................................*/

DTT_MAIN(dttw31)
{
  DT_F("dttw31");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  char tmp[80];
  dt_rc_e rc;

  sprintf(tmp, "hello E, mask=0x%08lx", ctl->dbg_mask);
  MessageBox(((HWND)NULL), tmp, "Dtack", MB_OK);

  arg[0].desc = NULL;
  rc = dt_enter_parse(argc, argv, F, "usage", param, arg, NULL);

  sprintf(tmp, "rc %d, mask=0x%08lx, feedback=%ld, outwin=%ld", 
    rc, ctl->dbg_mask, ctl->feedbackwin, ctl->outwin);
  MessageBox(((HWND)NULL), tmp, "Dtack", MB_OK);

  SendMessage((HWND)ctl->outwin, 
    LB_ADDSTRING, 0,
    (DWORD)(LPSTR)"hello");

  dt_dbg(ctl, F, DT_DBG_MASK_ANY, 
    "MASK_ANY debug to window=%ld", 
    ctl->outwin);

  sprintf(tmp, "should be debug by now");
  MessageBox(((HWND)NULL), tmp, "Dtack", MB_OK);

  dt_dbg(ctl, F, DT_DBG_MASK_ANY, "good-bye");
#ifdef NONO
#endif

  MessageBox(((HWND)NULL),
    "good-bye",
    "Dtack",
    MB_OK);

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
