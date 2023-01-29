/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#ifdef NEED_LOG
/*..........................................................................
 *..........................................................................*/

static
void
_log_va(
  const char *format,
  va_list arglist)
{
  char s[80];
  FILE *logfile;
  time_t clock = time(NULL);			/* current time 					*/
  struct tm *t = localtime(&clock);		/* format into fields 				*/

  sprintf(s, "tabchief.out");			/* open log file 					*/
  logfile = fopen(s, "at");

  strftime(s, sizeof(s),				/* express time stamp as string		*/
    "%Y%m%d.%H%M.%S", t);
  fprintf(logfile, "%s: ", s);			/* emit time stamp 					*/

  vfprintf(logfile, format,				/* format error message 			*/
    arglist);	

  fprintf(logfile, "\n");
  fclose(logfile);
}

/*..........................................................................
 *..........................................................................*/

static
void
_log(
  const char *format,
  ...)
{
  va_list arglist;
  va_start(arglist, format);
  _log_va(format, arglist);
  va_end(arglist);
}
#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
