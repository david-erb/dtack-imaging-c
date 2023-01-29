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
  19991219 DE added more special characters to dtstr_htmlencode()
  19990806 DE added dtstr_htmlencode() and cgiencode()
  19981230 DE obvious bug in dtstr_extract()
 *..........................................................................*/


/*..........................................................................
| TITLE
| The :str: Function Group
| 
|!Summary
| The :dtstr: functions provide simple string manipulation capabilities.
| They complement and enhance the standard C routines.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: str.c,v $ $Revision: 1.2 $");
#include <dtack/str.h>

/*..........................................................................
| NAME
| dtstr_choice() - find string in list
| 
| SUMMARY
| Finds a string in a given NULL-terminated list of strings.
| Does shortest unambiguous substring matching.
| Returns index of matched string.
| Returns 0 if given string is NULL, empty, or matches first list entry.
| Returns -1 if there is no match.
| Returns -n if there were multiple matches, where n is the number of
| possible matches.
| Currently no possible error conditions exist.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_choice(							/* find string in list of strings 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **choices,					/* list of strings to search 		*/
  const char *string,					/* string to be found 				*/
  int *choice)							/* returned index of match 			*/
{
  int l = string? strlen(string): 0;
  int n = 0;
  int i;
  if (l == 0)							/* null or empty string?		    */
  {
    *choice = 0;						/* default choice				    */
    return DT_RC_GOOD;
  }

  for (i=0; choices[i] != NULL; i++)	/* look through all choices		    */
  {
    if (!strcmp(choices[i], string))	/* exact match?					    */
	{
      *choice = i;
      n = 1;
      break;							/* always accept first exact match  */
	}

    if (l > (int)strlen(choices[i]))	/* choice too short for string?	    */
      continue;
    if (strncmp(choices[i], string, l))	/* string does not match choice?    */
      continue;
    *choice = i;						/* remember choice index		    */
    n++;								/* keep count of matches		    */
  }

  if (n == 0)							/* there were no matches?		    */
    *choice = -1;
  else
  if (n > 1)							/* more than one match?			    */
    *choice = -n;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_choice2() - find string in list
| 
| SUMMARY
| Finds a string in a given NULL-terminated list of strings.
| Does shortest unambiguous substring matching.
| Returns index of matched string.
| Returns 0 if given string is NULL, empty, or matches first list entry.
| Returns -1 if there is no match.
| Returns -n if there were multiple matches, where n is the number of
| possible matches.
| Currently no possible error conditions exist.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_choice2(							/* find string in list of strings 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **choices,					/* list of strings to search 		*/
  const char *string,					/* string to be found 				*/
  int *choice,							/* returned index of match 			*/
  char *message,						/* returned error message 			*/
  int lmax)								/* input max error message length 	*/
{
  dtstr_choice(ctl, choices, string,	/* get the choice 					*/
    choice);

  if (message != NULL && lmax > 0)		/* caller wants message back? 		*/
  if (*choice == -1)
    dtstr_printf(ctl, message, lmax,
      "Sorry, \"%s\" is"
      " not one of the allowed"
      " choices.",
      string);
  else
  if (*choice < 0)
    dtstr_printf(ctl, message, lmax,
      "Sorry, \"%s\""
      " matches %d of the allowed"
      " choices.",
      string, -(*choice));
  else
    message[0] = '\0';

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_to_double() - convert string to double
| 
| SUMMARY
| Convert string to double precision binary number.
| Three examples of valid strings are: 1, 1.2, -1.2e34.
| Error if string is not in the proper form.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_to_double(						/* convert string to double			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  double *value)						/* returned converted value 		*/
{
  DT_F("dtstr_to_double");
  int isnumber;
  DT_Q(dtstr_to_double2,(ctl,			/* try the conversion			    */
    string, value, &isnumber));
  if (!isnumber)						/* value is not a number?		    */
    return dt_err(ctl, F,
      "\"%s\" does not convert", string);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_to_double2() - try to convert string to double
| 
| SUMMARY
| Try to convert string to double precision binary number.
| Three examples of valid strings are: 1, 1.2, -1.2e34.
| Return value of ~*isnumber~ is 1 if conversion was successful,
| otherwise 0.
| No error if string cannot be converted.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_to_double2(						/* try to convert string to double	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  double *value,						/* returned converted value 		*/
  int *isnumber)						/* conversion success indicator 	*/
{
  double v;
  char *e;
  v = strtod(string, &e);				/* try the conversion			    */
  if (*e != '\0')						/* end of value not end of string?  */
  {
    long l;
    dtstr_to_long2(ctl, string,			/* see if it's a long			    */
      &l, isnumber);
    if (*isnumber != 0)
      *value = l;
  }
  else
  {
    *isnumber = 1;
    *value = v;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_to_long() - convert string to long integer
| 
| SUMMARY
| Convert string to long integer binary number.
| Two examples of valid strings are: 1, -234582383.
| Error if string is not in the proper form.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_to_long(							/* convert string to decimal long	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  long *value)							/* returned converted value 		*/
{
  DT_F("dtstr_to_long");
  int isnumber;
  DT_Q(dtstr_to_long2,(ctl,				/* try the conversion			    */
    string, value, &isnumber));
  if (!isnumber)						/* value is not a number?		    */
    return dt_err(ctl, F,
      "\"%s\" does not convert", string);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_to_long2() - try to convert string to long integer
| 
| SUMMARY
| Try to convert string to long integer binary number.
| Two examples of valid strings are: 1, -234582383.
| Return value of ~*isnumber~ is 1 if conversion was successful,
| otherwise 0.
| No error if string cannot be converted.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_to_long2(							/* convert string to decimal long	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  long *value,							/* returned converted value 		*/
  int *isnumber)						/* conversion success indicator 	*/
{
  long v;
  char *e;
  v = strtol(string, &e, 0);			/* try the conversion			    */
  if (*e != '\0')						/* end of value not end of string?  */
    *isnumber = 0;
  else
  {
    *isnumber = 1;
    *value = v;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_to_ulong() - convert string to long integer
| 
| SUMMARY
| Convert string to long integer binary number.
| Two examples of valid strings are: 1, -234582383.
| Error if string is not in the proper form.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_to_ulong(							/* convert string to decimal long	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  unsigned long *value)							/* returned converted value 		*/
{
  DT_F("dtstr_to_ulong");
  int isnumber;
  DT_Q(dtstr_to_ulong2,(ctl,				/* try the conversion			    */
    string, value, &isnumber));
  if (!isnumber)						/* value is not a number?		    */
    return dt_err(ctl, F,
      "\"%s\" does not convert", string);
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_to_ulong2() - try to convert string to long integer
| 
| SUMMARY
| Try to convert string to long integer binary number.
| Two examples of valid strings are: 1, -234582383.
| Return value of ~*isnumber~ is 1 if conversion was successful,
| otherwise 0.
| No error if string cannot be converted.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_to_ulong2(							/* convert string to decimal long	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be converted 			*/
  unsigned long *value,							/* returned converted value 		*/
  int *isnumber)						/* conversion success indicator 	*/
{
  long v;
  char *e;
  v = strtoul(string, &e, 0);			/* try the conversion			    */
  if (*e != '\0')						/* end of value not end of string?  */
    *isnumber = 0;
  else
  {
    *isnumber = 1;
    *value = v;
  }
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_tokenize() - separate string into tokens
| 
| SUMMARY
| Replace all occurrences of given ~separator~ character
| with the end-of-string character.
| Make no attempt to compress multiple continuous runs of
| the separator character with a single instance.
| Return the number of characters replaced, plus 1.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_tokenize(							/* separate string into tokens	    */
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to be separated 			*/
  char separator,						/* character separator 				*/
  int *ntokens)							/* returned number of tokens 		*/
{
  int n = 0;
  char *p = (char *)string;
  if (p && strlen(p))					/* no tokens if NULL or empty		*/
  {
    do {
      n++;
      p = strchr(p, separator);
      if (p)
        *p++ = '\0';
    } while (p);
  }

  if (ntokens != NULL)					/* caller wants to know how many?	*/
    *ntokens = n;

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_find_white() - find next whitespace character
| 
| SUMMARY
| Return pointer to next space, newline, tab, or end-of-string character.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_find_white(						/* find next whitespace character 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to search 				*/
  char **white)							/* returned pointer to next white 	*/
{
  while(*string != '\0' &&
		*string != '\t' &&
		*string != '\n' &&
		*string != ' ')
    string++;
  *white = string;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_find_nonwhite() - find next non-whitespace character
| 
| SUMMARY
| Return pointer to next character which is not one of:
| space, newline, tab, or end-of-string.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_find_nonwhite(					/* find next non-whitespace char	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to search 				*/
  char **nonwhite)						/* returned pointer to non-white 	*/
{
  while(*string != '\0' &&
		(*string == '\t' ||
		 *string == '\n' ||
		 *string == ' '))
    string++;
  *nonwhite = string;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_find_quote() - find next quote character
| 
| SUMMARY
| Return pointer to next character which is either a double-quote
| or the end-of-string character.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_find_quote(						/* find next quote character 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to search 				*/
  char **quote)							/* returned pointer to next quote 	*/
{
  while(*string != '\0' &&
		*string != '"')
    string++;
  *quote = string;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_argv() - make argv array from string
| 
| SUMMARY
| Parse ~string~ into whitespace separated tokens.
| Trim leading and trailing spaces from tokens.
| Keep spaces within tokens delimited by double-quote characters.
| Do not use standard shell backslash escaping.
| In no case may a double-quote character be part of a token.
| Terminate tokens by placing end-of-string characters directly into ~string~.
| Load the ~*argv~ array with pointers to the first ~maxargv~ tokens.
| Return ~*argc~ with the number of tokens found, which may exceed ~maxargv~.
| 
| END
 *..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dtstr_argv(								/* make argv from string			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* string to be parsed 				*/
  char **argv,							/* returned array of token pointers	*/
  int maxargv,							/* maximum tokens to return 		*/
  int *argc)							/* returned number of tokens found 	*/
{
  char *s = string;
  char *e = s + strlen(s);
  int n = 0;
  do {
    char *t;
    dtstr_find_nonwhite(ctl, s, &s);	/* find non-white				    */
    if (s == e)							/* end of string?				    */
      break;

    if (*s == '"')						/* token starts with a quote?	    */
      dtstr_find_quote(ctl, ++s, &t);	/* find close quote					*/
    else
      dtstr_find_white(ctl, s, &t);		/* find white				    	*/

    if (n < maxargv)					/* we can save this token?		    */
	{
      argv[n++] = s;					/* save where token starts		    */
      *t = '\0';						/* end the token				    */
	}

    s = t + 1;							/* skip the terminator			    */
  } while (s < e);

  *argc = n;
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_printf_va() - vprintf-like replacement
| 
| SUMMARY
| Substitute %-identified tokens in format using variable argument list.
| Place output string into ~string~ up to ~max~ characters.
| Silently truncate output if its length would exceed maximum.
| Always place an end-of-string character at the end, even when truncating.
| Make an effort to be reasonably compatible with the standard C vprintf(),
| but some limitations exist.
| 
| END
 *..........................................................................*/

/*..........................................................................
 * max must be at least 1, to hold the EOS
 * only supports: %NNNl[sefdxc%] where 
 *   NNN (possibly null) is decodable by strtol
 *   l is optional
 * -NNN works for efdx only
 * for e or f arg must be double
 * for ld or lx arg must be long
 * for d or x or c arg must be int
 * none of the formats within format should be longer than 15
 * none of the numeric formats should create output longer than 31
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_printf_va(						/* vprintf-like replacement			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* output string 					*/
  int max,								/* maximum output length 			*/
  const char *format,					/* format string with %-signs 		*/
  va_list ap)							/* variable argument list 			*/
{
  DT_F("dtstr_printf_va");
  int f = 0;
  int f0, i, islong;
  int s = 0;
  char *sub;
  const char *e;
  char mini[32];						/* longest format sub-string 		*/
  char tmp[64];							/* longest formatted number 		*/
  int spaces_before, spaces_after;
  int width;

  if (max < 1)
    return dt_err(ctl, F,
      "maximum string length %d too short",
      max);

  while(format[f])
  {
    if (format[f] == '%')
    {
      f0 = f;							/* remember start of format		    */
	  for (e=format+f+1;
           strchr("-.0123456789", *e);	/* see if number after the percent	*/
           e++);
      f = (int)(e - (char *)format);	/* skip the number				    */
      if (format[f] == 'l')				/* long indicator after the number?	*/
      {
        islong = 1;
        f++;
      }
      else
        islong = 0;

	  spaces_before = 0;
	  spaces_after = 0;
      DT_MEMCPY(mini, format+f0, f-f0+1);	/* get mini from format	    		*/
      mini[f-f0+1] = '\0';				/* make mini self standing			*/
      sub = tmp;						/* presume we will use tmp		    */
      switch(format[f++])
      {
        case 's': 
          sub = va_arg(ap, char *);		/* use caller's string directly	    */
		  width = atoi(format+f0+1);
		  if (width > (int)strlen(sub))
            spaces_after = width - strlen(sub);
		  if (-width > (int)strlen(sub))
            spaces_before = -width - strlen(sub);
          break;
        case 'e':
        case 'f': 
        case 'g': 
          sprintf(tmp, mini, va_arg(ap, double));
          break;
        case 'p': 
          sprintf(tmp, mini, va_arg(ap, void *));
          break;
        case 'd':
          if (islong)
		  {
			long x = va_arg(ap, long);
            sprintf(tmp, mini, x);
		  }
          else
		  {
			int x = va_arg(ap, int);
            sprintf(tmp, mini, x);
		  }
          break;
        case 'x':
        case 'u':
          if (islong)
		  {
			unsigned long x = va_arg(ap, unsigned long);
            sprintf(tmp, mini, x);
		  }
          else
		  {
			unsigned int x = va_arg(ap, unsigned int);
            sprintf(tmp, mini, x);
		  }
          break;
        case 'c':
          tmp[0] = (char)(va_arg(ap, int));
          tmp[1] = '\0';
        break;
        case '%':
          tmp[0] = '%';
          tmp[1] = '\0';
        break;
        default:
          sub = mini;
      }
      for (i=0; i<spaces_before; i++)
        if (s == max-1)
          break;
        else
          string[s++] = ' ';
      for (i=0; sub[i]; i++)			/* copy substituted string		    */
        if (s == max-1)
          break;
        else
          string[s++] = sub[i];
      for (i=0; i<spaces_after; i++)
        if (s == max-1)
          break;
        else
          string[s++] = ' ';
	}
    else
      string[s++] = format[f++];
    if (s == max-1)
      break;
  }
  string[s] = '\0';

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_printf() - printf-like replacement
| 
| SUMMARY
| Substitute %-identified tokens in format using variable argument list.
| Place output string into ~string~ up to ~max~ characters.
| Silently truncate output if its length would exceed maximum.
| Always place an end-of-string character at the end, even when truncating.
| Make an effort to be reasonably compatible with the standard C printf(),
| but some limitations exist.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API2
dtstr_printf(							/* printf-like replacement			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* output string 					*/
  int max,								/* maximum output length 			*/
  const char *format,					/* format string with %-signs 		*/
  ...)									/* variable argument list 			*/
{
  dt_rc_e rc;
  va_list ap;
  va_start(ap, format);
  rc = dtstr_printf_va(ctl,
    string, max, format, ap);
  va_end(ap);
  return rc;
}

/*..........................................................................
| NAME
| dtstr_printf() - printf-like replacement
| 
| SUMMARY
| Substitute %-identified tokens in format using variable argument list.
| Place output string into ~string~ up to ~max~ characters.
| Silently truncate output if its length would exceed maximum.
| Always place an end-of-string character at the end, even when truncating.
| Make an effort to be reasonably compatible with the standard C printf(),
| but some limitations exist.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_cpy(								/* strcpy-like replacement			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *string,							/* output string 					*/
  int max,								/* maximum output length 			*/
  const char *input)					/* input string						*/
{
  DT_F("dtstr_cpy");
  DT_Q(dtstr_printf,(ctl,
    string, max, "%s", input));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_parsechar() - return a character given a string
| 
| SUMMARY
| Parse the string, expecting to find a character or human-readable equivalent.
| Do not have an error if the string cannot be parsed,
| instead create a diagnostic message.
| The character whose value is 0 must be represented by the string "^@".
| Handle any output from dtstr_printchar().
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_parsechar(
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* input string 					*/
  char *c,								/* output character					*/
  char *message,						/* return error message 			*/
  int message_max)						/* maximum length of error message 	*/
{
  int ischar = 1;						/* presume everything is ok			*/

  if (strlen(string) == 1)				/* regular character? 				*/
    *c = string[0];
  else
  if (strlen(string) == 2 &&			/* control character?				*/
	  string[0] == '^' &&
	  ((string[1] >= 'A' && 
	    string[1] <= 'Z') ||
	   (string[1] >= 'a' && 
	    string[1] <= 'z')))
    *c = (char)(toupper(string[1]) - '@');
  else
  if (strlen(string) >= 3 &&			/* looks like a hex string? 		*/
	  string[0] == '0' &&
	  (string[1] == 'X' ||
	   string[1] == 'x'))
  {
	long value;
	dtstr_to_long2(ctl, string,			/* convert hex string to number 	*/
      &value, &ischar);
	if (value > 256)					/* number to large? 				*/
      ischar = 0;
	if (ischar)							/* number is a character? 			*/
      *c = (char)value;
  } 
  else
  if (!strcmp(string, "SPACE"))			/* named character? 				*/
    *c = ' ';
  else
  if (!strcmp(string, "ENTER"))
    *c = '\n';
  else
  if (!strcmp(string, "BACKSPACE"))
    *c = '\b';
  else
  if (!strcmp(string, "TAB"))
    *c = '\t';
  else
  if (!strcmp(string, "ESCAPE"))
    *c = 0x1b;
  else
    ischar = 0;
	  
  if (!ischar)
    dtstr_printf(ctl,
      message, message_max,
      "%s does not represent a character",
      string);
  else
  if (message != NULL &&
	  message_max > 0)
    message[0] = '\0';		

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_printchar() - output printable string for character
| 
| SUMMARY
| If ~c~ is printable, place it in the output unchanged.
| If it is non-printable, copy a human-readable equivalent instead.
| Silently truncate if output is not long enough.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_printchar(
  dt_ctl_t *ctl,						/* environment control 				*/
  char c,								/* input character					*/
  char *out,							/* output string 					*/
  int lmax)								/* maximum length of output string 	*/
{
  DT_F("dtstr_printchar");
  char s[32];

    if (c == ' ')						/* the space character?			    */
      strcpy(s, "SPACE");
    else
    if (isprint(c))						/* a printable character?		    */
      sprintf(s, "%c", c);
    else
    if (c == '\n')						/* the newline character?		    */
      strcpy(s, "ENTER");
    else
    if (c == '\b')						/* the backspace character?		    */
      strcpy(s, "BACKSPACE");
    else
    if (c == '\t')						/* the tab character?		 		*/
      strcpy(s, "TAB");
    else
    if (c >= 1 &&						/* control key?					    */ 
        c <= 26)
      sprintf(s, "^%c",
        c + 'A' - 1);
    else
    if (c == 0x1b)						/* the escape character?		    */
      strcpy(s, "ESCAPE");
    else
      sprintf(s, "0x%02x", 
        (unsigned)c & 0xff);

    DT_Q(dtstr_printf,(ctl, 
      out, lmax, "%s", s));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_printable() - replace non-printable characters in string
| 
| SUMMARY
| Copy ~string~ of length ~l~ to output of maximum length ~lmax~.
| When a non-printable character is found,
| copy a human-readable equivalent instead.
| Silently truncate if output is not long enough.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_printable(
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* input string 					*/
  int l,								/* length of input string 			*/
  char *out,							/* output string 					*/
  int lmax)								/* maximum length of output string 	*/
{
  DT_F("dtstr_printable");
  int i;
  if (l == -1)
    l = strlen(string);
  out[0] = '\0';
  for (i=0; i<l; i++)
  {
	char s[32];
	DT_Q(dtstr_printchar,(ctl,
      string[i], s, sizeof(s)));
    DT_Q(dtstr_printf,(ctl, 
      out+strlen(out), 
      lmax-strlen(out), 
      "%s%s", i?" ":"", s));
  }
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_print_ntype() - give string for number
| 
| SUMMARY
| Make an output ASCII string given a numeric value.
| The precision of the numeric value is specified by the ~ntype~ argument.
| Silently truncate if output buffer is not long enough.
| The end-of-string character is always the last character output,
| even when truncating.
| Return no error if ~ntype~ is unknown, but return a zero-length string.
| Convert numbers to the shortest possible string,
| for example the double precision number 26.0 is converted to "26".
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_print_ntype(						/* give string for ntype			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const void *v,						/* input numeric value pointer		*/
  dt_ntype_e ntype,						/* precision of the numeric value 	*/
  char *out,							/* output string buffer				*/
  int lmax)								/* maximum length of output 		*/
{
  DT_F("dtstr_print_ntype");
  char t[64];
  t[0] = '\0';
  switch(ntype)
  {
    case DT_NTYPE_B01:
      sprintf(t, " %d", (int)(*(dt_ntype_b01_t *)v) &0x0001);
    break;
    case DT_NTYPE_B08:
      sprintf(t, " %3d", (int)(*(dt_ntype_b08_t *)v) & 0x00ff);
    break;
    case DT_NTYPE_B16:
      sprintf(t, " %5d", (int)(*(dt_ntype_b16_t *)v) & 0xffffU);
    break;
    case DT_NTYPE_B32:
      sprintf(t, " %8lu", (unsigned long)(*(dt_ntype_b32_t *)v));
    break;
    case DT_NTYPE_S1:
      sprintf(t, " %3d", (int)(*(dt_ntype_s1_t *)v));
    break;
    case DT_NTYPE_S2:
      sprintf(t, " %5d", (int)(*(dt_ntype_s2_t *)v));
    break;
    case DT_NTYPE_S4:
      sprintf(t, " %8ld", (long)(*(dt_ntype_s4_t *)v));
    break;
    case DT_NTYPE_F1:
      sprintf(t, " %g", (*(dt_ntype_f1_t *)v));
    break;
    case DT_NTYPE_F2:
      sprintf(t, " %g", (*(dt_ntype_f2_t *)v));
    break;
  }
  DT_Q(dtstr_printf,(ctl, 
    out, lmax,
    "%s", strlen(t)? t: "?"));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_print_op() - give string for dt_op_e
| 
| SUMMARY
| Make an output ASCII string corresponding to ~op~.
| Silently truncate if output buffer is not long enough.
| The end-of-string character is always the last character output,
| even when truncating.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_print_op(							/* give string for op				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dt_op_e op,							/* op to make string for 			*/
  char *out,							/* output string buffer				*/
  int lmax)								/* maximum length of output 		*/
{
  DT_F("dtstr_print_op");
  const char *s;

#define CASE(C)							\
  case (C): s = #C; break;


  switch(op)
  {
	CASE(DT_OP_MIN)
	CASE(DT_OP_MAX)
    CASE(DT_OP_AVE)
    CASE(DT_OP_AND)
    CASE(DT_OP_OR)	
    CASE(DT_OP_XOR)	
    CASE(DT_OP_INVERT)
    CASE(DT_OP_ADD)	
    CASE(DT_OP_SUB)	
    CASE(DT_OP_SUBFROM)
    CASE(DT_OP_SUBABS)	
    CASE(DT_OP_MUL)		
    CASE(DT_OP_DIV)		
    CASE(DT_OP_ROW)		
    CASE(DT_OP_COL)		
    CASE(DT_OP_LUT)		
    CASE(DT_OP_CLEAR)		
    CASE(DT_OP_SET)		
    CASE(DT_OP_ABS)		
    CASE(DT_OP_CLIP)		
    CASE(DT_OP_CAST)
	default:
	  s = "DT_OP_UNKNOWN";
  }

  DT_Q(dtstr_printf,(ctl, 
    out, lmax, "%s", s+6));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_extract() - extract field
| 
| SUMMARY
| Extract ~nth~ field delimited by ~separator~ from ~string~.
| Return first field when ~nth~ is 0.
| Return "" if ~nth~ is greater than 0 and not that many fields exist.
| Return ~s~ if ~nth~ is 0 and there are no ~separator~ occurrences.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_extract(							/* extract field					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *string,					/* string to extract from 			*/
  const char *separator,				/* field separator string 			*/
  long nth,								/* field index wanted 				*/
  char *field,							/* where field returned 			*/
  int max)								/* max length of return string 		*/
{
  DT_F("dtstr_extract");
  int i, l;
  const char *p, *q;

  if (max <= 0)
	return DT_RC_GOOD;

  field[0] = '\0';
  l = strlen(separator);
  p = string;
  for (i=0; i<nth; i++)
  {
    p = strstr(p, separator);
	if (p == NULL)
	  return DT_RC_GOOD;
	p += l;
  }
  q = p;
  p = strstr(p, separator);
  if (p == NULL)
    p = string + strlen(string);
  l = DT_MIN((p - q), max-1);
  strncpy(field, q, l);
  field[l] = '\0';
  return DT_RC_GOOD;
}


/*..........................................................................
| NAME
| dtstr_htmlencode() - replace html special characters
| 
| SUMMARY
| Replace html special characters in ~input~.
| Write result to ~html~ up to ~max~ bytes.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_htmlencode(						/* replace html special characters	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *input,					/* input string 					*/
  char *html,							/* html string to write to			*/
  int max)								/* maximum output length 			*/
{
  DT_F("dtstr_htmlencode");
  int i, l, n;
  const char *p;
  char *q;
  char *special = "<>&\"';";
  char *replace[] = {
	"&lt;",
	"&gt;",
	"&amp;",
	"&quot;",
	"&#39;",
	"&#59;"
  };

  if (max <= 0)
	return DT_RC_GOOD;

  q = html;
  l = strlen(input);
  for (i=0; i<l; i++)					/* scan input characters 			*/
  {
	if (p=strchr(special, input[i]))	/* special input character? 		*/
	{
	  p = replace[p-special];			/* used replacement string 			*/
	  n = strlen(p);
	}
	else								/* not a special input character? 	*/
	{
	  p = input+i;
	  n = 1;
	}
	if (q-html+n >= max-1)				/* end of output space? 			*/
	  break;
	DT_MEMCPY(q, p, n);
	q += n;
  }

  *q = '\0';							/* terminate html string 			*/

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtstr_cgiencode() - replace cgi special characters
| 
| SUMMARY
| Replace cgi special characters in ~input~.
| Write result to ~cgi~ up to ~max~ bytes.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_cgiencode(						/* replace cgi special characters	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char *input,					/* input string 					*/
  char *cgi,							/* cgi string to write to			*/
  int max)								/* maximum output length 			*/
{
  DT_F("dtstr_cgiencode");
  int i, l, n;
  const char *p;
  char *q;
  char t[32];
  char *special = "~!@#$%^&*()+={}[]'\",./<>?\\|";

  if (max <= 0)
	return DT_RC_GOOD;

  q = cgi;
  l = strlen(input);
  for (i=0; i<l; i++)					/* scan input characters 			*/
  {
	if (input[i] == ' ')
	{
	  p = "+";
	  n = 1;
	}
	else
	if (p=strchr(special, input[i]))	/* special input character? 		*/
	{
	  sprintf(t, "%%%02X", input[i]);
	  p = t;							/* use replacement string 			*/
	  n = strlen(p);
	}
	else								/* not a special input character? 	*/
	{
	  p = input+i;
	  n = 1;
	}
	if (q-cgi+n >= max-1)				/* end of output space? 			*/
	  break;
	DT_MEMCPY(q, p, n);
	q += n;
  }

  *q = '\0';							/* terminate cgi string 			*/

  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/


/*..........................................................................
| NAME
| dtstr_cgiencode() - replace cgi special characters
| 
| SUMMARY
| Replace cgi special characters in ~input~.
| Write result to ~cgi~ up to ~max~ bytes.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtstr_cgidecode(						/* replace cgi special characters	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  char *p)								/* input/output string 				*/
{
  DT_F("dtstr_cgidecode");
  char *q = p;
  int i;
  int l = strlen(p) + 1;				/* q needs the EOS too 				*/
  for (i=0; i<l; i++)
  {
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
