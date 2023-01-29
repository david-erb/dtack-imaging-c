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
  CHANGES
  20000807 DE added win, yield, and mem to exclude list
  19991211 DE handle "null" value for errto, dbgto and sayto
  19980716 DE remove sunpro1 compiler warnings
 *..........................................................................*/




#include <dtack/base.h>
DT_RCSID("source/inter $RCSfile: enter.c,v $ $Revision: 1.2 $");

#include <dtack/str.h>
#include <dtack/os.h>
#include <dtack/list.h>
#include <dtack/liner.h>

#include <dtack/msw.h>

#ifdef DTCONFIG_MICROSOFT
#define FILENO _fileno
#define OPEN _open
#else
#define FILENO fileno
#define OPEN open
#endif

#define VALUE_OR_DEFAULT \
  (DTPARAM_FLAG_VALUE | \
   DTPARAM_FLAG_DEFAULT)

extern const char *dt_err_prefix;		/* back door into dt_err's output	*/

#ifdef DTCONFIG_BORLAND
#undef DTCONFIG_OPEN_WPERM
# define DTCONFIG_OPEN_WPERM \
    (S_IREAD | S_IWRITE)
#endif

#ifdef DTCONFIG_MICROSOFT
#undef DTCONFIG_OPEN_WPERM
# define DTCONFIG_OPEN_WPERM \
    (S_IREAD | S_IWRITE)
#endif

#ifndef  DTCONFIG_OPEN_ATEXT			/* not defined yet? 				*/
# define DTCONFIG_OPEN_ATEXT \
    (O_WRONLY | \
     O_BINARY | \
     O_APPEND | \
     O_CREAT)
#endif

										/* ................................ */

typedef struct {
  int line;
} feedback_t;
										/* ................................ */

#define FIRST_Y0 (1)

#define LABELWIDTH (20)
#define INPUTWIDTH (17)
#define GUTTER (4)

#define EMPTY_FIELD_CONTENTS ""

#define EXCLUDE_FIELD(ABBREV) \
  (!strcmp(ABBREV, "-dbgto") || \
   !strcmp(ABBREV, "-errto") || \
   !strcmp(ABBREV, "-sayto") || \
   !strcmp(ABBREV, "-dialog") || \
   !strcmp(ABBREV, "-win") || \
   !strcmp(ABBREV, "-yield") || \
   !strcmp(ABBREV, "-mem") || \
   !strcmp(ABBREV, "-keyboard") || \
   !strcmp(ABBREV, "-help") || \
   !strcmp(ABBREV, "-HELP"))

typedef struct {						/* pass-through argument 			*/
  int dummy;
} newfield_arg_t;

typedef struct {						/* arg from param_callback		    */
  int y;
  int i;
  int n;
  int h;								/* if 0, just counting, else cutoff	*/
  int y0;
  int labelwidth;
  int inputwidth;
  dtparam_flag_t flag;
  dt_enter_newfield_f *newfield;
  void *newfield_arg;
} param_arg_t;


/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_enter_newfield_callback(
  dt_ctl_t *ctl,
  void *arg,
  char *abbrev,
  char *desc,
  char *val,
  int x,
  int y,
  int labelwidth,
  int inputwidth)
{
  DT_F("dt_enter_newfield_callback");
  char s[80];	
  if (abbrev &&
	  (!strcmp(abbrev, "-help") ||		/* ignore help fields 				*/
	   !strcmp(abbrev, "-HELP") ||
	   !strcmp(abbrev, "-keyboard")))
    return DT_RC_GOOD;


  dtstr_printf(ctl, s, sizeof(s),		/* abbreviation for param			*/
    "%s", abbrev? abbrev: "-?" );
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* param field label				*/
    "%s", desc? desc: "?"      );
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* param initial value 				*/
    "%s", val? val: ""         );
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* x position 						*/
    "%d", x                    );
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* y position 						*/
    "%d", y                    );
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),	
    "%d", labelwidth           );
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),	
    "%d", inputwidth           );
  DT_Q(dtmsw_feedback,(ctl, 1, s));
  return DT_RC_GOOD;
}

