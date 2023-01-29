/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>							/* NCI fundamental declarations 	*/
#include <umdec.h>						/* public to the world 				*/

#include <dtack/os.h>						/* utility OS functions 			*/
#include <dtack/str.h>						/* utility string functions 		*/

DT_RCSID("source/mpeg $RCSfile: umdec.c,v $ $Revision: 1.7 $");

static const char *choices[] = {
  "ssg",
  "vp",
  NULL
};

extern umdec_init_f umdec_cortex_init;

static umdec_init_f *methods[] = {
  umdec_ssg_init,
  umdec_vp_init,
  NULL
};

/*..........................................................................
| NAME
| umdec_init() - initialize decoder object
|
| SUMMARY
| Initialize the decoder object ~mdec~.
| Expect ~spec~ to the of the form #type,arg1,arg1#.
| Have an error if #type# is not a member of a predefined set.
| Branch to the implementation identified by #type# and pass the #args# to it.
| Cause the implementation to make the callbacks ~feed~ and ~frame~
| when it wants more data or has completed a frame, respectively.
| 
| Expect ~flags~ to have one or more of the following bits set:
| :UMDEC_FLAG_RESERVED:	reserved for future use
| 
| Release all partially allocated resources before returning an error.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_init(								/* initialize decoder object 		*/
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *umdec,						/* decoder object returned			*/
  const char *spec,						/* decoder specification 			*/
  unsigned long flags)					/* process control flags 			*/
{
  DT_F("umdec_init");
  char *type;
  char *info;
  int choice;
  dt_rc_e rc;

  if (spec == NULL)
    spec = "";

  DT_Q(dtos_strdup,(ctl, spec,
    &type));

  info = strchr(type, ',');				/* info part after type		    	*/
  if (info)
    *info++ = '\0';
  else
    info = "";

  DT_C(dtstr_choice,(ctl, choices,		/* look up umdec type			    	*/
    type, &choice));

  if (choice == -1)						/* unknown type?				    */
  {
    rc = dt_err(ctl, F,
      "unknown umdec spec \"%s\"",
      type);
	goto cleanup;
  }
  else
  if (choice == -2)						/* ambiguous type?				    */
  {
    rc = dt_err(ctl, F,
      "ambiguous umdec spec \"%s\"",
      type);
	goto cleanup;
  }

  memset(umdec, 0, sizeof(*umdec));		/* clear structure				    */

  DT_C((methods[choice]),(ctl,			/* give info to implementation		*/
    umdec, info))

cleanup:
  DT_I(dtos_free2,(ctl, type,
    F, "type"));
  
  if (rc != DT_RC_GOOD)
    memset(umdec, 0, sizeof(*umdec));	/* clear structure				    */

  return rc;
}


/*..........................................................................
| NAME
| umdec_uninit() - release decoder object resources
|
| SUMMARY
| Uninitialize the decoder object ~mdec~.
| Release all memory and unlock hardware resources acquired
| during the object's lifetime.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_uninit(
  dt_ctl_t *ctl,
  umdec_t *umdec)
{
  DT_F("umdec_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (umdec->uninit)
    DT_Q((umdec->uninit),(ctl, 
      umdec));
  memset(umdec, 0, sizeof(*umdec));
  return rc;
}


/*..........................................................................
| NAME
| umdec_go() - start playing
|
| SUMMARY
| Start a loop in which data is fetched from the feed callback
| and decoded and displayed.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_go(
  dt_ctl_t *ctl,
  umdec_t *umdec,
  umdec_feed_callback_f *feed,			/* callback for food 				*/
  void *feed_arg,						/* feed callback argument 			*/
  umdec_frame_callback_f *frame,		/* callback for frame 				*/
  void *frame_arg,						/* frame callback argument 			*/
  umdec_control_t *control)				/* initial mode 					*/
{
  DT_F("umdec_go");
  dt_rc_e rc = DT_RC_GOOD;

  umdec->feed_callback = feed;
  umdec->feed_arg = feed_arg;
  umdec->frame_callback = frame;
  umdec->frame_arg = frame_arg;
  
  if (umdec->go)
    DT_Q((umdec->go),(ctl, 
      umdec, control));
  return rc;
}

