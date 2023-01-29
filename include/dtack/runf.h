/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




typedef struct dtrunf_elem_t {
  dtxy_small_t x, y, l;
  struct dtrunf_elem_t *h, *p;
} dtrunf_elem_t;

typedef dtrun_len_t dtrunf_len_t;		/* image run lengths,				*/
										/* negative lengths are runs of 0's	*/

typedef struct {
  dtrunf_elem_t **row;
  unsigned int *n;						/* number of runs on each row	    */
  dtrunf_elem_t *head;					/* head of run chain			    */
  unsigned long runs;					/* total number of runs			    */
  unsigned long segments;				/* number of segments to hold runs	*/
  unsigned long segsize;				/* size of each segment in bytes    */
  unsigned long joins;					/* number of joins during labelling	*/
  unsigned long reassigns;				/* number of reassigns			    */
  unsigned long blobs;					/* number of blobs after reassign	*/
} dtrunf_t;

#include <dtack/runf.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
