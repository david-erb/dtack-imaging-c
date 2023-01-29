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
| include $RCSfile: dtimage.h,v $ $Revision: 1.16 $
 *..........................................................................*/

#define DTIMAGE_SEG_MAXSIZE				/* max size of segment in image	    */\
  DTCONFIG_MALLOC_MAX

typedef union {
  void *dvoid;
  char *dchar;
  long  vlong;
  dtntype_b01_t *b01;					/* image data as bit				*/
  dtntype_b08_t *b08;					/* image data as unsigned char		*/
  dtntype_b16_t *b16;					/* image data as unsigned short		*/
  dtntype_b32_t *b32;					/* image data as unsigned long		*/
  dtntype_s1_t  *s1;					/* image data as signed char		*/
  dtntype_s2_t  *s2;					/* image data as signed short		*/
  dtntype_s4_t  *s4;					/* image data as signed long		*/
  dtntype_f1_t  *f1;					/* image data as float			  	*/
  dtntype_f2_t  *f2;					/* image data as double				*/
} dtimage_dataptr_t;

typedef struct {						/* image description structure		*/
  dtxy_t xe;							/* image width (x extent)			*/
  dtxy_t ye;							/* image height (y extend)			*/
  unsigned long flags;					/* DTIMAGE_FLAG values			    */
  unsigned int rowsize;					/* storage bytes needed by one row	*/
  dtimage_dataptr_t *row;				/* pointer to start of each row	    */
  dtntype_e ntype;						/* image data type					*/
  void *memalloc;
} dtimage_t;

										/* ................................ */
#define DTIMAGE_NOPOS 					/* value indicating no position		*/\
  ((dtxy_t)(-1))

										/* ................................ */
typedef
dt_rc_e 
DTCONFIG_API1
dtimage_pixel_f(						/* operate on pixel					*/
  dt_ctl_t *ctl,						/* environment control 				*/
  dtimage_t *image,						/* image pixel came from			*/
  dtxy_t x,								/* pixel column location			*/
  dtxy_t y,								/* pixel row location 				*/
  void *arg);							/* caller-defined value 			*/
										/* ................................ */
#define DTIMAGE_FLAG_FREE      (0x0001L)/* free image pixel space			*/
#define DTIMAGE_FLAG_PAGED     (0x0002L)/* image must be paged into memory	*/
#define DTIMAGE_FLAG_CONTIG    (0x0004L)/* rows are stored contiguously		*/
#define DTIMAGE_FLAG_SEGMENTED (0x0008L)/* image uses multiple segments		*/
										/* ................................ */
#define DTIMAGE_ADJ_FLAG_4    (0x0004)	/* 4-connected 						*/
#define DTIMAGE_ADJ_FLAG_8    (0x0008)	/* 8-connected 						*/
#define DTIMAGE_ADJ_FLAG_ONLY (0x0010)	/* keep only indicated pixels		*/
#define DTIMAGE_ADJ_FLAG_NOT  (0x0020)	/* don't keep indicated pixels		*/
										/* ................................ */
extern const char 
  **dtimage_gen_kernel_names;			/* predefined kernel names 			*/
										/* ................................ */
#include <dtack/image.p>					/* function prototypes				*/
										/* ................................ */
#define DTIMAGE_MEMALLOC(IMAGE) \
  ((dtmemalloc_t *)((IMAGE)->memalloc))

										/* ................................ */
