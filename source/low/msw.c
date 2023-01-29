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
  CHANGES
  20041027 DE don't yield when kicking because might eat VB's response too quick
  20001130 DE use custom wndproc and exit on certain messages
  20001130 DE different logic in yield to call defwndproc better
  20000615 DE yield for posted messages also, call DefWindowProc
  19991009 DE bug calling yield with NULL ctl
 *..........................................................................*/


  //  {  // if you wanted to see the invisible window then:
  //	dtmsw_t *msw = (dtmsw_t *)ctl->msw;
  //	ShowWindow((HWND)msw->pidwin, SW_SHOW);
  //	dt_dbg(ctl, F, DT_DBG_MASK_USR1,
  //	  "activated window 0x%08lx", msw->pidwin);
  //  }


/*..........................................................................
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: msw.c,v $ $Revision: 1.1 $");

#include <dtack/msw.h>
#include <dtack/errno.h>

#define MAX_OUT (28000)

static
void
out_string(
  dt_ctl_t *ctl,
  const char *string,
  int length);

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmsw_message_string(					/* convert message to string		*/
  dt_ctl_t *ctl,
  int message,
  const char **string)
{
#define MESS(S, M) case M: *string = #S; break;
  switch(message)
  {
  MESS(WM_NULL,                          0x0000)
  MESS(WM_CREATE,                        0x0001)
  MESS(WM_DESTROY,                       0x0002)
  MESS(WM_MOVE,                          0x0003)
  MESS(WM_SIZE,                          0x0005)
  MESS(WM_ACTIVATE,                      0x0006)
  MESS(WM_SETFOCUS,                      0x0007)
  MESS(WM_KILLFOCUS,                     0x0008)
  MESS(WM_ENABLE,                        0x000A)
  MESS(WM_SETREDRAW,                     0x000B)
  MESS(WM_SETTEXT,                       0x000C)
  MESS(WM_GETTEXT,                       0x000D)
  MESS(WM_GETTEXTLENGTH,                 0x000E)
  MESS(WM_PAINT,                         0x000F)
  MESS(WM_CLOSE,                         0x0010)
  MESS(WM_QUERYENDSESSION,               0x0011)
  MESS(WM_QUIT,                          0x0012)
  MESS(WM_QUERYOPEN,                     0x0013)
  MESS(WM_ERASEBKGND,                    0x0014)
  MESS(WM_SYSCOLORCHANGE,                0x0015)
  MESS(WM_ENDSESSION,                    0x0016)
  MESS(WM_SHOWWINDOW,                    0x0018)
  MESS(WM_WININICHANGE,                  0x001A)
  MESS(WM_DEVMODECHANGE,                 0x001B)
  MESS(WM_ACTIVATEAPP,                   0x001C)
  MESS(WM_FONTCHANGE,                    0x001D)
  MESS(WM_TIMECHANGE,                    0x001E)
  MESS(WM_CANCELMODE,                    0x001F)
  MESS(WM_SETCURSOR,                     0x0020)
  MESS(WM_MOUSEACTIVATE,                 0x0021)
  MESS(WM_CHILDACTIVATE,                 0x0022)
  MESS(WM_QUEUESYNC,                     0x0023)
  MESS(WM_GETMINMAXINFO,                 0x0024)
  MESS(WM_PAINTICON,                     0x0026)
  MESS(WM_ICONERASEBKGND,                0x0027)
  MESS(WM_NEXTDLGCTL,                    0x0028)
  MESS(WM_SPOOLERSTATUS,                 0x002A)
  MESS(WM_DRAWITEM,                      0x002B)
  MESS(WM_MEASUREITEM,                   0x002C)
  MESS(WM_DELETEITEM,                    0x002D)
  MESS(WM_VKEYTOITEM,                    0x002E)
  MESS(WM_CHARTOITEM,                    0x002F)
  MESS(WM_SETFONT,                       0x0030)
  MESS(WM_GETFONT,                       0x0031)
  MESS(WM_SETHOTKEY,                     0x0032)
  MESS(WM_GETHOTKEY,                     0x0033)
  MESS(WM_QUERYDRAGICON,                 0x0037)
  MESS(WM_COMPAREITEM,                   0x0039)
  MESS(WM_COMPACTING,                    0x0041)
  MESS(WM_COMMNOTIFY,                    0x0044)
  MESS(WM_WINDOWPOSCHANGING,             0x0046)
  MESS(WM_WINDOWPOSCHANGED,              0x0047)
  MESS(WM_POWER,                         0x0048)
  MESS(WM_COPYDATA,                      0x004A)
  MESS(WM_CANCELJOURNAL,                 0x004B)
  MESS(WM_NOTIFY,                        0x004E)
  MESS(WM_INPUTLANGCHANGEREQUEST,        0x0050)
  MESS(WM_INPUTLANGCHANGE,               0x0051)
  MESS(WM_TCARD,                         0x0052)
  MESS(WM_HELP,                          0x0053)
  MESS(WM_USERCHANGED,                   0x0054)
  MESS(WM_NOTIFYFORMAT,                  0x0055)
  MESS(WM_CONTEXTMENU,                   0x007B)
  MESS(WM_STYLECHANGING,                 0x007C)
  MESS(WM_STYLECHANGED,                  0x007D)
  MESS(WM_DISPLAYCHANGE,                 0x007E)
  MESS(WM_GETICON,                       0x007F)
  MESS(WM_SETICON,                       0x0080)
  MESS(WM_NCCREATE,                      0x0081)
  MESS(WM_NCDESTROY,                     0x0082)
  MESS(WM_NCCALCSIZE,                    0x0083)
  MESS(WM_NCHITTEST,                     0x0084)
  MESS(WM_NCPAINT,                       0x0085)
  MESS(WM_NCACTIVATE,                    0x0086)
  MESS(WM_GETDLGCODE,                    0x0087)
  MESS(WM_NCMOUSEMOVE,                   0x00A0)
  MESS(WM_NCLBUTTONDOWN,                 0x00A1)
  MESS(WM_NCLBUTTONUP,                   0x00A2)
  MESS(WM_NCLBUTTONDBLCLK,               0x00A3)
  MESS(WM_NCRBUTTONDOWN,                 0x00A4)
  MESS(WM_NCRBUTTONUP,                   0x00A5)
  MESS(WM_NCRBUTTONDBLCLK,               0x00A6)
  MESS(WM_NCMBUTTONDOWN,                 0x00A7)
  MESS(WM_NCMBUTTONUP,                   0x00A8)
  MESS(WM_NCMBUTTONDBLCLK,               0x00A9)
  MESS(WM_KEYDOWN,                       0x0100)
  MESS(WM_KEYUP,                         0x0101)
  MESS(WM_CHAR,                          0x0102)
  MESS(WM_DEADCHAR,                      0x0103)
  MESS(WM_SYSKEYDOWN,                    0x0104)
  MESS(WM_SYSKEYUP,                      0x0105)
  MESS(WM_SYSCHAR,                       0x0106)
  MESS(WM_SYSDEADCHAR,                   0x0107)
  MESS(WM_KEYLAST,                       0x0108)
  MESS(WM_IME_STARTCOMPOSITION,          0x010D)
  MESS(WM_IME_ENDCOMPOSITION,            0x010E)
  MESS(WM_IME_COMPOSITION,               0x010F)
  MESS(WM_INITDIALOG,                    0x0110)
  MESS(WM_COMMAND,                       0x0111)
  MESS(WM_SYSCOMMAND,                    0x0112)
  MESS(WM_TIMER,                         0x0113)
  MESS(WM_HSCROLL,                       0x0114)
  MESS(WM_VSCROLL,                       0x0115)
  MESS(WM_INITMENU,                      0x0116)
  MESS(WM_INITMENUPOPUP,                 0x0117)
  MESS(WM_MENUSELECT,                    0x011F)
  MESS(WM_MENUCHAR,                      0x0120)
  MESS(WM_ENTERIDLE,                     0x0121)
  MESS(WM_CTLCOLORMSGBOX,                0x0132)
  MESS(WM_CTLCOLOREDIT,                  0x0133)
  MESS(WM_CTLCOLORLISTBOX,               0x0134)
  MESS(WM_CTLCOLORBTN,                   0x0135)
  MESS(WM_CTLCOLORDLG,                   0x0136)
  MESS(WM_CTLCOLORSCROLLBAR,             0x0137)
  MESS(WM_CTLCOLORSTATIC,                0x0138)
  MESS(WM_MOUSEMOVE,                     0x0200)
  MESS(WM_LBUTTONDOWN,                   0x0201)
  MESS(WM_LBUTTONUP,                     0x0202)
  MESS(WM_LBUTTONDBLCLK,                 0x0203)
  MESS(WM_RBUTTONDOWN,                   0x0204)
  MESS(WM_RBUTTONUP,                     0x0205)
  MESS(WM_RBUTTONDBLCLK,                 0x0206)
  MESS(WM_MBUTTONDOWN,                   0x0207)
  MESS(WM_MBUTTONUP,                     0x0208)
  MESS(WM_MBUTTONDBLCLK,                 0x0209)
  MESS(WM_PARENTNOTIFY,                  0x0210)
  MESS(WM_ENTERMENULOOP,                 0x0211)
  MESS(WM_EXITMENULOOP,                  0x0212)
  MESS(WM_NEXTMENU,                      0x0213)
  MESS(WM_SIZING,                        0x0214)
  MESS(WM_CAPTURECHANGED,                0x0215)
  MESS(WM_MOVING,                        0x0216)
  MESS(WM_POWERBROADCAST,                0x0218)
  MESS(WM_DEVICECHANGE,                  0x0219)
  MESS(WM_IME_SETCONTEXT,                0x0281)
  MESS(WM_IME_NOTIFY,                    0x0282)
  MESS(WM_IME_CONTROL,                   0x0283)
  MESS(WM_IME_COMPOSITIONFULL,           0x0284)
  MESS(WM_IME_SELECT,                    0x0285)
  MESS(WM_IME_CHAR,                      0x0286)
  MESS(WM_IME_KEYDOWN,                   0x0290)
  MESS(WM_IME_KEYUP,                     0x0291)
  MESS(WM_MDICREATE,                     0x0220)
  MESS(WM_MDIDESTROY,                    0x0221)
  MESS(WM_MDIACTIVATE,                   0x0222)
  MESS(WM_MDIRESTORE,                    0x0223)
  MESS(WM_MDINEXT,                       0x0224)
  MESS(WM_MDIMAXIMIZE,                   0x0225)
  MESS(WM_MDITILE,                       0x0226)
  MESS(WM_MDICASCADE,                    0x0227)
  MESS(WM_MDIICONARRANGE,                0x0228)
  MESS(WM_MDIGETACTIVE,                  0x0229)
  MESS(WM_MDISETMENU,                    0x0230)
  MESS(WM_ENTERSIZEMOVE,                 0x0231)
  MESS(WM_EXITSIZEMOVE,                  0x0232)
  MESS(WM_DROPFILES,                     0x0233)
  MESS(WM_MDIREFRESHMENU,                0x0234)
  MESS(WM_CUT,                           0x0300)
  MESS(WM_COPY,                          0x0301)
  MESS(WM_PASTE,                         0x0302)
  MESS(WM_CLEAR,                         0x0303)
  MESS(WM_UNDO,                          0x0304)
  MESS(WM_RENDERFORMAT,                  0x0305)
  MESS(WM_RENDERALLFORMATS,              0x0306)
  MESS(WM_DESTROYCLIPBOARD,              0x0307)
  MESS(WM_DRAWCLIPBOARD,                 0x0308)
  MESS(WM_PAINTCLIPBOARD,                0x0309)
  MESS(WM_VSCROLLCLIPBOARD,              0x030A)
  MESS(WM_SIZECLIPBOARD,                 0x030B)
  MESS(WM_ASKCBFORMATNAME,               0x030C)
  MESS(WM_CHANGECBCHAIN,                 0x030D)
  MESS(WM_HSCROLLCLIPBOARD,              0x030E)
  MESS(WM_QUERYNEWPALETTE,               0x030F)
  MESS(WM_PALETTEISCHANGING,             0x0310)
  MESS(WM_PALETTECHANGED,                0x0311)
  MESS(WM_HOTKEY,                        0x0312)
  MESS(WM_PRINT,                         0x0317)
  MESS(WM_PRINTCLIENT,                   0x0318)
  MESS(WM_HANDHELDFIRST,                 0x0358)
  MESS(WM_HANDHELDLAST,                  0x035F)
  MESS(WM_AFXFIRST,                      0x0360)
  MESS(WM_AFXLAST,                       0x037F)
  MESS(WM_PENWINFIRST,                   0x0380)
  MESS(WM_PENWINLAST,                    0x038F)
  MESS(DTMSW_MESSAGE_PAUSE,              DTMSW_MESSAGE_PAUSE)
  MESS(DTMSW_MESSAGE_RESUME,             DTMSW_MESSAGE_RESUME)
  MESS(DTMSW_MESSAGE_STOP,               DTMSW_MESSAGE_STOP)
  MESS(DTMSW_MESSAGE_DESTROY,            DTMSW_MESSAGE_DESTROY)
  default: *string = "unknown windows message";
  }
  return DT_RC_GOOD;

}

