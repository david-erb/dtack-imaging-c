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
| include $RCSfile: dtparam.h,v $ $Revision: 1.9 $
 *..........................................................................*/

typedef int dtparam_flag_t;

#define DTPARAM_FLAG_LENGTHY    (0x0002)
#define DTPARAM_FLAG_VALUE      (0x0004)
#define DTPARAM_FLAG_PARSE	    (0x0008)
#define DTPARAM_FLAG_ALLOC      (0x0010)
#define DTPARAM_FLAG_DEFAULT    (0x0040)
#define DTPARAM_FLAG_NODESC     (0x0080)
#define DTPARAM_FLAG_PRIMARY    (0x0100)
#define DTPARAM_FLAG_SECONDARY  (0x0200)
#define DTPARAM_FLAG_TERTIARY   (0x0400)
#define DTPARAM_FLAG_QUATERNARY (0x0800)

typedef struct {						/* algorithm parameter structure	*/
  long version;							/* structure version				*/
										/* --------------------------------	*/
#define DTPARAM_DECLARE(TYPE, D) \
  TYPE           D;          \
  const char   * D ## _desc; \
  const char   * D ## _help; \
  dtparam_flag_t D ## _flag; \
  int            D ## _order

  DTPARAM_DECLARE(double, b);
  DTPARAM_DECLARE(long,   x);
  DTPARAM_DECLARE(long,   y);
  DTPARAM_DECLARE(long,   n);
  DTPARAM_DECLARE(char *, dialog);
  DTPARAM_DECLARE(double, dt);
  DTPARAM_DECLARE(double, dx);
  DTPARAM_DECLARE(double, dy);
  DTPARAM_DECLARE(double, dz);
  DTPARAM_DECLARE(long,   w);
  DTPARAM_DECLARE(long,   h);
  DTPARAM_DECLARE(double, t);
  DTPARAM_DECLARE(double, lambda);
  DTPARAM_DECLARE(double, gamma);
  DTPARAM_DECLARE(double, omega);
  DTPARAM_DECLARE(double, phi);
  DTPARAM_DECLARE(double, psi);
  DTPARAM_DECLARE(double, sigma);
  DTPARAM_DECLARE(double, k);
  DTPARAM_DECLARE(char *, mem);
  DTPARAM_DECLARE(double, max);
  DTPARAM_DECLARE(double, min);
  DTPARAM_DECLARE(double, mu);
  DTPARAM_DECLARE(double, epsilon);
  DTPARAM_DECLARE(double, offset);
  DTPARAM_DECLARE(long,   reps);
  DTPARAM_DECLARE(double, foreground);
  DTPARAM_DECLARE(double, background);
  DTPARAM_DECLARE(const char *, color);
  DTPARAM_DECLARE(const char *, connect);
  DTPARAM_DECLARE(const char *, dbg);
  DTPARAM_DECLARE(const char *, datadir);
  DTPARAM_DECLARE(const char *, input);
  DTPARAM_DECLARE(const char *, keyboard);
  DTPARAM_DECLARE(const char *, path);
  DTPARAM_DECLARE(const char *, output);
  DTPARAM_DECLARE(const char *, dbgto);
  DTPARAM_DECLARE(const char *, errto);
  DTPARAM_DECLARE(const char *, sayto);
  DTPARAM_DECLARE(const char *, precision);
  DTPARAM_DECLARE(const char *, op);
  DTPARAM_DECLARE(const char *, screen);
  DTPARAM_DECLARE(const char *, win);
  DTPARAM_DECLARE(const char *, variant);
  DTPARAM_DECLARE(const char *, yield);
  DTPARAM_DECLARE(const char *, help);
  DTPARAM_DECLARE(const char *, HELP);
  DTPARAM_DECLARE(const char *, cache);
  DTPARAM_DECLARE(const char *, config);
  DTPARAM_DECLARE(const char *, ident);
  DTPARAM_DECLARE(const char *, name);
  DTPARAM_DECLARE(const char *, parent);
										/* --------------------------------	*/
  dt_ctl_t ctl;							/* program control values			*/
} dtparam_t;
/*..........................................................................*/
/* old form, when scope has dtparam_t *param declared: 						*/
#define DTPARAM_DEFAULT(PARAM, DEFAULT)	\
  DTPARAM_DEFAULT_P(param, PARAM, DEFAULT)

										/* set default value for param		*/
#define DTPARAM_DEFAULT_P(P, PARAM, DEFAULT)                                  \
{                                                                             \
  (P)->PARAM = DEFAULT;			        		/* value                    */\
  (P)->PARAM ## _flag |= DTPARAM_FLAG_DEFAULT;  /* this is a default		*/\
  (P)->PARAM ## _flag &= ~DTPARAM_FLAG_VALUE;	/* this is not run-time		*/\
  (P)->PARAM ## _flag |= DTPARAM_FLAG_PARSE;    /* ok to parse for this		*/\
}


/*..........................................................................*/
/* old form: */
#define DTPARAM_VALUE(PARAM, DEFAULT) DTPARAM_SET_VALUE(PARAM, DEFAULT)

#define DTPARAM_SET_VALUE(PARAM, DEFAULT)	                                  \
{                                                                             \
  if (!(param->PARAM ## _flag &			/* param has no run-time value?		*/\
        DTPARAM_FLAG_VALUE) &&                                                \
      !(param->PARAM ## _flag &			/* param has no default?			*/\
        DTPARAM_FLAG_DEFAULT))                                                \
  {                                                                           \
	param->PARAM = DEFAULT;				/* use default given here			*/\
    param->PARAM ## _flag |= DTPARAM_FLAG_DEFAULT;                            \
  }                                                                           \
}
/*..........................................................................*/

#define DTPARAM_GET_VALUE(PARAM, DEFAULT)                                     \
  (((param != NULL) &&					/* param structure exists? 			*/\
	(param->PARAM ## _flag &			/* param has parsed value?	    	*/\
     DTPARAM_FLAG_VALUE ||                                                    \
     param->PARAM ## _flag &			/* param has been assigned default?	*/\
     DTPARAM_FLAG_DEFAULT))?                                                  \
  param->PARAM: (DEFAULT))

/*..........................................................................*/

typedef 
dt_rc_e 
DTCONFIG_API1
dtparam_callback_f(
  dt_ctl_t *, 
  void *,
  char *, char *, char *, 
  dtparam_flag_t *);

#include <dtack/param.p>					/* function prototypes				*/











/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
