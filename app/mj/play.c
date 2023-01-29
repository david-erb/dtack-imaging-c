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

#include <mj.h>
DT_RCSID("app/mj $RCSfile: server.c,v $ $Revision: 1.8 $");
#include <dtack/msw.h>

#ifndef DTDEPEND
#include <windows.h>
#include <commdlg.h>
#include <string.h>

#include <objbase.h>
#include <streams.h>
#include <initguid.h>
#include <strmif.h>
#include <control.h>
#include <uuids.h>
#include <evcode.h>						/* active movie event codes 		*/
#include <errors.h>						/* active movie error list 			*/
#endif

#include <com/mj1-3/guid.h>				/* mj project GUID's 				*/
#include <com/mj1-3/grabit_prop.h>		/* grabit property interface 		*/
#include <com/mj1-3/encode_prop.h>		/* encode property interface 		*/
#include <com/mj1-3/push_prop.h>		/* push property interface 			*/

#ifdef NONO
#ifdef AMGetErrorText
#undef AMGetErrorText
#endif
#define AMGetErrorText(H, B, M) (0)
#endif

typedef enum {
  STATE_UNINITIALIZED = 0,
  STATE_LOADED,
  STATE_RUNNING,
  STATE_STOPPED
} state_e;

#define HR_C(FUNC, ARGS)				/* check rc and goto cleanup		*/\
{                                                                             \
  DT_SETLOC                                                                   \
  HRESULT hr = FUNC ARGS;		                                              \
  if (hr & 0x8000000LU)                                                       \
  {                                                                           \
    TCHAR t[256];															  \
    if (!AMGetErrorText(hr, t, sizeof(t)/sizeof(*t))) t[0] = 0;      		  \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "%s returned hr 0x%08lx (%s)", #FUNC, hr, t);	                          \
    rc = DT_RC_BAD;                                                           \
    goto cleanup;                                                             \
  }                                                                           \
  else                                                                        \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "%s succeeded (0x%08lx)", #FUNC, hr);                                   \
}

