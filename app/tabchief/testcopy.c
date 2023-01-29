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
| ERRORS
| As yet undocumented.
| 
| END
 *..........................................................................*/

#include <tabchief.h>
DT_RCSID("app/tabchief $RCSfile: testcopy.c,v $ $Revision: 1.8 $");

#define TEST_SIZE (256)

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabchief_test_genfile(
  dtparam_t *param,
  const char *filename)
{
  DT_F("tabchief_test_genfile");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  unsigned char b[TEST_SIZE];
  unsigned int i;
  dt_rc_e rc;

  fd->fd = -1;
  DT_C(dtfd_open,(ctl, fd, filename, "w"));
  for (i=0; i<sizeof(b); i++)
    b[i] = i;
  DT_C(dtfd_write_all,(ctl, fd, 
    b, sizeof(b)));
cleanup:
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

static
dt_rc_e
tabchief_test_checkfile(
  dtparam_t *param,
  const char *filename)
{
  DT_F("tabchief_test_checkfile");
  dt_ctl_t *ctl = &param->ctl;
  dtfd_t _fd, *fd = &_fd;
  unsigned char b[TEST_SIZE];
  unsigned int i;
  dt_rc_e rc;

  fd->fd = -1;
  DT_C(dtfd_open,(ctl, fd, filename, "r"));
  DT_C(dtfd_read_all,(ctl, fd, 
    b, sizeof(b)));
  for (i=0; i<sizeof(b); i++)
	if (b[i] != i)
      break;
  dt_assert(ctl, i == sizeof(b), F,
    "%d of %d bytes correct",
    i, sizeof(b));
cleanup:
  if (fd->fd != -1)
    DT_I(dtfd_close,(ctl, fd));
  return rc;
}

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabchief_test_copy(
  dtparam_t *param,
  const char *ipcspec)
{
  DT_F("tabchief_test_copy");
  dt_ctl_t *ctl = &param->ctl;
  dt_rc_e rc;

  DT_C(tabchief_test_genfile,(param,	/* generate test file 				*/
    param->input));

  DT_C(tabchief_client,(				/* copy file to server 				*/
    param, ipcspec, "copyto"));

  DT_C(dtos_unlink,(ctl, 
    (char *)param->input));

  {
	const char *tmp = param->input;
	param->input = param->output;
	param->output = tmp;
  }

  DT_C(tabchief_client,(				/* copy file back from server		*/
    param, ipcspec, "copyfrom"));

  DT_C(tabchief_test_checkfile,(param,	/* check test file 					*/
    param->output));

  DT_C(dtos_unlink,(ctl,				/* remove temporary file 			*/
    (char *)param->output));

cleanup:

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
