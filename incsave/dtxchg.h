/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





typedef enum {
  DTXCHG_PNM,
  DTXCHG_PGM,
  DTXCHG_PBM,
  DTXCHG_BPE,
  DTXCHG_JPG,
  DTXCHG_RAD,
  DTXCHG_TIFF,
  DTXCHG_PNM_ASCII,
  DTXCHG_PGM_ASCII,
  DTXCHG_PBM_ASCII,
  DTXCHG_IPW,
  DTXCHG_RAWFILE,
  DTXCHG_ASCIIFILE,
  DTXCHG_LJ600,
  DTXCHG_LJ300,
  DTXCHG_BMP,
  DTXCHG_WMF,
  DTXCHG_WIN,
  DTXCHG_WXX,
  DTXCHG_DTM,
  DTXCHG_CANVAS,
  DTXCHG_HWS,
  DTXCHG_END_OF_LIST
} dtxchg_e;

#define DTXCHG_SCR_BEG DTXCHG_CANVAS
#define DTXCHG_SCR_END DTXCHG_HWS

#define DTXCHG_READABLE_BEG DTXCHG_PNM
#define DTXCHG_READABLE_END DTXCHG_IPW

										/* ................................ */
#ifdef __cplusplus
extern "C" {
#endif

extern 
const 
char ** const dtxchg_choices;			/* names for dtxchg_e deps			*/

extern 
const 
char ** const dtxchg_suffixes;			/* list of recognized suffixes 		*/

extern
const 
dtxchg_e * const dtxchg_suffixes_which;	/* types for suffixes				*/

#ifdef __cplusplus
}
#endif
										/* ................................ */

#define DTXCHG_ERR_BASE			(DT_ERR_XCHG)
#define DTXCHG_ERR_NOSPEC		(DTXCHG_ERR_BASE+1)
#define DTXCHG_ERR_EMPTYSPEC	(DTXCHG_ERR_BASE+2)
#define DTXCHG_ERR_NOSERVER		(DTXCHG_ERR_BASE+3)
#define DTXCHG_ERR_BADSUFFIX	(DTXCHG_ERR_BASE+4)
#define DTXCHG_ERR_LONGTYPE		(DTXCHG_ERR_BASE+5)
#define DTXCHG_ERR_BADTYPE		(DTXCHG_ERR_BASE+6)
#define DTXCHG_ERR_AMBTYPE		(DTXCHG_ERR_BASE+7)

#include <dtack/xchg.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