#define HR_I(FUNC, ARGS)				/* do func, only set rc if bad		*/\
{                                                                             \
  dt_rc_e trc;          		                                              \
  DT_SETLOC                                                                   \
  HRESULT hr = FUNC ARGS;		                                              \
  if (hr & 0x8000000LU)                                                       \
  {                                                                           \
    TCHAR t[256];															  \
    if (!AMGetErrorText(hr, t, sizeof(t)/sizeof(*t))) t[0] = 0;      		  \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "%s returned hr 0x%08lx (%s)", #FUNC, hr, t);	                          \
    if (rc == DT_RC_GOOD)                                                     \
      rc = DT_RC_BAD;                                                         \
  }                                                                           \
  else                                                                        \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "%s succeeded (0x%08lx)", #FUNC, hr);                                   \
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
mj_play(
  dtparam_t *param)
{
  DT_F("mj_server");
  dt_ctl_t *ctl = &param->ctl;
  IGraphBuilder *graph = NULL;
  IMediaControl *control = NULL;
  HANDLE ehandle;
  WCHAR path[256];
  IMediaEvent *event = NULL;
  IEnumFilters *filters = NULL;
  IFilter *filter = NULL;
  dtmj_grabit_prop_i *grabit_prop = NULL;
  dtmj_encode_prop_i *encode_prop = NULL;
  dtmj_push_prop_i *push_prop = NULL;
  long code, param1, param2;
  state_e state = STATE_UNINITIALIZED;
  dt_rc_e rc;
								
  CoInitialize(NULL);
										/* ................................ */

  HR_C(CoCreateInstance,(
    CLSID_FilterGraph,					/* CLSID of filter graph object		*/
	NULL,								/* outer unknown					*/
	CLSCTX_INPROC_SERVER,				/* type of server					*/
	IID_IGraphBuilder,					/* interface wanted					*/
	(void **)&graph));					/* returned object					*/

  HR_C(graph->QueryInterface,(			/* filter graph's media control 	*/
    IID_IMediaControl, 
    (void **)&control));

  HR_C(graph->QueryInterface,(			/* filter graph's event 			*/
    IID_IMediaEvent, 
    (void **)&event));

  HR_C(event->GetEventHandle,(			/* event handle to wait on 			*/
    (OAEVENT *)&ehandle));

    MultiByteToWideChar( CP_ACP, 0, 
      param->input, -1, 
      path, sizeof(path));

  HR_C(graph->RenderFile,(path, NULL));	/* load filter graph 				*/
  state = STATE_LOADED;

										/* ................................ */
  HR_C(graph->EnumFilters,(&filters));	/* get graph's filter list			*/

  while(1)
  {
	ULONG n;
	HR_C(filters->Next,(1, &filter, 	/* access next filter 				*/
      &n));
	if (n == 0)							/* no more filters? 				*/
	  break;

	HR_C(filter->QueryInterface,(		/* see if filter does grabit_prop	*/
      dtmj_grabit_prop_i_guid, 
      (void **)&grabit_prop));
	if (grabit_prop != NULL)			/* filter does grabit_prop?			*/
    {
	  HR_C(grabit_prop->put_ctl,(ctl));	/* give ctl to grabit_prop			*/
	  HR_C(grabit_prop->Release());		/* release access to grabit_prop	*/
	  grabit_prop = NULL;
	}

	HR_C(filter->QueryInterface,(		/* see if filter does encode_prop	*/
      dtmj_encode_prop_i_guid, 
      (void **)&encode_prop));
	if (encode_prop != NULL)			/* filter does encode_prop?			*/
    {
	  HR_C(encode_prop->put_ctl,(ctl));	/* give ctl to encode_prop			*/
	  HR_C(encode_prop->Release());		/* release access to encode_prop	*/
	  encode_prop = NULL;
	}

	HR_C(filter->QueryInterface,(		/* see if filter does push_prop		*/
      dtmj_push_prop_i_guid, 
      (void **)&push_prop));
	if (push_prop != NULL)				/* filter does push_prop?			*/
    {
	  HR_C(push_prop->put_ctl,(ctl));	/* give ctl to push_prop			*/
	  HR_C(push_prop->Release());		/* release access to push_prop		*/
	  push_prop = NULL;
	}

	HR_C(filter->Release());			/* release access to filter 		*/
	filter = NULL;
  }
  
  HR_C(filters->Release());				/* release access to filter list 	*/
  filters = NULL;
										/* ................................ */
  
  HR_C(control->Run,());				/* tell filter graph to play		*/
  state = STATE_RUNNING;

  while(1)
  {
	DWORD result;
	result = MsgWaitForMultipleObjects(	/* check for filter event			*/
      1, &ehandle,
      FALSE, 1000, QS_ALLINPUT);

	if (result != WAIT_TIMEOUT)			/* no filter events? 				*/
	{
	  HR_C(event->GetEvent,(
        &code, &param1, &param2, 0));

	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "got filter graph event 0x%08lx",
        code);

	  if (code == EC_COMPLETE || 
		  code == EC_USERABORT ||
		  code == EC_ERRORABORT)
	    break;
	}
	else
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC,
        "tick");
  }

cleanup:
										/* ................................ */
										/* OnMediaStop();					*/
  if (control != NULL &&
	  state == STATE_RUNNING)
  {
    HR_I(control->Stop,());
	state = STATE_STOPPED;
  }

										/* ................................ */
  if (push_prop != NULL)
	HR_I(push_prop->Release,());
  if (encode_prop != NULL)
	HR_I(encode_prop->Release,());
  if (grabit_prop != NULL)
	HR_I(grabit_prop->Release,());
  if (filter != NULL)
	HR_I(filter->Release,());
  if (filters != NULL)
	HR_I(filters->Release,());
  if (event != NULL)
	HR_I(event->Release,());
  if (control != NULL)
    HR_I(control->Release,());
  if (graph != NULL)
	HR_I(graph->Release,());

  CoUninitialize();
  
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
