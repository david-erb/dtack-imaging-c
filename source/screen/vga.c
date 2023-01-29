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

DT_RCSID("source/screen $RCSfile: vga.c,v $ $Revision: 1.1 $");

#include <dtack/str.h>
#include <dtack/vga.h>

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtvga_lookup_resolution(
  dt_ctl_t *ctl,
  char *string,
  int *xresolution,
  int *yresolution,
  const char ***resolutions)
{
  DT_F("dtvga_lookup_resolution");
  int choice;
# define NCHOICES 16
  static const char *choices[NCHOICES] = {
    "vga",
    "low",
    "medium",
    "high",
    "320",
	"640",
    "800",
    "1024",
    "320x200",
	"640x400",
	"640x480",
    "800x600",
    "1024x768",
    "320x240",
    "x",
    NULL,
  };
  static const int sel[NCHOICES] = {
    0, /* vga    */
    2,
    3,
    4,
    0, /* 320     */
    2,
    3,
    4,
    0, /* 320     */
    1, /* 640x400 */
    2,
    3,
    4,
    5, /* 320x240 mode X */
    5,
    0
  };
  static const struct {
    int x;
    int y;
  } res[] = {
    { 320, 200},
    { 640, 400},
    { 640, 480},
    { 800, 600},
    {1024, 768},
    { 320, 240}
  };

  if (string != NULL)					/* caller wants to do lookup?	    */
  {
    DT_Q(dtstr_choice,(ctl,				/* check against list of choices    */
      choices, string, &choice));
	if (choice == -1)
      return dt_err(ctl, F,
        "unknown resolution \"%s\"",
        string);
	else
	if (choice < -1)
      return dt_err(ctl, F,
        "ambiguous resolution \"%s\"",
        string);
	else								/* we must have a valid one		    */
	{	
	  *xresolution = res[sel[choice]].x;
	  *yresolution = res[sel[choice]].y;
	}
  }

  if (resolutions != NULL)				/* caller wants the list?		    */
    *resolutions = &choices[0];

  return DT_RC_GOOD;
}

/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dtvga_lookup_chipset(
  dt_ctl_t *ctl,
  char *string,
  dtvga_chipset_e *chipset,
  const char ***chipsets)
{
  DT_F("dtvga_lookup_chipset");
  int choice;
  static const char *choices[44] = {	/* must match dtvga_chipset_e		*/
    "auto",
    "acumos",
    "ati",
    "ati-18800",
    "ati-18800-1",
    "ati-28800",
    "ahead",
    "ahead-a",
    "ahead-b",
    "ct",
    "ct-451",
    "ct-452",
    "ct-453",
    "cirrus",
    "everex",
    "genoa",
    "ncr",
    "oak",
    "paradise",
    "wd",
    "wd-c00",
    "wd-c01",
    "realtek",
    "trident",
    "trident-8800",
    "trident-8900",
    "trident-9000",
    "tseng-et3000",
    "tseng-et4000",
    "vesa",
    "video7",
    "avance",
    "mxic",
    "s3",
    "primus",
    "vga",
    "vgax",
    "cirrus-5400",
    "cirrus-6400",
    "ncr-77Cx2",
    "oak-oti067",
    "oak-oti077",
    "oak-oti087",
    NULL
  };

  if (string != NULL)
  {
    DT_Q(dtstr_choice,(ctl,				/* check against list of choices    */
      choices, string, &choice));
    if (choice == -1)
      return dt_err(ctl, F,
        "unknown chipset \"%s\"",
        string);
    else
    if (choice < -1)
      return dt_err(ctl, F,
        "ambiguous chipset \"%s\"",
        string);
    else								/* we must have a valid one		    */
      *chipset = (dtvga_chipset_e)
        choice;
  }

  if (chipsets != NULL)					/* caller wants the list?		    */
    *chipsets = &choices[0];

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
