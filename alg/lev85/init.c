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
DT_RCSID("alg/lev85 $RCSfile: init.c,v $ $Revision: 1.9 $");
#include <dtlev85.h>

/*..........................................................................
| NAME
| dtlev85_init() - initialize structure
| 
| DESCRIPTION
| dtlev85_init() initializes a dtlev85 structure
| in preparation for accumulating regions into it.
|_index dtlev85 structure,, initialization
|_index initialization,, of dtlev85 structure
|
| The ~lev85~ structure is initialized.
| In particular the ~lev85->param~ member is set to the value
|_aside Determines ~param~ structure.
| of the ~param~ argument.
| This is the structure which will be used in later callback routines.
| 
| Initializing a dtlev85 structure means that the next call to
| dtlev85_blob() or dtlev85_hist()
| will be adding the first region to it.
| 
| This function allocates no resources.
|_aside Allocates no resources.
| There is no corresponding dtlev85_uninit() function.
| 
| PARAMETERS
|dtinclude(ctlonly.dx)
|
| RETURN VALUES
| The contents of the dtlev85 structure is cleared and
| certain members are also given initial values.
|dtinclude(return1.dx)
| 
| ERRORS
|dtinclude(noerror.dx)
| 
| CAVEATS
| Attempting to use a ~lev85~ structure
| in subsequent routines without first initializing it with dtlev85_init()
| may result in incorrect results.
| 
| SEE ALSO
| dtlev85_contribute()		contributes a region to the area
| dtlev85_compute()			computes uniformity for the area
| dtlev85_combine()			combines two areas into a single area
|
| EXAMPLE
| For two examples using dtlev85_init(), please refer to the
|_hyper dtlev85_compute()
| and
|_hyper sections, Example lev85e.c
|
| END
 *..........................................................................*/

dt_rc_e
dtlev85_init(							/* init structure					*/
  dtparam_t *param,						/* control parameter structure 		*/
  dtlev85_t *lev85)						/* array of structures to init 		*/
{
  memset(lev85, 0, sizeof(*lev85));		/*[Clear structure to zeros. 	   ]*/

  lev85->param = param;					/*[Assign param member.			   ]*/
  
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