#define DTIMAGE_MAPROWS_MAPPED(                                               \
  CTL, IMAGE, ROW, RAMPTR, DATASIZE)                                          \
{																			  \
  *(void **)(RAMPTR) =														  \
    (IMAGE)->row[(ROW)].dvoid;												  \
  if ((IMAGE)->flags &					/* rows are contiguous?				*/\
      DTIMAGE_FLAG_CONTIG)  												  \
  {																			  \
    if ((IMAGE)->flags &				/* image is segmented?				*/\
        DTIMAGE_FLAG_SEGMENTED)												  \
    { 																		  \
      long offset = (long)(ROW) *		/* offset into the image 			*/\
        (long)((IMAGE)->rowsize);                                             \
      unsigned int page =				/* page we are in 					*/\
        DTMEM_PAGE(CTL, 													  \
          (IMAGE)->memalloc, offset);										  \
      *(DATASIZE) = (unsigned int)		/* the rest of the page			    */\
        (DTIMAGE_MEMALLOC(IMAGE)->											  \
          page[page].datasize -												  \
        (unsigned int)DTMEM_OFFSET(CTL,	/* less how far into the page 		*/\
           (IMAGE)->memalloc, offset));										  \
	}																		  \
    else								/* image is not segmented? 			*/\
      *(DATASIZE) = (unsigned int)		/* the rest of the image		    */\
        ((long)((IMAGE)->rowsize) *        	                                  \
         (long)((IMAGE)->ye - (ROW)));                                        \
  }																			  \
  else									/* rows are not contiguous? 		*/\
    *(DATASIZE) = (IMAGE)->rowsize;		/* the rest of the row			    */\
}

#define DTIMAGE_GETROWS_MAPPED(                                               \
  CTL, IMAGE, ROW, RAMPTR, DATASIZE)                                          \
     DTIMAGE_MAPROWS_MAPPED(                                                  \
       CTL, IMAGE, ROW,                                                       \
       RAMPTR, DATASIZE)

#define DTIMAGE_MAPROW_MAPPED(                                                \
  CTL, IMAGE, ROW, RAMPTR)                                                    \
     {*(void **)(RAMPTR) =                                                    \
       (IMAGE)->row[(ROW)].dvoid;}

#define DTIMAGE_UNMAPROW_MAPPED(                                              \
  CTL, IMAGE, ROW) {}

#define DTIMAGE_GETROW_MAPPED(                                                \
  CTL, IMAGE, ROW, RAMPTR)                                                    \
     DTIMAGE_MAPROW_MAPPED(                                                   \
       CTL, IMAGE, ROW, RAMPTR)

#define DTIMAGE_PUTROW_MAPPED(                                                \
  CTL, IMAGE, ROW) {}

										/* ................................ */
#if DTPRJ_NOPAGING == DT_1				/* paging not allowed? 				*/

#  define DTIMAGE_MAPROWS2(CHECK, CTL, IMAGE, ROW, RAMPTR, DATASIZE)          \
   DTIMAGE_MAPROWS_MAPPED(CTL, IMAGE, ROW, RAMPTR, DATASIZE)

#  define DTIMAGE_GETROWS2(CHECK, CTL, IMAGE, ROW, RAMPTR, DATASIZE)          \
   DTIMAGE_GETROWS_MAPPED(CTL, IMAGE, ROW, RAMPTR, DATASIZE)

#  define DTIMAGE_MAPROW2(CHECK, CTL, IMAGE, ROW, RAMPTR)                     \
   DTIMAGE_MAPROW_MAPPED(CTL, IMAGE, ROW, RAMPTR)

#  define DTIMAGE_UNMAPROW2(CHECK, CTL, IMAGE, ROW)                           \
   DTIMAGE_UNMAPROW_MAPPED(CTL, IMAGE, ROW)

#  define DTIMAGE_GETROW2(CHECK, CTL, IMAGE, ROW, RAMPTR)                     \
   DTIMAGE_GETROW_MAPPED(CTL, IMAGE, ROW, RAMPTR)

#  define DTIMAGE_PUTROW2(CHECK, CTL, IMAGE, ROW)                             \
   DTIMAGE_PUTROW_MAPPED(CTL, IMAGE, ROW)
										/* ................................ */
#else  									/* paging allowed? 					*/

