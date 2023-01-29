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
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/t.h>
#include <dtack/mem.h>
#include <dtack/stack.h>
#include <dtack/prj.h>
#include <dtack/runf.h>

DT_RCSID("test $RCSfile: dttadj.c,v $ $Revision: 1.15 $");

#define DTTADJ_FLAG_BEG  (0x0001)
#define DTTADJ_FLAG_END  (0x0002)

#define COUNT_MAX (128)
#define COUNT_UNLIKELY (9999)

typedef struct {
  double v;
  int n;
  int m;
  dtimage_t *image;
  dtntype_s4_t a[COUNT_MAX];
  dtntype_s4_t s[COUNT_MAX];
} count_arg_t;

#define USAGE "[-Dnnnn] [-op n]"

static const char *ops[] = {"all",
  "default", "file", "whole", "paged", "time", NULL};

typedef struct {
  int flag;								/* flag telling type of callback 	*/
  dtimage_t *image;						/* connected image being analyzed 	*/
  double label;							/* current connected pixel label 	*/
  void *arg;							/* passed unchanged from caller 	*/
} dttadj_callback_arg_t;

typedef 
dt_rc_e 
dttadj_callback_f(					/* callback routine provided by	    */
  dt_ctl_t *ctl,						/* user of adjacency routines	    */
  dttadj_callback_arg_t *arg,
  dtxy_t x1,
  dtxy_t y1,
  dtxy_t x2,
  dtxy_t y2);

typedef struct {
  dtimage_t *image;
  dttadj_callback_f *adj_callback;
  void *adj_arg;
} my_deliver_arg_t;

/*..........................................................................
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#ifdef NONO
#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: adj_floo.c,v $ $Revision: 1.15 $");
#include <dtack/os.h>
#include <dtack/mem.h>
#endif

#define NTYPE dt_ntype_b08_t
#define NTYPE_E DT_NTYPE_B08
#define NTYPE_PTR b08

static struct {
  dt_ctl_t *ctl;
  dttadj_callback_f *callback;
  dttadj_callback_arg_t *arg;
  dtimage_t *input;
  NTYPE nv;
} global;

#define PIXELWRITE(X1, X2, Y, NV)                                             \
{                                                                             \
  dtxy_t tx;                                                                  \
  NTYPE *p;                                                                   \
  DT_C(global.callback,(global.ctl, 	/* call user's function			    */\
    global.arg,	                                                              \
    X1, Y, X2, Y))                                                            \
  DTIMAGE_GETROW2(DT_C, global.ctl,                                           \
    global.input, Y, &p);                                                     \
  for (tx=X1; tx<=X2; tx++)                                                   \
    p[tx] = global.nv;					/* set to invisible					*/\
  DTIMAGE_PUTROW2(DT_C, global.ctl,                                           \
    global.input, Y);                                                         \
}

/* ........................................................................ */

/******************************************************
 * FLOOD.C - optimized flood visit
 *
 * This algorithm visits once each all 4-way connected
 * interior points on a finite plane which share a 
 * common property, given a starting point which has 
 * the property and a function which can determine 
 * whether any given point also has it.  The common 
 * points need not form a convex region, that is, 
 * islands and peninsulas bounded by points which do 
 * not share the common property may exist within the
 * region of points which do.
 *
 * for ANSI C
 *
 * by Anton Treuenfels
 * last revision:  04/12/94
 * published in C/C++ Users Journal 12:8 Aug 1994
 *
 * references:
 * Lieberman, Henry, "How To Color In A Coloring Book",
 *   in Computer Graphics, Vol. 12, No. 3, Aug 1978
 * Polik, William F., "Area Filling Algorithms",
 *   in Computer Language, Vol. 3, No. 5, May 1986
 * Wilton, Richard, "PC & PS/2 Video Systems",
 *   Microsoft Press, 1987
 *****************************************************/

/****************************
 * HEADER SECTION - FLOOD.H *
 ****************************/

#ifndef SEEN_FLOOD
#define SEEN_FLOOD

#endif

/********************************************
 * USRDEF.H - additions to <stddef.h>
 *
 * last revised:   02/28/93
 ********************************************/

#ifndef SEEN_USRDEF
#define SEEN_USRDEF

/* condition codes */

#define DTTADJ_FALSE   0
#define DTTADJ_TRUE    (!(DTTADJ_FALSE))

#define OK		0
#define FAIL	-1

/* synonym types */

typedef unsigned char Byte;
typedef int Boolean;

#endif

/**************************
 * CODE SECTION - FLOOD.C *
 **************************/
/* line shadow */

typedef struct shdw {
	struct shdw *next;      /* forward link */
	int lft, rgt;           /* endpoints */
	int row, par;           /* row and parent row */
	Boolean ok;             /* valid flag */
} shadow;

/* shadow variables */

static int currRow;         /* current row */
static shadow *seedShadow;  /* current shadow */
static shadow *rowHead;     /* current row shadows */
static shadow *pendHead;    /* other pending shadows */
static shadow *freeHead;    /* unused shadow nodes */

/*****************************************************/

/* release shadow nodes */

static void freeshadows(shadow *s) {

	shadow *t;

	while (s) {
		t = s->next;
		free(s);
		s = t;
	}
}

/* make new shadow */

static void newshadow(int slft, int srgt,
		int srow, int prow) {

	shadow *newshad;

	if ((newshad = freeHead) != NULL)
		freeHead = freeHead->next;
	else if ((newshad = (shadow *)malloc(sizeof(shadow))) == NULL) {
		freeshadows(rowHead);
		freeshadows(pendHead);
		dt_err(NULL, "dttadj_flood_newshadow", "malloc failure");
		exit(-1);
	}

	newshad->lft = slft;  newshad->rgt = srgt;
	newshad->row = srow;  newshad->par = prow;
	newshad->ok = DTTADJ_TRUE;
	newshad->next = pendHead;
	pendHead = newshad;
}

/* make list of all shadows on one row */

static void makerow(void) {

	shadow *s, *t, *u;

	t = pendHead;
	pendHead = NULL;
	while ((s = t) != NULL) {
		t = t->next;
		if (s->ok) {
			if (rowHead == NULL) {
				currRow = s->row;
				s->next = NULL;
				rowHead = s;
			}
			else if (s->row == currRow) {
				if (s->lft <= rowHead->lft) {
					s->next = rowHead;
					rowHead = s;
				}
				else {
					for (u = rowHead; u->next; u = u->next)
						if (s->lft <= u->next->lft)
							break;
					s->next = u->next;
					u->next = s;
				}
			}
			else {
				s->next = pendHead;
				pendHead = s;
			}
		}
		else {
			s->next = freeHead;
			freeHead = s;
		}
	}
}

/* make new shadow(s) that don't overlap lines */

static void clipshadow(int lft, int rgt, int row,
		 shadow *line) {

	if (lft < (line->lft - 1))
		newshadow(lft, line->lft - 2, row, line->row);
	if (rgt > (line->rgt + 1))
		newshadow(line->rgt + 2, rgt, row, line->row);
}

/* discard shadow segments which overlap lines */

static void removeoverlap(shadow *rw) {

	shadow *chld;

	for (chld = pendHead; chld->row != rw->par;
			 chld = chld->next)
		;

	clipshadow(chld->lft, chld->rgt, chld->row, rw);
	if (rw->rgt > (chld->rgt + 1))
		rw->lft = chld->rgt + 2;
	else
		rw->ok = DTTADJ_FALSE;
	chld->ok = DTTADJ_FALSE;
}

/* make shadows of one child line */

static void makeshadows(int lft, int rgt) {

	shadow *p;

	if (currRow > seedShadow->par) {
		newshadow(lft, rgt, currRow + 1, currRow);
		clipshadow(lft, rgt, currRow - 1, seedShadow);
	}
	else if (currRow < seedShadow->par) {
		newshadow(lft, rgt, currRow - 1, currRow);
		clipshadow(lft, rgt, currRow + 1, seedShadow);
	}
	else {
		newshadow(lft, rgt, currRow + 1, currRow);
		newshadow(lft, rgt, currRow - 1, currRow);
	}

	for (p = rowHead; p && (p->lft <= rgt); p = p->next)
		if (p->ok)
			removeoverlap(p);
}

/*..........................................................................*
 *..........................................................................*/


#define CHECK_INTERIOR(X, Y) \
{ \
  NTYPE v; \
  DTIMAGE_GETPIXN(DT_C, global.ctl, \
	global.input, \
    X, Y, NTYPE, &v); \
  is_interior = (v == global.arg->label); \
}

/*..........................................................................*
 * visit all child lines found within one shadow 
 *..........................................................................*/

static void visitshadow(void) 
{
  DT_F("dttadj_flood_visitshadow");
  dt_ctl_t *ctl = global.ctl;
  int is_interior;
  dt_rc_e rc;

	int col, lft;
	if (currRow < 0 ||					/* row outside image extent? 		*/
		currRow >= global.input->ye)
	  return;
	for (col = seedShadow->lft; 
		 col <= seedShadow->rgt;
		 col++) 
	{
        CHECK_INTERIOR(col, currRow)
		if (is_interior)
		{
			if ((lft = col) == seedShadow->lft) 
			{
			  do {
				if (--lft >= 0)
      			  CHECK_INTERIOR(lft, currRow)
				else
				  is_interior = 0;
			  } while (is_interior);
			  lft++;
			}
			do {
			  if (++col < global.input->xe)
      			CHECK_INTERIOR(col, currRow)
			  else
				is_interior = 0;
			} while (is_interior);
			PIXELWRITE(lft, col-1, currRow, global.nv);
			makeshadows(lft, col-1);
		}
	}
cleanup:
  return;
}
/*..........................................................................*
 *..........................................................................*/


/* flood visit */

