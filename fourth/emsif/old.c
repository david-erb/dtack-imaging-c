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










/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
