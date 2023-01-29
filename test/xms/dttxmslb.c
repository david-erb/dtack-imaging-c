/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/*
 */

#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include <xmslib.h>
  
#ifdef __WATCOMC__
#   pragma aux 						  	/* specify Microsoft-callable funcs	*/\
      borland \
      "_*" \
      parm loadds caller [] \
      value struct float struct routine [ax] \
      modify [ax bx cx dx si di es];

#  pragma aux (borland) XMS_Setup;
#  pragma aux (borland) XMS_FreeMem;
#  pragma aux (borland) XMS_Version;
#  pragma aux (borland) XMS_AllocEMB;
#  pragma aux (borland) XMS_FreeEMB;
#  pragma aux (borland) XMS_LockEMB;
#  pragma aux (borland) XMS_UnlockEMB;
#  pragma aux (borland) XMS_MoveEMB;
#  pragma aux (borland) XMS_ReallocEMB;
#endif

#define DT_F(NAME) const char *F = NAME
#define DT_RC_GOOD 0
#define DT_RC_BAD  1
#define dt_rc_e int

/*..........................................................................
 *..........................................................................*/

void
dttxms_debug(
  const char *caller,
  const char *format,
  ...)
{
  va_list arg;
  printf("%s: ", caller);
  va_start(arg, format);
  vprintf(format, arg);
  printf("\n");
  va_end(arg);
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e dttxms_err(
  const char *f,
  const char *xmmfuncname,
  unsigned char error)
{
  dttxms_debug(f,
    "xmslb error 0x%02x in %s",
    error, xmmfuncname);

  return DT_RC_BAD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttxms_libinit(	
  void)
{
  DT_F("dttxms_libinit");
  unsigned int rc;
  unsigned int freemem;
  unsigned int totmem;
  unsigned int protocol;
  unsigned int internal;
  unsigned int hms;
  unsigned char error;

  rc = XMS_Setup();						/* init XMM library				    */
  if (rc == 0)
    return dttxms_err(F,
      "XMS_Setup", rc);

  error = XMS_FreeMem(&freemem, &totmem);
  if (error != 0)
    return dttxms_err(F,
      "XMS_FreeMem", error);

  error = XMS_Version(&protocol,
    &internal, &hms);
  if (error != 0)
    return dttxms_err(F,
      "XMS_Version", error);

  dttxms_debug(F,
    "XMS v%04x i%04x has %uK/%uK bytes available",
    protocol, internal, freemem, totmem);
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttxms_alloc(	
  XMShandle *handle,	
  long total)
{
  DT_F("dttxms_alloc");
  unsigned char error;
  unsigned int freemem;
  unsigned int totmem;
  unsigned int freemem_after;
  unsigned int totmem_after;
  unsigned int totalk;

  error = XMS_FreeMem(&freemem, &totmem);
  if (error != 0)
    return dttxms_err(F,
      "XMS_FreeMem", error);

  totalk = (unsigned int)((total+1023L) / 1024L);
  if (freemem < totalk)
  {
    dttxms_debug(F, 
      "need %uK bytes, but xms only has %uK/%uK",
      totalk, freemem, totmem);
  }
  else
  {
    error = XMS_AllocEMB(totalk, handle);
    if (error)
      return dttxms_err(F,
        "XMS_AllocEMB", error);

    error = XMS_FreeMem(&freemem_after,
      &totmem_after);
    if (error != 0)
      return dttxms_err(F,
        "XMS_FreeMem", error);

	dttxms_debug(F,
      "new handle 0x%04x size %uK"
      " free now %uK/%uK", 
      *handle, totalk, 
      freemem_after, totmem_after);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * if out of space, should return gotspace==0 instead of error
 * if out of space, leaves old space intact
 *..........................................................................*/

dt_rc_e 
dttxms_realloc(
  XMShandle *handle,
  long total)
{
  DT_F("dttxms_realloc");
  unsigned char error;
  unsigned int freemem;
  unsigned int totmem;
  unsigned int freemem_after;
  unsigned int totmem_after;
  unsigned int totalk;

  error = XMS_FreeMem(&freemem, &totmem);
  if (error != 0)
    return dttxms_err(F,
      "XMS_FreeMem", error);

  totalk = (unsigned int)((total+1023L) / 1024L);

  if (freemem < totalk)
  {
    dttxms_debug(F, 
      "need %uK bytes, but xms only has %uK/%uK",
      totalk, freemem, totmem);
  }
  else
  {
    error = XMS_ReallocEMB(				/* resize the space				    */
      *handle, totalk);
    if (error)							/* resizing failed?				    */
      return dttxms_err(F,
        "XMS_ReallocEMB", error);

    error = XMS_FreeMem(&freemem_after,
      &totmem_after);
    if (error != 0)
      return dttxms_err(F,
        "XMS_FreeMem", error);

	dttxms_debug(F,
      "handle 0x%04x size %uK"
      " free now %uK/%uK", 
      *handle, totalk, 
      freemem_after, totmem_after);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttxms_free(	
  XMShandle *handle)	
{
  DT_F("dttxms_free");
  unsigned char error;
  
  dttxms_debug(F, 
    "freeing handle 0x%04x", 
    *handle);

  error = XMS_FreeEMB(*handle);
  if (error)
    return dttxms_err(F,
      "XMS_FreeEMB", error);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

int
main(
  int argc,
  char *argv[])
{
  dt_rc_e rc = DT_RC_GOOD;
  
  rc = dttxms_libinit();				/* initialize xms library 			*/

  if (rc == DT_RC_GOOD)
  {
	XMShandle handle1;
	XMShandle handle2;
	XMShandle handle3;

    dttxms_alloc(&handle1, 1024);		/* make three allocations 			*/
	
	dttxms_alloc(&handle2, 1024);

	dttxms_alloc(&handle3, 1024);

    dttxms_realloc(&handle1, 2048);		/* try to increase first one 		*/

    dttxms_realloc(&handle1, 2048);		/* try again 						*/

    dttxms_free(&handle3);
    dttxms_free(&handle2);
    dttxms_free(&handle1);
  }
  return 0;
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
