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
#include <txms.h>

#ifdef __WATCOMC__
#  pragma aux (cdecl) initxms;
#  pragma aux (cdecl) getfreexmsmem;
#  pragma aux (cdecl) getxmsmem;
#  pragma aux (cdecl) resizexmsmem;
#  pragma aux (cdecl) freexmsmem;
#  pragma aux (cdecl) movexmsmem;
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
  int error)
{
  dttxms_debug(f,
    "txms error 0x%02x in %s",
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
  unsigned int freemem;
  unsigned int maxfree;
  int error;

  error = initxms();					/* init XMM library				    */
  if (error != 0)
    return dttxms_err(F,
      "initxms", error);

  error = getfreexmsmem(&freemem,		/* query free memory 				*/
    &maxfree);
  if (error != 0)
    return dttxms_err(F,
      "getfreexmsmem", error);

  dttxms_debug(F,
    "XMS %uK/%uK bytes available",
    freemem, maxfree);
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttxms_alloc(	
  uint *handle,	
  long total)
{
  DT_F("dttxms_alloc");
  int error;
  unsigned int freemem;
  unsigned int maxfree;
  unsigned int freemem_after;
  unsigned int maxfree_after;
  unsigned int totalk;

  error = getfreexmsmem(&freemem, 
    &maxfree);
  if (error != 0)
    return dttxms_err(F,
      "getfreexmsmem", error);

  totalk = (unsigned int)((total+1023L) / 1024L);
  if (freemem < totalk)
  {
    dttxms_debug(F, 
      "need %uK bytes, but xms only has %uK/%uK",
      totalk, freemem, maxfree);
  }
  else
  {
    error = getxmsmem(totalk, handle);
    if (error)
      return dttxms_err(F,
        "getxmsmem", error);

    error = getfreexmsmem(&freemem_after, 
      &maxfree_after);
    if (error != 0)
      return dttxms_err(F,
        "getfreexmsmem", error);

	dttxms_debug(F,
      "new handle 0x%04x size %uK"
      " free now %uK/%uK", 
      *handle, totalk, 
      freemem_after, maxfree_after);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * if out of space, should return gotspace==0 instead of error
 * if out of space, leaves old space intact
 *..........................................................................*/

dt_rc_e 
dttxms_realloc(
  uint *handle,
  long total)
{
  DT_F("dttxms_realloc");
  int error;
  unsigned int freemem;
  unsigned int maxfree;
  unsigned int freemem_after;
  unsigned int maxfree_after;
  unsigned int totalk;

  error = getfreexmsmem(&freemem, 
    &maxfree);
  if (error != 0)
    return dttxms_err(F,
      "getfreexmsmem", error);

  totalk = (unsigned int)((total+1023L) / 1024L);

  if (freemem < totalk)
  {
    dttxms_debug(F, 
      "need %uK bytes, but xms only has %uK/%uK",
      totalk, freemem, maxfree);
  }
  else
  {
    dttxms_debug(F,
      "going into resizexmsmem");
    error = resizexmsmem(				/* resize the space				    */
      totalk, *handle);
    dttxms_debug(F,
      "came out of resizexmsmem");
    if (error)							/* resizing failed?				    */
      return dttxms_err(F,
        "resizexmsmem", error);

    error = getfreexmsmem(&freemem_after, 
      &maxfree_after);
    if (error != 0)
      return dttxms_err(F,
        "getfreexmsmem", error);

	dttxms_debug(F,
      "handle 0x%04x size %uK"
      " free now %uK/%uK", 
      *handle, totalk, 
      freemem_after, maxfree_after);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttxms_free(	
  uint *handle)	
{
  DT_F("dttxms_free");
  int error;
  
  dttxms_debug(F, 
    "freeing handle 0x%04x", 
    *handle);

  error = freexmsmem(*handle);
  if (error)
    return dttxms_err(F,
      "freexmsmem", error);

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
	uint handle1;
	uint handle2;
	uint handle3;

    dttxms_alloc(&handle1, 1024);		/* make three allocations 			*/
	
	dttxms_alloc(&handle2, 1024);

	dttxms_alloc(&handle3, 1024);

#if 0
    dttxms_realloc(&handle1, 2048);		/* try to increase first one 		*/

    dttxms_realloc(&handle1, 2048);		/* try again 						*/
#endif

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
