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
| main program
| 
| END
 *..........................................................................*/

#include <tableau.h>

DT_RCSID("app/tableau $RCSfile: zlib.c,v $ $Revision: 1.8 $");

#ifndef DTDEPEND
#  include <zlib.h>
#endif

/*..........................................................................
 * compress buffer
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_zlib_compress(
  dt_ctl_t *ctl,
  void *input,							/* input buffer 					*/
  long ninput,							/* size of input buffer 			*/
  void **output,						/* output buffer 					*/
  long *maxout,							/* size of output buffer 			*/
  long *noutput)						/* amount of output 				*/
{
  DT_F("tableau_zlib_compress");
  uLongf ulongf;
  int zrc;
  dt_rc_e rc = DT_RC_GOOD;

  if (*maxout < ninput)					/* output size too small? 			*/
  {
	if (*output != NULL)
	  DT_C(dtos_free2,(ctl, *output,
        F, "buffer"));
	DT_C(dtos_malloc2,(ctl, output,
      ninput, F, "buffer"));
	*maxout = ninput;
  }

  ulongf = *maxout;
  zrc = compress(
    (Bytef *)(*output),					/* output buffer 					*/
    &ulongf,							/* returned size of output 			*/
    (Bytef *)input,						/* source buffer 					*/
    ninput);							/* how much to compress 			*/
  if (zrc != Z_OK)
  {
	rc = dt_err(ctl, F,
      "zlib.compress had error %d",
      zrc);
	goto cleanup;
  }
  *noutput = ulongf;

  if (DT_DBG(ctl, DT_DBG_MASK_COMPLEX))
  {
	unsigned char *c = (unsigned char *)input;
	dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
      "compressed %ld into %ld of %ld,"
      " first data"
      " %02x%02x%02x%02x"
      " %02x%02x%02x%02x",
      ninput, *noutput, *maxout,
      c[0], c[1], c[2], c[3],
      c[4], c[5], c[6], c[7]);
  }

cleanup:
  return rc;
}

/*..........................................................................
 * uncompress buffer
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_zlib_uncompress(
  dt_ctl_t *ctl,
  void *input,							/* input buffer 					*/
  long ninput,							/* size of input buffer 			*/
  void **output,						/* output buffer 					*/
  long *maxout,							/* size of output buffer 			*/
  long *noutput)						/* amount of output 				*/
{
  DT_F("tableau_zlib_uncompress");
  uLongf ulongf;
  int zrc;
  dt_rc_e rc = DT_RC_GOOD;

  if (*maxout < ninput*4)				/* output size too small? 			*/
  {
	if (*output != NULL)
	  DT_C(dtos_free2,(ctl, *output,
        F, "buffer"));
	DT_C(dtos_malloc2,(ctl, output,
      ninput*4, F, "buffer"));
	*maxout = ninput*4;
  }

  ulongf = *maxout;
  zrc = uncompress(
    (Bytef *)(*output),					/* output buffer 					*/
    &ulongf,							/* returned size of output 			*/
    (Bytef *)input,						/* source buffer 					*/
    ninput);							/* how much to compress 			*/
  if (zrc != Z_OK)
  {
	rc = dt_err(ctl, F,
      "zlib.uncompress had error %d",
      zrc);
	goto cleanup;
  }

  *noutput = ulongf;

  if (DT_DBG(ctl, DT_DBG_MASK_COMPLEX))
  {
	unsigned char *c = (unsigned char *)(*output);
	dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
      "uncompressed %ld into %ld of %ld,"
      " first data"
      " %02x%02x%02x%02x"
      " %02x%02x%02x%02x",
      ninput, *noutput, *maxout,
      c[0], c[1], c[2], c[3],
      c[4], c[5], c[6], c[7]);
  }

cleanup:
  return rc;
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
