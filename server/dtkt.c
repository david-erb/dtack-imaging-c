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
| As yet undocumented.
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

#include "dtt.h"

#define USAGE "usage: [params] [<dialog-spec>]\n"

#ifdef DTCONFIG_IMPLIED_MSW

#define NULL_HWND      ((HWND)NULL)
#define NULL_HINSTANCE ((HINSTANCE)NULL)
#define NULL_HMENU     ((HMENU)NULL)

#include <dtack/msw.h>						/* this will include windows.h      */

#define DTKT_BUTTON_W (70)
#define DTKT_BUTTON_H (30)

#define DTKT_COMMAND_EDIT   (0)                      
#define DTKT_COMMAND_DONE   (1)
#define DTKT_COMMAND_SHOW   (2)
#define DTKT_COMMAND_TAB    (3)
#define DTKT_COMMAND_RETURN (4)
#define DTKT_COMMAND_ESCAPE (5)

#define STATIC_W 100
#define H 20
#define EDIT_W 150

typedef struct {						/* child window tracking   			*/
  HWND hwnd;
  int x;
  int y;
  char contents[64];
  int isedit;
} dtkt_child_t;

typedef struct {						/* mother window and children       */
  dt_ctl_t *ctl;
  char *name;
  int initted;
  int nchildren;
  int xe;
  int ye;
  dtkt_child_t child[6];
} dtkt_window_info_t;

typedef dt_rc_e dtkt_msg_f(			/* dispatch one message type	    */
  dt_ctl_t *, 
  dtkt_window_info_t *, 
  HWND, WPARAM, LPARAM);

static dtkt_msg_f dtkt_show;		/* dispatched to by wndproc		    */
static dtkt_msg_f dtkt_done;
static dtkt_msg_f dtkt_cmd;
static dtkt_msg_f dtkt_paint;
static dtkt_msg_f dtkt_tab;
static dtkt_msg_f dtkt_enter;

# define INFO(H) ((dtkt_window_info_t *)GetWindowLong(H, 0))
# define DTKT_MESSAGE(M, F) case M: \
    {dtkt_window_info_t *w = INFO(hwnd); \
     dt_ctl_t *ctl = w->ctl; \
     DT_GI(dtkt_ ## F,(ctl, w, \
       hwnd, wParam, lParam)); } break;

#define COMMAND(M, F) \
  case DTKT_COMMAND_ ## M: \
    DT_Q(dtkt_ ## F,(ctl, \
      window_info, \
      hwnd, wParam, lParam)); \
  break;

/*..........................................................................*/
static
void redraw(
  dt_ctl_t *ctl,
  dtkt_child_t *child,
  HDC hdc)
{
  SelectObject(hdc,						/* background for text is white     */
    GetStockObject(WHITE_BRUSH));
  Rectangle(hdc, 
    child[0].x, 
    child[0].y-1,
    child[0].x+STATIC_W, 
    child[0].y+H+1);
  TextOut(hdc,							/* output field label               */
    child[0].x+2, child[0].y+2, 
    child[0].contents, 
    strlen(child[0].contents));
  child[1].contents[0] = '"';
  SendMessage(child[1].hwnd,			/* extract data from edit           */
    EM_GETLINE, 0,
    (DWORD)(child[1].contents+1));
  child[1].contents[strlen(child[1].contents)+1] = '\0';
  child[1].contents[strlen(child[1].contents)] = '"';
  TextOut(hdc,							/* output field echo                */
    child[1].x, child[1].y+H+2, 
    child[1].contents, 
    strlen(child[1].contents));
}

/*..........................................................................*/
static
void
dtkt_keygrab(							/* grab key if we are interested	*/
  HWND hwnd,
  UINT wMessage,
  WPARAM wParam,
  LPARAM lParam)
{
  int command;

  if (wMessage == WM_KEYUP)				/* we care about this message?		*/
  {
    if (wParam == VK_TAB)
      command = DTKT_COMMAND_TAB;
    else
    if (wParam == VK_ESCAPE)
      command = DTKT_COMMAND_ESCAPE;
    else
    if (wParam == VK_RETURN)
      command = DTKT_COMMAND_RETURN;
    else
      command = 0;
    if (command)						/* we care about this key?			*/
    {
      HWND parent = 
        (HWND)GetWindowWord(			/* find our parent				    */
          hwnd, GWW_HWNDPARENT);
      PostMessage(parent, WM_COMMAND,	/* queue a msg to parent		    */
	    command, 0L);
    }
  }
}

/*..........................................................................*/
LRESULT CALLBACK _export
dtkt_button_wndproc(					/* superclassing stock control		*/
  HWND hwnd,
  UINT wMessage,
  WPARAM wParam,
  LPARAM lParam)
{
  dtkt_keygrab(hwnd,
    wMessage, wParam, lParam);

  return CallWindowProc(				/* let the stock control have it   	*/
    dtmsw_button_wndproc,
    hwnd, wMessage,
    wParam, lParam);
}

