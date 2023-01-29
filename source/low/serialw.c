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
  20001019 DE added to the library
 *..........................................................................*/


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

DT_RCSID("source/low $RCSfile: serial.c,v $ $Revision: 1.1 $");
#include <dtack/serial.h>
#include <dtack/errno.h>

#include <dtack/os.h>
#include <dtack/str.h>

/*..........................................................................*/
#ifdef DTCONFIG_IMPLIED_MSW32
#ifndef DTDEPEND
#endif

#include <dtack/msw.h>

typedef struct {
  HANDLE handle;
} priv_t;

dt_rc_e
DTCONFIG_API1
dtserial_open(
  dt_ctl_t *ctl,
  dtserial_t *serial,
  const char *spec)
{
  DT_F("dtserial_open");
  priv_t *priv = NULL;
  dt_rc_e rc;

  if (spec == NULL)						/* caller is giving NULL spec? 		*/
  {
    rc = dt_err(ctl, F,
      "NULL serial spec");
    goto cleanup;
  }

  if (strlen(spec) == 0)				/* caller is giving 0-length spec? 	*/
  {
    rc = dt_err(ctl, F,
      "empty serial spec");
    goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(priv_t),
    F, "priv"));
  memset(priv, 0,						/* clear private structure 			*/
    sizeof(priv_t));
  priv->handle = INVALID_HANDLE_VALUE;
  
  priv->handle = CreateFile(spec,		/* open serial file 				*/
    GENERIC_READ | GENERIC_WRITE,
    0, NULL, OPEN_EXISTING, 0, NULL);
  if (priv->handle ==					/* open failed? 					*/
	  INVALID_HANDLE_VALUE)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	rc = dt_err(ctl, F,
      "CreateFile(\"%s\", ...)"
      DTERRNO_LASTFS,
      spec,
      DTERRNO_LASTES(e, s));
	goto cleanup;
  }

  memset(serial, 0, sizeof(*serial));	/* fill in caller's structure 		*/
  serial->priv = priv;
  serial->spec = spec;
  serial->initialized = 1;
  
cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (priv != NULL)
	{
	  if (priv->handle !=
		  INVALID_HANDLE_VALUE &&
		  priv->handle != (HANDLE)0)
	    CloseHandle(priv->handle);
	  DT_I(dtos_free2,(ctl, priv,
        F, "priv"));
	}
	memset(serial, 0, sizeof(*serial));
  }
  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtserial_close(
  dt_ctl_t *ctl,
  dtserial_t *serial)
{
  DT_F("dtserial_open");
  priv_t *priv = (priv_t *)serial->priv;
  dt_rc_e rc = DT_RC_GOOD;
  if (priv != NULL)
  {
	if (priv->handle !=
		INVALID_HANDLE_VALUE &&
		priv->handle != (HANDLE)0)
	  CloseHandle(priv->handle);
	DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
	memset(serial, 0, sizeof(*serial));
  }
  return rc;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtserial_write(
  dt_ctl_t *ctl,
  dtserial_t *serial,
  const void *buf,
  long n)
{
  DT_F("dtserial_write");
  priv_t *priv = (priv_t *)serial->priv;
  BOOL success;
  DWORD got;
  success = WriteFile(priv->handle,		/* write to serial port 			*/
    buf, n, &got, NULL);
  if (!success)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	return dt_err(ctl, F,
      "WriteFile(0x%08lx, buf, %ld)"
      DTERRNO_LASTFS,
      priv->handle, n,
      DTERRNO_LASTES(e, s));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtserial_read(
  dt_ctl_t *ctl,
  dtserial_t *serial,
  void *buf,
  long n,
  long *got)
{
  DT_F("dtserial_read");
  priv_t *priv = (priv_t *)serial->priv;
  BOOL success;
  success = ReadFile(priv->handle,		/* read from serial port 			*/
    buf, n, got, NULL);
  if (!success)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	return dt_err(ctl, F,
      "ReadFile(0x%08lx, buf, %ld)"
      DTERRNO_LASTFS,
      priv->handle, n,
      DTERRNO_LASTES(e, s));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtserial_setup(
  dt_ctl_t *ctl,
  dtserial_t *serial,
  const char *string)
{
  DT_F("dtserial_setup");
  priv_t *priv = (priv_t *)serial->priv;
  char s[64];
  int i, l;
  COMMTIMEOUTS timeouts;
  DCB dcb;
  BOOL success;

#if 0
  timeouts.ReadIntervalTimeout = 100;
  timeouts.ReadTotalTimeoutMultiplier = 100;
  timeouts.ReadTotalTimeoutConstant = 100;
  timeouts.WriteTotalTimeoutMultiplier = 100;
  timeouts.WriteTotalTimeoutConstant = 100;
  
  success = SetCommTimeouts(			/* get current com setup 			*/
    priv->handle, &timeouts);

  if (!success)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	return dt_err(ctl, F,
      "SetCommTimeouts(0x%08lx, %ld, %ld, %ld, %ld, %ld)"
      DTERRNO_LASTFS,
      priv->handle,
      timeouts.ReadIntervalTimeout,
      timeouts.ReadTotalTimeoutMultiplier,
      timeouts.ReadTotalTimeoutConstant,
      timeouts.WriteTotalTimeoutMultiplier,
      timeouts.WriteTotalTimeoutConstant,
      DTERRNO_LASTES(e, s));
  }
#endif

  DT_Q(dtstr_printf,(ctl, s, sizeof(s),
    "%s", string));
  l = strlen(s);
  for (i=0; i<l; i++)
	if (s[i] == '-')
	  s[i] = ',';

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "setting \"%s\" to \"%s\"", 
    serial->spec, s);

  success = GetCommState(priv->handle,	/* get current com setup 			*/
    &dcb);
  if (!success)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	return dt_err(ctl, F,
      "GetCommState(0x%08lx, &dcb)"
      DTERRNO_LASTFS,
      priv->handle,
      DTERRNO_LASTES(e, s));
  }

  success = BuildCommDCB(s, &dcb);		/* translate string					*/
  if (!success)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	return dt_err(ctl, F,
      "BuildCommDCB(\"%s\", &dcb)"
      DTERRNO_LASTFS,
      s,
      DTERRNO_LASTES(e, s));
  }
										/* ................................ */

