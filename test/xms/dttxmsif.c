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
 * first realloc fails A2, second succeeds
 * large model Borland 3.1
 * makefile
 * printed output
 * NCI node 10 Northgate 486/33 10/20/95:
 * DEVICE=C:\DOS\HIMEM.SYS              11552 02-03-91 1:00p
 * DEVICE=C:\DOS\EMM386.EXE 1024        91742 02-03-91 1:00p
 * dttxms_libinit: XMS v0200 has 14511104 bytes available
 * dttxms_alloc: new handle 0xa734 size 1024 core 14510080
 * dttxms_alloc: new handle 0xa722 size 1024 core 14509056
 * dttxms_alloc: new handle 0xa728 size 1024 core 14508032
 * dttxms_realloc: old handle 0xa734 new handle 0x4000 size 2048 core 14508032
 * dttxms_realloc: old handle 0x4000 new handle 0xffff size 2048 core 14505984
 * dttxms_realloc: XMM error XMM_BADXHAN (0xa2) in XMMrealloc
 * dttxms_free: freeing handle 0xa728
 * dttxms_free: freeing handle 0xa722
 * dttxms_free: freeing handle 0x4000
 * dttxms_free: XMM error XMM_BADXHAN (0xa2) in XMMfree
 */

#include <stdio.h>
#include <stdarg.h>
#include <malloc.h>
#include <string.h>
#include <xmsif.h>

#ifdef __WATCOMC__
#  pragma aux (cdecl) XMMlibinit;
#  pragma aux (cdecl) XMMcoreleft;
#  pragma aux (cdecl) XMMgetversion;
#  pragma aux (cdecl) XMMallcoreleft;
#  pragma aux (cdecl) XMMalloc;
#  pragma aux (cdecl) XMMrealloc;
#  pragma aux (cdecl) XMMfree;
#  pragma aux (cdecl) _XMMcopy;
#  pragma aux (cdecl) xmsif_vers_vers;
#  pragma aux (cdecl) xmsif_vers_date;
#  pragma aux (cdecl) xmsif_vers_time;
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
  const char *xmmfuncname)
{
  const char *s;
  switch(_XMMerror)
  {
#define CASE(ERROR) case ERROR: s = #ERROR; break;	
    CASE(XMM_NOINIT          )
    CASE(XMM_UMBHUGE         )
    CASE(XMM_BADPTR          )
    CASE(XMM_ELTOOBIG        )
    CASE(XMM_SKTOOBIG        )
    CASE(XMM_BADVERS         )

    CASE(XMM_UNIMP           )
    CASE(XMM_VDISK           )
    CASE(XMM_A20ERROR        )
    CASE(XMM_GENERROR        )
    CASE(XMM_UNRECERROR      )

    CASE(XMM_NOHMA           )
    CASE(XMM_HMAUSED         )
    CASE(XMM_HMATOOBIG       )
    CASE(XMM_HMANOALLOC      )
    CASE(XMM_A20STILLEN      )

    CASE(XMM_NOFREEX         )
    CASE(XMM_NOFREEXHAN      )
    CASE(XMM_BADXHAN         )
    CASE(XMM_BADSRCHAN       )
    CASE(XMM_BADSRCOFF       )
    CASE(XMM_BADDESTHAN      )
    CASE(XMM_BADDESTOFF      )
    CASE(XMM_BADLENGTH       )
    CASE(XMM_COPYOVERLAP     )
    CASE(XMM_PARITY          )
    CASE(XMM_NOLOCK          )
    CASE(XMM_LOCKED          )
    CASE(XMM_TOOMANYLOCKS    )
    CASE(XMM_LOCKFAIL        )

    CASE(XMM_UMBSMALLER      )
    CASE(XMM_NOFREEUMB       )
    CASE(XMM_BADUMBHAN       )
    default: s = "unknown";
  }

  dttxms_debug(f,
    "XMM error %s (0x%02x) in %s",
    s, _XMMerror, xmmfuncname);

  return DT_RC_BAD;
}
/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttxms_libinit(	
  void)
{
  DT_F("dttxms_libinit");
  unsigned long coreleft;
  int version;

  XMMlibinit();							/* init XMM library				    */
  if (_XMMerror)
    return dttxms_err(F,
      "XMMlibinit");

  coreleft = XMMcoreleft();
  if (_XMMerror)
    return dttxms_err(F,
      "XMMcoreleft");

  version = XMMgetversion();
  if (_XMMerror)
    return dttxms_err(F,
      "XMMgetversion");

  dttxms_debug(F,
    "XMS v%04x has %lu bytes available",
    version, coreleft);
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttxms_alloc(	
  int *handle,	
  long total)
{
  DT_F("dttxms_alloc");
  unsigned long coreleft;
  unsigned long coreleft_after;

  coreleft = XMMcoreleft();
  if (_XMMerror)
    return dttxms_err(F,
      "XMMcoreleft");

  if (coreleft < total)
  {
    dttxms_debug(F, 
      "need %lu bytes, but XMS manager largest block only has %lu",
      total, coreleft);
  }
  else
  {
    *handle = XMMalloc(total);
    if (_XMMerror)
      return dttxms_err(F,
        "XMMalloc");

    coreleft_after = XMMcoreleft();
    if (_XMMerror)
      return dttxms_err(F,
        "XMMcoreleft");

	dttxms_debug(F,
      "new handle 0x%04x size %ld"
      " core %lu", 
      *handle, total, 
      coreleft_after);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * if out of space, should return gotspace==0 instead of error
 * if out of space, leaves old space intact
 *..........................................................................*/

dt_rc_e 
dttxms_realloc(
  int *handle,
  long total)
{
  DT_F("dttxms_realloc");
  unsigned long coreleft;

  coreleft = XMMcoreleft();
  if (_XMMerror)
    return dttxms_err(F,
      "XMMcoreleft");

  if (coreleft < total)
  {
    dttxms_debug(F, 
      "need %lu bytes, but XMS manager only has %lu",
      total, coreleft);
  }
  else
  {
    int newhandle = 0;
    newhandle = XMMrealloc(				/* resize the space				    */
      *handle, total);
	dttxms_debug(F,
      "old handle 0x%04x"
      " new handle 0x%04x size %ld"
      " core %lu", 
      *handle,
      newhandle, total, coreleft);
    if (_XMMerror)						/* resizing failed?				    */
      return dttxms_err(F,
        "XMMrealloc");
    *handle = newhandle;				/* we have a new handle			    */
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e dttxms_write(	
  int *handle,	
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dttxms_write");

  XMMcopyto(size,
    (unsigned char *)buf, *handle, offset);
  if (_XMMerror)
    return dttxms_err(F,
      "XMMcopyto");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e dttxms_read(		
  int *handle,	
  long offset,
  void *buf,
  unsigned int size)
{
  DT_F("dttxms_read");

  XMMcopyfrom(size,
    *handle, offset, (unsigned char *)buf);
  if (_XMMerror)
    return dttxms_err(F,
      "XMMcopyfrom");

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e 
dttxms_free(	
  int *handle)	
{
  DT_F("dttxms_free");
  
  dttxms_debug(F, 
    "freeing handle 0x%04x", 
    *handle);

  XMMfree(*handle);
  if (_XMMerror)
    return dttxms_err(F,
      "XMMfree");

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
	int handle1;
	int handle2;
	int handle3;

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
