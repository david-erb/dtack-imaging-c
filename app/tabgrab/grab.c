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
| main program
| 
| END
 *..........................................................................*/

#include <tabgrab.h>
DT_RCSID("app/tabgrab $RCSfile: grab.c,v $ $Revision: 1.8 $");
#include <dtack/msw.h>

/*..........................................................................
 *..........................................................................*/

dt_rc_e
DTCONFIG_API1
tabgrab_grab(
  tabgrab_t *tabgrab)
{
  DT_F("tabgrab_grab");
  dt_ctl_t *ctl = tabgrab->ctl;
  tableau_header_t *header;
  tableau_header_t _header;
  void *b;
  int have_semaphore = 0;
  dt_rc_e rc;

  if (TABGRAB_IS_SHM(tabgrab))			/* we are using memory mapped file? */
  {
# ifdef DTCONFIG_IMPLIED_MSW32			/* on Win32? 						*/
	long result;
	header = (tableau_header_t *)
      tabgrab->b;
	b = (char *)tabgrab->b + 
      sizeof(tableau_header_t);
	if (TABGRAB_DO_LOCK(tabgrab))
	{
      result = WaitForSingleObject(		/* wait for OK to grab			    */
        (HANDLE)tabgrab->lock, 2000);
	  if (result == WAIT_TIMEOUT)
	  {
	    rc = dt_err(ctl, F,
          "timeout waiting for semaphore");
		goto cleanup;
	  }
	  have_semaphore = 1;
	}
# endif
  }
  else
  {
	header = &_header;
	b = tabgrab->b;
  }

  memset(header, 0,						/* clear header 					*/
    sizeof(tableau_header_t));
  header->l = tabgrab->l;
  header->n = tabgrab->frame;
  header->org = tabgrab->fb.org.org;	/* return org type we deliver	    */
  header->xe = tabgrab->fb.rowsize;
  header->ye = tabgrab->fb.ye;
  strcpy(header->codec,					/* deliver as uncompressed data 	*/
    TABLEAU_CODEC_UNCOMPRESSED);
  strcpy(header->cam, "cam0");

  DT_C(dtfb_control,(ctl, &tabgrab->fb,	/* start acquiring again			*/
    DTFB_CONTROL_ACQUIRE));

  DT_Q(dtfb_getblock,(ctl, 
    &tabgrab->fb,						/* suck bytes out 					*/
    b, 0, 0, 
    tabgrab->fb.xe, tabgrab->fb.ye));

  if (!tabgrab->frame)					/* first frame? 					*/
	DT_C(dtos_time,(ctl,				/* time AFTER first frame grab 		*/
      &tabgrab->beg));
  
  if (TABGRAB_IS_SHM(tabgrab))			/* we are using memory mapped file? */
  {
  }
  else									/* not using memory mapped file? 	*/
  {
	long position;
    DT_C(dtfd_seek,(ctl, &tabgrab->fd,	/* go back to start of file			*/
      0, DTFD_WHENCE_SET, &position));

	DT_C(dtfd_write_all,(ctl, 			/* write compression header to file	*/
      &tabgrab->fd,
      header, sizeof(*header)));

	DT_C(dtfd_write_all,(ctl,			/* write data to file				*/
      &tabgrab->fd,
      tabgrab->b, tabgrab->l));

	DT_C(dtfd_write_all,(ctl,			/* flush the file					*/
      &tabgrab->fd,  NULL, 0));
  }

  tabgrab->frame++;						/* increment frame number			*/
  
cleanup:
# ifdef DTCONFIG_IMPLIED_MSW32			/* on Win32? 						*/
  if (have_semaphore)
	ReleaseSemaphore(					/* signal grab is done				*/
      (HANDLE)tabgrab->lock, 1,
      NULL);
# endif

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
