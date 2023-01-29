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
  19981230 DE added connect timeout
 *..........................................................................*/


#include <dtack/base.h>							/* NCI fundamental declarations 	*/
#include <dtptz.h>						/* public to the world 				*/
#include <dtack/ipc.h>

#include <dtack/os.h>						/* utility OS functions 			*/
#include <dtack/str.h>						/* utility string functions 		*/

#include <dtack/errno.h>

DT_RCSID("source/ptz $RCSfile: ptzipc,v $ $Revision: 1.7 $");

#define DESC "ptz-ipc relay"

typedef struct {
  char *spec;
  long connect_timeout;
  dtipc_t ipc;
  int connected;
} priv_t;

#define CRLF "\r\n"
#define NOP "nop" CRLF

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtptz_ipc_connect(						/* connect to server				*/
  dt_ctl_t *ctl,
  priv_t *priv)
{
  DT_F("dtptz_ipc_connect");
  dt_rc_e rc = DT_RC_GOOD;

  if (!priv->connected)
  {
	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "connecting %s", priv->spec);

    DT_C(dtipc_find,(ctl, &priv->ipc,	/* connect to remote server 		*/
      priv->spec, 0, 0, 
      priv->connect_timeout));

	priv->connected = 1;
	
	DT_C(dtipc_write_all,(ctl,			/* write warmup nop					*/
      &priv->ipc, "nop\r\n", 5));
  }

cleanup:
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtptz_ipc_disconnect(					/* disconnect from server			*/
  dt_ctl_t *ctl,
  priv_t *priv)
{
  DT_F("dtptz_ipc_disconnect");
  dt_rc_e rc = DT_RC_GOOD;

  if (priv->connected)
  {
    if (priv->ipc.type != DTIPC_TYPE_NONE)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "disconnecting %s", priv->spec);
	  DT_G(dtipc_unfind,(ctl, 
        &priv->ipc));
	}

	priv->connected = 0;
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtptz_ipc_release(						/* release resources 				*/
  dt_ctl_t *ctl,
  priv_t *priv)
{
  DT_F("dtptz_ipc_release");
  dt_rc_e rc = DT_RC_GOOD;

  if (priv != NULL)
  {
	DT_I(dtptz_ipc_disconnect,(ctl,		/* disconnect from server 			*/
      priv));
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
dtptz_ipc_uninit(						/* close object						*/
  dt_ctl_t *ctl,
  dtptz_t *ptz)
{
  DT_F("dtptz_ipc_uninit");
  priv_t *priv = ptz->priv;
  DT_Q(dtptz_ipc_release,(ctl, priv));	/* release resources 				*/
  priv = NULL;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtptz_ipc_command(						/* execute ptz command				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtptz_t *ptz,							/* decoder object structure 		*/
  const char *cmd)
{
  DT_F("dtptz_ipc_command");
  priv_t *priv = ptz->priv;
  dt_rc_e rc = DT_RC_GOOD;

  DT_C(dtptz_ipc_connect,(ctl, priv));	/* connect to server 				*/
  
  if (!priv->connected)					/* could not connect?				*/
	goto cleanup;
	
  DT_GI(dtipc_write_all,(ctl,			/* write command to connection		*/
    &priv->ipc, cmd, strlen(cmd)));
  
  DT_GI(dtipc_write_all,(ctl,			/* write end-of-line to connection	*/
    &priv->ipc, CRLF, strlen(CRLF)));

  DT_GI(dtipc_write_all,(ctl,			/* write nop to flush command		*/
    &priv->ipc, NOP, strlen(NOP)));

  if (rc != DT_RC_GOOD)					/* write failed? 					*/
  {
	dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "attempting to recycle connection");
	
	DT_C(dtptz_ipc_disconnect,(ctl,		/* close old ipc connection 		*/
      priv));

    DT_C(dtptz_ipc_connect,(ctl,		/* reconnect to server 				*/
      priv));
    
    if (!priv->connected)				/* could not connect?				*/
	  goto cleanup;

	dt_dbg(ctl, F, DT_DBG_MASK_ANY,
      "connection recycled, re-sending "
      " \"%s\"",
      cmd);

	DT_C(dtipc_write_all,(ctl,			/* write command to new connection	*/
      &priv->ipc, cmd, strlen(cmd)));
  
    DT_C(dtipc_write_all,(ctl,			/* write end-of-line to connection	*/
      &priv->ipc, CRLF, strlen(CRLF)));

    DT_C(dtipc_write_all,(ctl,			/* write nop to flush command		*/
      &priv->ipc, NOP, strlen(NOP)));
  }


cleanup:
  if (rc != DT_RC_GOOD)
  {
	DT_C(dtptz_ipc_disconnect,(ctl,		/* close old ipc connection 		*/
      priv));
  }

  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtptz_ipc_stop(						/* stop all motion					*/
  dt_ctl_t *ctl,
  dtptz_t *ptz)
{
  DT_F("dtptz_ipc_stop");
  DT_Q(dtptz_ipc_command,(ctl, ptz, "<>"));
  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| dtptz_ipc_init() - initialize ptz object
|
| SUMMARY
| Initialize the decoder object ~mdec~.
| 
| END
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtptz_ipc_init(
  dt_ctl_t *ctl,
  dtptz_t *ptz,
  const char *spec)
{
  DT_F("dtptz_ipc_init");
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

  p = strchr(priv->spec, '+');			/* look for '+' in spec			    */
  if (p)								/* spec has a '+' in it?		    */
  {	
	*p++ = '\0';
	DT_C(dtstr_to_long,(ctl, p,
      &priv->connect_timeout));
  }
  else
	priv->connect_timeout = 10000;		/* max wait connecting to tabptz 	*/

  ptz->priv       = priv;
  ptz->uninit     = dtptz_ipc_uninit;
  ptz->stop       = dtptz_ipc_stop;
  ptz->command    = dtptz_ipc_command;

  ptz->initialized = 1;

cleanup:
  if (priv != NULL &&
	  rc != DT_RC_GOOD)
	DT_I(dtptz_ipc_release,(ctl, priv));

  if (rc != DT_RC_GOOD)
	memset(ptz, 0, sizeof(*ptz));

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtptz_ipc_rev(							/* deliver revision 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  const char **rev_string,				/* returned rev string 				*/
  int *rev_major,						/* returned rev major 				*/
  int *rev_minor,						/* returned rev minor 				*/
  long *rev_date)						/* returned rev date 				*/
{
  if (rev_string != NULL)
    *rev_string = "dtptz_ipc lib";
  if (rev_major != NULL)
    *rev_major = 1;
  if (rev_minor != NULL)
    *rev_minor = 2;
  if (rev_date != NULL)
    *rev_date = 19981230;
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
