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
| dts_per87 sample program - Perez&Gonzolez 1987
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
#include <dtack/os.h>
#include <dtwin.h>

DT_RCSID("tool $RCSfile: blob.c,v $ $Revision: 1.8 $");

#define USAGE "[params] input [output]"

static dt_rc_e local_parse(				/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n);

static dt_rc_e interact(				/* interact with blobs			    */
  dtparam_t *param,
  dtimage_t *image,
  char *window);
  
typedef struct {
  long nblobs;
  long n_sum;
  long n_min;
  long n_max;
  long xs_sum, ys_sum;
  dtxy_t x0_min, y0_min;
  dtxy_t xe_max, ye_max;
  dtxy_t xi, yi;						/* seed point					    */
  dtxy_t xs, ys;						/* coordinate sums					*/
  dtxy_t x0, y0;						/* upper left of enclosing box	    */
  dtxy_t xe, ye;						/* lower right of enclosing box	    */
  long n;								/* count							*/
  double v;								/* blob value					    */
} dttool_blob_t;

typedef int dttool_blob_id_t;

typedef struct {
  int n;								/* current count				    */
  int m;								/* max count					    */
  dttool_blob_t *blob;						/* blob list					    */
} dttool_blob_list_t;

#define DTTOOL_BLOB_FORMAT \
  "n %6ld s (%6ld,%6ld) 0 (%4ld,%4ld) e (%4ld,%4ld)"

static dtimage_adj_callback_f dttool_blob_callback;

/*..........................................................................*/