#if 1
										/* ................................ */
  dcb.fDtrControl = 1;
  dcb.fRtsControl = 1;
  success = SetCommState(priv->handle,	/* set new com setup 				*/
    &dcb);
  if (!success)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	return dt_err(ctl, F,
      "SetCommState(0x%08lx, &dcb)"
      DTERRNO_LASTFS,
      priv->handle,
      DTERRNO_LASTES(e, s));
  }
#endif

  success = GetCommState(priv->handle,	/* get current com setup 			*/
    &dcb);
  if (!success)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	return dt_err(ctl, F,
      "GetCommState(0x%08lx, &dcb)"
      DTERRNO_LASTFS,
      priv->handle,
      DTERRNO_LASTES(e, s));
  }

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "DCB.fDtrControl=0x%08lx DTR_CONTROL_ENABLE=0x%08lx", dcb.fDtrControl, DTR_CONTROL_ENABLE); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "DCB.fRtsControl=0x%08lx RTS_CONTROL_ENABLE=0x%08lx", dcb.fRtsControl, RTS_CONTROL_ENABLE); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "DCB.fOutxCtsFlow=0x%08lx", dcb.fOutxCtsFlow); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "DCB.fOutxDsrFlow=0x%08lx", dcb.fOutxDsrFlow); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "DCB.fDsrSensitivity=0x%08lx", dcb.fDsrSensitivity); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "DCB.StopBits=0x%08lx", (long)dcb.StopBits); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "DCB.Parity=0x%08lx", (long)dcb.Parity); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "DCB.ByteSize=0x%08lx", (long)dcb.ByteSize); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "DCB.fAbortOnError=0x%08lx", (long)dcb.fAbortOnError); 


  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtserial_detect(
  dt_ctl_t *ctl,
  dtserial_t *serial,
  char *string,
  long max)
{
  DT_F("dtserial_detect");
  priv_t *priv = (priv_t *)serial->priv;
  char s[64];
  int i, l;
  COMSTAT comstat;
  DWORD errors;
  BOOL success;

  success = ClearCommError(priv->handle,	/* get current com setup 			*/
    &errors, &comstat);
  if (!success)
  {
	char s[256];
	DWORD e = GetLastError();
	DTERRNO_LASTS(e, s, sizeof(s));
	return dt_err(ctl, F,
      "ClearCommError(0x%08lx, &errors, &stat)"
      DTERRNO_LASTFS,
      priv->handle,
      DTERRNO_LASTES(e, s));
  }
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "errors=0x%08lx", (long)errors); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "COMSTAT.fCtsHold=0x%08lx", (long)comstat.fCtsHold ); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "COMSTAT.fDsrHold=0x%08lx", (long)comstat.fDsrHold ); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "COMSTAT.fRlsdHold=0x%08lx", (long)comstat.fRlsdHold ); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "COMSTAT.fXoffHold=0x%08lx", (long)comstat.fXoffHold ); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "COMSTAT.fXoffSent=0x%08lx", (long)comstat.fXoffSent ); 
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "COMSTAT.fEof=0x%08lx", (long)comstat.fEof ); 

  if (string && max)
	*string = '\0';

  return DT_RC_GOOD;
}
#endif




/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