/*..........................................................................
 * QUATERNARY is not expected here, but handled nevertheless
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_enter_param_callback(
  dt_ctl_t *ctl,
  void *arg,
  char *abbrev,
  char *desc,
  char *val,
  dtparam_flag_t *flag)
{
  DT_F("dt_enter_param_callback");
  param_arg_t *param_arg = 
    (param_arg_t *)arg;
  dtparam_flag_t tflag;

  if (EXCLUDE_FIELD(abbrev))			/* don't want this one appearing?	*/
    return DT_RC_GOOD;

  tflag = *flag;						/* temp copy					    */

  if (									/* it has to be one of these	    */
     !(tflag & DTPARAM_FLAG_PRIMARY) &&
     !(tflag & DTPARAM_FLAG_SECONDARY) &&
     !(tflag & DTPARAM_FLAG_TERTIARY) &&
     !(tflag & DTPARAM_FLAG_QUATERNARY)
     )
    tflag |= DTPARAM_FLAG_PRIMARY;		/* pretend caller means primary	    */

  if ((tflag & param_arg->flag) ==		/* we do want this one now?		    */
      param_arg->flag)
  if ((tflag &							/* and lengthy flags match		    */
       DTPARAM_FLAG_LENGTHY) ==
      (param_arg->flag &
       DTPARAM_FLAG_LENGTHY))
  {
    int x;

    if (param_arg->h)					/* not just counting?			    */
	{
      if (tflag & DTPARAM_FLAG_LENGTHY)
        x = 0;
	  else
	  {
        if (param_arg->n ==				/* first in second column?		    */
            param_arg->h)
          param_arg->y =				/* go back to first row			    */
            param_arg->y0;

        if (param_arg->n >=				/* into second column?			    */
            param_arg->h)
          x = LABELWIDTH + INPUTWIDTH +	/* adjust X						    */
                GUTTER;		
        else
          x = 0;
	  }

      if (desc == NULL)
        desc = "";

	  DT_Q((*param_arg->newfield),(		/* create new field 				*/
        ctl,
        param_arg->newfield_arg,
        abbrev, desc, val,
        x, param_arg->y,
        param_arg->labelwidth,
        param_arg->inputwidth));

      param_arg->y++;
	}

    param_arg->n++;
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e 
dt_enter_somefields(	
  dt_ctl_t *ctl,
  dtparam_t *param,
  dtparam_flag_t flags,
  param_arg_t *param_arg)
{
  DT_F("dt_enter_somefields");

  param_arg->flag = flags;
  param_arg->h = 0;						/* this tells them just to count    */
  param_arg->n = 0;
  param_arg->y0 = param_arg->y;			/* remember the row				    */

  DT_Q(dtparam_callback,(param,			/* count fields						*/
    dt_enter_param_callback, 
    param_arg));
  if (flags & DTPARAM_FLAG_LENGTHY)		/* doing lengthy ones?			    */
  {
    param_arg->h =						/* one per line					    */
      param_arg->n;
    param_arg->inputwidth = 
      (int)(INPUTWIDTH * 2.5);
  }
  else									/* non-length ones?				    */
  {
    param_arg->h =						/* two per line					    */
      (param_arg->n+1) / 2;
    param_arg->inputwidth = INPUTWIDTH;
  }

  param_arg->labelwidth = LABELWIDTH;	/* max length of arg value		    */

  param_arg->n = 0;
  DT_Q(dtparam_callback,(param,			/* make fields						*/
    dt_enter_param_callback, 
    param_arg));

  if (param_arg->n)						/* point y past this block			*/
    param_arg->y =
      param_arg->y0 +
      param_arg->h + 1;

  return DT_RC_GOOD;
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dt_enter_fields(
  dtparam_t *param,						/* parameter description structure 	*/
  dt_enter_arg_t *arg,					/* argument description structure 	*/
  dt_enter_newfield_f *newfield,
  void *newfield_arg)
{
  DT_F("dt_enter_fields");
  dt_ctl_t *ctl = &param->ctl;
  param_arg_t param_arg;
  int i;
  int y;

  DTCSTACK_HIGHWATER;

  y = FIRST_Y0;
  for (i=0; arg[i].desc; i++)			/* for all expected positional args	*/
  {
	char tmp[32];
	int inputwidth = arg[i].val_max;
	inputwidth = DT_MIN(inputwidth,
      LABELWIDTH + 2 * INPUTWIDTH);
	dtstr_printf(ctl, tmp, sizeof(tmp),
      "arg%d");
    DT_Q((*newfield),(ctl,				/* field for argument description	*/
      newfield_arg, tmp,
      arg[i].desc, arg[i].val,
      0, y,
      LABELWIDTH, inputwidth));
    y++;
  }

  param_arg.newfield = newfield; 
  param_arg.newfield_arg = 
    newfield_arg;
  param_arg.y = y + (i? 1: 0);			/* blank line after positionals 	*/

  DT_Q(dt_enter_somefields,(ctl,		/* do long primary fields 			*/
    param,
    DTPARAM_FLAG_PRIMARY | 
    DTPARAM_FLAG_LENGTHY,
    &param_arg));
  DT_Q(dt_enter_somefields,(ctl,		/* do short primary fields 			*/
    param,
    DTPARAM_FLAG_PRIMARY,
    &param_arg));

  DT_Q(dt_enter_somefields,(ctl,		/* do long secondary fields			*/
    param,
    DTPARAM_FLAG_SECONDARY | 
    DTPARAM_FLAG_LENGTHY,
    &param_arg));
  DT_Q(dt_enter_somefields,(ctl,		/* do short secondary fields 		*/
    param,
    DTPARAM_FLAG_SECONDARY,
    &param_arg));

  DT_Q(dt_enter_somefields,(ctl,		/* do long tertiary fields 			*/
    param,
    DTPARAM_FLAG_TERTIARY | 
    DTPARAM_FLAG_LENGTHY,
    &param_arg));
  DT_Q(dt_enter_somefields,(ctl,		/* to short tertiary fields 		*/
    param,
    DTPARAM_FLAG_TERTIARY,
    &param_arg));

										/* note don't do QUATERNARY fields	*/
  
  return DT_RC_GOOD;
}

/* ........................................................................ *
 * emit no args or params if args is NULL
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dt_enter_feedback(
  dtparam_t *param,						/* parameter description structure 	*/
  const char *message,					/* parsing message 					*/
  dt_enter_arg_t *arg)					/* argument description structure 	*/
{
  DT_F("dt_enter_feedback");
  dt_ctl_t *ctl = &param->ctl;
  char s[80];

  DTCSTACK_HIGHWATER;

  if (message != NULL)
  {
    dtstr_printf(ctl, s, sizeof(s),		/* the message 						*/
      "%s", message); 	
    DT_Q(dtmsw_feedback,(ctl, 1, s));
    dtstr_printf(ctl, s, sizeof(s),		/* focus field 						*/
      "%s", ""); 
    DT_Q(dtmsw_feedback,(ctl, 1, s));
  }

  if (arg != NULL)
  {
    DT_Q(dt_enter_fields,(param, arg,	/* create feedback fields 			*/
      dt_enter_newfield_callback,
      NULL));
  }

  DT_Q(dtmsw_kick,(ctl,					/* tell them that there is feedback	*/
    DTMSW_KICK_FEEDBACK));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dt_enter_standard_to(					/* handle errto, dbgto, sayto		*/
  dtparam_t *param)
{
  DT_F("dt_enter_standard_to");
  dt_ctl_t *ctl = &param->ctl;
										/* ................................	*/
  if (ctl->errto == NULL &&				/* not already active? 				*/
	  param->errto_flag &				/* errto param has a value?			*/
      VALUE_OR_DEFAULT)
  {
    int fd;
										/* We need this "null" option in case
										   the main program has default errto
										   set to some file, and someone wants
										   to set it back to "not-specified"
										   from the command line.
										   The "not-specified" behavior for
										   a windows non-console mode
										   application is to open a separate
										   dedicated console window.
										*/ 
	if (!strcmp(param->errto, "null"))
	{
	  DT_Q(dtos_free2,(ctl, (char *)param->errto, F, "-errto"));
	  param->errto_flag &= ~DTPARAM_FLAG_ALLOC;
	  fd = 0;
	}
	else
	if (!strcmp(param->errto, "stdout")) /* a special filename? 			*/
	  fd = FILENO(DTCONFIG_STDOUT);
	else
	if (!strcmp(param->errto, "stderr"))
	  fd = FILENO(DTCONFIG_STDERR);
	else
	if (!strcmp(param->errto, "STDOUT"))
	{
	  dt_err_prefix = "ERROR";			/* so grep won't find it 			*/
	  fd = FILENO(DTCONFIG_STDOUT);
	}
	else
	if (!strcmp(param->errto, "STDERR"))
	{
	  dt_err_prefix = "ERROR";			/* so grep won't find it 			*/
	  fd = FILENO(DTCONFIG_STDERR);
	}
	else
      fd = OPEN(param->errto, 
        DTCONFIG_OPEN_ATEXT,
        DTCONFIG_OPEN_WPERM);
    if (fd == -1)
      return dt_err(ctl, F,
        "open(\"%s\", O_WRONLY)"
		" for errto failed errno %d",
        param->errto,
        errno);
    ctl->errto = (void *)(long)fd;
  }

  if (ctl->dbgto == NULL &&				/* not already active? 				*/
	  param->dbgto_flag &				/* dbgto param has a value?			*/
      VALUE_OR_DEFAULT)
  {
    int fd;
	if (!strcmp(param->dbgto, "null"))
	{
	  DT_Q(dtos_free2,(ctl, (char *)param->dbgto, F, "-dbgto"));
	  param->dbgto_flag &= ~DTPARAM_FLAG_ALLOC;
	  fd = 0;
	}
	else
	if (!strcmp(param->dbgto, "stdout")) /* a special filename? 			*/
	  fd = FILENO(DTCONFIG_STDOUT);
	else
	if (!strcmp(param->dbgto, "stderr"))
	  fd = FILENO(DTCONFIG_STDERR);
	else
	if (param->errto_flag &				/* errto param has a value?			*/
        VALUE_OR_DEFAULT &&
        !strcmp(param->dbgto,			/* debug to same place as errors? 	*/
                param->errto))
      fd = (int)(long)ctl->errto;
	else
      fd = OPEN(param->dbgto, 
        DTCONFIG_OPEN_ATEXT,
        DTCONFIG_OPEN_WPERM);
    if (fd == -1)
      return dt_err(ctl, F,
        "open(\"%s\", O_WRONLY)"
        " for dbgto failed errno %d",
        param->dbgto,
        errno);
    ctl->dbgto = (void *)(long)fd;
  }

  if (ctl->sayto == NULL &&				/* not already active? 				*/
	  param->sayto_flag &				/* sayto param has a value?			*/
      VALUE_OR_DEFAULT)
  {
    int fd;
	if (!strcmp(param->sayto, "null"))
	{
	  DT_Q(dtos_free2,(ctl, (char *)param->sayto, F, " -sayto"));
	  param->sayto_flag &= ~DTPARAM_FLAG_ALLOC;
	  fd = 0;
	}
	else
	if (!strcmp(param->sayto, "stdout")) /* a special filename? 			*/
	  fd = FILENO(DTCONFIG_STDOUT);
	else
	if (!strcmp(param->sayto, "stderr"))
	  fd = FILENO(DTCONFIG_STDERR);
	else
	if (param->errto_flag &				/* errto param has a value?			*/
        VALUE_OR_DEFAULT &&
        !strcmp(param->sayto,			/* info to same place as errors? 	*/
                param->errto))
      fd = (int)(long)ctl->errto;
	else
	if (param->dbgto_flag &				/* dbgto param has a value?			*/
        VALUE_OR_DEFAULT &&
        !strcmp(param->sayto,			/* info to same place as dbgors? 	*/
                param->dbgto))
      fd = (int)(long)ctl->dbgto;
	else
      fd = OPEN(param->sayto,
        DTCONFIG_OPEN_ATEXT,
        DTCONFIG_OPEN_WPERM);
    if (fd == -1)
      return dt_err(ctl, F,
        "open(\"%s\", O_WRONLY)"
        " for sayto failed errno %d",
        param->sayto,
        errno);
    ctl->sayto = (void *)(long)fd;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dt_enter_standard_param(				/* take action on standard params	*/
  dtparam_t *param)
{
  DT_F("dt_enter_standard_param");
  dt_ctl_t *ctl = &param->ctl;

  DTCSTACK_HIGHWATER;
										/* ................................	*/

  if (param->dbg_flag &					/* -D param has a value?			*/
       VALUE_OR_DEFAULT &&
      (param->dbg != NULL))				/* and it's not null?			    */
  {
	long dbg_mask;
    char msg[80];
	DT_Q(dt_dbg_parse,(ctl,				/* set dbg mask from dbg string		*/
      param->dbg, &dbg_mask,
      msg, sizeof(msg)));
    if (strlen(msg))					/* could not parse dbg string? 		*/
      return dt_err(ctl, F, 
        "%s", msg);
	ctl->dbg_mask |= dbg_mask;			/* put mask in environment 			*/
  }
										/* ................................	*/
  
  DT_Q(dt_enter_standard_to,(			/* handle -errto, -dbgto and -sayto	*/
    param));
										/* ................................	*/
  if (ctl != NULL &&					/* we have a ctl structure? 		*/
	  ctl->win == NULL &&				/* it does not have a win yet? 		*/
      param->win_flag &					/* -win param has a value?			*/
      VALUE_OR_DEFAULT &&
      (param->win != NULL))				/* and it's not null?			    */
  {
	DT_Q(dtos_malloc2,(ctl,				/* get space for server name 		*/
      (void **)&ctl->win,
      strlen(param->win)+1,
      F, "default windowing server"));
    strcpy((char *)ctl->win,			/* keep our own copy of the name 	*/
      param->win);
  }
										/* ................................	*/
  if (ctl != NULL &&					/* we have a ctl structure? 		*/
	  ctl->mem == NULL &&				/* it does not have a mem yet? 		*/
      param->mem_flag &					/* -mem param has a value?			*/
      VALUE_OR_DEFAULT &&
      (param->mem != NULL))				/* and it's not null?			    */
  {
	DT_Q(dtos_malloc2,(ctl,				/* get space for specification		*/
      (void **)&ctl->mem,
      strlen(param->mem)+1,
      F, "big memory source specs"));
    strcpy((char *)ctl->mem,			/* keep our own copy of the name 	*/
      param->mem);
  }
										/* ................................	*/
  if (param->yield_flag &				/* -yield param has a value?		*/
       VALUE_OR_DEFAULT &&
      (param->yield != NULL))			/* and it's not null?			    */
  {
	unsigned long yield_flags;
    char msg[80];
    DT_Q(dt_yield_parse,(				/* set yield from yield string		*/
      ctl, param->yield,
      &yield_flags,
      msg, sizeof(msg)));
    if (strlen(msg))					/* error message resulted? 			*/
      return dt_err(ctl, F, 
        "%s", msg);
	ctl->yield_flags = yield_flags;		/* put flags in the environment 	*/
    if (ctl->yield_flags !=				/* yield has been turned on? 		*/
		DT_YIELD_FLAG_OFF &&
		ctl->yield == NULL)				/* don't have yield function yet? 	*/
      ctl->yield = dt_yield_va;			/* start using standard function 	*/
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * must be called after dtparam_init()
 * meant to be called after param defaults are set
 * called by old programs written pre-dialog
 * programs expecting to do dialogs will call dt_interactive_dialog() inst
 * called by dt_interactive_dialog() as its first action
 * returns DT_RC_STOP if -help or -HELP is found
 *..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dt_enter_param(							/* parse params from console args	*/
  int argc,
  char *argv[],
  const char *progname,
  const char *usage,
  dtparam_t *param,
  int *pn)
{
  DT_F("dt_enter_param");
  dt_ctl_t *ctl = &param->ctl;
  char message[256];
  int n;

  DTCSTACK_HIGHWATER;

  if (progname != NULL)					/* caller is providing progname? 	*/
    ctl->progname = progname;

  DT_Q(dtparam_set,(param,				/* params we always parse for		*/
    DTPARAM_FLAG_TERTIARY,
    &param->dialog,   NULL, NULL, NULL,
    &param->keyboard, NULL, NULL, NULL,
    &param->dbgto,    NULL, NULL, NULL,
    &param->errto,    NULL, NULL, NULL,
    &param->sayto,    NULL, NULL, NULL,
    &param->mem,      NULL, NULL, NULL,
    &param->dbg,      NULL, NULL, NULL,
    &param->yield,    NULL, NULL, NULL,
    &param->win,      NULL, NULL, NULL,
    &param->help,     NULL, NULL, NULL,
    &param->HELP,     NULL, NULL, NULL,
    NULL));

  message[0] = '\0';
  DT_Q(dtparam_parse,(param,			/* get params from args				*/
    argc-1, argv+1, &n,
    message, sizeof(message)));

  if (message[0] != '\0')				/* parsing failed? 					*/
    dt_err(ctl, F, "%s", message);		/* this constitutes an error 		*/
  
  if (usage != NULL &&					/* caller is providing usage? 		*/
      (message[0] != '\0' ||			/* parsing failed?					*/
       param->help_flag &				/* or help has been specified?	    */
       DTPARAM_FLAG_VALUE ||
       param->HELP_flag &				/* or HELP has been specified?	    */
       DTPARAM_FLAG_VALUE))
    return dt_usage_say(				/* give usage						*/
      param, usage);

  if (message[0] != '\0')				/* parsing failed? 					*/
    return DT_RC_BAD;					/* we can do no more 				*/
  
  DT_Q(dt_enter_standard_param,(		/* deal with standard params		*/
    param));

  if (pn)								/* caller wants param count back? 	*/
    *pn = n + 1;						/* account for program name			*/

  return DT_RC_GOOD;
}

/* ........................................................................ *
| NAME
| dt_enter_parse() - parse command line and set up environment
| 
| DESCRIPTION
| dt_enter_parse() parses the command line according to
| the given parameter and argument description structures.
|
| The command line to be parsed is represented by the ~argv[]~
|_aside ~argc~ and ~argv[]~ are like from C :main:.
| array of string pointers.
| There are ~argc~ pointers in the array.
| The calling program's name is passed in the string ~progname~.
| A usage message is passed in the string ~usage~,
| which may contain multiple lines separated by newline characters.
| The description of the keyword parameters expected by the program
| is passed in the ~param~ structure.
| Values parsed from the command line will be returned in the same
| ~param~ structure.
| Each positional argument expected by the program
| is described by one element of the ~arg~ structure array.
| Values for the positional arguments will be returned in the same
| ~arg~ structures.
| If not NULL, ~check~ points to a caller-provided function which
| is called after parsing to do application dependent
| parameter and argument checking.
|
| Before going further, please note that this is not a required function.
|_aside This function convenient but not required.
| It provides a convenient means of consisting initialization of
| a ~param->ctl~ structure,
| but you may initialize the structure by directly setting its members.
| The libraries will also function correctly with ~ctl~ set to NULL
| or pointing to a structure of all zeros.
| 
| This function expects the command line strings to be ordered
| as follows:
|_index command line,, parsing
|_index parsing,, command line
|_aside Basic command line syntax.
| 
|:  command parameters -- arguments
| 
| where:
| :command:		the name of the file being executed, always ~argv[0]~
| :parameters:	optional list of :keyword-value: pairs in any order
| :--:			optional :token: of two hyphens indicating end of parameters
| :arguments:	optional list of :tokens: whose order and meaning are specific
| 				to the program being run
| :keyword:		:token: beginning with a ':-:'
|				which is an unambiguous abbreviation of
| 				one of the predefined set of 50 or so keywords
| :value:		required :token: following :keyword:
| :token:		one of ~argv[n]~, 
|				where ~i~ greater than 0 and less than ~argc~
| 
| The function examines all the ~argv[]~ tokens on the command line,
| checks them for correctness with respect to ~param~ and ~arg~,
| and stores the values in these two structures.
| Before calling dt_enter_parse(), you should have initialized
| both a ~param~ and an ~arg~ structure.
| 
| Initializing a ~param~ structure involves calling dtparam_init() on it,
|_aside Initialize parameter structure.
| then calling dtparam_set() one or more times to describe all the
| keywords which are to be considered parsable for the program.
| Non-parsable keywords and bad values
| found on the command line will cause this function
| to print ~usage~ and return :DT_RC_STOP:.
|
| Each keyword marked as parsable in ~param~ is not only
|_aside Keyword environment variable.
| allowed on the command line,
| but also searched for as an environment variable.
| For instance, if ~param->color~ is marked as parsable but
| :-color: does not appear on the command line,
| then the value of the parameter is taken from the environment variable
| :DT_COLOR: instead.
| this is a convenient means of setting personal defaults
| or often-used parameters.
| It also helps avoid command line limitations on DOS.
|
| For each keyword parameter which is marked as parsable and
| found on the command line or found in an environment variable,
| the corresponding value member of the ~param~ structure is
| set to the the keyword value.
| For example, suppose your program is run with two parameters:
|
|:  myprog -color blue -x 3
| 
| This function will assign ~param->color~ to point to the string "blue",
| and the long integer ~param->x~ to 3.
| 
| Initializing an ~arg~ structure involves calling dt_enter_arg()
|_Aside Initialize argument structure.
| to describe all the positional arguments which the program
| expects to follow the parameters on the command line.
| The final element of the ~arg~ array is a sentinel,
| marked by its ~desc~ member set to NULL.
| 
| Each positional argument is copied to
| the ~val~ string member of the corresponding ~arg~ structure.
| Pretend that your program was run with two arguments:
| 
|:  myprog myinput myoutput
|
| This function will copy the string "myinput" to ~arg[0].val~
| and "myoutput" to ~arg[1].val~.
| 
| Beyond copying the values of the command line strings into
| the ~val~ members of the ~arg~ structures,
| this function does nothing with the arguments.
| However, if there are more positional arguments of the command line
|_aside Error if too many arguments.
| than there are elements in the ~arg~ array,
| then this function will print ~usage~ and return :DT_RC_STOP:.
| If there are not enough command line parameters to
|_Aside Unspecified arguments cleared.
| meet the number of elements in the ~arg~ array,
| then the excess ~arg->val~ values are set to the empty string.
|
| Besides parsing the command line and stuffing values into
| ~param~ and ~arg~,
| this function also sets up an environment in ~param->ctl~.
|dxinclude(ctl1.dx)
| Here is a summary of the members of ~param->ctl~ which are set up:
| 
| :ctl->progname:		name of program being executed
| :ctl->mem:			big memory source specification string
| :ctl->yield_flags:	whether and when to yield timeslices
| :ctl->yield:			default yield function
| :ctl->win:			name of default windowing server
| :ctl->dbg_mask:		mask of bits enabling classes of debug messages
| :ctl->errto:			where to put error messages
| :ctl->dbgto:			where to put debug messages
| :ctl->sayto:			where to put informational messages
|
| The human running the program has
|_aside Environment control from the command line.
|_index environment,, control from command line
| control over all these things through
| standard keyword parameters on the command line.
| These standard parameters are always marked parsable by dt_enter_parse().
| If any of these parameters are not given on the command line,
| the environment variables with corresponding names are used.
| If the environment variables are missing,
| then compile-time defaults are used.
| Some of the standard parameters always parsed are:
|_aside Parameters always parsed.
|_index parameters,, always parsed
| :-mem:		big memory source specifications
| :-yield:		whether and when to yield timeslices
| :-win:		name of default windowing server
| :-D:			debug level
| :-errto:		where error messages go
| :-dbgto:		where debug messages go
| :-sayto:		where informational messages go
| :-dialog:		dialog type (full-screen interaction only)
| :-keyboard:	keyboard type (full-screen interaction only)
| :-help:		display short help and quit
| :-HELP:		display long help and quit
|
| Dtack Imaging Software provides an alternative to this function
| when you are using Dtack's interactive dialog front end.
| Programs using the interactive front end should call
| dt_interactive_parse() instead of this function.
| Please see
|_hyper document, dtdoclist(dtinter, title)
|
| RETURN VALUES
| The program environment at ~param->ctl~ is set up according
| to the standard keywords found on the command line.
| Values and flags from the keyword parameters on the command line
| are placed into the corresponding members of the ~param~ structure.
| Positional arguments string are copied into the ~arg[i].val~ fields.
| given in the variable argument list.
| The value of the function is always one of three different values:
| :DT_RC_GOOD:	function executed successfully
| :DT_RC_STOP:	parameter or argument parsing failed,
|				~usage~ printed to console or :stderr: and
|				dt_err() will have been called at least once
| :DT_RC_BAD:	function did not execuate successfully,
|				dt_err() will have been called at least once
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to malloc() memory for string copies
| - ~check~ structure returned error
| 
| CAVEATS
| Call dt_leave() to free any memory allocated by this function.
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :enter: Function Group
| 
| The following functions are also related:
| dtparam_init()	initialize parameter structure
| dt_enter_arg()	initialize argument structures
| dt_leave()		free memory allocated by this function
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv2.c:
| 
| The following code snippet highlights this function:
|:
|:  dtbpe_t bpe;
|:  dtbpe_init(NULL, &bpe, 0);			// allocate bpe resources
|:  dtbpe_uninit(NULL, &bpe);			// free bpe resources
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dt_enter_parse(							/* parse command line				*/
  int argc,								/* number of argv[] elements 		*/
  char *argv[],							/* array of string pointers			*/
  const char *progname,					/* program name 					*/
  const char *usage,					/* program usage description 		*/
  dtparam_t *param,						/* parameter description structure 	*/
  dt_enter_arg_t *arg,					/* argument description structure 	*/
  dt_enter_check_f *check,				/* caller's argument check function */
  void *appstruct)
{
  DT_F("dt_enter_parse");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t fd;
  char *string = NULL;
  char **argv2 = NULL;
  int i, n;
  const char *start;
  dt_rc_e rc = DT_RC_GOOD;
  
  fd.fd = -1;

  DTCSTACK_HIGHWATER;
  
  if (argc == 2 &&						/* exactly one arg? 				*/
	  argv[1][0] == '@')				/* which begins with an at-sign?	*/
  {
	int maxtokens;
	DT_C(dtfd_open,(ctl, &fd,			/* open @-file 						*/
      argv[1]+1, 
      DTCONFIG_FOPEN_RTEXT));			/* strip ^M and ^Z if DOS or MSW 	*/

    DT_C(dtliner_fd_string,(ctl, &fd,	/* make string from file 			*/
      "#*;", " \t", &string));

	maxtokens = strlen(string)/4;		/* estimate maximum tokens 			*/
	DT_C(dtos_malloc2,(ctl,				/* space for token pointers			*/
      (void **)&argv2, 
      maxtokens*sizeof(*argv2),
      F, "argv replacement"));
	DT_C(dtstr_argv,(ctl, string,		/* set up tokens 					*/
      argv2+1, maxtokens-1, &argc));
	argv2[0] = argv[0];
	argv = argv2;
	argc++;
  }

  DT_Q(dt_enter_param,					/* parse params on command line	    */
    (argc, argv, progname, usage,
     param, &n));

  argc -= n;
  argv += n;

  for (i=0; arg[i].desc; i++)			/* for each expected positional	    */
  {
    if (i < argc)						/* this one is on command line?		*/
      DT_Q(dtstr_printf,(ctl,			/* fill in value from command line	*/
        arg[i].val, arg[i].val_max,
        "%s", argv[i]))
    else								/* this one is not on command line?	*/
      arg[i].val[0] = '\0';
  }

  if (i < argc)							/* more args than positionals?		*/
  {
    dt_err(ctl, NULL,
      "Sorry, \"%s\" is an unexpected"
      " positional argument.", 
      argv[i]);
	return DT_RC_STOP;
  }

  if (check != NULL)					/* caller is providing function? 	*/
  {
    char message[DT_ENTER_MSG_MAXLEN];
    message[0] = '\0';

    DT_Q((*check),(ctl, appstruct,		/* check everything for input error	*/
	  param, arg,
      message, &start));

    if (message[0] != '\0')				/* there is argument error? 		*/
	{
	  if (ctl->feedbackwin != NULL)		/* we have a feedback channel?		*/
	  {
		DT_Q(dt_enter_feedback,(param,	/* output feedback 					*/
          message,						/* give the stop message 			*/
          ctl->feedbackonly? arg:		/* wanting full form description? 	*/
            (dt_enter_arg_t *)NULL));	/* give that or just message 		*/
		dt_dbg(ctl, F, DT_DBG_MASK_ARG,
          "stop message <%s>", 
          message);
		return DT_RC_STOP;
	  }
	  else
	  {
	    dt_err(ctl, NULL, "%s",			/* spit out message as error		*/
          message);
	    return dt_usage_say(			/* give usage as line-based output	*/
          param, usage);
	  }
	}
  }

  if (ctl->feedbackonly)				/* this call for arg feedback only? */
  {
	if (ctl->feedbackwin != NULL)
	  DT_Q(dt_enter_feedback,(param, 
        "", arg));
	dt_dbg(ctl, F, DT_DBG_MASK_ARG,
      "stopping after feedback");
    return DT_RC_STOP;
  }

  if (ctl->feedbackwin != NULL)			/* notify launcher we are running 	*/
  {
    int pid;
    char tmp[16];
	DT_Q(dtos_getpid,(ctl, &pid));
    dtstr_printf(ctl,
      tmp, sizeof(tmp),
      "%08lx", pid);
    DT_Q(dtmsw_feedback,(ctl, 1, tmp));	/* put our window id into feedback	*/
#ifdef DTCONFIG_IMPLIED_MSW32
    dtstr_printf(ctl,					/* unique system-wide id			*/
      tmp, sizeof(tmp),
      "%08lx", 
      GetCurrentProcessId());
#endif
    DT_Q(dtmsw_feedback,(ctl, 1, tmp));	/* put our process id into feedback	*/
    DT_Q(dtmsw_kick,(ctl,	
      DTMSW_KICK_LAUNCHED));
    DT_Q(dtmsw_pause,(ctl));			/* wait for confirmation 			*/
  }
  
cleanup:
  if (argv2 != NULL)
	DT_I(dtos_free2,(ctl, argv2,
      F, "argv replacement"));
  if (string != NULL)
	DT_I(dtos_free2,(ctl, string,
      F, "@-file string"));
  if (fd.fd != -1)
	DT_I(dtfd_close,(ctl, &fd));

  return rc;
}

/* ........................................................................ *
| NAME
| dt_enter_arg() - set up argument description structure
| 
| DESCRIPTION
|_index command line,, arguments, specifying
|_index arguments,, command line, specifying
| dt_enter_arg() initializes an argument description array
| in preparation for dt_enter_parse().
|
| The elements of the ~*arg~ array of structures
| are initialized from the ordered list of 4-tuples
| found in the variable argument list.
| The variable argument list must contain one group of 4 arguments
| for each element of ~*arg~,
| plus one final NULL argument to mark the end.
| 
| Each 4-tuple of variable arguments in the list contains
| the following values:
|
|:  const char *desc;					// description of the argument
|:  const char *help;					// short help for the argument
|:  const char *value;					// place to store argument value
|:  int valmax;							// maximum size of *value
|
| These 4-tuples correspond exactly to the members
|_Aside Structure used later in parsing.
| of the :dt_enter_arg_t: structure.
|
| A NULL for the :desc: member in the final 4-tuple
|_Aside NULL must be last argument.
| marks the end of the variable argument list.
| All arguments in the variable list after a NULL :desc: member
| are ignored.
|
| This function is a convenient means of initializing an array of
| argument description structures.
| It does no parsing of anything.
|_aside Does no parsing.
| 
| The argument description structures initialized by this function
| are used later by dt_enter_parse()
| to check the validity of command-line arguments.
| dt_enter_parse() also will store the actual values found on the command
| line into the ~arg->val~ structure member.
|
| This function is not required, merely convenient.
|_aside Convenient but not required.
| You may set the ~arg~ structure members directly.
| For example, pretend you expect two positional arguments
| whose lengths are relatively short (32 or fewer bytes).
| To do what this function does, your code might look like:
|
|:  dt_enter_arg_t arg[3];				// one extra structure
|:  char first[32];
|:  char second[32];
|:  memset(arg, 0, sizeof(arg));
|:  arg[0].val = first;					// set up the first
|:  arg[0].val_max = sizeof(first);
|:  arg[0].desc = "First argument";
|:  arg[1].val = second;				// set up the second
|:  arg[1].val_max = sizeof(second);
|:  arg[1].desc = "Second argument";
|:  arg[2].desc = NULL;					// sentinel
|
| RETURN VALUES
| One element of ~*arg~ is initialized from each 4-tuple
| given in the variable argument list.
| A final element of ~*arg~ is always returned with its member
| ~arg->desc~ set to NULL.
|dxinclude(return0.dx)
| 
| ERRORS
|dxinclude(noerror.dx)
| 
| CAVEATS
| The variable argument list must contain complete 4-tuples.
| 
| The variable argument list must have a NULL pointer as the last 4-tuple.
| 
| The number of elements of the ~arg~ array
|_aside Final ~arg~ element is a sentinel only.
| must be #one greater than# the number of 4-tuples
| in the variable argument list.
| 
| SEE ALSO
| Please refer to the
|_hyper Overview, "Overview"
| and
|_hyper sections, The :dbg: Function Group
| 
| The following function is also related:
| dt_enter_parse()		uses this ~arg~ array to parse command line
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtenv2.c:
| 
| The following code snippet highlights this function:
| 
|:  dt_enter_arg_t arg[1+1];			// space for one arg plus sentinel
|:
|:  dtparam_set(&param, 0, 	 	        // set default for params we want
|_Aside Describe keyword parameter.
|:    &param.color, NULL, NULL, "red",	// 4-tuple for "color"
|:    NULL);							// sentinel
| 
| END
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API2
dt_enter_arg(							/* set up argument structure 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_enter_arg_t *arg,					/* argument structure to set up 	*/
  ...)
{
  dt_rc_e rc;
  va_list ap;
  va_start(ap, arg);					/* start variable arg procesing		*/
  rc = dt_enter_arg_va(ctl, arg, ap);
  va_end(ap);
  return rc;
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dt_enter_arg_va(						/* set up argument structure 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_enter_arg_t *arg,					/* argument structure to set up 	*/
  va_list ap)							/* variable argument list		    */
{
  int i = 0;

  while(1)
  {
	DT_MEMSET((void *)&arg[i], 0, sizeof(*arg));	/* clear array element 				*/
    arg[i].desc = va_arg(ap, char *);	/* get desc for this arg		    */
    if (arg[i].desc == NULL)			/* no desc signals end of list	    */
      break;
    arg[i].help = va_arg(ap, char *);	/* get other stuff for this arg	    */
    arg[i].val = va_arg(ap, char *);
    arg[i].val_max = va_arg(ap, int);
    arg[i].reserved = 0;
    arg[i].flags = 0;
    i++;
  }

  return DT_RC_GOOD;
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API2
dt_enter_arg2(							/* set up argument structure 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_enter_arg_t *arg,					/* argument structure to set up 	*/
  ...)
{
  dt_rc_e rc;
  va_list ap;
  va_start(ap, arg);					/* start variable arg procesing		*/
  rc = dt_enter_arg2_va(ctl, arg, ap);
  va_end(ap);
  return rc;
}

/* ........................................................................ *
 * ........................................................................ */

dt_rc_e 
DTCONFIG_API1
dt_enter_arg2_va(						/* set up argument structure 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_enter_arg_t *arg,					/* argument structure to set up 	*/
  va_list ap)							/* variable argument list		    */
{
  int i = 0;

  while(1)
  {
	DT_MEMSET((void *)&arg[i], 0, sizeof(*arg));	/* clear array element 				*/
    arg[i].desc = va_arg(ap, char *);	/* get desc for this arg		    */
    if (arg[i].desc == NULL)			/* no desc signals end of list	    */
      break;
    arg[i].help = va_arg(ap, char *);	/* get other stuff for this arg	    */
    arg[i].val = va_arg(ap, char *);
    arg[i].val_max = va_arg(ap, int);
    arg[i].reserved = 0;
    arg[i].flags = va_arg(ap, unsigned long);
    i++;
  }

  return DT_RC_GOOD;
}

#if DTPRJ_NOINTER == DT_1

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dt_interactive_parse(					/* parse params from console args	*/
  int argc,
  char *argv[],
  const char *progname,
  const char *usage,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  dt_enter_check_f *check, 
  void *appstruct)
{
  DT_F("dt_interactive_parse");
  dt_ctl_t *ctl = &param->ctl;
  DT_Q(dt_enter_parse,(argc, argv,
    progname, usage, param,
    arg, check, appstruct));
  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dt_interactive_after(					/* have a dialog around params/args	*/
  dt_ctl_t *ctl,
  dtparam_t *param,
  const char *usage,
  dt_enter_arg_t *arg,
  dt_enter_check_f *check,
  void *appstruct,
  dt_rc_e xrc)
{
  return DT_RC_STOP;
}

/*..........................................................................*/
dt_rc_e 
DTCONFIG_API1
dt_interactive_leave(					/* clean up before leaving 			*/
  dtparam_t *param,
  dt_rc_e xrc)
{
  DT_F("dt_interactive_leave");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc = DT_RC_GOOD;

  DT_I(dt_leave,(param));				/* do low level epilogue 			*/

  return rc;
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
