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

DT_RCSID("test $RCSfile: dttmem2.c,v $ $Revision: 1.12 $");

#include <dtack/mem.h>
#include <dtack/heap.h>
#include <dtack/prj.h>

/*..........................................................................
 * run config string parse tests
 *..........................................................................*/

dt_rc_e
dttmem_parse(
  dt_ctl_t *ctl,
  int *assert_fail)
{
  DT_F("dttmem_parse");
  char name[128];
  long maxusage;
  unsigned int pagesize, npages;
  const char *end;
  const char *id;
# define A(COND, MSG, VAL) \
  { \
    char t[64]; \
    sprintf(t, "\"%-20s", id); t[strlen(id)+1] = '"'; t[20] = '\0'; \
    *assert_fail += DT_RC_GOOD != \
    dt_assert(ctl, COND, t, MSG, VAL); \
  }

  id = "";
  DT_Q(dtmem_parse,(ctl, id, name,
    sizeof(name),
    &maxusage, &pagesize, &npages,
    &end));
  id = "EOS";
  A(!strcmp(name, ""), "name \"\" (\"%s\")", name);
  A(maxusage == 0, "maxusage 0 (%ld)", maxusage);
  A(pagesize == 0, "pagesize 0 (%u)", pagesize);
  A(npages == 0, "npages 0 (%u)", npages);
  A(*end == '\0', "end EOS (0x%02x)", *end);

  id = "+";
  DT_Q(dtmem_parse,(ctl, id, name,
    sizeof(name),
    &maxusage, &pagesize, &npages,
    &end));
  id = "PLUS";
  A(!strcmp(name, ""), "name \"\" (\"%s\")", name);
  A(maxusage == 0, "maxusage 0 (%ld)", maxusage);
  A(pagesize == 0, "pagesize 0 (%u)", pagesize);
  A(npages == 0, "npages 0 (%u)", npages);
  A(*end == '\0', "end EOS (0x%02x)", *end);

  id = "ram";
  DT_Q(dtmem_parse,(ctl, id, name,
    sizeof(name),
    &maxusage, &pagesize, &npages,
    &end));
  A(!strcmp(name, "ram"), "name \"ram\" (\"%s\")", name);
  A(maxusage == 0, "maxusage 0 (%ld)", maxusage);
  A(pagesize == 0, "pagesize 0 (%u)", pagesize);
  A(npages == 0, "npages 0 (%u)", npages);
  A(*end == '\0', "end EOS (0x%02x)", *end);

  id = "ram,,,";
  DT_Q(dtmem_parse,(ctl, id, name,
    sizeof(name),
    &maxusage, &pagesize, &npages,
    &end));
  A(!strcmp(name, "ram"), "name \"ram\" (\"%s\")", name);
  A(maxusage == 0, "maxusage 0 (%ld)", maxusage);
  A(pagesize == 0, "pagesize 0 (%u)", pagesize);
  A(npages == 0, "npages 0 (%u)", npages);
  A(*end == '\0', "end EOS (0x%02x)", *end);

  id = "ram,1000K+";
  DT_Q(dtmem_parse,(ctl, id, name,
    sizeof(name),
    &maxusage, &pagesize, &npages,
    &end));
  A(!strcmp(name, "ram"), "name \"ram\" (\"%s\")", name);
  A(maxusage == 1024000L, "maxusage 1024000 (%ld)", maxusage);
  A(pagesize == 0, "pagesize 0 (%u)", pagesize);
  A(npages == 0, "npages 0 (%u)", npages);
  A(*end == '\0', "end EOS (0x%02x)", *end);

  id = "xxx,1M,,3+X";
  DT_Q(dtmem_parse,(ctl, id, name,
    sizeof(name),
    &maxusage, &pagesize, &npages,
    &end));
  A(!strcmp(name, "xxx"), "name \"xxx\" (\"%s\")", name);
  A(maxusage == 1048576L, "maxusage 1048576 (%ld)", maxusage);
  A(pagesize == 0, "pagesize 0 (%u)", pagesize);
  A(npages == 3, "npages 3 (%u)", npages);
  A(*end == 'X', "end X (0x%02x)", *end);

  id = "xxx,,,8000+X";
  DT_Q(dtmem_parse,(ctl, id, name,
    sizeof(name),
    &maxusage, &pagesize, &npages,
    &end));
  A(!strcmp(name, "xxx"), "name \"xxx\" (\"%s\")", name);
  A(maxusage == 0, "maxusage 0 (%ld)", maxusage);
  A(pagesize == 0, "pagesize 0 (%u)", pagesize);
  A(npages == 8000, "npages 8000 (%u)", npages);
  A(*end == 'X', "end X (0x%02x)", *end);

  id = "xxx,1M,65384,3";
  DT_Q(dtmem_parse,(ctl, id, name,
    sizeof(name),
    &maxusage, &pagesize, &npages,
    &end));
  A(!strcmp(name, "xxx"), "name \"xxx\" (\"%s\")", name);
  A(maxusage == 1048576L, "maxusage 1048576 (%ld)", maxusage);
  A(pagesize == 65384L, "pagesize 65384 (%u)", pagesize);
  A(npages == 3, "npages 3 (%u)", npages);
  A(*end == '\0', "end EOS (0x%02x)", *end);

  id = "xxx,1000,65384,+Y";
  DT_Q(dtmem_parse,(ctl, id, name,
    sizeof(name),
    &maxusage, &pagesize, &npages,
    &end));
  A(!strcmp(name, "xxx"), "name \"xxx\" (\"%s\")", name);
  A(maxusage == 1000, "maxusage 1000 (%ld)", maxusage);
  A(pagesize == 65384L, "pagesize 65384 (%u)", pagesize);
  A(npages == 0, "npages 0 (%u)", npages);
  A(*end == 'Y', "end Y (0x%02x)", *end);

  id = "xxx,,65384++";
  DT_Q(dtmem_parse,(ctl, id, name,
    sizeof(name),
    &maxusage, &pagesize, &npages,
    &end));
  A(!strcmp(name, "xxx"), "name \"xxx\" (\"%s\")", name);
  A(maxusage == 0, "maxusage 0 (%ld)", maxusage);
  A(pagesize == 65384L, "pagesize 65384 (%u)", pagesize);
  A(npages == 0, "npages 0 (%u)", npages);
  A(*end == '+', "end PLUS (0x%02x)", *end);

  return DT_RC_GOOD;
}

