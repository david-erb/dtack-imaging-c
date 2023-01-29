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

#define const							/* get around read-only _EMMerror	*/

#include <dtack/nofar.h>
#ifndef DTDEPEND
#include <emsif.h>
#endif

unsigned char _EMMerror;

#define MAX_BLOCKS (128)

/*
 * 16-bit qnx is the only unix-like 16-bit OS on which we would 
 * want to emulate ems
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
EMMlibinit(
  void)
{
  if (blocks == NULL)
  {
    blocks = (char __huge **)malloc(
      MAX_BLOCKS*sizeof(*blocks));
    if (blocks != NULL)
	{
      memset(blocks, 0, MAX_BLOCKS*sizeof(*blocks));
      _EMMerror = 0;
	}
    else
	{
      _EMMerror = 0xfd;
	}
    bytess = (unsigned long *)malloc(
      MAX_BLOCKS*sizeof(*bytess));
    if (bytess == NULL)
	{
	  free(blocks);
      _EMMerror = 0xfd;
	}
  }
  return 0;
}

/*..........................................................................
 *..........................................................................*/

unsigned long
EMMcoreleft(
  void)
{
  return 32000000UL;					/* some fake number				    */
}

/*..........................................................................
 *..........................................................................*/

int 
EMMalloc(
  unsigned long bytes)
{
  int handle;
  for (handle=0; handle < MAX_BLOCKS; handle++)
  {
    if (blocks[handle] == NULL)
	{
      blocks[handle] = (char __huge *)halloc(bytes, 1);
	  bytess[handle] = bytes;
      if (blocks[handle] == NULL)
        _EMMerror = EMM_TOOBIG;
      else
        _EMMerror = 0;
      break;
	}
  }
  if (handle == MAX_BLOCKS)
    _EMMerror = EMM_NOFREEHAN;

  return handle;
}

/*..........................................................................
 *..........................................................................*/

int 
EMMrealloc(
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
	  char __huge *h = blocks[handle];
	  char __huge *new = newblock;
	  unsigned long n = bytess[handle];
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
      _EMMerror = 0;
	}
    else
      _EMMerror = EMM_TOOBIG;
  }
  else
    _EMMerror = EMM_BADHANDLE;

  return handle;
}

/*..........................................................................
 *..........................................................................*/

int 
EMMfree(
  int handle)
{
  if (handle >= 0 &&
      handle < MAX_BLOCKS &&
      blocks[handle])
  {
    hfree(blocks[handle]);
    blocks[handle] = NULL;
    _EMMerror = 0;
  }
  else
    _EMMerror = EMM_BADHANDLE;
  return 0;
}

/*..........................................................................
 *..........................................................................*/

int 
EMMcopyfrom(
  unsigned long copylen,
  int handle,
  unsigned long foffset,
  unsigned char DTCONFIG_FAR *dest)
{
  if (copylen > 65535L)
	_EMMerror = EMM_BADLENGTH;
  else
  if (handle >= 0 &&
      handle < MAX_BLOCKS &&
      blocks[handle])
  {
	const char __huge *h = blocks[handle]+foffset;
	const unsigned int n = (unsigned int)copylen;
	memcpy_h2f(dest, h, n);
#ifdef NONO
	unsigned int i;
	for (i=0; i<n; i++)
      dest[i] = h[i];
#endif	
    _EMMerror = 0;
  }
  else
    _EMMerror = EMM_BADHANDLE;
  return 0;
}


/*..........................................................................
 *..........................................................................*/

int 
EMMcopyto(
  unsigned long copylen,
  unsigned char DTCONFIG_FAR *source,
  int handle,
  unsigned long foffset)
{
  if (copylen > 65535L)
	_EMMerror = EMM_BADLENGTH;
  else
  if (handle >= 0 &&
      handle < MAX_BLOCKS &&
      blocks[handle])
  {
	const char __huge *h = blocks[handle]+foffset;
	const unsigned int n = (unsigned int)copylen;
	memcpy_f2h(h, source, n);
#ifdef NONO
	unsigned int i;
	for (i=0; i<n; i++)
      h[i] = source[i];
#endif	
    _EMMerror = 0;
  }
  else
    _EMMerror = EMM_BADHANDLE;
  return 0;
}




#else

static char **blocks = NULL;

/*..........................................................................
 *..........................................................................*/

int 
EMMlibinit(
  void)
{
  if (blocks == NULL)
  {
    blocks = (char **)malloc(
      MAX_BLOCKS*sizeof(*blocks));
    if (blocks != NULL)
	{
      memset(blocks, 0, MAX_BLOCKS*sizeof(*blocks));
      _EMMerror = 0;
	}
    else
      _EMMerror = 0xfd;
  }
  return 0;
}

/*..........................................................................
 *..........................................................................*/

unsigned long
EMMcoreleft(
  void)
{
  return 32000000UL;					/* some fake number				    */
}

/*..........................................................................
 *..........................................................................*/

int 
EMMalloc(
  unsigned long bytes)
{
  int handle;
  for (handle=0; handle < MAX_BLOCKS; handle++)
  {
    if (blocks[handle] == NULL)
	{
      blocks[handle] = (char *)malloc(bytes);
      if (blocks[handle] == NULL)
        _EMMerror = EMM_TOOBIG;
      else
        _EMMerror = 0;
      break;
	}
  }
  if (handle == MAX_BLOCKS)
    _EMMerror = EMM_NOFREEHAN;

  return handle;
}

/*..........................................................................
 *..........................................................................*/

int 
EMMrealloc(
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
      _EMMerror = 0;
	}
    else
      _EMMerror = EMM_TOOBIG;
  }
  else
    _EMMerror = EMM_BADHANDLE;

  return handle;
}

/*..........................................................................
 *..........................................................................*/

int 
EMMfree(
  int handle)
{
  if (handle >= 0 &&
      handle < MAX_BLOCKS &&
      blocks[handle])
  {
    free(blocks[handle]);
    blocks[handle] = NULL;
    _EMMerror = 0;
  }
  else
    _EMMerror = EMM_BADHANDLE;
  return 0;
}

/*..........................................................................
 *..........................................................................*/

int 
EMMcopyfrom(
  unsigned long copylen,
  int handle,
  unsigned long foffset,
  unsigned char DTCONFIG_FAR *dest)
{
  if (handle >= 0 &&
      handle < MAX_BLOCKS &&
      blocks[handle])
  {
    memcpy(dest, blocks[handle]+foffset, copylen);
    _EMMerror = 0;
  }
  else
    _EMMerror = EMM_BADHANDLE;
  return 0;
}


/*..........................................................................
 *..........................................................................*/

int 
EMMcopyto(
  unsigned long copylen,
  unsigned char DTCONFIG_FAR *source,
  int handle,
  unsigned long foffset)
{
  if (handle >= 0 &&
      handle < MAX_BLOCKS &&
      blocks[handle])
  {
    memcpy(blocks[handle]+foffset, source, copylen);
    _EMMerror = 0;
  }
  else
    _EMMerror = EMM_BADHANDLE;
  return 0;
}






#endif



#ifdef NONO
#include <malloc.h>

#include <stdio.h>

main()
{
  char __huge *h;
  h = halloc(1, 0x10010);
  printf("0x%08lx\n", h);  
  printf("0x%08lx\n", h+0x00001);
  printf("0x%08lx\n", h+0x10000);
  printf("0x%08lx\n", h+0x10001);
  printf("0x%08lx\n", h+0x20001);
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
