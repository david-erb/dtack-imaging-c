/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DTLINK2_ISOLATE(CHECK, CTL, ELEM, TYPE, LIST) \
  { \
    (ELEM)->LIST.head = (ELEM); \
    (ELEM)->LIST.next = \
    (ELEM)->LIST.prev = NULL; \
  }

#define DTLINK2_LINK(CHECK, CTL, ELEM1, ELEM2, TYPE, LIST) \
  { \
    TYPE * const e1 = (ELEM1); \
    TYPE * const h1 = e1->LIST.head; \
    TYPE * const h2 = (ELEM2)->LIST.head; \
    TYPE *t; \
    h2->LIST.prev = e1; \
    for (t=h2; t->LIST.next; t=t->LIST.next) \
      t->LIST.head = h1; \
    t->LIST.head = h1; \
    if (e1->LIST.next) \
	{ \
      e1->LIST.next->LIST.prev = t; \
      t->LIST.next = e1->LIST.next; \
	} \
    e1->LIST.next = h2; \
  }

#define DTLINK2_RELINK(CHECK, CTL, ELEM1, ELEM2, TYPE, LIST) \
  { \
    TYPE * const e1 = (ELEM1); \
    TYPE * const h1 = e1->LIST.head; \
    TYPE * const h2 = (ELEM2); \
    TYPE *t; \
    h2->LIST.prev = e1; \
    for (t=h2; t->LIST.next; t=t->LIST.next) \
      t->LIST.head = h1; \
    t->LIST.head = h1; \
    if (e1->LIST.next) \
    { \
      e1->LIST.next->LIST.prev = t; \
      t->LIST.next = e1->LIST.next; \
	} \
    e1->LIST.next = h2; \
  }

#define DTLINK2_LINK1(CHECK, CTL, ELEM1, ELEM2, TYPE, LIST) \
  { \
    TYPE * const e1 = (ELEM1); \
    TYPE * const e2 = (ELEM2); \
    e2->LIST.prev = e1; \
    if (e1->LIST.next) \
      e1->LIST.next->LIST.prev = e2; \
    e2->LIST.next = e1->LIST.next; \
    e1->LIST.next = e2; \
    e2->LIST.head = e1->LIST.head; \
  }

#define DTLINK2_UNLINK1(CHECK, CTL, ELEM, TYPE, LIST) \
  { \
    if ((ELEM)->LIST.next) \
      (ELEM)->LIST.next->LIST.prev = (ELEM)->LIST.prev; \
    if ((ELEM)->LIST.prev) \
      (ELEM)->LIST.prev->LIST.next = (ELEM)->LIST.next; \
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
