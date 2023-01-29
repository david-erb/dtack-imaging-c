/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h> 

#  define NOCOMPMAN                     /* dont include COMPMAN             */
#  define NODRAWDIB                     /* dont include DRAWDIB             */
#  define NOAVIFMT                      /* dont include AVI file format     */
#  define NOMMREG                       /* dont include MMREG               */
#  define NOAVIFILE                     /* dont include AVIFile interface   */
#  define NOMCIWND                      /* dont include AVIWnd class        */
#  define NOMSACM                       /* dont include ACM stuff           */
#  include <vfw.h>

#include <wnvvfe.h>

/*..........................................................................
 *..........................................................................*/

static
LRESULT FAR PASCAL 
dtfb_vfw_error(                         /* error callback                   */
  HWND hwnd, 
  int n, 
  LPCSTR s)
{
  dtfb_vfw_priv_t *priv;
  dt_ctl_t *ctl;

  priv = (dtfb_vfw_priv_t *)
    capGetUserData(hwnd);
  if (priv == NULL)                     /* safety check                     */
  {
    MessageBox((HWND)NULL, "NULL priv", 
      "Tableau Error", MB_OK);
    return (LRESULT)TRUE;
  }

  if (n == 0)
  {
    priv->rc = DT_RC_GOOD;
    return (LRESULT)TRUE;
  }

  ctl = priv->ctl;
  if (ctl == NULL)                      /* safety check                     */
  {
    MessageBox((HWND)NULL, "NULL ctl", 
      "Tableau Error", MB_OK);
    return (LRESULT)TRUE;
  }

  priv->rc = dt_err(ctl, 
    priv->action,
    "result %d: \"%s\"", n, s);

  return (LRESULT)TRUE;
}

/*..........................................................................
 *..........................................................................*/

main()
{
  HWND hwnd;

  hwnd = capCreateCaptureWindow(        /* create window for capture        */
    (LPSTR)"Video Monitor"),
    WS_OVERLAPPED | 
    WS_CAPTION |
    WS_MINIMIZEBOX |
    WS_SYSMENU |
    (priv->flag_minimize? 
       WS_MINIMIZE: WS_VISIBLE),
    0, 0, fb->xe, fb->ye,
    NULL, 0);

  capSetUserData(hwnd, priv);

  capSetCallbackOnError(hwnd,           /* define the error callback        */
    (FARPROC)dtfb_vfw_error);

  ACTION(priv, "capDriverConnect");
  result = capDriverConnect(            /* connect capture driver to window */
    hwnd, 0);                           /* card #0                          */

  WnvVfe_Open(hwnd);                    /* this causes error callback!!!    */

// The error is: 418 "Unsupported video format."

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