LRESULT CALLBACK dtmsw_wndproc(
    HWND hwnd,        // handle to window
    UINT msg,        // message identifier
    WPARAM wParam,    // first message parameter
    LPARAM lParam)    // second message parameter
{
  const char *F = "dtmsw_wndproc";
  dt_ctl_t *ctl;
  dtmsw_t *msw;
  const char *msg_text;
  ctl = (dt_ctl_t *)GetWindowLong(hwnd, 24);
  if (!ctl) goto cleanup;

  msw = (dtmsw_t *)ctl->msw;	/* reference special msw structure 	*/
  if (!msw) goto cleanup;

  msw->n_getmessage++;				/* for debug					    */

  dtmsw_message_string(ctl,			/* look up message string 			*/
    msg, &msg_text);

  if (DT_DBG(ctl, DT_DBG_MASK_ASYNC))
  {
	dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "0x%08x 0x%04x (%s), 0x%04x, 0x%08lx",
	  hwnd, msg, msg_text, wParam, lParam);
  }

  if (msg ==					/* got close or end-session?		*/
	  WM_CLOSE ||
	  //msg ==
	  //WM_QUERYENDSESSION ||
	  msg ==
	  WM_ENDSESSION ||
	  msg ==
	  WM_QUIT)
  {
	dt_err(ctl, F, 
      "got %s, calling ExitProcess(0)", msg_text);
	ExitProcess(0);
  }
