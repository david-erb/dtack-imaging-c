/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




typedef enum {
  DTSTACK_CODE_OK,
  DTSTACK_CODE_OVERFLOW,
  DTSTACK_CODE_UNDERFLOW
} dtstack_code_e;

#define DTSTACK_DEFAULT_INITIAL_SIZE (16384U)
#define DTSTACK_DEFAULT_LIMIT (0L)

#define DTSTACK_FLAG_FREE (0x01)		/* free memallco when done		    */

typedef enum {
  DTSTACK_SET_FLAG_OVERFLOW,
  DTSTACK_SET_FLAG_POSITION
} dtstack_set_flag_e;

typedef struct {
  void *memalloc;
  char *el;								/* pointer into page's mapped data	*/
  char *elmin;
  char *elmax;

  long offset;							/* offset of mapped page in bytes	*/
  long offset_cur;						/* current size of stack in bytes	*/
  long offset_max;						/* arbitrary limit to offset		*/

  unsigned int a_units_cur;
  unsigned int a_units_inc;
  unsigned int a_units_max;
  unsigned int a_size_cur;
  unsigned int a_size_inc;
  unsigned int a_size_max;

  unsigned int elsize;
  unsigned int datasize;				/* nominal page size				*/

  int repage;							/* repage flag					    */
  int flags;							/* other flags					    */
} dtstack_t;

#define DTSTACK_DEPTH(STACK) \
  (((STACK)->offset + \
    ((STACK)->el - (STACK)->elmin)) / \
    (STACK)->elsize)

#define DTSTACK_ISEMPTY(STACK) \
  ((STACK)->offset == 0 && \
   (STACK)->el == (STACK)->elmin)
										/* ................................ */
#define DTSTACK_PUSHEL(STACK, EL)                                             \
{                                                                             \
  *(EL) = (STACK)->el;					/* return mapped pointer		    */\
  (STACK)->el += (STACK)->elsize;                                             \
}

										/* ................................ */
#define DTSTACK_POPEL(STACK, EL)                                              \
{                                                                             \
  (STACK)->el -= (STACK)->elsize;                                             \
  *(EL) = (void *)(STACK)->el;                                                \
}
										/* ................................	*/
#if DTPRJ_NOPAGING == DT_1				/* paging not allowed? 				*/
#  define DTSTACK_REPAGE_OR(STACK)		/* never bother with repaging		*/
#else
#  define DTSTACK_REPAGE_OR(STACK) \
     (STACK)->repage ||
#endif

#ifndef DTSTACK_PUSH
#define DTSTACK_PUSH(CHECK, CTL, STACK, EL, CODE)                             \
{                                                                             \
  if (DTSTACK_REPAGE_OR(STACK)			/* explicit repage required?		*/\
      (STACK)->el == (STACK)->elmax)	/* page is full?					*/\
  {                                                                           \
    CHECK(dtstack_push,(CTL, STACK,		/* increase stack pointers		    */\
      EL, CODE));                                                             \
  }                                                                           \
  else                                                                        \
    DTSTACK_PUSHEL(STACK, EL);                                                \
}
#endif

										/* ................................ */
#ifndef DTSTACK_POP
#define DTSTACK_POP(CHECK, CTL, STACK, EL, CODE)                              \
{                                                                             \
  if (DTSTACK_REPAGE_OR(STACK)			/* explicit repage required?		*/\
      (STACK)->el == (STACK)->elmin)	/* hit bottom of page?				*/\
  {                                                                           \
    CHECK(dtstack_pop,(CTL, STACK,		/* decrease stack pointers		    */\
      EL, CODE));                                                             \
  }                                                                           \
  else                                                                        \
    DTSTACK_POPEL(STACK, EL);                                                 \
}
#endif

										/* ................................ */
#ifndef DTSTACK_PEEK
#define DTSTACK_PEEK(CHECK, CTL, STACK, POS, EL)                              \
{                                                                             \
  const long delta = (POS) * (STACK)->elsize - (STACK)->offset;               \
  if (DTSTACK_REPAGE_OR(STACK)			/* explicit repage required?		*/\
      delta < 0 ||                                                            \
      (unsigned long)delta >= (STACK)->datasize)                              \
  {                                                                           \
    CHECK(dtstack_peek,(CTL, STACK, POS, EL));                                \
  }                                                                           \
  else                                                                        \
    *(EL) = (STACK)->elmin + (unsigned int)delta;                             \
}
#endif

										/* ................................ */