static void doflood(int seedx, int seedy)
{
	pendHead = rowHead = freeHead = NULL;
	newshadow(seedx, seedx, seedy, seedy);
	while (pendHead) {
		makerow();
		while (rowHead) {
			seedShadow = rowHead;
			rowHead = rowHead->next;
			if (seedShadow->ok)
				visitshadow();
			seedShadow->next = freeHead;
			freeHead = seedShadow;
		}
	}
	freeshadows(freeHead);
}

/* execute flood visit through guard function */

int flood(int seedx, int seedy)
{
	doflood(seedx, seedy);
	return(OK);
}

/* ........................................................................ */
/*
 * flood: set the pixel at (x,y) and all of its 4-connected neighbors
 * with the same pixel value to the new pixel value nv.
 * A 4-connected neighbor is a pixel above, below, left, or right of a pixel.
 */

static
dt_rc_e
dttadj_flood_b08(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t x0,							/* seed point 						*/
  dtxy_t y0,
  NTYPE nv,								/* new pixel value 					*/
  dttadj_callback_f callback,
  dttadj_callback_arg_t *arg)
{
  DT_F("dttadj_flood_b08_better");
  global.ctl = ctl;
  global.callback = callback;
  global.arg = arg;
  global.input = input;
  global.nv = nv;
  DTIMAGE_GETPIXN(DT_Q, ctl,			/* flood value from seed point 		*/
    input,
    x0, y0, NTYPE, &global.arg->label);

  if (flood(x0, y0) != OK)
    return dt_err(ctl, F, 
      "flood returned not OK");
  return DT_RC_GOOD;
}

/*__________________________________________________________________________
 * connect one blob
 *__________________________________________________________________________*/

#ifdef NONO

static
dt_rc_e
dttadj_flood1(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t x, 
  dtxy_t y,
  double invisible,
  unsigned long flags,
  dttadj_callback_f callback,
  void *arg)
{
  DT_F("dttadj_flood1");
  dttadj_callback_arg_t callback_arg;
  dt_rc_e rc;

  rc = dtimage_check_1(ctl,				/* check simple stuff				*/
    input, "input", NULL, F);
  if (rc != DT_RC_GOOD)
    return rc;

  if (input->row[y].b08[x] ==			/* already invisible?			    */
      invisible)
    return DT_RC_GOOD;

  callback_arg.arg = arg;
  callback_arg.flag =					/* start blob						*/
    DTTADJ_FLAG_BEG;

  DT_G(dttadj_flood_b08,(ctl,
    input, x, y, (NTYPE)invisible,
    callback, 
    &callback_arg));

  if (rc == DT_RC_GOOD)
  {
    callback_arg.flag =					/* finish blob					    */
      DTTADJ_FLAG_END;
    DT_G(callback,(ctl, &callback_arg,
      0, 0, 0, 0));
  }

  if (rc == DT_RC_STOP)					/* callback said to stop?		    */
    rc = DT_RC_GOOD;

  return rc;
}
#endif

/*__________________________________________________________________________
 * connect binary image
 *__________________________________________________________________________*/

static
dt_rc_e
dttadj_flood(
  dt_ctl_t *ctl,
  dtimage_t *input,
  double invisible,
  unsigned long flags,
  dttadj_callback_f callback,
  void *arg)
{
  DT_F("dttadj_flood");
  int x, y;
  int flag;
  const dt_ntype_e ntypes[] = {
    NTYPE_E, DT_NTYPE_NULL};
  const NTYPE invisible_ntype =
    (NTYPE)invisible;
  dttadj_callback_arg_t callback_arg;
  int got = 0;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input, "input",
    ntypes, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  callback_arg.arg = arg;
  flag = DTTADJ_FLAG_BEG;				/* no old blob to finish		    */

  for (y=0; y<input->ye; y++)			/* search pattern				    */
  {
	NTYPE *p;
    DTIMAGE_GETROW2(DT_C, ctl,
      input, y, &p);
    got = 1;
	for (x=0; x<input->xe; x++)
	{
	  if (p[x] != invisible_ntype)		/* pixel not invisible?			    */
	  { 
		callback_arg.flag = flag;
		flag =							/* have old blob to finish		    */
	      DTTADJ_FLAG_END |
	      DTTADJ_FLAG_BEG;
		DT_C(dttadj_flood_b08,(ctl,
          input, x, y, invisible_ntype,
          callback, &callback_arg));
	  }
	}
    DTIMAGE_PUTROW2(DT_C, ctl,
      input, y);
    got = 0;
  }

cleanup:
  if (got)
    DTIMAGE_PUTROW2(DT_I, ctl, input, y);
  if (rc == DT_RC_GOOD)
  {
    callback_arg.flag =					/* don't start any new blob		    */
      DTTADJ_FLAG_END;
    if (flag & DTTADJ_FLAG_END)	/* any blob to finish?			    */
      DT_I(callback,(ctl,				/* finish last old blob			    */ 
        &callback_arg,
        0, 0, 0, 0))
    if (rc == DT_RC_STOP)
      rc = DT_RC_GOOD;
  }

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  return rc;
}


/*..........................................................................
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#ifdef NONO
#include <dtack/base.h>
DT_RCSID("source/high $RCSfile: adj_fill.c,v $ $Revision: 1.15 $");

#include <dtack/os.h>
#include <dtack/mem.h>
#include <dtack/stack.h>
#endif

#define S1 char
#define S2 short
#define S4 dtntype_s4_t
#define S8 double

typedef int fillxy_t;					/* image indexes				    */
										/* this has to be int not unsigned 	*/
										/*   because of the way the rows 	*/
										/*   are traversed 					*/

typedef struct {
  fillxy_t y;
  fillxy_t xl;
  fillxy_t xr;
  int dy;
} stack_el_t;

#define ELSIZE (sizeof(stack_el_t))

/*__________________________________________________________________________
 *__________________________________________________________________________*/

#define STACK_CHECK \
  if (flow == DTSTACK_CODE_OVERFLOW) \
    return dt_err(ctl, F, "pixel stack overflow"); \
  else \
  if (flow == DTSTACK_CODE_UNDERFLOW) \
    return dt_err(ctl, F, "pixel stack underflow");


#define PUSH(Y, XL, XR, DY)				/* put new segment on stack 		*/\
  if (Y+(DY)>=ymin && Y+(DY)<=ymax) \
  { \
    stack_el_t *el; \
    DTSTACK_PUSH(DT_Q, ctl, stack, \
      (void **)&el, &flow); \
    STACK_CHECK; \
    el->y = Y; \
    el->xl = XL; \
    el->xr = XR; \
    el->dy = DY; \
    if (++depth > max_depth) \
      max_depth = depth; \
  }

#define POP(Y, XL, XR, DY)				/* pop segment off stack 			*/\
  { \
    stack_el_t *el; \
    DTSTACK_POP(DT_Q, ctl, stack, \
      (void **)&el, &flow); \
    STACK_CHECK; \
    Y  = el->y + (DY = el->dy); \
    XL = el->xl; \
    XR = el->xr; \
    depth--; \
  }

/* ........................................................................ */

typedef unsigned char pixel_t;

