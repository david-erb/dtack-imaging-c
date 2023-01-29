/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */


// 2005-06-13 changed to use io.dll from http://www.geekhideout.com/iodll.shtml

/*..........................................................................
| TITLE
| The :portio: Function Group
| 
|!Summary
| The :dtportio: functions work together to do external bus I/O port operations.
|
| The :dtportio_t: data structure used by all these functions
| contains no public members.
|
|!Example dtport1.c - Simple I/O Port Usage
|_index param Function Group, :portio: Function Group, example of
|_index dtportio_inb(),, example of
|_index dtportio_init(),, example of
|_index dtportio_uninit(),, example of
|_index dtportio_outb(),, example of
|:// Example Program dtport1
|:// Illustrates simple I/O port usage.
|:// Two required numeric arguments are port and value.
|:// Most error checking left out for clarity.
|:
|:#include <dtack/base.h>
|:#include <dtack/portio.h>
|:
|:static void barf(
|:  const char *message)
|:{
|:  printf("dtport1: %s\n", message);
|:  exit(0);
|:}
|:
|:int
|:main(
|:  int argc,
|:  char *argv[])
|:{
|:  char *e;
|:  int port, value;
|:  dtportio_t portio;
|:  dt_rc_e rc;
|:  int oldval, newval;
|:  
|:  if (argc != 3) barf("wrong argument count");
|:  
|:  port = (int)strtol(argv[1], &e, 0);
|:  if (*e) barf("port not a number");
|:  if (port < 0 || port > 0xfff) barf("port value out of range 0-4095");
|:  
|:  value = (int)strtol(argv[2], &e, 0);
|:  if (*e) barf("value not a number");
|:  if (value < 0 || value > 0xff) barf("value out of range 0-255");
|:
|:  rc = dtportio_init(NULL, &portio, NULL);
|:  if (rc != DT_RC_GOOD) exit(0);
|:  
|:  rc = dtportio_inb(NULL, &portio, port, &oldval);
|:  if (rc != DT_RC_GOOD) exit(0);
|:  
|:  rc = dtportio_outb(NULL, &portio, port, value);
|:  if (rc != DT_RC_GOOD) exit(0);
|:  
|:  rc = dtportio_inb(NULL, &portio, port, &newval);
|:  if (rc != DT_RC_GOOD) exit(0);
|:  
|:  rc = dtportio_uninit(NULL, &portio);
|:  if (rc != DT_RC_GOOD) exit(0);
|:
|:  printf("dtport1: I/O port 0x%03x old=0x%02x value=0x%02x new=0x%02x\n", 
|:    (unsigned int)port, oldval, (int)value, newval);
|:
|:  return 0;
|:}
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: portio.c,v $ $Revision: 1.2 $");
#include <dtack/msw.h>
#include <dtack/portio.h>
#include <dtack/errno.h>
#include <dtack/os.h>

#define DESC "port io"
typedef void (WINAPI *PORTOUT) (short int Port, char Data);
typedef char (WINAPI *PORTIN) (short int Port);

/*..........................................................................
 *..........................................................................*/