/*..........................................................................*/
LRESULT CALLBACK _export
dtkt_edit_wndproc(					/* superclassing stock control		*/
  HWND hwnd,
  UINT wMessage,
  WPARAM wParam,
  LPARAM lParam)
{
  dtkt_keygrab(hwnd,
    wMessage, wParam, lParam);

  return CallWindowProc(				/* let the stock control have it   	*/
    dtmsw_edit_wndproc,
    hwnd, wMessage,
    wParam, lParam);
}


/*..........................................................................*/
static
dt_rc_e
dtkt_superclass(
  dt_ctl_t *ctl,
  char *classname,
  char *superclassname,
  WNDPROC new_wndproc,
  WNDPROC *old_wndproc)
{
  DT_F("dtkt_superclass");
  WNDCLASS class;
  BOOL wrc;

  if (*old_wndproc)						/* already have this one?		    */
    return DT_RC_GOOD;

  sprintf(superclassname, "dt%s",		/* make name for super class	    */
    classname);

  wrc = GetClassInfo(NULL_HINSTANCE,				/* get info on base class		    */
          classname, &class);
  if (!wrc)
    return dt_err(ctl, F,
      "GetClassInfo(NULL, \"%s\", ...) failed",
      classname);
  *old_wndproc        = class.lpfnWndProc;
  class.lpfnWndProc   = new_wndproc;
  class.cbClsExtra    = 4;
  class.hInstance     = dtmsw_hinstance;
  class.lpszClassName = superclassname;
  if (!RegisterClass(&class))
    return dt_err(ctl, F, 
	  "RegisterClass(\"%s\") failed",
      superclassname);
  dt_dbg(ctl, F, DT_DBG_MASK_WIN,
    "new class \"%s\" created",
    superclassname);

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtkt_log_create(
  dt_ctl_t *ctl,
  dtkt_window_info_t *window_info,
  HWND hwnd)
{
  DT_F("dtkt_log_create");
  HWND edit;
  HLOCAL bufhandle;
  char *buf;
  int i;

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "\"%s\" got CREATE",
    window_info->name);

  edit = CreateWindow(					/* create mother window				*/
	"EDIT", "",
	WS_CHILD |
    WS_VISIBLE |
    WS_BORDER |
    WS_HSCROLL |
    WS_VSCROLL |
    ES_AUTOVSCROLL |
    ES_LEFT |
    ES_MULTILINE,
    0, 0, window_info->xe-8, window_info->ye-30,
    hwnd, NULL_HMENU, dtmsw_hinstance, NULL);
  if (edit == 0)
    return dt_err(ctl, F,
      "CreateWindow for big edit failed");

  bufhandle = (HLOCAL)SendMessage(edit,
    EM_GETHANDLE, 0, 0L);

  bufhandle = LocalAlloc(LMEM_MOVEABLE,
    1000);
  if (bufhandle == NULL)
    return dt_err(ctl, F,
      "LocalAlloc(LMEM_MOVEABLE, 1000) failed");
  buf = LocalLock(bufhandle);

  PostMessage(edit, EM_SETHANDLE,		/* attach our memory			    */
    (UINT)bufhandle, 0L);

  for (i=0; i<100; i++)					/* fill edit buffer				    */
    sprintf(buf+i*10, 
      "line %3d\r\n", i);

  LocalUnlock(bufhandle);

  window_info->child[0].hwnd = edit;	/* save for later reference		    */

  return DT_RC_GOOD;
}

/*..........................................................................*/
static
dt_rc_e
dtkt_log_paint(
  dt_ctl_t *ctl,
  dtkt_window_info_t *window_info,
  HWND hwnd,
  WPARAM wParam,
  LPARAM lParam)
{
  DT_F("dtkt_log_paint");
  dt_rc_e rc = DT_RC_GOOD;

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "\"%s\" got PAINT", 
    window_info->name);
      
  if (!window_info->initted)			/* first paint?					    */
  {
	window_info->initted = 1;
	dt_dbg(ctl, F, DT_DBG_MASK_WIN,
      "initting \"%s\"",
	  window_info->name);
	DT_GI(dtkt_log_create,(ctl,
	  window_info, hwnd));
  }

  if (rc == DT_RC_GOOD)      
  {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    EndPaint(hwnd, &ps);
  }

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dtkt_log_done(
  dt_ctl_t *ctl,
  dtkt_window_info_t *window_info,
  HWND hwnd,
  WPARAM wParam,
  LPARAM lParam)
{
  DT_F("dtkt_log_done");

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "\"%s\" got DONE",
    window_info->name);

  PostQuitMessage(0);					/* quit message loop in WinMain		*/

  return DT_RC_GOOD;
}