/*..........................................................................
| NAME
| umdec_set_control() - set decoder operation parameters
|
| SUMMARY
| Change the operation of the decoder according to the contents of ~control~.
| Use the following numbers of this structure:
| :int control->speed:		playback speed and direction
| :int control->volume:		audio volume
| :int control->contrast:	display contrast
| May be called from a callback function.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_set_control(						/* set operation parameters 		*/
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *mdec,						/* decoder object structure 		*/
  umdec_control_t *control)				/* control parameters 				*/
{
  DT_F("umdec_set_control");
  if (mdec->set_control)				/* implementation has method?	 	*/
    DT_Q((mdec->set_control),(ctl,		/* pass control down to it 			*/
      mdec, control));
  mdec->control = *control;				/* copy the structure			    */
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| umdec_get_control() - get decoder operation parameters
|
| SUMMARY
| Return the current decoder operating parameters in ~control~.
| Set all the members of ~control~ to their current values.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_get_control(						/* get operation parameters 		*/
  dt_ctl_t *ctl,							/* environment control 				*/
  umdec_t *mdec,						/* decoder object structure 		*/
  umdec_control_t *control)				/* returned control parameters		*/
{
  DT_F("umdec_get_control");
  if (mdec->get_control)				/* implementation has method?	 	*/
    DT_Q((mdec->get_control),(ctl,		/* let implementation give control	*/
      mdec, control));
  *control = mdec->control;				/* copy the structure			    */
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| umdec_get_status() - get decoder status
|
| SUMMARY
| Return the current decoder status in ~status~.
| Return at least the following members:
| const char *status->id		pretty identifier string for log messages
| const char *status->stream	pretty string describing mpeg stream
| long status->frame			most recently completed frame number
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_get_status(						/* get operation parameters 		*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *mdec,						/* decoder object structure 		*/
  umdec_status_t *status)				/* returned status					*/
{
  DT_F("umdec_get_status");
  if (mdec->get_status)					/* implementation has method?	 	*/
    DT_Q((mdec->get_status),(ctl,		/* let implementation give status	*/
      mdec, status));
  *status = mdec->status;				/* copy the structure			    */
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| umdec_overlay_image() - load image into overlay
|
| SUMMARY
| Load the image from ~filename~ onto the overlay screen.
| Place the upper left corner of the image at column ~x~ and row ~y~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_overlay_image(					/* load image into overlay			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *mdec,						/* decoder object structure 		*/
  const char *filename,					/* filename containing image 		*/
  int x,								/* column 							*/
  int y)								/* row 								*/
{
  DT_F("umdec_overlay_image");
  if (mdec->overlay_image)				/* implementation has method?	 	*/
    DT_Q((mdec->overlay_image),(ctl,	/* let implementation do the work	*/
      mdec, filename, x, y));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| umdec_overlay_text() - display text in overlay
|
| SUMMARY
| Display the text from ~string~ on the overlay.
| Place the upper left corner of the string at column ~x~ and row ~y~.
| Character height is ~height~ pixels.
| Characters are displayed in color ~foreground~ on color ~background~.
| Interpret the LSB of each color as blue, the next as green, then red.
| Ignore the MSB of each color value.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_overlay_text(						/* load text into overlay			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *mdec,						/* decoder object structure 		*/
  const char *string,					/* text to display 					*/
  int x,								/* column 							*/
  int y,								/* row 								*/
  int height,							/* text height in pixels 			*/
  long foreground,						/* foreground color RGB 			*/
  long background)						/* background color RGB 			*/
{
  DT_F("umdec_overlay_text");
  if (mdec->overlay_text)				/* implementation has method?	 	*/
    DT_Q((mdec->overlay_text),(ctl,		/* let implementation do the work	*/
      mdec, string, x, y, height,
      foreground, background));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| umdec_overlay_clear() - clear overlay
|
| SUMMARY
| Clear all images and text from overlay permanently.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
umdec_overlay_clear(					/* clear overlay					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  umdec_t *mdec)						/* decoder object structure 		*/
{
  DT_F("umdec_overlay_clear");
  if (mdec->overlay_clear)				/* implementation has method?	 	*/
    DT_Q((mdec->overlay_clear),(ctl,	/* let implementation do the work	*/
      mdec));
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
