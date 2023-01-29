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
| main program
| 
| END
 *..........................................................................*/

#include <tableau.h>

DT_RCSID("app/tableau $RCSfile: encode.c,v $ $Revision: 1.8 $");

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tableau_run_stats(
  dtparam_t *param,
  dtimage_t *input,
  dthist_t *fhist,
  dthist_t *lhist)
{
  DT_F("tableau_run_stats");
  dt_ctl_t *ctl = &param->ctl;
  dthist_bin_t bin1, bin2;
  dtxy_t y;
  dt_rc_e rc = DT_RC_GOOD;

  for (y=0; y<input->ye; y++)			/* for all rows in image		    */
  {
    void *p1;
	dtxy_t x0, x;
	const dtxy_t xmax = input->xe;

    DTIMAGE_GETROW(ctl, input, y, &p1);	/* access row 						*/

	x0 = 0;
	for (x=x0+1; x<=xmax; x++)
	{
	  const dt_ntype_b08_t *p = 
        (dt_ntype_b08_t *)p1;
	  if (x==xmax || p[x] != p[x0])		/* run starts here?				    */
	  {
	    DTHIST_VALUE_TO_BIN(DT_C, ctl,	/* find bin for gray level 			*/
          fhist, (int)p[x0]-128, 
          &bin1);
        fhist->freq[bin1]++;			/* accumulate run counts 			*/

		DTHIST_VALUE_TO_BIN(DT_C, ctl,	/* find bin for run length 			*/
          &lhist[bin1], x-x0-1, &bin2);
		lhist[bin1].freq[bin2]++;			

		x0 = x;							/* remember start of this run 		*/
	  }
	}
    DTIMAGE_UNMAPROW(ctl, input, y);	/* no change to the input image	    */
  }

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
