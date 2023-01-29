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
| include $RCSfile: dtpnm.h,v $ $Revision: 1.3 $
 *..........................................................................*/

typedef enum {
  DTPNM_COMPRESS_NONE = 0,
  DTPNM_COMPRESS_BPE = 100,
  DTPNM_COMPRESS_RAD = 200
} dtpnm_compress_e;

typedef enum {
  DTPNM_MAGIC_PBM_RAW = 4,				/* from the pgm manual  			*/
  DTPNM_MAGIC_PBM_ASCII = 1,
  DTPNM_MAGIC_PGM_RAW = 5,
  DTPNM_MAGIC_PGM_ASCII = 2
} dtpnm_magic_e;

typedef struct {
  unsigned long xe;
  unsigned long ye;
  int ptype;
  dt_ntype_e ntype;
  dtpnm_magic_e magic;
  dtpnm_compress_e compress;
} dtpnm_t;

#include <dtack/pnm.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
