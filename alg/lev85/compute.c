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
DT_RCSID("alg/lev85 $RCSfile: compute.c,v $ $Revision: 1.9 $");
#include <dtlev85.h>

/*..........................................................................
| NAME
| dtlev85_compute() - compute final uniformity of area
| 
| DESCRIPTION
| dtlev85_compute() computes the final uniformity of the area
| after region contributions.
|_index dtlev85 structure,, computation of uniformity using
|_index uniformity,, final computation of
| 
| The computed uniformity is placed in the structure member ~lev85->u~.
| If no regions have contributed to the structure,
| then the uniformity ~lev85->u~ is set to 0.0.
|
| The given ~lev85~ structure must have been initialized by dtlev85_init().  
| It will normally have had one or more region contributions
| accumulated into it by dtlev85_blob()
| or dtlev85_hist().
|
| This function effectively finishes the implementation of
| equations (7) and (8) of [lev85].
| The summation term in equation (8)
| has already been done as part of the accumulation.
|
| PARAMETERS
|dtinclude(ctlonly.dx)
|
| RETURN VALUES
| The ~lev85->u~ structure member is assigned the area's uniformity.
|dtinclude(return1.dx)
| 
| ERRORS
|dtinclude(noerror.dx)
| 
| CAVEATS
| Attempting to use the ~lev85~ structure without first initializing it
| may result in incorrect results.
| 
| Computing the uniformity of an area with no contributed regions will
| result in a value of 0.0.
| 
| SEE ALSO
| dtlev85_init()			initializes lev85 structure prior to use
| dtlev85_hist()	contributes a region's histogram to the area
| dtlev85_blob()	contributes a blob directly to the area
| dtlev85_combine()			combines two areas into a single area
|
| EXAMPLE
| Example :lev85c.c:, immediately below, illustrates 
| dtlev85_compute() used in a simple-minded program.
| For two other examples using dtlev85_compute(), please refer to the
|_hyper Example lev85d.c, "Example lev85d.c"
| and
|_hyper sections, Example lev85e.c
|
| EXAMPLE lev85c.c
|_index dtlev85_hist(),, example of
|:// Example Program dtlev85c
|:// Illustrates dtlev85_init(), dtlev85_hist(), and 
|:// dtlev85_compute().
|:// Single argument is the gray level image filename, such as:
|://     dtlev85c  data/dice64.pgm
|:// Uniformity of image taken as monolithic region is computed and printed.
|:// All error checking left out for clarity.
|:
|:#include <dtack/base.h>
|:#include <dtlev85.h>
|:
|:void main(int argc, char *argv[])
|:{
|:  dtimage_t seg;
|:  dtimage_t gray;
|:  dthist_t hist;
|:  dtlev85_t lev85;
|:
|:  dtimage_import(NULL,		 		// read and allocate gray image
|:    &gray, argc>1? argv[1]: "");
|:  dtimage_create(NULL, &seg,			// create space for segmented image
|_aside Image filled with 1's is the blob.
|:    gray.xe, gray.ye, DT_NTYPE_B08);	// same size as gray image
|:  dtimage_constant(NULL, &seg, 1);	// make one giant blob of 1's
|:  dtimage_hist_alloc(NULL, &gray,		// space for histogram
|:    &hist);
|:  dtimage_hist(NULL, &gray, &hist);	// get histogram of gray image
|:
|:  dtlev85_init(NULL, &lev85);			// initialize lev85 structure
|:  dtlev85_hist(NULL,					// contribute giant blob
|:    &lev85, &hist);
|:  dtlev85_compute(NULL, &lev85);		// compute final uniformity
|:
|:  printf("dtlev85c:"					// print the uniformity
|:    " uniformity %7.5f\n",
|:    lev85.u);
|:  printf("dtlev85c: normal exit\n");
|:}
|
| END
 *..........................................................................*/

dt_rc_e
dtlev85_compute(						/* compute uniformity of area		*/
  dtparam_t *param,						/* control parameter structure 		*/
  dtlev85_t *lev85)						/* algorithm housekeeping			*/
{
  DT_F("dtlev85_compute");
  dt_ctl_t *ctl = param? 
    &param->ctl: NULL;

  if (lev85->n > 0)						/* got some regions?			    */
  {
	double fdiff;
	double varjmax;
    fdiff = lev85->fmax - lev85->fmin;
	varjmax = fdiff * fdiff / 2.0;		/* maximum variance					*/

    dt_dbg(ctl, F, DT_DBG_MASK_INTERMD,
      "Ua %8.2e, Aa %6ld,"
      " fmin %g, fmax %g,"
      " varjmax %8.2e",
      lev85->Ua, lev85->Aa,
      lev85->fmin, 
      lev85->fmax, 
      varjmax);

    if (varjmax != 0.0)					/* there is some variation?		    */
      lev85->u = 1.0 - lev85->Ua /		/* normalize and unitize			*/
        (double)lev85->Aa / varjmax;
    else								/* no variation among all pixels?	*/
      lev85->u = 1.0;
  }
  else									/* no regions?					    */
    lev85->u = 0.0;

  return DT_RC_GOOD;
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
