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
#include <dtack/prj.h>

DT_RCSID("test $RCSfile: dttmem.c,v $ $Revision: 1.12 $");

#include <dtack/mem.h>

#define USAGE "[-Dnnnn] [-op all|disk|ram]"

static const char *ops[] = {
  "default", 
  "parse", "config", 
  "ram", "ems", 
  "xmsif", 
  "xmslb", 
  "txms",
  "xmsif-time", 
  "xmslb-time", 
  "txms-time",
  "prealloc", "disk", "time", NULL};

#define NOPREALLOC NULL, 0, 0

dt_rc_e
dttmem_parse(
  dt_ctl_t *ctl,
  int *assert_fail);
dt_rc_e
dttmem_config(
  dt_ctl_t *ctl,
  const char *tmpdir,			  
  int *assert_fail);

typedef
dt_rc_e 
DTCONFIG_API1
dttmem_init_f(
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  const char *name,
  long maxusage,
  unsigned int pagesize,
  unsigned int npages);

/*..........................................................................
 * set patnum in ram
 *..........................................................................*/

static
void
set_pattern(
  dt_ctl_t *ctl,
  int patnum,
  char *data,
  int size)
{
  int i;
  switch(patnum)
  {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
	{
	  char c = patnum * 0x10 + patnum;
      for (i=0; i<size; i++)
        data[i] = c;
	}
    break;
  }
}

/*..........................................................................
 * verify patnum is in ram
 *..........................................................................*/

static
void
ver_pattern(
  dt_ctl_t *ctl,
  int patnum,
  char *data,
  int size,
  char *id,
  int *assert_fail)
{
  int i;
  switch(patnum)
  {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
	{
	  char c = patnum * 0x10 + patnum;
      for (i=0; i<size; i++)
        if (data[i] != c)
          break;
	}
    break;
  }
  if (i < size)
  {
    dt_assert(ctl, 0, id, "pattern %d verify failed at %d of %d",
	  patnum, i, size);
    *assert_fail += 1;
  }
  else
    dt_assert(ctl, 1, id, "pattern %d verified all of %d",
	  patnum, size);
}

/*..........................................................................
 * pattern overwrite
 *..........................................................................*/

dt_rc_e
dttmem_twobuf(
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  long size,
  char *id,
  int *assert_fail)
{
  DT_F("dttmem_twobuf");
  char *data;
  int size1 = 79;						/* fairly small chunks			    */
  int size2 = 87;
  long offset1 = 0;
  long offset2 = size - size2;

  DT_Q((memalloc->map),(ctl, memalloc,	/* map the first piece	 			*/
    offset1, (void **)&data, NULL));
  set_pattern(ctl, 1, data, size1);		/* set pattern 1 in first piece	    */
  DT_Q((memalloc->put),(ctl, memalloc,	/* put the first piece back		    */
    offset1));

  DT_Q((memalloc->map),(ctl, memalloc,	/* map a second piece				*/
    offset2, (void **)&data, NULL));
  set_pattern(ctl, 2, data, size2);		/* set pattern 2 in second piece    */
  DT_Q((memalloc->put),(ctl, memalloc,	/* put the second piece back	    */
    offset2));

  DT_Q((memalloc->get),(ctl, memalloc,	/* get the first piece again		*/
    offset1, (void **)&data, NULL));
  ver_pattern(ctl, 1, data, size1,		/* verify pattern 1 in first piece	*/
    id, assert_fail);

  DT_Q((memalloc->get),(ctl, memalloc,	/* get the second piece again		*/
    offset2, (void **)&data, NULL));
  ver_pattern(ctl, 2, data, size2,		/* verify pattern 2 in second piece	*/
    id, assert_fail);

  return DT_RC_GOOD;
}

/*..........................................................................
 * force paging
 *..........................................................................*/

dt_rc_e
dttmem_eightbuf(
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  unsigned int size,
  unsigned int units,
  char *id,
  int *assert_fail)
{
  DT_F("dttmem_eightbuf");
  char *data;
  unsigned unit;
  double total = (double)size * (double)units;
  long offset;
  int i;
  char subid[128];
# define NEIGHTBUF 8
  static double unit_loc[NEIGHTBUF] = {	/* random access				    */
    6.0/(double)NEIGHTBUF,
    2.0/(double)NEIGHTBUF,
    0.0/(double)NEIGHTBUF,
    5.0/(double)NEIGHTBUF,
    3.0/(double)NEIGHTBUF,
    1.0/(double)NEIGHTBUF,
    4.0/(double)NEIGHTBUF,
    7.0/(double)NEIGHTBUF
  };

  for (i=0; i<NEIGHTBUF; i++)
  {
    unit = (unsigned)
      (unit_loc[i] * total / size);		/* unit we will test on			    */
    offset = (long)((double)unit *		/* offset of that unit			    */
      (double)size);
    DT_Q((memalloc->map),(ctl,			/* map a piece		 			    */
      memalloc, offset, 
      (void **)&data, NULL));
	set_pattern(ctl, i+1, data, size);	/* set pattern in piece				*/
    DT_Q((memalloc->put),(ctl,			/* put the piece back			    */
      memalloc, offset));
  }

  for (i=0; i<NEIGHTBUF; i++)
  {
    unit = (unsigned)
      (unit_loc[i] * total / size);		/* unit we will test on			    */
    offset = (long)((double)unit *		/* offset of that unit			    */
      (double)size);
    DT_Q((memalloc->get),(ctl,			/* map the piece again				*/
      memalloc, offset, 
      (void **)&data, NULL));
    sprintf(subid, "%s unit %4u",
      id, unit);
    ver_pattern(ctl, i+1, data, size,	/* verify pattern 1 in the piece	*/
      subid, assert_fail);
    DT_Q((memalloc->unmap),(ctl,		/* unmap the piece					*/
      memalloc, offset));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

#define PSIZE (8)

static char patch[PSIZE][PSIZE] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 2, 2, 2, 2, 2, 2, 0},
  {0, 2, 2, 2, 2, 2, 2, 0},
  {0, 2, 2, 2, 2, 2, 2, 0},
  {0, 2, 2, 2, 2, 2, 2, 0},
  {0, 2, 2, 2, 2, 2, 2, 0},
  {0, 2, 2, 2, 2, 2, 2, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}
};

#define PATCH(I, J) patch[I][J]

