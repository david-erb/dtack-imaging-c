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
DT_RCSID("source/high $RCSfile: xchg_dat.c,v $ $Revision: 1.1 $");

#include <dtack/lut8.h>
#include <dtack/xchg.h>

static const char *choices[] = {		/* must match dtxchg_e! 			*/
  "pnm",
  "pgm",
  "pbm",
  "bpe",
  "jpg",
  "rad",
  "tiff",
  "pna",
  "pga",
  "pba",
  "ipw",
  "raw",
  "ascii",
  "lj6",
  "lj3",
  "bmp",								/* Microsoft windows bitmap 		*/
  "wmf",								/* Microsoft windows metafile 		*/
  "win",
  "wxx",
  "dtm",
  "canvas",
  "hws",
  NULL
};

static const char *suffixes[] = {		/* must match suffixes_which! 		*/
  "pnm",
  "pgm",
  "pbm",
  "bpe",
  "jpg",
  "rad",
  "tif",
  "ipw",
  "raw",
  "asc",
  "lj6",
  "lj3",
  "bmp",
  "wmf",
  NULL
};

static dtxchg_e suffixes_which[] = {	/* select which given suffix	    */
  DTXCHG_PNM,
  DTXCHG_PGM,
  DTXCHG_PBM,
  DTXCHG_BPE,
  DTXCHG_JPG,
  DTXCHG_RAD,
  DTXCHG_TIFF,
  DTXCHG_IPW,
  DTXCHG_RAWFILE, 
  DTXCHG_ASCIIFILE,
  DTXCHG_LJ600,
  DTXCHG_LJ300,
  DTXCHG_BMP,
  DTXCHG_WMF
};

const char    ** const dtxchg_choices         = choices;
const char    ** const dtxchg_suffixes        = suffixes;
const dtxchg_e * const dtxchg_suffixes_which  = suffixes_which;





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
