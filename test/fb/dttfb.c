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
 * make -f qw1.mak SRC=/usr/erb/prj/dt OUT=/usr/erb/prj/dtqw1
 *..........................................................................*/
#include <dtack/t.h>
#include <dtack/mem.h>
#include <dtack/lut8.h>
#include <dtack/color.h>
#include <dtack/fb.h>

DT_RCSID("test $RCSfile: dttfb.c,v $ $Revision: 1.8 $");

#ifndef DTDEPEND
#  include <zlib.h>
#endif

static const char *ops[] = {"all",
  "syncsave", 
  "overlapsave", 
  "asyncsave", 
  "clear", 
  "freeze", 
  "acquire", 
  "snapshot", 
  "reset", 
  NULL};

#define usage ""

/*..........................................................................*/

static
dt_rc_e 
dttfb_alloc(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void **b,
  long size,
  int max,
  int *got)
{
  DT_F("dttfb_alloc");
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  for (i=0; i<max; i++)
    DT_C(dtos_malloc2,(ctl,				/* space for full-image buffer 		*/
      (void **)&b[i], size,
      F, "buffer"));

cleanup:
  *got = i;

  for (i=0; i<4 && *got>0; i++)
  {
    *got -= 1;
    DT_I(dtos_free2,(ctl, b[*got],
      F, "buffer overflow"));
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e 
dttfb_free(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  void **b,
  int max)
{
  DT_F("dttfb_free");
  int i;
  dt_rc_e rc = DT_RC_GOOD;

  for (i=0; i<max; i++)
    DT_I(dtos_free2,(ctl,				/* space for full-image buffer 		*/
      b[i], F, "buffer"));

  return rc;
}

/*..........................................................................*/

static
dt_rc_e 
dttfb_asyncsave(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int max,
  dt_color_org_t *org,
  long delay,
  const char *pathname)
{
  DT_F("dttfb_asyncsave");
  void **b;
  long size = 
    (long)(fb->rowsize) *
    (long)(fb->ye);
  dtfd_t fd;
  int i;
  dt_rc_e rc;

  fd.fd = -1;

  DT_C(dtos_malloc2,(ctl,
    (void **)&b, max*sizeof(*b),
    F, "buffer pointers"));

  DT_C(dttfb_alloc,(ctl, fb, b, 
    size, max, &max));

  dt_say(ctl, F,
    "ready for %d of size %ld", 
    max, size);

  for (i=0; i<max; i++)					/* fill the memory 					*/
  {
	DT_C(dtfb_control,(ctl, fb,			/* snapshot memory					*/
      DTFB_CONTROL_SNAPSHOT));
    DT_C(dtfb_getblock,(ctl, fb,		/* suck bytes out 					*/
      b[i], 0, 0, 
      fb->xe, fb->ye));
	dt_say(ctl, F, "got %3d", i);
    if (delay)
      DT_C(dtos_delay,(ctl, delay));
  }

  for (i=0; i<max; i++)					/* write bytes to file 				*/
  {
	char filename[128];
    dtstr_printf(ctl,					/* name of the file 				*/
      filename, sizeof(filename),
      pathname, i);
    DT_C(dtfd_open,(ctl, &fd,			/* open file for output 			*/
      filename, "w"));
    DT_C(dtfd_write_all,(ctl, &fd,		/* write the entire fb to the file 	*/
      b[i], size));
	DT_C(dtfd_close,(ctl, &fd));
	dt_say(ctl, F, "wrote %s", 
      filename);
    fd.fd = -1;
  }

cleanup:
  if (fd.fd != -1)						/* file was opened? 				*/
    DT_I(dtfd_close,(ctl, &fd));		/* close it 						*/

  if (b != NULL)
  {
	DT_I(dttfb_free,(ctl, fb, b, max));	/* free individual buffers 			*/
    DT_I(dtos_free2,(ctl, b,			/* free buffer pointers 			*/
      F, "buffer pointers"));
  }

  return rc;
}

/*..........................................................................*/

static
dt_rc_e 
dttfb_syncsave(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int max,
  dt_color_org_t *org,
  long delay,
  const char *pathname)
{
  DT_F("dttfb_syncsave");
  void **b;
  long size = 
    (long)(fb->rowsize) *
    (long)(fb->ye);
  dtfd_t fd;
  int i;
  dt_rc_e rc;

  fd.fd = -1;

  DT_C(dtos_malloc2,(ctl,
    (void **)&b, size,
    F, "buffer"));

  dt_say(ctl, F,
    "ready for %d of size %ld", 
    max, size);

  for (i=0; i<max; i++)					/* fill the memory 					*/
  {
	char filename[128];
	DT_C(dtfb_control,(ctl, fb,			/* snapshot memory					*/
      DTFB_CONTROL_SNAPSHOT));
    DT_C(dtfb_getblock,(ctl, fb,		/* suck bytes out 					*/
      b, 0, 0, 
      fb->xe, fb->ye));
    dtstr_printf(ctl,					/* name of the file 				*/
      filename, sizeof(filename),
      pathname, i);
    DT_C(dtfd_open,(ctl, &fd,			/* open file for output 			*/
      filename, "w"));
    DT_C(dtfd_write_all,(ctl, &fd,		/* write the entire fb to the file 	*/
      b, size));
	DT_C(dtfd_close,(ctl, &fd));
    fd.fd = -1;
	dt_say(ctl, F, "wrote %s", 
      filename);
    if (delay)
      DT_C(dtos_delay,(ctl, delay));
  }

cleanup:
  if (fd.fd != -1)						/* file was opened? 				*/
    DT_I(dtfd_close,(ctl, &fd));		/* close it 						*/

  if (b != NULL)
	DT_I(dtos_free,(ctl, b));			/* free buffer 						*/

  return rc;
}

/*..........................................................................*/

static
dt_rc_e 
dttfb_overlapsave(
  dt_ctl_t *ctl,
  dtfb_t *fb,
  int max,
  int sigma,
  dt_color_org_t *org,
  long delay,
  const char *pathname)
{
  DT_F("dttfb_overlapsave");
  unsigned char *b = NULL;
  unsigned long *a = NULL;
  unsigned long *z = NULL;
  unsigned long *data;
  dt_color_convert_t convert;
  void *out = NULL;
  unsigned int n =
    fb->xe * fb->ye;
  unsigned int size =						/* size required for input 			*/
    (long)(fb->rowsize) *
    (long)(fb->ye);
  unsigned int zmax = 0;
  unsigned int datasize;
  dtfd_t fd;
  int i;
  dt_rc_e rc;

  fd.fd = -1;
  convert.initialized = 0;

  DT_C(dtos_malloc2,(ctl,
    (void **)&b, size,
    F, "input buffer"));

  DT_C(dtos_malloc2,(ctl,
    (void **)&a, size*sizeof(*a),
    F, "accumulator"));

  if (fb->org.org != org->org)			/* color conversion required? 		*/
  {
    DT_C(dt_color_convert_init,(ctl,
      &convert, &fb->org, org));
	DT_C(dt_color_org_size,(ctl,		/* size required for output 		*/
      org, n, &size));
    DT_C(dtos_malloc2,(ctl,				/* get space for output 			*/
      (void **)&out, size,
      F, "out buffer"));
  }

  if (pathname[strlen(pathname)-1] == 
	  'z')
  {
	zmax = size;
	DT_C(dtos_malloc2,(ctl,
      (void **)&z, zmax,
      F, "compressed output buffer"));
  }

  dt_dbg(ctl, F, DT_DBG_MASK_USR1,
    "ready for %d frames of %ux%u"
    " (%u pixels)"
    " (output bytes %u)", 
    max, fb->xe, fb->ye, n, size);

  DT_C(dtfb_control,(ctl, fb,			/* start acquiring					*/
    DTFB_CONTROL_ACQUIRE));
  for (i=0; i<max; i++)					/* fill the memory 					*/
  {
	int loop;
	char filename[128];
	int j;
	
    for (loop=0; 
	     loop==0 || 
		 loop<sigma; 
	     loop++)
    {
      DT_C(dtfb_getblock,(ctl, fb,		/* suck bytes out 					*/
        b, 0, 0, fb->xe, fb->ye));
	
      DT_C(dtfb_control,(ctl, fb,		/* start acquiring again			*/
        DTFB_CONTROL_ACQUIRE));

	  if (loop < sigma-1)				/* we are doing frame averaging? 	*/
	  if (loop == 0)					/* first time through? 				*/
	    for (j=0; j<size; j++)
	      a[j] = b[j];
	  else								/* next time through? 				*/
	    for (j=0; j<size; j++)
	      a[j] += b[j];
	}

	if (sigma == 2)
      for (j=0; j<size; j++)
	    b[j] = (a[j] + b[j]) >> 1;
	else
	if (sigma == 4)
      for (j=0; j<size; j++)
	    b[j] = (a[j] + b[j]) >> 2;
	else
	if (sigma > 1)
	{
	  const unsigned long t = sigma;
      for (j=0; j<size; j++)
	    b[j] = (a[j] + b[j]) / t;
	}
	
	if (fb->org.org != org->org)		/* color conversion required? 		*/
	{
	  DT_C(dt_color_convert,(ctl,
        &convert, b, &fb->org, 
        fb->xe, fb->ye,
        out, org));
	  data = out;
	}
	else								/* no color conversion? 			*/
	  data = (unsigned long *)b;		/* use raw buffer 					*/

										/* ................................ */
	if (z != NULL)						/* we are going to compress? 		*/
	{
	  uLongf ulongf = zmax;
	  int zrc;
	  zrc = compress(
        (Bytef *)(z+1),					/* output buffer 					*/
        &ulongf,						/* returned size of output 			*/
        (Bytef *)data,					/* source buffer 					*/
        size);							/* how much to compress 			*/
	  if (zrc != Z_OK)
	  {
		rc = dt_err(ctl, F,
          "zlib.compress had error %d",
          zrc);
		goto cleanup;
	  }
	  
	  data = z;
	  *z = ulongf;						/* put size in file too 			*/
	  datasize = ulongf + sizeof(*z);	/* how much to write 				*/

	  if (DT_DBG(ctl, DT_DBG_MASK_COMPLEX))
		dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
          "compressed %ld into %ld",
          size, datasize);
	}
	else								/* not compressing? 				*/
	  datasize = size;					/* will write raw size 				*/

										/* ................................ */
	if (strcmp(pathname, "/dev/null"))	/* not gating off output?		    */
	{
      dtstr_printf(ctl,					/* name of the file 				*/
        filename, sizeof(filename),
        pathname, i);
      DT_C(dtfd_open,(ctl, &fd,			/* open file for output 			*/
        filename, "w"));

	  DT_C(dtfd_write_all,(ctl, &fd,	/* write the entire fb to the file 	*/
        data, datasize));

/*      fsync(fd.fd); */

	  DT_C(dtfd_close,(ctl, &fd));
	  fd.fd = -1;
	  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
        "compressed %5d into %5d (%3d%%)"
        " and wrote %s", 
        size, datasize,
        (100*datasize)/size,
        filename);
	}
	else
	  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
        "compressed %5d into %5d (%3d%%)"
        " for frame %04d",
        size, datasize,
        (100*datasize)/size, i);

    if (delay)
      DT_C(dtos_delay,(ctl, delay));
  }

