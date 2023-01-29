/*..........................................................................
| NAME
| :dtevent: Function Group - operating system event operations
|
| SUMMARY
| The object ~event~ encapsulates the ability
| of a process to set and wait on events.
|
| END
 *..........................................................................*/

/*..........................................................................
  CHANGES
  20000808 DE added to the library
 *..........................................................................*/


#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: event.c,v $ $Revision: 1.8 $");
#include <dtack/event.h>
#include <dtack/str.h>
#include <dtack/errno.h>

#if defined(DTCONFIG_IMPLIED_MSW32)
#include <dtack/msw.h>

/*..........................................................................
| NAME
| dtevent_create() - create event object
|
| SUMMARY
| Create the event object ~event~.
| If ~id~ is 0, the event will be private.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtevent_create(							/* create new event					*/
  dt_ctl_t *ctl,						/* environment control				*/
  dtevent_t *event,						/* returned event structure 		*/
  long key)								/* system-wide event id 			*/
{
  DT_F("dtevent_create");
  const char *name;
  char name2[32];
  HANDLE handle;
  dt_rc_e rc = DT_RC_GOOD;

  if (key == 0)							/* unnamed event? 					*/
	name = NULL;
  else									/* named event? 					*/
  {
	DT_C(dtstr_printf,(ctl,				/* win32 wants a string 			*/
      name2, sizeof(name2),
      "dtevent-%08lx", key));
	name = name2;
  }

  memset(event, 0, sizeof(*event));		/* clear event structure 			*/

  handle = CreateEvent(
    NULL,								/* security attributes 				*/
	TRUE,								/* manual reset						*/
	FALSE,								/* initial state					*/
    name);								/* event's name 					*/
  if (!handle)							/* system call failed? 				*/
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "CreateEvent(NULL, "
      "FALSE, FALSE, \"%s\")" 
      DTERRNO_LASTFS,
      name? name: "NULL",
      DTERRNO_LASTES(e, s));
	goto cleanup;
  }

  event->key = key;
  event->sid = (int)handle;
  event->created = 1;

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "created 0x%08lx 0x%08lx 0x%08lx", 
    event, event->key, handle);

  event->initialized = 1;

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(dtevent_destroy,(ctl, event));

  return rc;
}

/*..........................................................................
| NAME
| dtevent_destroy() - destroy event object
|
| SUMMARY
| Free all resources associated with ~event~ object.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtevent_destroy(						/* release event object 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event)						/* event object 					*/
{
  DT_F("dtevent_destroy");
  HANDLE handle = (void *)event->sid;
  dt_rc_e rc = DT_RC_GOOD;
  if (event->created)					/* event object got created? 		*/
  {
	BOOL sysrc;
	sysrc = CloseHandle(handle);
	if (!sysrc)
	{
	  char s[256];
	  DWORD e = GetLastError();
	  DTERRNO_LASTS(e, s, sizeof(s));
	  rc = dt_err(ctl, F,
        "CloseHandle(0x%08lx) on key 0x%08lx" 
        DTERRNO_LASTFS,
        handle, event->key,
       DTERRNO_LASTES(e, s));
	}

    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "destroyed 0x%08lx 0x%08lx 0x%08lx", 
      event, event->key, handle);
  }
  memset(event, 0, sizeof(*event));
  
  return rc;
}

