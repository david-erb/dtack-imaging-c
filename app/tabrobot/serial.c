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
#include <dtserial.h>
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
	rc = dt_err(ctl, F,
      "CreateFile(\"%s\", ...)"
      DTERRNO_FMT,
      spec,
      DTERRNO_ARG(errno));
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
  void *buf,
  long n)
{
  DT_F("dtserial_write");
  priv_t *priv = (priv_t *)serial->priv;
  BOOL success;
  DWORD got;
  success = WriteFile(priv->handle,		/* write to serial port 			*/
    buf, n, &got, NULL);
  if (!success)
	return dt_err(ctl, F,
      "WriteFile(0x%08lx, buf, %ld)"
      DTERRNO_FMT,
      priv->handle, n,

      DTERRNO_ARG(errno));
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
  DCB dcb;
  BOOL success;

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
	return dt_err(ctl, F,
      "GetCommState(0x%08lx, &dcb)"
      DTERRNO_FMT,
      priv->handle,
      DTERRNO_ARG(errno));

  success = BuildCommDCB(s, &dcb);		/* translate string					*/
  if (!success)
	return dt_err(ctl, F,
      "GetCommState(0x%08lx, &dcb)"
      DTERRNO_FMT,
      priv->handle,
      DTERRNO_ARG(errno));

  success = SetCommState(priv->handle,	/* set new com setup 				*/
    &dcb);
  if (!success)
	return dt_err(ctl, F,
      "SetCommState(0x%08lx, &dcb)"
      DTERRNO_FMT,
      priv->handle,
      DTERRNO_ARG(errno));

  return DT_RC_GOOD;
}

/*..........................................................................*/
#else

typedef struct {
  dtfd_t fd;
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
  int isreadable;
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
  priv->fd.fd = -1;
  
  DT_C(dtos_isreadable,(ctl, spec,		/* see if serial device exists 		*/
    &isreadable));
  if (!isreadable)
  {
	rc = dt_err(ctl, F,
      "\"%s\" does not exist", spec);
	goto cleanup;
  }

  DT_C(dtfd_open,(ctl, &priv->fd,		/* simply open serial device 		*/
    spec, "rw"));
  
  memset(serial, 0, sizeof(*serial));
  serial->priv = priv;
  serial->spec = spec;
  serial->initialized = 1;
  
cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (priv != NULL)
	{
	  if (priv->fd.fd != -1)
	    DT_I(dtfd_close,(ctl, &priv->fd));
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
	if (priv->fd.fd != -1)
	  DT_I(dtfd_close,(ctl, &priv->fd));
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
  void *buf,
  long n)
{
  DT_F("dtserial_write");
  priv_t *priv = (priv_t *)serial->priv;
  DT_Q(dtfd_write_all,(ctl, &priv->fd,	/* write bytes to serial port 		*/
    buf, n));
  DT_Q(dtfd_write_all,(ctl, &priv->fd,	/* flush serial port 				*/
    NULL, 0));
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
  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "setting \"%s\" to \"%s\"", 
    serial->spec, string);
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
