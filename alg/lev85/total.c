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
DT_RCSID("alg/lev85 $RCSfile: total.c,v $ $Revision: 1.9 $");
#include <dtlev85.h>

static dtrun_deliver_f dtlev85_total_deliver;

typedef struct {
  dtlev85_t *lev85;
  dtimage_t *gray;
} myarg_t;

/*..........................................................................
| NAME
| dtlev85_total() - compute total uniformity
| 
| DESCRIPTION
| dtlev85_total() computes a single uniformity number characterizing
| all non-background pixels in an image.
|_index uniformity,, total
|_index total uniformity,, computing
|
| The regions defined by 4- or 8-connected pixels in the ~segmented~ image 
| are extracted by connected components analysis.
| Using the corresponding locations in the ~gray~ image,
| total uniformity ~lev85->u~ is computed.
| The number of regions is returned in ~lev85->n~.
| Regions in ~segmented~ whose values are ~background~ are ignored.
|
| The total uniformity represents the uniformity of an area
| which is composed of all non-background regions in the image.
| 
| If no pixel in the ~segmented~ image has the value ~background~,
| then all pixels in the image contribute to the uniformity.
| This is desirable in some applications.
| 
| PARAMETERS
| Besides ~param->ctl~,
| the following member of the ~param~ structure is the only one used
| by this function:
| ~param->connect~	component connectivity.
|					If specified, must be either "4" or "8".
|					Default if unspecified is "4".
|_index param, connect, component connectivity
|dtinclude(params0.dx)
|
| RETURN VALUES
| The total uniformity is returned in ~lev85->u~.
| The number of regions contributing to the uniformity is returned in
| ~lev85->n~.
| The sum of the areas of all the contributing regions is returned in
| ~lev85->Aa~.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - ~gray~ image does not have a valid data type
| - ~segmented~ image does not have a valid data type
| - unable to allocate space for the temporary histogram
| - error during connected components analysis
| 
| CAVEATS
| The ~background~ value refers to a particular pixel value in
| the ~segmented~image, not one from the ~gray~ image.
| Any pixels in the ~segmented~ image whose values are equal to
| ~background~ are silently ignored.
| 
| SEE ALSO
| dtlev85_label()		calculate uniformity for individual regions
|
| EXAMPLE lev85a.c
|:// Example Program dtlev85a
|:// Illustrates dtlev85_total().
|:// Three arguments are: 1) the gray level image filename, 
|:// 2) the segmented image filename, and 3) the background value
|://     dtlev85a  data/dice64.pgm  data/dice64s.pgm  0
|:// total uniformity of non-background regions is computed and printed
|:// all error checking left out for clarity
|:
|:#include <dtack/base.h>
|:#include <dtlev85.h>
|:
|:void main(int argc, char *argv[])
|:{
|:  dtimage_t gray;
|:  dtimage_t segmented;
|:  double background;
|:  dtlev85_t lev85;
|:
|:  dtimage_import(NULL,		 		// read and allocate gray image
|:    &gray, argv[1]);
|:  dtimage_import(NULL,		 		// read and allocate segmented image
|:    &segmented, argv[2]);
|:  background = atof(argv[3]);			// get background from command line
|:
|:  dtlev85_total(NULL, &gray,			// compute total uniformity
|:    &segmented, background, &lev85);
|:
|:  printf("dtlev85a: uniformity %0.5f"	// print the uniformity
|:    " over %ld regions covering"
|:    " %ld%% of the image area\n",
|:    lev85.u, lev85.n,
|:    (100L * lev85.Aa) /
|:    ((long)gray.xe * (long)gray.ye));
|:  printf("dtlev85a: normal exit\n");
|:}
|
| END
 *..........................................................................*/

dt_rc_e
dtlev85_total(							/* total non-background uniformity	*/
  dtparam_t *param,						/* control parameter structure 		*/
  dtimage_t *gray,						/* input gray gray-level image		*/
  dtimage_t *segmented,					/* input segmented image			*/
  double background,					/* background in segmented image    */
  dtlev85_t *lev85)						/* returned structure 				*/
{
  static char *F = "dtlev85_total";
  dt_ctl_t *ctl = param? 
    &param->ctl: NULL;
  const char *connect =
    DTPARAM_GET_VALUE(connect, "4");
  myarg_t myarg;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    gray, "gray",
    DTLEV85_GRAY_NTYPE_LIST,			/* gray ntypes are constrained		*/
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    segmented, "segmented",
    NULL,								/* segmented ntype can be any 		*/
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_Q(dtlev85_init,(param, lev85));

  myarg.lev85 = lev85;					/*[Prepare communication structure.]*/
  myarg.gray = gray;
  
  DT_C(dtimage_adj_blob,(ctl,			/*+Do connected components 		   ]*/
    segmented,							/*[analysis.					   ]*/
    0, 0,								/*[Only use parts of images not    ]*/
    DT_MIN(gray->xe, segmented->xe),	/*[outside the smaller of the two  ]*/
    DT_MIN(gray->ye, segmented->ye),	/*[input images. 				   ]*/
    background,
    DTIMAGE_ADJ_FLAG_NOT |				/*[Ignore background pixels.	   ]*/
	(connect[0] == '4'?				
       DTIMAGE_ADJ_FLAG_4:
       DTIMAGE_ADJ_FLAG_8),
    dtlev85_total_deliver, &myarg));	/*[Call our function for each blob.]*/

  DT_C(dtlev85_compute,(param, lev85));	/*+Compute total uniformity.	   ]*/

cleanup:

  return rc;
}

/*..........................................................................
| called for each blob
| simply adds blob since we know the blob is good 
| run_scan has already ignored the invisible background pixels
 *..........................................................................*/
static
dt_rc_e
dtlev85_total_deliver(	
  dt_ctl_t *ctl,
  dtrun_t *run,
  dtrun_blob_t *blob,
  void *arg)
{
  DT_F("dtlev85_total_deliver");
  myarg_t *myarg = (myarg_t *)arg;		/* extricate our argument 			*/
  dt_stats_t stats;

  stats.have = 0;						/* no stats computed yet 			*/
  stats.want = 0;						/* don't want anything extra 		*/

  DT_Q(dtlev85_blob,(					/* simply add blob to uniformity 	*/
    myarg->lev85->param, 
    myarg->lev85, 
    myarg->gray,
    run, blob, &stats));

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
