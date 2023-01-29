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
  20000104 DE properly initialize table for egcs
  19991211 DE remove DTPARAM_FLAG_ALLOC definition
 *..........................................................................*/




#include <dtack/base.h>
#include <dtack/os.h>
DT_RCSID("source/low $RCSfile: param.c,v $ $Revision: 1.2 $");
#include <dtack/prj.h>
#include <dtack/str.h>

#define DT1D_Q(FUNC, ARGS)				/* check rc and return				*/\
{                                                                             \
  dt1d_rc_e rc;                                                               \
  DT_SETLOC                                                                   \
  rc = FUNC ARGS;		                                                      \
  if (rc != DT1D_RC_GOOD) 		                                              \
  {                                                                           \
    dt_dbg_trace(ctl, F,                                                      \
      DT_RCSID_STRING, __LINE__,                                              \
      "%s returned rc %d", #FUNC, rc);                                        \
    return (dt_rc_e)rc;                                                       \
  }                                                                           \
}

typedef enum {
  DECIMAL, FLOATE, STRING				/* parameter types				  	*/
} type_t;

typedef struct {						/* form of simple parse entry		*/
  char *abbrev;							/* flag abbreviation				*/
  type_t type;							/* parameter type					*/
  int lengthy;							/* whether lengthy or not		    */
  char *format;							/* format for sscanf				*/
  unsigned int value_offset;			/* value offset in param struct		*/
  unsigned int desc_offset;				/* description offset in param		*/
  unsigned int help_offset;				/* help offset in param				*/
  unsigned int order_offset;			/* order offset in param			*/
  unsigned int flag_offset;				/* flag offset in param struct		*/
  char *desc;							/* description					  	*/
} table_t;

#define OFF(MEM) \
  (unsigned int)&(((dtparam_t *)0)->MEM), \
  (unsigned int)&(((dtparam_t *)0)->MEM ## _desc), \
  (unsigned int)&(((dtparam_t *)0)->MEM ## _help), \
  (unsigned int)&(((dtparam_t *)0)->MEM ## _order), \
  (unsigned int)&(((dtparam_t *)0)->MEM ## _flag)

#define VALUE(PARAM, T) (*(const char **)((char *)PARAM + T->value_offset))
#define DESC(PARAM, T)  (*(char **)((char *)PARAM + T->desc_offset))
#define HELP(PARAM, T)  (*(char **)((char *)PARAM + T->help_offset))
#define ORDER(PARAM, T)  (*(int *)((char *)PARAM + T->order_offset))
#define FLAG(PARAM, T)  (*(dtparam_flag_t *)((char *)PARAM + T->flag_offset))

#define DTPARAM_COUNT (53)

static table_t table[DTPARAM_COUNT] = {
  {"-D",           STRING,  1, "%s",  OFF(dbg),         "Debug mask"},
  {"-datadir",     STRING,  1, "%s",  OFF(datadir),     "Data directory"},
  {"-b",           FLOATE,  0, "%le", OFF(b),           "b factor"},
  {"-background",  FLOATE,  0, "%le", OFF(background),  "Color of off pixels"},
  {"-cache",       STRING,  0, "%s",  OFF(cache),       "Cache"},
  {"-color",       STRING,  0, "%s",  OFF(color),       "Color"},
  {"-config",      STRING,  0, "%ld", OFF(config),      "Configuration"},
  {"-connect",     STRING,  0, "%ld", OFF(connect),     "Connectedness"},
  {"-dbgto",       STRING,  1, "%s",  OFF(dbgto),       "Debug output to"},
  {"-dialog",      STRING,  1, "%s",  OFF(dialog),      "Dialog server"},
  {"-dt",          FLOATE,   0, "%le", OFF(dt),         "Delta t"},
  {"-dx",          FLOATE,   0, "%le", OFF(dx),         "Delta x"},
  {"-dy",          FLOATE,   0, "%le", OFF(dy),         "Delta y"},
  {"-dz",          FLOATE,   0, "%le", OFF(dz),         "Delta z"},
  {"-epsilon",     FLOATE,   0, "%le", OFF(epsilon),    "Algorithm epsilon"},
  {"-errto",       STRING,  1, "%s",  OFF(errto),       "Errors output to"},
  {"-foreground",  FLOATE,   0, "%le", OFF(foreground), "Color of on pixels"},
  {"-gamma",       FLOATE,   0, "%le", OFF(gamma),      "Algorithm gamma"},
  {"-h",           DECIMAL, 0, "%ld", OFF(h),           "Height"},
  {"-HELP",        STRING,  0, "%s",  OFF(HELP),        "Long help"},
  {"-help",        STRING,  0, "%s",  OFF(help),        "Short help"},
  {"-ident",       STRING,  1, "%s",  OFF(ident),       "Identification"},
  {"-input",       STRING,  1, "%s",  OFF(input),       "Input name"},
  {"-output",      STRING,  1, "%s",  OFF(output),      "Output name"},
  {"-keyboard",    STRING,  1, "%s",  OFF(keyboard),    "Keyboard name"},
  {"-k",           FLOATE,   0, "%le", OFF(k),          "Algorithm k"},
  {"-lambda",      FLOATE,   0, "%le", OFF(lambda),     "Algorithm lambda"},
  {"-mem",         STRING,  1, "%s",  OFF(mem),         "Memory source"},
  {"-max",         FLOATE,   0, "%le", OFF(max),        "Maximum"},
  {"-min",         FLOATE,   0, "%le", OFF(min),        "Minimum"},
  {"-mu",          FLOATE,   0, "%le", OFF(mu),         "Algorithm mu"},
  {"-n",           DECIMAL, 0, "%ld", OFF(n),           "Count"},
  {"-name",        STRING,  0, "%s", OFF(name),         "Name"},
  {"-offset",      FLOATE,  0, "%le", OFF(offset),      "Offset"},
  {"-omega",       FLOATE,  0, "%le", OFF(omega),       "Algorithm omega"},
  {"-op",          STRING,  0, "%s",  OFF(op),          "Operation"},
  {"-parent",      STRING,  0, "%s",  OFF(parent),      "Parent process"},
  {"-path",        STRING,  0, "%s",  OFF(path),        "Data file search path"},
  {"-phi",         FLOATE,  0, "%le", OFF(phi),         "Algorithm phi"},
  {"-precision",   STRING,  0, "%s",  OFF(precision),   "Precision"},
  {"-psi",         FLOATE,  0, "%le", OFF(psi),         "Algorithm psi"},
  {"-reps",        DECIMAL, 0, "%ld", OFF(reps),        "Repetitions"},
  {"-sayto",       STRING,  1, "%s",  OFF(sayto),       "Info output to"},
  {"-screen",      STRING,  1, "%s",  OFF(screen),      "Screen interface"},
  {"-sigma",       FLOATE,  0, "%le", OFF(sigma),       "Algorithm sigma"},
  {"-t",           FLOATE,  0, "%le", OFF(t),           "Threshold"},
  {"-variant",     STRING,  0, "%s",  OFF(variant),     "Execution variant"},
  {"-w",           DECIMAL, 0, "%ld", OFF(w),           "Width"},
  {"-win",         STRING,  1, "%s",  OFF(win),         "Display server"},
  {"-x",           DECIMAL, 0, "%ld", OFF(x),           "Output window x"},
  {"-y",           DECIMAL, 0, "%ld", OFF(y),           "Output window y"},
  {"-yield",       STRING,  0, "%ld", OFF(yield),       "Yield mode"},
  {NULL,           STRING,  0, "",    OFF(yield),       ""}
};

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtparam_format_value(					/* format one value					*/
  dtparam_t *param,
  table_t *t,
  char *string,
  int lmax)
{
  dt_ctl_t *ctl = &param->ctl;

  switch(t->type)						/* format the value					*/
  {
	case DECIMAL:   
	  dtstr_printf(ctl, string, lmax, t->format,
        *(long *)&VALUE(param, t));
	break;
	case FLOATE:
	{
	  double d = *(double *)&VALUE(param, t);
	  if (d == (long)d)
    	dtstr_printf(ctl, string, lmax, "%ld.0", (long)d);
	  else
	  if (d*10.0 == (long)(d*10.0))
    	dtstr_printf(ctl, string, lmax, "%.1f", d);
	  else
	  if (d*100.0 == (long)(d*100.0))
    	dtstr_printf(ctl, string, lmax, "%.2f", d);
	  else
	  if (d*1000.0 == (long)(d*1000.0))
    	dtstr_printf(ctl, string, lmax, "%.3f", d);
	  else
    	dtstr_printf(ctl, string, lmax, t->format, d);
	}
	break;
	case STRING:
	{
      const char *v = VALUE(param, t);
      dtstr_printf(ctl, string, lmax, "%s", v? v: "");
	}
	break;
    default:
      dtstr_printf(ctl, string, lmax, "???");
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * dtparam_setflags - set flags of all params
 * set all ~param-~xxx_flag> to the given value
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtparam_setflags(						/* set flags of all params			*/
  dtparam_t *param,						/* param structure to operate on 	*/
  dtparam_flag_t flag)					/* flag value to set on each param 	*/
{
  table_t *t;

  for (t=table; t->abbrev; t++)			/* go through parse table			*/
    FLAG(param, t) = flag;

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtparam_callback(						/* callback for each param			*/
  dtparam_t *param,						/* param structure to be traversed 	*/
  dtparam_callback_f *callback,			/* function to call for each param	*/
  void *arg)							/* arg passed through to callback 	*/
{
  DT_F("dtparam_callback");
  dt_ctl_t *ctl = &param->ctl;
  table_t *t;
  char *desc;
  char temp[256];
  dtntype_s4_t order[DTPARAM_COUNT];
  int n = 0;

  for (t=table; t->abbrev; t++)			/* collect orders					*/
    order[n++] = ORDER(param, t);

  DT1D_Q(dt1d_sort,(DTNTYPE_S4,			/* sort orders						*/
    order, n));

  while(n)
  {
    n--;
    if (n && 							/* not lowest order?				*/
        order[n] == order[n-1])			/* but same as previous order?		*/
      continue;
    for (t=table; t->abbrev; t++)		/* go through parse table			*/
    if ((ORDER(param, t) == 			/* param is at current order level?	*/
         order[n]) &&
        (FLAG(param, t) &				/* param is marked for parsing?		*/
         DTPARAM_FLAG_PARSE))
    {
      if ((FLAG(param, t) &				/* param has a default?				*/
           DTPARAM_FLAG_DEFAULT) ||
          (FLAG(param, t) &				/* param has a run-time value?		*/
           DTPARAM_FLAG_VALUE))
        DT_Q(dtparam_format_value,		/* format the value					*/
          (param, t,
           temp, sizeof(temp)))
      else								/* no default value?				*/
	    strcpy(temp, "");

      if (!(FLAG(param, t) &			/* value has a description?			*/
            DTPARAM_FLAG_NODESC))
        desc = DESC(param, t);
      else
        desc = NULL;
  
      DT_Q(callback,(ctl, arg,			/* do the callback				    */
        t->abbrev, desc, temp,
        &FLAG(param, t)));
    }
  }
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtparam_parse() - parse params from argc/argv
| 
| DESCRIPTION
|_index argc/argv,, parsing
|_index parsing,, argc/argv
|_index parsing,, :main: program arguments
|_index main program arguments, :main: program arguments, parsing
| dtparam_parse() parses a command line in argc/argv form
|_aside Puts values in ~param~ structure.
| and stores the values in a given parameter structure.
| 
| Starting with ~argv[0]~, this function scans up to ~argc~
| strings from the ~argv~ array.
| Keyword-value pairs are interpreted according to the current
|_aside Initialize ~param~ first.
| setup in the ~param~ structure.
| The values are minimally interpreted and stored in the ~param~ structure.
| The number of tokens examined is returned in ~*nused~ 
| if ~nused~ is not NULL.
| If there is a parsing error,
| a friendly response is placed in ~*message~ up to ~lmax~ bytes long.
|
| Stated informally, this function scans for keyword-value
|_aside Ends at non-keyword.
| pairs until it finds something that is not a keyword.
| Non-keywords are: the special value ':--:', negative integers,
| anything not beginning with a ':-:', :-help:, :-HELP:,
|_index help,, run-time
|_index run-time,, help
| anything not in the list of marked keywords,
| and the end of the ~argv[]~ array.
| Scanning also stops if it finds a bad value for a keyword.
|
| For all values found, :DTPARAM_FLAG_VALUE: bit in the ~param-xxx_flag~
| is set and the :DTPARAM_FLAG_DEFAULT: bit is cleared.
| This signifies that the parameter in question has been
| explicitly found on the command line and is therefore not
| considered a default.
|
| This function does not interpret any parameter value beyond converting
|_aside No interpretation.
| to :long: or :double: if necessary.
| In particular, it does not set the environment debug mask ~ctl->dbg_mask~
| when it finds the :-D: keyword.
|
| No values of ~argv~ are changed.  Upon return from this function,
|_Aside Copies what it needs.
| the caller is free to dispose of the ~argv~ space in any way.  
| This function will make private copies of any strings necessary.  
| The function dtparam_free() should be called to free these string copies. 
|
| To mark a keyword for parsing, just set the
|_Aside Keywords must be marked in ~param~.
| :DTPARAM_FLAG_PARSE: bit in the flag member of ~param~
| corresponding to the keyword desired.
| For example, if your program expects the :-color: keyword,
| set the flag like:
| 
|:  param->color_flag |= DTPARAM_FLAG_PARSE
| 
| This function assumes that the ~argv[]~ array has tokens
| in the following order:
|_index command line,, parsing
|_index parsing,, command line
|_index keyword,, command line
| 
|:  parameters -- other stuff
| 
| where:
| :parameters:	optional list of :keyword/value: pairs in any order
| :--:			optional :token: of two hypens indicating end of parameters
| :other stuff:	not examined by this function
| :keyword:		:token: beginning with a hyphen (-)
|				which is an unambiguous abbreviation of
| 				one of the predefined set of 50 or so keywords
| :value:		required :token: following :keyword:
| :token:		one of ~argv[n]~, where ~i~ is
| 				greater than 0 and less than ~argc~
|
| The keyword may always be separated from its value by one or more
|_aside Separate keyword from value by space.
| spaces or tabs.
| In fact, this is the recommended way of entering values
| on the command line.
| However, where there would not be ambiguity,
| the value may be immediately adjacent to its keyword
| with no intervening whitespace.
| The rules for determining when whitespace may be omitted are complex,
| but having the whitespace be optional is consistent with other Unix-like
| programs.
| An exact specification may not be required.
| Since most command line arguments are parsed in an interactive
| situation, feedback should be nearly instantaneous.
| Coupled with good error messages, syntax problems should be obvious
| and can be corrected and the command retried.
| 
| This function stops scanning the ~argv[]~ array when
|_aside Parsing stops at end of parameters.
| it is expecting a :keyword: but it finds:
| 
| 1.	the value ':--:'
| 2.	a negative integer
| 3.	something with no leading ':-:' character
| 4.	the value :-help: or :-HELP:
| 5.	the end of the array
| 6.	something with a leading ':-:' character which is not an
|		unambiguous abbreviation for one of the predefined keywords
| 		which have been marked for parsing
| 
| The last case generates a non-empty ~message~.
|_aside The two parsing errors.
| Parsing also stops and generates a ~message~
| if it finds a :keyword: which as a :value:
| which cannot be interpreted,
| such as a non-numeric value for a numeric parameter
|
| The types of error messages returned by this function are:
|_index error messages,, from parsing
|_index parsing,, error messages
| 
| - Sorry, "-xxx" is not understood.
| - Sorry, "-xxx" is ambiguous.
| - Sorry, "-xxx" appears to be missing its value (try -help).
| - Sorry, "1x3" is not an integer.
| - Sorry, "3x4" is not of form.
|
| Besides looking for marked keywords on the command line,
|_Aside Environment variables used too.
| the environment variable space is searched as well.
| For each keyword which is marked parsable but not found on the command line,
| this function searches for an environment variable of the same name.
| If this environment variable is found,
| its value is used as though the keyword and value were given
| on the command line.
|
| For example, assume you run your program like:
|
|:  set DT_COLOR=blue
|:  myprog -background 35
|
| If you have marked both ~param->color~ and ~param->background~
|_index environment variables
| as parsable, then they will both have values.
| ~param->color~ will be "blue" and ~param->background~ will be 35.
| Note that if the the :DT_BACKGROUND: environment variable
| were defined, it would have been ignored
| because it was found first on the command line.
|
| If a keyword is marked for parsing but is not in the ~argv[]~ array
|_Aside Compile-time default.
|_index defaults,, compile-time
|_index compile-time,, defaults
| and is not found as an environment variable,
| then one more place is checked for a value.
| This is the :dtprj_param_defaults: static table.
| This table is defined in :dtprj.h: and declared in :prj_dat.c:.
| It may contain a compile-time default for the parameter
| in question.
| 
| To summarize, each marked parameter generates a search in the
| following three places in the order given:
| 
| 1.	~argv[]~ array
| 2.	environment variable
| 3.	:dtprj_param_defaults: table
|
| The ~argc~ and ~argv~ arguments to a C-language :main: program
|_aside Adjust ~argc~ and ~argv~ if from :main:.
| are suitable for use with this function with one minor adjustment.
| This function examines
| all ~argv~ tokens starting with ~argv[0]~.
| As passed from :main:
| the ~argv[0]~ points to the name of the program,
| not to the first keyword as expected.
| To remedy this, you should use something like:
| 
|:  main(int argc, char *argv[])
|:  {
|:    dtparam_parse(&param, 
|:      argc-1, argv+1,					// don't want to see program name
|:      &nused);
|
| The complete list of possible keywords is:
|_index keywords,, predefined
|
|dxinclude(params4.dx) 
|
| RETURN VALUES
| Value fields in ~param~ are set.
| Bits in flag fields are set and cleared.
| An error string is placed in ~message~ in case of parse errors, otherwise
| ~message~ is set to the empty string.
| ~nused~ is set to the number of tokens used,
| even in case of parse errors.
|dxinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to get malloc() memory for string copies
| 
| CAVEATS
| You must initialize the ~param~ structure and mark the desired
| keywords before calling this function.
| 
| This function allocates memory which must be freed by calling
|_Aside Don't forget to free. 
| dtparam_free().
|
| Parse errors do not generate an error return code from
| from this function, simply an error message.
|
| SEE ALSO
| Please refer to the
|_hyper section, The :param: Function Group Introduction
| 
| The following functions are also related:
| dtparam_init()			initialize parameter structure
| dtparam_set()				mark keywords for parsing
| dt_enter_parse()			uses this function
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtparam1.c:
| 
| The following code snippet highlights this function:
| 
|:  dtparam_t param = {0};
|:  int nused;
|:  char message[80];
|:  dtparam_init(&param);				// initialize param structure
|:
|:  dtparam_set(&param, 0, 	 	        // set default for params we want
|:    &param.color, NULL, NULL, "red",	// 4-tuple for "color"
|:    NULL);							// sentinel
|:
|:  dtparam_parse(&param, 				// parse command line
|:    argc-1, argv+1, 
|:    &nused, 
|:    message, sizeof(message));
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtparam_parse(							/* parse params from argc/argv		*/
  dtparam_t *param,						/* parameter structure to use 		*/
  int argc,								/* number of argv elements 			*/
  char *argv[],							/* tokens from command line			*/
  int *nused,							/* returned number of tokens used 	*/
  char *message,						/* returned parse error message 	*/
  int lmax)								/* max length of message 			*/
{
  DT_F("dtparam_arg");
  dt_ctl_t *ctl = &param->ctl;
  int isnumber = 0;
  int i;

  DTCSTACK_HIGHWATER;

  for (i=0;								/* scan args in list				*/
       i<argc;							/* until list exhausted				*/
       i++)
  {
    table_t *t;
    table_t *u = NULL;
    char *a = argv[i];
    int la = strlen(a);
	char *v;
    int ishelp = 0;
    int n = 0;


    if (argv[i][0] != '-')				/* last flag is found?				*/
	  break;

	if (!strcmp(argv[i], "--"))			/* special "--" token found? 		*/
	{
	  i++;								/* eat this token 					*/
      break;
	}

	{
      long tmp_long;
      DT_Q(dtstr_to_long2,(ctl,			/* see if it's a negative number	*/
        argv[i], &tmp_long, 
        &isnumber));
      if (isnumber)
        break;
	}

	for (t=table; t->abbrev; t++)		/* scan parse table					*/
    if (FLAG(param, t) &				/* only for params to be parsed		*/
        DTPARAM_FLAG_PARSE)
	{
      int lt = strlen(t->abbrev);
	  if (lt == la)						/* could be exact match				*/
	  {
        if (!strcmp(t->abbrev, a))
		{
		  u = t;
          n = 1;
		  break;						/* assume only one match in table   */
		}
	  }
      else
	  {
        if (!strncmp(t->abbrev, a,		/* check for match					*/
              DT_MIN(lt, la)))
	    {
          if (u == NULL)
		  {
			u = t;
			n = 1;
		  }
          else
          if (lt > (int)strlen(u->abbrev))
		  {
            u = t;						/* remember the longest match		*/
		  }
		  else
          if (lt == (int)strlen(u->abbrev))
		  {
  		    n++;						/* count multiple matches			*/
		  }
	    }
	  }
	}

	if (n == 0)							/* no match?						*/
	{
      DT_Q(dtstr_printf,(ctl,
        message, lmax,
        "Sorry, \"%s\""
        " is not understood"
        " (try -help).", a));
	  break;
	}
	else
    if (n > 1)							/* too many matches?				*/
	{
      DT_Q(dtstr_printf,(ctl,
        message, lmax,
        "Sorry, \"%s\""
        " is ambiguous"
        " (try -help).", a));
	  break;
	}

    if (!strcmp(u->abbrev, "-help") ||	/* this is -help?				    */
        !strcmp(u->abbrev, "-HELP"))	/* or this is -HELP?			    */
      ishelp = 1;

    if (strlen(a) > strlen(u->abbrev))	/* value is in with abbrev?			*/
	{
	  v = a + strlen(u->abbrev);
	}
	else
	{
  	  if (++i >= argc)					/* no args left to hold value?		*/
        if (ishelp)						/* but this is help?			    */
		{
          v = "";						/* then we don't care			    */
		  i--;
		}
        else							/* any param besides -help?		    */
		{
          DT_Q(dtstr_printf,(ctl,
            message, lmax,
            "Sorry, \"%s\""
            " appears to be missing"
            " its value (try -help)", 
            a));
		  break;
		}
      else								/* value is next arg			    */
  	    v = argv[i];
	}

	if (u->type == DECIMAL)				/* integer parameter?				*/
	{
      int isnumber;
      DT_Q(dtstr_to_long2,(ctl, v,		/* get the value				    */
        (long *)(&VALUE(param, u)),
        &isnumber));
      if (!isnumber)					/* invalid value?					*/
	  {
		DT_Q(dtstr_printf,(ctl,
          message, lmax,
          "Sorry, %s \"%s\""
          " is not an integer.",
          a, v));
		break;
	  }
	}
	else		
	if (u->type != STRING)				/* numeric parameter?				*/
	{
      n = sscanf(v, u->format,			/* get the value					*/
        &VALUE(param, u));				/* put directly in caller's struct	*/

      if (n != 1)						/* invalid value?					*/
	  {
		DT_Q(dtstr_printf,(ctl,
          message, lmax,
          "Sorry, %s \"%s\""
          " is not of form \"%s\".",
          a, v, u->format));
		break;
	  }
	}
	else								/* string parameter?				*/
	{
      if (FLAG(param, u) &				/* it has an allocated value?		*/
          DTPARAM_FLAG_ALLOC &&
		  strlen(VALUE(param, u)) <		/* and new value is longer? 		*/
		  strlen(v))
	  {
		DT_Q(dtos_free2,(ctl,			/* free the old value			    */
          (void *)VALUE(param, u),
          F, u->abbrev));
		FLAG(param, u) &=
          ~DTPARAM_FLAG_ALLOC;
	  }
		

      if (!(FLAG(param, u) &			/* it has no allocated value?		*/
			DTPARAM_FLAG_ALLOC))
        DT_Q(dtos_malloc2,(ctl,			/* get new space for it				*/
		  (void **)&VALUE(param, u),
          strlen(v)+1,
          F, u->abbrev));

      strcpy((char *)VALUE(param, u), 
        v);
      FLAG(param, u) |=					/* set allocated flag				*/
        DTPARAM_FLAG_ALLOC;
	}
  
    FLAG(param, u) |=					/* set validity flag				*/
      DTPARAM_FLAG_VALUE;
    FLAG(param, u) &=					/* un-set default flag				*/
      ~DTPARAM_FLAG_DEFAULT;

    if (ishelp)							/* stop parsing when we get -help   */
	{
	  i++;
      break;
	}
  }

  if (nused)							/* caller wants to this?		    */
    *nused = i;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtparam_init() - initialize param structure
| 
| DESCRIPTION
|_index param structure, ~param~ structure, initializing
| dtparam_init() initializes a ~param~ structure.
|_aside Puts values in ~param~ structure.
|
| The flag fields in ~param~ are set to initial values.
| The description fields in ~param~ are set to point to
| the default descriptions.
| All other fields are cleared to zero.
|
| This function allocates no resources itself.
| In particular, it does not allocate space for the ~param~ structure.
| 
| After this function, all keywords are marked as not parsable
|_aside All keywords disabled.
| and as not having valid values.
| If you intend to use this ~parse~ structure in a call
| to dtparam_parse() and some keywords are expected,
| you should set the :DTPARAM_FLAG_PARSE: bit
| of the flag field for those keywords.
| 
| This function is required before calling dtparam_parse().
|
| This function presumes that the program environment
|_aside Make sure ~param->ctl~ is valid.
| in ~param->ctl~ is valid.
| You should never call this function immediately after allocating
| a ~param~ structure from the stack or the heap without first
| clearing the structure such as:
| 
|:  param = (dtparam_t *)malloc(sizeof(*param));
|:  DT_MEMSET(param->ctl, 0, sizeof(param->ctl));
| 
| The ~param->ctl~ environment structure within ~param~
| is not changed by this function.
|
| RETURN VALUES
| All keyword-related fields in ~param~ are set 
| to initial values.
|dxinclude(return1.dx)
| 
| ERRORS
|dxinclude(noerror.dx)
| 
| CAVEATS
| After calling this function, you must mark each desired
| keyword by setting its :DTPARAM_FLAG_PARSE: flag
| before calling dtparam_parse().
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :param: Function Group Introduction
| 
| The following function is also related:
| dtparam_free()			free resources
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtparam1.c:
| 
| The following code snippet highlights this function:
|
|:  dtparam_t param = {0};				// allocate cleared param structure
|:  dtparam_init(&param);				// initialize param structure
|:  dtparam_free(&param);				// free param resources
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtparam_init(							/* init parameter structure			*/
  dtparam_t *param)						/* param structure to init 			*/
{
  table_t *t;

  for (t=table; t->abbrev; t++)			/* go through parse table			*/
  {
	DESC(param, t) = t->desc;			/* use "standard" desc			    */
	
	HELP(param, t) = NULL;				/* clear help field 				*/
	ORDER(param, t) = 0;				/* clear order field 				*/
	FLAG(param, t) = 0;					/* clear flag field 				*/

    if (t->lengthy)						/* the value can be lengthy?		*/
      FLAG(param, t) |= 
        DTPARAM_FLAG_LENGTHY;

    if (t->type == STRING)				/* param would have a string value?	*/
	  VALUE(param, t) = NULL;			/* clear value field 				*/
    else
      VALUE(param, t) = 0;
  }
  param->version = 1;					/* mark structure allocated		    */
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtparam_free() - free space in param structure
| 
| DESCRIPTION
| dtparam_free() frees space in a ~param~ structure
|_aside Frees resources.
| which may have been allocated during parsing.
|
| This function
|_aside Clears structure.
| frees the value fields for all keywords in ~param~ with string type
| which are marked as having a valid value.
| It also frees default fields which are marked as being allocated.
| It also clears the flag bits, description fields,
| and help fields of all keywords.
|
| This function frees no resources besides
| the value fields just mentioned.
| In particular, it does not free space for the ~param~ structure.
| It also does not free any description or help fields.
|
| After this call, the ~param~ structure should not be used
|_aside Recycle with care.
| in a dtparam_parse() call without an intervening dtparam_init() call.
| 
| The ~param->ctl~ environment structure within ~param~
| is unaffected by this function.
|
| RETURN VALUES
| Frees selected value fields in ~param~.
|dxinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to free() memory
| 
| CAVEATS
| You must use this function to free resources allocated during parsing
|_aside Initialize first.
| to avoid memory leak errors when your program completes.
|
| Do not call this function unless the ~param~ structure as been
| initialized by dtparam_init().
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :param: Function Group Introduction
| 
| The following functions are also related:
| dtparam_init()			initialize ~param~ structure
| dtparam_parse()			parse command line, can allocate memory
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtparam1.c:
| 
| The following code snippet highlights this function:
|
|:  dtparam_t param = {0};
|:  dtparam_init(&param);				// initialize param structure
|:  dtparam_free(&param);				// free param resources
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtparam_free(							/* free space in param structure	*/
  dtparam_t *param)						/* param structure to operate on 	*/
{
  DT_F("dtparam_free");
  dt_ctl_t *ctl = &param->ctl;
  table_t *t;

  if (param->version == 0)				/* not allocated?				    */
    return DT_RC_GOOD;

  for (t=table; t->abbrev; t++)			/* go through parse table			*/
  {
    DESC(param, t) = NULL;				/* clear description			    */
    HELP(param, t) = NULL;				/* clear help					    */

    if (FLAG(param, t) &				/* value has been allocated?		*/
        DTPARAM_FLAG_ALLOC)
	{
      if (VALUE(param, t) != NULL)		/* the value is not null?	    	*/
  	    DT_Q(dtos_free2,(ctl, 
          (void *)VALUE(param, t),
          F, t->abbrev));
	  VALUE(param, t) = NULL;
	}

    FLAG(param, t) = 0;					/* clear flag bits				    */
  }

  param->version = 0;					/* mark structure not allocated	    */

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtparam_set() - mark keywords for later parsing
| 
| DESCRIPTION
| dtparam_set() marks one or more keywords in a ~param~ structure
| for later parsing.
|
| The variable argument list must contain one group of 4
| arguments for each keyword to be initialized,
| plus one final NULL argument to mark the end.
| The value, description, help, and flag fields of ~param~ are initialized
|_aside Keyword fields initialized.
| from the list of 4-tuples found in the variable argument list.
| ~flags~ are applied to the flag field of each keyword.
|
| This function does not actually parse anything.
|_aside Does no parsing.
|
| This function is not required.
|_aside Convenient but not required.
| It is merely a convenient means of setting up the ~param~ structure.
| If you prefer, you may set up the ~param~ structure by direct
| assignment.
| For more information on the ~param~ structure, please see
|_hyper section, The :param: Function Group Introduction
|
| This function sets up at least one keyword for parsing,
| but it can also do more than one at a time.
| You can either set up multiple keywords with one call to this function,
| or you can call this function multiple times.
|
| You will recall that the :dtparam_t: structure definition
|_aside These are the keyword fields
| contains the following four fields for each of the 50 or so
| predefined keywords.
| It is the job of this function to initialize these four fields:
|_index dtparam_t structure, :dtparam_t: structure
|_index structure,, :dtparam_t:
| 
|:  TYPE           xxx;					// current value of the keyword
|:  const char    *xxx_desc;			// keyword description
|:  const char    *xxx_help;			// keyword help text
|:  dtparam_flag_t xxx_flag;			// keyword operational flags
| 
| Each 4-tuple of variable arguments in the list contains the
|_aside This is a 4-tuple.
| following values:
| 
|:  void *address,						// keyword address
|:  char *desc,							// keyword short description
|:  char *help,							// keyword help text
|:  char *default,						// keyword default value
| 
| The first argument in the 4-tuple (~address~) identifies the keyword
| which is being marked.
| It should be the address of the value field of ~param~.
| For example, if you are setting up the :-color: keyword,
| its 4-tuple might be:
|
|:  &param->color, "Blob Colour", NULL, "blue"
|
| It is considered an error to provide a ~address~ 
| which is not the address of one of the keyword value fields in ~param~.
|
| If not NULL,
| the second argument in the 4-tuple (~desc~) 
| should point to a very short (32 characters or less)
|_Aside Keyword's descriptions.
| description of the meaning of the keyword to the current program.
| This description should be suitable for use as a data-entry
| field label on a full-screen dialog.
| The string pointed to should also be static:
| its value is copied into the ~param~ structure.
| If NULL, a predefined description string will be used.
| 
| If not NULL,
| the third argument in the 4-tuple (~help~) 
| should point to a longer help message
| describing the parameter.
| This message may be used in a pop-up window displayed
| at the human operator's request.
| Like ~desc~,
| the string pointed to by ~help~ should also be static.
| If NULL, there will be no extra help for this parameter.
|
| If not NULL,
| the fourth argument in the 4-tuple (~default~)
|_aside ~default~ is interpreted here.
| is interpreted according to the keyword type.
| The resulting value is placed in the value field for the keyword.
| ~default~ is always a string pointer,
| even when the keyword type is :long: or :double:.
| It is considered an error if the value of ~default~
| cannot be converted to the appropriate type.
| If the type of the keyword is :char *:,
| no interpretation is done,
| but, space for a copy of the string is allocated with malloc().
|_aside Memory allocated for default strings.
| This space will be freed when dtparam_free() is called.
|
| The flag field for each keyword is set as follows:
|_aside Flag field initialized.
| 
| ~flags~ argument			provides starting value, subject to modifications
| 							of other bits in this table
| :DTPARAM_FLAG_PARSE:		bit always set, meaning this keyword is now
| 							marked as parsable
| :DTPARAM_FLAG_DEFAULT:	bit set if the fourth argument of the 4-tuple 
|							(~default~) is not NULL, meaning a value for this
| 							keyword has been assigned a runtime default
| :DTPARAM_FLAG_VALUE:		bit always cleared, meaning a value for this
| 							keyword is not parsed yet
| 
| You will normally pass ~flags~ either as 0 or with
|_aside Hint to dialog manager.
| one of the following bits alone set.
| These give the dialog manager a hint on where to put the
| data-entry field with respect to the other parameter fields
| on the screen.
| If unsure, simply set ~flags~ to 0.
| This function will not set any of the above flags if any of them are
| already set for a particular param.
| Please refer to
|_hyper document, dtdoclist(dtinter, title)
|_index interactive Function Group, :interactive: Function Group,
|
| :DTPARAM_FLAG_PRIMARY:	this parameter belongs in the first group
| 							on a full-screen dialog
| :DTPARAM_FLAG_SECONDARY:	this parameter belongs in the second group
| :DTPARAM_FLAG_TERTIARY:	this parameter belongs in the third group
| :DTPARAM_FLAG_QUATERNARY:	this parameter belongs in the fourth group
| 
| The ~param->ctl~ environment structure within ~param~
| is unaffected by this function.
|
| RETURN VALUES
| Initialize fields in ~param~.
|dxinclude(return1.dx)
| 
| ERRORS
| Unsuccessful completion may be due to:
| - unable to malloc() for string copies
| - cannot interpret ~default~ for numeric keyword type
| - ~address~ is not inside ~param~ structure
| 
| CAVEATS
| You must use the dtparam_free() function to free resources 
| possibly allocated by this function.
| 
| SEE ALSO
| Please refer to the
|_hyper section, The :param: Function Group Introduction
| 
| The following functions are also related:
| dtparam_init()			initialize ~param~ structure
| dtparam_parse()			parse for keywords set up by this function
|
| EXAMPLES
| For a complete and working example program, please see
|_hyper section, Example :dtparam1.c:
| 
| The following code snippet highlights this function:
| 
|:  dtparam_t param = {0};
|:  dtparam_init(&param);				// initialize param structure
|:  dtparam_set(&param, 0, 	 	        // set default for params we want
|:    &param.color, NULL, NULL, "red",	// 4-tuple for "color"
|:    NULL);							// sentinel
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API2
dtparam_set(							/* mark keywords for parsing 		*/
  dtparam_t *param,						/* ~param~ structure to mark 		*/
  dtparam_flag_t flags,					/* flags for all keywords 			*/
  ...)
{
  dt_rc_e rc;
  va_list ap;
  va_start(ap, flags);
  rc = dtparam_set_va(param, flags, ap);
  va_end(ap);
  return rc;
}

/*..........................................................................*
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtparam_set_va(							/* mark keywords for parsing 		*/
  dtparam_t *param,						/* ~param~ structure to mark 		*/
  dtparam_flag_t flags,					/* flags for all keywords 			*/
  va_list ap)
{
  DT_F("dtparam_set");
  dt_ctl_t *ctl = &param->ctl;
  table_t *t, *u;
  char env[32];

  DTCSTACK_HIGHWATER;

  if (!(flags&DTPARAM_FLAG_PRIMARY) &&	/* it has to be one of these	    */
      !(flags&DTPARAM_FLAG_SECONDARY) &&
      !(flags&DTPARAM_FLAG_TERTIARY) &&
      !(flags&DTPARAM_FLAG_QUATERNARY))
    flags |= DTPARAM_FLAG_PRIMARY;


  while(1)
  {
    void *value;
    char *desc;
    char *help;
    const char *dflt;
	int alloc = 0;

    value = va_arg(ap, void *);			/* value in param structure			*/
    if (value == NULL)					/* end of list?						*/
      break;
    desc = va_arg(ap, char *);			/* other things from arguments		*/
    help = va_arg(ap, char *);
    dflt = va_arg(ap, char *);
    for (t=table; t->abbrev; t++)		/* search table for this value		*/
      if (value == (void *)&VALUE(param, t))
        break;
    if (t->abbrev == NULL)
      return dt_err(ctl, F,
        "value for \"%s\""
        " not in param structure",
        desc? desc: "undescribed");

	if (dflt == NULL)					/* caller is not giving default? 	*/
	{
	  char dummy1[64];					/* big enough to hold one abbrev 	*/
	  char *upper = dummy1+8;			/* memcheck+watcom bug			    */
	  int i, l;
	  strcpy(upper, "DT_");				/* prefix for name of env variable 	*/
	  for (i=1; t->abbrev[i]; i++)
	    upper[2+i] = (char)toupper(
          t->abbrev[i]);
	  upper[2+i] = '\0';

      DT_Q(dtos_getenv,(ctl,			/* peek at value from env variable	*/
        upper, env, sizeof(env), &l));

	  if (l > 0)						/* environment variable has value? 	*/
	  {
		int l2;
		DT_Q(dtos_malloc2,(ctl,			/* space for environment value 		*/
          (void **)&dflt, l+2,
          F, t->abbrev));
        alloc = 1;						/* remember we allocated this 		*/
        DT_Q(dtos_getenv,(ctl,			/* get full value from env variable	*/
          upper, (char *)dflt,
          l+2, &l2));
        if (l2 >= l+2)
          return dt_err(ctl, F,
            "env variable %s length %d"
            " longer than max %d",
            upper, l2, l+1);
	    dt_dbg(ctl, F, DT_DBG_MASK_ARG,
	  	   "%s came from env %s: \"%s\"", 
           t->abbrev, upper, dflt);
	  }
	  else
	    dflt = NULL;
	}

	if (dflt == NULL)					/* no value from caller or env?		*/
	{
	  dtprj_param_defaults_t *dflts =	/* for short 						*/
	    dtprj_param_defaults;
	  int i;
	  for (i=0; dflts[i].abbrev; i++)	/* search project defaults table 	*/
	  {
	    if (!strcmp(t->abbrev,
      	      dflts[i].abbrev))
		{
		  dflt = dflts[i].dflt;
  	      dt_dbg(ctl, F, DT_DBG_MASK_ARG,
	  	     "%s came from prj_dat: \"%s\"", 
             t->abbrev, dflt);
		  break;
		}
	  }
	}

	if (dflt == NULL)					/* no value from caller env or prj?	*/
      dt_dbg(ctl, F, DT_DBG_MASK_ARG,
 	    "%s has no default",
         t->abbrev);

    if (dflt != NULL)					/* some sort of default arrived at? */
	if (t->type == DECIMAL)				/* integer parameter?				*/
	{
      int isnumber;
      DT_Q(dtstr_to_long2,(ctl, dflt,	/* get the value				    */
        (long *)(&VALUE(param, t)),
        &isnumber));
      if (!isnumber)					/* invalid value?					*/
        return dt_err(ctl, F,
          "invalid default value"
         " for integer arg %s \"%s\"",
          t->abbrev, dflt);
	}
	else
	if (t->type != STRING)				/* numeric parameter?				*/
	{
      int n;
      n = sscanf(dflt, t->format,		/* get the value					*/
        (void *)&VALUE(param, t));		/* put directly in caller's struct	*/

      if (n != 1)						/* invalid value?					*/
        return dt_err(ctl, F,
          "invalid value for arg"
          " %s \"%s\" (format \"%s\")",
          t->abbrev, dflt, t->format);
	}
	else								/* string parameter?				*/
	{
      if (FLAG(param, t) &				/* value has been allocated?		*/
           DTPARAM_FLAG_ALLOC)
  	    DT_Q(dtos_free2,(ctl,			/* free the old value			    */
          (void *)VALUE(param, t), 
          F, t->abbrev));
	  VALUE(param, t) = dflt;			/* assign the new value */
	}

    for (u=table; u->abbrev; u++)		/* raise order of those above us	*/
      if (ORDER(param, u) >
          ORDER(param, t))
        ORDER(param, u) += 1;
    ORDER(param, t) += 1;				/* raise our order too				*/
  
    if (desc)
      DESC(param, t) = desc;
	else
	if (DESC(param, t) == NULL)
	  DESC(param, t) = t->desc;

    if (help)
      HELP(param, t) = help;

	if (!(FLAG(param, t) & DTPARAM_FLAG_PRIMARY) &&
		!(FLAG(param, t) & DTPARAM_FLAG_SECONDARY) &&
		!(FLAG(param, t) & DTPARAM_FLAG_TERTIARY) &&
		!(FLAG(param, t) & DTPARAM_FLAG_QUATERNARY))
      FLAG(param, t) |= flags;			/* get what caller is giving	    */
	
    FLAG(param, t) &=					/* this is not runtime				*/
      ~DTPARAM_FLAG_VALUE;
    if (dflt != NULL)					/* caller is giving a default?		*/
      FLAG(param, t) |=					/* mark this as default				*/
        DTPARAM_FLAG_DEFAULT;
    if (alloc)							/* we allocated space for default?	*/
      FLAG(param, t) |=					/* mark this too					*/
        DTPARAM_FLAG_ALLOC;
    FLAG(param, t) |=					/* ok to parse for this				*/
      DTPARAM_FLAG_PARSE;
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