dt_rc_e
dttmem_patch_inside(
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  unsigned int x0,
  unsigned int y0,
  unsigned long tripwire,
  char *id,
  int *assert_fail)
{
  DT_F("dttmem_patch_inside");
  unsigned int i, j;
  char data[PSIZE][PSIZE];
  int err = 0;

  ctl->tripwire.mem = 0;

  DT_Q(dtmem_clear,(ctl, memalloc));	/* clear memory						*/
  DT_MEMSET(data, 2, PSIZE*PSIZE);			/* make patch of 2's				*/
  DT_Q(dtmem_putpatch,(ctl, memalloc,	/* put small patch of 2's			*/
    data, x0, y0, PSIZE-2, PSIZE-2));

  DT_MEMSET(data, 0xff, PSIZE*PSIZE);		/* set data to nonoccurring ff's	*/
  DT_Q(dtmem_getpatch,(ctl, memalloc,	/* get a bigger patch				*/
    data, x0-1, y0-1, PSIZE, PSIZE));

  for (i=0; i<PSIZE && !err; i++)
  {
    for (j=0; j<PSIZE && !err; j++)
    {
      if (PATCH(i, j) != data[i][j])
      {
        dt_assert(ctl, 0, F,
          "%s data[%d][%d] = %d but expected %d",
          id, i, j, data[i][j], PATCH(i, j));
        *assert_fail += 1;
        err = 1;
      }
    }
  }

  if (!err)
# ifdef DTCONFIG_TRIPWIRE				/* supposed to be doing tripwiring?	*/
  {
    *assert_fail += DT_RC_GOOD !=
      dt_assert(ctl,
        ctl->tripwire.mem == tripwire,
        F, "%s tripwire 0x%08lx (0x%08lx)",
        id, ctl->tripwire.mem, tripwire);
  }
# else
    dt_assert(ctl, 1, F,
      "%s OK", id);
# endif

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttmem_patchbuf(
  dt_ctl_t *ctl,
  dtmemalloc_t *memalloc,
  unsigned int size,
  unsigned int units,
  unsigned long tripwire,
  char *id,
  int *assert_fail)
{
  DT_F("dttmem_patchbuf");
  char subid[128];

  sprintf(subid, "%s upper left", id);
  DT_Q(dttmem_patch_inside,(ctl, memalloc,
    1, 1, 
    tripwire, subid, assert_fail));

  sprintf(subid, "%s lower right", id);
  DT_Q(dttmem_patch_inside,(ctl, memalloc,
    size-(PSIZE-1), units-(PSIZE-1),
    tripwire, subid, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * simple test of memory, independent of type
 *..........................................................................*/

dt_rc_e
dttmem_simple(
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  void **pages,
  unsigned int pagesize,
  unsigned int npages,
  unsigned int size,
  unsigned int units,
  char *id,
  int *assert_fail)
{
  DT_F("dttmem_simple");
  dtmemalloc_t *memalloc;
 
  if (pages)							/* preallocated?				    */
  {
    DT_Q(dtmemram_prealloc,(ctl,		/* pre-allocate some space		    */
      memsource,
      (void **)&memalloc, 
      size, units,
      pages, pagesize, npages,
      F, "only alloc"));
  }
  else
  {
    DT_Q((memsource->alloc),(ctl,		/* allocate some new space		    */
      memsource,
      (void **)&memalloc, 
      0, 0,
      size, units,
      F, "only alloc"));

    if (memalloc == NULL)
      return dt_err(ctl, F,
        "no space in memsource %s for %u units of %d bytes",
        memsource->name, units, size);

    memsource->nextalloc++;				/* sequence number					*/
  }

  DT_Q(dttmem_twobuf,(ctl, memalloc,	/* verify read/write in space	    */
    (long)size*(long)units,
    id, assert_fail));

  DT_Q((memalloc->free),(ctl,			/* free the allocation			    */
    memalloc,
    F, "only alloc"));

  return DT_RC_GOOD;
}

/*..........................................................................
 * test copying of memory, independent of type
 *..........................................................................*/

dt_rc_e
dttmem_copy(
  dt_ctl_t *ctl,
  const char *configure,
  unsigned int pagesize,
  unsigned int npages,
  unsigned int size,
  unsigned int units,
  char *id,
  int *assert_fail)
{
  DT_F("dttmem_simple");
  void *ctl_memsource;
  dtmemalloc_t *memalloc1 = NULL;
  dtmemalloc_t *memalloc2 = NULL;
  long total = (long)size * (long)units;
  long cmp;
  dt_rc_e rc;
 
  ctl_memsource = ctl->memsource;		/* take over control's memsource	*/
  DT_C(dtmem_configure,(ctl,			/* set up internal memsources	    */
    configure));
  DT_C(dtmem_alloc,(ctl,				/* make first allocation		    */
    (void **)&memalloc1,
    pagesize, npages,
    size, units, 
    F, "first alloc"));
  DT_C(dtmem_alloc,(ctl,				/* make second allocation		    */
    (void **)&memalloc2,
    pagesize/2, npages*2,
    size/2, units*2,
    F, "second alloc"));
  DT_C(dtmem_set,(ctl, memalloc1, 1));	/* set first memory to 1's		    */
  DT_C(dtmem_set,(ctl, memalloc2, 2));	/* set second memory to 2's			*/
  DT_C(dtmem_copy,(ctl, memalloc1,		/* copy first memory to second	    */
    memalloc2));
  DT_C(dtmem_cmp,(ctl, memalloc1,		/* compare memories				    */
    memalloc2, 0, &cmp));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, cmp == 0, F,
      "%s simple copy %ld (%ld)", 
      id, cmp, 0L);

  DT_C(dtmem_realloc,(ctl,				/* increase first allocation	    */
    (void *)memalloc1,
    pagesize, npages,
    size, units+1, 
    F, "first increase realloc"));
  DT_C(dtmem_set2,(ctl, memalloc1,		/* set new part to 3's				*/
    total, size, 3));
  DT_C(dtmem_cmp,(ctl, memalloc1,		/* compare memories				    */
    memalloc2, 0, &cmp));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, cmp == total+1, F,
      "%s realloc 1 only %ld (%ld)",
      id, cmp, total+1);

  DT_C(dtmem_realloc,(ctl,				/* increase second allocation	    */
    (void *)memalloc2,
    pagesize, npages,
    size, units+1, 
    F, "second increase realloc"));
  DT_C(dtmem_set2,(ctl, memalloc2,		/* set new part to 3's				*/
    total, size, 3));
  DT_C(dtmem_cmp,(ctl, memalloc1,		/* compare memories				    */
    memalloc2, 0, &cmp));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, cmp == 0, F,
      "%s realloc both %ld (%ld)", 
      id, cmp, 0L);

  DT_C(dtmem_set2,(ctl, memalloc1,		/* set last byte in memory to 5	    */
    total+size-1, 1, 5));
  DT_C(dtmem_cmp,(ctl, memalloc1,		/* compare memories				    */
    memalloc2, 0, &cmp));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, cmp == (long)(total+size), F,
      "%s last byte 5 %ld (%ld)", 
      id, cmp, total+size);

  DT_C(dtmem_copy,(ctl, memalloc1,		/* copy first memory to second	    */
    memalloc2));
  DT_C(dtmem_cmp,(ctl, memalloc1,		/* compare memories				    */
    memalloc2, 0, &cmp));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, cmp == 0, F,
      "%s realloced copy %ld (%ld)",
      id, cmp, 0);

  DT_C(dtmem_set2,(ctl, memalloc1,		/* set mid byte in memory to 6	    */
    units*size/2-1, 1, 6));
  DT_C(dtmem_set2,(ctl, memalloc2,		/* set mid byte in memory to 7	    */
    units*size/2-1, 1, 7));

  DT_C(dtmem_realloc,(ctl,				/* decrease first allocation	    */
    (void *)memalloc1,
    0, 0, size/2, units,
    F, "first decrease realloc"));
  DT_C(dtmem_copy,(ctl, memalloc2,		/* copy second memory to first	    */
    memalloc1));
  DT_C(dtmem_realloc,(ctl,				/* decrease second allocation	    */
    (void *)memalloc2,
    0, 0, size/2, units,
    F, "second decrease realloc"));
  DT_C(dtmem_cmp,(ctl, memalloc1,		/* compare memories				    */
    memalloc2, 0, &cmp));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, cmp == 0, F,
      "%s realloc both %ld (%ld)",
      id, cmp, units*size/2);

