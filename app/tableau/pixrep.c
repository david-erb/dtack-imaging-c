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

#include <dtack/base.h>
#include <dtack/ipc.h>
#include <dtack/os.h>
#include <dtack/str.h>
#include <dtack/lut8.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/scr.h>
#include <dtack/xchg.h>

#include <tableau.h>

DT_RCSID("test/tableau $RCSfile: pixrep.c,v $ $Revision: 1.8 $");

dt_rc_e
DTCONFIG_API1
tableau_pixrep(
  dt_ctl_t *ctl,
  dtimage_t *input,
  int factor,
  dtimage_t *output)
{
  const dtxy_t ye = input->ye;
  const dtxy_t xe = input->xe;
  dtxy_t x, y;

  for (y=0; y<ye; y++)				/* traverse output image		    */
  {
    const dt_ntype_b08_t * const p =
      input->row[y].b08;
	dt_ntype_b08_t * const qc1 = 
      output->row[y*2].b08;
	dt_ntype_b08_t * const qc2 =
      output->row[y*2+1].b08;
	dtxy_t x2 = 0;
	for (x=0; x<xe; x++)
	{
	  qc1[x2] = qc1[x2+1] =
	  qc2[x2] = qc2[x2+1] = p[x];
	  x2 += 2;
	}
  }
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
