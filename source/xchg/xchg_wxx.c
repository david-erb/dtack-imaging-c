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
| This class of export function packs the arguments together
| into an RPC-like stream and uses dtwinc functions to send them.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: xchg_wxx.c,v $ $Revision: 1.1 $");

#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/mem.h>
#include <dtack/str.h>
#include <dtack/dib.h>
#include <dtack/os.h>
#include <dtack/msw.h>

#define DESC "Microsoft Windows"

/*..........................................................................
 * write a temporary file with a wmf image in it
 * tell front end to read this file
 * also give the image size to the front end to save it endless trouble
 *..........................................................................*/

static
dt_rc_e
export_file(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *id,
  dtimage_t *image,
  dtxy_t xpos,
  dtxy_t ypos)
{
  DT_F("dtxchg_wxx_export_file");
  char *type;
  char s[80];
  int pid;

  if (!strncmp(tmpdir, "wmf,", 4))
  {
    type = "wmf";
    tmpdir += 4;
  }
  else
  if (!strncmp(tmpdir, "bmp,", 4))
  {
    type = "bmp";
    tmpdir += 4;
  }
  else
    return dt_err(ctl, F,
      "Sorry, \"%s\" does not indicate"
      " with a known type",
      tmpdir);
  
  dtstr_printf(ctl, s, sizeof(s),		/* 1. the feedback command 			*/
	"export");
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 2. name of image window			*/
	"%s", id);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 3. x position					*/
	"%u", xpos);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 4. y position					*/
	"%u", ypos);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 5. image width					*/
	"%u", image->xe);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 6. image height					*/
	"%u", image->ye);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 7. type of file we want			*/
	"%s", type);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  DT_Q(dtos_getpid,(ctl, &pid));
  dtstr_printf(ctl, s, sizeof(s),		/* 8. name of tmp file				*/
	"%s/%08x.%s", tmpdir, pid, type);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  if (!strcmp(type, "wmf"))
    DT_Q(dtimage_write_wmf_file,(ctl,	/* write the temporary wmf file		*/
      image, s, 0, 0, 0))
  else
  if (!strcmp(type, "bmp"))
    DT_Q(dtimage_write_bmp_file,(ctl,	/* write the temporary bmp file		*/
      image, s, 0, 0, 0))

  DT_Q(dtmsw_kick,(ctl,					/* kick front end 					*/
    DTMSW_KICK_FEEDBACK));

  DT_Q(dtmsw_pause,(ctl));				/* wait for confirmation 			*/

  DT_Q(dtos_unlink,(ctl, s));			/* remove the file 					*/

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_wxx_export_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  int xpos,
  int ypos)
{
  DT_F("dtxchg_wxx_export_image");

  if (!strncmp(server, "file,", 5))
    DT_Q(export_file,(ctl,
      server+5, id, image,
      xpos, ypos))
  else
    return dt_err(ctl, F,
      "Sorry, \"%s\" is not a known"
      " method",
      server);

  return DT_RC_GOOD;
}


/*..........................................................................
 * the file has the Adobe placeable header in it
 * we deduce the image size from the extents specified in this header
 * id is an image window name possibly including spaces
 *..........................................................................*/

static
dt_rc_e
import_file(
  dt_ctl_t *ctl,
  const char *tmpdir,
  const char *id,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtxchg_wxx_import_file");
  char *type;
  char s[80];
  int pid;

  if (!strncmp(tmpdir, "wmf,", 4))
  {
    type = "wmf";
    tmpdir += 4;
  }
  else
  if (!strncmp(tmpdir, "bmp,", 4))
  {
    type = "bmp";
    tmpdir += 4;
  }
  else
    return dt_err(ctl, F,
      "Sorry, \"%s\" does not indicate"
      " with a known type",
      tmpdir);
  
  dtstr_printf(ctl, s, sizeof(s),		/* 1. the feedback command 			*/
	"import");
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 2. name of image window we want 	*/
	"%s", id);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  dtstr_printf(ctl, s, sizeof(s),		/* 3. type of file we want			*/
	"%s", type);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  DT_Q(dtos_getpid,(ctl, &pid));
  dtstr_printf(ctl, s, sizeof(s),		/* 4. name of tmp file for image	*/
	"%s/%08x.%s", tmpdir, pid, type);
  DT_Q(dtmsw_feedback,(ctl, 1, s));

  DT_Q(dtmsw_kick,(ctl,					/* kick front end 					*/
    DTMSW_KICK_FEEDBACK));

  DT_Q(dtmsw_pause,(ctl));				/* wait for confirmation 			*/

  if (!strcmp(type, "wmf"))
    DT_Q(dtimage_read_wmf_file,(ctl,	/* read the file as wmf				*/
      image, s))
  else
  if (!strcmp(type, "bmp"))
    DT_Q(dtimage_read_bmp_file,(ctl,	/* read the file as bmp				*/
      image, s))

  DT_Q(dtos_unlink,(ctl, s));			/* remove the file 					*/

  return DT_RC_GOOD;
}

/*..........................................................................
 * server is either "wmf,handle" or "dib,handle"
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_wxx_import_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtxchg_wxx_import_image");

  if (!strncmp(server, "file,", 5))
    DT_Q(import_file,(ctl,
      server+5, id, image,
      pagesize, npages))
  else
    return dt_err(ctl, F,
      "Sorry, \"%s\" is not a known"
      " method",
      server);

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
