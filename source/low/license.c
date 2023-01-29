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
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: license.c,v $ $Revision: 1.1 $");
#include <dtack/licens.h>
#include <dtack/str.h>
#include <dtack/os.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlicense_request(						/* verify license					*/
  dt_ctl_t *ctl,
  dtlicense_t *license,
  const char *dirname,
  const char *product)
{
  DT_F("dtlicense_request");
  char filename[256];
  int isreadable;

  dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
    "dirname=\"%s\", product=\"%s\"",
    dirname==NULL? "NULL": dirname, 
    product==NULL? "NULL": product);

#if DTPRJ_FLEXLM == DT_1

  if (dirname == NULL)
	dirname = ".";
  DT_Q(dtstr_printf,(ctl,				/* compose license filename 		*/
    filename, sizeof(filename),
    "%s/dtlicens.dat", dirname));
  DT_Q(dtos_isreadable,(ctl, filename,	/* see if license file exists 		*/
    &isreadable));
  if (!isreadable)
	return dt_err(ctl, F, "missing license file %s", filename);

  if (CHECKOUT(LM_RESTRICTIVE, product, "1.0", filename))
	return dt_err(ctl, F, "%s", ERRSTRING());
#endif
  
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlicense_release(						/* release license					*/
  dt_ctl_t *ctl,
  dtlicense_t *license)
{
  DT_F("dtlicense_release");

#if DTPRJ_FLEXLM == DT_1
  CHECKIN(); 
#endif
  
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
