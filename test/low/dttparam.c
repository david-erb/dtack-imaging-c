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
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/t.h>

DT_RCSID("test $RCSfile: dttparam.c,v $ $Revision: 1.8 $");

#define DTPARAM_FLAG_ALLOC (0x0010)

#define USAGE "[-Dnnnn]"

#define NOMATCH   "is not understood"
#define AMBIGUOUS "is ambiguous"
#define NOVALUE   "appears to be missing its value (try -help)"
#define BADINT    "is not an integer"
#define BADFORM   "is not of form"

#define CHECK1 *assert_fail = *assert_fail + DT_RC_GOOD != dtt_check
#define CHECK2 \
  assert_fail += DT_RC_GOOD != dtt_check(ctl, strlen(message) == 0, \
    "empty message (\"%s\")", message); \
  CHECK1
#define CHECK3(NUSED) *assert_fail = *assert_fail + \
   DT_RC_GOOD != dtt_check(ctl, nused==NUSED, \
  strcat(strcpy(tmp, s), ", nused %d (%d)"), nused, NUSED);

/*..........................................................................
- NAME
- dttparam_parse_string() - parse params from string
- 
- DESCRIPTION
- Parse values from a string form.   ~string~ is a normal string
- containing keyword tokens and their values.  Whitespace in the
- string separates the tokens.  Whitespace may be spaces, tabs, or
- newlines.
-
- The contents of ~string~ is not changed.  Upon return from this function,
- the caller is free to reuse or free the ~string~ space.  This function
- will make priv copies of any strings necessary.  The function
- dtparam_free() should be called to free these priv copies.
-
- For all values found, set FLAG_VALUE bit in the ~param-~xxx_flag> 
- indicating that values in the param structure have been initialized.
- 
- RETURN VALUES
- Function return value will be DT_RC_GOOD on success.  
- Any other return value indicates failure or noncompletion.
- When DT_RC_GOOD is not returned, any other return values may not be valid.
- 
- Return argument ~nused~ to indicated how many args in the argv array
- were actually used.
-
- ERRORS
- No possible errors.
- 
- END
 *..........................................................................*/

#define ISWHITE(C) (C == newline || C == space || C == tab || C == nul)

dt_rc_e
dttparam_parse_string(					/* parse params from string			*/
  dtparam_t *param,
  const char *string,
  int *nused,
  char *message,
  int lmax)
{
  DT_F("dttparam_parse_string");
  dt_ctl_t *ctl = &param->ctl;
  int i, l, n;
  int argc;
  char **argv;
  char *s;
  dt_rc_e rc = DT_RC_GOOD;
  static const char newline = '\n';
  static const char space = ' ';
  static const char tab = '\t';
  static const char nul = '\0';

  *nused = 0;

  l = strlen(string);
  if (l > 0)							/* not parsing empty string?	    */
  {
    DT_Q(dtos_strdup,(ctl, string,		/* we will stomp on this string		*/
      &s));

    argc = 0;
    for (i=0; i<l; i++)
      if (ISWHITE(s[i+1]) &&
          !ISWHITE(s[i]))
	  {
        argc++;							/* count tokens						*/
	    s[i+1] = '\0';					/* and mark token ends				*/
  	  }

    if (argc > 0)						/* any arguments?				    */
    {
      n = argc * sizeof(*argv);			/* get space for argv pointers		*/
      DT_Q(dtos_malloc2,(ctl, 
        (void **)&argv, n,
        F, "argv array"));

      argc = 0;
      for (i=0; i<l; i++)				/* find token beginnings			*/
        if (!ISWHITE(s[i]) &&
            (i==0 || ISWHITE(s[i-1])))
          argv[argc++] = &s[i];

      DT_G(dtparam_parse,(param,		/* do the parsing					*/
        argc, argv, nused,
        message, lmax));

      DT_Q(dtos_free2,(ctl, argv, 
        F, "argv array"));
    }
    DT_Q(dtos_free2,(ctl, s, 
      F, "param string dup"));
  }

  return rc;
}
#undef ISWHITE

/*..........................................................................*/

