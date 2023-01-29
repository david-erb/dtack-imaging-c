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
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/config.h>

#define const							/* get around read-only _XMMerror	*/

#include <dtack/nofar.h>
#ifndef DTDEPEND
#include <xmsif.h>
#endif

unsigned char _XMMerror;

#define MAX_BLOCKS (128)

char xmsif_vers_vers[] = "dt fake xmsif";
char xmsif_vers_date[] = "date";
char xmsif_vers_time[] = "time";

/*
 * 16-bit qnx is the only unix-like 16-bit OS on which we would 
 * want to emulate xms
 */

#ifdef DTCONFIG_IMPLIED_QNX416
#ifndef DTDEPEND
#include <malloc.h>
#endif


static char __huge **blocks = NULL;
static unsigned long *bytess;

/*..........................................................................
 *..........................................................................*/
static
void
memcpy_f2h(
  char __huge *h,
  char *f,
  unsigned int n)
{
  unsigned int m;
  m = 65536L - ((long)h & 0xffffL);		/* amount in first segment 			*/
  if (m > n)							/* not bleeding into next segment?	*/
    m = n;
  memcpy((void __far *)h, f, m);		/* copy first part 					*/
  if (m < n)							/* bleeding into next segment? 		*/
    memcpy((void __far *)(h+m), f+m, 	/* copy second part 				*/
      n-m);
}

/*..........................................................................
 *..........................................................................*/
static
void
memcpy_h2f(
  char *f,
  char __huge *h,
  unsigned int n)
{
  unsigned int m;
  m = 65536L - ((long)h & 0xffffL);		/* amount in first segment 			*/
  if (m > n)							/* not bleeding into next segment?	*/
    m = n;
  memcpy(f, (void __far *)h, m);		/* copy first part 					*/
  if (m < n)							/* bleeding into next segment? 		*/
    memcpy(f+m, (void __far *)(h+m), 	/* copy second part 				*/
      n-m);
}

/*..........................................................................
 *..........................................................................*/

int 
XMMlibinit(
  void)
{
  if (blocks == NULL)					/* haven't been called yet? 		*/
  {
    blocks = (char **)malloc(
      MAX_BLOCKS*sizeof(*blocks));
    if (blocks != NULL)
	{
      memset(blocks, 0, 
        MAX_BLOCKS*sizeof(*blocks));
      _XMMerror = 0;
	}
    else
      _XMMerror = 0xfd;
    bytess = (unsigned long *)malloc(
      MAX_BLOCKS*sizeof(*bytess));
    if (bytess == NULL)
	{
	  free(blocks);
      _XMMerror = 0xfd;
	}
  }
  return 0;
}

/*..........................................................................
 *..........................................................................*/

unsigned long
XMMcoreleft(
  void)
{
  return 32000000UL;					/* some fake number				    */
}

/*..........................................................................
 *..........................................................................*/

unsigned long
XMMallcoreleft(
  void)
{
  return 32000000UL;					/* some fake number				    */
}

/*..........................................................................
 *..........................................................................*/

int 
XMMalloc(
  unsigned long bytes)
{
  int handle;
  for (handle=1;						/* find a free handle			    */
       handle < MAX_BLOCKS; 
       handle++)
  {
    if (blocks[handle] == NULL)
	{
      blocks[handle] = (char __huge *)halloc(bytes, 1);
	  bytess[handle] = bytes;
      if (blocks[handle] == NULL)
        _XMMerror = XMM_NOFREEX;
      else
        _XMMerror = 0;
      break;
	}
  }
  if (handle == MAX_BLOCKS)
    _XMMerror = XMM_NOFREEXHAN;

  return handle;
}

/*..........................................................................
 *..........................................................................*/

int 
XMMrealloc(
  int handle,
  unsigned long bytes)
{
  if (handle >= 0 &&
      handle < MAX_BLOCKS &&
      blocks[handle])
  {
    char __huge *newblock = (char __huge *)halloc(
      bytes, 1);
    if (newblock)
	{
	  const char __huge *h = blocks[handle];
	  char __huge *new = newblock;
	  const unsigned long n = bytess[handle];
	  unsigned long i;
	  if (bytes < n)
	    n = bytes;
	  for (i=0; i<(n/0x10000UL); i++)	/* for each segment of huge space 	*/
	  {
		memcpy((void __far *)new,		/* copy first half of the segment 	*/
          (void __far *)h, 32768u);
		new += 32768u;
		h += 32768u;
		memcpy((void __far *)new,		/* copy second half of the segment 	*/
          (void __far *)h, 32768u);
		new += 32768u;
		h += 32768u;
	  }
	  n %= 0x10000UL;					/* partial segment left over 		*/
	  if (n)
	    memcpy((void __far *)new, 
          (void __far *)h, n);
	  hfree(blocks[handle]);
      blocks[handle] = newblock;
	  bytess[handle] = bytes;
      _XMMerror = 0;
	}
    else
      _XMMerror = XMM_NOFREEX;
  }
  else
    _XMMerror = XMM_BADXHAN;

  return handle;
}

/*..........................................................................
 *..........................................................................*/

int 
XMMfree(
  int handle)
{
  if (handle >= 0 &&
      handle < MAX_BLOCKS &&
      blocks[handle])
  {
    hfree(blocks[handle]);
    blocks[handle] = NULL;
    _XMMerror = 0;
  }
  else
    _XMMerror = XMM_BADXHAN;
  return 0;
}