#define CMP(P, X, V) ((size == 1 && ((S1 *)(P))[(X)] != V ## 1) || \
                      (size == 2 && ((S2 *)(P))[(X)] != V ## 2) || \
                      (size == 4 && ((S4 *)(P))[(X)] != V ## 4) || \
                      (size == 8 && ((S8 *)(P))[(X)] != V ## 8))

/* ........................................................................
 * if searching for seed point x0 and y0 are returned the seed point
 * if no seed point is found, then y0 is set to input->ye
 * adapted from Paul Heckbert "Graphics Gems", Academic Press, 1990
 * ........................................................................ */

static
dt_rc_e
dttadj_fill_size(
  dt_ctl_t *ctl,
  dtimage_t *input,
  const fillxy_t xmin,
  const fillxy_t ymin,
  const fillxy_t xmax,
  const fillxy_t ymax,
  dtxy_t *x0,							/* seed point 						*/
  dtxy_t *y0,
  int search,							/* to search for seed?			    */
  const pixel_t * const nv,				/* new pixel value 					*/
  dtstack_t *stack,
  unsigned long *pmax_depth,
  dttadj_callback_f callback,
  dttadj_callback_arg_t *arg)
{
  DT_F("dttadj_fill_size");
  fillxy_t x = *x0;
  fillxy_t y = *y0;
  fillxy_t z;
  const int size = DT_CAST(int, dt_ntype_sizeof[input->ntype]);
  dtstack_code_e flow = DTSTACK_CODE_OK;
  fillxy_t l, x1, x2;

    const S1 nv1 =						/* background pixel value			*/
      size == 1? *((S1 *)nv): 0;
    const S2 nv2 = 
      size == 2? *((S2 *)nv): 0;
    const S4 nv4 = 
      size == 4? *((S4 *)nv): 0;
    const S8 nv8 = 
      size == 8? *((S8 *)nv): 0;

  pixel_t *p;
  fillxy_t dy;
  unsigned long depth = 0;
  unsigned long max_depth = 0;

  if (search)							/* supposed to search for seed? 	*/
  {
    for (; y<input->ye; y++)			/* search pattern				    */
    {
	  DTIMAGE_GETROW2(DT_Q, ctl,
        input, y, &p);
  	  for (; x<input->xe; x++)
	    if (CMP(p, x, nv))				/* pixel not invisible?			    */
  	      break;
      if (x < input->xe)
        break;
	  x = 0;
    }
	*x0 = x;							/* return the seed point 			*/
    *y0 = y;
    if (y == input->ye)
      return DT_RC_GOOD;
  }
  else									/* not supposed to search? 			*/
    DTIMAGE_GETROW2(DT_Q, ctl,
      input, y, &p);

  DTIMAGE_UNMAPROW2(DT_Q, ctl,			/* may not need this row any more 	*/
    input, y);

  if (!CMP(p, x, nv) ||					/* already on an invisible pixel?	*/
      x < xmin ||							/* or out of bounds?			    */
      x > xmax ||
      y < ymin ||
      y > ymax)
    return DT_RC_GOOD;

  arg->image = input;					/* connected image being analyzed 	*/
  DTIMAGE_GETPIX1(DT_Q, ctl,			/* current connected pixel label 	*/
    input, x, y, &arg->label);

  {
    const S1 ov1 =						/* pixel value at seed point		*/
      size == 1? ((S1 *)p)[x]: 0;
    const S2 ov2 = 
      size == 2? ((S2 *)p)[x]: 0;
    const S4 ov4 = 
      size == 4? ((S4 *)p)[x]: 0;
    const S8 ov8 = 
      size == 8? ((S8 *)p)[x]: 0;

    PUSH(y, x, x, 1);					/* needed in some cases 			*/
    PUSH(y+1, x, x, -1);				/* seed segment (popped 1st) 		*/

    while (!DTSTACK_ISEMPTY(stack))
    {
										/* pop segment off stack and fill a	*/
										/* neighboring scan line 			*/
	  POP(y, x1, x2, dy);
      DTIMAGE_GETROW2(DT_Q, ctl,
        input, y, &p);
										/* segment of scan line y-dy for	*/
										/* x1<=x<=x2 was previously filled,	*/
										/* now explore adjacent pixels in	*/
										/* scan line y						*/
	  for (x=x1; x>=xmin; x--)
	    if (CMP(p, x, ov))
		  break;
	  if (x >= x1)
        goto skip;
	  l = x + 1;
	  if (l < x1)						/* leak on left? 					*/
        PUSH(y, l, x1-1, -dy);
	  x = x1 + 1;
	  do 
      {
	    for (; x<=xmax; x++)
  	      if (CMP(p, x, ov))
		    break;

		if (callback != NULL)
          DT_Q(callback,(ctl, arg,		/* call user's function			    */
            l, y, x-1, y));

        if (size == 1)					/* zap the input pixels we used 	*/
          for (z=l; z<x; z++)	
            ((S1 *)p)[z] = nv1;
        else
        if (size == 2) 
          for (z=l; z<x; z++)	
            ((S2 *)p)[z] = nv2;
        else
        if (size == 4) 
          for (z=l; z<x; z++)	
            ((S4 *)p)[z] = nv4;
        else
        if (size == 8) 
          for (z=l; z<x; z++)	
            ((S8 *)p)[z] = nv8;

	    PUSH(y, l, x-1, dy);
	    if (x > x2+1)					/* leak on right? 					*/
          PUSH(y, x2+1, x-1, -dy);
skip:   for (x++; x<=x2; x++)
  	      if (!CMP(p, x, ov))
		    break;
	    l = x;
  	  } while (x<=x2);

      DTIMAGE_PUTROW2(DT_Q, ctl,
        input, y);
	}
  }

  *pmax_depth = DT_MAX(*pmax_depth,
    max_depth);

  return DT_RC_GOOD;
}

/*__________________________________________________________________________
 * connect one blob
 *__________________________________________________________________________*/

static
dt_rc_e
dttadj_fill_one(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t x, 
  dtxy_t y,
  double invisible,
  unsigned long flags,
  dttadj_callback_f callback,
  void *arg)
{
  DT_F("dttadj_fill_one");
  dtstack_t stack;
  dttadj_callback_arg_t callback_arg;
  unsigned long max_depth = 0;
  dt_ntype_f2_t nv;
  dt_rc_e rc;

  rc = dtimage_check_1(ctl,				/* check simple stuff				*/
    input, "input", NULL, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DTIMAGE_GETPIX1(DT_Q, ctl,			/* check the seed point 			*/
    input, x, y, &nv);
  if (nv == invisible)					/* already invisible?	    		*/
    return DT_RC_GOOD;

  DT_Q(dtstack_init,(ctl,				/* init stack						*/
    &stack, ELSIZE, 
    0,									/* default initial depth			*/
    0,									/* default max depth			    */
    F));

  callback_arg.arg = arg;
  callback_arg.flag =					/* start blob						*/
    DTTADJ_FLAG_BEG;

  DT_C_1D(dt1d_cast,(
    &invisible, 1, DT_NTYPE_F2,
    &nv, 1, input->ntype));
  DT_C(dttadj_fill_size,(ctl,		/* fill from this seed				*/
    input, 
    0, 0,								/* use entire input image 			*/
    DT_CAST(int, input->xe-1), 
    DT_CAST(int, input->ye-1),
    &x, &y, 0, (pixel_t *)&nv,
    &stack, &max_depth,
    callback, &callback_arg));

  callback_arg.flag =					/* finish blob					    */
    DTTADJ_FLAG_END;
  if (callback != NULL)
    DT_C(callback,(ctl, &callback_arg,
      0, 0, 0, 0));

cleanup:
  if (rc == DT_RC_STOP)					/* callback said to stop?		    */
    rc = DT_RC_GOOD;

  DT_I(dtstack_free,(ctl, &stack, F));	/* free stack					    */

  return rc;
}


/*__________________________________________________________________________
 * connect binary image
 *__________________________________________________________________________*/

static
dt_rc_e
dttadj_fill2(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t xmin,
  dtxy_t ymin,
  dtxy_t xmax,
  dtxy_t ymax,
  double invisible,
  unsigned long flags,
  dttadj_callback_f callback,
  void *arg)
{
  DT_F("dttadj_fill2");
  dtxy_t x, y;
  int flag;
  dttadj_callback_arg_t callback_arg;
  dtstack_t stack;
  unsigned long max_depth = 0;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    input, "input",
    NULL, DTIMAGE_FLAG_PAGED, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_Q(dtstack_init,(ctl,				/* init stack						*/
    &stack, ELSIZE,
    0,									/* default initial depth			*/
    0,									/* default max depth			    */
    F));

  callback_arg.arg = arg;
  flag = DTTADJ_FLAG_BEG;			/* no old blob to finish		    */

  x = y = 0;
  do {
    dt_ntype_f2_t nv;
    DT_C_1D(dt1d_cast,(
      &invisible, 1, DT_NTYPE_F2,
      &nv, 1, input->ntype));
	callback_arg.flag = flag;
    DT_C(dttadj_fill_size,(ctl,	/* search for seed and fill			*/
      input, 
      DT_CAST(int, xmin),
      DT_CAST(int, ymin),
      DT_CAST(int, xmax-1), 
      DT_CAST(int, ymax-1),
      &x, &y, 1, 
      (pixel_t *)&nv,
      &stack, &max_depth,
      callback, &callback_arg));
	if (y < input->ye)
	flag =								/* next time have blob to finish	*/
      DTTADJ_FLAG_END |
	  DTTADJ_FLAG_BEG;
  } while (y < input->ye);

  dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
    "stack maximum depth %lu,"
    " (%lu bytes of %lu allocated)",
    max_depth, max_depth*ELSIZE,
    ((dtmemalloc_t *)stack.memalloc)->
      total);

cleanup:
  if (rc == DT_RC_GOOD &&				/* everything is ok? 				*/
	  flag & DTTADJ_FLAG_END)		/* any blob to finish?			    */
  {
    callback_arg.flag =					/* don't start any new blob		    */
      DTTADJ_FLAG_END;
    DT_I(callback,(ctl,					/* finish last old blob			    */ 
      &callback_arg,
      0, 0, 0, 0))
  }

  if (rc == DT_RC_STOP)
    rc = DT_RC_GOOD;

  DT_I(dtstack_free,(ctl, &stack, F));	/* free stack					    */

  return rc;
}


/*__________________________________________________________________________
 * connect binary image
 *__________________________________________________________________________*/

static
dt_rc_e
dttadj_fill(
  dt_ctl_t *ctl,
  dtimage_t *input,
  double invisible,
  unsigned long flags,
  dttadj_callback_f callback,
  void *arg)
{
  DT_F("dttadj_fill");
  DT_Q(dttadj_fill2,(ctl,
    input,
    0, 0, input->xe, input->ye,			/* use entire image 				*/
	invisible, flags,
    callback, arg));
  return DT_RC_GOOD;
}
/*..........................................................................
| NAME
| Introduction - intro
| 
| DESCRIPTION
| As yet undocumented.
| 
| RETURN VALUES
| Function return value will be DT_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

/*__________________________________________________________________________
 * connect binary image
| based on run technology
| run technology delivers blobs, changed into runs per standard adj interface
 *__________________________________________________________________________*/
#ifdef NONO
static
dt_rc_e
dttadj_runf2(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t xmin,
  dtxy_t ymin,
  dtxy_t xmax,
  dtxy_t ymax,
  double invisible,
  unsigned long flags,
  dttadj_callback_f callback,
  void *arg)
{
  return DT_RC_GOOD;
}
#endif
/*__________________________________________________________________________
 * connect binary image
 *__________________________________________________________________________*/

static
dt_rc_e
dttadj_runf(
  dt_ctl_t *ctl,
  dtimage_t *image,
  double invisible,
  unsigned long flags,
  dttadj_callback_f callback,
  void *arg)
{
  DT_F("dttadj_runf");
  dttadj_callback_arg_t callback_arg;
  int flag;
  dtrunf_t run;
  const dtrunf_elem_t *p;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, "image",
    dtrun_ntype_list, 0, F);
  if (rc != DT_RC_GOOD)
    return rc;

  DT_Q(dtrunf_alloc_struct,(ctl,		/* get space for runs			    */
    image, &run));

  DT_C(dtrunf_scan,(ctl,				/* scan image for runs			    */
	image, &run, invisible, -1));
  DT_C(dtrunf_label,(ctl,				/* make run labels				    */
    image, flags, &run));

  callback_arg.image = image;
  callback_arg.arg = arg;
  flag = DTTADJ_FLAG_BEG;				/* no old blob to finish		    */

  for (p=run.head; p!=NULL; p=p->p)		/* traverse blob links			    */
  {
	const dtxy_t x0 = p->x;
	if (p->h == p)						/* this run is a blob head?		    */
	{
      DTIMAGE_GETPIX1(DT_C,				/* get value of blob's label 		*/
        ctl, image, x0, p->y,
        &callback_arg.label);

	  callback_arg.flag = flag;
      flag =							/* have old blob to finish		    */
	    DTTADJ_FLAG_END |
	    DTTADJ_FLAG_BEG;
	}
    DT_C(callback,(ctl,					/* process all pixels in run	    */
      &callback_arg,
      x0, p->y, x0+p->l, p->y));
  }

  callback_arg.flag =					/* don't start any new blob		    */
    DTTADJ_FLAG_END;
  if (flag & DTTADJ_FLAG_END)			/* any blob to finish?			    */
    callback(ctl, &callback_arg, 		/* finish last old blob			    */
      0, 0, 0, 0);

cleanup:
  if (rc == DT_RC_STOP)					/* callback said to stop? 			*/
    rc = DT_RC_GOOD;					/* not considered an error 			*/

  if (rc == DT_RC_GOOD)
    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
      "segments %ld, runs %ld, blobs %ld",
	  (long)run.segments,
      (long)run.runs,
      (long)run.blobs);

  DT_I(dtrunf_free_all,(ctl,			/* free all associated space	    */
    image, &run));						

  return rc;
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttadj_run_deliver(	
  dt_ctl_t *ctl,
  dtrun_t *run,
  dtrun_blob_t *blob,
  void *arg)
{
  DT_F("dttadj_run_deliver");
  my_deliver_arg_t *my_deliver_arg =	/* arg prepared by adj_run 			*/
    (my_deliver_arg_t *)arg;

  dtimage_t *image =					/* connected image being analyzed 	*/
    my_deliver_arg->image;

  dttadj_callback_f				/* adj callback routine we call 	*/
    *adj_callback =
      my_deliver_arg->adj_callback;

  dttadj_callback_arg_t adj_arg;	/* arg expected by adj callback 	*/

  DT_MEMSET(&adj_arg, 0, sizeof(adj_arg));
  adj_arg.image = image;				/* pass these to adj callback 		*/
  adj_arg.arg = 
    my_deliver_arg->adj_arg;

  {
    dtrun_id_t p;
    adj_arg.flag =						/* at first no old blob to finish   */
      DTTADJ_FLAG_BEG;
	adj_arg.label = blob->label;
	
    p = blob->phead;
	do {
	  dtrun_elem_t *e;
	  DTRUN_PEEK(DT_Q,					/* access next run in blob chain  	*/
        ctl, run, p, &e);
	  DT_Q(adj_callback,(ctl,			/* process all pixels in run	    */
        &adj_arg,
        e->x, e->y, 
        e->x+e->l, e->y));
	  adj_arg.flag = 0;					/* next run does not start blob		*/
	  p = e->p;							/* point to next run in chain	    */
	} while(p != DTRUN_NOID);

	adj_arg.flag =						/* have old blob to finish		    */
      DTTADJ_FLAG_END;
    DT_Q(adj_callback,(ctl,				/* finish blob			   			*/
      &adj_arg,
      0, 0, 0, 0));
  }

  return DT_RC_GOOD;
}

/*__________________________________________________________________________
| connect binary image
| based on run technology
| run technology delivers blobs, changed into runs per standard adj interface
 *__________________________________________________________________________*/

static
dt_rc_e
dttadj_run_run(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtxy_t xmin,
  dtxy_t ymin,
  dtxy_t xmax,
  dtxy_t ymax,
  double invisible,
  unsigned long flags,
  dttadj_callback_f callback,
  void *arg)
{
  DT_F("dttadj_run_run");
  my_deliver_arg_t my_deliver_arg;
  dtrun_t run;
  dt_rc_e rc;

  rc = dtimage_check_1f(ctl,			/* check simple stuff				*/
    image, "image",
    dtrun_ntype_list, 
    DTIMAGE_FLAG_PAGED, F);		
  if (rc != DT_RC_GOOD)
    return rc;

  DT_Q(dtrun_init,(ctl, &run));			/* get space for runs			    */

  DT_C(dtimage_run_scan,(ctl,			/* scan image for runs			    */
	image, &run,
	xmin, ymin, xmax, ymax,
    invisible, -1));

  my_deliver_arg.image = image;
  my_deliver_arg.adj_arg = arg;
  my_deliver_arg.adj_callback = callback;

  DT_C(dtimage_run_label,(ctl,			/* make blobs out of runs		    */
    image, &run, flags,
    dttadj_run_deliver, 
    &my_deliver_arg));

cleanup:
  if (rc != DT_RC_BAD)
    dt_dbg(ctl, F, DT_DBG_MASK_COMPLEX,
      "runs %ld, blobs %ld",
      (long)run.runs,
      (long)run.blobs);

  DT_I(dtrun_uninit,(ctl, &run));		/* free all associated space	    */

  return rc;
}

/*__________________________________________________________________________
 * connect binary image
 *__________________________________________________________________________*/
#ifdef NONO
static
dt_rc_e
dttadj_run_nokeep(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t xmin,
  dtxy_t ymin,
  dtxy_t xmax,
  dtxy_t ymax,
  double invisible,
  unsigned long flags,
  dttadj_callback_f callback,
  void *arg)
{
  DT_F("dttadj_run_nokeep");
  
  DT_Q(dttadj_run_run,(ctl, input,
    xmin, ymin, xmax, ymax, 
    invisible, flags, 
    callback, arg));

#ifdef DTTADJ_DEFAULT_IS_FILL
  DT_Q(dttadj_fill2,(ctl,
    input, 
    xmin, ymin, xmax, ymax, 
    invisible, flags, 
    callback, arg));
#endif

  return DT_RC_GOOD;
}
#endif
/*__________________________________________________________________________
 * connect binary image
 * this function call is obsolete, kept only because of test programs
 * run_run uses the blob machinery anyway
 *__________________________________________________________________________*/

#ifdef NONO
static
dt_rc_e
dttadj_run(
  dt_ctl_t *ctl,
  dtimage_t *input,
  dtxy_t xmin,
  dtxy_t ymin,
  dtxy_t xmax,
  dtxy_t ymax,
  double invisible,
  unsigned long flags,
  dttadj_callback_f callback,
  void *arg)
{
  DT_F("dttadj_run");

  DT_Q(dttadj_run_run,(ctl, input,
    xmin, ymin, xmax, ymax, 
    invisible, flags, 
    callback, arg));

#ifdef DTTADJ_DEFAULT_IS_FILL
  dtimage_t dup;
  dt_rc_e rc;
  DT_Q(dtimage_dup,(ctl, input, &dup));
  DT_G(dttadj_fill2,(ctl,
    &dup,
    xmin, ymin, xmax, ymax, 
    invisible, flags, 
    callback, arg));
  DT_I(dtimage_free,(ctl, &dup)); 
  return rc;
#endif

  return DT_RC_GOOD;
}
#endif

/*..........................................................................*/
/*..........................................................................*/

static 
dt_rc_e
dttadj_make(
  dt_ctl_t *ctl,
  dtimage_t *source,
  dtimage_t *expect,
  dtxy_t ncols,
  dtxy_t nrows,
  dt_ntype_e ntype,
  dt_ntype_b08_t *source_data,
  dt_ntype_b08_t *expect_data,
  count_arg_t *count_arg)
{
  DT_F("dttadj_make");
  dtxy_t y;

  DT_Q(dtimage_create,(ctl,				/* make image to hold given data    */
    source, ncols, nrows, ntype));
 
  for (y=0; y<nrows; y++)				/* copy given data row by row		*/
  {
    void *p;
    DTIMAGE_MAPROW(ctl, source, y, &p);
	DT_Q_1D(dt1d_cast,(
      source_data, ncols, DT_NTYPE_B08,
      p, ncols, ntype));
    DTIMAGE_PUTROW(ctl, source, y);
    source_data += ncols;
  }

  count_arg->image = source;			/* callback needs source image	    */

  if (expect_data != NULL)
  {
    DT_Q(dtimage_create,(ctl,			/* make image to hold expected data */
      expect, ncols, nrows, ntype));
 
    for (y=0; y<nrows; y++)				/* copy given data row by row		*/
    {
      void *p;
      DTIMAGE_MAPROW(ctl, expect, y, &p);
	  DT_Q_1D(dt1d_cast,(
        expect_data, ncols, DT_NTYPE_B08,
        p, ncols, ntype));
      DTIMAGE_PUTROW(ctl, expect, y);
      expect_data += ncols;
    }
  }

  return DT_RC_GOOD;
}

/*..........................................................................*/
static 
dt_rc_e
dttadj_init(
  dt_ctl_t *ctl,
  count_arg_t *count_arg)
{
  int i;

  memset(count_arg, 0, 
    sizeof(*count_arg));

  for (i=0; i<COUNT_MAX; i++)			/* clear output					    */
  {
    count_arg->a[i] = COUNT_UNLIKELY;
    count_arg->s[i] = COUNT_UNLIKELY;
  }

  count_arg->n = 0;
  count_arg->m = 0;						/* 0 means unlimited max		    */
  count_arg->v = -1.0;					/* don't search for anything	    */

  return DT_RC_GOOD;
}
/*..........................................................................*/
static 
dt_rc_e
dttadj_compare_and_free(
  dt_ctl_t *ctl,
  dtimage_t *image,
  dtimage_t *expect,
  char *F,
  char *desc)
{
  long cmp;

  DT_Q(dtimage_cmp,(ctl,				/* compare images				  	*/
    image, expect, 0, 
    &cmp, NULL, NULL));
  dt_assert(ctl, cmp == 0,
    F, "%s result image %ld (0)",
    desc, cmp);
  DT_Q(dtimage_free,(ctl,				/* free images					    */
    expect));
  DT_Q(dtimage_free,(ctl,
    image));

  return DT_RC_GOOD;
}

/*..........................................................................*/


dt_rc_e
DTCONFIG_API1
dttadj_check(
  dt_ctl_t *ctl,
  dtimage_t *source,
  unsigned int expect_n,
  dtntype_s4_t *expect_a,
  dtntype_s4_t *expect_s,
  count_arg_t *count_arg,
  char *F,
  char *desc)
{
  long cmp;
  unsigned int n = DT_MAX(expect_n-1, (unsigned int)count_arg->n);

  DT_Q_1D(dt1d_sort,(DTNTYPE_S4,
    count_arg->a, count_arg->n));
  DT_Q_1D(dt1d_sort,(DTNTYPE_S4,
    count_arg->s, count_arg->n));

  if (ctl->dbg_mask & DT_DBG_MASK_OBJECT)
  {
    unsigned int i;
    dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
      "%4s  %6s (%6s)   %6s (%6s)",
      "    ",
      " size ", "expect",
      "  sum ", "expect");
	for (i=0; i<n; i++)
	{
	  dt_dbg(ctl, F, DT_DBG_MASK_OBJECT,
        "%4d. %6d (%6d)   %6d (%6d)",
        i,
		i<(unsigned)count_arg->n?count_arg->a[i]:(-1), i<expect_n-1?expect_a[i]:(-1),
		i<(unsigned)count_arg->n?count_arg->s[i]:(-1), i<expect_n-1?expect_s[i]:(-1));
	}
  }

  dt_assert(ctl,
    (unsigned)count_arg->n==expect_n-1, F, 
    "%s blob count got %d (expected %u)", 
    desc, count_arg->n, expect_n-1);

  DT_Q_1D(dt1d_cmp,(					/* compare blob sizes				*/
    DTNTYPE_S4,
    count_arg->a, count_arg->n+1,
    expect_a, expect_n,
    0, &cmp));

  dt_assert(ctl, cmp==0, F, 
    "%s blob sizes (cmp=%ld)",
    desc, cmp);

  DT_Q_1D(dt1d_cmp,(					/* compare blob sums				*/
    DTNTYPE_S4,
    count_arg->s, count_arg->n+1,
    expect_s, expect_n,
    0, &cmp));

  dt_assert(ctl, cmp==0, F, 
    "%s blob sums  (cmp=%ld)", 
    desc, cmp);

  return DT_RC_GOOD;
}

/*..........................................................................
 * function called by dttadj for each pixel
 *..........................................................................*/

static 
dt_rc_e 
count_func(
  dt_ctl_t *ctl,
  dttadj_callback_arg_t *arg,
  dtxy_t x1,
  dtxy_t y1,
  dtxy_t x2,
  dtxy_t y2)
{
  DT_F("count_func");
  count_arg_t *count_arg = (count_arg_t *)arg->arg;
  int flag = arg->flag;
  dtxy_t x = 0;
  dtxy_t y = 0;

  arg->flag = 0;						/* clear flag for next blob		    */

  if (flag & DTTADJ_FLAG_END)			/* finishing an old blob?			*/
  {
    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "    total size %ld",
      count_arg->a[count_arg->n]);
  
    count_arg->n++;						/* up blob count				    */

    if (count_arg->m &&					/* caller has specified limit?	    */
        count_arg->n == count_arg->m)	/* we got the max we want?		    */
      return DT_RC_STOP;

    if (!(flag & DTTADJ_FLAG_BEG))		/* not also starting a new blob?	*/
      return DT_RC_GOOD;
  }

  if (flag & DTTADJ_FLAG_BEG)			/* starting a new blob?				*/
  {
    if (arg->label == count_arg->v)		/* check value of pixel we want	 	*/
    {
      dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
        "found arg->v %d, (%2d,%2d)",
        (int)count_arg->v, x, y);
      return DT_RC_STOP;
    }

    count_arg->a[count_arg->n] = 0;		/* clear count in this blob			*/
    count_arg->s[count_arg->n] = 0;

    dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
      "blob %ld started",
      count_arg->n);
  }


  for (y=y1; y<=y2; y++)
    for (x=x1; x<=x2; x++)
	{
      count_arg->a[count_arg->n]++;		/* up count in this blob		    */
      count_arg->s[count_arg->n] += 
        (long)arg->label;
	}

  dt_dbg(ctl, F, DT_DBG_MASK_PIXEL,
    "    (%2d,%2d) (%2d,%2d) size now %ld, sum now %ld",
    x1, y1, x2, y2,
    count_arg->a[count_arg->n],
    count_arg->s[count_arg->n]);
  
  return DT_RC_GOOD;					/* keep doing blobs				    */
}

