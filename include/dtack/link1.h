/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DTLINK1_ISOLATE(CHECK, CTL, ELEM, TYPE, LIST) \
  { \
    (ELEM)->LIST.head = (ELEM); \
    (ELEM)->LIST.next = NULL; \
  }

#define DTLINK1_LINK(CHECK, CTL, ELEM1, ELEM2, TYPE, LIST) \
  { \
    TYPE * const e1 = (ELEM1); \
    TYPE * const h1 = e1->LIST.head; \
    TYPE * const h2 = (ELEM2)->LIST.head; \
    TYPE *t; \
    for (t=h2; t->LIST.next; t=t->LIST.next) \
      t->LIST.head = h1; \
    t->LIST.head = h1; \
    t->LIST.next = e1->LIST.next; \
    e1->LIST.next = h2; \
  }

#define DTLINK1_RELINK(CHECK, CTL, ELEM1, ELEM2, TYPE, LIST) \
  { \
    TYPE * const e1 = (ELEM1); \
    TYPE * const h1 = e1->LIST.head; \
    TYPE * const h2 = (ELEM2); \
    TYPE *t; \
    for (t=h2; t->LIST.next; t=t->LIST.next) \
      t->LIST.head = h1; \
    t->LIST.head = h1; \
    t->LIST.next = e1->LIST.next; \
    e1->LIST.next = h2; \
  }

#define DTLINK1_LINK1(CHECK, CTL, ELEM1, ELEM2, TYPE, LIST) \
  { \
    TYPE * const e1 = (ELEM1); \
    TYPE * const e2 = (ELEM2); \
    e2->LIST.next = e1->LIST.next; \
    e1->LIST.next = e2; \
    e2->LIST.head = e1->LIST.head; \
  }

#define DTLINK1_UNLINK1(CHECK, CTL, ELEM, TYPE, LIST) \
  { \
    TYPE * const e = (ELEM); \
    TYPE *t; \
    for (t=e->LIST.head; t->LIST.next!=e; t=t->LIST.next); \
    t->LIST.next = e->LIST.next; \
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