/*..........................................................................
 *..........................................................................*/

int 
_XMMcopy(
  unsigned long copylen,
  int shandle,
  unsigned long soffset,
  int dhandle,
  unsigned long doffset)
{
  if (copylen > 65535L)
	_XMMerror = XMM_BADLENGTH;
  else
  if ((shandle == 0 || 
      (shandle >= 0 && shandle < MAX_BLOCKS && blocks[shandle])) &&
      (dhandle == 0 || 
      (dhandle >= 0 && dhandle < MAX_BLOCKS && blocks[dhandle])))
  {
    if (shandle == 0)
	{
	  const char __huge *h = blocks[dhandle]+doffset;
	  const unsigned int n = (unsigned int)copylen;
	  const char *source = (char *)soffset;
	  memcpy_f2h(h, source, n);
	}
    else
    if (dhandle == 0)
	{
	  const char __huge *h = blocks[shandle]+soffset;
	  const unsigned int n = (unsigned int)copylen;
	  const char *dest = (char *)doffset;
  	  memcpy_h2f(dest, h, n);
	}
	else
	{
	  const char __huge *source = blocks[shandle]+soffset;
	  const char __huge *dest = blocks[dhandle]+doffset;
#     define BUFSIZE ((unsigned int)10000)
	  char *buf = (char *)malloc(BUFSIZE);
	  unsigned int n, i;
      n = (unsigned int)(copylen / (unsigned long)BUFSIZE);
	  for (i=0; i<n; i++)
	  {
  	    memcpy_h2f(buf, source, BUFSIZE);
		source += BUFSIZE;
  	    memcpy_f2h(dest, buf, BUFSIZE);
		dest += BUFSIZE;
	  }
	  n = (unsigned int)(copylen % (unsigned long)BUFSIZE);
	  if (n)
	  {
  	    memcpy_h2f(buf, source, n);
  	    memcpy_f2h(dest, buf, n);
	  }
      free(buf);
	}
    _XMMerror = 0;
  }
  else
    _XMMerror = XMM_BADXHAN;
  return 0;
}

#else
static char **blocks = NULL;

/*..........................................................................
 *..........................................................................*/

int 
XMMlibinit(
  void)
{
  if (blocks == NULL)
  {
    blocks = (char **)malloc(
      MAX_BLOCKS*sizeof(*blocks));
    if (blocks != NULL)
	{
      memset(blocks, 0, MAX_BLOCKS*sizeof(*blocks));
      _XMMerror = 0;
	}
    else
      _XMMerror = 0xfd;
  }
  return 0;
}

/*..........................................................................
 *..........................................................................*/

unsigned long
XMMcoreleft(
  void)
{
  return 32000000UL;					/* some fake number				    */
}

/*..........................................................................
 *..........................................................................*/

unsigned long
XMMallcoreleft(
  void)
{
  return 32000000UL;					/* some fake number				    */
}

/*..........................................................................
 *..........................................................................*/

int 
XMMalloc(
  unsigned long bytes)
{
  int handle;
  for (handle=1;						/* find a free handle			    */
       handle < MAX_BLOCKS; 
       handle++)
  {
    if (blocks[handle] == NULL)
	{
      blocks[handle] = (char *)malloc(bytes);
      if (blocks[handle] == NULL)
        _XMMerror = XMM_NOFREEX;
      else
        _XMMerror = 0;
      break;
	}
  }
  if (handle == MAX_BLOCKS)
    _XMMerror = XMM_NOFREEXHAN;

  return handle;
}

/*..........................................................................
 *..........................................................................*/

int 
XMMrealloc(
  int handle,
  unsigned long bytes)
{
  if (handle >= 0 &&
      handle < MAX_BLOCKS &&
      blocks[handle])
  {
    char *newblock = realloc(
      blocks[handle], bytes);
    if (newblock)
	{
      blocks[handle] = newblock;
      _XMMerror = 0;
	}
    else
      _XMMerror = XMM_NOFREEX;
  }
  else
    _XMMerror = XMM_BADXHAN;

  return handle;
}

/*..........................................................................
 *..........................................................................*/

int 
XMMfree(
  int handle)
{
  if (handle >= 0 &&
      handle < MAX_BLOCKS &&
      blocks[handle])
  {
    free(blocks[handle]);
    blocks[handle] = NULL;
    _XMMerror = 0;
  }
  else
    _XMMerror = XMM_BADXHAN;
  return 0;
}

/*..........................................................................
 *..........................................................................*/

int 
_XMMcopy(
  unsigned long copylen,
  int shandle,
  unsigned long soffset,
  int dhandle,
  unsigned long doffset)
{
  if ((shandle == 0 || 
      (shandle >= 0 && shandle < MAX_BLOCKS && blocks[shandle])) &&
      (dhandle == 0 || 
      (dhandle >= 0 && dhandle < MAX_BLOCKS && blocks[dhandle])))
  {
    if (shandle == 0)
      memcpy(blocks[dhandle]+doffset, (void *)soffset, copylen);
    else
    if (dhandle == 0)
      memcpy((void *)doffset, blocks[shandle]+soffset, copylen);
	else
      memcpy(blocks[dhandle]+doffset, blocks[shandle]+soffset, copylen);
    _XMMerror = 0;
  }
  else
    _XMMerror = XMM_BADXHAN;
  return 0;
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
