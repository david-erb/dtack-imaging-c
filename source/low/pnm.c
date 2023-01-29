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

#include <dtack/base.h>
DT_RCSID("source/low $RCSfile: pnm.c,v $ $Revision: 1.1 $");
#include <dtack/pnm.h>
#include <dtack/str.h>


#define ISNEWLINE(C) ((C) == 0x0a || (C) == 0x0d)
#define ISWHITE(C) ((C) == 0x0a || (C) == 0x0d || (C) == 0x20 || (C) == 0x09)

/*..........................................................................
 * read a byte from a pnm file
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtpnm_read_byte(						/* read a byte from a pnm file		*/
  dt_ctl_t *ctl,
  dtpnm_t *pnm,
  dtfd_t *fd,
  const char *what,
  char *byte)
{
  DT_F("dtpnm_read_byte");
  long got;

  DT_Q(dtfd_read,(ctl, fd,
    byte, 1, &got));
  if (got != 1)
    return dt_err(ctl, F, "end of \"%s\" getting %s",
      fd->filename, what);

  while (*byte == '#')					/* start of comment?			    */
  {
    do {
      DT_Q(dtfd_read,(ctl, fd,
        byte, 1, &got));
      if (got != 1)
        return dt_err(ctl, F, "end of \"%s\" in comment getting %s",
          fd->filename, what);
    } while(!ISNEWLINE(*byte));

    DT_Q(dtfd_read,(ctl, fd,
      byte, 1, &got));
    if (got != 1)
      return dt_err(ctl, F, "end of \"%s\" after comment getting %s",
        fd->filename, what);
  }
  
  return DT_RC_GOOD;
}

/*..........................................................................
 * read an ascii double from a pnm file
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtpnm_read_double(						/* read ascii double from pnm file	*/
  dt_ctl_t *ctl,
  dtpnm_t *pnm,
  dtfd_t *fd,
  const char *what,
  double *val)
{
  DT_F("dtpnm_read_double");
  char byte;
  char s[32];							/* big enough for a double 			*/
  int i = 0;
  int isnumber;

  do {									/* skip leading whitespace		    */
    DT_Q(dtpnm_read_byte,(ctl,
      pnm, fd, what, &byte));
  } while(ISWHITE(byte));

  do {
	if (i >= (int)sizeof(s)-2)
      return dt_err(ctl, F,
        "string \"%s\" too long"
        " in \"%s\" getting %s",
        s, fd->filename, what);
    s[i++] = byte;
    s[i] = '\0';
    DT_Q(dtpnm_read_byte,(ctl,			/* get next byte from file		    */
      pnm, fd, what, &byte));
  } while(!ISWHITE(byte));
  
  DT_Q(dtstr_to_double2,(ctl, s, val,	/* convert to double 				*/
    &isnumber));

  if (!isnumber)
    return dt_err(ctl, F,
      "invalid number \"%s\" in \"%s\""
      " getting %s",
      s, fd->filename, what);
  return DT_RC_GOOD;
}

/*..........................................................................
 * read an ascii long integer from a pnm file
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtpnm_read_ulong(						/* read ascii long from a pnm file	*/
  dt_ctl_t *ctl,
  dtpnm_t *pnm,
  dtfd_t *fd,
  const char *what,
  unsigned long *val)
{
  DT_F("dtpnm_read_ulong");
  char byte;

  do {									/* skip leading whitespace		    */
    DT_Q(dtpnm_read_byte,(ctl,
      pnm, fd, what, &byte));
  } while(ISWHITE(byte));

  *val = 0;
  do {
    if (byte >= '0' && byte <= '9')		/* it's a decimal digit?		    */
      *val = *val * 10 + byte - '0';
    else
      return dt_err(ctl, F,
        "non-digit (0x%02x) in \"%s\" getting %s",
        byte, fd->filename, what);
    DT_Q(dtpnm_read_byte,(ctl,			/* get next byte from file		    */
      pnm, fd, what, &byte));
  } while(!ISWHITE(byte));
  return DT_RC_GOOD;
}

/*..........................................................................
 * read pnm header from opened file
 *..........................................................................*/