cleanup:
  return DefWindowProc(hwnd,
				msg,
	wParam, lParam);
}

/*..........................................................................
 *..........................................................................*/


#ifdef DTCONFIG_IMPLIED_MSW

dt_rc_e
dtmsw_pidwin(							/* make invisible pid window		*/
  dt_ctl_t *ctl,
  int *pidwin)
{
  DT_F("dtmsw_pidwin");
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;	/* reference special msw structure 	*/
  char modulename[256];
  WNDCLASS c;
  ATOM atom;
  int i;

  if (msw == NULL)
    return dt_err(ctl, F,
      "no msw structure");

  if (msw->pidwin != 0)					/* already have one?			    */
  {
    *pidwin = msw->pidwin;				/* just use it					    */
    return DT_RC_GOOD;
  }

  if (msw->hinstance == 0)				/* program instance not known yet?	*/
    msw->hinstance = (int)				/* get Win32 Program Instance	   	*/
      GetModuleHandle(NULL);

  if (msw->hinstance == 0)
    return dt_err(ctl, F,
      "no hinstance in msw structure");

  if (ctl->progname == NULL)
    GetModuleFileName(NULL, modulename,
      sizeof(modulename));
  else
	strcpy(modulename, ctl->progname);

  for (i=0; modulename[i]!='\0'; i++)	/* make class/window name lowercase */
	modulename[i] =
	  tolower(modulename[i]);

  DT_MEMSET(&c, 0, sizeof(c));
  c.lpfnWndProc = dtmsw_wndproc; // DefWindowProc;
  c.hInstance =							/* make sure invisible class exists */
    (HINSTANCE)msw->hinstance;
  c.lpszClassName = modulename;
  c.cbClsExtra = 64;
  c.cbWndExtra = 64;

  atom = RegisterClass(&c);				/* on win32 second instance doesn't */
  if (atom == 0)						/*   return 0! 						*/
    return dt_err(ctl, F,
      "RegisterClass(\"%s\") failed"
#     ifdef DTCONFIG_IMPLIED_MSW32
        " (GetLastError=%ld)"
#     endif
      ,
      c.lpszClassName
#     ifdef DTCONFIG_IMPLIED_MSW32
        , GetLastError()
#     endif
    );

  msw->pidwin = (int)CreateWindow(		/* create the invisible window 		*/
    c.lpszClassName,					/* class name 						*/
    c.lpszClassName,					/* window name 						*/
	WS_OVERLAPPEDWINDOW,
    100, // CW_USEDEFAULT,
    200, // CW_USEDEFAULT,
    200, // CW_USEDEFAULT,
    100, // CW_USEDEFAULT,
    (HWND)NULL, 
    (HMENU)NULL,
    (HINSTANCE)msw->hinstance, 
    NULL);
  if (msw->pidwin == 0)
  {
	DWORD e = GetLastError();
	char s[4096];
    DTERRNO_LASTS(e, s, sizeof(s));
	return dt_err(ctl, F,
      "CreateWindow(\"%s\") failed" 
      DTERRNO_LASTFS,
				  c.lpszClassName,
      DTERRNO_LASTES(e, s));
  }

  SetWindowLong((HWND)msw->pidwin, 24, ctl);

  *pidwin = msw->pidwin;

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "pidwin \"%s\" is 0x%04x",
    c.lpszClassName, msw->pidwin);

