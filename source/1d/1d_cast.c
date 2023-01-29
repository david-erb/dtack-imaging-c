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
| Function return value will be DT1D_RC_GOOD on success.  
| Any other return value indicates failure or noncompletion.
| When DT1D_RC_GOOD is not returned, any other return values may not be valid.
| 
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <dtack/config.h>				/* platform-specific defines		*/
#include <dtack/ntype.h>
#include <dtack/stats.h>
#include <dtack/1d.h>

/*..........................................................................*/
#define BACK 0
#define FORE 1
static dt1d_ntype_b08_t table[16][4] = {
  {BACK, BACK, BACK, BACK},
  {BACK, BACK, BACK, FORE},
  {BACK, BACK, FORE, BACK},
  {BACK, BACK, FORE, FORE},
  {BACK, FORE, BACK, BACK},
  {BACK, FORE, BACK, FORE},
  {BACK, FORE, FORE, BACK},
  {BACK, FORE, FORE, FORE},
  {FORE, BACK, BACK, BACK},
  {FORE, BACK, BACK, FORE},
  {FORE, BACK, FORE, BACK},
  {FORE, BACK, FORE, FORE},
  {FORE, FORE, BACK, BACK},
  {FORE, FORE, BACK, FORE},
  {FORE, FORE, FORE, BACK},
  {FORE, FORE, FORE, FORE},
};

/*..........................................................................*/
#define CAST_TO_B01(PTYPE) \
  { \
    PTYPE            *       p = (PTYPE *)p1; \
    dt1d_ntype_b01_t * const q = (dt1d_ntype_b01_t *)q1; \
    dt1d_i_t x; \
	for (x=0; x<n; x++) \
	{ \
	  q[x] = (dt1d_ntype_b01_t)( \
             ((p[0] != 0) << 7) | \
             ((p[1] != 0) << 6) | \
             ((p[2] != 0) << 5) | \
             ((p[3] != 0) << 4) | \
             ((p[4] != 0) << 3) | \
             ((p[5] != 0) << 2) | \
             ((p[6] != 0) << 1) | \
             ((p[7] != 0))); \
	  p += 8; \
	} \
  } \
  *didit = 1; \
  break;

/*..........................................................................*/
#define CAST_FROM_B01(QTYPE) \
  { \
    dt1d_ntype_b01_t * const p = (dt1d_ntype_b01_t *)p1; \
    QTYPE          *       q = (QTYPE *)q1; \
    dt1d_i_t x; \
    dt1d_ntype_b08_t *t; \
    for (x=0; x<n; x++) \
    { \
	  t = table[p[x]>>4];				/* most significant 4 bits		    */\
	  q[0] = t[0]; \
	  q[1] = t[1]; \
	  q[2] = t[2]; \
	  q[3] = t[3]; \
	  t = table[p[x]&0xf];				/* least significant bits		    */\
	  q[4] = t[0]; \
	  q[5] = t[1]; \
	  q[6] = t[2]; \
	  q[7] = t[3]; \
	  q += 8; \
	} \
  } \
  *didit = 1; \
  break;

/*..........................................................................*/
static
dt1d_rc_e
dt1d_cast_to_b01(
  void *p1,
  void *q1,
  dt1d_i_t n,
  const dt1d_ntype_e ntype,
  int *didit)
{
  n /= 8;								/* address input as bytes		    */

    switch(ntype)
    {
      case   DT1D_NTYPE_B08:   CAST_TO_B01(dt1d_ntype_b08_t);
      case   DT1D_NTYPE_B16:   CAST_TO_B01(dt1d_ntype_b16_t);
      case   DT1D_NTYPE_B32:   CAST_TO_B01(dt1d_ntype_b32_t);
      case   DT1D_NTYPE_S1:    CAST_TO_B01(dt1d_ntype_s1_t);
      case   DT1D_NTYPE_S2:    CAST_TO_B01(dt1d_ntype_s2_t);
      case   DT1D_NTYPE_S4:    CAST_TO_B01(dt1d_ntype_s4_t);
      case   DT1D_NTYPE_F1:    CAST_TO_B01(dt1d_ntype_f1_t);
      case   DT1D_NTYPE_F2:    CAST_TO_B01(dt1d_ntype_f2_t);
	}

  return DT1D_RC_GOOD;
}