#if defined(DTCONFIG_IMPLIED_MSW32)
/*..........................................................................
| NAME
| dtportio_open - init portio operations
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_init(							/* init portio operations			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  const char *spec)						/* bus spec 						*/
{
  DT_F("dtportio_init");
  
  HINSTANCE hinstance;
  PORTIN inb;
  PORTOUT outb;
  dt_rc_e rc = DT_RC_GOOD;

  hinstance = LoadLibrary(
    "dtportioxp.dll");
  if (hinstance == NULL)
  {
	DWORD e = GetLastError();
	rc = dt_err(ctl, F,
      "LoadLibarary(\"dtportioxp.dll\")"
      DTERRNO_FMT,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  inb = (PORTIN)GetProcAddress(hinstance, 
    "PortIn");
  if (inb == NULL)
  {
	DWORD e = GetLastError();
	rc = dt_err(ctl, F,
      "GetProcAddress(0x%08lx, "
      " \"PortIn\")"
      DTERRNO_FMT,
      hinstance,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  outb = (PORTOUT)GetProcAddress(hinstance, 
    "PortOut");
  if (outb == NULL)
  {
	DWORD e = GetLastError();
	rc = dt_err(ctl, F,
      "GetProcAddress(0x%08lx, "
      " \"PortOut\")"
      DTERRNO_FMT,
      hinstance,
      DTERRNO_ARG(e));
	goto cleanup;
  }

  DT_MEMSET(portio, '\0',					/* clear structure memory			*/
    sizeof(*portio));

  portio->handle = (void *)hinstance;
  portio->inb = (dtportio_inb_f *)inb;
  portio->outb = (dtportio_outb_f *)outb;
 
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "loaded dtportioxp.dll at hinstance 0x%08lx",
    hinstance);

cleanup:
  if (rc != DT_RC_GOOD &&
	  hinstance != NULL)
    FreeLibrary(hinstance);

  return rc;
}

/*..........................................................................
| NAME
| dtportio_uninit() - uninit portio operations
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_uninit(						/* close and release				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio)					/* portio object descriptor 		*/
{
  DT_F("dtportio_close");
  dt_rc_e rc = DT_RC_GOOD;

  if (portio->handle != NULL)
  {
	dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "freeing dtportioxp.dll at hinstance 0x%08lx",
      portio->handle);
    FreeLibrary((HINSTANCE)portio->handle);
  }

  DT_MEMSET(portio, '\0', sizeof(*portio));	/* clear structure memory			*/

  return rc;
}

/*..........................................................................
| NAME
| dtportio_inb() - input byte from port
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_inb(							/* input byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to input from 				*/
  int *value)							/* returned byte value 				*/
{
  DT_F("dtportio_inb");
  PORTIN inb;
  if (portio->inb == NULL)
    return dt_err(ctl, F,
      "inb operation not initialized");

  inb = (PORTIN)portio->inb;
  *value = inb(port);

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtportio_outb() - output byte from port
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_outb(							/* output byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to output to 				*/
  int value)							/* value to output					*/
{
  DT_F("dtportio_outb");
  PORTOUT outb;
  if (portio->outb == NULL)
    return dt_err(ctl, F,
      "outb operation not initialized");

  outb = (PORTOUT)portio->outb;
  outb(port, value);

  return DT_RC_GOOD;
}

#else
#if defined(DTCONFIG_LG1)
/*..........................................................................
 *..........................................................................*/
/*..........................................................................
 *..........................................................................*/

typedef struct {
  dtfd_t fd;
} priv_t;

/*..........................................................................
| NAME
| dtportio_open - init portio operations
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_init(							/* init portio operations			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  const char *spec)						/* bus spec 						*/
{
  DT_F("dtportio_init");
  priv_t *priv = NULL;
  int sysfd;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtos_malloc2,(ctl,
    (void **)&priv, sizeof(*priv),
    F, "priv"));
  DT_MEMSET(priv, '\0',					/* clear structure memory			*/
    sizeof(*priv));
  priv->fd.fd = -1;
  
  DT_MEMSET(portio, '\0',					/* clear structure memory			*/
    sizeof(*portio));

  if (spec == NULL)						/* no spec given? 					*/
	spec = "/dev/port";					/* default spec */

  sysfd = open(spec, O_RDWR);			/* open port device 				*/
  if (sysfd == -1)						/* could not open port device? 		*/
  {
	int e = errno;
	rc = dt_err(ctl, F,
      "open(\"%s\", O_RDWR) failed"
      DTERRNO_FMT, 
      spec, DTERRNO_ARG(e));
	goto cleanup;
  }

  DT_C(dtfd_fd,(ctl, &priv->fd,			/* wrap fd structure around fd 		*/
    spec, sysfd));

  portio->handle = (void *)priv;
 
  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
    "initialized I/O bus \"%s\"", spec);

cleanup:
  if (rc != DT_RC_GOOD &&
	  priv != NULL)
    DT_I(dtos_free2,(ctl, priv, F, "priv"));

  return rc;
}

/*..........................................................................
| NAME
| dtportio_uninit() - uninit portio operations
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_uninit(						/* close and release				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio)					/* portio object descriptor 		*/
{
  DT_F("dtportio_close");
  priv_t *priv = (priv_t *)portio->handle;
  dt_rc_e rc = DT_RC_GOOD;
  
  if (priv != NULL)
  {
	if (priv->fd.fd != -1)
	  DT_I(dtfd_close,(ctl, &priv->fd));
    DT_I(dtos_free2,(ctl, priv, F, "priv"));
  }

  return rc;
}

/*..........................................................................
| NAME
| dtportio_inb() - input byte from port
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_inb(							/* input byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to input from 				*/
  int *value)							/* returned byte value 				*/
{
  DT_F("dtportio_inb");
  priv_t *priv = (priv_t *)portio->handle;
  unsigned char byte;

  DT_Q(dtfd_seek,(ctl, &priv->fd,		/* seek to port offset 				*/
    port, DTFD_WHENCE_SET, NULL));
  DT_Q(dtfd_read_all,(ctl, &priv->fd,	/* get the byte 					*/
    &byte, 1));
  *value = (int)byte & 0xff;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtportio_outb() - output byte from port
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_outb(							/* output byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to output to 				*/
  int value)							/* value to output					*/
{
  DT_F("dtportio_outb");
  priv_t *priv = (priv_t *)portio->handle;
  unsigned char byte;

  byte = value;
  DT_Q(dtfd_seek,(ctl, &priv->fd,		/* seek to port offset 				*/
    port, DTFD_WHENCE_SET, NULL));
  DT_Q(dtfd_write_all,(ctl, &priv->fd,	/* put the byte 					*/
    &byte, 1));

  return DT_RC_GOOD;
}

#else
/*..........................................................................
 *..........................................................................*/
/*..........................................................................
| NAME
| dtportio_open - init portio operations
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_init(							/* init portio operations			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  const char *spec)						/* bus spec 						*/
{
  DT_F("dtportio_init");
  return dt_err_notlinked(ctl, F, DESC);  
}

/*..........................................................................
| NAME
| dtportio_uninit() - uninit portio operations
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_uninit(						/* close and release				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio)					/* portio object descriptor 		*/
{
  DT_F("dtportio_close");
  return dt_err_notlinked(ctl, F, DESC);  
}

/*..........................................................................
| NAME
| dtportio_inb() - input byte from port
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_inb(							/* input byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to input from 				*/
  int *value)							/* returned byte value 				*/
{
  DT_F("dtportio_inb");
  return dt_err_notlinked(ctl, F, DESC);  
}

/*..........................................................................
| NAME
| dtportio_outb() - output byte from port
| 
| SUMMARY
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtportio_outb(							/* output byte						*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtportio_t *portio,					/* portio object descriptor 		*/
  int port,								/* port to output to 				*/
  int value)							/* value to output					*/
{
  DT_F("dtportio_outb");
  return dt_err_notlinked(ctl, F, DESC);  
}

#endif
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
