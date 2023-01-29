/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




http://ganesh.mit.edu/sanjeev/atcs/heapsort.c.html

Sample Code 

/*
 *  heapsort.c
 *
 *
 */
#include 

typedef dtntype_b32_t unlong;

typedef int (*__comp_func)(const void *, const void *);
typedef void (*__cpy1_func)(unlong, unlong);
typedef int (*__comp1_func)(unlong, unlong);

void heapsort(void *, unlong, unlong, __comp_func);
void _heapsort(unlong, __comp1_func, __cpy1_func);

static char *hBase;
static unlong hSize;
static __comp_func hCompare;

static int std_compare(unlong, unlong);
static void std_copy(unlong, unlong);

static void heapsort1(unlong);
static __comp1_func h1Compare;
static __cpy1_func h1Copy;


/* ---------- standard heapsort ---------- */


void heapsort(void *base, unlong n, unlong size, __comp_func compare)
{
        hBase = base;
        hSize = size;
        hCompare = compare;
        _heapsort(n, std_compare, std_copy);
}


static int std_compare(unlong i, unlong j)
{
        return((*hCompare)(hBase + i * hSize, hBase + j * hSize));
}

static void std_copy(unlong i, unlong j)
{
        //register unlong size = hSize;
        //register void *p = hBase + i * size;
        //register void *q = hBase + j * size;
        // sadly, this had to be killed out of assembly.  grrr...
        // but i have to admit i don't know why this is not using the standard ANSI memcpy function
        // here.
        
        memcpy (hBase + i * hSize, hBase + j * hSize, hSize);
        
        
        
///     asm {
//1     move.b (q)+, d0
//              move.b d0, (p)+
//              subq.l #1, size
//              bne.s @1
//      }
}

/* ---------- general heapsort ---------- */


void _heapsort(unlong n, __comp1_func compare, __cpy1_func cpy)
{
        h1Compare = compare;
        h1Copy = cpy;
        heapsort1(n);
}


/*
 *  sort elements 
 *
 */

static void heapsort1(unlong n)
{
        register unlong l,j,ir,i;               
 
        l=(n >> 1)+1;
        ir=n;
        for (;;) {
                if (l > 1)
                        (*h1Copy)(n, (--l-1));
                else {
                        (*h1Copy)(n, (ir-1));
                        (*h1Copy)((ir-1), 0);
                        if (--ir == 1) {
                        (*h1Copy)(0,n);
                                return;
                        }
                }
                i=l;
                j=l << 1;
                while (j <= ir) {
                        if (j < ir && (*h1Compare)(j-1, j) < 0) ++j;
                        if ((*h1Compare)(n, j-1) < 0) {
                                (*h1Copy)(i-1, (j-1));
                                j += (i=j);
                        }
                        else j=ir+1;
                }
                (*h1Copy)(i-1, n);
        }
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
