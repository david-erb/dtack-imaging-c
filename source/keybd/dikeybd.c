/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


// ------------------------------------------------------------------
// CHANGES
// 2004-10-15 DE 02-02 added wait
// ------------------------------------------------------------------

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/



#include <dtack/base.h>
#include <dtack/os.h>
#include <dtack/str.h>

DT_RCSID("pxgrab $RCSfile: init.c,v $ $Revision: 1.8 $");
#include <dtack/errno.h>
#include <dtack/msw.h>
#include <dtack/dikeybd.h>

#define DIRECTINPUT_VERSION 0x0800		/* which version to emulate          */
#include <dinput.h>

#define INITIALIZED 0xDEAFBABEL

#define DIOK(FUNC, ARGS)				/* check rc and goto cleanup		*/\
{ 																			  \
  HRESULT hr;                                                                 \
  hr = FUNC ARGS;				                                              \
  if (hr != DI_OK)			 		                                          \
  {                                                                           \
	char s[4096];															  \
    dtdikeybd_translate_error(ctl, dikeybd, hr, s, sizeof(s));			      \
	DT_CLEANUP((ctl, F, "%s%s: %s", #FUNC, #ARGS, s));						  \
    goto cleanup; 															  \
  }                                                                           \
  else 																		  \
  if (trace)																  \
  { 																		  \
	char s[4096];															  \
	dt_dbg(ctl, F, DT_DBG_MASK_TRACE,										  \
      "%s ok", #FUNC, #ARGS);												  \
  } 																		  \
}


/*..........................................................................
 *..........................................................................*/

#define MKEYBD (8)

typedef struct {
  LPDIRECTINPUT8 di;
  LPDIRECTINPUTDEVICE8 keybd[MKEYBD];
  int acquired[MKEYBD];
  HANDLE event;
  int mkeybd;
  int nkeybd;
  int waitable;
  char device_name[MKEYBD][1024];
  char event_name[MAX_PATH];
  dt_ctl_t *ctl;
} priv_t;
 
/*..........................................................................
 *..........................................................................*/

static
BOOL
CALLBACK
dtdikeybd_enumerate(
  LPCDIDEVICEINSTANCE device,
  void *opaque)
{
  DT_F("dtdikeybd_enumerate");
  dtdikeybd_t *dikeybd = (dtdikeybd_t *)opaque;
  priv_t *priv = (priv_t *)dikeybd->priv;
  dt_ctl_t *ctl = priv->ctl;
  const char *type;
  LPDIRECTINPUTDEVICE8 keybd;
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;
  int trace = 1;
  dt_rc_e rc = DT_RC_GOOD;

  switch(device->dwDevType & 0xff)
  {
  case DI8DEVTYPE_MOUSE:
	type = "mouse";
	break;
  case DI8DEVTYPE_KEYBOARD:
	type = "keyboard";
	if (priv->nkeybd == priv->mkeybd)
	  DT_CLEANUP((ctl, F, "too many keyboards"))

										/* save name of device for messages */
	DT_C(dtstr_printf,(ctl,
	  priv->device_name[priv->nkeybd],
	  sizeof(priv->device_name[priv->nkeybd]),
	  "%s/%s",
	  device->tszInstanceName,
	  device->tszProductName));

	DIOK(priv->di->CreateDevice,(
	  device->guidInstance,
	  &keybd,
	  NULL));
	priv->keybd[priv->nkeybd++] = keybd;

    DIOK(keybd->SetDataFormat,(&c_dfDIKeyboard));
	  DIOK(keybd->SetCooperativeLevel,(
		(HWND)msw->pidwin,
		DISCL_NONEXCLUSIVE | DISCL_BACKGROUND));

    DIOK(keybd->Acquire,());
	priv->acquired[priv->nkeybd-1] = 1;
 	break;
  case DI8DEVTYPE_JOYSTICK:
	type = "joystick";
	break;
  case DI8DEVTYPE_DEVICE:
	type = "other";
	break;
  default:
	type = "unknown";
  }

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
	"%s: %s (%s)",
	type,
	device->tszInstanceName,
    device->tszProductName);

cleanup:
	if (rc == DT_RC_GOOD)
	  return TRUE;
	else
	  return FALSE;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdikeybd_scanq(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd,
  char *key)							/* key which is down or 0 if none */
{
  DT_F("dtdikeybd_scanq");
  priv_t *priv;
  int i, n;
  int trace = 0;
  dt_rc_e rc = DT_RC_GOOD;

  if (!dikeybd ||
	  dikeybd->initialized != INITIALIZED)
	DT_CLEANUP((ctl, F, "dikeybd not initialized"));

  priv = (priv_t *)dikeybd->priv;

										/* not waitable? */
  if (!priv->waitable)
	DT_CLEANUP((ctl, F, 
	  "dtikeybd_waitable() has not been called"));

										/* scan each keyboard */
  for (i=0; i<priv->nkeybd; i++)
  {
	DIDEVICEOBJECTDATA data;
	unsigned long n = 1;
										/* fetch single event from a single keyboard */
	DIOK(priv->keybd[i]->GetDeviceData,(
	  sizeof(data), &data, &n, 0));

	if (n)								/* event happened on this keyboard? */
	{
#if 0
	  dt_dbg(ctl, F, DT_DBG_MASK_ASYNC, 
		"%s event offset=0x%08lx data=0x%08lx time=0x%08lx seq=0x%08lx",
		priv->device_name[i],
		data.dwOfs,
		data.dwData,
		data.dwTimeStamp,
		data.dwSequence);
#endif
	  if (data.dwData)					/* event is a press event? */
	  {
		*key = data.dwOfs;
		goto cleanup;
	  }
	}
  }
										/* apparently no key down */
  *key = 0;
cleanup:
  return rc;
}

/*..........................................................................
 * watchfrog #2
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdikeybd_wait(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd,
  long timeout,
  char *key)							/* key which is down or 0 if timeout */
{
  DT_F("dtdikeybd_wait");
  priv_t *priv;
  long waitrc;
  int i, j, n;
  int nevents;
  int trace = 0;
  dt_rc_e rc = DT_RC_GOOD;

  if (!dikeybd ||
	  dikeybd->initialized != INITIALIZED)
	DT_CLEANUP((ctl, F, "dikeybd not initialized"));

  priv = (priv_t *)dikeybd->priv;

										/* not waitable? */
  if (!priv->waitable)
	DT_CLEANUP((ctl, F, 
	  "dtikeybd_waitable() has not been called"));


  while (1)
  {
										/* check for keys in queue */
	DT_C(dtdikeybd_scanq,(ctl, 
	  dikeybd, key));

	if (*key)							/* got a key? */
	  break;
										/* wait for event until timeout */
	waitrc = WaitForSingleObject(
	  priv->event, timeout);

	if (waitrc == WAIT_ABANDONED)
	{
	  DT_CLEANUP((ctl, F, "WAIT_ABANDONED"));
	}
	else
	  if (waitrc == WAIT_TIMEOUT)			/* simply timed out?   */
	  {
		goto cleanup;						/* return no change to caller */
	  }
	  else
	  if (waitrc == WAIT_FAILED)
	  {	
		char s[1024];
		DWORD e = GetLastError();
		DTERRNO_LASTS(e, s, sizeof(s));
		rc = dt_err(ctl, F,
		  "WaitForSingleObject(\"%s\", %d)"
		  DTERRNO_LASTFS,
		  priv->event_name, timeout,
		  DTERRNO_LASTES(e, s));
		goto cleanup;
	  }
	  else
	  if (waitrc != WAIT_OBJECT_0)
	  {
		DT_CLEANUP((ctl, F, "unexpected wait return code %d (0x%08lx)", waitrc, waitrc));
	  }
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdikeybd_waitable(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd)
{
  DT_F("dtdikeybd_waitable");
  priv_t *priv;
  int pid;
  int i;
  char keys[256];
  int trace = 0;
  dt_rc_e rc = DT_RC_GOOD;

  if (!dikeybd ||
	  dikeybd->initialized != INITIALIZED)
	DT_CLEANUP((ctl, F, "dikeybd not initialized"));

  priv = (priv_t *)dikeybd->priv;

										/* already waitable? */
  if (priv->waitable)
	goto cleanup;

  dtos_getpid(ctl, &pid);

  DT_C(dtstr_printf,(ctl,
	priv->event_name,
	sizeof(priv->event_name),
	"dtdikeybd_%08x_%08x", pid,
	GetTickCount()));

  priv->event = CreateEvent(
	0,									/* security */
	0,									/* not manual reset */
	FALSE,								/* initially non-signalled */
	priv->event_name);

  if (priv->event == NULL)
  {
	char s[1024];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "CreateEvent(0, 0, FALSE, \"%s\")"
      DTERRNO_LASTFS,
      priv->event_name,
      DTERRNO_LASTES(e, s));
	goto cleanup;
  }
  else
  if ((long)priv->event == ERROR_ALREADY_EXISTS)
  {
	DT_CLEANUP((ctl, F, "event \"%s\" already exists",
	  priv->event_name));
  }
  
  for (i=0; i<priv->nkeybd; i++)
  {
	DIPROPDWORD dipdw; 
	dipdw.diph.dwSize = sizeof(DIPROPDWORD); 
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER); 
	dipdw.diph.dwObj = 0; 
	dipdw.diph.dwHow = DIPH_DEVICE; 
	dipdw.dwData = 100; 

	if (priv->acquired[i])
	  DIOK(priv->keybd[i]->Unacquire,());

	DIOK(priv->keybd[i]->SetProperty,(DIPROP_BUFFERSIZE, &dipdw.diph));
	DIOK(priv->keybd[i]->SetEventNotification,(priv->event));

	DIOK(priv->keybd[i]->Acquire,());
  }

  priv->waitable = 1;
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdikeybd_isdown(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd,
  int vkey,
  int *isdown)
{
  DT_F("dtdikeybd_isdown");
  priv_t *priv;
  int i;
  char keys[256];
  int trace = 0;
  dt_rc_e rc = DT_RC_GOOD;

  if (!dikeybd ||
	  dikeybd->initialized != INITIALIZED)
	DT_CLEANUP((ctl, F, "dikeybd not initialized"));

  priv = (priv_t *)dikeybd->priv;
  
  *isdown = 0;
  for (i=0; i<priv->nkeybd; i++)
  {
	DIOK(priv->keybd[i]->GetDeviceState,(sizeof(keys), keys));
	*isdown |= !!(keys[vkey] & 0x80);
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdikeybd_dbgdown(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd,
  const char *f)
{
  DT_F("dtdikeybd_dbgdown");
  priv_t *priv;
  int i;
  char keys[256];
  int trace = 0;
  dt_rc_e rc = DT_RC_GOOD;

  if (!dikeybd ||
	  dikeybd->initialized != INITIALIZED)
	DT_CLEANUP((ctl, F, "dikeybd not initialized"));

  priv = (priv_t *)dikeybd->priv;
  
  for (i=0; i<priv->nkeybd; i++)
  {
	int vkey;
	DIOK(priv->keybd[i]->GetDeviceState,(sizeof(keys), keys));
	for (vkey=0; vkey<256; vkey++)
	  if (keys[vkey])
		dt_dbg(ctl, f, DT_DBG_MASK_ANY, "keybd %d: 0x%02x is down", i, vkey);
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdikeybd_uninit(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd)
{
  DT_F("dtdikeybd_uninit");
  int i;
  int trace = 1;
  dt_rc_e rc = DT_RC_GOOD;

  if (dikeybd &&
	  dikeybd->initialized == INITIALIZED)
  {
	if (dikeybd->priv)
	{
	  priv_t *priv = (priv_t *)dikeybd->priv;

	  for (i=0; i<priv->nkeybd; i++)
	  {
		if (priv->keybd[i])
		{
		  if (priv->acquired[i])
			priv->keybd[i]->Unacquire();
		  priv->keybd[i]->Release();
		}
	  }

	  if (priv->event)			
		CloseHandle(priv->event)		/* don't do this until AFTER device releases */;

	  if (priv->di)
		priv->di->Release();
	  DT_I(dtos_free2,(ctl, priv,
		F, "priv"));
	}
	dikeybd->initialized = 0;
  }

  //  memset(dikeybd, 0, sizeof(*dikeybd));

  return rc;
}


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdikeybd_init(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd)
{
  DT_F("dtdikeybd_init");
  priv_t *priv;
  dtmsw_t *msw = (dtmsw_t *)ctl->msw;
  int trace = 1;
  dt_rc_e rc = DT_RC_GOOD;

  memset(dikeybd, 0, sizeof(*dikeybd));
  
  dikeybd->initialized = INITIALIZED;	/* we could uninitialize here		*/
  
  DT_C(dtos_malloc2,(ctl,
	(void **)&priv, sizeof(*priv),
    F, "priv"));

  memset(priv, 0, sizeof(*priv));

  dikeybd->priv = (void *)priv;
  priv->ctl = ctl;
  priv->mkeybd = MKEYBD;

  DIOK(DirectInput8Create,(
	(HINSTANCE)msw->hinstance,
	DIRECTINPUT_VERSION,
	IID_IDirectInput8,
	(void **)&priv->di,
	NULL));

  DIOK(priv->di->EnumDevices,(
	0,
	dtdikeybd_enumerate,
	(void *)dikeybd,
	DIEDFL_ATTACHEDONLY));

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(dtdikeybd_uninit,(ctl, dikeybd))
  return rc;
}

/*..........................................................................
 *..........................................................................*/

void
DTCONFIG_API1
dtdikeybd_translate_error(
  dt_ctl_t *ctl,
  dtdikeybd_t *dikeybd,
  long hr,
  char *message,
  int max)
{
  DT_F("dtdikeybd_translate_error");
  const char *e = NULL;

  switch(hr)
  {
  case DIERR_OLDDIRECTINPUTVERSION:
	e = "The application requires a newer version of DirectInput.";
	break;
  
  case DIERR_BETADIRECTINPUTVERSION:
	e = "The application was written for an unsupported prerelease version of DirectInput.";
	break;

   
  case DIERR_BADDRIVERVER:
	e = "The object could not be created due to an incompatible driver version or mismatched or incomplete driver components.";
	break;

  
  case DIERR_DEVICENOTREG:
	e = "The device or device instance or effect is not registered with DirectInput.";
	break;
  
  case DIERR_NOTFOUND:
	e = "The requested object does not exist.";
	break;

  
  case DIERR_INVALIDPARAM:
	e = "An invalid parameter was passed to the returning function, or the object was not in a state that admitted the function to be called.";
	break;
  
  case DIERR_NOINTERFACE:
	e = "The specified interface is not supported by the object";
	break;
  
  case DIERR_GENERIC:
	e = "An undetermined error occured inside the DInput subsystem";
	break;
  
  case DIERR_OUTOFMEMORY:
	e = "The DInput subsystem couldn't allocate sufficient memory to complete the caller's request.";
	break;
  
  case DIERR_UNSUPPORTED:
	e = "The function called is not supported at this time";
	break;
  
  case DIERR_NOTINITIALIZED:
	e = "This object has not been initialized";
	break;

  
  case DIERR_ALREADYINITIALIZED:
	e = "This object is already initialized";
	break;

  
  case DIERR_NOAGGREGATION:
	e = "This object does not support aggregation";
	break;
  
  case DIERR_OTHERAPPHASPRIO:
	e = "Another app has a higher priority level, preventing this call from succeeding.";
	break;
  
  case DIERR_INPUTLOST:
	e = "Access to the device has been lost.  It must be re-acquired.";
	break;

  
  case DIERR_ACQUIRED:
	e = "The operation cannot be performed while the device is acquired.";
	break;

  
  case DIERR_NOTACQUIRED:
	e = "The operation cannot be performed unless the device is acquired.";
	break;


  case E_PENDING:
	e = "Data is not yet available.";
	break;

  
  case DIERR_INSUFFICIENTPRIVS:
	e = "Unable to IDirectInputJoyConfig_Acquire because the user does not have sufficient privileges to change the joystick configuration.";
	break;
  
  case DIERR_DEVICEFULL:
	e = "The device is full.";
	break;
  
  case DIERR_MOREDATA:
	e = "Not all the requested information fit into the buffer.";
	break;
  
  case DIERR_NOTDOWNLOADED:
	e = "The effect is not downloaded.";
	break;
  
  case DIERR_HASEFFECTS:
	e = "The device cannot be reinitialized because there are still effects attached to it.";
	break;
  
  case DIERR_NOTEXCLUSIVEACQUIRED:
	e = "The operation cannot be performed unless the device is acquired in DISCL_EXCLUSIVE mode.";
	break;
  
  case DIERR_INCOMPLETEEFFECT:
	e = "The effect could not be downloaded because essential information is missing.  For example, no axes have been associated with the effect, or no type-specific information has been created.";
	break;
  
  case DIERR_NOTBUFFERED:
	e = "Attempted to read buffered device data from a device that is not buffered.";
	break;
  
  case DIERR_EFFECTPLAYING:
	e = "An attempt was made to modify parameters of an effect while it is playing.  Not all hardware devices support altering the parameters of an effect while it is playing.";
	break;
  
  case DIERR_UNPLUGGED:
	e = "The operation could not be completed because the device is not plugged in.";
	break;

  default:
	dtstr_printf(ctl, message, max, "unknown directinput error 0x%08lx", hr);
  }

  if (e)
	dtstr_printf(ctl, message, max, "%s", e);
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtdikeybd_rev(								/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "directx keybd lib";
  if (rev_major != NULL)
    *rev_major = 2;
  if (rev_minor != NULL)
    *rev_minor = 2;
  if (rev_date != NULL)
    *rev_date = 20041015;
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