/*..........................................................................
| NAME
| dtevent_access() - access event object
|
| SUMMARY
| Access the event object ~event~.
| If ~id~ is 0, the event will be private.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtevent_access(							/* access new event					*/
  dt_ctl_t *ctl,						/* environment control				*/
  dtevent_t *event,						/* returned event structure 		*/
  long key)								/* system-wide event id 			*/
{
  DT_F("dtevent_access");
  const char *name;
  char name2[32];
  HANDLE handle;
  dt_rc_e rc = DT_RC_GOOD;

  if (key == 0)							/* unnamed event? 					*/
	name = NULL;
  else									/* named event? 					*/
  {
	DT_C(dtstr_printf,(ctl,				/* win32 wants a string 			*/
      name2, sizeof(name2),
      "dtevent-%08lx", key));
	name = name2;
  }

  memset(event, 0, sizeof(*event));		/* clear event structure 			*/

  handle = OpenEvent(
    EVENT_ALL_ACCESS,					/* accessed							*/
	TRUE,								/* inheritance						*/
    name);								/* event's name 					*/
  if (!handle)							/* system call failed? 				*/
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "OpenEvent(NULL, "
      "EVENT_ALL_ACCESS, TRUE, \"%s\")" 
      DTERRNO_LASTFS,
      name? name: "NULL",
      DTERRNO_LASTES(e, s));
	goto cleanup;
  }

  event->key = key;
  event->sid = (int)handle;
  event->created = 1;

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "accessed 0x%08lx 0x%08lx 0x%08lx", 
    event, event->key, handle);

  event->initialized = 1;

cleanup:
  if (rc != DT_RC_GOOD)
	DT_I(dtevent_destroy,(ctl, event));

  return rc;
}

/*..........................................................................
| NAME
| dtevent_release() - release event object
|
| SUMMARY
| Free all resources associated with ~event~ object.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtevent_release(						/* release event object 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event)						/* event object 					*/
{
  DT_F("dtevent_release");
  HANDLE handle = (void *)event->sid;
  dt_rc_e rc = DT_RC_GOOD;
  if (event->created)					/* event object got created? 		*/
  {
	BOOL sysrc;
	sysrc = CloseHandle(handle);
	if (!sysrc)
	{
	  char s[256];
	  DWORD e = GetLastError();
	  DTERRNO_LASTS(e, s, sizeof(s));
	  rc = dt_err(ctl, F,
        "CloseHandle(0x%08lx) on key 0x%08lx" 
        DTERRNO_LASTFS,
        handle, event->key,
       DTERRNO_LASTES(e, s));
	}
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "released 0x%08lx 0x%08lx 0x%08lx", 
      event, event->key, handle);
  }
  memset(event, 0, sizeof(*event));
  
  return rc;
}

/*..........................................................................
| NAME
| dtevent_set() - set event object
|
| SUMMARY
| Set event object.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtevent_set(							/* set event object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event)						/* event object 					*/
{
  DT_F("dtevent_set");
  HANDLE handle = (void *)event->sid;
  BOOL sysrc;
  dt_rc_e rc = DT_RC_GOOD;

  if (!event->initialized)				/* event object got created? 		*/
	return dt_err(ctl, F,
      "uninitialized structure");
  
  sysrc = SetEvent(handle);				/* set event state					*/
  if (!sysrc)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "SetEvent(0x%08lx) on key 0x%08lX" 
      DTERRNO_LASTFS,
      handle, event->key,
      DTERRNO_LASTES(e, s));
  }

  if (DT_DBG(ctl, DT_DBG_MASK_IPC) &&
	  DT_DBG(ctl, DT_DBG_MASK_PIXEL))
  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "set 0x%08lx 0x%08lx 0x%08lx", 
    event, event->key, handle);

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtevent_reset() - reset event object
|
| SUMMARY
| Reset event object.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtevent_reset(							/* reset event object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event)						/* event object 					*/
{
  DT_F("dtevent_reset");
  HANDLE handle = (void *)event->sid;
  BOOL sysrc;
  dt_rc_e rc = DT_RC_GOOD;

  if (!event->initialized)				/* event object got created? 		*/
	return dt_err(ctl, F,
      "uninitialized structure");
  
  sysrc = ResetEvent(handle);				/* reset event state					*/
  if (!sysrc)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "ResetEvent(0x%08lx) on key 0x%08lX" 
      DTERRNO_LASTFS,
      handle, event->key,
      DTERRNO_LASTES(e, s));
  }

  if (DT_DBG(ctl, DT_DBG_MASK_IPC) &&
	  DT_DBG(ctl, DT_DBG_MASK_PIXEL))
  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "reset 0x%08lx 0x%08lx 0x%08lx", 
    event, event->key, handle);

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtevent_pulse() - pulse event object
|
| SUMMARY
| Pulse event object.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtevent_pulse(							/* pulse event object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event)						/* event object 					*/
{
  DT_F("dtevent_pulse");
  HANDLE handle = (void *)event->sid;
  BOOL sysrc;
  dt_rc_e rc = DT_RC_GOOD;

  if (!event->initialized)				/* event object got created? 		*/
	return dt_err(ctl, F,
      "uninitialized structure");
  
  sysrc = PulseEvent(handle);			/* pulse event state					*/
  if (!sysrc)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "PulseEvent(0x%08lx) on key 0x%08lX" 
      DTERRNO_LASTFS,
      handle, event->key,
      DTERRNO_LASTES(e, s));
  }

  if (DT_DBG(ctl, DT_DBG_MASK_IPC) &&
	  DT_DBG(ctl, DT_DBG_MASK_PIXEL))
  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "pulse 0x%08lx 0x%08lx 0x%08lx", 
    event, event->key, handle);