#ifdef NONO
  {
	FILE *file;
	file = fopen("pidwin.out", "w");	/* open scratch file 			*/
	if (file != NULL)
	{
	  fprintf(file,						/* write pid and window name 		*/
        "pidwin \"%s\" is 0x%04x\n",
        c.lpszClassName, msw->pidwin);
	  fclose(file);
	}
  }
#endif
  
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtmsw_out_string
| 
| DESCRIPTION
| Put string into window.
| 
| RETURN VALUES
| None.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

static
void
out_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
{
  DT_F("dtmsw_out_string");
  dtmsw_t *msw;
  char *s;
  int n, m, i;

  if (ctl == NULL ||
	  ctl->msw == NULL)
  {
	if (string != NULL)
  	  fwrite(string, 1, length, stdout);
	return;
  }

  msw = (dtmsw_t *)ctl->msw;			/* reference special msw structure 	*/

#if 0
  printf("msw->outwin=0x%08lx\n",
    (long)msw->outwin);
#endif

#if defined(DTCONFIG_IMPLIED_MSW16) &&									  	  \
    defined(DTCONFIG_MICROSOFT)			/* QuickWin available?				*/
#define USE_QUICKWIN_NOT
#endif

#ifdef USE_QUICKWIN						/* use QuickWin? 					*/
  if ((long)msw->outwin == 1)			/* persistent output window?		*/
	_wsetexit(_WINEXITPERSIST);
  else
	_wsetexit(_WINEXITNOPERSIST);
#else
#if defined(DTCONFIG_IMPLIED_MSW16)
  return;								/* don't even try for VC++ Win16!   */
#endif
#endif

#ifdef DTCONFIG_IMPLIED_MSW32
  if (string == NULL)					/* this is the final call? 			*/
  {
	if ((long)msw->outwin == 1 &&		/* outputting to console? 			*/
		msw->n_outstring > 0)			/* and something was written? 		*/
	{
	  HANDLE handle;
	  const char *s = "hit ENTER to close this window";
	  DWORD got;
	  char c[1];

	  handle = GetStdHandle(			/* handle to console output buffer 	*/
  		STD_OUTPUT_HANDLE);
	  WriteFile(handle, s, strlen(s),	/* put our string in console buffer */
        &got, NULL);
	  handle = GetStdHandle(			/* handle to console input buffer 	*/
  		STD_INPUT_HANDLE);
	  ReadFile(handle, c, sizeof(c),	/* wait for key						*/
        &got, NULL);
	  msw->outwin = NULL;				/* only delay once 					*/
	}
	else
	if (msw->outwin != NULL &&
		(long)msw->outwin < 16 &&		/* outputting to console? 			*/
		msw->n_outstring > 0)			/* and something was written? 		*/
	{
	  HANDLE handle;
	  const char *s = "this window will self destruct soon";
	  DWORD got;
	  handle = GetStdHandle(			/* handle to console output buffer 	*/
  		STD_OUTPUT_HANDLE);
	  WriteFile(handle, s, strlen(s),	/* put our string in console buffer */
        &got, NULL);
	  Sleep((long)msw->outwin*1000);	/* easy win32 call 					*/
	  msw->outwin = NULL;				/* only delay once 					*/
	}
  }
#endif
  if (string == NULL)					/* this is the final call? 			*/
    return;

  if (length < 0)						/* caller not giving length? 		*/
    length = strlen(string);

  s = msw->outbuf;			
  n = msw->outbufn;
  m = msw->outbufm;

  for (i=0; i<length; i++)				/* for each input character			*/
  {
	if (string[i] == '\n' ||			/* newline in input string? 		*/
		n > m-4)
	{
	  s[n>m-4? m-3: n+0] = 0x0d;
	  s[n>m-4? m-2: n+1] = 0x0a;
	  s[n>m-4? m-1: n+2] = '\0';

      if (msw->outwin == NULL)			/* no outwin given to us?		    */
	  {
	  }
#     ifdef USE_QUICKWIN				/* use QuickWin? 					*/
	  else
	  if ((long)msw->outwin < 16)		/* supposed to use console?			*/
	  {
		if (msw->console == 0)
		{
		  struct _wopeninfo wi;
		  struct _wsizeinfo ws;
		  wi._version = _QWINVER;
		  wi._title = "title";
		  wi._wbufsize = _WINBUFDEF;
		  ws._version = _QWINVER;
		  ws._type = _WINSIZEMAX;
		  msw->console = _wgetfocus();
		  _wsetsize(msw->console, &ws);
		}
		printf("%s", s);
	  }
#     endif
#     ifdef DTCONFIG_IMPLIED_MSW32		/* win32 console feature available? */
	  else
	  if ((long)msw->outwin < 16)		/* supposed to use console?			*/
	  {
		HANDLE handle;
		DWORD written;
		AllocConsole();					/* get console if we don't have one */
		handle = GetStdHandle(			/* handle to console output buffer 	*/
  		  STD_OUTPUT_HANDLE);
		WriteFile(handle, s, strlen(s),	/* put our string in console buffer */
          &written, NULL);
	  }
#     endif
      else
#     ifdef DTCONFIG_IMPLIED_MSW16
	  {
  	    SendMessage((HWND)msw->outwin, 
          EM_REPLACESEL, 0,
          (DWORD)(LPSTR)s);
	  }
#     endif
#     ifdef DTCONFIG_IMPLIED_MSW32
	  {
		if (msw->n_outstring > MAX_OUT)	 /* output full? 					*/
		{
		  SendMessage((HWND)msw->outwin, /* select first half 				*/
            EM_SETSEL, 0,
            (DWORD)(msw->n_outstring/2));
		  SendMessage((HWND)msw->outwin, /* delete first half 				*/
            EM_REPLACESEL, 0,
            (DWORD)(LPSTR)(""));
		  SendMessage((HWND)msw->outwin, /* move back to end 				*/
            EM_SETSEL, -1,
            (DWORD)(MAX_OUT));
		  msw->n_outstring -=
		    msw->n_outstring/2;
		}
  	    SendMessage((HWND)msw->outwin,	/* put new line into output 		*/
          EM_REPLACESEL, 0,
          (DWORD)(LPSTR)s);
	  }
#     endif
      msw->n_outstring += strlen(s);	/* count chars we output			*/
	  if (n > m-4)						/* involuntary continuation? 		*/
	  {
		s[0] = s[1] = s[2] = '>';		/* start next line with >>> 		*/
		n = 3;
	  }
	  else
	    n = 0;
	}
	else								/* not newline in input string? 	*/
	  s[n++] = string[i];
  }
  msw->outbufn = n;

}
#endif