cleanup:
  if (memalloc2)
    DT_I(dtmem_free,(ctl, memalloc2,
      F, "second alloc"));
  if (memalloc1)
    DT_I(dtmem_free,(ctl, memalloc1,
      F, "first alloc"));
  DT_I(dtmem_unconfigure,(ctl));		/* set up internal memsources	    */
  ctl->memsource = ctl_memsource;		/* restore control's memsource		*/
  return rc;
}

/*..........................................................................
 * test memory file reading and writing, independent of type
 *..........................................................................*/

dt_rc_e
dttmem_io(
  dt_ctl_t *ctl,
  const char *configure,
  unsigned int pagesize,
  unsigned int npages,
  unsigned int size,
  unsigned int units,
  char *id,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttmem_simple");
  void *ctl_memsource;
  dtmemalloc_t *memalloc1 = NULL;
  dtmemalloc_t *memalloc2 = NULL;
  char filename[128];
  dtfd_t fd;
  long total = (long)size * (long)units;
  long half = total / 2;
  long quarter = total / 4;
  long got;
  int eof;
  long cmp;
  dt_rc_e rc;

  DT_Q(dtstr_printf,(ctl,				/* make temp filename		    	*/
    filename, sizeof(filename),
    "%s/dttmem.tmp", tmpdir));

  ctl_memsource = ctl->memsource;		/* take over control's memsource	*/
  DT_C(dtmem_configure,(ctl,			/* set up internal memsources	    */
    configure));
  DT_C(dtmem_alloc,(ctl,				/* make first allocation		    */
    (void **)&memalloc1,
    pagesize, npages,
    size, units, 
    F, "first alloc"));
  DT_C(dtmem_alloc,(ctl,				/* make second allocation		    */
    (void **)&memalloc2,
    pagesize/2, npages*2,
    size/2, units*2,
    F, "second alloc"));

  DT_C(dtmem_set,(ctl, memalloc1, 1));	/* set first memory to 1's		    */
  DT_C(dtmem_set,(ctl, memalloc2, 2));	/* set second memory to 2's			*/

  DT_C(dtfd_open,(ctl, &fd, 
    filename, "w"));
  DT_C(dtmem_write,(ctl, memalloc1,		/* write entire first memory		*/
    0, 0, &fd));
  DT_C(dtfd_close,(ctl, &fd));

  DT_C(dtfd_open,(ctl, &fd, 
    filename, "r"));
  DT_C(dtmem_read,(ctl, memalloc2,		/* read entire second memory		*/
    0, 0, &fd, NULL, NULL));
  DT_C(dtfd_close,(ctl, &fd));

  DT_C(dtmem_cmp,(ctl, memalloc1,		/* compare memories				    */
    memalloc2, 0, &cmp));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, cmp == 0, F,
      "%s simple I/O %ld (%ld)", 
      id, cmp, 0L);

  DT_C(dtmem_set2,(ctl, memalloc1,		/* set 2nd half of 1st mem to 3's  	*/
    half, 0, 3));
  DT_C(dtfd_open,(ctl, &fd, 
    filename, "w"));
  DT_C(dtmem_write,(ctl, memalloc1,		/* write 2nd half of 1st memory		*/
    half, 0, &fd));
  DT_C(dtmem_write,(ctl, memalloc1,		/* write 2nd quarter of 1st memory	*/
    quarter, quarter, &fd));
  DT_C(dtfd_close,(ctl, &fd));

  DT_C(dtfd_open,(ctl, &fd, 
    filename, "r"));
  DT_C(dtmem_read,(ctl, memalloc2,		/* read 2nd half of 2nd memory		*/
    half, total-half, &fd, &got, &eof));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, 
      got == total-half && eof == 0, F,
      "%s partial half got %ld (%ld), eof %d (%d)", 
      id, got, total-half, eof, 0);
  DT_C(dtmem_read,(ctl, memalloc2,		/* read 2nd quarter of 2nd memory	*/
    quarter, 0, &fd,
    &got, &eof));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, 
      got == quarter && eof == 1, F,
      "%s partial quarter got %ld (%ld), eof %d (%d)", 
      id, got, quarter, eof, 1);
  DT_C(dtfd_close,(ctl, &fd));

  DT_C(dtmem_cmp,(ctl, memalloc1,		/* compare memories				    */
    memalloc2, 0, &cmp));
  *assert_fail += DT_RC_GOOD !=
    dt_assert(ctl, cmp == 0, F,
      "%s partial I/O %ld (%ld)", 
      id, cmp, 0L);


cleanup:
  if (memalloc2)
    DT_I(dtmem_free,(ctl, memalloc2,
      F, "second alloc"));
  if (memalloc1)
    DT_I(dtmem_free,(ctl, memalloc1,
      F, "first alloc"));
  {
    int do_unlink = 0;
    if (fd.filename)
      do_unlink = 1;
    if (fd.file)
      DT_I(dtfd_close,(ctl, &fd));
    if (do_unlink)
      DT_I(dtos_unlink,(ctl, 
        filename));
  }
  DT_I(dtmem_unconfigure,(ctl));		/* set up internal memsources	    */
  ctl->memsource = ctl_memsource;		/* restore control's memsource		*/
  return rc;
}

/*..........................................................................
 * timing test of memory, single buffer, independent of type
 *..........................................................................*/

dt_rc_e
dttmem_time(
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  void **pages,
  unsigned int pagesize,
  unsigned int npages,
  unsigned int size,
  unsigned int units,
  long loops,
  char *id,
  int *assert_fail)
{
  DT_F("dttmem_time");
  dtmemalloc_t *memalloc = NULL;
  const long total = 
    (long)size * (long)units;
  long loop;
  dtos_time_t mark1, mark2;
  dt_rc_e rc;
 
  DT_C((memsource->alloc),(ctl,			/* allocate some new space		    */
    memsource,
    (void **)&memalloc, 
    0, 0,
    size, units,
    F, "only alloc"));

  if (memalloc == NULL)
    return dt_err(ctl, F,
      "no space in memsource %s for %u units of %d bytes",
      memsource->name, units, size);

  DT_C(dtos_time,(ctl, &mark1));		/* start timing					    */
  for (loop=0; loop<loops; loop++)		/* loop for timing				    */
  {
    long offset = 0;
    void *ramptr;
    unsigned int datasize;
    do {
      DT_C((memalloc->get),(ctl,		/* read next block into memory	    */
        memalloc, offset,
        &ramptr, &datasize));
      DT_C((memalloc->put),(ctl,		/* write the block back into memory	*/
        memalloc, offset));
      offset += (long)datasize;			/* on to next block				    */
	} while(offset < total);
  }
  DT_C(dtos_time,(ctl, &mark2));
  DT_C(dtt_time_report,(ctl,			/* report time					    */
    &mark1, &mark2, id));

cleanup:
  if (memalloc)
    DT_I((memalloc->free),(ctl,			/* free the allocation			    */
      memalloc,
      F, "only alloc"));

  return rc;
}

/*..........................................................................
 * test of memory patching, independent of type
 *..........................................................................*/

