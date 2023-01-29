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
| This class of export function packs the arguments together
| into an RPC-like stream and uses dtwinc functions to send them.
| 
| END
 *..........................................................................*/

#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: xchg_win.c,v $ $Revision: 1.1 $");

#include <dtack/lut8.h>
#include <dtack/xchg.h>
#include <dtack/os.h>
#include <dtack/ipc.h>
#include <dtack/mem.h>

#include <dtack/win_r.h>

#include <dtack/winc.h>				

#define DESC "window client"

/*..........................................................................
| two ways to do this:
| 1.	if image is not contiguous, then it would be too slow to output the rows
|		one at a time, so compact the rows and output several together
| 2.	if rows are contiguous, just output them from from their native
| 		buffers, which are presumably fairly much greater than a single row
 *..........................................................................*/

#if DTPRJ_NOWIN == DT_1
#else

static
dt_rc_e
dtxchg_win_export_blocks(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  const char *id,
  dtimage_t *image,
  int x0,
  int y0)
{
  DT_F("dtxchg_win_export_blocks");
  dtwin_r_export_data_arg_t arg;
  dtwin_r_export_data_ret_t *ret = NULL;
  int bpp;
  dtxy_t y;
  dtxy_t i;
  void *data = NULL;
  unsigned int datasize;
  dtxy_t nrows;
  dt_rc_e rc;

  strcpy(arg.name, id);					/* build args structure to pass in	*/
  arg.endian = DTWIN_R_ENDIAN;			/* so server knows our endian-ness 	*/
  arg.x0 = x0;
  arg.xe = image->xe;
  arg.bits = 
    dt_ntype_bits[image->ntype];

  if (arg.bits == 1)
    bpp = 8;
  else
    bpp = 1;

  y = 0;

  if (!(image->flags &					/* image rows are not contiguous? 	*/
        DTIMAGE_FLAG_CONTIG))
  {
	datasize = DT_CAST(unsigned int,
      DT_MIN(512L*512L,					/* pick a decent buffer size 		*/
        DTCONFIG_MALLOC_MAX));
	do {
	  DT_C(dtos_malloc3,(ctl,			/* attempt to get the buffer 		*/
        &data, datasize,
        F, "compaction buffer"));
	  if (data != NULL)					/* got the buffer? 					*/
	    break;
	} while(datasize /= 2 >				/* try again with smaller size 		*/
			image->rowsize * 2);		/* while it's worth it 				*/
  }
  
  if (data != NULL)						/* need to compact rows				*/
  {
	nrows = datasize / image->rowsize;	/* how many rows in block           */ 
    do {								/* loop over blocks 				*/
	  char *d = (char *)data;
	  nrows = DT_MIN(nrows,				/* in case of short block at end 	*/
        image->ye-y);
      for (i=0; i<nrows; i++)			/* rows to fill up the block 		*/
	  {
        void *p;
	    DTIMAGE_GETROW2(DT_C, ctl,		/* map the next row	                */
          image, y+i, &p);
        DT_MEMCPY(d, p, image->rowsize);	/* add row to data block			*/
		DTIMAGE_UNMAPROW2(DT_C, ctl,	/* release the row	                */
          image, y+i);
		d += image->rowsize;

	  }

#ifdef NONO
	  DT_C(dtmem_getpatch,(ctl,			/* compact patch into buffer 		*/
        image->memalloc, data,
        image->x0+0, image->y0+y,
        image->rowsize, nrows));
#endif

      arg.y0 = y + y0;					/* starting output row 				*/
      arg.ye = nrows;
      arg.d.d_val = (char *)data;
      arg.d.d_len = (image->xe/bpp) * 
        arg.ye;

      DT_C(winc->export_data,(ctl,		/* send the block				    */
        winc, &arg, &ret));

      if (ret != NULL &&
		  ret->rc != DT_RC_GOOD)		/* check server's rc			    */
	  {
        rc = dt_err(ctl, F,
          "server error");
		goto cleanup;
	  }

	  if (ret != NULL)
	  DT_C(dtos_free2,(ctl, ret,		/* free reply structure 			*/
        F, "received reply"));
	  ret = NULL;

      y += nrows;
    } while(y < image->ye);
  }
  else									/* use in-place memory source 		*/
  {
    do {								/* loop over blocks 				*/
      void *p;

	  DTIMAGE_GETROWS2(DT_C, ctl,		/* map the next block               */
        image, y,	
        &p, &datasize);
      nrows = datasize /				/* how many rows in block           */ 
        image->rowsize;	
   
      arg.y0 = y + y0;
      arg.ye = DT_MIN(nrows, image->ye-y);
      arg.d.d_val = (char *)p;
      arg.d.d_len = (image->xe/bpp) * arg.ye;

      DT_C(winc->export_data,(ctl,		/* send the block				    */
        winc, &arg, &ret));

      DTIMAGE_UNMAPROW2(DT_C, ctl,		/* don't need this block any more   */
        image, y);
      y += nrows;

      if (ret != NULL &&
		  ret->rc != DT_RC_GOOD)		/* check server's rc			    */
	  {
        rc = dt_err(ctl, F,
          "server error");
		goto cleanup;
	  }

	  if (ret != NULL)
	  DT_C(dtos_free2,(ctl, ret,		/* free reply structure 			*/
        F, "received reply"));
	  ret = NULL;
    } while(y < image->ye);
  }

cleanup:
  if (ret != NULL)
    DT_I(dtos_free2,(ctl, ret,			/* free reply structure 			*/
      F, "received reply"));
  if (data != NULL)
    DT_I(dtos_free2,(ctl, data,
      F, "compaction buffer"));

  return rc;
}
#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_win_export_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  int xpos,
  int ypos)
{
  DT_F("dtxchg_win_export_image");
#if DTPRJ_NOWIN == DT_1
  return dt_err_notlinked(ctl, F, DESC);  
#else
  dtwinc_t winc_block;
  dtwinc_t *winc = &winc_block;
  dtwin_r_create_window_arg_t arg;
  dtwin_r_create_window_ret_t *ret = NULL;
  dt_rc_e rc;
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B08, DT_NTYPE_NULL};

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, id, 
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_Q(dtwinc_init,(ctl, winc,			/* establish window structure 		*/
    server));
  
  strcpy(arg.name, id);					/* build args structure to pass in	*/
  arg.endian = DTWIN_R_ENDIAN;			/* so server knows our endian-ness 	*/
  arg.x0 = xpos;
  arg.y0 = ypos;
  arg.xe = image->xe;
  arg.ye = image->ye;
  arg.bits = 
    dt_ntype_bits[image->ntype];

  DT_C(winc->create_window,(ctl,		/* create the window				*/
    winc, &arg, &ret));

  if (ret != NULL &&
	  ret->rc != DT_RC_GOOD)			/* check server's rc			    */
  {
	rc = dt_err(ctl, F,
      "server error");
	goto cleanup;
  }

  if (ret != NULL)
  DT_C(dtos_free2,(ctl, ret,			/* free reply structure 			*/
    F, "received reply"));
  ret = NULL;

  DT_C(dtxchg_win_export_blocks,(ctl,	/* export data in blocks		    */
    winc, id, image, 0, 0));