/*..........................................................................*/
static
dt_rc_e
dttadj_file_ntype(
  dt_ctl_t *ctl,
  const char *filename,
  dt_ntype_e ntype)
{					
  DT_F("dttadj_file_ntype");
  dtimage_t source;
  dtimage_t dup;
  count_arg_t *count_arg = NULL;
  dt_ntype_b08_t *data = NULL;
  int ncols, nrows, nruns, nblobs;
  unsigned long flags;
  dtntype_s4_t *expect_labels = NULL;
  dtntype_s4_t *expect_areas = NULL;
  dtntype_s4_t *expect_sums = NULL;
  char desc[64];
  char format[64];
  FILE *file;
  int good;
  int i;
#define NFLAGSS (3)
  unsigned long flagss[NFLAGSS] = {
    0,
    DTIMAGE_ADJ_FLAG_4,
    DTIMAGE_ADJ_FLAG_8};
  dt_rc_e rc;

  file = fopen(filename, "r");
  if (file == NULL)
    return dt_err(ctl, F,
      "unable to open \"%s\" to read", filename);

  DT_C(dtos_malloc2,(ctl,
    (void **)&count_arg, 
    sizeof(*count_arg),
    F, "count_arg"));
  DT_C(dtos_malloc2,(ctl,
    (void **)&data, 
    40*40*sizeof(*data),
    F, "data"));
  DT_C(dtos_malloc2,(ctl,
    (void **)&expect_labels, 
    256*sizeof(*expect_labels),
    F, "expect_labels"));
  DT_C(dtos_malloc2,(ctl,
    (void **)&expect_areas, 
    256*sizeof(*expect_areas),
    F, "expect_areas"));
  DT_C(dtos_malloc2,(ctl,
    (void **)&expect_sums, 
    256*sizeof(*expect_sums),
    F, "expect_sums"));

  while(1)
  {
    DT_C(dttutil_read_runadj,(ctl, file,
      desc, 
      &ncols, &nrows, &nruns, &nblobs,
      expect_labels,
      expect_areas,
      expect_sums,
  	  data,
	  &flags,
	  &good));
    if (!good)
      break;
	expect_areas[nblobs] = COUNT_UNLIKELY;
	expect_sums[nblobs] = COUNT_UNLIKELY;

    sprintf(format, "(%%-5s%%d) %-3s %-24s",
      dt_ntype_string[ntype], desc);

    DT_C(dttadj_make,(ctl,				/* make image structures			*/
      &source, NULL,
      ncols, nrows, ntype,
      data,
      NULL,
      count_arg));

										/* ................................ */
	for (i=0; i<NFLAGSS; i++)
    if (flagss[i] == flags ||
        flagss[i] & flags)
	{
  	  DT_C(dttadj_init,(ctl,
        count_arg));
      DT_C(dttadj_run_run,(		/* do the callback by run method    */
        ctl, &source, 
        0, 0, source.xe, source.ye,
        0.0, flagss[i],
        count_func, (void *)count_arg));
	  sprintf(desc, format, "runs",
        flagss[i]);
      DT_C(dttadj_check,(
        ctl, &source, nblobs+1,
        expect_areas, expect_sums,
        count_arg, F, desc));
	}
										/* ................................ */
	if (flags == 0 || 
		flags & DTIMAGE_ADJ_FLAG_4)
	if (!(source.flags &
          DTIMAGE_FLAG_PAGED))
	{
  	  DT_C(dttadj_init,(ctl,
        count_arg));
	  DT_C(dttadj_runf,(			/* do the callback by runf method   */
        ctl, &source, 0.0, 
        DTIMAGE_ADJ_FLAG_4,
        count_func, (void *)count_arg));
  	  sprintf(desc, format, "runf",
        4);
	  DT_C(dttadj_check,(
        ctl, &source, nblobs+1,
        expect_areas, expect_sums,
        count_arg, F, desc));
	}
										/* ................................ */

	if (flags == 0 || 
		flags & DTIMAGE_ADJ_FLAG_4)
	{
	  DT_C(dtimage_dup,(ctl,			/* tmp image to contaminate 		*/
        &source, &dup));
  	  DT_C(dttadj_init,(ctl,
        count_arg));
      DT_G(dttadj_fill,(				/* do the callback by fill method	*/
        ctl, &dup, 0.0,
        DTIMAGE_ADJ_FLAG_4,
        count_func, (void *)count_arg));
	  if (rc == DT_RC_GOOD)
	  {
  	    sprintf(desc, format, "fill", 
          4);
        DT_C(dttadj_check,(
          ctl, &source, nblobs+1,
          expect_areas, expect_sums,
          count_arg, F, desc));
	  }
	  DT_C(dtimage_free,(ctl, &dup));	/* free the contaminated image 		*/
	}
										/* ................................ */

	if (flags == 0 || 
		flags & DTIMAGE_ADJ_FLAG_4)
	if (ntype == DT_NTYPE_B08)
	{
	  DT_C(dtimage_dup,(ctl,
        &source, &dup));
  	  DT_C(dttadj_init,(ctl,
        count_arg));
      DT_C(dttadj_flood,(			/* do the callback by flood method	*/
        ctl, &dup, 0.0,
        DTIMAGE_ADJ_FLAG_4,
        count_func, (void *)count_arg));
  	  sprintf(desc, format, "flood",
        4);
      DT_C(dttadj_check,(
        ctl, &source, nblobs+1,
        expect_areas, expect_sums,
        count_arg, F, desc));
	  DT_C(dtimage_free,(ctl, &dup));
	}
										/* ................................ */
    DT_C(dtimage_free,(ctl, 
      &source));
  }

cleanup:
  if (expect_sums)
    DT_I(dtos_free2,(ctl, expect_sums, F, "expect_sums"));
  if (expect_areas)
    DT_I(dtos_free2,(ctl, expect_areas, F, "expect_areas"));
  if (expect_labels)
    DT_I(dtos_free2,(ctl, expect_labels, F, "expect_labels"));
  if (data)
    DT_I(dtos_free2,(ctl, data, F, "data"));
  if (count_arg)
    DT_I(dtos_free2,(ctl, count_arg, F, "count_arg"));

  fclose(file);

  return rc;
}

