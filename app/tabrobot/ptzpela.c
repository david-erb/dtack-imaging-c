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
  20000125 DE added iris
 *..........................................................................*/


/*..........................................................................
 * rev history
 * 19970508 1.0 first
 *..........................................................................*/

#include <dtack/base.h>							/* NCI fundamental declarations 	*/
#include <dtptz.h>						/* public to the world 				*/
#include <dtserial.h>

#include <dtack/os.h>						/* utility OS functions 			*/
#include <dtack/str.h>						/* utility string functions 		*/

#include <dtack/errno.h>

DT_RCSID("source/ptz $RCSfile: ptzpela,v $ $Revision: 1.7 $");

#define DESC "Pelco-Intercept"

#if DTPRJ_PTZ_PELA == DT_1

typedef struct {
  char *spec;
  char *serial_spec;
  char *setup;
  unsigned char address;
  dtserial_t serial;
} priv_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtptz_pela_release(						/* release resources 				*/
  dt_ctl_t *ctl,
  priv_t *priv)
{
  DT_F("dtptz_pela_release");
  dt_rc_e rc = DT_RC_GOOD;

  if (priv != NULL)
  {
	if (priv->serial.initialized)
	  DT_I(dtserial_close,(ctl, 
        &priv->serial));
	if (priv->spec != NULL)
      DT_I(dtos_free2,(ctl, priv->spec,
        F, "priv->spec"));
    DT_I(dtos_free2,(ctl, priv,
      F, "priv"));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtptz_pela_uninit(						/* close object						*/
  dt_ctl_t *ctl,
  dtptz_t *ptz)
{
  DT_F("dtptz_pela_uninit");
  priv_t *priv = ptz->priv;
  DT_Q(dtptz_pela_release,(ctl, priv));
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtptz_pela_command(						/* execute ptz command				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtptz_t *ptz,							/* decoder object structure 		*/
  const char *cmd)
{
  DT_F("dtptz_pela_command");
  priv_t *priv =
    ptz->priv;
  unsigned char string[11];
  int i;
  dt_rc_e rc;
  
  if (!strcmp(cmd, "nop"))
	return DT_RC_GOOD;

#define TRANSLATE(CMD, C1, C2, C3, C4) 	\
  if (!strcmp(cmd, CMD)) 				\
  {										\
	string[5] = C1;				\
	string[6] = C2;				\
	string[7] = C3;				\
	string[8] = C4;				\
  }										\
  else

  TRANSLATE(DTPTZ_COMMAND_RESET, 		/* reset 							*/
	0x00, 0x0f, 0x00, 0x00)
  TRANSLATE(DTPTZ_COMMAND_STOP,			/* stop 							*/
	0x00, 0x00, 0x00, 0x00)		
	
#define S1 0x28							/* slow speed					    */
#define S2 0x3F

  TRANSLATE("<x", 0x00, 0x04, S1, S2)		/* continuous pan left 				*/
  TRANSLATE(">x", 0x00, 0x02, S1, S2)		/* continuous pan right 			*/
  TRANSLATE("<y", 0x00, 0x10, S1, S2)		/* continuous tilt down 			*/
  TRANSLATE(">y", 0x00, 0x08, S1, S2)		/* continuous tilt up 				*/
	
  TRANSLATE("<<x", 0x00, 0x04, 0x3F, 0x3F)	/* continuous pan left 				*/
  TRANSLATE(">>x", 0x00, 0x02, 0x3F, 0x3F)	/* continuous pan right 			*/
  TRANSLATE("<<y", 0x00, 0x10, 0x3F, 0x3F)	/* continuous tilt down 			*/
  TRANSLATE(">>y", 0x00, 0x08, 0x3F, 0x3F)	/* continuous tilt up 				*/
	
  TRANSLATE("-x", 0x00, 0x04, S1, S2)		/* momentary pan left 				*/
  TRANSLATE("+x", 0x00, 0x02, S1, S2)		/* momentary pan right 				*/
  TRANSLATE("-y", 0x00, 0x10, S1, S2)		/* momentary tilt down 				*/
  TRANSLATE("+y", 0x00, 0x08, S1, S2)		/* momentary tilt up 				*/

  TRANSLATE("<U", 0x00, 0x0c, S1, S2)		/* continuous up left 				*/
  TRANSLATE(">U", 0x00, 0x0a, S1, S2)		/* continuous up right 				*/
  TRANSLATE("<D", 0x00, 0x14, S1, S2)		/* continuous down left 			*/
  TRANSLATE(">D", 0x00, 0x12, S1, S2)		/* continuous down right 			*/
	
  TRANSLATE("-U", 0x00, 0x0c, S1, S2)		/* momentary up left 				*/
  TRANSLATE("+U", 0x00, 0x0a, S1, S2)		/* momentary up right 				*/
  TRANSLATE("-D", 0x00, 0x14, S1, S2)		/* momentary down left 				*/
  TRANSLATE("+D", 0x00, 0x12, S1, S2)		/* momentary down right 			*/
	
  TRANSLATE("<z", 0x00, 0x40, S1, S2)		/* continuous zoom wide 			*/
  TRANSLATE(">z", 0x00, 0x20, S1, S2)		/* continuous zoom tele 			*/
  TRANSLATE("-z", 0x00, 0x40, S1, S2)		/* momentary zoom wide 				*/
  TRANSLATE("+z", 0x00, 0x20, S1, S2)		/* momentary zoom tele 				*/

  TRANSLATE("<f", 0x02, 0x00, 0x00, 0x00)	/* continuous focus near 			*/
  TRANSLATE(">f", 0x01, 0x00, 0x00, 0x00)	/* continuous focus far 			*/
  TRANSLATE("-f", 0x02, 0x00, 0x00, 0x00)	/* momentary focus near 			*/
  TRANSLATE("+f", 0x01, 0x00, 0x00, 0x00)	/* momentary focus far 				*/

  TRANSLATE("<o", 0x08, 0x00, 0x00, 0x00)	/* continuous iris close 			*/
  TRANSLATE(">o", 0x04, 0x00, 0x00, 0x00)	/* continuous iris open				*/
  TRANSLATE("-o", 0x08, 0x00, 0x00, 0x00)	/* momentary iris close 			*/
  TRANSLATE("+o", 0x04, 0x00, 0x00, 0x00)	/* momentary iris open 				*/

  if (cmd[0] == 'S' ||
	  cmd[0] == 'C' ||
	  cmd[0] == 'G')
  {
	long preset;
	string[5] = 0x00;
	if (cmd[0] == 'S')
	  string[6] = 0x03;
	if (cmd[0] == 'C')
	  string[6] = 0x05;
	if (cmd[0] == 'G')
	  string[6] = 0x07;
	string[7] = 0x00;
	DT_C(dtstr_to_long,(ctl, cmd+1,
      &preset));
	string[8] = preset;
	
  }
  else

  {
	rc = dt_err(ctl, F,
      "unrecognized command \"%s\"",
      cmd);
	goto cleanup;
  }

  if (DT_DBG(ctl, DT_DBG_MASK_COMMAND) &&
	  DT_DBG(ctl, DT_DBG_MASK_PIXEL))
	dt_dbg(ctl, F, DT_DBG_MASK_ANY,
       "%s", cmd);

  string[0] = 0x41;
  string[1] = 0x54;
  string[2] = 0x0d;
  string[3] = 0xa0;						/* STX							    */
  string[4] = priv->address;			/* receiver address				    */
  string[9] = 0xaf;						/* ETX							    */
  string[10] = 0;						/* crc							    */
  for (i=3; i<10; i++)
	string[10] ^= string[i];

  DT_C(dtserial_write,(ctl,				/* write command to serial port 	*/
    &priv->serial,
    string, sizeof(string)));

  if (cmd[0] == '+' ||					/* one of the momentary commands? 	*/
	  cmd[0] == '-')
	DT_C(dtptz_pela_command,(ctl,		/* issue immediate stop 			*/
      ptz, "<>"));
  
cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtptz_pela_stop(						/* stop all motion					*/
  dt_ctl_t *ctl,
  dtptz_t *ptz)
{
  DT_F("dtptz_pela_stop");
  DT_Q(dtptz_pela_command,(ctl, ptz, "<>"));
  return DT_RC_GOOD;
}

#endif

/*..........................................................................
| NAME
| dtptz_pela_init() - initialize ptz object
|
| SUMMARY
| Initialize the decoder object ~mdec~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtptz_pela_init(
  dt_ctl_t *ctl,
  dtptz_t *ptz,
  const char *spec)
{
  DT_F("dtptz_pela_init");
# if DTPRJ_PTZ_PELA == DT_1
  priv_t *priv = NULL;
  int ntokens;
  char *p;
  dt_rc_e rc;

  if (spec == NULL)
  {
    rc = dt_err(ctl, F,
      "NULL ptz spec");
    goto cleanup;
  }

  if (strlen(spec) == 0)
  {
    rc = dt_err(ctl, F,
      "empty ptz spec");
    goto cleanup;
  }

  DT_C(dtos_malloc2,(ctl,				/* space for private structure	    */
    (void **)&priv,
    sizeof(priv_t),
    F, "priv"));
  memset(priv, 0,						/* clear private structure 			*/
    sizeof(priv_t));

  DT_C(dtos_strdup,(ctl, spec,			/* get space for spec edit 			*/
    &priv->spec));

  DT_C(dtstr_tokenize,(ctl,				/* separate spec into tokens 		*/
    priv->spec,
    ',', &ntokens));

  p = priv->spec;

  priv->serial_spec = p;
  
  if (ntokens > 1)						/* COM settings given after serial_spec? */
  {
	p = p + strlen(p) + 1;
	priv->setup = p;
  }

  DT_C(dtserial_open,(ctl,				/* open serial device 				*/
    &priv->serial,
    priv->serial_spec));

  if (priv->setup != NULL)				/* setup string in spec? 			*/
  {
    DT_C(dtserial_setup,(ctl,			/* setup serial device 				*/
      &priv->serial,
      priv->setup));
  }

  if (ntokens > 2)						/* address given after COM settings? */
  {
	long a;
	p = p + strlen(p) + 1;
	DT_C(dtstr_to_long,(ctl, p, &a));
	priv->address = (unsigned char)
	  ((unsigned long)a & 0xff);
	dt_dbg(ctl, F, DT_DBG_MASK_CONFIG,
	  "serial address 0x%02lx", 
	  (unsigned long)priv->address);
  }
  
  ptz->priv       = priv;
  ptz->uninit     = dtptz_pela_uninit;
  ptz->stop       = dtptz_pela_stop;
  ptz->command    = dtptz_pela_command;

  ptz->initialized = 1;

cleanup:
  if (priv != NULL &&
	  rc != DT_RC_GOOD)
	DT_I(dtptz_pela_release,(ctl, priv));

  if (rc != DT_RC_GOOD)
	memset(ptz, 0, sizeof(*ptz));

  return rc;
# else
  return dt_err_notlinked(ctl, F, DESC);  
# endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtptz_pela_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtptz_pela lib";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 2;
  if (rev_date != NULL)
    *rev_date = 20011006;
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