/*..........................................................................
 * run config string tests
 *..........................................................................*/

dt_rc_e
dttmem_config(
  dt_ctl_t *ctl,
  const char *tmpdir,			  
  int *assert_fail)
{
  DT_F("dttmem_config");
  dtmemsource_t *memsource;
  dtmemalloc_t *memalloc1;
  dtmemalloc_t *memalloc2;
  dtmemalloc_t *memalloc3;
  char *id;
  char name[128];
  int i;
# define B(I, COND) \
  { \
    char t[64]; \
    sprintf(t, "\"%-20s", id); t[strlen(id)+1] = '"'; t[20] = '\0'; \
    *assert_fail += DT_RC_GOOD != \
    dt_assert(ctl, COND, t, \
    "[%d] \"%s\" %ld %u %u", I, \
    memsource[I].name? memsource[i].name: "NULL", \
    memsource[I].maxusage, \
    memsource[I].pagesize, memsource[I].npages); \
  }

  id = NULL;
  DT_Q(dtmem_configure,(ctl, id));
  memsource = (dtmemsource_t *)ctl->memsource;
  id = "all defaults";
  i = 0;
  B(i, !strcmp(memsource[i].name, "ram") &&
    memsource[i].maxusage == 0 &&
    memsource[i].pagesize == DTMEMRAM_PAGESIZE_DEFAULT && 
    memsource[i].npages == 0); 
  for (i=1; i<3; i++)
    B(i, memsource[i].name[0] == '\0' &&
      memsource[i].maxusage == 0 &&
      memsource[i].pagesize == 0 && 
      memsource[i].npages == 0); 
  DT_Q(dtmem_unconfigure,(ctl));


  if (dtprj_memdisk)					/* compiled-in support for mem_disk? */
  {

  DT_Q(dtstr_printf,(ctl, name, sizeof(name),
    "ram,1M+%s/dttmem%%02d.tmp,60M,,8",
    tmpdir));
  DT_Q(dtmem_configure,(ctl, name));
  memsource = (dtmemsource_t *)ctl->memsource;
  id = "60 MB filesystem";
  i = 0;
  B(i, !strcmp(memsource[i].name, "ram") &&
    memsource[i].maxusage == 1048576L &&
    memsource[i].pagesize == DTMEMRAM_PAGESIZE_DEFAULT && 
    memsource[i].npages == 0); 
  i = 1;
  DT_Q(dtstr_printf,(ctl, name, sizeof(name),
    "%s/dttmem%%02d.tmp",
    tmpdir));
  B(i, !strcmp(memsource[i].name, name) &&
    memsource[i].maxusage == 62914560L &&
    memsource[i].pagesize == DTMEMDISK_PAGESIZE_DEFAULT && 
    memsource[i].npages == 8); 
  for (i=2; i<4; i++)
    B(i, memsource[i].name[0] == '\0' &&
      memsource[i].maxusage == 0L &&
      memsource[i].pagesize == 0 && 
      memsource[i].npages == 0); 
  DT_Q(dtmem_unconfigure,(ctl));

										/* ................................ */
										/* small ram, fallback onto disk  	*/
  DT_Q(dtstr_printf,(ctl, name, sizeof(name),
    "ram,10K+%s/dttmem%%02d.tmp",
    tmpdir));
  DT_Q(dtmem_configure,(ctl, name));
  memsource = (dtmemsource_t *)ctl->memsource;
  id = "10K ram";
  i = 0;
  B(i, !strcmp(memsource[i].name, "ram") &&
    memsource[i].maxusage == 10240L &&
    memsource[i].pagesize == DTMEMRAM_PAGESIZE_DEFAULT && 
    memsource[i].npages == 0); 
  i = 1;
  DT_Q(dtstr_printf,(ctl, name, sizeof(name),
    "%s/dttmem%%02d.tmp",
    tmpdir));
  B(i, !strcmp(memsource[i].name, name) &&
    memsource[i].maxusage == 0 &&
    memsource[i].pagesize == DTMEMDISK_PAGESIZE_DEFAULT && 
    memsource[i].npages == 2); 
  for (i=2; i<4; i++)
    B(i, memsource[i].name[0] == '\0' &&
      memsource[i].maxusage == 0L &&
      memsource[i].pagesize == 0 && 
      memsource[i].npages == 0);

# define D(ID, MEMALLOC, TYPE) \
  { \
    char t[64]; \
    sprintf(t, "\"%-20s", id); t[strlen(id)+1] = '"'; t[20] = '\0'; \
    *assert_fail += DT_RC_GOOD != \
    dt_assert(ctl, TYPE == MEMALLOC->mem.type, t, \
      "\"%s\" type is %d (\"%s\")", ID, TYPE, #TYPE); \
  }

  DT_Q(dtmem_alloc,(ctl,				/* use all ram in one piece			*/
    (void **)&memalloc1,
    0, 0, 10240, 1, F, "#1A"));
  D("#1A", memalloc1, DTMEM_TYPE_MALLOC);
  DT_Q(dtmem_alloc,(ctl,				/* then expect disk					*/
    (void **)&memalloc2,
    0, 0, 10240, 1, F, "#1B"));
  D("#1B", memalloc2, DTMEM_TYPE_DISK);
  DT_Q(dtmem_free,(ctl, memalloc2,
    F, "#1B"));
  DT_Q(dtmem_free,(ctl, memalloc1,		/* last free does unconfigure 		*/
    F, "#1A"));

  DT_Q(dtstr_printf,(ctl, name, sizeof(name),
    "ram,11K+%s/dttmem%%02d.tmp",
    tmpdir));
  DT_Q(dtmem_configure,(ctl, name));
  DT_Q(dtmem_alloc,(ctl,				/* use all ram in two pices		    */
    (void **)&memalloc1,
    0, 0, 11263, 1, F, "#2A"));
  D("#2A", memalloc1, DTMEM_TYPE_MALLOC);
  DT_Q(dtmem_alloc,(ctl, 
    (void **)&memalloc2,
    0, 0, 1, 1, F, "#2B"));
  D("#2B", memalloc2, DTMEM_TYPE_MALLOC);
  DT_Q(dtmem_alloc,(ctl,				/* then expect disk					*/
    (void **)&memalloc3,
    0, 0, 10240, 1, F, "#3"));
  D("#2C", memalloc3, DTMEM_TYPE_DISK);
  DT_Q(dtmem_free,(ctl, memalloc3,
    F, "#2C"));
  DT_Q(dtmem_free,(ctl, memalloc2,
    F, "#2B"));
  DT_Q(dtmem_free,(ctl, memalloc1,		/* last free does unconfigure 		*/
    F, "#2A"));

										/* ................................ */
										/* simulate out-of-ram				*/
  DT_Q(dtstr_printf,(ctl, name, sizeof(name),
    "ram,,2000+%s/dttmem%%02d.tmp,,2000",
    tmpdir));
  DT_Q(dtmem_configure,(ctl, name));
  memsource = (dtmemsource_t *)ctl->memsource;
  id = "out-of-ram";
  i = 0;
  B(i, !strcmp(memsource[i].name, "ram") &&
    memsource[i].maxusage == 0 &&
    memsource[i].pagesize == 2000 && 
    memsource[i].npages == 0); 
  i = 1;
  DT_Q(dtstr_printf,(ctl, name, sizeof(name),
    "%s/dttmem%%02d.tmp",
    tmpdir));
  B(i, !strcmp(memsource[i].name, name) &&
    memsource[i].maxusage == 0 &&
    memsource[i].pagesize == 2000 && 
    memsource[i].npages == 2); 
  for (i=2; i<4; i++)
    B(i, memsource[i].name[0] == '\0' &&
      memsource[i].maxusage == 0L &&
      memsource[i].pagesize == 0 && 
      memsource[i].npages == 0);

  DT_Q(dtheap_set_limit,(ctl,			/* set artificial malloc limit	    */
    ctl->heap,
    -1, 20000));

  DT_Q(dtmem_alloc,(ctl,				/* use all ram in one piece			*/
    (void **)&memalloc1,
    0, 0, 2000, 5, F, "#1A"));
  D("#1A", memalloc1, DTMEM_TYPE_MALLOC);
  DT_Q(dtmem_alloc,(ctl,				/* then expect disk					*/
    (void **)&memalloc2,
    0, 0, 2000, 5, F, "#1B"));
  D("#1B", memalloc2, DTMEM_TYPE_DISK);
  DT_Q(dtmem_free,(ctl, memalloc2,
    F, "#1B"));
  DT_Q(dtmem_free,(ctl, memalloc1,		/* last free does unconfigure 		*/
    F, "#1A"));

										/* ................................ */
  DT_Q(dtstr_printf,(ctl, name, sizeof(name),
    "ram,,1000+%s/dttmem%%02d.tmp,,1000",
    tmpdir));

  DT_Q(dtheap_set_limit,(ctl,			/* set artificial malloc limit	    */
    ctl->heap,
    -1, 10000));

  DT_Q(dtmem_configure,(ctl, name));

  DT_Q(dtmem_alloc,(ctl,				/* use all ram in two pices		    */
    (void **)&memalloc1,
    0, 0, 1000, 4, F, "#2A"));
  D("#2A", memalloc1, DTMEM_TYPE_MALLOC);
  DT_Q(dtmem_alloc,(ctl, 
    (void **)&memalloc2,
    0, 0, 1000-500, 1, F, "#2B"));
  D("#2B", memalloc2, DTMEM_TYPE_MALLOC);
  DT_Q(dtmem_alloc,(ctl,				/* then expect disk					*/
    (void **)&memalloc3,
    0, 0, 1000, 5, F, "#3"));
  D("#2C", memalloc3, DTMEM_TYPE_DISK);
  DT_Q(dtmem_free,(ctl, memalloc3,
    F, "#3"));
  DT_Q(dtmem_free,(ctl, memalloc2,
    F, "#2B"));
  DT_Q(dtmem_free,(ctl, memalloc1,
    F, "#2A"));

  DT_Q(dtheap_set_limit,(ctl,			/* unset artificial malloc limit    */
    ctl->heap,
    -1, -1));
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
