/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




/* ----------------------------- MNI Header -----------------------------------
@NAME       : globals.h
@INPUT      : 
@OUTPUT     : 
@RETURNS    : 
@DESCRIPTION: Declarations form global variables used internally by the
              Berkeley MPEG decoding engine (except for those specific
	      to image dithering, which are declared in dither.h).
@METHOD     : 
@GLOBALS    : ditherType
              input
              [shmemFlag]
              totNumFrames
@CALLS      : 
@CREATED    : Greg Ward, 94/6/16
@MODIFIED   : 
---------------------------------------------------------------------------- */

#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <stdio.h>
#include "mpeg.h"

#if (FULL_COLOR_ONLY)
# define DoDitherImage(l,Cr,Cb,disp,h,w) ColorDitherImage(l,Cr,Cb,disp,h,w)
#endif

extern DitherEnum ditherType;
extern FILE *input;

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
