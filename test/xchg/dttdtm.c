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

#include <dtack/t.h>

DT_RCSID("test $RCSfile: dttdtm.c,v $ $Revision: 1.2 $");

#ifndef DTDEPEND
#include <dtm.h>						/* NCSA Data Transfer Mechanism	    */
#include <ris.h>						/* NCSA DTM standard RIS type	    */

extern void RISsetDimensions(char *, int, int);
#endif

#define USAGE "[-D nnnn] [-op xxxx]"

#define NCOLS 256
#define NROWS 256

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttdtm_msg(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttdtm_msg");
  char header[DTM_MAX_HEADER];
  int outport;
  int dtmrc;

  MSGsetClass(header);
  MSGsetString(header, "message from dttdtm");

  outport = DTMmakeOutPort(":6060", DTM_DEFAULT);
  if (outport == DTMERROR)
    return dt_err(ctl, F, 
      "DTM error %d in DTMmakeOutPort",
      outport);

  dtmrc = DTMbeginWrite(outport, header, DTMHL(header));
  if (dtmrc == DTMERROR)
    return dt_err(ctl, F, 
      "DTM error %d in DTMbeginWrite",
      dtmrc);

  dtmrc = DTMendWrite(outport);
  if (dtmrc == DTMERROR)
    return dt_err(ctl, F, 
      "DTM error %d in DTMendWrite",
      dtmrc);

  dtmrc = DTMdestroyPort(outport);
  if (dtmrc == DTMERROR)
    return dt_err(ctl, F, 
      "DTM error %d in DTMdestroyPort",
      dtmrc);

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttdtm_send_blocks(
  dt_ctl_t *ctl,
  dtimage_t *image,
  int outport,
  int *assert_fail)
{
  DT_F("dttdtm_send_blocks");
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
dttdtm_send_image(
  dt_ctl_t *ctl,
  dtimage_t *image,
  int *assert_fail)
{
  DT_F("dttdtm_send_image");
  char header[RISsize];
  int outport;
  int dtmrc;
  dt_rc_e rc = DT_RC_GOOD;

  RISsetClass(header);					/* build header from macros		    */
  RISsetDimensions(header, 
    image->xe, image->ye);
  RISsetTitle(header, "dttdtm");
  RISsetType(header, RIS8BIT);

  outport = DTMmakeOutPort(":6060",		/* make port to dtmge			    */
    DTM_DEFAULT);
  if (outport == DTMERROR)
    return dt_err(ctl, F, 
      "DTM error %d in DTMmakeOutPort",
      outport);

  dtmrc = DTMbeginWrite(outport,		/* write header to dtmge		    */
    header, RISHL(header));
  if (dtmrc == DTMERROR)
    rc = dt_err(ctl, F, 
      "DTM error %d in DTMbeginWrite",
      dtmrc);

  DT_GI(dttdtm_send_blocks,(ctl,		/* send all blocks of image		    */
    image, outport, assert_fail));

  if (dtmrc != DTMERROR)
  {
    dtmrc = DTMendWrite(outport);		/* finish write operation			*/
    if (dtmrc == DTMERROR)
      rc = dt_err(ctl, F, 
        "DTM error %d in DTMendWrite",
        dtmrc);
  }

  dtmrc = DTMdestroyPort(outport);		/* get rid of port to dtmge	    */
  if (dtmrc == DTMERROR)
    rc = dt_err(ctl, F, 
      "DTM error %d in DTMdestroyPort",
      dtmrc);

  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttdtm_image(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttdtm_image");
  dtimage_t img1;
  dt_rc_e rc;

  DT_Q(dtimage_create,(ctl, &img1,		/* make new image 				    */
    NCOLS, NROWS, DT_NTYPE_B08));

  DT_G(dtimage_gen_test1,(ctl, &img1));	/* generate test pattern			*/

  DT_GI(dttdtm_send_image,(ctl,		/* send image to dtmge		    */
    &img1, assert_fail));

  DT_Q(dtimage_free,(ctl, &img1));		/* free image					    */

  return rc;
}

/*..........................................................................
 *..........................................................................*/

DTT_TEST(dttdtm)
{
  DT_F("dttdtm");
  dt_ctl_t *ctl = &param->ctl;
  int n;
  int assert_fail = 0;

  DT_Q(dtparam_init,(param));			/* init param structure             */

										/* set default for params we want	*/
  DTPARAM_DEFAULT(dbg, NULL);
  DTPARAM_DEFAULT(op, "fast");

  DT_Q(dt_interactive_parse,(			/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;

  if (argc != 0)
    return dt_interactive_usage(
      param, USAGE);

  if (!strcmp(param->op, "all") ||
      !strcmp(param->op, "fast") ||
      !strcmp(param->op, "msg"))
  {
    DT_Q(dttdtm_msg,(ctl,				/* run message tests			    */
      &assert_fail));
  }

  if (!strcmp(param->op, "all") ||
      !strcmp(param->op, "fast") ||
      !strcmp(param->op, "image"))
  {
    DT_Q(dttdtm_image,(ctl,				/* run image tests				    */
      &assert_fail));
  }

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
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
