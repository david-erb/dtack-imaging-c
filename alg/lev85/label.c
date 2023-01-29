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
DT_RCSID("alg/lev85 $RCSfile: label.c,v $ $Revision: 1.9 $");
#include <dtlev85.h>
#include <dtack/stack.h>
#include <dtack/mem.h>
#include <dtack/os.h>

static dtrun_deliver_f dtlev85_label_deliver;

typedef struct {
  dtparam_t *param;
  dtimage_t *gray;
  dtlev85_t *lev85;
} myarg_t;
  
/*..........................................................................
| NAME
| dtlev85_label() - uniformity for each label
| 
| DESCRIPTION
| dtlev85_label() computes a uniformity number for each unique
| label found in a segmented image.
|_index uniformity,, by label
|_index label uniformity,, computing
|
| Uniformity values are computed using ~gray~ image pixels from
| regions defined by 4- or 8-connected pixels in the ~segmented~ image.
| The caller-supplied ~callback~ routine is called
| once for each different label found in the ~segmented~ image.
| The value of ~callback_arg~ is passed unchanged to ~callback~.
|
| You should write a callback function with a prototype like:
|
|:  dt_rc_e
|:  dtlev85_label_callback(				// caller-provided prototype	   
|:    dt_ctl_t *ctl,					// environment control structure 	
|:    void *callback_arg,				// passed through from application 	
|:    dtlev85_t *lev85)					// input computed uniformity 		
| 
| The ~lev85~ structure passed to the callback routine represents
|_aside You write a custom callback.
| the uniformity of an area comprised of all regions whose pixel
| value in the ~segmented~ image was ~label~.
| 
| The uniformity measure is passed to ~callback~ in ~lev85->u~.
| The number of regions contributing to the uniformity is passed in
| ~lev85->n~.
| The total number of pixels in all regions in the area is passed in
| ~lev85->Aa~.
| The segmented label of the area is passed in ~lev85->label~.
| 
| Consider the small images below in which a value of ':..:'
| indicates some don't-care pixel value.
| Three calls would be made to the ~callback~ routine.
| label ':..:':	one region, gray values of ':..:'
| label 11:		two regions, gray values in the 20's and 30's
| label 22:		one region, gray values in the 40's
| label 66:		one region, gray values in the 70's
| 
|:    segmented image (labels)                 gray level image
|:  .. .. .. .. .. .. .. .. .. ..       .. .. .. .. .. .. .. .. .. ..
|:  .. 11 11 11 .. .. 22 22 22 ..       .. 23 24 23 .. .. 45 47 43 ..
|:  .. 11 11 11 .. .. 22 22 22 ..       .. 25 25 24 .. .. 45 46 44 ..
|:  .. 11 11 11 .. .. 22 22 22 ..       .. 23 26 24 .. .. 46 44 45 ..
|:  .. .. .. .. .. .. .. .. .. ..       .. .. .. .. .. .. .. .. .. ..
|:  .. 66 66 66 .. .. 11 11 11 ..       .. 73 74 73 .. .. 35 37 33 ..
|:  .. 66 66 66 .. .. 11 11 11 ..       .. 75 75 74 .. .. 35 36 34 ..
|:  .. 66 66 66 .. .. 11 11 11 ..       .. 73 76 74 .. .. 36 34 35 ..
|:  .. .. .. .. .. .. .. .. .. ..       .. .. .. .. .. .. .. .. .. ..
|
| If ~gray~ and ~segmented~ are not the same size,
| then the minimum dimensions are used.
| Pixels in the larger image are ignored.
| Pixel 0,0 in ~gray~ is aligned with pixel 0,0 in ~segmented~.
| 
| This routine is most useful when the labels in the segmented
| image truly represent distinct regions for which a uniformity
| measure is meaningful.
| These regions need not be geometrically proximate:
| multiple disconnected regions, if given the same label,
| will all contribute to one uniformity.
| 
| Since this routine passes no geometric location information to the
| ~callback~ routine, it is not suitable for applications
| which must access the region's pixels on which the uniformity
| is being calculated.
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
| There are no return values.
| If desired, the callback routine may values in the ~callback_arg~
| structure for passing back to the calling routine.
|dtinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - ~gray~ image does not have a valid data type
| - ~segmented~ image does not have a valid data type
| - unable to allocate space for the temporary lev85 structures
| - invalid ~param->connect~
| - error during connected components analysis
|
| CAVEATS
| No geometric information about the regions is made available
| to the ~callback~ routine.
|
| Don't confuse the callback used by this function with the
| callback used by the connected components processing.
| The callback used by this function is done on a per-label basis,
| whereas the connected components callback is done
| on a connected-pixel basis.
|
| EXAMPLE lev85b.c
|:// Example Program dtlev85b
|:// Illustrates dtlev85_label().
|:// Two arguments are: 1) the gray level image filename, and
|:// 2) the segmented image filename, for example:
|://     dtlev85b  data/dice64.pgm  data/dice64s.pgm
|:// Uniformity of each label is computed and printed.
|:// All error checking left out for clarity.
|:
|:#include <dtack/base.h>
|:#include <dtlev85.h>
|:
|:dt_rc_e mycallback(					// callback routine just prints
|:  dt_ctl_t *ctl,		
|:  void *callback_arg,	
|:  dtlev85_t *lev85)	
|:{
|:  printf(
|:    "dtlev85b: label %g: uniformity %0.5f"
|:    " over %ld regions"
|:    " of total area %ld\n", 
|:    lev85->label,
|:    lev85->u, lev85->n, lev85->Aa);
|:  return DT_RC_GOOD;
|:}
|:
|:void main(int argc, char *argv[])
|:{
|:  dtimage_t gray;
|:  dtimage_t segmented;
|:  dtimage_import(NULL,		 		// read and allocate gray image
|:    &gray, argv[1]);
|:  dtimage_import(NULL,		 		// read and allocate segmented image
|:    &segmented, argv[2]);
|:
|:  dtlev85_label(NULL, &gray,			// compute uniformity by labels
|:    &segmented, mycallback, NULL);	// call mycallback for each label
|:  printf("dtlev85b: normal exit\n");
|:}
| 
| SEE ALSO
| dtlev85_total()		calculate single uniformity for all regions
| dtlev85_blob()		blob-by-blob uniformity
|
| END
 *..........................................................................*/

