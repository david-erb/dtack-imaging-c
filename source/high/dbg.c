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
DT_RCSID("source/high $RCSfile: dbg.c,v $ $Revision: 1.1 $");
#include <dtack/str.h>

#include <dtack/mem.h>

/*..........................................................................
| NAME
| dtimage_to_string() - describe image as string
| 
| SUMMARY
| Output a string describing the image.
| Include ~desc~ as part of the string only if it is not NULL.
| If ~desc~ is NULL, use the image's memory address as its identifier.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_to_string(						/* describe image as string			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image being operated on			*/
  const char *desc,						/* image description 				*/
  char *string,							/* returned string 					*/
  int max)								/* max length of string 			*/
{
  DT_F("dtimage_str");
  char t[32];
  const char *d;
  char d2[32];
  DT_Q(dtntype_to_string,(ctl,
    image->ntype, t, sizeof(t)));

  if (desc != NULL &&					/* caller is giving description? 	*/
	  desc[0] != '\0')
	d = desc;							/* use it 							*/
  else									/* caller not giving description? 	*/
  {
	d = NULL;
	if (image->memalloc != NULL)		/* image has memalloc? 				*/
	{
	  dtmemalloc_t *memalloc =			/* address memalloc 				*/
        (dtmemalloc_t *)image->memalloc;
	  d = memalloc->mem.name;			/* use memalloc's memory name 		*/
	}
	if (d == NULL)						/* still no name? 					*/
	{
	  DT_Q(dtstr_printf,(ctl,			/* make name from memory address 	*/
        d2, sizeof(d2), 
        "0x%08lx", 
        image->row[9].dvoid));
	  d = d2;
	}
  }
  DT_Q(dtstr_printf,(ctl,
    string, max,
    "%s(%u, %u, %s)",
    d != NULL? d: "image",
    image->xe, image->ye, t));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_dbg_op() - output debug message
| 
| SUMMARY
| Output a consistently formatted debug message.
| Format and output the message only if the :DT_DBG_MASK_IMG: bit
| is set in ~ctl->dbg_mask~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_dbg_op(							/* output debug message				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const dtimage_t *image,				/* image being operated on			*/
  const char *desc,						/* image description 				*/
  const char *f)						/* caller ID 						*/
{
  DT_F("dtimage_dbg_op");
  if (DT_DBG(ctl, DT_DBG_MASK_IMG))		/* this debug mask is on?			*/
  {
	char s[256];
	DT_Q(dtimage_to_string,(ctl,
      image, desc, s, sizeof(s)));
    dt_dbg(ctl, f, DT_DBG_MASK_IMG,
      "%s", s);
  }

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtimage_dbg_values() - output ASCII pixel values as debug
| 
| SUMMARY
| Emit one debug message for each row of an image.
| For each row, only format and print as many pixels as fit
| in 100 characters.
| Emit the string ~f~ as part of the debug if it is not NULL.
| No debug messages are output if no bits coincide between
| ~mask~ and ~ctl->dbg_mask~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtimage_dbg_values(						/* output pixel values as debug 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image whose pixels to print		*/
  const char *f,						/* caller ID 						*/
  const long mask)						/* debug mask 						*/
{
  DT_F("dtimage_dbg_values");
  dtxy_t x, y;
  dtxy_t ye = DT_MIN(image->ye, 8);
  char t[64];
  char s[100];

  if (DT_DBG(ctl, mask))				/* debug will emerge? 				*/
  for (y=0; y<ye; y++)
  {
    s[0] = '\0';						/* initialize s to null string	    */
    for (x=0; x<image->xe; x++)
	{
      DT_Q(dtimage_sprintf_pixel,(ctl,	/* do next pixel				    */
        image, x, y, t, sizeof(t)));
      if (strlen(s)+strlen(t)+1 >		/* pixel would cause overrun?  	    */
          sizeof(s))
        break;							/* do no more on this row		    */
      strcat(s, t);						/* add pixel to string			    */
	}
    dt_dbg(ctl, f, mask, "row %3u: %s",
      y, s);
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
