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
  CHANGES
  19991218 DE changed some long to dtntype_s4_t
 *..........................................................................*/

/*..........................................................................
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: col.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

typedef enum {
  FLAG_PUT,
  FLAG_GET,
} flag_e;

/*..........................................................................*/
#define COL(T)															  	  \
static																		  \
dt_rc_e																		  \
dtimage_col_ ## T(						/* copy data array to image column 	*/\
  dt_ctl_t *ctl,															  \
  dtimage_t *image,						/* image to receive data 			*/\
  dtxy_t col,							/* column number 					*/\
  void *data,							/* data to be copied to column 		*/\
  int dir,								/* traversal direction 				*/\
  flag_e flag)							/* put/get flag 					*/\
{																			  \
  DT_F("dtimage_col_" #T);												  	  \
  const dtxy_t xe = image->xe;												  \
  const dtxy_t ye = image->ye;												  \
  dtxy_t x, y;																  \
  unsigned int i, n;														  \
  T *p = NULL;																  \
  T *q = (T *)data;															  \
  dt_rc_e rc = DT_RC_GOOD;													  \
  if (dir == -1)															  \
    q += ye - 1;															  \
  for (y=0; y<ye; y+=n)					/* for each image row 				*/\
  {																			  \
	DTIMAGE_GETROWS2(DT_C, ctl,			/* access the next block of rows 	*/\
      image, y, &p, &n);													  \
	n /= image->rowsize;				/* number of rows in the block 		*/\
	p += col;							/* offset to column pixel 			*/\
	for (i=0; i<n; i++)					/* for each row in block 			*/\
	{																		  \
      if (flag == FLAG_PUT)													  \
  	    *p = *q;															  \
      else																	  \
      if (flag == FLAG_GET)													  \
  	    *q = *p;															  \
      q += dir;																  \
      p += xe;																  \
    }																		  \
	p = NULL;																  \
	DTIMAGE_UNMAPROW2(DT_C, ctl, 											  \
      image, y);															  \
  }																			  \
cleanup:																	  \
  if (p != NULL)															  \
	DTIMAGE_UNMAPROW2(DT_I, ctl, 											  \
      image, y);															  \
   return rc;																  \
}

COL(char)
COL(short)
COL(dtntype_s4_t)
COL(double)

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_col_put(
  dt_ctl_t *ctl,															  
  dtimage_t *image,						/* image to receive data 			*/
  dtxy_t col,							/* column number 					*/
  void *data,							/* data to be copied to column 		*/
  int dir)								/* traversal direction 				*/
{
  DT_F("dtimage_col_put");
  int size;

  size = (int)dt_ntype_sizeof[			/* size of pixels in bytes 			*/
    image->ntype];
  
  if (size == 0)						/* pixel is not a whole byte? 		*/
	return dt_err(ctl, F,
      "cannot transpose"
      " single-bit images");

  if (size == 1)
	DT_Q(dtimage_col_char,(ctl,
      image, col, data, dir, FLAG_PUT))
  else
  if (size == 2)
	DT_Q(dtimage_col_short,(ctl, 
      image, col, data, dir, FLAG_PUT))
  else
  if (size == 4)
	DT_Q(dtimage_col_dtntype_s4_t,(ctl,
      image, col, data, dir, FLAG_PUT))
  else
  if (size == 8)
	DT_Q(dtimage_col_double,(ctl,
      image, col, data, dir, FLAG_PUT))

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_col_get(
  dt_ctl_t *ctl,															  
  dtimage_t *image,						/* image to receive data 			*/
  dtxy_t col,							/* column number 					*/
  void *data,							/* data to be copied to column 		*/
  int dir)								/* traversal direction 				*/
{
  DT_F("dtimage_col_get");
  int size;

  size = (int)dt_ntype_sizeof[			/* size of pixels in bytes 			*/
    image->ntype];
  
  if (size == 0)						/* pixel is not a whole byte? 		*/
	return dt_err(ctl, F,
      "cannot operate on"
      " single-bit images");

  if (size == 1)
	DT_Q(dtimage_col_char,(ctl,
      image, col, data, dir, FLAG_GET))
  else
  if (size == 2)
	DT_Q(dtimage_col_short,(ctl, 
      image, col, data, dir, FLAG_GET))
  else
  if (size == 4)
	DT_Q(dtimage_col_dtntype_s4_t,(ctl,
      image, col, data, dir, FLAG_GET))
  else
  if (size == 8)
	DT_Q(dtimage_col_double,(ctl,
      image, col, data, dir, FLAG_GET))

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