/*..........................................................................*/
static
dt1d_rc_e
from0_b01(
  void *p1,
  void *q1,
  dt1d_i_t n,
  const dt1d_ntype_e ntype,
  int *didit)
{
  n /= 8;								/* address input as bytes		    */

    switch(ntype)
    {
      case   DT1D_NTYPE_B08:   CAST_FROM_B01(dt1d_ntype_b08_t);
      case   DT1D_NTYPE_B16:   CAST_FROM_B01(dt1d_ntype_b16_t);
      case   DT1D_NTYPE_B32:   CAST_FROM_B01(dt1d_ntype_b32_t);
      case   DT1D_NTYPE_S1:    CAST_FROM_B01(dt1d_ntype_s1_t);
      case   DT1D_NTYPE_S2:    CAST_FROM_B01(dt1d_ntype_s2_t);
      case   DT1D_NTYPE_S4:    CAST_FROM_B01(dt1d_ntype_s4_t);
      case   DT1D_NTYPE_F1:    CAST_FROM_B01(dt1d_ntype_f1_t);
      case   DT1D_NTYPE_F2:    CAST_FROM_B01(dt1d_ntype_f2_t);
	}

  return DT1D_RC_GOOD;
}

/*..........................................................................*/
/*..........................................................................*/
#define CAST1(TYPE) 					\
  memcpy(q1, p1, n*sizeof(TYPE)); 		\
  *didit = 1; 							\
  break;

/*..........................................................................*/
#define CAST2(PTYPE, QTYPE) \
  { \
    PTYPE * const p = (PTYPE *)p1; \
    QTYPE * const q = (QTYPE *)q1; \
    dt1d_i_t x; \
    for (x=0; x<n; x++) \
      q[x] = (QTYPE)p[x]; \
  } \
  *didit = 1; \
  break;

