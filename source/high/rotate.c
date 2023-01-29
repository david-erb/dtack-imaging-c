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
DT_RCSID("source/high $RCSfile: transpos.c,v $ $Revision: 1.1 $");
#include <dtack/mem.h>

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtimage_rotate(
  dt_ctl_t *ctl,
  dtimage_t *input,
  double angle,							/* angle in degrees 				*/
  dtimage_t *output)
{
  DT_F("dtimage_rotate");
  dt_ntype_e ntypes[] = {				/* we can handle any ntype		    */
	DTNTYPE_B08,						/* 8-bit unsigned binary			*/
	DTNTYPE_B16,						/* 16-bit unsigned binary			*/
	DTNTYPE_B32,						/* 32-bit unsigned binary			*/
	DTNTYPE_S1,							/* 8-bit signed integer			    */
	DTNTYPE_S2,							/* 16-bit signed integer		    */
	DTNTYPE_S4,							/* 32-bit signed integer		    */
	DTNTYPE_F1,							/* single precision float		    */
	DTNTYPE_F2,							/* double precision float		    */
	DTNTYPE_NULL
  };
  const dtxy_t xe = input->xe;		
  const dtxy_t ye = input->ye;
  dtxy_t x;
  dtxy_t y = 0;
  unsigned int i, n;
  char *p = NULL;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input, "input",
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
	return rc;

  if (output->xe != 0)
	return dt_err(ctl, F,
      "pre-allocated output not"
      " supported at this time");

  angle = fmod(angle, 360.0);
  if (angle < 0.0)
	angle = 360.0 + angle;

  if (angle != 0.0 &&
	  angle != 90.0 &&
	  angle != 180.0 &&
	  angle != 270.0)
	return dt_err(ctl, F,
	  "angle %g not supported"
      " (only 0, 90, 180, and 270)");

  if (angle == 0.0)
  {
	DT_C(dtimage_dup,(ctl, input, 
      output));
	goto cleanup;
  }

  if (angle == 90.0 || angle == 270.0)
    DT_C(dtimage_create,(ctl, output,	/* create the new image 			*/
      ye, xe, input->ntype))
  else
    DT_C(dtimage_create,(ctl, output,
      xe, ye, input->ntype))

  for (y=0; y<input->ye; y+=n)			/* for each input row 				*/
  {
	DTIMAGE_GETROWS2(DT_C, ctl,			/* access the next block of rows 	*/
      input, y, &p, &n);
	n /= input->rowsize;				/* number of rows in block 			*/
	for (i=0; i<n; i++)					/* for each row in block 			*/
	{
	  if (angle == 90.0)
		DT_Q(dtimage_col_put,(ctl,
          output, output->xe-(y+i)-1, 
          (void *)p, 1))
	  else
	  if (angle == 270.0)
		DT_Q(dtimage_col_put,(ctl,
          output, y+i, 
          (void *)p, -1))
	  else								/* 180 degree rotation? 			*/
	  {
		dtxy_t r = output->ye-(y+i)-1;
		void *q;
		dtxy_t x;
		DTIMAGE_GETROW2(DT_C, ctl,		/* access the proper output row		*/
          output, r, &q);
		switch(input->ntype)
		{
  		  case DT_NTYPE_B08:
  		  case DT_NTYPE_S1:
		    for (x=0; x<xe; x++)
		      ((char *)q)[x] = ((char *)p)[xe-x-1];
		  break;
  		  case DT_NTYPE_B16:
  		  case DT_NTYPE_S2:
		    for (x=0; x<xe; x++)
		      ((short *)q)[x] = ((short *)p)[xe-x-1];
		  break;
  		  case DT_NTYPE_B32:
  		  case DT_NTYPE_S4:
  		  case DT_NTYPE_F1:
		    for (x=0; x<xe; x++)
		      ((long *)q)[x] = ((long *)p)[xe-x-1];
		  break;
  		  case DT_NTYPE_F2:
		    for (x=0; x<xe; x++)
		      ((double *)q)[x] = ((double *)p)[xe-x-1];
		  break;
		}
		DTIMAGE_PUTROW2(DT_C, ctl,		/* release output row				*/
		  output, r);
	  }
	  p += input->rowsize;
	}
	p = NULL;
	DTIMAGE_UNMAPROW2(DT_C, ctl,		/* release the block of rows 		*/
      input, y);
  }
  
cleanup:
  if (p != NULL)
	DTIMAGE_UNMAPROW2(DT_I, ctl, input, y);
  
  if (rc != DT_RC_GOOD &&				/* operation failed? 				*/
	  output->xe != 0)					/* but image was allocated? 		*/
	DT_I(dtimage_free,(ctl, output));

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
