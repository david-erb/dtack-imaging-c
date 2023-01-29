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

#include <dtack/base.h>
#include <dtack/ipc.h>
#include <dtack/lut8.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>
#include <dtack/xchg.h>
#include <tableau.h>

#include <dtack/main.h>

#include <dtack/os.h>
#include <dtack/str.h>

DT_RCSID("app/tableau $RCSfile: compress.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_compress(
  dtparam_t *param,
  const char *input_name,
  const char *output_name)
{
  DT_F("tabtool_compress");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t input;
  void *b;
  dt1d_i_t isize, msize, osize;
  dtfd_t fd;
  dt_rc_e rc;
  static dt_ntype_e ntypes[] = {		/* can handle only byte images 		*/
    DT_NTYPE_S1, DT_NTYPE_NULL};

  input.xe = 0;
  fd.fd = -1;

  DT_C(dtimage_import,(ctl, &input,		/* read image from file 			*/
    input_name));
  
  rc = dtimage_check_1f(ctl,			/* tracer						    */
    &input, "input",
    ntypes, 0, F);						/* cannot handle paged images 		*/
  if (rc != DT_RC_GOOD)
    goto cleanup;

  isize = input.rowsize * input.ye;
  {
	dt_ntype_s1_t * const s1 =			/* assume data is signed byte 		*/
      input.row[0].s1;
	const dt_ntype_s1_t t =				/* convert threshold for speed 		*/
      (dt_ntype_s1_t)param->t;
	dt1d_i_t i;
    for (i=0; i<isize; i++)				/* clip small values 				*/
      if (s1[i] > -t &&
	  	  s1[i] < t)
        s1[i] = 0;
  }

  msize = isize;
  DT_C(dtos_malloc2,(ctl,				/* space for for the encoded buffer */
    (void **)&b, msize,
    F, "encode buffer"));

  DT_C_1D(dt1d_rlerb_encode,(			/* encode 							*/
    input.row[0].dvoid, isize,
    b, msize, &osize));
  
  DT_C(dtfd_open,(ctl, &fd,				/* open file for output 			*/
    output_name, "w"));
  DT_C(dtfd_write_all,(ctl, &fd,		/* write the entire fb to the file 	*/
      b, osize));
  DT_C(dtfd_close,(ctl, &fd));
  fd.fd = -1;

cleanup:
  if (input.xe != 0)
	DT_I(dtimage_free,(ctl, &input));
  if (fd.fd != -1)						/* file was opened? 				*/
    DT_I(dtfd_close,(ctl, &fd));		/* close it 						*/

  if (b != NULL)
  {
    DT_I(dtos_free2,(ctl, b,			/* free buffer pointers 			*/
      F, "encode buffer"));
  }

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