/*..........................................................................*/
void
DTCONFIG_API1
dtmsw_dbg_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
{
#ifdef DTCONFIG_IMPLIED_MSW
  out_string(ctl, string, length);
#endif
}

/*..........................................................................*/
void
DTCONFIG_API1
dtmsw_err_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
{
#ifdef DTCONFIG_IMPLIED_MSW
  out_string(ctl, string, length);
#endif
}

/*..........................................................................*/
void
DTCONFIG_API1
dtmsw_say_string(
  dt_ctl_t *ctl,
  const char *string,
  int length)
{
#ifdef DTCONFIG_IMPLIED_MSW
  out_string(ctl, string, length);
#endif
}

/*..........................................................................*/
static
dt_rc_e 
dtmsw_post_message(
  dt_ctl_t *ctl,
  int message,
  int wparam,
  long lparam,
  const char *F)
{
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;	/* reference special msw structure 	*/
  if (msw == NULL)
    return DT_RC_GOOD;
#ifdef DTCONFIG_IMPLIED_MSW
  else
  if ((long)msw->outwin >= 16)			/* a real window? 					*/
  {
	BOOL winrc;
	winrc = PostMessage(				/* kick the window 					*/
      (HWND)msw->outwin,	
      message, wparam, lparam);
	if (winrc != TRUE)
	{
#     ifdef DTCONFIG_IMPLIED_MSW32
	  DWORD winerr = GetLastError();
#     else
	  DWORD winerr = 0;
#     endif
      return dt_err(ctl, F,
        "PostMessage(0x%08lx,"
        " %d, 0x%04x, 0x%08lx)"
        " failed, GetLastError=%ld",
        msw->outwin, message, wparam, lparam,
        winerr);
	}
  }
#endif
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsw_pause(
  dt_ctl_t *ctl)
{
  DT_F("dtmsw_pause");
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;	/* reference special msw structure 	*/
#ifdef DTCONFIG_IMPLIED_MSW
  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, "%s pausing on pidwin 0x%08lx", ctl->progname, msw->pidwin);

  while(1)
  {
	MSG msg;
	GetMessage(&msg, 
      (HWND)msw->pidwin, 0, 0);
	msw->n_getmessage++;				/* for debug					    */
    
    if (msg.message ==					/* got redundant pause? 			*/
		DTMSW_MESSAGE_PAUSE)
	{
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "DTMSW_MESSAGE_PAUSE received"
        " (but already paused)");
	}
    else
    if (msg.message ==					/* got resume? 						*/
		DTMSW_MESSAGE_RESUME)
	{
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "DTMSW_MESSAGE_RESUME received");
	  break;
	}
    else
    if (msg.message ==					/* got stop? 						*/
		DTMSW_MESSAGE_STOP)
	{
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "DTMSW_MESSAGE_STOP received");
	  return DT_RC_STOP;
	}
    else
    if (msg.message ==					/* got close? 						*/
		WM_CLOSE)
	{
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "WM_CLOSE received");
	  return DT_RC_STOP;
	}
	else								/* got paint?						*/
    if (msg.message == WM_PAINT)
	{
	  PAINTSTRUCT paintstruct;
	  HDC dc;
	  RECT rect;
	  char t[256];
	  dc = BeginPaint(msg.hwnd, &paintstruct);
	  //	  GetClientRect(msg.hwnd, &rect); 
	  //	  FillRect(dc, &rect, GetStockObject(GRAY_BRUSH)); 
	  //	  sprintf(t, "%08ld", msw->n_getmessage);
	  //	  TextOut(dc, 0, 0, t, strlen(t));
	  EndPaint(msg.hwnd, &paintstruct);
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "WM_PAINT received");
	}
	else
    if (msg.message ==					/* got windows quit? 				*/
		WM_QUIT)
	{
      return dt_err(ctl, F, 
        "WM_QUIT received");
	}
	else
    if (msg.message ==					/* got request to end session?		*/
		WM_QUERYENDSESSION)
	{
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "WM_QUERYENDSESSION received");
	  return DT_RC_STOP;
	}
	else
    if (msg.message ==					/* got session-end notification?	*/
		WM_ENDSESSION)
	{
      dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "WM_ENDSESSION received");
	  return DT_RC_STOP;
	}
	else
	{
	  const char *s;
	  dtmsw_message_string(ctl,
        msg.message, &s);
  	  dt_dbg(ctl, F, 
        DT_DBG_MASK_ASYNC,
        "%d=0x%04x=%s"
        " on pidwin 0x%08x ignored",
        msg.message,
        msg.message,
        s,
		msw->pidwin);
	}
  }
