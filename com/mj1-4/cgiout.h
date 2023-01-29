/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




class dtmj_cgiout_c: 
  public dtmj_push_c
{

public:
static 
CUnknown *
CreateInstance(
  LPUNKNOWN punk, 
  HRESULT *phr);

										// implement virtuals in dtmj_push_c
STDMETHODIMP GetClassID(CLSID *pClsid);
LPAMOVIESETUP_FILTER GetSetupData();

HRESULT 
accept(
  dt_ctl_t *ctl,			
  const char *dgspec,	
  int *connected);

HRESULT 
hangup(
  dt_ctl_t *ctl,
  int *connected);

HRESULT 
stream(
  dt_ctl_t *ctl,
  const void *buffer,
  long length,
  int *connected);

HRESULT 
feedback(
  dt_ctl_t *ctl,
  char *cmd,
  long max);

private:
  dtmj_cgiout_c(CLSID clsid, TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);
  ~dtmj_cgiout_c();
}; 







/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */
