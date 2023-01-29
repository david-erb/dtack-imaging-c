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
| dttool enclose utility - draw enclosing boxes around objects
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
DT_RCSID("tool $RCSfile: enclose.c,v $ $Revision: 1.11 $");
#include <dtack/lut8.h>

#define USAGE "[params] binary gray dest"

#define OP_ENCLOSE (0x01)
#define OP_NUMBER  (0x02)
#define OP_FILL    (0x04)

typedef struct {
  unsigned long minarea;
  unsigned long maxarea;
  unsigned long op;
  double color;
  dtimage_t *output;
} arg_t;


/*..........................................................................*/
dt_rc_e
DTCONFIG_API1
dttool_enclose_deliver(	
  dt_ctl_t *ctl,
  dtrun_t *run,
  dtrun_blob_t *blob,
  void *arg)
{
  DT_F("dttool_enclose_deliver");
  arg_t *a = (arg_t *)arg;
  dtstats_t stats;

  stats.have = 0;					/* no stats computed yet 			*/
  stats.want = 	
    DTSTATS_AREA |
    DTSTATS_BOUNDBOX |
    DTSTATS_CENTROID;

  DT_Q(dtimage_blob_stats,(ctl,
    a->output, run, blob, &stats));

  {
	if (stats.area > a->minarea &&
		(a->maxarea == 0 ||				/* maximum area specified? 			*/
		 stats.area <= a->maxarea))
	{
	  if (ctl->dbg_mask &
          DT_DBG_MASK_STATS)
  	    dt_dbg(ctl, F, DT_DBG_MASK_STATS,
	      "blob %3d: label %g box"
          " (%3d, %3d)-(%3d, %3d),"
          " centroid (%3d, %3d), area %5ld",
		  stats.area,
		  blob->label,
          stats.xmin, stats.ymin,
          stats.xmax, stats.ymax,
		  (int)(stats.xcentroid/2),
		  (int)(stats.ycentroid/2),
	      stats.area);

	  if (a->op & OP_ENCLOSE)
	  {
        DT_Q(dtimage_plot_line,(ctl,
          a->output, 
          stats.xmin, stats.ymin, stats.xmax, stats.ymin, 
          a->color));
        DT_Q(dtimage_plot_line,(ctl,
          a->output, stats.xmax, stats.ymin, stats.xmax, stats.ymax, a->color));
        DT_Q(dtimage_plot_line,(ctl,
          a->output, stats.xmax, stats.ymax, stats.xmin, stats.ymax, a->color));
        DT_Q(dtimage_plot_line,(ctl,
          a->output, stats.xmin, stats.ymax, stats.xmin, stats.ymin, a->color));
	  }
	  if (a->op & OP_NUMBER)
	  {
		char string[32];
		dtxy_t xpos = (int)stats.xcentroid;
		dtxy_t ypos = (int)stats.ycentroid;
		dtxy_t width;
		dtxy_t height;
		sprintf(string, "%ld", stats.area+1);
		width = strlen(string) * 8;
		height = 16;
		if (xpos < width/2)
          xpos = 0;
		else
		  xpos -= width/2;
		if (ypos < height/2)
          ypos = 0;
		else
		  ypos -= height/2;
        DT_Q(dtimage_plot_text_8x16,(	/* draw text					    */
          ctl, a->output, 
          string, xpos, ypos,
          a->color));
	  }
	}

  }
  
  return DT_RC_GOOD;					/* keep doing blobs				    */
}


/*..........................................................................*/
DTTOOL_MAIN(dttool_enclose)
{
  dt_ctl_t *ctl = &param->ctl;
  dtimage_t binary, gray;				/* image structures					*/
  arg_t arg;
  int n;
  int index;

  DT_Q(dtparam_init,(param));			/* init param structure             */

  DT_Q(dtparam_set,(param, 
    DTPARAM_FLAG_PRIMARY,
    &param->background,  NULL, NULL, "0.0",
    &param->op,  NULL, NULL, "enclose",
    &param->color,  NULL, NULL, "5",
    &param->min,  NULL, NULL, "0",
    &param->max,  NULL, NULL, "0",
    &param->connect,  NULL, NULL, "4",
    &param->x,  NULL, NULL, NULL,
    &param->y,  NULL, NULL, NULL,
    NULL));

  DT_Q(dttool_parse,(param, F, USAGE,	/* go parse args					*/
    argc, argv, &n));

  argc -= n;
  argv += n;
  if (argc != 3)						/* check args						*/
    DT_Q(dttool_usage,(param, USAGE));

  DT_Q(dtimage_import,(ctl,				/* get binary image					*/
    &binary, argv[0]));

  DT_Q(dtimage_import,(ctl,				/* get gray image					*/
    &gray, argv[1]));

  DT_MEMSET(&arg, 0, sizeof(arg));			/* clear callback arg structure 	*/

  if (strstr(param->op, "enc"))
    arg.op |= OP_ENCLOSE;
  if (strstr(param->op, "num"))
    arg.op |= OP_NUMBER;
  if (strstr(param->op, "fil"))
    arg.op |= OP_FILL;

  DT_Q(dtlut8_name_to_index,(ctl,		/* convert color argument 			*/
    param->color, &index));
  if (index < 0)
    DT_Q(dtstr_to_double,(ctl,
      param->color, &arg.color))
  else
    arg.color = index;

  arg.output = &gray;					/* set up callback arguments		*/
  arg.minarea = DT_CAST(unsigned long,
    param->min);
  arg.maxarea = DT_CAST(unsigned long,
    param->max);

  DT_Q(dtimage_adj_blob,(ctl,			/* use callback on each blob		*/
    &binary,
	0, 0, binary.xe, binary.ye,
    param->background,					/* background is invisible			*/
	param->connect[0] == '8'?
	  DTIMAGE_ADJ_FLAG_8: 
	  DTIMAGE_ADJ_FLAG_4, 
    dttool_enclose_deliver, &arg));

  DT_Q(dtimage_export,(ctl,				/* output resulting image			*/
    &gray, argv[2], 
    DTIMAGE_NOPOS, DTIMAGE_NOPOS));

  DT_Q(dtimage_free,(ctl,				/* free image						*/
    &gray));
  DT_Q(dtimage_free,(ctl,			   
    &binary));

  DT_Q(dtparam_free,(param));			/* free param space					*/

  return DT_RC_GOOD;
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