/*..........................................................................*/
static
dt_rc_e
dttadj_file(
  dt_ctl_t *ctl,
  const char *filename)
{					
  DT_F("dttadj_file");
  DT_Q(dttadj_file_ntype,(ctl, filename, DT_NTYPE_S4));
  DT_Q(dttadj_file_ntype,(ctl, filename, DT_NTYPE_B08));
  DT_Q(dttadj_file_ntype,(ctl, filename, DT_NTYPE_B16));
  DT_Q(dttadj_file_ntype,(ctl, filename, DT_NTYPE_S1));
  DT_Q(dttadj_file_ntype,(ctl, filename, DT_NTYPE_S2));
  DT_Q(dttadj_file_ntype,(ctl, filename, DT_NTYPE_F1));
  DT_Q(dttadj_file_ntype,(ctl, filename, DT_NTYPE_F2));
  return DT_RC_GOOD;
}

/*..........................................................................*/

static
dt_rc_e
dttadj_whole(
  dt_ctl_t *ctl)
{
  DT_F("dttadj_whole");
  dtimage_t source, expect;
  count_arg_t *count_arg = NULL;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,
    (void **)&count_arg, 
    sizeof(*count_arg),
    F, "count_arg"));

  {										/* ................................ */
#define NCOLS 10
#define NROWS 10
#define DESC "count-stop"
    static dt_ntype_b08_t source_data[NCOLS*NROWS] = {
       2,2,2,2,2,2,2,2,2,2,
       2,2,2,2,2,2,2,2,2,2,
       2,2,2,2,4,0,0,0,2,2,
       1,1,1,1,4,1,1,1,4,2,
       2,2,2,2,4,2,2,2,4,2,
       2,2,2,2,4,4,4,4,4,2,
       2,2,2,2,4,1,1,1,4,2,
       2,4,4,4,4,2,2,1,4,2,
       2,2,2,2,2,2,2,1,1,0,
	   0,0,0,0,0,0,0,0,0,0
	};
    static dt_ntype_b08_t expect_data[NCOLS*NROWS] = {
       2,2,2,2,2,2,2,2,2,2,
       2,2,2,2,2,2,2,2,2,2,
       2,2,2,2,2,2,2,2,2,2,
       1,1,1,1,2,1,1,1,2,2,
       2,2,2,2,2,2,2,2,2,2,
       2,2,2,2,2,2,2,2,2,2,
       2,2,2,2,2,1,1,1,2,2,
       2,2,2,2,2,2,2,1,2,2,
       2,2,2,2,2,2,2,1,1,0,
	   0,0,0,0,0,0,0,0,0,0
	};
    dtntype_s4_t expect_a1[] = {3,  3, COUNT_UNLIKELY};
    dtntype_s4_t expect_s1[] = {0,  3, COUNT_UNLIKELY};
    dtntype_s4_t expect_a2[] = {3, 17, COUNT_UNLIKELY};
    dtntype_s4_t expect_s2[] = {0, 68, COUNT_UNLIKELY};

    DT_C(dttadj_make,(ctl,				/* make image structures			*/
      &source, &expect,
      NCOLS, NROWS, DT_NTYPE_B08,
      &source_data[0],
      &expect_data[0],
      count_arg));


	DT_C(dttadj_init,(ctl,
      count_arg));
    count_arg->m = 2;					/* we just want two blobs		    */
    DT_C(dttadj_run_run,(				/* do the callback by run method    */
      ctl, &source,
      0, 0, source.xe, source.ye,
      2.0, 0,
      count_func, (void *)count_arg));
    DT_C(dttadj_check,(ctl,				/* check run callback results	    */
      &source,
      sizeof(expect_a1)/sizeof(*expect_a1),
      expect_a1, expect_s1,
      count_arg,
      F, "(run)  " DESC));

	DT_C(dttadj_init,(ctl,
      count_arg));
    count_arg->m = 2;					/* we just want two blobs		    */
    DT_C(dttadj_fill,(					/* do the callback				    */
      ctl, &source, 2.0, 0,
      count_func, (void *)count_arg));

    DT_C(dttadj_check,(ctl,				/* check callback results		    */
      &source,
      sizeof(expect_a2)/sizeof(*expect_a2),
      expect_a2, expect_s2,
      count_arg,
      F, "(fill) " DESC));
    DT_C(dttadj_compare_and_free,(ctl,	/* compare and free images		    */
      &source, &expect,
      F, "(fill) " DESC));

#undef NCOLS
#undef NROWS
#undef DESC
  }

  {										/* ................................ */
#define NCOLS 10
#define NROWS 10
#define DESC "whole-image"
    static dt_ntype_b08_t source_data[NCOLS*NROWS] = {
       0,0,0,0,1,1,1,0,0,0,
       0,0,0,9,9,9,1,0,0,0,
       0,0,0,9,1,9,1,0,0,0,
       0,0,0,9,1,1,1,0,0,0,
       0,0,0,9,9,9,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,1,0,0,0,0,0,0,0,
       1,1,1,1,1,0,0,0,0,1
	};
    static dt_ntype_b08_t expect_data[NCOLS*NROWS] = {
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0
	};
    dtntype_s4_t expect_a1[] = {1, 6, 9, 9,  COUNT_UNLIKELY};
    dtntype_s4_t expect_s1[] = {1, 6, 9, 81, COUNT_UNLIKELY};

    DT_C(dttadj_make,(ctl,				/* make image structures			*/
      &source, &expect,
      NCOLS, NROWS, DT_NTYPE_B08,
      &source_data[0],
      &expect_data[0],
      count_arg));

	DT_C(dttadj_init,(ctl,
      count_arg));
    DT_C(dttadj_run_run,(			/* do the callback by run method    */
      ctl, &source, 
      0, 0, source.xe, source.ye,
      0.0, 0,
      count_func, (void *)count_arg));
    DT_C(dttadj_check,(ctl,				/* check run callback results	    */
      &source,
      sizeof(expect_a1)/sizeof(*expect_a1),
      expect_a1, expect_s1,
      count_arg,
      F, "(run)  " DESC));

	DT_C(dttadj_init,(ctl,
      count_arg));
    DT_C(dttadj_fill,(				/* do the callback				    */
      ctl, &source, 0.0, 0,
      count_func, (void *)count_arg));
    DT_C(dttadj_check,(ctl,				/* check callback results		    */
      &source,
      sizeof(expect_a1)/sizeof(*expect_a1),
      expect_a1, expect_s1,
      count_arg,
      F, "(fill) " DESC));
    DT_C(dttadj_compare_and_free,(ctl,	/* compare and free images		    */
      &source, &expect,
      F, "(fill) " DESC));

#undef NCOLS
#undef NROWS
#undef DESC
  }

  {										/* ................................ */
#define NCOLS 10
#define NROWS 10
#define DESC "search-stop"
    static dt_ntype_b08_t source_data[NCOLS*NROWS] = {
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,2,1,1,1,0,0,
       1,1,1,1,2,3,3,3,2,0,
       0,0,0,0,2,0,0,0,2,0,
       0,0,0,0,2,2,2,2,2,0,
       0,0,0,0,2,1,1,1,2,0,
       0,2,2,2,2,0,0,1,2,0,
       0,0,0,0,0,0,0,1,1,0
	};
    static dt_ntype_b08_t expect_data[NCOLS*NROWS] = {
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,3,3,3,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,1,1,1,0,0,
       0,0,0,0,0,0,0,1,0,0,
       0,0,0,0,0,0,0,1,1,0
	};
    dtntype_s4_t expect_a1[] = {3, COUNT_UNLIKELY};
    dtntype_s4_t expect_s1[] = {3, COUNT_UNLIKELY};
    dtntype_s4_t expect_a2[] = {3, 4, 17, COUNT_UNLIKELY};
    dtntype_s4_t expect_s2[] = {3, 4, 34, COUNT_UNLIKELY};

    DT_C(dttadj_make,(ctl,				/* make image structures			*/
      &source, &expect,
      NCOLS, NROWS, DT_NTYPE_B08,
      &source_data[0],
      &expect_data[0],
      count_arg));


	DT_C(dttadj_init,(ctl,
      count_arg));
    count_arg->v = 3.0;					/* go til we find a 3	   			*/
    DT_C(dttadj_run_run,(			/* do the callback by run method    */
      ctl, &source,
      0, 0, source.xe, source.ye,
      0.0, 0,
      count_func, (void *)count_arg));
    DT_C(dttadj_check,(ctl,				/* check run callback results	    */
      &source,
      sizeof(expect_a1)/sizeof(*expect_a1),
      expect_a1, expect_s1,
      count_arg,
      F, "(run)  " DESC));

	DT_C(dttadj_init,(ctl,
      count_arg));
    count_arg->v = 3.0;					/* go til we find a 3	   			*/
    DT_C(dttadj_fill,(				/* do the callback				    */
      ctl, &source, 0.0, 0,
      count_func, (void *)count_arg));

    DT_C(dttadj_check,(ctl,				/* check callback results		    */
      &source,
      sizeof(expect_a2)/sizeof(*expect_a2),
      expect_a2, expect_s2,
      count_arg,
      F, "(fill) " DESC));
    DT_C(dttadj_compare_and_free,(ctl,	/* compare and free images		    */
      &source, &expect,
      F, "(fill) " DESC));

#undef NCOLS
#undef NROWS
#undef DESC
  }

  {										/* ................................ */
#define NCOLS 10
#define NROWS 10
#define DESC "individual"
    static dt_ntype_b08_t source_data[NCOLS*NROWS] = {
       2,2,2,2,0,1,0,0,0,0,
       2,1,1,1,3,3,3,0,0,0,
       2,3,2,1,3,0,3,3,0,0,
       2,3,2,3,3,1,1,3,1,0,
       0,3,2,2,2,2,1,3,1,0,
       0,3,3,3,1,1,1,3,1,0,
       0,0,0,0,0,3,3,3,1,0,
       0,0,0,0,0,3,1,1,1,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
	};
    static dt_ntype_b08_t expect_data[NCOLS*NROWS] = {
       2,2,2,2,0,1,0,0,0,0,
       2,1,1,1,0,0,0,0,0,0,
       2,3,2,1,0,0,0,0,0,0,
       2,3,2,0,0,1,1,0,1,0,
       0,3,2,2,2,2,1,0,1,0,
       0,3,3,3,1,1,1,0,1,0,
       0,0,0,0,0,0,0,0,1,0,
       0,0,0,0,0,0,1,1,1,0,
       0,0,0,0,0,0,0,0,0,0,
       0,0,0,0,0,0,0,0,0,0,
	};
    dtntype_s4_t expect_a[] = {15,
      COUNT_UNLIKELY};
    dtntype_s4_t expect_s[] = {45,
      COUNT_UNLIKELY};

    DT_C(dttadj_make,(ctl,				/* make image structures			*/
      &source, &expect,
      NCOLS, NROWS, DT_NTYPE_B08,
      &source_data[0],
      &expect_data[0],
      count_arg));
	DT_C(dttadj_init,(ctl,
      count_arg));

    DT_C(dttadj_fill_one,(ctl,		/* this is a miss					*/
      &source, 5, 2, 0.0, 0,
      count_func, (void *)count_arg));

    DT_C(dttadj_fill_one,(ctl,		/* do the callback				    */
      &source, 4, 2, 0.0, 0,
      count_func, (void *)count_arg));

    DT_C(dttadj_check,(ctl,				/* check callback results		    */
      &source,
      sizeof(expect_a)/sizeof(*expect_a),
      expect_a, expect_s,
      count_arg,
      F, "(fill) " DESC));
    DT_C(dttadj_compare_and_free,(ctl,	/* compare and free images		    */
      &source, &expect,
      F, "(fill) " DESC));

#undef NCOLS
#undef NROWS
#undef DESC
  }