/*..........................................................................*/
static
dt1d_rc_e
from0_b08(
  void *p1,
  void *q1,
  dt1d_i_t n,
  const dt1d_ntype_e ntype,
  int *didit)
{
    switch(ntype)
    {
      case   DT1D_NTYPE_B08:   CAST1(dt1d_ntype_b08_t);
      case   DT1D_NTYPE_B16:   CAST2(dt1d_ntype_b08_t, dt1d_ntype_b16_t);
      case   DT1D_NTYPE_B32:   CAST2(dt1d_ntype_b08_t, dt1d_ntype_b32_t);
      case   DT1D_NTYPE_S1:    CAST2(dt1d_ntype_b08_t, dt1d_ntype_s1_t);
      case   DT1D_NTYPE_S2:    CAST2(dt1d_ntype_b08_t, dt1d_ntype_s2_t);
      case   DT1D_NTYPE_S4:    CAST2(dt1d_ntype_b08_t, dt1d_ntype_s4_t);
      case   DT1D_NTYPE_F1:    CAST2(dt1d_ntype_b08_t, dt1d_ntype_f1_t);
      case   DT1D_NTYPE_F2:    CAST2(dt1d_ntype_b08_t, dt1d_ntype_f2_t);
	}

  return DT1D_RC_GOOD;
}
/*..........................................................................*/
static
dt1d_rc_e
from0_b16(
  void *p1,
  void *q1,
  dt1d_i_t n,
  const dt1d_ntype_e ntype,
  int *didit)
{
    switch(ntype)
    {
      case   DT1D_NTYPE_B08:   CAST2(dt1d_ntype_b16_t, dt1d_ntype_b08_t);
      case   DT1D_NTYPE_B16:   CAST1(dt1d_ntype_b16_t);
      case   DT1D_NTYPE_B32:   CAST2(dt1d_ntype_b16_t, dt1d_ntype_b32_t);
      case   DT1D_NTYPE_S1:    CAST2(dt1d_ntype_b16_t, dt1d_ntype_s1_t);
      case   DT1D_NTYPE_S2:    CAST2(dt1d_ntype_b16_t, dt1d_ntype_s2_t);
      case   DT1D_NTYPE_S4:    CAST2(dt1d_ntype_b16_t, dt1d_ntype_s4_t);
      case   DT1D_NTYPE_F1:    CAST2(dt1d_ntype_b16_t, dt1d_ntype_f1_t);
      case   DT1D_NTYPE_F2:    CAST2(dt1d_ntype_b16_t, dt1d_ntype_f2_t);
	}

  return DT1D_RC_GOOD;
}
/*..........................................................................*/
static
dt1d_rc_e
from0_b32(
  void *p1,
  void *q1,
  dt1d_i_t n,
  const dt1d_ntype_e ntype,
  int *didit)
{
    switch(ntype)
    {
      case   DT1D_NTYPE_B08:   CAST2(dt1d_ntype_b32_t, dt1d_ntype_b08_t);
      case   DT1D_NTYPE_B16:   CAST2(dt1d_ntype_b32_t, dt1d_ntype_b16_t);
      case   DT1D_NTYPE_B32:   CAST1(dt1d_ntype_b32_t);
      case   DT1D_NTYPE_S1:    CAST2(dt1d_ntype_b32_t, dt1d_ntype_s1_t);
      case   DT1D_NTYPE_S2:    CAST2(dt1d_ntype_b32_t, dt1d_ntype_s2_t);
      case   DT1D_NTYPE_S4:    CAST2(dt1d_ntype_b32_t, dt1d_ntype_s4_t);
      case   DT1D_NTYPE_F1:    CAST2(dt1d_ntype_b32_t, dt1d_ntype_f1_t);
      case   DT1D_NTYPE_F2:    CAST2(dt1d_ntype_b32_t, dt1d_ntype_f2_t);
	}

  return DT1D_RC_GOOD;
}
/*..........................................................................*/
static
dt1d_rc_e
from0_s1(
  void *p1,
  void *q1,
  dt1d_i_t n,
  const dt1d_ntype_e ntype,
  int *didit)
{
    switch(ntype)
    {
      case   DT1D_NTYPE_B08:   CAST2(dt1d_ntype_s1_t, dt1d_ntype_b08_t);
      case   DT1D_NTYPE_B16:   CAST2(dt1d_ntype_s1_t, dt1d_ntype_b16_t);
      case   DT1D_NTYPE_B32:   CAST2(dt1d_ntype_s1_t, dt1d_ntype_b32_t);
      case   DT1D_NTYPE_S1:    CAST1(dt1d_ntype_s1_t);
      case   DT1D_NTYPE_S2:    CAST2(dt1d_ntype_s1_t, dt1d_ntype_s2_t);
      case   DT1D_NTYPE_S4:    CAST2(dt1d_ntype_s1_t, dt1d_ntype_s4_t);
      case   DT1D_NTYPE_F1:    CAST2(dt1d_ntype_s1_t, dt1d_ntype_f1_t);
      case   DT1D_NTYPE_F2:    CAST2(dt1d_ntype_s1_t, dt1d_ntype_f2_t);
	}

  return DT1D_RC_GOOD;
}
/*..........................................................................*/
static
dt1d_rc_e
from0_s2(
  void *p1,
  void *q1,
  dt1d_i_t n,
  const dt1d_ntype_e ntype,
  int *didit)
{
    switch(ntype)
    {
      case   DT1D_NTYPE_B08:   CAST2(dt1d_ntype_s2_t, dt1d_ntype_b08_t);
      case   DT1D_NTYPE_B16:   CAST2(dt1d_ntype_s2_t, dt1d_ntype_b16_t);
      case   DT1D_NTYPE_B32:   CAST2(dt1d_ntype_s2_t, dt1d_ntype_b32_t);
      case   DT1D_NTYPE_S1:    CAST2(dt1d_ntype_s2_t, dt1d_ntype_s1_t);
      case   DT1D_NTYPE_S2:    CAST1(dt1d_ntype_s2_t);
      case   DT1D_NTYPE_S4:    CAST2(dt1d_ntype_s2_t, dt1d_ntype_s4_t);
      case   DT1D_NTYPE_F1:    CAST2(dt1d_ntype_s2_t, dt1d_ntype_f1_t);
      case   DT1D_NTYPE_F2:    CAST2(dt1d_ntype_s2_t, dt1d_ntype_f2_t);
	}

  return DT1D_RC_GOOD;
}
/*..........................................................................*/
static
dt1d_rc_e
from0_s4(
  void *p1,
  void *q1,
  dt1d_i_t n,
  const dt1d_ntype_e ntype,
  int *didit)
{
    switch(ntype)
    {
      case   DT1D_NTYPE_B08:   CAST2(dt1d_ntype_s4_t, dt1d_ntype_b08_t);
      case   DT1D_NTYPE_B16:   CAST2(dt1d_ntype_s4_t, dt1d_ntype_b16_t);
      case   DT1D_NTYPE_B32:   CAST2(dt1d_ntype_s4_t, dt1d_ntype_b32_t);
      case   DT1D_NTYPE_S1:    CAST2(dt1d_ntype_s4_t, dt1d_ntype_s1_t);
      case   DT1D_NTYPE_S2:    CAST2(dt1d_ntype_s4_t, dt1d_ntype_s2_t);
      case   DT1D_NTYPE_S4:    CAST1(dt1d_ntype_s4_t);
      case   DT1D_NTYPE_F1:    CAST2(dt1d_ntype_s4_t, dt1d_ntype_f1_t);
      case   DT1D_NTYPE_F2:    CAST2(dt1d_ntype_s4_t, dt1d_ntype_f2_t);
	}

  return DT1D_RC_GOOD;
}
/*..........................................................................*/
static
dt1d_rc_e
from0_f1(
  void *p1,
  void *q1,
  dt1d_i_t n,
  const dt1d_ntype_e ntype,
  int *didit)
{
    switch(ntype)
    {
      case   DT1D_NTYPE_B08:   CAST2(dt1d_ntype_f1_t, dt1d_ntype_b08_t);
      case   DT1D_NTYPE_B16:   CAST2(dt1d_ntype_f1_t, dt1d_ntype_b16_t);
      case   DT1D_NTYPE_B32:   CAST2(dt1d_ntype_f1_t, dt1d_ntype_b32_t);
      case   DT1D_NTYPE_S1:    CAST2(dt1d_ntype_f1_t, dt1d_ntype_s1_t);
      case   DT1D_NTYPE_S2:    CAST2(dt1d_ntype_f1_t, dt1d_ntype_s2_t);
      case   DT1D_NTYPE_S4:    CAST2(dt1d_ntype_f1_t, dt1d_ntype_s4_t);
      case   DT1D_NTYPE_F1:    CAST1(dt1d_ntype_f1_t);
      case   DT1D_NTYPE_F2:    CAST2(dt1d_ntype_f1_t, dt1d_ntype_f2_t);
	}

  return DT1D_RC_GOOD;
}
/*..........................................................................*/
static
dt1d_rc_e
from0_f2(
  void *p1,
  void *q1,
  dt1d_i_t n,
  const dt1d_ntype_e ntype,
  int *didit)
{
    switch(ntype)
    {
      case   DT1D_NTYPE_B08:   CAST2(dt1d_ntype_f2_t, dt1d_ntype_b08_t);
      case   DT1D_NTYPE_B16:   CAST2(dt1d_ntype_f2_t, dt1d_ntype_b16_t);
      case   DT1D_NTYPE_B32:   CAST2(dt1d_ntype_f2_t, dt1d_ntype_b32_t);
      case   DT1D_NTYPE_S1:    CAST2(dt1d_ntype_f2_t, dt1d_ntype_s1_t);
      case   DT1D_NTYPE_S2:    CAST2(dt1d_ntype_f2_t, dt1d_ntype_s2_t);
      case   DT1D_NTYPE_S4:    CAST2(dt1d_ntype_f2_t, dt1d_ntype_s4_t);
      case   DT1D_NTYPE_F1:    CAST2(dt1d_ntype_f2_t, dt1d_ntype_f1_t);
      case   DT1D_NTYPE_F2:    CAST1(dt1d_ntype_f2_t);
	}

  return DT1D_RC_GOOD;
}