cleanup:
  if (fd.fd != -1)						/* file was opened? 				*/
    DT_I(dtfd_close,(ctl, &fd));		/* close it 						*/

  if (out != NULL)
	DT_I(dtos_free,(ctl, out));			/* free output buffer 				*/

  if (convert.initialized)
  DT_I(dt_color_convert_uninit,(ctl,
    &convert, &fb->org, org));

  if (a != NULL)
	DT_I(dtos_free,(ctl, a));			/* free buffer 						*/

  if (b != NULL)
	DT_I(dtos_free,(ctl, b));			/* free buffer 						*/

  return rc;
}

/*..........................................................................*/

static
dt_rc_e 
DTCONFIG_API1
dttfb_arg_check(						/* check args and params			*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttfb_arg_check");

  DT_Q(dtt_check_op,(ctl, param,		/* make sure op is understood	    */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................
| NAME
| main program
| 
| END
 *..........................................................................*/

DTT_MAIN(dttfb)
{
  DT_F("dttfb");
  dt_ctl_t *ctl = &param->ctl;
  char fbspec[64];
  dt_enter_arg_t arg[2];
  dt_color_org_t org;
  char message[80];
  int assert_fail = 0;					/* failed assertion count		    */
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DT_GI(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->op, NULL, NULL, "default",
    &param->reps, NULL, NULL, "1",
    &param->output, NULL, NULL, "/tmp/dttfb%03d.out",
    &param->background, NULL, NULL, "64",
    &param->color, NULL, NULL, "luminance",
    &param->sigma, "Frame average", NULL, "1",
    &param->n, NULL, NULL, "1024",
    &param->dt, NULL, NULL, "0",
    NULL));

  DT_GI(dt_enter_arg,(ctl,				/* set up positionals			    */
    arg,
    "Fb spec", NULL,
      fbspec, sizeof(fbspec),
    NULL));
  
  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, usage, param,
    arg, dttfb_arg_check, NULL));
  
  DT_GI(dt_color_org_parse,(ctl,		/* get output color spec bits 		*/
    &org, param->color,
    message, sizeof(message)));
  if (message[0] != '\0')
    rc = dt_err(ctl, F, "%s", message);

  while (rc == DT_RC_GOOD)
  {
	dtos_time_t mark1, mark2;
	dtfb_t fb;
    long reps;
	
	DT_GI(dtos_time,(ctl, &mark1));		/* get time at start 				*/

	DT_GI(dtfb_init,(ctl, &fb,			/* initialize frame buffer 		*/
      fbspec));

    for (reps=0; 
         reps < param->reps && 
         rc == DT_RC_GOOD;
         reps++)
	{
#     define ISOP(OP) (DT_ABBREV(OP, \
        param->op))

      if (ISOP("overlapsave"))
      {
		DT_GI(dttfb_overlapsave,(ctl,	/* save to disk 					*/
          &fb, 
          (int)param->n,
          (int)param->sigma,
          &org,
          (long)param->dt,				/* temporal subsampling			    */
          param->output));
      }
	  else
      if (ISOP("syncsave"))
      {
		DT_GI(dttfb_syncsave,(ctl,		/* save to disk 					*/
          &fb, 
          (int)param->n,
          &org,
          (long)param->dt,				/* temporal subsampling			    */
          param->output));
      }
	  else
      if (ISOP("asyncsave"))
      {
		DT_GI(dttfb_asyncsave,(ctl,		/* save to disk 					*/
          &fb, 
          (int)param->n,
          &org,
          (long)param->dt,				/* temporal subsampling			    */
          param->output));
      }
	  else
      if (ISOP("clear"))
      {
		DT_GI(dtfb_control,(ctl, &fb,	/* freeze memory					*/
          DTFB_CONTROL_FREEZE));
		DT_GI(dtfb_clear,(ctl, &fb,
          (int)param->background));
      }
	  else
      if (ISOP("freeze"))
      {
		DT_GI(dtfb_control,(ctl, &fb,	/* freeze memory					*/
          DTFB_CONTROL_FREEZE));
      }
	  else
      if (ISOP("acquire"))
      {
		DT_GI(dtfb_control,(ctl, &fb,	/* acquire memory					*/
          DTFB_CONTROL_ACQUIRE));
      }
	  else
      if (ISOP("snapshot"))
      {
		DT_GI(dtfb_control,(ctl, &fb,	/* snapshot memory					*/
          DTFB_CONTROL_SNAPSHOT));
      }
	  else
      if (ISOP("reset"))
      {
		DT_GI(dtfb_control,(ctl, &fb,	/* reset card						*/
          DTFB_CONTROL_RESET));
      }
	  else
        rc = dt_err(ctl, F,
          "no case for op \"%s\"",
          param->op);
	}

	DT_G(dtfb_uninit,(ctl, &fb));	
	DT_GI(dtos_time,(ctl, &mark2));		/* get time at end 					*/

	if (DT_DBG(ctl, DT_DBG_MASK_TIME))
	  DT_G(dtt_time_report,(ctl,
        &mark1, &mark2, param->op));

    DT_G(dt_interactive_after,(         /* make post display                */
      ctl, param, usage,
      arg, dttfb_arg_check, NULL, rc));
  }
										/* ................................ */

  DT_I(dt_interactive_leave,(			/* clean up before leaving			*/
    param, rc));

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

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
