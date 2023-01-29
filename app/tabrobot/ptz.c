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
#include <dtptz.h>						/* public to the world 				*/

#include <dtack/os.h>						/* utility OS functions 			*/
#include <dtack/str.h>						/* utility string functions 		*/

DT_RCSID("source/ptz $RCSfile: ptz.c,v $ $Revision: 1.7 $");

static const char *choices[] = {
  "pela",
  "ipc",
  NULL
};

extern dtptz_init_f dtptz_pela_init;

static dtptz_init_f *methods[] = {
  dtptz_pela_init,
  dtptz_ipc_init,
  NULL
};

/*..........................................................................
| NAME
| dtptz_init() - initialize ptz object
|
| SUMMARY
| Initialize the ptz object ~ptz~.
| Expect ~spec~ to the of the form #type,arg1,arg1#.
| Have an error if #type# is not a member of a predefined set.
| Branch to the implementation identified by #type# and pass the #args# to it.
| Cause the implementation to make the callbacks ~feed~ and ~frame~
| when it wants more data or has completed a frame, respectively.
| 
| Expect ~flags~ to have one or more of the following bits set:
| :PTZ_FLAG_RESERVED:	reserved for future use
| 
| Release all partially allocated resources before returning an error.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtptz_init(								/* initialize ptz object 			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtptz_t *ptz,							/* ptz object returned				*/
  const char *spec,						/* ptz specification 				*/
  unsigned long flags)					/* process control flags 			*/
{
  DT_F("dtptz_init");
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

  DT_C(dtstr_choice,(ctl, choices,		/* look up ptz type			    	*/
    type, &choice));

  if (choice == -1)						/* unknown type?				    */
  {
    rc = dt_err(ctl, F,
      "unknown ptz spec \"%s\"",
      type);
	goto cleanup;
  }
  else
  if (choice == -2)						/* ambiguous type?				    */
  {
    rc = dt_err(ctl, F,
      "ambiguous ptz spec \"%s\"",
      type);
	goto cleanup;
  }

  memset(ptz, 0, sizeof(*ptz));			/* clear structure				    */

  DT_C((methods[choice]),(ctl,			/* give info to implementation		*/
    ptz, info))

cleanup:
  DT_I(dtos_free2,(ctl, type,
    F, "type"));
  
  if (rc != DT_RC_GOOD)
    memset(ptz, 0, sizeof(*ptz));		/* clear structure				    */

  return rc;
}


/*..........................................................................
| NAME
| dtptz_uninit() - release ptz object resources
|
| SUMMARY
| Uninitialize the ptz object ~ptz~.
| Release all memory and unlock hardware resources acquired
| during the object's lifetime.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtptz_uninit(
  dt_ctl_t *ctl,
  dtptz_t *ptz)
{
  DT_F("dtptz_uninit");
  dt_rc_e rc = DT_RC_GOOD;
  if (ptz->uninit)						/* implementation has method?	 	*/
    DT_Q((ptz->uninit),(ctl, ptz));		/* call virtual method				*/
  memset(ptz, 0, sizeof(*ptz));
  return rc;
}


/*..........................................................................
| NAME
| dtptz_stop() - release ptz object resources
|
| SUMMARY
| Stopialize the ptz object ~ptz~.
| Release all memory and unlock hardware resources acquired
| during the object's lifetime.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtptz_stop(
  dt_ctl_t *ctl,
  dtptz_t *ptz)
{
  DT_F("dtptz_stop");
  dt_rc_e rc = DT_RC_GOOD;
  if (ptz->stop)						/* implementation has method?	 	*/
    DT_Q((ptz->stop),(ctl, ptz));		/* call virtual method				*/
  return rc;
}


/*..........................................................................
| NAME
| ptz_command() - display text in overlay
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
dtptz_command(							/* load text into overlay			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtptz_t *ptz,							/* ptz object structure 			*/
  const char *cmd)						/* cmd to display 					*/
{
  DT_F("dtptz_command");
  if (ptz->command)						/* implementation has method?	 	*/
    DT_Q((ptz->command),(ctl,			/* call virtual method				*/
      ptz, cmd));
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