#endif
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dtmsw_yield_win(
  dt_ctl_t *ctl,
  dtmsw_t *msw,
  int win)
{
  DT_F("dtmsw_yield_win");
  dt_rc_e rc = DT_RC_GOOD;
  
#ifdef DTCONFIG_IMPLIED_MSW
  {
  MSG msg;
  int n = 0;

	if (win != -1 &&
	    win != 0 &&
		!IsWindow((HWND)win))
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
			 "window 0x%08lx is gone, stopping",
			 win);
	  return DT_RC_STOP;
    }

  while (rc == DT_RC_GOOD &&
    PeekMessage(&msg,
      (HWND)win, 
      0, 0, PM_REMOVE))
  {
	int do_defwindowproc = 1;
	const char *msg_text;

	msw->n_getmessage++;				/* for debug					    */
    n++;

    dtmsw_message_string(ctl,			/* look up message string 			*/
      msg.message, &msg_text);
	dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "0x%08x 0x%04x (%s), 0x%04x, 0x%08lx",
	  win,
      msg.message, msg_text, msg.wParam,
      msg.lParam);

	msw->n_peekmessage++;				/* for debug					    */
    if (msg.message == 
		DTMSW_MESSAGE_PAUSE)
	{
      do_defwindowproc = 0;
	  DT_Q(dtmsw_pause,(ctl));			/* wait for resume message 			*/
	}
    else
    if (msg.message ==					/* got redundant resume? 			*/
		DTMSW_MESSAGE_RESUME)
	{
      do_defwindowproc = 0;
	}
    else
    if (msg.message ==					/* got stop? 						*/
		DTMSW_MESSAGE_STOP)
	{
      do_defwindowproc = 0;
	  rc = DT_RC_STOP;
	}
    else
    if (msg.message ==					/* got close or end-session?		*/
		WM_CLOSE ||
	    msg.message ==
		WM_QUERYENDSESSION ||
        msg.message ==
		WM_ENDSESSION)
	{
	  rc = DT_RC_STOP;
	}
	else								/* got paint?						*/
    if (msg.message == WM_PAINT)
	{
	  PAINTSTRUCT paintstruct;
	  HDC dc;
	  RECT rect;
	  char t[256];
      do_defwindowproc = 0;
	  //	  dc = BeginPaint(msg.hwnd, &paintstruct);
	  //	  GetClientRect(msg.hwnd, &rect); 
	  //	  FillRect(dc, &rect, GetStockObject(GRAY_BRUSH)); 
	  //	  sprintf(t, "%08ld", msw->n_getmessage);
	  //	  TextOut(dc, 0, 0, t, strlen(t));
	  EndPaint(msg.hwnd, &paintstruct);
	}
	else								/* got windows quit? 				*/
    if (msg.message == WM_QUIT)
	{
      rc = dt_err(ctl, F, 
        "WM_QUIT received");
	}

	if (do_defwindowproc)
      DefWindowProc(win,
        msg.message,
        msg.wParam, msg.lParam);
  }

  if (n == 0 &&
      DT_DBG(ctl, DT_DBG_MASK_ASYNC) &&
      DT_DBG(ctl, DT_DBG_MASK_PIXEL))
    dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
      "no messages on window 0x%08x",
      win);

  }