# ifndef DTIMAGE_MAPROWS2
# define DTIMAGE_MAPROWS2(CHECK, CTL, IMAGE,								  \
           ROW, RAMPTR, DATASIZE)											  \
  {                                                                           \
     if (!((IMAGE)->flags &				/* image is not paged? 				*/\
           DTIMAGE_FLAG_PAGED))                                               \
       DTIMAGE_MAPROWS_MAPPED(CTL,		/* do it the easy way 				*/\
         IMAGE, ROW, RAMPTR, DATASIZE)                                        \
     else								/* image is paged? 					*/\
     {                                                                        \
       unsigned int page;                                                     \
       if (DTMEM_ISMAPPED(CTL,			/* desired page is mapped?			*/\
             (IMAGE)->memalloc,                                               \
             (IMAGE)->row[(ROW)].vlong,                                       \
             &page))                                                          \
         DTMEM_LINK(CTL,				/* simply link and get info 		*/\
           (IMAGE)->memalloc,                                                 \
           (IMAGE)->row[(ROW)].vlong,                                         \
           page, RAMPTR, DATASIZE)                                            \
       else								/* desired page is not mapped?		*/\
         CHECK(																  \
           DTIMAGE_MEMALLOC(IMAGE)->map,									  \
             (CTL, (dtmemalloc_t *)(IMAGE)->memalloc,						  \
             (IMAGE)->row[(ROW)].vlong,                                       \
             (void **)(RAMPTR), 											  \
             DATASIZE))        												  \
     }                             											  \
   }
#  endif


#  ifndef DTIMAGE_GETROWS2
#  define DTIMAGE_GETROWS2(CHECK, CTL, IMAGE,								  \
            ROW, RAMPTR, DATASIZE) 											  \
  {                                                                           \
     if (!((IMAGE)->flags &				/* image is not paged? 				*/\
           DTIMAGE_FLAG_PAGED))                                               \
       DTIMAGE_GETROWS_MAPPED(CTL,		/* do it the easy way 				*/\
         IMAGE, ROW, RAMPTR, DATASIZE)                                        \
     else								/* image is paged? 					*/\
     {                                                                        \
       unsigned int page;                                                     \
       if (DTMEM_ISMAPPED(CTL,			/* desired page is mapped?			*/\
             (IMAGE)->memalloc,                                               \
             (IMAGE)->row[(ROW)].vlong,                                       \
             &page))                                                          \
         DTMEM_LINK(CTL,				/* simply link and get info 		*/\
           (IMAGE)->memalloc,                                                 \
           (IMAGE)->row[(ROW)].vlong,                                         \
           page, RAMPTR, DATASIZE)                                            \
       else								/* desired page is not mapped?		*/\
         CHECK(																  \
           DTIMAGE_MEMALLOC(IMAGE)->get,									  \
             (CTL, (dtmemalloc_t *)(IMAGE)->memalloc,						  \
             (IMAGE)->row[(ROW)].vlong,                                       \
             (void **)(RAMPTR), 											  \
             DATASIZE))        												  \
     }                             											  \
   }
#  endif


#  ifndef DTIMAGE_MAPROW2
#  define DTIMAGE_MAPROW2(CHECK, CTL, IMAGE,								  \
            ROW, RAMPTR) 													  \
  {                                                                           \
     if (!((IMAGE)->flags &				/* image is not paged? 				*/\
           DTIMAGE_FLAG_PAGED))                                               \
       DTIMAGE_MAPROW_MAPPED(CTL,		/* do it the easy way 				*/\
         IMAGE, ROW, RAMPTR)                                                  \
     else								/* image is paged? 					*/\
     {                                                                        \
       unsigned int page;                                                     \
       if (DTMEM_ISMAPPED(CTL,			/* desired page is mapped?			*/\
             (IMAGE)->memalloc,                                               \
             (IMAGE)->row[(ROW)].vlong,                                       \
             &page))                                                          \
         DTMEM_LINK(CTL,				/* simply link and get info 		*/\
           (IMAGE)->memalloc,                                                 \
           (IMAGE)->row[(ROW)].vlong,                                         \
           page, RAMPTR, &page)												  \
       else								/* desired page is not mapped?		*/\
         CHECK(																  \
           DTIMAGE_MEMALLOC(IMAGE)->map,									  \
             (CTL, (dtmemalloc_t *)(IMAGE)->memalloc,						  \
             (IMAGE)->row[(ROW)].vlong,                                       \
             (void **)(RAMPTR), 											  \
             (unsigned int *)NULL))    										  \
     }                             											  \
   }
