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
| The :prj: Function Group
| 
|!Summary
| The :dtprj: functions control access to project configuration.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: prj.c,v $ $Revision: 1.2 $");
#include <dtack/prj.h>

/*..........................................................................
| NAME
| dtprj_param_default - look up param default
| 
| SUMMARY
| Compare given ~param~ value against all entries in the project param
| defaults table.
| If found, return pointer to default value.
| Otherwise return NULL.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtprj_param_default(					/* look up param default string		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *param,					/* param string with leading "-"	*/
  const char **value)					/* returned param value				*/
{
  dtprj_param_defaults_t *d =			/* for short 						*/
	dtprj_param_defaults;
  int i;

  *value = NULL;						/* presume we don't find it 		*/

  for (i=0; d[i].abbrev; i++)			/* search project defaults table 	*/
  {
	if (!strcmp(param, d[i].abbrev))
	{
	  *value = d[i].dflt;
	  break;
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