#ifdef NONO
/*..........................................................................*/
static
dt_rc_e
dtkt_log_size(
  dt_ctl_t *ctl,
  dtkt_window_info_t *window_info,
  HWND hwnd,
  WPARAM wParam,
  LPARAM lParam)
{
  DT_F("dtkt_log_size");

  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
    "\"%s\" got SIZE",
    window_info->name);

  MoveWindow(window_info->child[0].hwnd,
    5, 0,
    LOWORD(lParam)-5,
    HIWORD(lParam), TRUE);

  return DT_RC_GOOD;
}
#endif

/*..........................................................................*/
LRESULT CALLBACK _export
dtkt_log_wndproc(
  HWND hwnd,
  UINT wMessage,
  WPARAM wParam,
  LPARAM lParam)
{
  DT_F("dtkt_log_wndproc");
  dt_rc_e rc = DT_RC_GOOD;

  printf("%04x ", wMessage);
  fflush(stdout);

  switch(wMessage)
  {
    DTKT_MESSAGE(WM_PAINT,   log_paint);
    DTKT_MESSAGE(WM_DESTROY, log_done);
    default:
      return DefWindowProc(hwnd,
        wMessage, wParam, lParam);
  }

  if (rc != DT_RC_GOOD)
    PostQuitMessage(0);

  return 0L;
}

/*..........................................................................*/

dt_rc_e
dtkt_log(
  dt_ctl_t *ctl)
{
  DT_F("dtkt_log");
  HWND hwnd;
  MSG msg;
  WNDCLASS wndclass;
  int sysrc;
  static dtkt_window_info_t window_info;
B
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = dtkt_log_wndproc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = sizeof(dtkt_window_info_t *);
  wndclass.hIcon = LoadIcon(NULL_HINSTANCE, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(NULL_HINSTANCE, IDC_ARROW);
  wndclass.hbrBackground = GetStockObject(WHITE_BRUSH);
  wndclass.hInstance = dtmsw_hinstance;
  wndclass.lpszMenuName = NULL;
  wndclass.lpszClassName = "dtktl";
  sysrc = RegisterClass(&wndclass);
  if (sysrc == 0)
    return dt_err(ctl, F,
      "RegisterClass failed");
  
  window_info.name = "DTKT";
  window_info.ctl = ctl;
  window_info.initted = 0;
  window_info.xe = 520;
  window_info.ye = 240;

  hwnd = CreateWindow(					/* create mother window				*/
	"dtkt",
    window_info.name,
	WS_OVERLAPPEDWINDOW,
    32, 230,							/* mother window location		   	*/
    window_info.xe,						/* mother window size		       	*/
    window_info.ye+32,
    NULL_HWND, NULL_HMENU, dtmsw_hinstance, NULL);
  if (hwnd == 0)
    return dt_err(ctl, F,
      "CreateWindow for mother failed");

  SetWindowLong(hwnd, 0,				/* associate info with window       */
    (long)&window_info);

  ShowWindow(hwnd, dtmsw_ncmdshow);
  UpdateWindow(hwnd);

  while(GetMessage(&msg,				/* loop until DONE button		   */
    NULL_HWND, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

#endif

/*..........................................................................*/

DTT_MAIN(dtkt)
{
  DT_F("dtkt");
  dt_ctl_t *ctl = &param->ctl;
  int assert_fail = 0;
  dt_rc_e rc = DT_RC_GOOD;
  int n;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking             */

										/* ................................ */
  DT_GI(dtparam_init,(param));			/* init param structure             */

  DTPARAM_DEFAULT(dbg, 0);				/* set default for params we want   */
  DTPARAM_DEFAULT(op, "default");

  DT_GI(dt_interactive_parse,(			/* go parse args                    */
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;

  if (argc != 0)
    return dt_interactive_usage(
      param, USAGE);


#ifdef DTCONFIG_IMPLIED_MSW
										/* ................................ */
  if (!strcmp(param->op, "dialog") ||
      !strcmp(param->op, "wait"))
  {
    DT_GI(dtkt_dialog,(ctl));
  }
										/* ................................ */
  if (!strcmp(param->op, "log") ||
      !strcmp(param->op, "default") ||
      !strcmp(param->op, "wait"))
  {
    DT_GI(dtkt_log,(ctl));
  }

										/* ................................ */
#else
  if (strcmp(param->op, "default"))		/* caller wants specific test?      */
  {
    DT_Q(dt_assert_defined,(ctl, 0, F,
      "DTCONFIG_IMPLIED_MSW", 
      "Microsoft Windows"));
    assert_fail++;
  }
#endif
										/* ................................ */

  DT_I(dt_interactive_leave,(			/* clean up before leaving          */
    param, rc));

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed        */
    &assert_fail));

  if (assert_fail)						/* any failed assertions?           */
    return DT_RC_BAD;
  else
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
