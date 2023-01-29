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

#include <dtack/base.h>
#include <dsched.h>
DT_RCSID("app/tabchief $RCSfile: load.c,v $ $Revision: 1.8 $");

#ifdef DTCONFIG_IMPLIED_UNIXLIKE
#define MODE_READ "r"
#define MODE_WRITE "w"
#endif 

#ifdef DTCONFIG_IMPLIED_MSW
#define MODE_READ "rb"
#define MODE_WRITE "wb"
#endif 

/* ------------------------------------------------------------------------
 * ------------------------------------------------------------------------ */

void
DTCONFIG_API1
dsched_load(							/* load structure from database 	*/
  dsched_t *dsched,
  const char *database)
{
  FILE *file;
  char line[256];
  int n;
  if (ISERROR(dsched))
    return;
  file = dsched_open(dsched, 
    database, MODE_READ);
  if (file == NULL)
    return;
  memset(line, 0, sizeof(line));
  while (fgets(line, 
           sizeof(line), file))
  {
	if (line[strlen(line)-1] == '\n')
	  line[strlen(line)-1] = '\0';
	if (!strncmp(line,					/* stop reading rules at sentinel	*/
		  DSCHED_SENTINEL,
          strlen(line)))
	  break;
    dtstr_tokenize(NULL, line, ' ', &n);
	if (n != 5)
	  dsched_error(dsched, "Sorry, database file \"%s\" is mangled.",
        database);
	else
	{
	  char *day = line;
	  char *start = day + strlen(day) + 1;
	  char *end = start + strlen(start) + 1;
	  char *period = end + strlen(end) + 1;
	  char *duration = period + strlen(period) + 1;
	  dsched_add(dsched, day,
        start, end, period, duration);
	}
	memset(line, 0, sizeof(line));
  }
  fclose(file);
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
