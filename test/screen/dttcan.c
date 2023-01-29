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
#include <dtack/lut8.h>
#include <dtack/keymap.h>
#include <dtack/keybd.h>
#include <dtack/list.h>
#include <dtack/svc.h>
#include <dtack/canvas.h>
#include <dtack/xchg.h>
#include <dtack/prj.h>

#define USAGE "[-Dnnnn] [-w width] [-h height] [server]"

DT_RCSID("test $RCSfile: dttcan.c,v $ $Revision: 1.5 $");

static char *serverlist[] = {			/* list of servers to test		    */
  "fastg,,640",
  "svgac,,640",
  "hws,x",
  "hws,qw4"
};

static const int *runtimelist[] = {		/* runtime availability of servers	*/
  &dtprj_fastgraph,
  &dtprj_svgacc,
  &dtprj_x,
  &dtprj_qw4
};

/*..........................................................................*/

dt_rc_e
dtt_canvas_input(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int xe,
  int ye,
  char *id,
  int *assert_fail)
{
  DT_F("dtt_canvas_input");
  dtimage_t img1;
  int opened = 0;
  int x0, y0;
  int again;
  dt_rc_e rc = DT_RC_GOOD;

										/* ................................ */
  img1.xe = 0;

  DT_C(dtimage_create,(ctl,				/* make an image					*/
    &img1, xe, ye,
    DT_NTYPE_B08));

  DT_C(dtimage_gen_test1,(ctl,			/* generate a test pattern			*/
    &img1));

  DT_C(dtcanvas_open,(ctl, canvas));	/* go into graphics mode			*/
  opened = 1;

  {
	dtlut8_t lut[256];
	DT_Q(dtlut8_standard,(ctl, lut));	/* make standard color lut 			*/
	DT_Q(dtcanvas_putcolor,(ctl,		/* output lut to canvas 			*/
      canvas, lut, 0, 256));
  }


  x0 = y0 = 0;
  again = 1;
  do {
    dt_event_t event;
    int dx = 0;
    int dy = 0;

    DT_C(dtcanvas_clear,(ctl, canvas,	/* clear canvas					    */
      0));

    DT_C(dtxchg_canvas_export_image,	/* put image on the canvas			*/
      (ctl, canvas, &img1,
       x0+dx, y0+dy));

	while (dx == 0 && dy == 0)
	{
      DT_Q(dtcanvas_input,(ctl,			/* wait for input 					*/
        canvas, NULL, 0, 3, &event));
      if (event.type == 
          DT_EVENT_KEYBOARD)
	  {
        dx = dy = 0;
	    switch(event.keys[0])
	    {
	      case DT_KEY_UPARROW:     dy = -1; break;
	      case DT_KEY_DOWNARROW:   dy =  1; break;
	      case DT_KEY_LEFTARROW:   dx = -1; break;
	      case DT_KEY_RIGHTARROW:  dx =  1; break;
	      case DT_KEY_ESCAPE:      again = 0; break;
	    }
        x0 += dx;
        y0 += dy;
	  }
	}
  } while(again);

  DT_C(dtcanvas_close,(ctl, canvas));	/* close canvas					    */
  opened = 0;

cleanup:

  if (opened)
  DT_C(dtcanvas_close,(ctl, canvas));

  if (img1.xe)
  DT_Q(dtimage_free,(ctl, &img1));		/* free image space					*/

  return rc;
}

/*..........................................................................*/

dt_rc_e
dtt_canvas_simple(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  int xe,
  int ye,
  int loops,
  int *assert_fail)
{
  DT_F("dtt_canvas_simple");
  dtimage_t img1;
  dtimage_t img2;
  int opened = 0;
  dtos_time_t mark1, mark2;
  dtos_time_t mark3, mark4;
  long cmprc;
  int loop;
  char tmp[64];
  dt_rc_e rc = DT_RC_GOOD;

										/* ................................ */
  img1.xe = 0;
  img2.xe = 0;

  DT_C(dtimage_create,(ctl,				/* make an image					*/
    &img1, xe, ye,
    DT_NTYPE_B08));

  DT_C(dtimage_create,(ctl,				/* make second image				*/
    &img2, xe, ye,
    DT_NTYPE_B08));

  DT_C(dtimage_gen_test1,(ctl,			/* generate a test pattern			*/
    &img1));

  DT_C(dtcanvas_open,(ctl, canvas));	/* go into graphics mode			*/
  opened = 1;

  DT_C(dtos_time,(ctl, &mark1));
  for (loop=0; loop<loops; loop++)
    DT_C(dtxchg_canvas_export_image,	/* put image on the canvas			*/
      (ctl, canvas, &img1, 0, 0));
  DT_C(dtos_time,(ctl, &mark2));

  DT_C(dtos_time,(ctl, &mark3));
  for (loop=0; loop<loops; loop++)
    DT_C(dtxchg_canvas_import_image,	/* get image from the canvas		*/
      (ctl, canvas, &img2, 0, 0));
  DT_C(dtos_time,(ctl, &mark4));
  
  DT_C(dtcanvas_close,(ctl, canvas));	/* close canvas					    */
  opened = 0;

  sprintf(tmp, "canvas %s,%s put (%d)", 
    canvas->server, canvas->driver, loops);
  DT_C(dtos_time_report,(ctl,
    &mark1, &mark2, tmp))

  sprintf(tmp, "canvas %s,%s get (%d)", 
    canvas->server, canvas->driver, loops);
  DT_C(dtos_time_report,(ctl,
    &mark3, &mark4, tmp))

  DT_C(dtimage_cmp,(ctl,				/* compare two images				*/
    &img1, &img2, 0, 
    &cmprc, NULL, NULL));

  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, cmprc == 0,
      F, "image comparison %ld (0)",
      cmprc);