#ifdef NONO
  {										/* ................................ */
#define NCOLS 16
#define NROWS 8
#define DESC "on-bit"
    static dt_ntype_b01_t source_data[(NCOLS/8)*NROWS] = {
       0x00, 0x00,
       0x01, 0x80,
       0x01, 0x80,
       0x00, 0x00,
       0x01, 0x80,
       0x01, 0x80,
       0x01, 0x80,
       0x00, 0x00
	};
    static dt_ntype_b08_t expect_data[(NCOLS/8)*NROWS] = {
       0x00, 0x00,
       0x00, 0x00,
       0x00, 0x00,
       0x00, 0x00,
       0x00, 0x00,
       0x00, 0x00,
       0x00, 0x00,
       0x00, 0x00
	};
    dtntype_s4_t expect_a[] = {4, 6,
      COUNT_UNLIKELY};
    dtntype_s4_t expect_s[] = {4, 6,
      COUNT_UNLIKELY};

    DT_C(dttadj_make,(ctl,				/* make image structures			*/
      &source, &expect,
      NCOLS, NROWS, DT_NTYPE_B01,
      &source_data[0],
      &expect_data[0],
      count_arg));

	DT_C(dttadj_init,(ctl,
      count_arg));
    DT_C(dttadj,(ctl,				/* do the callback				    */
      &source, 0.0,
      count_func, (void *)count_arg));

    DT_C(dttadj_check,(ctl,				/* check callback results		    */
      &source,
      sizeof(expect_a)/sizeof(*expect_a),
      expect_a, expect_s,
      count_arg,
      F, "(fill) " DESC));
    DT_C(dttadj_compare_and_free,(ctl,	/* compare and free images		    */
      &source, &expect,
      F, "(fill) " DESC));

#undef NCOLS
#undef NROWS
#undef DESC
  }

  {										/* ................................ */
#define NCOLS 16
#define NROWS 8
#define DESC "off-bit"
    static dt_ntype_b01_t source_data[(NCOLS/8)*NROWS] = {
       0x00, 0x00,
       0x01, 0x80,
       0x01, 0x80,
       0x00, 0x00,
       0x01, 0x80,
       0x01, 0x80,
       0x01, 0x80,
       0x00, 0x00
	};
    static dt_ntype_b08_t expect_data[(NCOLS/8)*NROWS] = {
       0xff, 0xff,
       0xff, 0xff,
       0xff, 0xff,
       0xff, 0xff,
       0xff, 0xff,
       0xff, 0xff,
       0xff, 0xff,
       0xff, 0xff
	};
    dtntype_s4_t expect_a[] = {118,
      COUNT_UNLIKELY};
    dtntype_s4_t expect_s[] = {0,
      COUNT_UNLIKELY};

    DT_C(dttadj_make,(ctl,				/* make image structures			*/
      &source, &expect,
      NCOLS, NROWS, DT_NTYPE_B01,
      &source_data[0],
      &expect_data[0],
      count_arg));

	DT_C(dttadj_init,(ctl,
      count_arg));
    DT_C(dttadj,(ctl,				/* do the callback				    */
      &source, 1.0,
      count_func, (void *)count_arg));

    DT_C(dttadj_check,(ctl,				/* check callback results		    */
      &source,
      sizeof(expect_a)/sizeof(*expect_a),
      expect_a, expect_s,
      count_arg,
      F, "(fill) " DESC));
    DT_C(dttadj_compare_and_free,(ctl,	/* compare and free images		    */
      &source, &expect,
      F, "(fill) " DESC));

#undef NCOLS
#undef NROWS
#undef DESC
  }
