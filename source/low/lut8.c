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
| All functions Return DTOS_RANWELL on success.  
| Any other return value indicates failure.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: lut8.c,v $ $Revision: 1.1 $");
#include <dtack/lut8.h>
#include <dtack/str.h>

static unsigned char standard_red[DTLUT8_NSTANDARD] =
  {000, 000, 000, 255, 255, 000, 000, 255, 255};
static unsigned char standard_green[DTLUT8_NSTANDARD] =
  {000, 000, 000, 000, 000, 255, 255, 255, 255};
static unsigned char standard_blue[DTLUT8_NSTANDARD] =
  {000, 000, 255, 000, 255, 000, 255, 000, 255};

static const char *standard_name[DTLUT8_NSTANDARD+1] = 
  {
	"transparent",
	"black",
	"blue",
	"red",
	"magenta",
	"green",
	"cyan",
	"yellow",
	"white",
    NULL
  };


/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlut8_standard(
  dt_ctl_t *ctl,
  dtlut8_t *lut)
{
  int i;

  for (i=0; i<256; i++)					/* make it all gray 				*/
    lut[i].r =
    lut[i].g =
    lut[i].b =
    lut[i].i = (dtlut8_data_t)i;

  for (i=0; i<DTLUT8_NSTANDARD; i++)
  {
	int j = dtlut8_standard_index[i];
    lut[j].r = standard_red[i];
    lut[j].g = standard_green[i];
    lut[j].b = standard_blue[i];
  }
    
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlut8_name_to_index(
  dt_ctl_t *ctl,
  const char *name,
  int *idx)
{
  DT_F("dtlut8_name_to_index");
  DT_Q(dtstr_choice,(ctl, standard_name, name, idx));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtlut8_index_to_name(
  dt_ctl_t *ctl,
  int idx,
  char *name)
{
  if (idx >= 0 && idx < DTLUT8_NSTANDARD)
    strcpy(name, standard_name[idx]);
  else
    strcpy(name, "unknown color");

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