dt_rc_e
dttmem_patch(
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  void **pages,
  unsigned int pagesize,
  unsigned int npages,
  unsigned int size,
  unsigned int units,
  unsigned long tripwire,
  char *id,
  int *assert_fail)
{
  DT_F("dttmem_patch");
  dtmemalloc_t *memalloc;
  
  if (pages)							/* preallocated?				    */
  {
    DT_Q(dtmemram_prealloc,(ctl,		/* pre-allocate some space		    */
      memsource,
      (void **)&memalloc, 
      size, units,
      pages, pagesize, npages,
      F, "only alloc"));
  }
  else
  {
    DT_Q((memsource->alloc),(ctl,		/* allocate some new space		    */
      memsource,
      (void **)&memalloc, 
      0, 0,							
      size, units,
      F, "only alloc"));

    if (memalloc == NULL)
      return dt_err(ctl, F,
        "no space in memsource %s for %u units of %d bytes",
        memsource->name, units, size);

    memsource->nextalloc++;				/* sequence number					*/
  }

  DT_Q(dttmem_patchbuf,(ctl, memalloc,	/* verify read/write in space	    */
    size, units, tripwire,
    id, assert_fail));

  DT_Q((memalloc->free),(ctl,			/* free the allocation			    */
    memalloc,
    F, "only alloc"));

  return DT_RC_GOOD;
}


/*..........................................................................
 * simple test of paging memory, independent of type
 *..........................................................................*/

dt_rc_e
dttmem_paging(
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  char *id,
  int *assert_fail)
{
  DT_F("dttmem_paging");
  dtmemalloc_t *memalloc;
  unsigned int size = 400;
  unsigned int units = 400;

  DT_Q((memsource->alloc),(ctl,			/* allocate some space			    */
    memsource,
    (void **)&memalloc, 
    0, 0,
    size, units,						/* room for at least one big image  */
    F, "only alloc"));

  if (memalloc == NULL)
    return dt_err(ctl, F,
      "no space in memsource %s for %u units of %d bytes",
      memsource->name, units, size);

  memsource->nextalloc++;				/* sequence number					*/


  DT_Q(dttmem_eightbuf,(ctl, memalloc,	/* verify paging					*/
    size, units, id, assert_fail));

  DT_Q((memalloc->free),(ctl,			/* free the allocation			    */
    memalloc, F, "only alloc"));

  return DT_RC_GOOD;
}

/*..........................................................................
 * test multiple allocations of memory, independent of type
 *..........................................................................*/

