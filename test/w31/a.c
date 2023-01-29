/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>

void
a1(dt_ctl_t *ctl)
{
  DT_F("a1");
  dt_dbg(ctl, F, DT_DBG_MASK_ANY, "function a1 called first");
  dt_dbg(ctl, F, DT_DBG_MASK_ANY, "function a1 called second");
  MessageBox(((HWND)NULL),
    "function a1 called",
    "Dtack",
    MB_OK);
  dt_dbg(ctl, F, DT_DBG_MASK_ANY, "function a1 called third");
}

int 
LibMain(
  HINSTANCE hInstance,
  WORD wDataSeg,
  WORD wHeapSize,
  LPSTR lpszCmdLine)
{
#ifdef NONO
  MessageBox(((HWND)NULL),
    "a.dll LibMain called",
    "Dtack",
    MB_OK);
#endif
  if(wHeapSize > 0)
    UnlockData(0);
  return(1);
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