#endif
cleanup:
  if (count_arg)
    DT_I(dtos_free2,(ctl, count_arg, F, "count_arg"));
  return rc;
}

#ifdef NONO
/*..........................................................................*/

static
dt_rc_e
dttadj_paged(
  dt_ctl_t *ctl)
{
  DT_F("dttadj_paged");
  dtimage_t source;
  count_arg_t count_arg;
  void *memsource = NULL;
  dt_rc_e rc;

  {										/* ................................ */
#define DESC "paged"
    dtntype_s4_t expect_a[] = {833, COUNT_UNLIKELY};
    dtntype_s4_t expect_s[] = {833, COUNT_UNLIKELY};

    source.xe = 0;
    DT_C(dtimage_create,(ctl, &source,
      33, 33,							/* fill stack highwater will be 272	*/
      DT_NTYPE_B08));

    count_arg.image = &source;			/* callback needs source image	    */
										/* ................................ */
    DT_C(dtimage_gen_rect_islands,(		/* lots of tiny holes in big blob   */
      ctl, &source, 1.0, 0.0,	
      1, 1, 1));

	DT_C(dttadj_init,(ctl,
      &count_arg));
    DT_C(dttadj_run,(		/* do the callback by run method    */
      ctl, &source, 0.0, 0,
      count_func, (void *)&count_arg));
    DT_C(dttadj_check,(ctl,				/* check run callback results	    */
      &source,
      sizeof(expect_a)/sizeof(*expect_a),
      expect_a, expect_s,
      &count_arg,
      F, "(run)  " DESC));
										/* ................................ */
    DT_C(dtimage_gen_rect_islands,(		/* lots of tiny holes in big blob   */
      ctl, &source, 1.0, 0.0,	
      1, 1, 1));

    memsource = ctl->memsource;
    ctl->memsource = NULL;
    DT_C(dtmem_configure,(ctl,			/* extent size is 32*8=256 so 240	*/
      "ram,,240"));						/*   causes paging within extents   */
	DT_C(dttadj_init,(ctl,
      &count_arg));
    DT_C(dttadj_fill,(				/* do the callback by fill method   */
      ctl, &source, 0.0, 
      -64,								/* 2048/64=32 pushes per extent	    */
      count_func, (void *)&count_arg));
    DT_C(dtmem_unconfigure,(ctl));
    ctl->memsource = memsource;
    memsource = NULL;

    DT_C(dttadj_check,(ctl,				/* check callback results		    */
      &source,
      sizeof(expect_a)/sizeof(*expect_a),
      expect_a, expect_s,
      &count_arg,
      F, "(fill ram) " DESC));
										/* ................................ */
    if (dtprj_xmsif)
	{
      DT_C(dtimage_gen_rect_islands,(	/* lots of tiny holes in big blob   */
        ctl, &source, 1.0, 0.0,	
        1, 1, 1));

      memsource = ctl->memsource;
      ctl->memsource = NULL;
      DT_C(dtmem_configure,(ctl,		/* extent size is 32*8=256 so 240	*/
        "xms,,240,1"));					/*   causes paging within extents   */
  	  DT_C(dttadj_init,(ctl,
        &count_arg));
      DT_C(dttadj_fill,(	/* do the callback by fill method   */
        ctl, &source, 0.0, 
        -64,							/* 2048/64=32 pushes per extent	    */
        count_func, 
        (void *)&count_arg));
      DT_C(dtmem_unconfigure,(ctl));
      ctl->memsource = memsource;
      memsource = NULL;

      DT_C(dttadj_check,(ctl,			/* check callback results		    */
        &source,
        sizeof(expect_a)/sizeof(*expect_a),
        expect_a, expect_s,
        &count_arg,
        F, "(fill xms) " DESC));
	}
										/* ................................ */

    DT_Q(dtimage_free,(ctl, &source));

#undef DESC
  }
    return DT_RC_GOOD;    

cleanup:
  if (memsource)
  {
    DT_I(dtmem_unconfigure,(ctl));
    ctl->memsource = memsource;
  }
  if (source.xe)
    DT_I(dtimage_free,(ctl, &source));
  return rc;
}
#endif