dt_rc_e
dtlev85_label(							/* compute by-label uniformity	    */
  dtparam_t *param,						/* control parameter structure 		*/
  dtimage_t *gray,						/* input gray-level image			*/
  dtimage_t *segmented,					/* input segmented image			*/
  dtlev85_label_callback_f callback,	/* function called for each label 	*/
  void *callback_arg)					/* arg passed through to callback 	*/
{
  static char *F = "dtlev85_label";
  dt_ctl_t *ctl = param? 
    &param->ctl: NULL;
  const char *connect =
    DTPARAM_GET_VALUE(connect, "4");
  myarg_t myarg;
  dt_rc_e rc;

  if (strcmp(connect, "4") &&			/*[Check ~param->connect~. 		   ]*/
	  strcmp(connect, "8"))
	return dt_err(ctl, F,
      "invalid param->connect \"%s\"",
      connect);

  rc = dtimage_check_1f(ctl,			/*[Check input image validity.	   ]*/
    gray, "gray",
    DTLEV85_GRAY_NTYPE_LIST,			/*[Gray ntypes are constrained.	   ]*/
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  rc = dtimage_check_1f(ctl,			
    segmented, "segmented",
    NULL,								/*[Segmented ntype can be any.	   ]*/
    DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  memset(&myarg, 0, sizeof(myarg));		/*+Prepare structure for private   ]*/
  myarg.param = param;					/*[communication with blob		   ]*/
  myarg.gray = gray;					/*[callback.	   				   ]*/
  
  DT_C(dtimage_adj_blob,(ctl,			/*+Do connected components 		   ]*/
    segmented,							/*[analysis.					   ]*/
    0, 0,								/*[Only use parts of images not    ]*/
    DT_MIN(gray->xe, segmented->xe),	/*[outside the smaller of the two  ]*/
    DT_MIN(gray->ye, segmented->ye),	/*[input images. 				   ]*/
    0.0,								/*[Ignore no pixel values. 		   ]*/
	connect[0] == '4'?				
      DTIMAGE_ADJ_FLAG_4:
      DTIMAGE_ADJ_FLAG_8,
    dtlev85_label_deliver, &myarg));	/*[Call our function for each blob.]*/

										/*+After all blobs are examined,   ]*/
  while (myarg.lev85 != NULL)			/*[go through list of lev85's.	   ]*/
  {
	dtlev85_t *lev85 = myarg.lev85;
	DT_C(dtlev85_compute,(param,		/*[Compute uniformity for label,   ]*/
      lev85));
    DT_G(callback,(ctl,					/*[and give uniformity to caller's ]*/
      callback_arg, lev85));			/*[callback. 					   ]*/
	if (rc == DT_RC_STOP)				/*[Stop scanning if callback	   ]*/
	{									/*[returns DT_RC_STOP. 			   ]*/
      rc = DT_RC_GOOD;
	  goto cleanup;
	}
	myarg.lev85 = lev85->next;
    DT_C(dtos_free2,(ctl, lev85,		/*[Free each lev85 structure from  ]*/
      F, "lev85 structure"));			/*[list as it is processed. 	   ]*/
  }
  
cleanup:								/*+After processing list, 		   ]*/
  if (rc == DT_RC_GOOD)
  while (myarg.lev85 != NULL)			/*[go through any remaining list,  ]*/
  {
	dtlev85_t *lev85 = myarg.lev85;
	myarg.lev85 = lev85->next;
    DT_I(dtos_free2,(ctl, lev85,		/*[freeing lev85 structures.	   ]*/
      F, "lev85 structure"));
  }

  return rc;
}

/*..........................................................................
| called for each blob
| first searches list of uniformity structures to see if this label
| is already started
| creates a uniformity if none already exists for this label,
| otherwise adds blob to existing uniformity
 *..........................................................................*/

static
dt_rc_e
dtlev85_label_deliver(	
  dt_ctl_t *ctl,
  dtrun_t *run,
  dtrun_blob_t *blob,
  void *arg)
{
  DT_F("dtlev85_label_deliver");
  myarg_t *myarg = (myarg_t *)arg;		/* extricate our argument 			*/
  dtlev85_t *lev85;
  dt_stats_t stats;

  for (lev85 = myarg->lev85;			/* search all lev85's so far 		*/
	   lev85 != NULL;
	   lev85 = lev85->next)
    if (lev85->label ==					/* found previous lev85 with label?	*/
		blob->label)		
      break;

  if (lev85 == NULL)					/* found no lev85 with this label? 	*/
  {
	DT_Q(dtos_malloc2,(ctl,				/* make a new lev85 structure 		*/
      (void **)&lev85,
      sizeof(*lev85),
      F, "lev85 structure"));
    DT_Q(dtlev85_init,(myarg->param,	/* init new lev85 structure 		*/
      lev85));
	lev85->label = blob->label;
	lev85->next = myarg->lev85;			/* link new structure at list head 	*/
	myarg->lev85 = lev85;
  }

  stats.have = 0;						/* no stats computed yet 			*/
  stats.want = 0;						/* don't want anything extra 		*/
  
  DT_Q(dtlev85_blob,(					/* simply add blob to this lev85 	*/
    myarg->param, 
    lev85, 
    myarg->gray, 
    run, blob,
    &stats));

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
