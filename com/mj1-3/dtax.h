/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#include <dtack/base.h>

typedef dt_rc_e dtax_rc_e;

#define DTAX_RC_GOOD DT_RC_GOOD
#define DTAX_RC_BAD DT_RC_BAD

#ifndef DTAX_F
#define DTAX_F(FUNC) static char *F = TEXT(FUNC);
#endif

#ifndef TEXT
#define TEXT(A) A
#endif

#define DTAX_C(FUNC, ARGS)				/* check rc and goto cleanup		*/\
{                                                                             \
  HRESULT hr;                                                                 \
  hr = FUNC ARGS;		                                                      \
  if (hr & 0x80000000LU)   		                                              \
  {                                                                           \
    rc = dt_err(ctl, F,                                                       \
      "%s failed hr=0x%08lx",                                                 \
      #FUNC, hr);                                                             \
    goto cleanup;		                                                      \
  }                                                                           \
  else                                                                        \
    rc = DT_RC_GOOD;                                                          \
}


#define DTAX_Q(FUNC, ARGS)				/* check rc and return				*/\
{                                                                             \
  HRESULT hr;                                                                 \
  hr = FUNC ARGS;		                                                      \
  if (hr & 0x80000000LU)   		                                              \
  {                                                                           \
    dt_err(ctl, F,                                                            \
      "%s failed hr=0x%08lx",                                                 \
      #FUNC, hr);                                                             \
    return hr;			                                                      \
  }                                                                           \
}


#define DTAX_I(FUNC, ARGS)				/* check rc and return				*/\
{                                                                             \
  HRESULT hr;                                                                 \
  hr = FUNC ARGS;		                                                      \
  if (hr & 0x80000000LU)   		                                              \
  {                                                                           \
    dt_rc_e trc =                                                             \
    dt_err(ctl, F,                                                            \
      "%s failed hr=0x%08lx",                                                 \
      #FUNC, hr);                                                             \
    if (rc == DT_RC_GOOD)                                                     \
      rc = trc;                                                               \
  }                                                                           \
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
