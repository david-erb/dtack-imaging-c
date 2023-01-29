/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


#include <stdio.h>
#include <windows.h>


#define DTMAIN_MESSAGE \
  { \
    char tmp[512]; \
    sprintf(tmp, 

#define DTMAIN_BOX ); \
    MessageBox((HWND)NULL, tmp, "dtwinexec", MB_OK); \
  }

										/* ................................type */

int 
PASCAL 
WinMain(                                               
  HINSTANCE hInstance,                                                     
  HINSTANCE hPrevInstance,                                                 
  LPSTR lpszCmdLine,                                                       
  int nCmdShow)                                                            
{       																	 
  HANDLE handle;

  handle = WinExec(lpszCmdLine, nCmdShow);
  if (handle < 32)
  {
    DTMAIN_MESSAGE "Sorry, had error %d"
      " trying to run \"%s\".", 
      (int)handle, lpszCmdLine DTMAIN_BOX
  }
  
  return 0;
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
