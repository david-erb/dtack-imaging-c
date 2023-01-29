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
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <tabchief.h>
DT_RCSID("app/tabchief $RCSfile: split.c,v $ $Revision: 1.8 $");

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

int
DTCONFIG_API1
dsched_split(							/* separate string into tokens 		*/
  char *p,								/* string changed in place 			*/
  char c)								/* separator character 				*/
{
  char *q = p;
  int i;
  int l = strlen(p) + 1;				/* q needs the EOS too 				*/
  int n = 0;
  for (i=0; i<l; i++)
  {
    if (p[i] == c)
	{
      *q++ = '\0';
	  n++;
	}
	else
    if (p[i] == '+')
	{
      *q++ = ' ';
	}
	else
    if (p[i] == '%' && p[i+1] != '%')
	{
	  char t[3];
	  t[0] = p[i+1];
	  t[1] = p[i+2];
	  t[2] = '\0';
      *q++ = (char)strtol(t, 0, 16);
	  i += 2;
	}
	else
    if (p[i] == '%' && p[i+1] == '%')
	{
      *q++ = '%';
	  i += 1;
	}
	else
	  *q++ = p[i];
  }
  return n + 1;
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
