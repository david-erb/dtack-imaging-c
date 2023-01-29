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

DT_RCSID("source/low $RCSfile: usage.c,v $ $Revision: 1.1 $");

#ifdef DTCONFIG_MICROSOFT
#define FILENO _fileno
#else
#define FILENO fileno
#endif

extern
void
DTCONFIG_API1
dt_dbg_string(							/* no prefix or substitutes			*/
  dt_ctl_t *ctl,
  const char *string,
  int length);

/*..........................................................................*/
static
dt_rc_e
dt_usage_sect(
  dt_ctl_t *ctl,
  const char *beg,
  const char *end,
  const char *string,
  const char *progname,
  dt_dbg_string_f *dbg_string)
{
  const char *s = string;
  const char *e;
  const char *p;
  int lbeg = strlen(beg);
  int lend = strlen(end);
  int in = lbeg == 0;					/* lbeg==0 means automatically in	*/
  int l;
  do {
    e = strchr(s, '\n');
    if (e)
      l = (int)(e - s);
    else
      l = strlen(s);
    if (in)								/* in the interesting section?	    */
	{
      if (l == 0 && lend == 0 ||		/* end trigger is empty line?	    */
          lend > 0 &&					/* or we have an end trigger 		*/
          (p=strstr(s, end)) != NULL &&	/* and it is within this line? 		*/
          p-s < l)			
        break;
	  if (progname != NULL &&
		  strlen(progname) > 0)
	  {
        dbg_string(ctl, progname, -1);
        dbg_string(ctl, ": ", -1);
	  }
      dbg_string(ctl, s, l);
      dbg_string(ctl, "\n", 1);
	}
    else								/* not in the section?			    */
    if (l == lbeg &&					/* found the section we want?	    */
        !strncmp(s, beg, l)) 
      in = 1;
    s = e + 1;
  } while(e && *s);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dt_usage_say - say usage string
| 
| DESCRIPTION
| dt_interactive_usage() outputs to the console all or part of a preformatted
| usage string.
|
| Contrary to its name, this function outputs through the dbg interface.
| In addition, if the dbgto value is unspecified or is stderr,
| then output goes through stdout.
| Is is to make it easy to pipe through more for new users.
| 
| If the substring "SYNOPSIS" is found in ~string~,
| then the ~string~ is assumed to consist of lines separated by
| newline characters and organized into sections.
| Depending on whether ~param->HELP~, ~param->help~, or neither
| has been specified, all or just some of the sections are output.
| 
| However, if the substring "SYNOPSIS" is not found in ~string~,
| then the entire thing is always output
| irregardless of the state of ~param~.
| The word "usage:" is printed before the string
| and a newline character is printed after it.
| 
| Sections are delimited by certain section-beginning key words 
| at the start of a line in ~string~.
| All lines following the key word, up until a line containing
| another key word, are considered part of the section.
| 
| If ~param~ indicates that -HELP is present,
| entire ~string~ is printed.
| 
| If ~param~ indicates that -help is present,
| then the only following sections are printed:
| SYNOPSIS, SUMMARY, ARGUMENTS, and PARAMETERS.
| The word "usage:" is printed before the string.
| The key word beginning the section is omitted except for
| PARAMETERS and ARGUMENTS.
| No parameters beyond -help are printed.
| 
| If ~param~ indicates that neither -HELP nor -help is present,
| then only SYNOPSIS is printed.
| The word "usage:" is printed before the string.
| 
| Called by dtinteractive_parse() when parsing error or when
| -help or -HELP is given.
| Called by dtinteractive_dialog() when dialogs are off and parsing error.
|
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dt_usage_say(							/* say usage string					*/
  dtparam_t *param,
  const char *string)
{
  DT_F("dt_usage_say");
  dt_ctl_t *ctl = &param->ctl;
  dt_dbg_string_f *dbg_string;
  void *old_dbgto;
  const char *progname;
  dt_rc_e rc;

# define SECTION0(BEG, END) \
    DT_C(dt_usage_sect,(ctl, \
      BEG, END, string, \
      progname, dbg_string));
# define SECTION1(BEG, END) \
    dbg_string(ctl, progname, -1); \
    dbg_string(ctl, ": ", -1); \
    dbg_string(ctl, BEG, -1); \
    dbg_string(ctl, "\n", 1); \
    SECTION0(BEG, END);

  if (ctl != NULL)
    old_dbgto = ctl->dbgto;
  
  if (ctl != NULL &&					/* control structure given?		    */
      ctl->dbg_string != NULL)			/* caller has specified function?	*/
    dbg_string = ctl->dbg_string;		/* use caller's function		    */
  else									/* no control or no function?	    */
  {
    dbg_string = dt_dbg_string;			/* use standard function		    */
	if (ctl->dbgto == NULL ||			/* no special output file? 			*/
		ctl->dbgto == stderr)			/* or using stderr? 				*/
      ctl->dbgto = (void *)(long)		/* temporary switch to stdout 		*/
        FILENO(stdout);
  }

  if (ctl &&
	  ctl->progname != NULL)			/* progname is set?				    */
    progname = ctl->progname;			/* use it						    */
  else
    progname = "dt";

  if (strstr(string, "SYNOPSIS") ==		/* string is not dx-generated? 		*/
      NULL)
  {
    dbg_string(ctl, progname, -1);
    dbg_string(ctl, ": usage: ", -1);
    dbg_string(ctl, string, -1);
	dbg_string(ctl, "\n", 1);
  }
  else
  if (param->HELP_flag &				/* -HELP given?					    */
      DTPARAM_FLAG_VALUE)
  {
    SECTION0("", "END OF FILE");		/* use everything				    */
  }
  else
  if (param->help_flag &				/* -help given?					    */
      DTPARAM_FLAG_VALUE)
  {
    dbg_string(ctl, progname, -1);
    dbg_string(ctl, ": usage: ", -1);
    DT_C(dt_usage_sect,(ctl,			/* synopsis without extra progname 	*/
      "SYNOPSIS", "", string,
      NULL, dbg_string));
    SECTION0("SUMMARY", "");			/* pull out short sections		    */
    SECTION1("ARGUMENTS", "");
    SECTION1("PARAMETERS", "-help");	/* skip boring parameters		    */

	  if (progname != NULL)
	  {
        dbg_string(ctl, progname, -1);
        dbg_string(ctl, ": ", -1);
	  }
      dbg_string(ctl, "for more help, please use -HELP (capitalized)", -1);
      dbg_string(ctl, "\n", 1);
  }
  else									/* neither help given			    */
  {
    dbg_string(ctl, progname, -1);
    dbg_string(ctl, ": usage: ", -1);
    DT_C(dt_usage_sect,(ctl,			/* synopsis without extra progname 	*/
      "SYNOPSIS", "", string,
      NULL, dbg_string));
  }
cleanup:
  if (ctl != NULL)
    ctl->dbgto = old_dbgto;
  return DT_RC_STOP;
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
