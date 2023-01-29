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
| include $RCSfile: dtos.h,v $ $Revision: 1.8 $
 *..........................................................................*/

typedef struct {
  double user;							/* user time used by process	    */
  double sys;							/* system time used by process	    */
  double wall;							/* wall clock time				    */
} dtos_time_t;

#ifdef DTCONFIG_IMPLIED_DOS16
#  define DTOS_PTR_TO_ULONG(P) \
     ((unsigned long)(FP_SEG((void *)(P))) * 16UL + \
      (unsigned long)(FP_OFF((void *)(P))))
#else
#  define DTOS_PTR_TO_ULONG(P) \
     ((unsigned long)(P))
#endif
										/* ................................ */
typedef struct {
  void *ptr;
  long size;
  const char *who;
  const char *why;
} dtos_heap_table_t;

typedef enum {
  DTOS_HEAP_OPTION_NO_NEARHEAP = 1
} dtos_heap_option_e;

										/* ................................	*/
#include <dtack/os.p>





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