static
dt_rc_e
dttool_blob_cmp(
  dt_ctl_t *ctl,
  dttool_blob_t *blob1,
  dttool_blob_t *blob2,
  int *cmp)
{
  if (blob1->n  == blob2->n  &&
      blob1->xs == blob2->xs &&
      blob1->ys == blob2->ys &&
      blob1->x0 == blob2->x0 &&
      blob1->y0 == blob2->y0 &&
      blob1->xe == blob2->xe &&
      blob1->ye == blob2->ye)
    *cmp = 0;
  else
    *cmp = 1;
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttool_blob_find(
  dt_ctl_t *ctl,
  dttool_blob_list_t *list,
  dttool_blob_t *blob,
  dttool_blob_id_t *id)
{
  DT_F("dttool_blob_find");
  int i;
  int cmp;
  for (i=0; i<list->n; i++)
  {
    DT_Q(dttool_blob_cmp,(ctl,
      &list->blob[i], blob, &cmp));
    if (!cmp)
      break;
  }
  if (i == list->n)
    *id = -1;
  else
    *id = i;
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttool_blob_add(
  dt_ctl_t *ctl,
  dttool_blob_list_t *list,
  dttool_blob_t *blob,
  dttool_blob_id_t *id)
{
  DT_F("dttool_blob_add");
  if (list->n == list->m)
    return dt_err(ctl, F,
      "blob list full (%d)",
      list->n);
  list->blob[list->n] = *blob;
  if (id != NULL)						/* caller wants id back?		    */
    *id = list->n;
  list->n++;
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttool_blob_del(
  dt_ctl_t *ctl,
  dttool_blob_list_t *list,
  dttool_blob_id_t id)
{
  int i;
  list->n--;
  for (i=id; i<list->n; i++)
    list->blob[i] = list->blob[i+1];
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttool_blob_sprintf(
  dt_ctl_t *ctl,
  dttool_blob_t *blob,
  char *string)
{
  sprintf(string, DTTOOL_BLOB_FORMAT,
    blob->n,
    (long)blob->xs, (long)blob->ys,
    (long)blob->x0, (long)blob->y0,
    (long)blob->xe, (long)blob->ye);
  return DT_RC_GOOD;
}

/*..........................................................................
 * called for each pixel by dtimage_adj_callback
 *..........................................................................*/

static int dttool_blob_callback(
  dt_ctl_t *ctl,
  dtimage_adj_callback_arg_t *arg,
  dtxy_t x1,
  dtxy_t y1,
  dtxy_t x2,
  dtxy_t y2)
{
  dttool_blob_t *blob = (dttool_blob_t *)arg->arg;
  int flag = arg->flag;
  dtxy_t x, y;

  arg->flag = 0;						/* clear flag for next blob		    */

  if (flag & DTIMAGE_ADJ_FLAG_END)		/* finishing an old blob?			*/
  {
    blob->xe -= blob->x0 - 1;			/* lower right corner --> box size 	*/
    blob->ye -= blob->y0 - 1;

    blob->nblobs++;
    if (blob->nblobs == 1)				/* very first blob?				    */
    {
      blob->n_sum = blob->n;
      blob->xs_sum = blob->xs;
      blob->ys_sum = blob->ys;
	  blob->n_min = blob->n;
	  blob->n_max = blob->n;
      blob->x0_min = blob->x0;
      blob->y0_min = blob->y0;
      blob->xe_max = blob->xe;
      blob->ye_max = blob->ye;
	}
	else
	{
	  blob->n_sum += blob->n;
	  blob->xs_sum += blob->xs;
	  blob->ys_sum += blob->ys;
	  blob->n_min = DT_MIN(blob->n_min, blob->n);
	  blob->n_max = DT_MAX(blob->n_max, blob->n);
	  blob->x0_min = DT_MIN(blob->x0_min, blob->x0);
	  blob->y0_min = DT_MIN(blob->y0_min, blob->y0);
	  blob->xe_max = DT_MAX(blob->xe_max, blob->xe);
	  blob->ye_max = DT_MAX(blob->ye_max, blob->ye);
	}

    if (!(flag & DTIMAGE_ADJ_FLAG_BEG))	/* not also starting a new blob?	*/
      return 1;
  }


  if (flag & DTIMAGE_ADJ_FLAG_BEG)		/* starting a new blob?				*/
  {
	blob->n = 0;
	blob->xi = x1;						/* remember seed point			    */
	blob->yi = y1;
	blob->xs = 0;
	blob->ys = 0;
	blob->x0 = x1;
	blob->y0 = y1;
	blob->xe = x1;
	blob->ye = y1;
  }

  for (y=y1; y<=y2; y++)
    for (x=x1; x<=x2; x++)
	{
	  blob->n++;						/* sum into existing blob			*/
	  blob->xs += x;
	  blob->ys += y;
	  blob->x0 = DT_MIN(blob->x0, x);
	  blob->y0 = DT_MIN(blob->y0, y);
	  blob->xe = DT_MAX(blob->xe, x);
	  blob->ye = DT_MAX(blob->ye, y);
	}
  
  return 1;								/* keep doing blobs				    */
}

/*..........................................................................*/
static
dt_rc_e
dttool_blob_pick(
  dt_ctl_t *ctl,
  dtimage_t *image,
  char *window,
  dtwin_state_t *state,
  dttool_blob_list_t *list,
  double foreground,
  double color,
  int *redisplay)
{
  DT_F("dttool_blob_pick");
  dttool_blob_t blob;
  char string[256];
  char *act;
  double v;
  dttool_blob_id_t id;
  dtimage_t patch;

  DT_Q(dtimage_get_pixel,(ctl,
    image, 
    state->x, state->y, &v));
  if (v == color)
    v = foreground;
  else
  if (v == foreground)
    v = color;
  else
  {
    dt_say(ctl, NULL,
      "value %d at (%4d,%4d) not foreground %d or color %d",
      (int)v, state->x, state->y,
      (int)foreground,
      (int)color);
    return DT_RC_GOOD;
  }
      
  blob.nblobs = 0;
  DT_Q(dtimage_adj_fill1,(ctl,			/* try to find the blob here	    */
    image,
    state->x, state->y, v,
    dttool_blob_callback,
    (void *)&blob));

  if (blob.n == 0)						/* no blob here?				    */
    dt_say(ctl, F,
      "no blob at (%4d,%4d)",
      state->x, state->y);
  else
  {
    DT_Q(dttool_blob_find,(ctl,			/* see if we have blob in list	    */
      list, &blob, &id));
    if (id == -1)						/* not yet in list?				    */
	{
      DT_Q(dttool_blob_add,(ctl,			/* add it						    */
        list, &blob, &id));
      act = "add";
	}
    else								/* already in list?			   	    */
	{
      DT_Q(dttool_blob_del,(ctl,			/* delete it					    */
        list, id));
      act = "del";
	}
    DT_Q(dttool_blob_sprintf,(ctl,			/* print out blob description	    */
      &blob, string));
    DT_Q(dtimage_overlay,(ctl,		  	/* allocate patch over blob		    */
      image, &patch,
      blob.x0, blob.y0,
      blob.xe, blob.ye));
    DT_Q(dtimage_export_patch,(ctl,		/* output just the patch		    */
      &patch, window,
      blob.x0, blob.y0));
    DT_Q(dtimage_free,(ctl, &patch));	/* no more need for patch structure	*/
    dt_say(ctl, NULL, "%s: %s",
      act, string);
    *redisplay = 0;						/* don't redisplay				    */
  }
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
void
summary(
  dt_ctl_t *ctl,
  dttool_blob_t *blob,
  char *F,
  char *prefix)
{
  dt_say(ctl, F, "%s%ld blobs",
    prefix,
    blob->nblobs);
  dt_say(ctl, F,
    "%sarea: total %ld, min %ld, ave %1.2f, max %ld pixels",
    prefix,
    blob->n_sum,
    blob->n_min,
    (double)blob->n_sum / (double)blob->nblobs,
    blob->n_max);
  dt_say(ctl, F, 
    "%slocation: min (%ld,%ld) centroid (%1.2f,%1.2f) max (%ld,%ld)",
    prefix,
    blob->x0_min, blob->y0_min,
    (double)blob->xs_sum / (double)blob->n_sum,
    (double)blob->ys_sum / (double)blob->n_sum,
    blob->xe_max, blob->ye_max);
}

/*..........................................................................*/

DTTOOL_MAIN(dttool_blob)
{
  dtimage_t input;						/* image structure					*/
  
  dt_ctl_t *ctl = &param->ctl;
  int iswindow;
  int n;
  dt_rc_e rc;

  rc = local_parse(param,				/* parse parameters				   	*/
    argc, argv, &n);
  if (rc != DT_RC_GOOD) return rc;

  argc -= n;
  argv += n;

  if (argc < 1)							/* check remaining args				*/
    return dt_usage_say(
      param, USAGE);

  DT_Q(dtimage_import,(ctl,				/* get input image					*/
    &input, argv[0]));

  if (!strncmp(param->op, "summary",		/* just print summary				*/
        strlen(param->op)))
  {
    dttool_blob_t blob;

    if (argc != 1)						/* check remaining args				*/
      return dt_usage_say(
        param, USAGE);

    blob.nblobs = 0;
    DT_Q(dtimage_adj,(					/* get summary of blobs				*/
      ctl,
      &input, param->background,
      dttool_blob_callback,
      (void *)&blob));

    summary(ctl, &blob, "", "  ");		/* print summary				    */
  }
  else
  if (!strncmp(param->op, "compare",	/* compare blob finding methods		*/
        strlen(param->op)))
  {
    dttool_blob_t blob_run, blob_fill;
    dtos_time_t mark1, mark2;

    if (argc != 1)						/* check remaining args				*/
      return dt_usage_say(
        param, USAGE);

    DT_Q(dtos_time,(ctl, &mark1));
    DT_MEMSET(&blob_run, 0, 
      sizeof(blob_run));
    DT_Q(dtimage_adj_run,(				/* do blobs by run method		    */
      ctl,
      &input, param->background,
      dttool_blob_callback,
      (void *)&blob_run));
    DT_Q(dtos_time,(ctl, &mark2));
    dt_dbg(ctl, F, DT_DBG_MASK_TIME,
      "%-16s run  user %1.2f, system %1.2f, wall %1.2f seconds",
      argv[0],
      mark2.user - mark1.user,
      mark2.system - mark1.system,
      mark2.wall - mark1.wall);

    DT_Q(dtos_time,(ctl, &mark1));
    DT_MEMSET(&blob_fill, 0, 
      sizeof(blob_fill));
    DT_Q(dtimage_adj_fill,(				/* do blobs by fill method		    */
      ctl,
      &input, param->background,
      dttool_blob_callback,
      (void *)&blob_fill));
    DT_Q(dtos_time,(ctl, &mark2));
    dt_dbg(ctl, F, DT_DBG_MASK_TIME,
      "%-16s fill user %1.2f, system %1.2f, wall %1.2f seconds",
      argv[0],
      mark2.user - mark1.user,
      mark2.system - mark1.system,
      mark2.wall - mark1.wall);

    if (blob_run.nblobs != blob_fill.nblobs ||
		blob_run.n_sum  != blob_fill.n_sum ||
        blob_run.xs_sum != blob_fill.xs_sum ||
        blob_run.ys_sum != blob_fill.ys_sum ||
        blob_run.n_min  != blob_fill.n_min ||
        blob_run.n_max  != blob_fill.n_max ||
        blob_run.x0_min != blob_fill.x0_min ||
        blob_run.y0_min != blob_fill.y0_min ||
        blob_run.xe_max != blob_fill.xe_max ||
        blob_run.ye_max != blob_fill.ye_max)
	{
	  summary(ctl, &blob_run,
	    "", "run  ");
	  summary(ctl, &blob_run,
        "", "fill ");
	}
  }
  else								
  if (!strcmp(param->op, "interact"))	/* caller wants to interact?	  	*/
  {
    char *window;

    if (argc != 2)						/* check remaining args				*/
      return dt_usage_say(
        param, USAGE);

    window = argv[1];

    DT_Q(dtwin_iswindow,(ctl, window,
      &iswindow));

    if (!iswindow)						/* dest must be window			    */
      return dt_err(ctl, F,
        "\"%s\" is not a window",
        window);

    if (strcmp(argv[0], window))		/* we didn't just get it from here?	*/
      DT_Q(dtimage_export,(ctl,			/* display input image				*/
        &input, window, DTIMAGE_NOPOS, DTIMAGE_NOPOS));

    rc = interact(param,
      &input, window);
  }
  else
    rc = dt_err(ctl, F, 
      "unknown op \"%s\"", param->op);

  DT_Q(dtimage_free,(ctl,				/* free image space				    */
    &input));

  DT_Q(dtparam_free,(param));			/* free param space					*/

  return rc;
}

/*..........................................................................*/

static
dt_rc_e
interact(
  dtparam_t *param,
  dtimage_t *input,
  char *window)
{
  dt_ctl_t *ctl = &param->ctl;
  const char *blob_temp = "blob list";
  char input_window[DT_WINDOWNAME_MAXLEN];
  dtwin_state_t state;
  dttool_blob_list_t list;
  dt_rc_e rc;

  list.m = 1000;						/* space for some blobs			    */
  DT_Q(dtos_malloc2,(ctl,
    (void **)&list.blob,
    list.m * sizeof(*list.blob),
    F, blob_temp));

  do {
    DT_G(dtwin_wait,(ctl,				/* wait for user input			    */
      input_window, &state));
    if (rc != DT_RC_GOOD)
      break;
    dtwin_dbg_state(ctl, F,				/* debug new window state		    */
      DT_DBG_MASK_ASYNC,
      input_window, &state);
    if (!strcmp(input_window, window))	/* this is an event on our window?	*/
	{
      int redisplay = 0;
      if (state.left)					/* left button down?			    */
        DT_G(dttool_blob_pick,(ctl, 
          input, window,
          &state, &list,
          param->foreground,
          param->color,
          &redisplay))
      else
        dt_say(ctl, F,
          "nothing done for event");
      if (rc == DT_RC_GOOD &&
          redisplay)
        DT_G(dtimage_export,(ctl,		/* redisplay image					*/
          input, window, DTIMAGE_NOPOS, DTIMAGE_NOPOS));
	}									/* event not in our window?			 */
    else
      dt_say(ctl, F,
        "event not in our window");
  } while(rc == DT_RC_GOOD &&			/* until error						*/
          state.active &&				/* or until window destroyed	    */
          !state.right);				/* or until right button hit	    */

  DT_Q(dtos_free2,(ctl, list.blob,		/* free blob list space			    */
    F, blob_temp));

  return rc;
}

/*..........................................................................*/

static dt_rc_e local_parse(				/* parse params for this program	*/
  dtparam_t *param,
  int argc,
  char *argv[],
  int *n)
{
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;

  DT_Q(dtparam_init,(param));			/* init param structure             */

										/* set default for params we want	*/
  DTPARAM_DEFAULT(foreground, 255.0);	/* blobs initially selectable	    */
  DTPARAM_DEFAULT(background, 0.0);		/* invisible background				*/
  DTPARAM_DEFAULT(color, 4);			/* color it changes to after pick   */
  DTPARAM_DEFAULT(op, "interact");		/* operation to do on blobs			*/
  DTPARAM_DEFAULT(dbg, 0);

  rc = dt_enter_param(					/* go parse args					*/
    argc, argv, F, USAGE,
    param, n);

  return rc;
}

/*..........................................................................*/






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