cleanup:
  return rc;
}

/*..........................................................................
| NAME
| dtevent_wait() - wait on event object
|
| SUMMARY
| Wait until event becomes set.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtevent_wait(							/* wait event object 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtevent_t *event,						/* event object 					*/
  long timeout,							/* maximum wait in milliseconds 	*/
  int *set)								/* return true if set 				*/
{
  DT_F("dtevent_wait");
  HANDLE handle = (void *)event->sid;
  DWORD sysrc;
  int e;
  dt_rc_e rc = DT_RC_GOOD;

  if (!event->initialized)				/* event object got created? 		*/
	return dt_err(ctl, F,
      "uninitialized structure");
  
  sysrc = WaitForSingleObject(handle, 0);
  if (sysrc != WAIT_FAILED &&
	  sysrc != WAIT_TIMEOUT &&
	  sysrc != WAIT_ABANDONED)
  {
    if (DT_DBG(ctl, DT_DBG_MASK_IPC))
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "immediately got 0x%08lx 0x%08lx:0x%08lx", 
      event, event->key, handle);
	*set = 1;
	goto cleanup;
  }

  if (timeout > 0)
  {
	if (DT_DBG(ctl, DT_DBG_MASK_IPC) &&
	    DT_DBG(ctl, DT_DBG_MASK_PIXEL))
	  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
		"waiting for 0x%08lx 0x%08lx:0x%08lx", 
		event, event->key, handle);

	sysrc = WaitForSingleObject(handle, timeout);
  }

  if (sysrc == WAIT_TIMEOUT)
  {
    if (DT_DBG(ctl, DT_DBG_MASK_IPC))
    dt_dbg(ctl, F, DT_DBG_MASK_IPC,
      "WaitForSingleObject(0x%08lx, %d) timeout on 0x%08lx 0x%08lx:0x%08lx",
      handle, timeout, event, event->key, handle);
	*set = 0;
	goto cleanup;
  }

  if (sysrc == WAIT_ABANDONED)
  {
    DT_CLEANUP((ctl, F,
      "WaitForSingleObject(0x%08lx, %d) abandoned on 0x%08lx 0x%08lx:0x%08lx", 
      handle, timeout, event, event->key, handle));
  }

  if (sysrc == WAIT_FAILED)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	DT_CLEANUP((ctl, F,
      "WaitForSingleObject(0x%08lx, %d) on 0x%08lx 0x%08lx:0x%08lx)" 
      DTERRNO_LASTFS,
      handle, timeout, event, event->key, handle,
      DTERRNO_LASTES(e, s)));
  }

  if (DT_DBG(ctl, DT_DBG_MASK_IPC) &&
	  DT_DBG(ctl, DT_DBG_MASK_PIXEL))
  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "got 0x%08lx 0x%08lx:0x%08lx", 
    event, event->key, handle);

  *set = 1;
  
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtevent_rev(						/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtevent lib";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 3;
  if (rev_date != NULL)
    *rev_date = 20011102;
  return DT_RC_GOOD;
}


#endif 									/* end if WIN32 					*/