/*..........................................................................*/

typedef struct {
  long n;
} time_arg_t;

static dt_rc_e time_func(
  dt_ctl_t *ctl,
  dttadj_callback_arg_t *arg,
  dtxy_t x1,
  dtxy_t y1,
  dtxy_t x2,
  dtxy_t y2)
{
  time_arg_t *time_arg = (time_arg_t *)arg->arg;
  int flag = arg->flag;
  arg->flag = 0;
  if (flag & DTTADJ_FLAG_END)
    time_arg->n++;

#ifdef NONO
  unsigned int v = arg->image->row[y1].b08[x1];
dt_say(ctl, "time_func",
  "x %3d-%3d, y %3d-%3d, v %3u, FLAG_BEG %d, FLAG_END %d, n %d",
  x1, x2, y1, y2, v,
  flag & DTTADJ_FLAG_BEG,
  flag & DTTADJ_FLAG_END,
  time_arg->n);
#endif

  return DT_RC_GOOD;					/* keep doing blobs				    */
}

/*..........................................................................
 *..........................................................................*/


dt_rc_e
DTCONFIG_API1
dttadj_time(
  dt_ctl_t *ctl,
  dtimage_t *image,
  long nblobs,
  void *arg,
  const char *desc,
  long loops)
{
  DT_F("dttadj_time");
  time_arg_t time_arg;
  dtos_time_t mark1, mark2;
  char f[64];
  long loop;

										/* ................................ */
  DT_Q(dtos_time,(ctl, &mark1));
  for (loop=0; loop<loops; loop++)
  {
    time_arg.n = 0;
    DT_Q(dttadj_run_run,(			/* do the callback via stack runs   */
      ctl, image,
      0, 0, image->xe, image->ye,
      0.0, 0,
      time_func, (void *)&time_arg));
  }
  DT_Q(dtos_time,(ctl, &mark2));

  dt_assert(ctl,
    nblobs == -1 || time_arg.n == nblobs,
    F, "%-16s run  blob count %ld (%ld)",
    desc, time_arg.n, nblobs);

  sprintf(f, "runs  %s %-3s (%2ld)",	/* description of image				*/
    desc,
    dt_ntype_string[image->ntype],
    loops);
  dtt_time_report(ctl,
    &mark1, &mark2, f);
										/* ................................ */
  if (!(image->flags &					/* check first 						*/
       DTIMAGE_FLAG_PAGED))
  {
    DT_Q(dtos_time,(ctl, &mark1));
    for (loop=0; loop<loops; loop++)
    {
      time_arg.n = 0;
      DT_Q(dttadj_runf,(			/* do the callback via linear runs  */
        ctl, image, 0.0, 0,
        time_func, (void *)&time_arg));
    }
    DT_Q(dtos_time,(ctl, &mark2));

    dt_assert(ctl,
      nblobs == -1 || time_arg.n == nblobs,
      F, "%-16s runf blob count %ld (%ld)",
      desc, time_arg.n, nblobs);

    sprintf(f, "runf  %s %-3s (%2ld)", 	/* description of image				*/
      desc,
      dt_ntype_string[image->ntype],
      loops);
    dtt_time_report(ctl,
      &mark1, &mark2, f);
  }

										/* ................................ */
  {
	DT_Q(dtos_time,(ctl, &mark1));
    for (loop=0; loop<loops; loop++)
	{
	  dtimage_t dup;
	  time_arg.n = 0;
	  DT_Q(dtimage_dup,(ctl, image,		/* make a temporary image 			*/
        &dup)); 
	  
	  DT_Q(dttadj_fill,(			/* do the callback				    */
        ctl, &dup, 0.0, 0,
        time_func, (void *)&time_arg));
	  
	  DT_Q(dtimage_free,(ctl, &dup));	/* free temporary image 			*/
	}
	DT_Q(dtos_time,(ctl, &mark2));

	dt_assert(ctl,
      nblobs == -1 || time_arg.n == nblobs,
      F, "%-16s fill blob count %ld (%ld)",
      desc, time_arg.n, nblobs);

	sprintf(f, "fill  %s %-3s (%2ld)",	/* description of image				*/
	  desc, 
      dt_ntype_string[image->ntype],
      loops);
	dtt_time_report(ctl,
      &mark1, &mark2, f);
  }
										/* ................................ */
  if (image->ntype == DT_NTYPE_B08)
  {
	DT_Q(dtos_time,(ctl, &mark1));
    for (loop=0; loop<loops; loop++)
	{
	  dtimage_t dup;
	  time_arg.n = 0;
	  DT_Q(dtimage_dup,(ctl, image,		/* make a temporary image 			*/
        &dup)); 
	  
	  DT_Q(dttadj_flood,(			/* do the callback				    */
        ctl, &dup, 0.0, 0,
        time_func, (void *)&time_arg));
	  
	  DT_Q(dtimage_free,(ctl, &dup));	/* free temporary image 			*/
	}
	DT_Q(dtos_time,(ctl, &mark2));

	dt_assert(ctl,
      nblobs == -1 || time_arg.n == nblobs,
      F, "%-16s flood blob count %ld (%ld)",
      desc, time_arg.n, nblobs);

	sprintf(f, "flood %s %-3s (%2ld)",	/* description of image				*/
	  desc, 
      dt_ntype_string[image->ntype],
	  loops);
	dtt_time_report(ctl,
      &mark1, &mark2, f);
  }
										/* ................................ */

  return DT_RC_GOOD;    
}

/*..........................................................................*/

dt_rc_e
DTCONFIG_API1
dttadj_arg_check(          				/* check args and params            */
  dt_ctl_t *ctl,
  void *app,
  dtparam_t *param,
  dt_enter_arg_t *arg,
  char *msg,
  const char **start)
{
  DT_F("dttadj_arg_check");

  DT_Q(dtt_check_op,(ctl, param,        /* make sure op is understood       */
    ops, msg, start));

  if (msg[0] == '\0')					/* no problems yet? 				*/
  DT_Q(dtt_check_datadir,(ctl, param,	/* make sure datadir exists       	*/
    msg, start));

  return DT_RC_GOOD;
}

/*..........................................................................*/

DTT_MAIN(dttadj)
{
  DT_F("dttadj");
  dt_ctl_t *ctl = &param->ctl;
  dt_enter_arg_t arg[1];
  char testdata[128];
  int assert_fail = 0;                  /* failed assertion count           */
  dt_rc_e rc = DT_RC_GOOD;

  DT_GI(dt_heapmon_init,(ctl));         /* init memory checking             */

  DT_GI(dtparam_init,(param));          /* init param structure             */

  DT_GI(dtparam_set,(param, 0,          /* set default for params we want   */
    &param->op, NULL, NULL, "default",
    &param->datadir, NULL, NULL, NULL,
    &param->input, NULL, NULL, NULL,
    &param->reps, NULL, NULL, "1",
    &param->n, "Timing loops", NULL, "5",
    &param->w, NULL, NULL, "256",
    &param->h, NULL, NULL, "256",
    NULL));

  arg[0].desc = NULL;                   /* no arguments                     */
  
  DT_GI(dt_interactive_parse,(			/* parse command line interactively	*/
    argc, argv, F, USAGE, param,
    arg, dttadj_arg_check, NULL));
                                        /* ................................ */

  DT_GI(dtstr_printf,(ctl, testdata,
    sizeof(testdata), "%s/%s",
    param->datadir, 
    param->input? param->input:
    "runadj.dat"));

  while (rc == DT_RC_GOOD)
  {
    long reps;
    for (reps=0;
         reps < param->reps &&
         rc == DT_RC_GOOD;
         reps++)
    {

      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "default"))
      {
        DT_GI(dttadj_whole,(ctl));		/* test whole-image routine(s)	    */
	    DT_GI(dttadj_file,(ctl,			/* read from file					*/
          testdata));
      }
    
      if (!strcmp(param->op, "whole"))
      {
        DT_GI(dttadj_whole,(ctl));		/* test whole-image routine(s)	    */
      }
    
#ifdef NONO
      if (!strcmp(param->op, "paged"))
      {
        DT_GI(dttadj_paged,(ctl));		/* test paged routine(s)		    */
      }
#endif
    
      if (!strcmp(param->op, "file"))
      {
	    DT_GI(dttadj_file,(ctl,			/* read from file					*/
          testdata));
      }
    
      if (!strcmp(param->op, "all") ||
          !strcmp(param->op, "time"))
      {
        DT_GI(dtt_image_list,(param,
          dtt_blob_list,
          param->input,
          DT_NTYPE_B08,
          dttadj_time, NULL));
        DT_GI(dtt_image_list,(param,
          dtt_blob_list,
          param->input,
          DT_NTYPE_F2,
          dttadj_time, NULL));
      }
    }

    DT_G(dt_interactive_after,(         /* make post display                */
      ctl, param, USAGE,
      arg, dttadj_arg_check, NULL, rc));
  }

                                        /* ................................ */

  DT_I(dt_interactive_leave,(           /* clean up before leaving          */
    param, rc));

  DT_I(dt_heapmon_uninit,(ctl,          /* check all memory is freed        */
    &assert_fail));

  if (assert_fail)                      /* any failed assertions?           */
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
