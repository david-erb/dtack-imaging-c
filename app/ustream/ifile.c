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
| dtistream_file() - input stream on file
|
| SUMMARY
| Not documented yet.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("app/ustream $RCSfile: ifile.c,v $ $Revision: 1.8 $");

#include <dtstream.h>
#include <dtistream.h>
#include <dtack/os.h>

typedef struct {
  dtfd_t fd;
  long file_size; 
  long bytes_left;
  long packet_size;
} priv_t;

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtistream_file_is_available(			/* check for client data pending 	*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  int *is_available)
{
  *is_available = 1;
  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtistream_file_recv(					/* get incoming data packet			*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  void **packet,
  long *packet_length)
{
  DT_F("dtistream_file_recv");
  priv_t *priv = (priv_t *)istream->priv;

  if (priv->bytes_left == 0)			/* file is depleted? 				*/
  {
	*packet_length = 0;
	return DT_RC_GOOD;
  }

  *packet_length =						/* how much data to read 			*/
    DT_MIN(priv->packet_size,
      priv->bytes_left);
  DT_Q(dtos_malloc2,(ctl,				/* get space for the packet 		*/
    packet, *packet_length,
    F, "file packet"));

  DT_Q(dtfd_read_all,(ctl, &priv->fd,	/* actually read the data 			*/
    *packet, *packet_length));

  priv->bytes_left -= *packet_length;
  return DT_RC_GOOD;
}


/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtistream_file_uninit(					/* release resources 				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream)					/* this dtistream object 			*/
{
  DT_F("dtistream_file_uninit");
  priv_t *priv = (priv_t *)istream->priv;
  dt_rc_e rc = DT_RC_GOOD;
  if (priv != NULL)
  {
	DT_I(dtfd_close,(ctl, &priv->fd));	/* close input file 				*/
	if (priv != NULL)
      DT_I(dtos_free2,(ctl, priv,		/* free private data structure 		*/
        F, "priv"));
  }
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtistream_file_init(					/* make istream on file				*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtistream_t *istream,					/* this dtistream object 			*/
  const char *spec,
  dtstream_id_t *id,					/* our ID 							*/
  dtstream_id_t *permit,				/* permitted server ID				*/
  long timeout)
{
  DT_F("dtistream_file_init");
  priv_t *priv = NULL;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,				/* space for priv structure 		*/
    &istream->priv, sizeof(*priv),
    F, "priv"));
  priv = (priv_t *)istream->priv;
  memset(priv, 0, sizeof(*priv));

  priv->fd.fd = -1;

  DT_C(dtfd_open,(ctl, &priv->fd,		/* open the file 					*/
    spec, "r"));
  DT_C(dtfd_seek,(ctl, &priv->fd, 0,	/* get file size 					*/
    DTFD_WHENCE_END, 
    &priv->file_size));
  DT_C(dtfd_seek,(ctl, &priv->fd, 0,	/* return to start of file 			*/
    DTFD_WHENCE_SET, NULL));

  priv->bytes_left = priv->file_size;
  priv->packet_size = 1024;

  istream->is_available =				/* assign methods 					*/
    dtistream_file_is_available;
  istream->recv = 
    dtistream_file_recv;
  istream->uninit = 
    dtistream_file_uninit;

cleanup:
  if (rc != DT_RC_GOOD)
  {
	if (priv != NULL)
	{
	  if (priv->fd.fd != -1)
	    DT_I(dtfd_close,(ctl,
          &priv->fd));
	  DT_I(dtos_free2,(ctl, priv,
        F, "priv"));
	}
  }
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
