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
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
#include <dtack/lut8.h>
#include <dtack/xchg.h>

DT_RCSID("source/high $RCSfile: xchg_dtm.c,v $ $Revision: 1.1 $");

/*..........................................................................
 *..........................................................................*/

#if DTPRJ_NCSA_DTM != DT_1
dt_rc_e
DTCONFIG_API1
dtxchg_dtm_export_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  dtxy_t xpos,
  dtxy_t ypos)
{
  DT_F("dtxchg_dtm_export_image");

  return dt_err(ctl, F,
    "NCSA Data Transfer Mechanism has not been linked with this executable");
}
#endif

/*..........................................................................
 *..........................................................................*/

#if DTPRJ_NCSA_DTM == DT_1				/* the rest of the file			    */

#ifndef DTDEPEND
#	include <dtm.h>						/* NCSA Data Transfer Mechanism	    */
#	include <ris.h>						/* NCSA DTM standard RIS type	    */
#endif

extern void RISsetDimensions(			/* ris.h missed this one		    */
  char *, int, int);

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
dtxchg_dtm_export_blocks(
  dt_ctl_t *ctl,
  dtimage_t *image,
  int outport)
{
  DT_F("dtxchg_dtm_export_blocks");
  unsigned int y;
  int dtmrc;

  y = 0;	
  do {
    void *p;
    unsigned int datasize;
    unsigned int nrows;

    DTIMAGE_GETROWS(ctl, image, y,		/* map the next block               */
      &p, &datasize);
    nrows = datasize / image->rowsize;	/* how many rows in block           */
   
    dtmrc = DTMwriteDataset(outport,	/* send the block				    */
      p, datasize, DTM_CHAR);
    if (dtmrc == DTMERROR)
      return dt_err(ctl, F, 
        "DTM error %d in DTMwriteDataset",
        dtmrc);

    DTIMAGE_UNMAPROW(ctl, image, y);	/* don't need this block any more   */
    y += nrows;
  } while(y < image->ye);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_dtm_export_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  dtxy_t xpos,
  dtxy_t ypos)
{
  DT_F("dtxchg_dtm_export_image");
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B08, 0};
  char header[RISsize];
  char tmp[128];
  int outport;
  int dtmrc;
  dt_rc_e rc = DT_RC_GOOD;

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, id, 
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;


  if (strlen(server) > sizeof(tmp)-1)
    return dt_err(ctl, F, 
      "DTM server name \"%s\" too long",
      server);

  if (strlen(id) > sizeof(tmp)-1)
    return dt_err(ctl, F, 
      "window title \"%s\" too long",
      id);

  RISsetClass(header);					/* build header from macros		    */
  RISsetDimensions(header, 
    image->xe, image->ye);
  strcpy(tmp, id);						/* keep id const				    */
  RISsetTitle(header, tmp);
  RISsetType(header, RIS8BIT);

  strcpy(tmp, server);					/* keep server const			    */
  outport = DTMmakeOutPort(tmp,			/* make port to ximage/collage	    */
    DTM_DEFAULT);
  if (outport == DTMERROR)
    return dt_err(ctl, F, 
      "DTM error %d in DTMmakeOutPort",
      outport);

  dtmrc = DTMbeginWrite(outport,		/* write header to collage		    */
    header, RISHL(header));
  if (dtmrc == DTMERROR)
    rc = dt_err(ctl, F, 
      "DTM error %d in DTMbeginWrite",
      dtmrc);

  DT_GI(dtxchg_dtm_export_blocks,(ctl,	/* send all blocks of image		    */
    image, outport));

  if (dtmrc != DTMERROR)
  {
    dtmrc = DTMendWrite(outport);		/* finish write operation			*/
    if (dtmrc == DTMERROR)
      rc = dt_err(ctl, F, 
        "DTM error %d in DTMendWrite",
        dtmrc);
  }

  dtmrc = DTMdestroyPort(outport);		/* get rid of port to collage	    */
  if (dtmrc == DTMERROR)
    rc = dt_err(ctl, F, 
      "DTM error %d in DTMdestroyPort",
      dtmrc);

  return rc;
}
#endif





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