dt_rc_e
DTCONFIG_API1
dtpnm_read_header(						/* read pnm header from opened file	*/
  dt_ctl_t *ctl,
  dtpnm_t *pnm,
  dtfd_t *fd)
{
  DT_F("dtpnm_read_header");
  unsigned long ptype;
  unsigned long width;
  unsigned long height;
  char byte;
  dt_ntype_e ntype;

  DT_Q(dtpnm_read_byte,(ctl, pnm,
    fd, "magic number", &byte));
  if (byte != 'P')
    return dt_err(ctl, F,
      "magic number does not begin with P in \"%s\"",
      fd->filename);
  DT_Q(dtpnm_read_ulong,(ctl, pnm,
    fd, "magic number", &ptype));
  if (ptype != 2 &&
      ptype != 4 &&
      ptype != 5 &&
      ptype != DTPNM_COMPRESS_BPE + 4 &&
      ptype != DTPNM_COMPRESS_BPE + 5 &&
      ptype != DTPNM_COMPRESS_RAD + 5)
    return dt_err(ctl, F,
      "magic number P%lu in \"%s\" is not supported",
      ptype, fd->filename);

  DT_Q(dtpnm_read_ulong,(ctl, pnm,		/* read the width				    */
    fd, "width", &width));

  DT_Q(dtpnm_read_ulong,(ctl, pnm,		/* read the height				    */
    fd, "height", &height));

  if (ptype == 2 ||						/* one of the pgm types?		    */
	  ptype == 5 ||
	  ptype == DTPNM_COMPRESS_BPE + 5 ||
	  ptype == DTPNM_COMPRESS_RAD + 5)
  {
	double maxgray;
	int i;
    DT_Q(dtpnm_read_double,(ctl, pnm,
      fd, "maxgray", &maxgray));
	for (i=0;							/* first search for exact match 	*/
         dt_ntype_list[i] != 
         DT_NTYPE_NULL; 
         i++)
      if (maxgray == dt_ntype_maxval[i+1])
	    break;

	if (dt_ntype_list[i] ==				/* no exact match found? 			*/
		DT_NTYPE_NULL)
	for (i=0;							/* then search for nearest match 	*/
         dt_ntype_list[i] != 
         DT_NTYPE_NULL; 
         i++)
      if (maxgray <= dt_ntype_maxval[i+1])
	    break;

	if (dt_ntype_list[i] ==				/* still no match?					*/
		DT_NTYPE_NULL)
      return dt_err(ctl, F,
        "unsupported maxgray %e in \"%s\"",
        maxgray, fd->filename);
	ntype = dt_ntype_list[i];
  }
  else
	ntype = DT_NTYPE_B01;				/* this indicates a bit image	    */

  DT_MEMSET(pnm, 0, sizeof(*pnm));			/* clear header for return 			*/
  
  if (ptype / 100 == 					/* BPE compression on?			    */
	  DTPNM_COMPRESS_BPE / 100)
	pnm->compress =
	  DTPNM_COMPRESS_BPE;
  else
  if (ptype / 100 == 					/* RAD compression on?			    */
	  DTPNM_COMPRESS_RAD / 100)
	pnm->compress =
	  DTPNM_COMPRESS_RAD;
  pnm->ptype = (int)(ptype % 100);
  pnm->xe = width;	
  pnm->ye = height;
  pnm->ntype = ntype;

  return DT_RC_GOOD;
}

/*..........................................................................
 * write pnm header to opened file
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
dtpnm_write_header(						/* write pnm header to opened file	*/
  dt_ctl_t *ctl,
  dtpnm_t *pnm,
  dtfd_t *fd)
{
  DT_F("dtpnm_read_header");
  long compress;
  char header[80];

  if (pnm->compress ==					/* BPE oppression on?			    */
	  DTPNM_COMPRESS_BPE)
    compress = DTPNM_COMPRESS_BPE;
  else							
  if (pnm->compress ==					/* RAD oppression on?			    */
	  DTPNM_COMPRESS_RAD)
    compress = DTPNM_COMPRESS_RAD;
  else									/* no compression on?			    */
    compress = 0;

  if (pnm->magic == 1 ||				/* bit image?					    */
	  pnm->magic == 4)
  {
    sprintf(header,						/* make up the header				*/
      "P%ld\n%ld\n%ld\n",
	  pnm->magic + compress,
      pnm->xe, pnm->ye);
  }
  else
  if (pnm->magic == 2 ||				/* not bit image?					*/
	  pnm->magic == 5)
  {
	char maxgray_string[64];
	DT_Q(dtstr_print_ntype,(ctl,
	  dt_ntype_maxval_ptr[pnm->ntype],
      pnm->ntype,
      maxgray_string,
      sizeof(maxgray_string)));
    sprintf(header,						/* make up the header				*/
      "P%ld\n%ld\n%ld\n%s\n",
      pnm->magic + compress, 
      pnm->xe, pnm->ye, 
      maxgray_string);
  }
  else
    return dt_err(ctl, F,
      "magic %ld invalid",
      pnm->magic);

  DT_Q(dtfd_write_all,(ctl, fd,			/* write all header data			*/
    header, strlen(header)));

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