cleanup:

  if (opened)
  DT_C(dtcanvas_close,(ctl, canvas));

  if (img2.xe)
  DT_Q(dtimage_free,(ctl, &img2));		/* free image space					*/
  if (img1.xe)
  DT_Q(dtimage_free,(ctl, &img1));		/* free image space					*/

  return rc;
}

/*..........................................................................*/

dt_rc_e
dtt_canvas_text(
  dt_ctl_t *ctl,
  dtcanvas_t *canvas,
  char *id,
  int *assert_fail)
{
  DT_F("dtt_canvas_text");
  dtimage_t img;
  int opened = 0;
  int i, j;
char *FONT = 
"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.:_-+=!@#$%^&*() ~{}[]'\"<>,/?";
  dt_rc_e rc = DT_RC_GOOD;

										/* ................................ */
  img.xe = 0;

  DT_C(dtimage_create,(ctl,				/* make an image					*/
    &img, 
    128, 128,
    DT_NTYPE_B08));

  DT_C(dtcanvas_open,(ctl, canvas));	/* go into graphics mode			*/

  for (i=0; i<7; i++)
  {
    for (j=0; j<13; j++)
      printf("%c", FONT[i*13+j]);
    printf("\n");
  }

  DT_C(dtxchg_canvas_import_image,(ctl,
    canvas, &img, 0, 0));

  DT_C(dtcanvas_input,(ctl,				/* wait for input 					*/
    canvas, NULL, 0, 3, NULL));
#ifdef NONO
  DT_C(dtimage_export,(ctl, &img,
    "font.pgm", 0, 0));
#endif
cleanup:

  if (opened)
  DT_C(dtcanvas_close,(ctl, canvas));

  if (img.xe)
  DT_Q(dtimage_free,(ctl, &img));		/* free image space					*/

  return rc;
}

/*..........................................................................*/

DTT_MAIN(dttcan)
{
  DT_F("dttcan");
  dt_ctl_t *ctl = &param->ctl;
  int assert_fail = 0;
  int n, i;
  dtcanvas_t canvas;
  int have_canvas = 0;
  char **servers;
  int nservers;
  const int **runtime;
  dt_rc_e rc = DT_RC_GOOD;
										/* ................................ */
  DT_Q(dtparam_init,(param));			/* init param structure             */

  DTPARAM_DEFAULT(dbg, 0);				/* set default for params we want	*/
  DTPARAM_DEFAULT(mem, 0);
  DTPARAM_DEFAULT(op, "default");
  DTPARAM_DEFAULT(w, 0);
  DTPARAM_DEFAULT(h, 0);
  DTPARAM_DEFAULT(dx, 256);
  DTPARAM_DEFAULT(dy, 256);
  DTPARAM_DEFAULT(n, 10);

  DT_Q(dt_enter_param,(					/* go parse args					*/
    argc, argv, F, USAGE, param, &n));

  argc -= n;
  argv += n;

  if (argc == 0)						/* no command line arguments?	    */
  {
    servers = serverlist;
    nservers = sizeof(serverlist) /
      sizeof(*serverlist);
    runtime = runtimelist;
  }
  else
  {
    servers = argv;
    nservers = argc;
    runtime = NULL;
  }
										/* ................................ */
  for (i=0; i<nservers; i++)
  {
    if (runtime != NULL &&				/* server not on command line?	    */
        *runtime[i] == 0)				/* but no server runtime support?	*/
      continue;

    DT_C(dtcanvas_init,(ctl,			/* obtain canvas resources		    */
      &canvas, servers[i]));
    have_canvas = 1;
  
    if (!strcmp(param->op, "all") ||
        !strcmp(param->op, "default") ||
        !strcmp(param->op, "fast"))
    {
      DT_C(dtt_canvas_simple,(ctl, 		/* write/read, no pause				*/
        &canvas,   
        (int)param->dx, 
        (int)param->dy,
        1,
        &assert_fail));
    }
  										/* ................................ */
    if (!strcmp(param->op, "all") ||
        !strcmp(param->op, "time"))
    {
      DT_C(dtt_canvas_simple,(ctl, 		/* write/read multiple, no pause	*/
        &canvas,   
        (int)param->dx, 
        (int)param->dy,
        (int)param->n,
        &assert_fail));
    }
  										/* ................................ */
    if (!strcmp(param->op, "all") ||
        !strcmp(param->op, "input"))
    {
      DT_C(dtt_canvas_input,(ctl,		/* display and handle input			*/
        &canvas,   
        (int)param->dx, 
        (int)param->dy,
        F, &assert_fail));
    }
    DT_C(dtcanvas_uninit,(ctl,			/* free canvas resources		    */
      &canvas));
    have_canvas = 0;
  }
										/* ................................ */

cleanup:
  if (have_canvas)
    DT_I(dtcanvas_uninit,(ctl,			/* free canvas resources		    */
      &canvas));

  if (assert_fail)						/* any failed assertions?		    */
    return DT_RC_BAD;
  else
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