#endif
  if (msw)
    msw->n_yield++;						/* for debug					    */
  return rc;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtmsw_yield(
  dt_ctl_t *ctl)
{
  DT_F("dtmsw_yield");
  dtmsw_t *msw;

  if (!ctl)
	return DT_RC_GOOD;

  msw = (dtmsw_t *)ctl->msw;			/* reference special msw structure 	*/

  if (!msw)
	return DT_RC_GOOD;

  DT_Q(dtmsw_yield_win,(ctl, msw,		/* check for window messages		*/
    msw->pidwin));
  DT_Q(dtmsw_yield_win,(ctl, msw,		/* check for posted messages		*/
    -1));
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmsw_kick(
  dt_ctl_t *ctl,
  dtmsw_kick_e kick)
{
  DT_F("dtmsw_kick");
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;
  if (msw != NULL &&
	  msw->outwin != NULL &&
	  (long)msw->outwin >= 16)
  {
	int key;
    long keyflags;
    switch(kick)
    {
      case DTMSW_KICK_FEEDBACK:
	    key = DTMSW_KEY_FEEDBACK;
		keyflags = DTMSW_KEYFLAGS_FEEDBACK;
      break;
      case DTMSW_KICK_LAUNCHED:
	    key = DTMSW_KEY_LAUNCHED;
		keyflags = DTMSW_KEYFLAGS_LAUNCHED;
      break;
      case DTMSW_KICK_STATUS:
	    key = DTMSW_KEY_STATUS;
		keyflags = DTMSW_KEYFLAGS_STATUS;
      break;
      case DTMSW_KICK_FINISH:
	    key = DTMSW_KEY_FINISH;
		keyflags = DTMSW_KEYFLAGS_FINISH;
      break;
	  default:
	    key = 0;
	    keyflags = 0;
    }
	if (key != 0)						/* we know what to do? 				*/
	{
#     ifdef DTCONFIG_IMPLIED_MSW
	  DT_Q(dtmsw_post_message,(ctl,
        WM_KEYDOWN, 
        key, (DWORD)keyflags, F));
#     endif
	  msw->n_kick++;					/* for debug					    */
	}
  }
  //  DT_Q(dtmsw_yield,(ctl));				/* kick implies a yield 			*/
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmsw_status(
  dt_ctl_t *ctl,
  const char *string)
{
  DT_F("dtmsw_status");
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;

  if (msw != NULL &&
	  msw->statuswin != NULL)			/* we have a status window? 		*/
  {
#  ifdef DTCONFIG_IMPLIED_MSW
    PostMessage((HWND)msw->statuswin,	/* replace with new string 			*/
      WM_SETTEXT, 0,
      (DWORD)(LPSTR)string);
#  endif

#  ifdef DTCONFIG_IMPLIED_MSW32

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "send status \"%s\", statuswin=0x%08lx",
    string, msw->statuswin);

    SendMessage((HWND)msw->statuswin,	/* select entire text box			*/
      EM_SETSEL, 0, (LPARAM)(INT)(-1));
  	SendMessage((HWND)msw->statuswin, 	/* replace with new string 			*/
      EM_REPLACESEL, 0,
      (DWORD)(LPSTR)string);
#  endif
  }
  if (msw != NULL)
    msw->n_status++;					/* for debug					    */
  DT_Q(dtmsw_kick,(ctl,					/* notify about status change 		*/
    DTMSW_KICK_STATUS));
  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtmsw_feedback(							/* output feedback line 			*/
  dt_ctl_t *ctl,
  unsigned long flags,
  const char *string)
{
  DT_F("dtmsw_feedback");
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;	/* reference special msw structure 	*/
  if (ctl->feedbackwin != NULL)			/* we have a feedback window? 		*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
	  "sending \"%s\", feedbackwin=0x%08lx", string, ctl->feedbackwin);

#ifdef DTCONFIG_IMPLIED_MSW
    if (flags & 1)						/* caller wants to append? 			*/
      SendMessage(
        (HWND)ctl->feedbackwin, 
        LB_ADDSTRING, 0,
        (DWORD)(LPSTR)string);
    else
      SendMessage(						/* caller wants to replace? 		*/
        (HWND)ctl->feedbackwin, 
        LB_INSERTSTRING, 0,
        (DWORD)(LPSTR)string);
#else
    printf("%s\n", string);
#endif
    if (msw != NULL)
      msw->n_feedback++;				/* for debug					    */
  }
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