/*..........................................................................*/
dt1d_rc_e
DT1D_API
dt1d_cast(
  void *vin,
  dt1d_i_t nin,
  dt1d_ntype_e in_ntype,
  void *vout,
  dt1d_i_t nout,
  dt1d_ntype_e out_ntype)
{
  int didit = 0;
  dt1d_rc_e rc = DT1D_RC_BAD;

  const dt1d_i_t n =					/* limit length to minimum		    */
    DT1D_MIN(nin, nout);

  if (in_ntype == DT1D_NTYPE_B01 &&
	  out_ntype == DT1D_NTYPE_B01)
  {	
	const char * const cin = (char *)vin;
	char * const cout = (char *)vout;
	const unsigned int m =
      DT1D_CAST(unsigned int,
        (double)n *
        dt1d_ntype_sizeof[in_ntype]);
	unsigned int i;
	for (i=0; i<m; i++)
      cout[i] = cin[i];
	rc = DT1D_RC_GOOD;
  }
  else
  if (out_ntype == DT1D_NTYPE_B01)
  {
	rc = dt1d_cast_to_b01(
      vin, vout, n, 
      in_ntype, &didit);
  }
  else
  {
    switch(in_ntype)
	{
	  case DT1D_NTYPE_B01: rc = from0_b01(
             vin, vout, n, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_B08: rc = from0_b08(
             vin, vout, n, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_B16: rc = from0_b16(
             vin, vout, n, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_B32: rc = from0_b32(
             vin, vout, n, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_S1:  rc = from0_s1(
             vin, vout, n, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_S2:  rc = from0_s2(
             vin, vout, n, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_S4:  rc = from0_s4(
             vin, vout, n, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_F1:  rc = from0_f1(
             vin, vout, n, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_F2:  rc = from0_f2(
             vin, vout, n, out_ntype, &didit);
      break;
	}
  }

  return rc;
}

/*..........................................................................*/
#define CAST(PTYPE, QTYPE) \
  { \
    PTYPE *p = (PTYPE *)p1 + poff; \
    QTYPE *q = (QTYPE *)q1 + offout; \
    PTYPE *mp = (PTYPE *)p1 + np; \
    QTYPE *mq = (QTYPE *)q1 + nq; \
    while(p<mp && q<mq) \
    { \
      *q = (QTYPE)(*p); \
      p += pinc; \
	  q += qinc; \
    } \
  } \
  *didit = 1; \
  break;

/*..........................................................................*/
#define BODY(T) 						\
static									\
dt1d_rc_e								\
from1_ ## T(							\
  void *p1,								\
  dt1d_i_t poff,						\
  dt1d_i_t pinc,						\
  dt1d_i_t np,							\
  void *q1,								\
  dt1d_i_t offout,						\
  dt1d_i_t qinc,						\
  dt1d_i_t nq,							\
  const dt1d_ntype_e ntype,				\
  int *didit)							\
{										\
    switch(ntype)						\
    {									\
      case   DT1D_NTYPE_B08:   CAST(dt1d_ntype_ ## T ## _t, dt1d_ntype_b08_t);\
      case   DT1D_NTYPE_B16:   CAST(dt1d_ntype_ ## T ## _t, dt1d_ntype_b16_t);\
      case   DT1D_NTYPE_B32:   CAST(dt1d_ntype_ ## T ## _t, dt1d_ntype_b32_t);\
      case   DT1D_NTYPE_S1:    CAST(dt1d_ntype_ ## T ## _t, dt1d_ntype_s1_t);\
      case   DT1D_NTYPE_S2:    CAST(dt1d_ntype_ ## T ## _t, dt1d_ntype_s2_t);\
      case   DT1D_NTYPE_S4:    CAST(dt1d_ntype_ ## T ## _t, dt1d_ntype_s4_t);\
      case   DT1D_NTYPE_F1:    CAST(dt1d_ntype_ ## T ## _t, dt1d_ntype_f1_t);\
      case   DT1D_NTYPE_F2:    CAST(dt1d_ntype_ ## T ## _t, dt1d_ntype_f2_t);\
	}									\
										\
  return DT1D_RC_GOOD;					\
}

BODY(b08)
BODY(b16)
BODY(b32)
BODY(s1)
BODY(s2)
BODY(s4)
BODY(f1)
BODY(f2)
	 
/*..........................................................................*/
dt1d_rc_e
DT1D_API
dt1d_cast2(
  void *vin,
  dt1d_i_t offin,
  dt1d_i_t incin,
  dt1d_i_t nin,
  dt1d_ntype_e in_ntype,
  void *vout,
  dt1d_i_t offout,
  dt1d_i_t incout,
  dt1d_i_t nout,
  dt1d_ntype_e out_ntype)
{
  int didit = 0;
  dt1d_rc_e rc = DT1D_RC_BAD;

  switch(in_ntype)
  {
	  case DT1D_NTYPE_B08: rc = from1_b08(
             vin, offin, incin, nin, vout, offout, incout, nout, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_B16: rc = from1_b16(
             vin, offin, incin, nin, vout, offout, incout, nout, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_B32: rc = from1_b32(
             vin, offin, incin, nin, vout, offout, incout, nout, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_S1:  rc = from1_s1(
             vin, offin, incin, nin, vout, offout, incout, nout, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_S2:  rc = from1_s2(
             vin, offin, incin, nin, vout, offout, incout, nout, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_S4:  rc = from1_s4(
             vin, offin, incin, nin, vout, offout, incout, nout, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_F1:  rc = from1_f1(
             vin, offin, incin, nin, vout, offout, incout, nout, out_ntype, &didit);
      break;
	  case DT1D_NTYPE_F2:  rc = from1_f2(
             vin, offin, incin, nin, vout, offout, incout, nout, out_ntype, &didit);
      break;
  }

  if (!didit)
	rc = dt1d_err_ntype(in_ntype, dt1d_ntype_list);
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
