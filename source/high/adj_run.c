/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: adj_run.c,v $ $Revision: 1.15 $");
#include <dtack/stack.h>
#include <dtack/mem.h>

/*..........................................................................
| NAME
| dtimage_adj_run_blob() - connected components extraction from runs
| 
| SUMMARY
| Break down the image into runs of identical pixels, then
| extract blobs formed by adjacent runs.
| Assume 4-connectivity unless the :DTIMAGE_ADJ_FLAG_8: bit is set.
| Ignore pixels with the ~invisible~ value if the :DTIMAGE_ADJ_FLAG_NOT:
| bit is set.
| Ignore all except invisible pixels if the :DTIMAGE_ADJ_FLAG_ONLY:
| bit is set.
| Ignore no pixels if both or neither of these bits is set.
| Examine only pixels in the rectangular area specified.
| As each blob is found, pass the blob and the unchanged ~arg~
| to the ~deliver~ function.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_adj_run_blob(					/* extract blobs using runs			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *input,						/* input image 						*/
  dtxy_t xmin,							/* upper left column of rectangle	*/
  dtxy_t ymin,							/* upper left row 					*/
  dtxy_t xmax,							/* lower right column 				*/
  dtxy_t ymax,							/* lower right row 					*/
  double invisible,						/* value of invisible pixels 		*/
  unsigned long flags,					/* process control 					*/
  dtrun_deliver_f *deliver,				/* function to call for each blob 	*/
  void *arg)							/* user argument to pass to deliver */
{
  DT_F("dtimage_adj_run_blob");
  dtrun_t run;
  int scan_flag;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input, "input",
    dtrun_ntype_list, 
    DTIMAGE_FLAG_PAGED, F);		
  if (rc != DT_RC_GOOD)
    return rc;

  if ((flags &							/* keep both?						*/
       (DTIMAGE_ADJ_FLAG_ONLY |
	    DTIMAGE_ADJ_FLAG_NOT)) ==
       (DTIMAGE_ADJ_FLAG_ONLY |
	    DTIMAGE_ADJ_FLAG_NOT))
    scan_flag = 0;
  else
  if (flags & DTIMAGE_ADJ_FLAG_ONLY)	/* keep invisible pixels only?		*/
    scan_flag = 1;
  else
  if (flags & DTIMAGE_ADJ_FLAG_NOT)		/* keep non-invisible pixels only?	*/
    scan_flag = -1;
  else
    scan_flag = 0;						/* keep all pixels by default		*/
  
  DT_Q(dtrun_init,(ctl, &run));			/* get space for runs			    */

  DT_C(dtimage_run_scan,(ctl,			/* scan image for runs			    */
	input, &run,
	xmin, ymin, xmax, ymax,
    invisible, scan_flag));

  DT_C(dtimage_run_label,(ctl,			/* make blobs out of runs		    */
    input, &run, flags,
    deliver, arg));

cleanup:
  if (rc != DT_RC_BAD)
    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
      "runs %ld, blobs %ld",
      (long)run.runs,
      (long)run.blobs);

  DT_I(dtrun_uninit,(ctl, &run));		/* free all associated space	    */

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