#ifndef DTSTACK_POKE
#define DTSTACK_POKE(CHECK, CTL, STACK, POS, EL)                              \
{                                                                             \
  const long delta = (POS) * (STACK)->elsize - (STACK)->offset;               \
  if (DTSTACK_REPAGE_OR(STACK)			/* explicit repage required?		*/\
      delta < 0 ||                                                            \
      (unsigned long)delta >= (STACK)->datasize)                              \
  {                                                                           \
    CHECK(dtstack_poke,(CTL, STACK, POS, EL));                                \
  }                                                                           \
  else                                                                        \
    *(EL) = (STACK)->elmin + (unsigned int)delta;                             \
}
#endif

#if DTPRJ_NOPAGING == DT_1				/* paging not allowed? 				*/

#define DTSTACK_DIRECT1(CHECK, CTL, STACK, POS, EL)                           \
    {*((char **)(EL)) = (STACK)->elmin + (POS);}

#ifndef DTSTACK_PEEK1
#define DTSTACK_PEEK1(CHECK, CTL, STACK, POS, EL)                             \
  DTSTACK_DIRECT1(CHECK, CTL, STACK, POS, EL)
#endif

#ifndef DTSTACK_POKE1
#define DTSTACK_POKE1(CHECK, CTL, STACK, POS, EL)                             \
  DTSTACK_DIRECT1(CHECK, CTL, STACK, POS, EL)
#endif

#ifndef DTSTACK_LOCK1
#define DTSTACK_LOCK1(CHECK, CTL, STACK, POS, EL)                             \
  DTSTACK_DIRECT1(CHECK, CTL, STACK, POS, EL)
#endif

#ifndef DTSTACK_UNLOCK1
#define DTSTACK_UNLOCK1(CHECK, CTL, STACK, POS)
#endif

#else  									/* paging is allowed? 				*/
										/* ................................ */
#ifndef DTSTACK_PEEK1
#define DTSTACK_PEEK1(CHECK, CTL, STACK, POS, EL)                             \
{                                                                             \
  const long delta = (POS) - (STACK)->offset;                                 \
  if (DTSTACK_REPAGE_OR(STACK)			/* explicit repage required?		*/\
      delta < 0 ||                                                            \
      (unsigned long)delta >= (STACK)->datasize)                              \
  {                                                                           \
    CHECK(dtstack_peek1,(CTL, STACK, POS, EL));                               \
  }                                                                           \
  else                                                                        \
    *(EL) = (STACK)->elmin + (unsigned int)delta;                             \
}
#endif

										/* ................................ */
#ifndef DTSTACK_POKE1
#define DTSTACK_POKE1(CHECK, CTL, STACK, POS, EL)                             \
{                                                                             \
  const long delta = (POS) - (STACK)->offset;                                 \
  if (DTSTACK_REPAGE_OR(STACK)			/* explicit repage required?		*/\
      delta < 0 ||                                                            \
      (unsigned long)delta >= (STACK)->datasize)                              \
  {                                                                           \
    CHECK(dtstack_poke1,(CTL, STACK, POS, EL));                               \
  }                                                                           \
  else                                                                        \
    *(EL) = (STACK)->elmin + (unsigned int)delta;                             \
}
#endif
										/* ................................ */
#ifndef DTSTACK_LOCK1
#define DTSTACK_LOCK1(CHECK, CTL, STACK, POS, EL)                             \
{                                                                             \
  CHECK(((dtmemalloc_t *)                                                     \
    (STACK)->memalloc)->map,(CTL,                                             \
      (dtmemalloc_t *)(STACK)->memalloc,                                      \
      POS, EL, NULL));                                                        \
}
#endif

										/* ................................ */
#ifndef DTSTACK_UNLOCK1
#define DTSTACK_UNLOCK1(CHECK, CTL, STACK, POS)                               \
{                                                                             \
  CHECK(((dtmemalloc_t *)                                                     \
    (STACK)->memalloc)->unmap,(CTL,                                           \
      (dtmemalloc_t *)(STACK)->memalloc,                                      \
      POS));                                                                  \
}
#endif


#endif									/* ifndef DTPRJ_NOPAGING  			*/

#include <dtack/stack.p>






/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
