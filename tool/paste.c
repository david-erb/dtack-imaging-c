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
| dttoolpaste utility - paste one image on another
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Program exit value will be DT_EXIT_GOOD on success.  
| Any other exit value indicates failure or noncompletion.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dttool.h>

DT_RCSID("tool $RCSfile: paste.c,v $ $Revision: 1.5 $");

#define USAGE "[params] image1 image2 dest"
static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

dt_rc_e dttool__paste(						/* paste one image on another		*/
  dt_ctl_t *ctl,
  dtimage_t *image1,
  dtimage_t *image2,
  dtxy_t x0,
  dtxy_t y0);

/*..........................................................................*/

DTTOOL_MAIN(dttool_paste)
{
  dtimage_t image1, image2;				/* image structures					*/
  
  dt_ctl_t *ctl = &param->ctl;
  int n;

  DT_Q(parse,(param,					/* parse parameters				   	*/
     argc, argv, &n));

  argc -= n;
  argv += n;
  if (argc != 3)						/* check args						*/
    DT_Q(dttool_usage,(param, USAGE));

  DT_Q(dtimage_import,(ctl,				/* get image1 						*/
    &image1, argv[0]));

  DT_Q(dtimage_import,(ctl,				/* get image2 						*/
    &image2, argv[1]));

  if (param->op &&
	  !strcmp(param->op, "center"))
  {
	param->x = (image2.xe-image1.xe) / 2;
	param->y = (image2.ye-image1.ye) / 2;
  }

  DT_Q(dttool__paste,(ctl,				/* paste image1 on image2			*/
    &image1, &image2,
    (dtxy_t)param->x, 
    (dtxy_t)param->y));

  DT_Q(dtimage_export,(ctl,				/* output resulting image			*/
    &image2, argv[2], DTIMAGE_NOPOS, DTIMAGE_NOPOS));

  DT_Q(dtimage_free,(ctl,				/* free image						*/
    &image2));
  DT_Q(dtimage_free,(ctl,			   
    &image1));

  DT_Q(dtparam_free,(param));			/* free param space					*/

  return DT_RC_GOOD;
}

/*..........................................................................*/

static dt_rc_e parse(					/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n)
{
  dt_ctl_t *ctl = &param->ctl;

  DT_Q(dtparam_init,(param));			/* init param structure             */
										
										/* set default for params we want	*/
  DTPARAM_DEFAULT(x, 10);
  DTPARAM_DEFAULT(y, 10);
  DTPARAM_DEFAULT(op, "none");
  DTPARAM_DEFAULT(dbg, 0);

  DT_Q(dttool_parse,(param, F, USAGE,	/* go parse args					*/
    argc, argv, n));

  return DT_RC_GOOD;
}


/*..........................................................................*/

dt_rc_e dttool__paste(					/* paste blobs by box in color		*/
  dt_ctl_t *ctl,
  dtimage_t *image1,
  dtimage_t *image2,
  dtxy_t x0,
  dtxy_t y0)
{
  dtxy_t x, y;
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B08, DTNTYPE_NULL};
  dt_rc_e rc;

  rc = dtimage_check_2(ctl,				/* check simple stuff				*/
    image1, "image1",
    image2, "image2",
    ntypes, F, NULL, NULL);
  if (rc != DT_RC_GOOD)
    return rc;

  for (y=0;
       y<image1->ye &&
       y+y0<image2->ye;
       y++)
  {
    dt_ntype_b08_t *p, *q;				/* coursing pointers			    */
    p = image1->row[y].b08;
    q = image2->row[y+y0].b08 + x0;
    for (x=0; 
         x<image1->xe &&
         x+x0<image2->xe;
         x++)
      q[x] = p[x];
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