#  endif


#  ifndef DTIMAGE_GETROW2
#  define DTIMAGE_GETROW2(CHECK, CTL, IMAGE,								  \
            ROW, RAMPTR)													  \
  {                                                                           \
     if (!((IMAGE)->flags &				/* image is not paged? 				*/\
           DTIMAGE_FLAG_PAGED))                                               \
       DTIMAGE_GETROW_MAPPED(CTL,		/* do it the easy way 				*/\
         IMAGE, ROW, RAMPTR)                                                  \
     else								/* image is paged? 					*/\
     {                                                                        \
       unsigned int page;                                                     \
       if (DTMEM_ISMAPPED(CTL,			/* desired page is mapped?			*/\
             (IMAGE)->memalloc,                                               \
             (IMAGE)->row[(ROW)].vlong,                                       \
             &page))                                                          \
         DTMEM_LINK(CTL,				/* simply link and get info 		*/\
           (IMAGE)->memalloc,                                                 \
           (IMAGE)->row[(ROW)].vlong,                                         \
           page, RAMPTR, &page)												  \
       else								/* desired page is not mapped?		*/\
         CHECK(																  \
           DTIMAGE_MEMALLOC(IMAGE)->get,									  \
             (CTL, (dtmemalloc_t *)(IMAGE)->memalloc,						  \
             (IMAGE)->row[(ROW)].vlong,                                       \
             (void **)(RAMPTR), 											  \
             (unsigned int *)NULL))    										  \
     }                             											  \
   }
#  endif


#  ifndef DTIMAGE_UNMAPROW2
#  define DTIMAGE_UNMAPROW2(CHECK, CTL, IMAGE, 								  \
            ROW)       														  \
  {                                                                           \
     if (!((IMAGE)->flags &				/* image is not paged? 				*/\
           DTIMAGE_FLAG_PAGED))                                               \
       DTIMAGE_UNMAPROW_MAPPED(CTL,		/* do it the easy way 				*/\
         IMAGE, ROW)                                                          \
     else								/* image is paged? 					*/\
     {                                                                        \
       unsigned int page;                                                     \
       DTMEMSTD_UNMAP2(CHECK, CTL,	                                          \
         (IMAGE)->memalloc,                                                   \
         (IMAGE)->row[(ROW)].vlong,                                           \
         page);																  \
     }                             											  \
   }
#  endif



#  ifndef DTIMAGE_PUTROW2
#  define DTIMAGE_PUTROW2(CHECK, CTL, IMAGE,								  \
            ROW)                   											  \
  {                                                                           \
     if (!((IMAGE)->flags &				/* image is not paged? 				*/\
           DTIMAGE_FLAG_PAGED))                                               \
       DTIMAGE_UNMAPROW_MAPPED(CTL,		/* do it the easy way 				*/\
         IMAGE, ROW)                                                          \
     else								/* image is paged? 					*/\
     {                                                                        \
       unsigned int page;                                                     \
       DTMEMSTD_PUT2(CHECK, CTL,	                                          \
         (IMAGE)->memalloc,                                                   \
         (IMAGE)->row[(ROW)].vlong,                                           \
         page);																  \
     }                             											  \
   }
#  endif

#endif

										/* ................................ */
