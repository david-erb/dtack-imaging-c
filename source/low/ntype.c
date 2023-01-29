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
| dts_parse - do argument parsing for samples
| 
| DESCRIPTION
| As yet undocumented.
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

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: ntype.c,v $ $Revision: 1.2 $");
#include <dtack/str.h>

#define NTYPE_INVALID "invalid"

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtntype_lookup(
  dt_ctl_t *ctl,
  const char *string,
  dtntype_e *v,
  char *message,
  int max)
{
  DT_F("dtntype_lookup");
  int choice;

  message[0] = '\0';

  if (!strcmp(string, "same"))
  { 
    *v = DTNTYPE_UNKNOWN;
    return DT_RC_GOOD;
  }

  DT_Q(dtstr_choice,(ctl,				/* look up ntype in list 			*/
    dt_ntype_string,
    string, &choice));
  
  if (choice == -1 || 
      choice == 0)						/* string list has "unknown" first	*/
  {
    DT_Q(dtstr_printf,(ctl, 
      message, max,
      "Sorry, precision \"%s\""
      " is not understood.", 
      string));
  }

  if (choice < -1)
  {
    DT_Q(dtstr_printf,(ctl, 
      message, max,
      "Sorry, precision \"%s\""
      " is ambiguous (%d matches).",
      string, -choice));
  }

  *v = dt_ntype_alias[choice-1];

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtntype_to_string(
  dt_ctl_t *ctl,
  dtntype_e ntype,
  char *string,
  int max)
{
  DT_F("dtntype_to_string");
  int i;
  const char *s = NULL;

  for (i=0;								/* search ntype list 				*/
	   dt_ntype_list[i] !=				/* until end of list 				*/
		 DTNTYPE_NULL;
	   i++)
  {
	if (ntype == dt_ntype_list[i])		/* found the ntype? 				*/
	{
	  s = dt_ntype_string[i+1];
	  break;
	}
  }

  if (s == NULL)						/* never found ntype in list? 		*/
	s = NTYPE_INVALID;

  DT_Q(dtstr_printf,(ctl,				/* copy ntype string to result		*/
    string, max,
    "%s", s));
  
  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtntype_sizeof(
  dt_ctl_t *ctl,
  dtntype_e ntype,
  double *size)
{
  DT_F("dtntype_sizeof");
  int i;

  for (i=0;								/* search ntype list 				*/
	   dt_ntype_list[i] !=				/* until end of list 				*/
		 DTNTYPE_NULL;
	   i++)
  {
	if (ntype == dt_ntype_list[i])		/* found the ntype? 				*/
	{
	  *size = dt_ntype_sizeof[i+1];
	  return DT_RC_GOOD;
	}
  }
  
  return dt_err(ctl, F, 
    "ntype %d is invalid", ntype);
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtntype_promote(
  dt_ctl_t *ctl,
  dtntype_e ntype1,
  dtntype_e ntype2,
  dtntype_e *ntype3,
  char *string,
  int max)
{
  DT_F("dtntype_promote");
  char s1[32], s2[32], s3[32];

  DT_Q(dtntype_to_string,(ctl,			/* check ntype1 					*/
    ntype1, s1, sizeof(s1)));
  if (!strcmp(s1, NTYPE_INVALID))
	return dt_err(ctl, F,
      "ntype1 %d is invalid",
      ntype1);

  DT_Q(dtntype_to_string,(ctl,			/* check ntype2 					*/
    ntype2, s2, sizeof(s2)));
  if (!strcmp(s2, NTYPE_INVALID))
	return dt_err(ctl, F,
      "ntype2 %d is invalid",
      ntype2);

  if (ntype1 == DTNTYPE_F2 ||
	  ntype2 == DTNTYPE_F2)
    *ntype3 = DTNTYPE_F2;
  else
  if (ntype1 == DTNTYPE_F1 ||
	  ntype2 == DTNTYPE_F1)
    *ntype3 = DTNTYPE_F1;
  else
  if (ntype1 == DTNTYPE_S4 ||
	  ntype2 == DTNTYPE_S4)
    *ntype3 = DTNTYPE_S4;
  else
  if (ntype1 == DTNTYPE_B32 ||
	  ntype2 == DTNTYPE_B32)
    *ntype3 = DTNTYPE_B32;
  else
  if (ntype1 == DTNTYPE_S2 ||
	  ntype2 == DTNTYPE_S2)
    *ntype3 = DTNTYPE_S2;
  else
  if (ntype1 == DTNTYPE_B16 ||
	  ntype2 == DTNTYPE_B16)
    *ntype3 = DTNTYPE_B16;
  else
  if (ntype1 == DTNTYPE_S1 ||
	  ntype2 == DTNTYPE_S1)
    *ntype3 = DTNTYPE_S1;
  else
    *ntype3 = DTNTYPE_B08;

  DT_Q(dtntype_to_string,(ctl,			/* string for ntype3				*/
    *ntype3, s3, sizeof(s3)));

  if (string != (char *)NULL)			/* caller wants string returned 	*/
	DT_Q(dtstr_printf,(ctl,				/* copy ntype string to result		*/
      string, max,
      "%s with %s promotes to %s",
      s1, s2, s3));
  
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
