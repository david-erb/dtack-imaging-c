/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




//--------------------------------------------------------------------------
// netout to stdout
//--------------------------------------------------------------------------

#include <windows.h>
#include "dtax.h"
#include <dtack/list.h>
#include <dtack/ipc.h>
#include <dtack/os.h>
#include <dtack/dg.h>
#define DTDEPEND
#include <dtack/msw.h>
#include <dtack/errno.h>

#include "dtmj.h"
#include "netoutc.h"

#define CR (0x0d)
#define LF (0x0a)

#define CONNECTED_NOT      DTMJ_NETOUT_CONNECTED_NOT
#define CONNECTED_JPEGPUSH DTMJ_NETOUT_CONNECTED_JPEGPUSH
#define CONNECTED_TEXTPUSH DTMJ_NETOUT_CONNECTED_TEXTPUSH

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
/* wait for connection                                                      */

extern
HRESULT 
dtmj_netout_accept(
  dt_ctl_t *ctl,                        /* environment control              */
  dtdg_server_t *dgserver,              /* dg server object                 */
  const char *dgspec,                   /* dg server spec                   */
  int *connected)                       /* returned true if we have a client*/
{
  DT_F("dtmj_netout_accept");
  static char *q;
  dt_rc_e rc = DT_RC_GOOD;
  
  q = getenv("QUERY_STRING");
  if (q == NULL ||                      /* no query string?                 */
      strstr(q, "textpush"))            /* sender wants text only?          */
    *connected = CONNECTED_TEXTPUSH;
  else                                  /* sender wants real jpeg?          */
    *connected = CONNECTED_JPEGPUSH;
  
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "QUERY_STRING=\"%s\"", q? q: "NULL");

  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
/* stream out data                                                          */

extern
HRESULT 
dtmj_netout_stream(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  const void *buffer,
  long length,
  int *dgconnected)
{
  DT_F("dtmj_netout_c_stream");
  HANDLE h = GetStdHandle(
    STD_OUTPUT_HANDLE);
  long n;
  long got;
  dt_rc_e rc;

  for (n=0; n<length; n+=got)
  {
    BOOL ok;
    
    ok = WriteFile(h,                   /* write data to stream             */
      (char *)buffer + n, 
      DT_MIN(length-n, 
        DTCONFIG_WRITE_MAX),
      &got, NULL);
    
    if (!ok)                            /* write failed?                    */
    {
      DWORD e = GetLastError();
      char s[256];
      DTERRNO_LASTS(e, s, sizeof(s));
      rc = dt_err(ctl, F,
        "WriteFile("
        "0x%08lx, 0x%08lx, %ld"
        "&got, NULL)"
        DTERRNO_LASTFS,
        h,
        (char *)buffer + n, 
        DT_MIN(length-n, 
          DTCONFIG_WRITE_MAX),
        DTERRNO_LASTES(e, s));
      goto cleanup;
    }

    if (got == 0)                       /* end-of-file?                     */
    {
      *dgconnected = CONNECTED_NOT;
      break;
    }
  }

cleanup:
  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
/* check for feedback                                                       */

extern
HRESULT 
dtmj_netout_feedback(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver,
  char *cmd,
  long max)
{
  DT_F("dtmj_netout_feedback");
  HANDLE h = GetStdHandle(
    STD_INPUT_HANDLE);
  DWORD result;
  int l;
  dt_rc_e rc;

  WaitForSingleObject(h, 0);            /* check for input available        */
  if (result != WAIT_TIMEOUT)           /* no feedback available?           */
  {
    cmd[0] = '\0';
    goto cleanup;
  }

  l = 0;
  while(1)
  {
    BOOL ok;
    long got = 0;
    char c;
    
    ok = ReadFile(h, &c, 1,             /* read single character            */
      &got, NULL);
    
    if (!ok)                            /* write failed?                    */
    {
      DWORD e = GetLastError();
      char s[256];
      DTERRNO_LASTS(e, s, sizeof(s));
      rc = dt_err(ctl, F,
        "ReadFile("
        "0x%08lx, &c, 1, "
        "&got, NULL)"
        DTERRNO_LASTFS,
        h,
        DTERRNO_LASTES(e, s));
      goto cleanup;
    }

    if (got == 0)                       /* no more data?                    */
    {
      cmd[0] = '\0';
     goto cleanup;
    }

    if (l < max-1)                      /* buffer not overfull?             */
      cmd[l++] = c;

    if (c == '\n')
      break;
  }

  cmd[l] = '\0';

  while (l > 0 &&                       /* nuke the crlf's in the feedback  */
         cmd[l-1] == LF ||
         cmd[l-1] == CR)
    cmd[--l] = '\0';

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "feedback \"%s\"", cmd);
  
cleanup:
  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
// quit client connection due to stream error

extern
HRESULT 
dtmj_netout_hangup(
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver)
{
  DT_F("dtmj_netout_hangup");
  dt_rc_e rc = DT_RC_GOOD;

  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
/* release all server resources                                             */

extern
HRESULT 
dtmj_netout_destruct(                   /* release all server resources     */
  dt_ctl_t *ctl,
  dtdg_server_t *dgserver)
{
  DT_F("dtmj_netout_destruct");
  dt_rc_e rc = DT_RC_GOOD;

  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
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
