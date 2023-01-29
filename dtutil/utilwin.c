/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#ifndef DTDEPEND
#include <windows.h>
#include <string.h>
#include <stdio.h>
#endif


#define DTMAIN_MESSAGE \
  { \
    char tmp[128]; \
    sprintf(tmp, 

#define DTMAIN_BOX ); \
    MessageBox((HWND)NULL, tmp, "dtutil", MB_OK); \
  }

int
_pascal _export
dtutil_string_global(
  const char *string,
  int len)
{
  HANDLE handle;
  char t[32];

  DTMAIN_MESSAGE "string=0x%08lx, len=%d", string, len DTMAIN_BOX

  strncpy(t, string, 10);
  t[9] = '\0';

  DTMAIN_MESSAGE "string=\"%s\"...", t DTMAIN_BOX

  DTMAIN_MESSAGE "string[%d]=0x%02x '%c'", 
    len-1, string[len-1], string[len-1] DTMAIN_BOX

  DTMAIN_MESSAGE "string=\"%s\"", string DTMAIN_BOX

  handle = GlobalAlloc(GMEM_MOVEABLE|GMEM_SHARE, len+1);

  DTMAIN_MESSAGE "handle=0x%04x, size=%ld, flags=0x%04x", 
    (int)handle, GlobalSize(handle), GlobalFlags(handle) DTMAIN_BOX

  if (handle)
  {
    char _huge *h = GlobalLock(handle);
	DTMAIN_MESSAGE "h=0x%08lx", h DTMAIN_BOX
    if (h)
    {
      char _far *p = (char _far *)(h+0);
	  DTMAIN_MESSAGE "p=0x%08lx", p DTMAIN_BOX
      *p = 0;
	  DTMAIN_MESSAGE "0 p=0x%08lx", p+0 DTMAIN_BOX
      p[15] = 0;
	  DTMAIN_MESSAGE "15 p=0x%08lx", p+15 DTMAIN_BOX
      p[16] = 0;
	  DTMAIN_MESSAGE "16 p=0x%08lx", p+16 DTMAIN_BOX
      memcpy(p, string, len);
      p[len] = '\0';
      GlobalUnlock(handle);
	}
  }
  return (int)handle;
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
