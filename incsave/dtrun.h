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
| include $RCSfile: dtrun.h,v $ $Revision: 1.16 $
 *..........................................................................*/

										/* ................................ */

#define DTRUN_LEN_NTYPE DTNTYPE_S4
typedef dtntype_s4_t dtrun_len_t;		/* image run lengths,				*/
										/* negative lengths are runs of 0's	*/

#define DTRUN_NOID ((dtrun_id_t)(-1))

#define DTRUN_ID(I) ((dtrun_id_t)(I))

#define DTRUN_PEEK(CHECK, CTL, RUN, P, T)                              \
  DTSTACK_PEEK1(CHECK, CTL, (dtstack_t *)(RUN)->stack,                 \
    (long)(P), (void **)(T));

#define DTRUN_POKE(CHECK, CTL, RUN, P, T)                              \
  DTSTACK_POKE1(CHECK, CTL, (dtstack_t *)(RUN)->stack,                 \
    (long)(P), (void **)(T));

#define DTRUN_LOCK(CHECK, CTL, RUN, P, T)                              \
  DTSTACK_LOCK1(CHECK, CTL, (dtstack_t *)(RUN)->stack,                 \
    (long)(P), (void **)(T));

#define DTRUN_UNLOCK(CHECK, CTL, RUN, P)                               \
  DTSTACK_UNLOCK1(CHECK, CTL, (dtstack_t *)(RUN)->stack,               \
    (long)(P));

typedef long *dtrun_id_t;

typedef struct dtrun_blob_t {
  double label;
  dtrun_id_t phead;
  long count;
  struct {
    struct dtrun_blob_t *head;
    struct dtrun_blob_t *next;
    struct dtrun_blob_t *prev;
  } status;
} dtrun_blob_t;

typedef struct {
  dtxy_small_t x, y, l;
  dtrun_blob_t *blob;
  dtrun_id_t p;
} dtrun_elem_t;

#define DTRUN_DELIVER_ARGS \
  dt_ctl_t *ctl, \
  dtrun_t *run, \
  dtrun_blob_t *blob, \
  void *arg

typedef struct dtrun_t {
  void *stack;							/* really dtstack_t				    */
  dtrun_id_t head;						/* head of run chain			    */
  unsigned long runs;					/* total number of runs			    */
  unsigned long blobs;					/* number of blobs after reassign	*/
} dtrun_t;

typedef 
dt_rc_e
DTCONFIG_API1
dtrun_deliver_f(DTRUN_DELIVER_ARGS);

extern dt_ntype_e *dtrun_ntype_list;	/* list of ntypes we can do 		*/

										/* ................................ */
#include <dtack/run.p>						/* function prototypes				*/









/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