#define DTIMAGE_GETPIXN(CHECK,  	        								  \
          CTL, IMAGE, X, Y, TYPE, VALUE)									  \
  {																			  \
    void *dt_tmp_p;															  \
    DTIMAGE_GETROW2(CHECK, CTL, IMAGE, Y, &dt_tmp_p);						  \
    *VALUE = ((TYPE *)dt_tmp_p)[(X)];										  \
    DTIMAGE_UNMAPROW2(CHECK, CTL, IMAGE, Y);								  \
  }

										/* ................................ */
#define DTIMAGE_SETPIXN(CHECK,												  \
          CTL, IMAGE, X, Y, TYPE, VALUE)									  \
  {																			  \
    void *dt_tmp_p;															  \
    DTIMAGE_GETROW2(CHECK, CTL, IMAGE, Y, &dt_tmp_p);						  \
    ((TYPE *)dt_tmp_p)[(X)] = VALUE;										  \
    DTIMAGE_PUTROW2(CHECK, CTL, IMAGE, Y);									  \
  }

										/* ................................ */
#define DTIMAGE_GETPIX1(CHECK, CTL,                                           \
          IMAGE, X, Y, VALUE)		  	    	 							  \
  {																			  \
    void *dt_tmp_p;															  \
    DTIMAGE_GETROW2(CHECK, CTL, IMAGE, Y, &dt_tmp_p);						  \
    DTNTYPE_GET1(CHECK, CTL, dt_tmp_p, X, (IMAGE)->ntype, VALUE);       	  \
    DTIMAGE_UNMAPROW2(CHECK, CTL, IMAGE, Y);								  \
  }
										/* ................................ */
#define DTIMAGE_SETPIX1(CHECK, CTL,                                           \
          IMAGE, X, Y, VALUE)			      	 							  \
  {																			  \
    void *dt_tmp_p;															  \
    DTIMAGE_GETROW2(CHECK, CTL, IMAGE, Y, &dt_tmp_p);						  \
    DTNTYPE_SET1(CHECK, CTL, dt_tmp_p, X, (IMAGE)->ntype, VALUE);   		  \
    DTIMAGE_PUTROW2(CHECK, CTL, IMAGE, Y);	    							  \
  }
										/* ................................ */
#define DTIMAGE_SETPIXS(CHECK, CTL, 		                                  \
          IMAGE, X1, X2, Y, VALUE)			   	 							  \
  {																			  \
    void *dt_tmp_p;															  \
    DTIMAGE_GETROW2(CHECK, CTL, IMAGE, Y, &dt_tmp_p);						  \
    DTNTYPE_SETS(CHECK, CTL, dt_tmp_p, X1, X2, (IMAGE)->ntype, VALUE); 	  	  \
    DTIMAGE_PUTROW2(CHECK, CTL, IMAGE, Y);	    							  \
  }

										/* ................................ */

#  define DTIMAGE_MAPROWS(CTL, IMAGE, ROW, RAMPTR, DATASIZE)                  \
   DTIMAGE_MAPROWS2(DT_Q, CTL, IMAGE, ROW, RAMPTR, DATASIZE)

#  define DTIMAGE_GETROWS(CTL, IMAGE, ROW, RAMPTR, DATASIZE)                  \
   DTIMAGE_GETROWS2(DT_Q, CTL, IMAGE, ROW, RAMPTR, DATASIZE)

#  define DTIMAGE_MAPROW(CTL, IMAGE, ROW, RAMPTR)                             \
   DTIMAGE_MAPROW2(DT_Q, CTL, IMAGE, ROW, RAMPTR)

#  define DTIMAGE_UNMAPROW(CTL, IMAGE, ROW)                                   \
   DTIMAGE_UNMAPROW2(DT_Q, CTL, IMAGE, ROW)

#  define DTIMAGE_GETROW(CTL, IMAGE, ROW, RAMPTR)                             \
   DTIMAGE_GETROW2(DT_Q, CTL, IMAGE, ROW, RAMPTR)

#  define DTIMAGE_PUTROW(CTL, IMAGE, ROW)                                     \
   DTIMAGE_PUTROW2(DT_Q, CTL, IMAGE, ROW)





/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