cleanup:
  if (ret != NULL)
    DT_I(dtos_free2,(ctl, ret,			/* free reply structure 			*/
      F, "received reply"));

  DT_I(dtwinc_uninit,(ctl, winc));

  return rc;
#endif
}

/*..........................................................................
 *..........................................................................*/
#if DTPRJ_NOWIN == DT_1
#else

static
dt_rc_e
dtxchg_win_import_blocks(
  dt_ctl_t *ctl,
  dtwinc_t *winc,
  const char *id,
  dtimage_t *image)
{
  DT_F("dtxchg_win_import_blocks");
  dtwin_r_import_data_arg_t arg;
  dtwin_r_import_data_ret_t *ret = NULL;
  dtxy_t y;
  dt_rc_e rc;

  strcpy(arg.name, id);					/* build args structure to pass in	*/
  arg.endian = DTWIN_R_ENDIAN;			/* so server knows our endian-ness 	*/
  arg.x0 = 0;
  arg.xe = image->xe;

  y = 0;	
  do {
    void *p;
    unsigned int datasize;
    dtxy_t nrows;

    DTIMAGE_MAPROWS2(DT_C, ctl,			/* map the next block               */
      image, y, &p, &datasize);
    nrows = datasize / image->rowsize;	/* how many rows in block           */
   
    arg.y0 = y;
    arg.ye = DT_MIN(nrows, image->ye-y);

	DT_C(winc->import_data,(ctl,		/* get the block				    */
      winc, &arg, &ret));

    if (ret != NULL &&
	    ret->rc != DT_RC_GOOD)			/* check server's rc			    */
	{
      rc = dt_err(ctl, F,
        "server error");
	  goto cleanup;
	}

	DT_MEMCPY(p, ret->d.d_val,			/* copy data to image space 		*/
      (unsigned int)ret->d.d_len);

	DT_C(dtos_free2,(ctl,				/* free data space 					*/
      ret->d.d_val,
      F, "received reply"));
    ret->d.d_val = NULL;

    if (ret != NULL)
	DT_C(dtos_free2,(ctl, ret,			/* free reply structure 			*/
      F, "received reply"));
    ret = NULL;

    DTIMAGE_PUTROW2(DT_C, ctl,			/* don't need this block any more   */
      image, y);	
    y += nrows;
  } while(y < image->ye);

cleanup:
  if (ret != NULL)
  {
    if (ret->d.d_val != NULL)
	  DT_I(dtos_free2,(ctl,				/* free data space 					*/
        ret->d.d_val,
        F, "received reply"));
    DT_I(dtos_free2,(ctl, ret,			/* free reply structure 			*/
      F, "received reply"));
  }

  return rc;
}
#endif

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_win_import_image(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtimage_t *image,
  unsigned int pagesize,
  unsigned int npages)
{
  DT_F("dtxchg_win_import_image");
#if DTPRJ_NOWIN == DT_1
  return dt_err_notlinked(ctl, F, DESC);  
#else
  dtwinc_t winc_block;
  dtwinc_t *winc = &winc_block;
  dtwin_r_query_arg_t arg;
  dtwin_r_query_ret_t *ret = NULL;
  dt_ntype_e ntype;
  dt_rc_e rc;
  dt_ntype_e ntypes[] =					/* datatypes we can handle			*/
    {DT_NTYPE_B08, DT_NTYPE_NULL};

  DT_Q(dtwinc_init,(ctl, winc,			/* establish window structure 		*/
    server));

  strcpy(arg.name, id);					/* build args structure to pass in	*/
  arg.endian = DTWIN_R_ENDIAN;			/* so server knows our endian-ness 	*/

  DT_C(winc->query,(ctl, winc,			/* query the window					*/
    &arg, &ret));

  if (ret != NULL &&
	  ret->rc != DT_RC_GOOD)			/* check server's rc			    */
  {
	rc = dt_err(ctl, F,
      "server error");
	goto cleanup;
  }

  if (ret->state.xe == 0)				/* window does not exist? 			*/
  {
    rc = dt_err(ctl, F,
      "Sorry, window \"%s\""
      " does not exist",
      arg.name);
	goto cleanup;
  }

  if (ret->state.bits == 8)
    ntype = DT_NTYPE_B08;
  else
  {
    rc = dt_err(ctl, F,
      "unsupported bits-per-pixel %d"
      " in window \"%s\" on ipc server \"%s\"",
      (int)ret->state.bits, id, 
      winc->server);
	goto cleanup;
  }

  DT_C(dtimage_create3,(ctl, image,		/* get space for the image		    */
    pagesize, npages,
    ret->state.xe, ret->state.ye,
    ntype, NULL));

  if (ret != NULL)
  DT_C(dtos_free2,(ctl, ret,			/* free reply structure 			*/
    F, "received reply"));
  ret = NULL;

  rc = dtimage_check_1f(ctl,			/* check image validity			    */
    image, id, 
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    goto cleanup;

  DT_C(dtxchg_win_import_blocks,(ctl,	/* import data in blocks		    */
    winc, id, image));

cleanup:
  if (ret != NULL)
    DT_I(dtos_free2,(ctl, ret,			/* free reply structure 			*/
      F, "received reply"));

  DT_I(dtwinc_uninit,(ctl, winc));

  return rc;
#endif
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_win_export_lut8(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  const dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtxchg_win_export_lut8");
#if DTPRJ_NOWIN == DT_1
  return dt_err_notlinked(ctl, F, DESC);  
#else
  dtwinc_t winc_block;
  dtwinc_t *winc = &winc_block;
  dtwin_r_export_color_arg_t arg;
  dtwin_r_export_color_ret_t *ret = NULL;
  unsigned char tmp[1024];
  dt_rc_e rc;

  if (start < 0)
    return dt_err(ctl, F,
      "invalid color start %d",
      start);

  if (start+count > 256)
    return dt_err(ctl, F,
      "invalid color count %d for start %d",
      count, start);

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "called with server \"%s\"",
    server? server: "NULL");

  DT_Q(dtwinc_init,(ctl, winc,			/* establish window structure 		*/
    server));

  strcpy(arg.name, id);					/* build args structure to pass in	*/
  arg.endian = DTWIN_R_ENDIAN;			/* so server knows our endian-ness 	*/
  arg.start = start;
  arg.count = count;
  arg.d.d_len = count * 4;
  arg.d.d_val = (char *)tmp;
  
  {										/* reformat lut data				*/
	unsigned char *r = tmp;
	unsigned char *g = r + count;
	unsigned char *b = g + count;
	unsigned char *i = b + count;
	int j;
    for (j=0; j<count; j++)
    {
	  r[j] = lut[j].r; 
	  g[j] = lut[j].g;
	  b[j] = lut[j].b;
	  i[j] = lut[j].i;
	}
  }

  DT_C(winc->export_color,(ctl,			/* export the lut					*/
    winc, &arg, &ret));
  
  if (ret != NULL &&
	  ret->rc != DT_RC_GOOD)			/* check server's rc			    */
    rc = dt_err(ctl, F,
      "server error");

cleanup:
  if (ret != NULL)
    DT_I(dtos_free2,(ctl, ret,			/* free reply structure 			*/
      F, "received reply"));

  DT_I(dtwinc_uninit,(ctl, winc));

  return rc;
#endif
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_win_import_lut8(
  dt_ctl_t *ctl,
  const char *server,
  const char *id,
  dtlut8_t *lut,
  int start,
  int count)
{
  DT_F("dtxchg_win_import_lut8");
#if DTPRJ_NOWIN == DT_1
  return dt_err_notlinked(ctl, F, DESC);  
#else
  dtwinc_t winc_block;
  dtwinc_t *winc = &winc_block;
  dtwin_r_import_color_arg_t arg;
  dtwin_r_import_color_ret_t *ret = NULL;
  dt_rc_e rc;

  if (start < 0)
    return dt_err(ctl, F,
      "invalid color start %d",
      start);

  if (start+count > 256)
    return dt_err(ctl, F,
      "invalid color count %d for start %d",
      count, start);

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "called with server \"%s\"",
    server? server: "NULL");

  DT_Q(dtwinc_init,(ctl, winc,			/* establish window structure 		*/
    server));

  strcpy(arg.name, id);					/* build args structure to pass in	*/
  arg.endian = DTWIN_R_ENDIAN;			/* so server knows our endian-ness 	*/
  arg.start = start;
  arg.count = count;

  DT_C(winc->import_color,(ctl,			/* get the lut			    		*/
    winc, &arg, &ret));
  
  if (ret != NULL &&
	  ret->rc != DT_RC_GOOD)			/* check server's rc			    */
  {
    rc = dt_err(ctl, F,
      "server error");
	goto cleanup;
  }
  
  {										/* put data in caller's space 		*/
	unsigned char *r = 
      (unsigned char *)ret->d.d_val;
	unsigned char *g = r + count;
	unsigned char *b = g + count;
	unsigned char *i = b + count;
	int j;
    for (j=0; j<count; j++)
    {
	  lut[j].r = r[j];
	  lut[j].g = g[j];
	  lut[j].b = b[j];
	  lut[j].i = i[j];
	}
  }

cleanup:
  if (ret != NULL)
  {
    if (ret->d.d_val != NULL)
	  DT_I(dtos_free2,(ctl,				/* free data space 					*/
        ret->d.d_val,
        F, "received reply"));
    DT_I(dtos_free2,(ctl, ret,			/* free reply structure 			*/
      F, "received reply"));
  }

  DT_I(dtwinc_uninit,(ctl, winc));

  return rc;
#endif
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtxchg_win_stop(
  dt_ctl_t *ctl,
  const char *server)
{
  DT_F("dtxchg_win_stop");
#if DTPRJ_NOWIN == DT_1
  return dt_err_notlinked(ctl, F, DESC);  
#else
  dtwinc_t winc_block;
  dtwinc_t *winc = &winc_block;
  dtwin_r_query_arg_t arg;
  dtwin_r_query_ret_t *ret = NULL;
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_IPC,
    "called with server \"%s\"",
    server? server: "NULL");

  DT_Q(dtwinc_init,(ctl, winc,			/* establish window structure 		*/
    server));

  ((long *)arg.name)[0] = 0;			/* notify that a command follows 	*/
  strcpy(arg.name+sizeof(long), 		/* encode the command itself		*/
	DTWIN_R_QUERY_STOP);
  arg.endian = DTWIN_R_ENDIAN;			/* so server knows our endian-ness 	*/

  DT_C(winc->query,(ctl,				/* send the query					*/
    winc, &arg, &ret));
  
  if (ret != NULL &&
	  ret->rc != DT_RC_GOOD)			/* check server's rc			    */
  {
    rc = dt_err(ctl, F,
      "server error");
	goto cleanup;
  }

cleanup:
  if (ret != NULL)
    DT_I(dtos_free2,(ctl, ret,			/* free reply structure 			*/
      F, "received reply"));

  DT_I(dtwinc_uninit,(ctl, winc));

  return rc;
#endif
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
