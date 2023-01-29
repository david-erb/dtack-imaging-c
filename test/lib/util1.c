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

#include <dtack/t.h>
#include <dtack/stack.h>
#include <dtack/mem.h>

DT_RCSID("test $RCSfile: util1.c,v $ $Revision: 1.7 $");

#define USAGE "[-Dnnnn]"

extern
dt_rc_e
DTCONFIG_API1
dtimage_gen_interlock16(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double background,
  double foreground);

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttutil_read_runadj(
  dt_ctl_t *ctl,
  FILE *file,
  char *desc,
  int *ncols,
  int *nrows,
  int *nruns,
  int *nblobs,
  dtntype_s4_t *expect_labels,
  dtntype_s4_t *expect_areas,
  dtntype_s4_t *expect_sums,
  dt_ntype_b08_t *data,
  unsigned long *flags,
  int *good)
{					
  DT_F("dttutil_read1");
  dt_ntype_b08_t *p;
  int n;
  int v;
  int i, j;

    n = fscanf(file, " DESC %s", desc);
    if (n != 1)
	{
	  *good = 0;
	  return DT_RC_GOOD;
	}

    n = fscanf(file, " FLAGS %lu NCOLS %d NROWS %d NRUNS %d NBLOBS %d",
      flags, ncols, nrows, nruns, nblobs);
    if (n != 5)
      return dt_err(ctl, F,
		"error reading FLAGS... line element %d for \"%s\"",
        n+1, desc);

    for (i=0; i < *nruns; i++)
	{
	  long l;
      if (i == 0)
	    n = fscanf(file, " LABELS %ld", &l);
	  else
	    n = fscanf(file, "%ld", &l); 
      if (n != 1)
        return dt_err(ctl, F, "error reading label %d for \"%s\"", i, desc);
	  expect_labels[i] = (dtntype_s4_t)l;
    }

    for (i=0; i < *nblobs; i++)
	{
	  long l;
      if (i == 0)
	    n = fscanf(file, " AREAS %ld", &l); 
	  else
	    n = fscanf(file, "%ld", &l); 
      if (n != 1)
        return dt_err(ctl, F, "error reading area %d for \"%s\"", i, desc);
	  expect_areas[i] = (dtntype_s4_t)l;
    }

    for (i=0; i < *nblobs; i++)
	{
	  long l;
      if (i == 0)
	    n = fscanf(file, " SUMS %ld",  &l);
	  else
	    n = fscanf(file, "%ld", &l); 
      if (n != 1)
        return dt_err(ctl, F, "error reading sum %d for \"%s\"", i, desc);
	  expect_sums[i] = (dtntype_s4_t)l;
    }

    p = data;
    for (i=0; i < *nrows; i++)
    for (j=0; j < *ncols; j++)
	{
      if (i == 0 && j == 0)
  	    n = fscanf(file, " %d", &v);
	  else
  	    n = fscanf(file, "%d", &v);
      if (n != 1)
        return dt_err(ctl, F,
          "error reading data (%d,%d) for \"%s\"", j, i, desc);
      *p++ = v;
    }

  *good = 1;
  return DT_RC_GOOD;    
}
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtt_image_synthetic(					/* gen image and call function		*/
  dtparam_t *param,
  const char *select,
  dt_ntype_e ntype,					
  dtt_image_f *do_image,				/* function to call				    */
  void *arg)							/* caller's arg to pass to function	*/
{
  DT_F("dtt_image_synthetic");

  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;

  dtimage_t image;
  dtxy_t x, y;

  DT_Q(dtimage_create,(ctl, &image,
    param->w, param->h,
    ntype));

  if (!strcmp(select, "empty"))
  {
	DT_C(dtimage_constant,(ctl,			/* no blobs						    */
      &image, 0.0));
    DT_C(do_image,(ctl, &image,
      0, 
      arg, "empty", param->n));
  }

  if (!strcmp(select, "solid"))
  {
	DT_C(dtimage_constant,(ctl,			/* one solid blob				    */
      &image, 1.0));
    DT_C(do_image,(ctl, &image,
      1,
      arg, "solid", param->n));
  }

  if (!strcmp(select, "checquer"))
  {
	double foreground = 2;
	DT_C(dtimage_constant,(ctl,			/* checkerboard					    */
      &image, 1.0));
    if (rc == DT_RC_GOOD)
    for (y=0; y<image.ye; y++)
	{
      for (x=(y&1); x<image.xe; x+=2)
        DTIMAGE_SETPIX1(DT_C, ctl,
          &image, x, y, foreground);
	}
    DT_C(do_image,(ctl, &image,
      param->w * param->h,
      arg, "checquer", param->n));
  }

  if (!strcmp(select, "tiny_blobs"))
  {
    DT_C(dtimage_gen_rect_islands,(		/* lots of tiny blobs			    */
      ctl, &image, 0.0, 1.0,
      1, 1, 1));
    DT_C(do_image,(ctl, &image,
      param->w/2 * param->h/2,
      arg, "tiny_blobs", param->n));
  }

  if (!strcmp(select, "tiny_holes"))
  {
    DT_C(dtimage_gen_rect_islands,(		/* lots of tiny holes in big blob   */
      ctl, &image, 1.0, 0.0,
      1, 1, 1));
    DT_C(do_image,(ctl, &image,
      1,
      arg, "tiny_holes", param->n));
  }

  if (!strcmp(select, "tree"))
  {
    DT_C(dtimage_gen_rect_tree,(		/* tree								*/
      ctl, &image, 1.0, 2.0, 9999));
    DT_C(do_image,(ctl, &image,
      2,
      arg, "tree", param->n));
  }

  if (!strcmp(select, "spiral"))
  {
    DT_C(dtimage_gen_rect_spiral,(		/* spiral							*/
      ctl, &image, 1.0, 2.0, 2));
    DT_C(do_image,(ctl, &image,
      2,
      arg, "spiral", param->n));
  }

  if (!strcmp(select, "interlock16"))
  {
    DT_C(dtimage_gen_interlock16,(		/* interlock16						*/
      ctl, &image, 1.0, 2.0));
    DT_C(do_image,(ctl, &image,
      513,
      arg, "interlock16", param->n));
  }

cleanup:
  DT_I(dtimage_free,(ctl, &image));

  return rc;
}
/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtt_image_list(							/* read/gen images and do them		*/
  dtparam_t *param,
  const char **list,
  const char *select,
  dt_ntype_e ntype,
  dtt_image_f *do_image,				/* function to call				    */
  void *arg)							/* caller's arg to pass to function	*/
{
  DT_F("dtt_image_list");
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t image;
  dtimage_t cast;
  char filename[256];
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  image.xe = 0;
  cast.xe = 0;

  for (i=0; list[i]; i++)
  {
	if (select && *select &&			/* caller has a specific selection? */
		!DT_ABBREV(list[i], select)) 
      continue;

	if (!strchr(list[i], '.'))			/* no dot in the name? 				*/
	{
	  DT_Q(dtt_image_synthetic,(param,
        list[i], ntype, do_image, arg));
	  continue;
	}

    if (list[i][0] == '/')				/* leading slash in the name?		*/
      DT_Q(dtstr_printf,(ctl,			/* get it from /img 				*/
        filename, sizeof(filename),
        "/img%s", list[i]))
    else								/* no leading slash in the name?   	*/
      DT_Q(dtstr_printf,(ctl,			/* get it from datadir 				*/
        filename, sizeof(filename),
        "%s/%s", param->datadir,
        list[i]));

    DT_C(dtimage_import,(ctl,			/* get image from file or window	*/
      &image, filename));

	DT_C(dtimage_create,(ctl,			/* space to change precision into 	*/
      &cast, image.xe, image.ye,
      ntype));
	DT_C(dtimage_cast,(ctl, &image,		/* change precision 				*/
      &cast));
	DT_C(dtimage_free,(ctl, &image));	/* free image from file 			*/
	image.xe = 0;

    DT_C(do_image,(ctl, &cast,
      -1,
      arg, list[i], param->n));

    DT_C(dtimage_free,(ctl, &cast));
	cast.xe = 0;
  }

cleanup:
  if (cast.xe)
    DT_I(dtimage_free,(ctl, &cast));
  if (image.xe)
    DT_I(dtimage_free,(ctl, &image));

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
