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
| TITLE
| Connected Components
|
|!Summary
| Implement a means by which like-valued adjacent pixels
| are collected into blobs.
|
| As each blob is extracted, deliver it to a caller-provided function of the form:
| 
|:  dt_rc_e								// return DT_RC_GOOD, BAD or STOP	
|:  blob_deliver(						// user-provided callback 			
|:    dt_ctl_t *ctl,					// environment control 				
|:    dtrun_t *run,						// run stack structure 				
|:    dtrun_blob_t *blob,				// blob description structure
|:    void *arg)						// passed through without change 	
|
| A blob is represented by the structure:
| 
|:  typedef struct {					// blob description structure 		
|:    double label;						// pixel value of this blob 		
|:    dtrun_id_t p;			   			// first run in the blob 			
|:    long count;						// number of runs in the blob 		
|:  } dtrun_blob_t;
|
| Individual runs are identified by :dtrun_id_t: scalar numbers.
| A run can be referenced using its identifier in a call such as:
| 
|:  DTRUN_PEEK(DT_Q, ctl,				// access run in blob chain 	
|:    run, p, &elem);
| 
| Runs are represented by the structure:
| 
|:  typedef struct {					// run description structure 		
|:    dtxy_small_t x;					// run start column					
|:    dtxy_small_t y;					// run start row					
|:    dtxy_small_t l;					// run length in pixels 			
|:    dtrun_blob_t *blob;				// pointer back to blob 			
|:    dtrun_id_t p;						// link to next run in blob 		
|:  } dtrun_elem_t;
|
| The caller-provided delivery function may traverse the list of runs by
| starting at ~blob->p~ then following the ~elem->p~ links.
| Any desired operation may be performed on the runs.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: adj.c,v $ $Revision: 1.15 $");

/*..........................................................................
| NAME
| dtimage_adj_blob() - connected components extraction
| 
| SUMMARY
| Extract blobs formed by adjacent pixels with identical values.
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
| Choose an efficient means of operation based on computer architecture,
| image size, or other considerations.
| Use any available technique such as run analysis, region filling,
| or region flooding.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_adj_blob(						/* extract blobs					*/
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
  DT_F("dtimage_adj_blob");

  DT_Q(dtimage_adj_run_blob,(ctl,		/* we can only do run blobs 		*/
    input,
    xmin, ymin, xmax, ymax, 
    invisible, flags, 
    deliver, arg));

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