dt_rc_e
dttmem_multiple(
  dt_ctl_t *ctl,
  dtmemsource_t *memsource,
  void **pages,
  unsigned int pagesize,
  unsigned int npages,
  char *id,
  int *assert_fail)
{
  DT_F("dttmem_multiple");
# define NMULTIPLE 10
  dtmemalloc_t *memalloc[NMULTIPLE];
  char subid[128];
  int i;

  for (i=0; i<NMULTIPLE; i++)
  {
    sprintf(subid, "%s %2d", id, i);
    if (pages)							/* preallocated memory?			    */
	{
      DT_Q(dtmemram_prealloc,(ctl,		/* set up to use preallocation		*/
        memsource,
        (void **)&memalloc[i],
        100, 100, 
        pages+i*npages,
        pagesize, npages,
        F, subid));
	}
    else
    {
      DT_Q((memsource->alloc),(ctl,		/* allocate some new space		    */
        memsource,
        (void **)&memalloc[i], 
        0, 0,
        100, 100, 
        F, subid));

      if (memalloc[i] == NULL)
	  {
        int n = i;
        for (i=0; i<n; i++)
        {
          sprintf(subid, "%s %2d", id, i);
          DT_Q((memalloc[i]->free),(ctl,
            memalloc[i], F, subid));
        }
        return dt_err(ctl, F,
          "no space in memsource %s for %u units of %d bytes",
          memsource->name, 100, 100);
	  }

      memsource->nextalloc++;			/* sequence number					*/
    }
  }

  for (i=0; i<NMULTIPLE; i++)
  {
    sprintf(subid, "%s %2d", id, i);
    DT_Q(dttmem_twobuf,(ctl,			/* verify read/write in space	    */
      memalloc[i],
      100*100, subid, assert_fail));
  }

  for (i=0; i<NMULTIPLE; i++)
  {
    sprintf(subid, "%s %2d", id, i);
    DT_Q((memalloc[i]->free),(ctl,		/* free the allocation			    */
      memalloc[i], F, subid));
  }

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of disk memory and simple test of it
 *..........................................................................*/

void
disk_name(
  char *name,
  int max,
  const char *tmpdir)
{
  dtstr_printf(NULL, name, max, "%s/dttmem%%02d.mem", tmpdir);
}

/*..........................................................................
 * explicit allocation of disk memory and simple test of it
 *..........................................................................*/

dt_rc_e
dttmem_disk_simple(
  dt_ctl_t *ctl,
  const char *tmpdir,				   
  int *assert_fail)
{
  DT_F("dttmem_disk_simple");
  dtmemsource_t memsource;
  char name[128];
  disk_name(name, sizeof(name), tmpdir);

  DT_Q(dtmemdisk_init,(ctl,				/* init the memory source		    */
    &memsource, name,
    100, 0, 0));
  
  DT_Q(dttmem_simple,(ctl, &memsource,	/* run simple test on memory	    */
    NOPREALLOC, 500, 1200,
    F, assert_fail));

  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of disk memory and simple test of it
 *..........................................................................*/

dt_rc_e
dttmem_disk_copy(
  dt_ctl_t *ctl,
  const char *tmpdir,				   
  int *assert_fail)
{
  DT_F("dttmem_disk_copy");
  char id[128];
  char name[128];
  disk_name(name, sizeof(name), tmpdir);

  sprintf(id, "%s unit page", F);
  DT_Q(dttmem_copy,(ctl, name,
    100, 1, 100, 100,
    id, assert_fail));

  sprintf(id, "%s multi page", F);
  DT_Q(dttmem_copy,(ctl, name,
    100, 1, 10, 100,
    id, assert_fail));

  sprintf(id, "%s single page", F);
  DT_Q(dttmem_copy,(ctl, name,
    110, 1, 10, 10,
    id, assert_fail));

  sprintf(id, "%s excess page", F);
  DT_Q(dttmem_copy,(ctl, name,
    1000, 1, 2, 10,
    id, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of disk memory and simple I/O of it
 *..........................................................................*/

dt_rc_e
dttmem_disk_io(
  dt_ctl_t *ctl,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttmem_disk_io");
  char id[128];
  char name[128];
  disk_name(name, sizeof(name), tmpdir);

  sprintf(id, "%s unit page", F);
  DT_Q(dttmem_io,(ctl, name,
    100, 1, 100, 100,
    id, tmpdir, assert_fail));

  sprintf(id, "%s multi page", F);
  DT_Q(dttmem_io,(ctl, name,
    100, 1, 10, 100,
    id, tmpdir, assert_fail));

  sprintf(id, "%s single page", F);
  DT_Q(dttmem_io,(ctl, name,
    110, 1, 10, 10,
    id, tmpdir, assert_fail));

  sprintf(id, "%s excess page", F);
  DT_Q(dttmem_io,(ctl, name,
    1000, 1, 2, 10,
    id, tmpdir, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of disk memory and paging test of it
 *..........................................................................*/

dt_rc_e
dttmem_disk_paging(
  dt_ctl_t *ctl,
  const char *tmpdir,				   
  int *assert_fail)
{
  DT_F("dttmem_disk_paging");
  dtmemsource_t memsource;
  char name[128];
  disk_name(name, sizeof(name), tmpdir);
  DT_Q(dtmemdisk_init,(ctl,			/* init the memory source		    */
    &memsource, name,
    0, 0, 0));
  
  DT_Q(dttmem_paging,(ctl, &memsource,	/* run paging test on memory	    */
    F, assert_fail));

  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of multiple disk allocations and simple tests of them
 *..........................................................................*/

dt_rc_e
dttmem_disk_multiple(
  dt_ctl_t *ctl,
  const char *tmpdir,				   
  int *assert_fail)
{
  DT_F("dttmem_disk_multiple");
  dtmemsource_t memsource;
  char name[128];
  disk_name(name, sizeof(name), tmpdir);

  DT_Q(dtmemdisk_init,(ctl,			/* init the memory source		    */
    &memsource, name,
    100, 0, 0));
  
  DT_Q(dttmem_multiple,(ctl, 			/* run multiple tests on memory	    */
    &memsource, NOPREALLOC, F, assert_fail));

  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of ram memory and timing tests
 *..........................................................................*/

dt_rc_e
dttmem_disk_time(
  dt_ctl_t *ctl,
  long loops,
  const char *tmpdir,				   
  int *assert_fail)
{
  DT_F("dttmem_disk_time");
  char id[128];
  dtmemsource_t memsource;
  long max;
  char name[128];
  disk_name(name, sizeof(name), tmpdir);

  max = DT_MAX(loops / 5L, 1);
  sprintf(id, "disk 63000 (%ld)", max);
  DT_Q(dtmemdisk_init,(ctl,				/* init the memory source		    */
    &memsource, name,
    0, 63000U, 2));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    max, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  max = DT_MAX(loops / 5L, 1);
  sprintf(id, "disk 16384 (%ld)", max);
  DT_Q(dtmemdisk_init,(ctl,				/* init the memory source		    */
    &memsource, name,
    0, 16384, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    max, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  max = DT_MAX(loops / 10L, 1);
  sprintf(id, "disk 4000 (%ld)", max);
  DT_Q(dtmemdisk_init,(ctl,				/* init the memory source		    */
    &memsource, name,
    0, 4000, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    max, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  max = DT_MAX(loops / 50L, 1);
  sprintf(id, "disk 512 (%ld)", max);
  DT_Q(dtmemdisk_init,(ctl,				/* init the memory source		    */
    &memsource, name,
    0, 512, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    max, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * run tests on disk memory
 *..........................................................................*/

dt_rc_e
dttmem_disk(
  dt_ctl_t *ctl,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttmem_disk");

  DT_Q(dttmem_disk_simple,(ctl,			/* run simple disk tests		    */
    tmpdir, assert_fail));

  DT_Q(dttmem_disk_multiple,(ctl,		/* run multiple disk tests		    */
    tmpdir, assert_fail));

  DT_Q(dttmem_disk_paging,(ctl,			/* run paging disk tests		    */
    tmpdir, assert_fail));

  DT_Q(dttmem_disk_copy,(ctl,			/* run copy disk tests		    	*/
    tmpdir, assert_fail));

  DT_Q(dttmem_disk_io,(ctl,				/* run I/O disk tests		    	*/
    tmpdir, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of ram memory and simple test of it
 *..........................................................................*/

dt_rc_e
dttmem_ram_simple(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_ram_simple");
  char id[128];
  dtmemsource_t memsource;

  sprintf(id, "%s at default pagesize",
    F);
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 100, 0, 0));
  DT_Q(dttmem_simple,(ctl, &memsource,	/* run simple test on memory	    */
    NOPREALLOC, 400, 400,
    id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  sprintf(id, "%s at small pagesize",
    F);
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 100, 4000, 0));
  DT_Q(dttmem_simple,(ctl, &memsource,	/* run simple test on memory	    */
    NOPREALLOC, 400, 400,
    id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of ram memory and simple test of it
 *..........................................................................*/

dt_rc_e
dttmem_ram_copy(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_ram_copy");
  char id[128];

  sprintf(id, "%s unit page", F);
  DT_Q(dttmem_copy,(ctl, "ram",
    100, 101, 100, 100,
    id, assert_fail));

  sprintf(id, "%s multi page", F);
  DT_Q(dttmem_copy,(ctl, "ram",
    100, 11, 10, 100,
    id, assert_fail));

  sprintf(id, "%s single page", F);
  DT_Q(dttmem_copy,(ctl, "ram",
    110, 1, 10, 10,
    id, assert_fail));

  sprintf(id, "%s excess page", F);
  DT_Q(dttmem_copy,(ctl, "ram",
    1000, 1, 2, 10,
    id, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of ram memory and simple test of it
 *..........................................................................*/

dt_rc_e
dttmem_ram_io(
  dt_ctl_t *ctl,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttmem_ram_io");
  char id[128];

  sprintf(id, "%s unit page", F);
  DT_Q(dttmem_io,(ctl, "ram",
    100, 101, 100, 100,
    id, tmpdir, assert_fail));

  sprintf(id, "%s multi page", F);
  DT_Q(dttmem_io,(ctl, "ram",
    100, 11, 10, 100,
    id, tmpdir, assert_fail));

  sprintf(id, "%s single page", F);
  DT_Q(dttmem_io,(ctl, "ram",
    110, 1, 10, 10,
    id, tmpdir, assert_fail));

  sprintf(id, "%s excess page", F);
  DT_Q(dttmem_io,(ctl, "ram",
    1000, 1, 2, 10,
    id, tmpdir, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of ram memory and timing tests
 *..........................................................................*/

dt_rc_e
dttmem_ram_time(
  dt_ctl_t *ctl,
  long loops,
  int *assert_fail)
{
  DT_F("dttmem_ram_time");
  char id[128];
  dtmemsource_t memsource;
  long max;

  max = loops * 1000L;
  sprintf(id, "ram 63000 (%ld)", max);
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 0, 63000U, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 400, 400,
    max, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  max = loops * 200L;
  sprintf(id, "ram 16384 (%ld)", max);
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 0, 16384, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 400, 400,
    max, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  max = loops * 50;
  sprintf(id, "ram 4000 (%ld)", max);
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 0, 4000, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 400, 400,
    max, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  max = loops * 5;
  sprintf(id, "ram 512 (%ld)", max);
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 0, 512, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 400, 400,
    max, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
dttmem_ram_patch(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_ram_patch");
  char id[128];
  dtmemsource_t memsource;

  sprintf(id, "%s at default pagesize",
    F);
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 0, 0, 0));
  DT_Q(dttmem_patch,(ctl, &memsource,	/* run patch test on memory	    	*/
    NOPREALLOC, 100, 100,
    DTMEM_TRIPWIRE_PUTPATCH_ONEPASS |
    DTMEM_TRIPWIRE_GETPATCH_ONEPASS,
    id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  sprintf(id, "%s at medium pagesize",
    F);
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 0, 500, 0));
  DT_Q(dttmem_patch,(ctl, &memsource,	/* run patch test on memory	    */
    NOPREALLOC, 100, 100,
    0,
    id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  sprintf(id, "%s at small pagesize",
    F);
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 0, 100, 0));
  DT_Q(dttmem_patch,(ctl, &memsource,	/* run patch test on memory	    */
    NOPREALLOC, 100, 100,
    0,
    id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}


/*..........................................................................
 * explicit allocation of ram memory and paging test of it
 *..........................................................................*/

dt_rc_e
dttmem_ram_paging(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_ram_paging");
  dtmemsource_t memsource;

  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 0, 0, 0));
  
  DT_Q(dttmem_paging,(ctl, &memsource,	/* run paging test on memory	    */
    F, assert_fail));

  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

										/* ................................ */
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 0, 6500, 0));				/* this time for segmented memory   */
  
  DT_Q(dttmem_paging,(ctl, &memsource,	/* run paging test on memory	    */
    F, assert_fail));

  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of multiple ram allocations and simple tests of them
 *..........................................................................*/

dt_rc_e
dttmem_ram_multiple(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_ram_multiple");
  dtmemsource_t memsource;

  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 100, 0, 0));
  
  DT_Q(dttmem_multiple,(ctl, 			/* run multiple tests on memory	    */
    &memsource, NOPREALLOC, 
    F, assert_fail));

  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * run tests on ram memory
 *..........................................................................*/

#ifdef NONO
void left(char *s)
{
  int n;
  void *m[40];
  for (n=0; n<40; n++)
    m[n] = NULL;

  for (n=0; n<40; n++)
  {
    m[n] = malloc(16384);
    if (m[n] == NULL)
      break;
  }

  printf("%s: got %d 16384-byte blocks\n", s, n);

  for (n=0; n<40; n++)
    if (m[n]) free(m[n]);
}
#endif
#define left(S)

dt_rc_e
dttmem_ram(
  dt_ctl_t *ctl,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttmem_ram");

  DT_Q(dttmem_ram_simple,(ctl,			/* run simple ram tests		    	*/
    assert_fail));

  DT_Q(dttmem_ram_patch,(ctl,			/* run patch ram tests		    	*/
    assert_fail));

  DT_Q(dttmem_ram_multiple,(ctl,		/* run multiple ram tests		    */
    assert_fail));

  DT_Q(dttmem_ram_paging,(ctl,			/* run paging ram tests		    	*/
    assert_fail));

  DT_Q(dttmem_ram_copy,(ctl,			/* run copy ram tests		    	*/
    assert_fail));

  DT_Q(dttmem_ram_io,(ctl,				/* run I/O ram tests		    	*/
    tmpdir, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * E M S
 *..........................................................................*/

/*..........................................................................
 * explicit allocation of ems memory and simple test of it
 *..........................................................................*/

dt_rc_e
dttmem_ems_simple(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_ems_simple");
  char id[128];
  dtmemsource_t memsource;

  sprintf(id, "%s at default pagesize",
    F);
  DT_Q(dtmemems_init,(ctl,				/* init the memory source		    */
    &memsource,
    "EMS", 100, 0, 0));
  DT_Q(dttmem_simple,(ctl, &memsource,	/* run simple test on memory	    */
    NOPREALLOC, 500, 1200,
    id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  sprintf(id, "%s at small pagesize",
    F);
  DT_Q(dtmemems_init,(ctl,				/* init the memory source		    */
    &memsource,
    "EMS", 100, 4000, 0));
  DT_Q(dttmem_simple,(ctl, &memsource,	/* run simple test on memory	    */
    NOPREALLOC, 500, 1200,
    id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of ems memory and simple test of it
 *..........................................................................*/

dt_rc_e
dttmem_ems_copy(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_ems_copy");
  char id[128];

  sprintf(id, "%s unit page", F);
  DT_Q(dttmem_copy,(ctl, "ems",
    100, 1, 100, 100,
    id, assert_fail));

  sprintf(id, "%s multi page", F);
  DT_Q(dttmem_copy,(ctl, "ems",
    100, 1, 10, 100,
    id, assert_fail));

  sprintf(id, "%s single page", F);
  DT_Q(dttmem_copy,(ctl, "ems",
    110, 1, 10, 10,
    id, assert_fail));

  sprintf(id, "%s excess page", F);
  DT_Q(dttmem_copy,(ctl, "ems",
    1000, 1, 2, 10,
    id, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of ems memory and simple I/O on it
 *..........................................................................*/

dt_rc_e
dttmem_ems_io(
  dt_ctl_t *ctl,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttmem_ems_io");
  char id[128];

  sprintf(id, "%s unit page", F);
  DT_Q(dttmem_io,(ctl, "ems",
    100, 1, 100, 100,
    id, tmpdir, assert_fail));

  sprintf(id, "%s multi page", F);
  DT_Q(dttmem_io,(ctl, "ems",
    100, 1, 10, 100,
    id, tmpdir, assert_fail));

  sprintf(id, "%s single page", F);
  DT_Q(dttmem_io,(ctl, "ems",
    110, 1, 10, 10,
    id, tmpdir, assert_fail));

  sprintf(id, "%s excess page", F);
  DT_Q(dttmem_io,(ctl, "ems",
    1000, 1, 2, 10,
    id, tmpdir, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of ems memory and paging test of it
 *..........................................................................*/

dt_rc_e
dttmem_ems_paging(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_ems_paging");
  dtmemsource_t memsource;

  DT_Q(dtmemems_init,(ctl,				/* init the memory source		    */
    &memsource,
    "EMS", 0, 0, 0));
  
  DT_Q(dttmem_paging,(ctl, &memsource,	/* run paging test on memory	    */
    F, assert_fail));

  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of multiple ems allocations and simple tests of them
 *..........................................................................*/

dt_rc_e
dttmem_ems_multiple(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_ems_multiple");
  dtmemsource_t memsource;

  DT_Q(dtmemems_init,(ctl,				/* init the memory source		    */
    &memsource,
    "EMS", 100, 0, 0));
  
  DT_Q(dttmem_multiple,(ctl, 			/* run multiple tests on memory	    */
    &memsource, NOPREALLOC, F, assert_fail));

  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of ems memory and timing tests
 *..........................................................................*/

dt_rc_e
dttmem_ems_time(
  dt_ctl_t *ctl,
  long loops,
  int *assert_fail)
{
  DT_F("dttmem_ems_time");
  char id[128];
  dtmemsource_t memsource;
  long max;

  sprintf(id, "ems 63000 (%ld)",
    loops);
  DT_Q(dtmemems_init,(ctl,				/* init the memory source		    */
    &memsource,
    "EMS", 0, 63000U, 2));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    loops, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  sprintf(id, "ems 16384 (%ld)",
    loops);
  DT_Q(dtmemems_init,(ctl,				/* init the memory source		    */
    &memsource,
    "EMS", 0, 16384, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    loops, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  sprintf(id, "ems 4000 (%ld)",
    loops);
  DT_Q(dtmemems_init,(ctl,				/* init the memory source		    */
    &memsource,
    "EMS", 0, 4000, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    loops, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  max = DT_MAX(loops / 2L, 1);
  sprintf(id, "ems 512 (%ld)", max);
  DT_Q(dtmemems_init,(ctl,				/* init the memory source		    */
    &memsource,
    "EMS", 0, 512, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    max, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * run tests on ems memory
 *..........................................................................*/

dt_rc_e
dttmem_ems(
  dt_ctl_t *ctl,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttmem_ems");

  DT_Q(dttmem_ems_simple,(ctl,			/* run simple ems tests		    	*/
    assert_fail));

  DT_Q(dttmem_ems_multiple,(ctl,		/* run multiple ems tests		    */
    assert_fail));

  DT_Q(dttmem_ems_paging,(ctl,			/* run paging ems tests		  		*/
    assert_fail));

  DT_Q(dttmem_ems_copy,(ctl,			/* run copy ems tests		    	*/
    assert_fail));

  DT_Q(dttmem_ems_io,(ctl,				/* run I/O ems tests		    	*/
    tmpdir, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * X M S
 *..........................................................................*/

/*..........................................................................
 * explicit allocation of xms memory and simple test of it
 *..........................................................................*/

dt_rc_e
dttmem_xms_simple(
  dt_ctl_t *ctl,
  dttmem_init_f *xms_init,
  const char *name,
  int *assert_fail)
{
  DT_F("dttmem_xms_simple");
  char id[128];
  dtmemsource_t memsource;

  sprintf(id, "%s at default pagesize",
    F);
  DT_Q(xms_init,(ctl,					/* init the memory source		    */
    &memsource,
    name, 0, 0, 0));
  DT_Q(dttmem_simple,(ctl, &memsource,	/* run simple test on memory	    */
    NOPREALLOC, 500, 1200,
    id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  sprintf(id, "%s at small pagesize",
    F);
  DT_Q(xms_init,(ctl,					/* init the memory source		    */
    &memsource,
    name, 0, 4000, 0));
  DT_Q(dttmem_simple,(ctl, &memsource,	/* run simple test on memory	    */
    NOPREALLOC, 500, 1200,
    id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of xms memory and simple test of it
 *..........................................................................*/

dt_rc_e
dttmem_xms_copy(
  dt_ctl_t *ctl,
  dttmem_init_f *xms_init,
  const char *name,
  int *assert_fail)
{
  DT_F("dttmem_xms_copy");
  char id[128];

  sprintf(id, "%s unit page", F);
  DT_Q(dttmem_copy,(ctl, name,
    100, 1, 100, 100,
    id, assert_fail));

  sprintf(id, "%s multi page", F);
  DT_Q(dttmem_copy,(ctl, name,
    100, 1, 10, 100,
    id, assert_fail));

  sprintf(id, "%s single page", F);
  DT_Q(dttmem_copy,(ctl, name,
    110, 1, 10, 10,
    id, assert_fail));

  sprintf(id, "%s excess page", F);
  DT_Q(dttmem_copy,(ctl, name,
    1000, 1, 2, 10,
    id, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of xms memory and simple I/O of it
 *..........................................................................*/

dt_rc_e
dttmem_xms_io(
  dt_ctl_t *ctl,
  dttmem_init_f *xms_init,
  const char *name,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttmem_xms_io");
  char id[128];

  sprintf(id, "%s unit page", F);
  DT_Q(dttmem_io,(ctl, name,
    100, 1, 100, 100,
    id, tmpdir, assert_fail));

  sprintf(id, "%s multi page", F);
  DT_Q(dttmem_io,(ctl, name,
    100, 1, 10, 100,
    id, tmpdir, assert_fail));

  sprintf(id, "%s single page", F);
  DT_Q(dttmem_io,(ctl, name,
    110, 1, 10, 10,
    id, tmpdir, assert_fail));

  sprintf(id, "%s excess page", F);
  DT_Q(dttmem_io,(ctl, name,
    1000, 1, 2, 10,
    id, tmpdir, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of xms memory and paging test of it
 *..........................................................................*/

dt_rc_e
dttmem_xms_paging(
  dt_ctl_t *ctl,
  dttmem_init_f *xms_init,
  const char *name,
  int *assert_fail)
{
  DT_F("dttmem_xms_paging");
  dtmemsource_t memsource;

  DT_Q(xms_init,(ctl,					/* init the memory source		    */
    &memsource,
    name, 0, 0, 0));
  
  DT_Q(dttmem_paging,(ctl, &memsource,	/* run paging test on memory	    */
    F, assert_fail));

  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of multiple ems allocations and simple tests of them
 *..........................................................................*/

dt_rc_e
dttmem_xms_multiple(
  dt_ctl_t *ctl,
  dttmem_init_f *xms_init,
  const char *name,
  int *assert_fail)
{
  DT_F("dttmem_xms_multiple");
  dtmemsource_t memsource;

  DT_Q(xms_init,(ctl,			/* init the memory source		    */
    &memsource,
    name, 100, 0, 0));
  
  DT_Q(dttmem_multiple,(ctl, 			/* run multiple tests on memory	    */
    &memsource, NOPREALLOC, F, assert_fail));

  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of xms memory and timing tests
 *..........................................................................*/

dt_rc_e
dttmem_xms_time(
  dt_ctl_t *ctl,
  dttmem_init_f *xms_init,
  const char *name,
  long loops,
  int *assert_fail)
{
  DT_F("dttmem_xms_time");
  char id[128];
  dtmemsource_t memsource;
  long max;

  sprintf(id, "%s 63000 (%ld)",
    name, loops);
  DT_Q(xms_init,(ctl,					/* init the memory source		    */
    &memsource,
    name, 0, 63000U, 2));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    loops, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  sprintf(id, "%s 16384 (%ld)",
    name, loops);
  DT_Q(xms_init,(ctl,					/* init the memory source		    */
    &memsource,
    name, 0, 16384, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    loops, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  sprintf(id, "%s  4000 (%ld)",
    name, loops);
  DT_Q(xms_init,(ctl,					/* init the memory source		    */
    &memsource,
    name, 0, 4000, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    loops, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  max = DT_MAX(loops / 2L, 1);
  sprintf(id, "%s   512 (%ld)", 
    name, max);
  DT_Q(xms_init,(ctl,					/* init the memory source		    */
    &memsource,
    name, 0, 512, 0));
  DT_Q(dttmem_time,(ctl, &memsource,	/* run timing test on memory	    */
    NOPREALLOC, 500, 1200,
    max, id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  return DT_RC_GOOD;
}

/*..........................................................................
 * run tests on xms memory
 *..........................................................................*/

dt_rc_e
dttmem_xms(
  dt_ctl_t *ctl,
  dttmem_init_f *xms_init,
  const char *name,
  const char *tmpdir,
  int *assert_fail)
{
  DT_F("dttmem_xms");

  DT_Q(dttmem_xms_simple,(ctl,			/* run simple ems tests		    */
    xms_init, name,
    assert_fail));

  DT_Q(dttmem_xms_multiple,(ctl,		/* run multiple ems tests		    */
    xms_init, name,
    assert_fail));

  DT_Q(dttmem_xms_paging,(ctl,			/* run paging ems tests		    */
    xms_init, name,
    assert_fail));

  DT_Q(dttmem_xms_copy,(ctl,			/* run copy xms tests		    	*/
    xms_init, name,
    assert_fail));

  DT_Q(dttmem_xms_io,(ctl,				/* run I/O xms tests		    	*/
    xms_init, name,
    tmpdir, assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................
 * P R E A L L O C
 *..........................................................................*/

/*..........................................................................
 * explicit allocation of prealloc memory and simple test of it
 *..........................................................................*/

dt_rc_e
dttmem_prealloc_simple(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_prealloc_simple");
  char id[128];
# define NPAGES 5
# define PAGESIZE 10000
  void *pages[NPAGES];
  int i;
  dtmemsource_t memsource;

  DT_Q(dtos_malloc2,(ctl,
    &pages[0], (long)PAGESIZE*(long)NPAGES,
    F, "big preallocated page"));
  sprintf(id, "%s one preallocated page",
    F);
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 100, 0, 0));
  DT_Q(dttmem_simple,(ctl, &memsource,	/* run simple test on memory	    */
    pages, 
    (unsigned)PAGESIZE*(unsigned)NPAGES, 1,
    PAGESIZE, NPAGES, 
    id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));
  DT_Q(dtos_free2,(ctl,
    pages[0],
    F, "big preallocated page"));

  for (i=0; i<NPAGES; i++)
    DT_Q(dtos_malloc2,(ctl,
      &pages[i], PAGESIZE,
      F, "small preallocated page"));

  sprintf(id, "%s %d preallocated pages",
    F, NPAGES);
  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 100, 0, 0));
  DT_Q(dttmem_simple,(ctl, &memsource,	/* run simple test on memory	    */
    pages, PAGESIZE, NPAGES,
    PAGESIZE, NPAGES, 
    id, assert_fail));
  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  for (i=0; i<NPAGES; i++)
    DT_Q(dtos_free2,(ctl,
      pages[i],
      F, "small preallocated page"));

  return DT_RC_GOOD;
}

/*..........................................................................
 * explicit allocation of multiple prealloc allocations and simple tests of them
 *..........................................................................*/

dt_rc_e
dttmem_prealloc_multiple(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_prealloc_multiple");
  dtmemsource_t memsource;
  void *pages[NMULTIPLE];
  int i;

  for (i=0; i<NMULTIPLE; i++)
  DT_Q(dtos_malloc2,(ctl,
    &pages[i], PAGESIZE,
    F, "preallocated page"));

  DT_Q(dtmemram_init,(ctl,				/* init the memory source		    */
    &memsource,
    "RAM", 100, 0, 0));
  
  DT_Q(dttmem_multiple,(ctl, 			/* run multiple tests on memory	    */
    &memsource, pages, PAGESIZE, 1,
    F, assert_fail));

  DT_Q((memsource.uninit),(ctl,			/* get rid of the memory source	    */
    &memsource));

  for (i=0; i<NMULTIPLE; i++)
  DT_Q(dtos_free2,(ctl,
    pages[i],
    F, "preallocated page"));

  return DT_RC_GOOD;
}

/*..........................................................................
 * run tests on ram memory
 *..........................................................................*/

dt_rc_e
dttmem_prealloc(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_prealloc");

  DT_Q(dttmem_prealloc_simple,(ctl,		/* run simple prealloc tests	    */
    assert_fail));
  DT_Q(dttmem_prealloc_multiple,(ctl,	/* run multiple prealloc tests	    */
    assert_fail));

  return DT_RC_GOOD;
}

/*..........................................................................*/

dt_rc_e 
DTCONFIG_API1
dttmem_arg_check(						/* check params for this program	*/
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttmem_arg_check");

  DT_Q(dtt_check_op,(ctl, param,		/* make sure op is understood	    */
    ops, msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................
 *..........................................................................*/

DTT_TEST(dttmem)
{
  DT_F("dttmem");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  void *memsource;
  int assert_fail = 0;
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));			/* init memory checking			    */

  ctl->tripwire.flags |=				/* we'll do tripwiring tests	    */
    DT_TRIPWIRE_MEM;

  DT_GI(dtparam_init,(param));			/* init param structure             */

  DTPARAM_DEFAULT(op, "default");		/* set default for params we want	*/

  DT_GI(dtparam_set,(param, 0,			/* set default for params we want	*/
    &param->op, NULL, NULL, "default",
    &param->reps, NULL, NULL, "1",
    &param->n, "Timing loops", NULL, "100",
    &param->output, "Tmp dir", NULL, "/tmp",
    NULL));

  arg[0].desc = NULL;					/* no args to this program		    */
  
  DT_GI(dt_enter_parse,(				/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttmem_arg_check, NULL));

  memsource = ctl->memsource;			/* squirrel this away during tests  */

  ctl->memsource = NULL;				/* let tests have a fresh start	    */

  {
    long reps;
    for (reps=0; 
         reps < param->reps && 
         rc == DT_RC_GOOD;
         reps++)
	{
      if (DT_ABBREV("default", param->op) ||
          DT_ABBREV("parse", param->op))
      {
        DT_GI(dttmem_parse,(ctl,		/* run parse tests				    */
          &assert_fail));
      }
    
      if (DT_ABBREV("default", param->op) ||
          DT_ABBREV("config", param->op))
      {
        DT_GI(dttmem_config,(ctl,		/* run config tests				    */
          param->output, &assert_fail));
      }
    
      if (DT_ABBREV("default", param->op) ||
          DT_ABBREV("ram", param->op))
      {
        DT_GI(dttmem_ram,(ctl,			/* run ram tests				    */
           param->output, &assert_fail));
      }
  
      if ((DT_ABBREV("default", param->op) &&
           dtprj_emsif) ||
          DT_ABBREV("ems", param->op))
      {
        DT_GI(dttmem_ems,(ctl,			/* run ems tests				    */
           param->output, &assert_fail));
      }
  
      if ((DT_ABBREV("default", param->op) &&
           dtprj_xmsif) ||
          DT_ABBREV("xmsif", param->op))
      {
        DT_GI(dttmem_xms,(ctl,			/* run xmsif tests				    */
          dtmemxmsif_init, "xmsif",
          param->output, &assert_fail));
      }
  
      if ((DT_ABBREV("default", param->op) &&
           dtprj_xmslb) ||
          DT_ABBREV("xmslb", param->op))
      {
        DT_GI(dttmem_xms,(ctl,			/* run xmslb tests				    */
          dtmemxmslb_init, "xmslb",
          param->output, &assert_fail));
      }
  
      if ((DT_ABBREV("default", param->op) &&
           dtprj_txms) ||
          DT_ABBREV("txms", param->op))
      {
        DT_GI(dttmem_xms,(ctl,			/* run txms tests				    */
          dtmemtxms_init, "txms",
          param->output, &assert_fail));
      }
    
      if (DT_ABBREV("default", param->op) ||
          DT_ABBREV("prealloc", param->op))
      {
        DT_GI(dttmem_prealloc,(ctl,		/* run prealloc tests			    */
          &assert_fail));
      }
    
      if ((DT_ABBREV("default", param->op) &&
           dtprj_memdisk) ||
          DT_ABBREV("disk", param->op))
      {
        DT_GI(dttmem_disk,(ctl,			/* run disk tests				    */
           param->output, &assert_fail));
      }
    
      if (DT_ABBREV("time", param->op))
      {
        DT_Q(dttmem_ram_time,(ctl,		/* run ram timing tests		    	*/
          param->n, &assert_fail));
		if (dtprj_memdisk)
        DT_Q(dttmem_disk_time,(ctl,		/* run disk timing tests		   	*/
          param->n, param->output,
          &assert_fail));
        if (dtprj_emsif)
        DT_Q(dttmem_ems_time,(ctl,		/* run ems timing tests		    	*/
          param->n, &assert_fail));
	  }

      if ((DT_ABBREV("time", param->op) &&
           dtprj_xmsif) ||
          DT_ABBREV("xmsif-time", param->op))
      {
        DT_Q(dttmem_xms_time,(ctl,		/* run xmsif timing tests	    	*/
          dtmemxmsif_init, "xmsif",
          param->n, &assert_fail));
	  }

      if ((DT_ABBREV("time", param->op) &&
           dtprj_xmslb) ||
          DT_ABBREV("xmslb-time", param->op))
      {
        DT_Q(dttmem_xms_time,(ctl,		/* run xmslb timing tests	    	*/
          dtmemxmslb_init, "xmslb",
          param->n, &assert_fail));
	  }

      if ((DT_ABBREV("time", param->op) &&
           dtprj_txms) ||
          DT_ABBREV("txms-time", param->op))
      {
        DT_Q(dttmem_xms_time,(ctl,		/* run txms timing tests	    	*/
          dtmemtxms_init, "txms",
          param->n, &assert_fail));
      }
    }  
  }

  ctl->memsource = memsource;			/* replace value we protected		*/

  DT_I(dt_leave,(param));				/* clean up before leaving			*/

  DT_I(dt_heapmon_uninit,(ctl,			/* check all memory is freed	    */
    &assert_fail));

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