static
dt_rc_e
dttparam_test1(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttparam_test1");
  dtparam_t test_param;
  char *s;
  char tmp[128];
  int nused;
  char message[128];
  dt_rc_e rc = DT_RC_GOOD;

  DT_MEMSET(&test_param, 0,				/* clear param structure			*/
    sizeof(test_param));
  DT_GI(dtparam_init,(&test_param));	/* init param for testing 			*/
  test_param.ctl = *ctl;				/* but use our ctl structure	    */

  DTPARAM_DEFAULT_P(&test_param,		/* default for debug				*/
    x, 0xffff);		
  DTPARAM_DEFAULT_P(&test_param,		/* potentially ambiguous			*/
    min, 1.0);
  DTPARAM_DEFAULT_P(&test_param,		/* default for string				*/
    precision, "xyzw");

  DT_GI(dtparam_set,(&test_param, 0,	/* another way of initializing	    */
    &test_param.max, NULL, NULL, "1.1",
    &test_param.input, NULL, NULL, "",
    &test_param.output, NULL, NULL, "",
    &test_param.win, NULL, NULL, "IPC",	/* potentially ambiguous			*/
    &test_param.w, NULL, NULL, "100",	/* potentially ambiguous			*/
    &test_param.help, NULL, NULL, "xx",
    NULL));

  message[0] = '\0';

  DT_GI(dttparam_parse_string,(&test_param, "", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.x == 0xffff, "hex default after empty string");

  DT_GI(dttparam_parse_string,(&test_param, " ", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.x == 0xffff, "hex default after blank char");

  DT_GI(dttparam_parse_string,(&test_param, "         ", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.x == 0xffff, "hex default after blank string");

  DT_GI(dttparam_parse_string,(&test_param, s="-x0xfe", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.x == 0xfe, s);

  DT_GI(dttparam_parse_string,(&test_param, s="-x 0xff", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.x == 0xff, s);

  DT_GI(dttparam_parse_string,(&test_param, s="-x 100", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.x == 100, s);

  DT_GI(dttparam_parse_string,(&test_param, s="-x0xfe", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.x == 0xfe, s);

  DT_GI(dttparam_parse_string,(&test_param, "", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.x == 0xfe, "hex leftover");



  DT_GI(dttparam_parse_string,(&test_param, "", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.max == 1.1, "double default");

  DT_GI(dttparam_parse_string,(&test_param, s="-ma 3", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.max == 3, s);

  DT_GI(dttparam_parse_string,(&test_param, s="-max 4.5678",
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.max == 4.5678, s);
  CHECK3(2);

  DT_GI(dttparam_parse_string,(&test_param, s="-max 9e87", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.max == 9e87, s);

  DT_GI(dttparam_parse_string,(&test_param, "", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, test_param.max == 9e87, "double leftover");



  DT_GI(dttparam_parse_string,(&test_param, "", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.precision, "xyzw"), s="string default");
  CHECK3(0);

  DT_GI(dttparam_parse_string,(&test_param, s="-pre poof", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.precision, "poof"), s);

  DT_GI(dttparam_parse_string,(&test_param, s="-inpu snarl", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.input, "snarl"), s);

  DT_GI(dttparam_parse_string,(&test_param, s="-output -output", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.output, "-output"), s);

  DT_GI(dttparam_parse_string,(&test_param, "", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.output, "-output"), "string leftover");



  DT_GI(dttparam_parse_string,(&test_param, s="-win ABC", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "ABC"), s);
  CHECK2(ctl, test_param.w == 100, "w == 100 (%ld)", test_param.w);

  DT_GI(dttparam_parse_string,(&test_param, s="-w 300", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "ABC"), "win ABC still");
  CHECK2(ctl, test_param.w == 300, "w == 300 (%ld)", test_param.w);

  DT_GI(dttparam_parse_string,(&test_param, s="-w301", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "ABC"), "win ABC still");
  CHECK2(ctl, test_param.w == 301, "w == 301 (%ld)", test_param.w);

  DT_GI(dttparam_parse_string,(&test_param, s="-w 400 -win XYZ", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "XYZ"), s);
  CHECK2(ctl, test_param.w == 400, "w == 400 (%ld)", test_param.w);
  CHECK3(4);

  DT_GI(dttparam_parse_string,(&test_param, s="-w401 -winJKL", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "JKL"), s);
  CHECK2(ctl, test_param.w == 401, "w == 401 (%ld)", test_param.w);


										/* ................................ */
										/* end at first non keyword			*/
  DT_GI(dttparam_parse_string,(&test_param, s="-w 155 -max 166 -output yy1", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "JKL"), s);
  CHECK2(ctl, test_param.w == 155, "w == 155 (%ld)", test_param.w);
  CHECK2(ctl, test_param.max == 166, "max == 166 (%g)", test_param.max);
  CHECK2(ctl, !strcmp(test_param.output, "yy1"), "output %s (\"%s\")",
    test_param.output, "yy1");
  CHECK3(6);
  DT_GI(dttparam_parse_string,(&test_param, s="-w 177 0 -max 188 -output zz1", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "JKL"), s);
  CHECK2(ctl, test_param.w == 177, "w == 177 (%ld)", test_param.w);
  CHECK2(ctl, test_param.max == 166, "max == 166 (%g)", test_param.max);
  CHECK2(ctl, !strcmp(test_param.output, "yy1"), "output %s (\"%s\")",
    test_param.output, "yy1");
  CHECK3(2);
  DT_GI(dttparam_parse_string,(&test_param, s="-w 188 -max 199 -1 -output zz1", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "JKL"), s);
  CHECK2(ctl, test_param.w == 188, "w == 188 (%ld)", test_param.w);
  CHECK2(ctl, test_param.max == 199, "max == 199 (%g)", test_param.max);
  CHECK2(ctl, !strcmp(test_param.output, "yy1"), "output %s (\"%s\")",
    test_param.output, "yy1");
  CHECK3(4);
  DT_GI(dttparam_parse_string,(&test_param, s="w -w 111 -max 122 -output xx1", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "JKL"), s);
  CHECK2(ctl, test_param.w == 188, "w == 188 (%ld)", test_param.w);
  CHECK2(ctl, test_param.max == 199, "max == 199 (%g)", test_param.max);
  CHECK2(ctl, !strcmp(test_param.output, "yy1"), "output %s (\"%s\")",
    test_param.output, "yy1");
  CHECK3(0);



										/* ................................ */
										/* special -- token 				*/
  DT_GI(dttparam_parse_string,(&test_param, s="-w33 -max44 -output xx", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "JKL"), s);
  CHECK2(ctl, test_param.w == 33, "w == 33 (%ld)", test_param.w);
  CHECK2(ctl, test_param.max == 44, "max == 44 (%g)", test_param.max);
  CHECK2(ctl, !strcmp(test_param.output, "xx"), "output %s (\"%s\")",
    test_param.output, "xx");
  CHECK3(4);
  DT_GI(dttparam_parse_string,(&test_param, s="-w 55 -max 66 -output yy", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "JKL"), s);
  CHECK2(ctl, test_param.w == 55, "w == 55 (%ld)", test_param.w);
  CHECK2(ctl, test_param.max == 66, "max == 66 (%g)", test_param.max);
  CHECK2(ctl, !strcmp(test_param.output, "yy"), "output %s (\"%s\")",
    test_param.output, "yy");
  CHECK3(6);
  DT_GI(dttparam_parse_string,(&test_param, s="-w 77 -- -max 88 -output zz", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "JKL"), s);
  CHECK2(ctl, test_param.w == 77, "w == 77 (%ld)", test_param.w);
  CHECK2(ctl, test_param.max == 66, "max == 66 (%g)", test_param.max);
  CHECK2(ctl, !strcmp(test_param.output, "yy"), "output %s (\"%s\")",
    test_param.output, "yy");
  CHECK3(3);
  DT_GI(dttparam_parse_string,(&test_param, s="-w 88 -max 99 -- -output zz", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "JKL"), s);
  CHECK2(ctl, test_param.w == 88, "w == 88 (%ld)", test_param.w);
  CHECK2(ctl, test_param.max == 99, "max == 99 (%g)", test_param.max);
  CHECK2(ctl, !strcmp(test_param.output, "yy"), "output %s (\"%s\")",
    test_param.output, "yy");
  CHECK3(5);
  DT_GI(dttparam_parse_string,(&test_param, s="-- -w 11 -max 22 -output xx", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.win, "JKL"), s);
  CHECK2(ctl, test_param.w == 88, "w == 88 (%ld)", test_param.w);
  CHECK2(ctl, test_param.max == 99, "max == 99 (%g)", test_param.max);
  CHECK2(ctl, !strcmp(test_param.output, "yy"), "output %s (\"%s\")",
    test_param.output, "yy");
  CHECK3(1);



										/* ................................ */
  test_param.x = 203;
  DT_GI(dttparam_parse_string,(&test_param, s="-help -x -x 204 -ZZZ", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.help, "-x"), s);
  CHECK2(ctl, test_param.x == 203, "x == 203 (%ld)", test_param.x);
  CHECK3(2);

  test_param.x = 203;
  DT_GI(dttparam_parse_string,(&test_param, s="-help0 -x", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.help, "0"), s);
  CHECK2(ctl, test_param.x == 203, "x == 203 (%ld)", test_param.x);
  CHECK3(1);

  test_param.x = 203;
  DT_GI(dttparam_parse_string,(&test_param, s="-x 204 -help1", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.help, "1"), s);
  CHECK2(ctl, test_param.x == 204, "x == 204 (%ld)", test_param.x);
  CHECK3(3);

  test_param.x = 203;
  DT_GI(dttparam_parse_string,(&test_param, s="-x 204 -help", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.help, ""), s);
  CHECK2(ctl, test_param.x == 204, "x == 204 (%ld)", test_param.x);
  CHECK2(ctl, test_param.help_flag & DTPARAM_FLAG_VALUE, "help has value");
  CHECK3(3);

  DT_GI(dttparam_parse_string,(&test_param, s="-help", 
    &nused, message, sizeof(message)));
  CHECK2(ctl, !strcmp(test_param.help, ""), s);
  CHECK2(ctl, test_param.help_flag & DTPARAM_FLAG_VALUE, "help has value");

										/* --------------------------------	*/
  if (rc == DT_RC_GOOD)
  {
    dttparam_parse_string(&test_param, s="-m 0", 
      &nused, message, sizeof(message));
    CHECK1(ctl, !!strstr(message, AMBIGUOUS), 
      "%s: %s", s, message);
    CHECK3(0);
  
    dttparam_parse_string(&test_param, s="-win ABC -w",
      &nused, message, sizeof(message));
    CHECK1(ctl, !!strstr(message, NOVALUE), 
      "%s: %s", s, message);
    CHECK3(3);
  
    dttparam_parse_string(&test_param, s="-w x",
      &nused, message, sizeof(message));
    CHECK1(ctl, !!strstr(message, BADINT), 
      "%s: %s", s, message);
    CHECK3(1);
  
    dttparam_parse_string(&test_param, s="-w 1 -max x",
      &nused, message, sizeof(message));
    CHECK1(ctl, !!strstr(message, BADFORM), 
      "%s: %s", s, message);
    CHECK3(3);
  
    DT_GI(dtos_free2,(ctl, (void *)test_param.help,
      F, "previous help value"));
    test_param.help_flag &= ~DTPARAM_FLAG_VALUE;
    test_param.help_flag &= ~DTPARAM_FLAG_ALLOC;
    dttparam_parse_string(&test_param, s="-y 0 -help",
      &nused, message, sizeof(message));
    CHECK1(ctl, !!strstr(message, NOMATCH), 
      "%s: %s", s, message);
    CHECK1(ctl, !(test_param.help_flag & DTPARAM_FLAG_VALUE),
      "that help has no value");
  }

  DT_GI(dtparam_free,(&test_param));	/* free test param strings		    */

  return DT_RC_GOOD;
}

/*..........................................................................*/
#   define EMIT sprintf(NULL,
#   define TIME ); sprintf(NULL, "%c\n", 0x0d);
#   define DX(XX) ((XX) * 110)
#   define DY(YY) ((YY) * 220)
#   define AX(XX) DX(XX + 48 * (vbform->y>32))
#   define AY(YY) DY(YY - 25 * (vbform->y>32))
typedef struct {
  int y;
  int i;
} vbform_t;

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
callback_vbform(
  dt_ctl_t *ctl,
  void *arg,
  char *abbrev,
  char *desc,
  char *val,
  dtparam_flag_t *flag)
{
  vbform_t *vbform = (vbform_t *)arg;

  if (!strcmp(abbrev, "-HELP"))
    abbrev = "-help2";

  EMIT "  Begin Label labela"               TIME
  EMIT "    Caption = \"%s\"", desc         TIME
  EMIT "    Left    = %d", AX(1)            TIME
  EMIT "    Top     = %d", AY(vbform->y)    TIME
  EMIT "    Width   = %d", DX(20)           TIME
  EMIT "    Height  = %d", DY(1)            TIME
  EMIT "    Index   = %d", vbform->i        TIME
  EMIT "    Visible = %d", 0                TIME
  EMIT "  End"                              TIME

  EMIT "  Begin TextBox texta"              TIME
  EMIT "    Text    = \"%s\"", abbrev       TIME
  EMIT "    Left    = %d", AX(24)           TIME
  EMIT "    Top     = %d", AY(vbform->y)    TIME
  EMIT "    Width   = %d", DX(20)           TIME
  EMIT "    Height  = %d", DY(1)            TIME
  EMIT "    Index   = %d", vbform->i        TIME
  EMIT "    Visible = %d", 0                TIME
  EMIT "  End"                              TIME

  vbform->i++;
  vbform->y++;

  return DT_RC_GOOD;
}

/*..........................................................................*/

#ifdef NONO

static
dt_rc_e
DTCONFIG_API1
callback_invisible(
  dt_ctl_t *ctl,
  void *arg,
  char *abbrev,
  char *desc,
  char *val,
  dtparam_flag_t *flag)
{
  if (!strcmp(abbrev, "-HELP"))
    abbrev = "-help2";

  EMIT "  %s_label.Visible = 0", abbrev+1  TIME
  EMIT "  %s_text.Visible = 0",  abbrev+1  TIME

  return DT_RC_GOOD;
}

#endif

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
callback_const(
  dt_ctl_t *ctl,
  void *arg,
  char *abbrev,
  char *desc,
  char *val,
  dtparam_flag_t *flag)
{
  vbform_t *vbform = (vbform_t *)arg;

  if (!strcmp(abbrev, "-HELP"))
    abbrev = "-help2";

  EMIT "  Const field_%s = %d", abbrev+1, vbform->i++   TIME

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttparam_vbform(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttparam_vbform");
  dtparam_t test_param;
  vbform_t vbform;
  dt_rc_e rc = DT_RC_GOOD;

  DT_MEMSET(&test_param, 0,				/* clear param structure			*/
    sizeof(test_param));

  DT_GI(dtparam_init,(&test_param));	/* init param for testing 			*/
  test_param.ctl = *ctl;				/* but use our ctl structure	    */

  DT_GI(dtparam_setflags,(&test_param,	/* let them all hang out		    */
    DTPARAM_FLAG_PARSE));

  vbform.i = 0;
  vbform.y = 8;

  EMIT "' ------------------------------------------'" TIME
  EMIT "VERSION 2.00" TIME
  EMIT "Begin Form dtparama" TIME
  EMIT "  ClientHeight    =   7410" TIME
  EMIT "  ClientLeft      =   1050" TIME
  EMIT "  ClientTop       =   1425" TIME
  EMIT "  ClientWidth     =   9270" TIME
  EMIT "  Height          =   7815" TIME
  EMIT "  Left            =    990" TIME
  EMIT "  ScaleHeight     =   7410" TIME
  EMIT "  ScaleWidth      =   9270" TIME
  EMIT "  Top             =   1080" TIME
  EMIT "  Width           =   9390" TIME
  EMIT "' ------------------------------------------'" TIME
  DT_GI(dtparam_callback,(
    &test_param,
    callback_vbform, &vbform));
  EMIT "End" TIME

  EMIT "' ------------------------------------------'" TIME
  DT_GI(dtparam_callback,(
    &test_param,
    callback_const, &vbform));

  EMIT "' ------------------------------------------'" TIME
  EMIT "Sub Form_Load ()" TIME
  EMIT "End Sub" TIME

  EMIT "' ------------------------------------------'" TIME

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_TEST(dttparam)
{
  DT_F("dttparam");
  dt_ctl_t *ctl = &param->ctl;
  int n;
  int assert_fail = 0;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DTPARAM_DEFAULT(dbg, 0);				/* set default for params we want	*/
  DTPARAM_DEFAULT(op, "test1");

  DT_GI(dt_enter_param,(				/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;

  if (argc != 0)
    return dt_usage_say(
      param, USAGE);

  if (rc == DT_RC_GOOD)
  if (!strcmp(param->op, "test1"))
  {
    DT_GI(dttparam_test1,(ctl, &assert_fail));
  }

  if (rc == DT_RC_GOOD)
  if (!strcmp(param->op, "vb.frm"))
  {
    DT_GI(dttparam_vbform,(ctl, &assert_fail));
  }

  DT_I(dt_leave,(param));				/* clean up before leaving			*/

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
    return rc;
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
