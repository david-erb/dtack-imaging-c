/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */





#define DTMJ_PUSH_CONNECTED_NOT (0)
#define DTMJ_PUSH_CONNECTED_TEXTPUSH (1)
#define DTMJ_PUSH_CONNECTED_JPEGPUSH (2)
#define DTMJ_PUSH_CONNECTED_DISCONNECTED (3)

class dtmj_push_c: 
  public CBaseRenderer,
  public dtmj_push_prop_i,
  public ISpecifyPropertyPages,
  public CPersistStream
{

public:

  DECLARE_IUNKNOWN;

  STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

  dtmj_push_c(
    CLSID clsid, 
    TCHAR *tszName, 
    LPUNKNOWN punk, 
    HRESULT *phr);

  ~dtmj_push_c();

  HRESULT dtmj_push_c::DoRenderSample(IMediaSample *isample);
  HRESULT dtmj_push_c::CheckMediaType(const CMediaType *media);
  HRESULT dtmj_push_c::SetMediaType(const CMediaType *Media);

  STDMETHODIMP GetPages(CAUUID *pages);	// return property page(s)
  STDMETHODIMP get_prop(				// get properties
    dtmj_push_prop_t *prop);
  STDMETHODIMP put_prop(				// set properties
    dtmj_push_prop_t *prop, 
    char *message, int max);
  STDMETHODIMP put_ctl(					// set ctl environment
    dt_ctl_t *ctl);

  virtual STDMETHODIMP GetClassID(CLSID *pClsid) = 0;
  HRESULT WriteToStream(IStream *pStream);
  HRESULT ReadFromStream(IStream *pStream);
  int SizeMax();
  DWORD GetSoftwareVersion();

  virtual LPAMOVIESETUP_FILTER GetSetupData() = 0;

										/* ................................ */
virtual
HRESULT 
accept(
  dt_ctl_t *ctl,			
  const char *dgspec,	
  int *connected) = 0;

virtual
HRESULT 
hangup(
  dt_ctl_t *ctl,
  int *connected) = 0;

virtual
HRESULT 
stream(
  dt_ctl_t *ctl,
  const void *buffer,
  long length,
  int *connected) = 0;

virtual
HRESULT 
feedback(
  dt_ctl_t *ctl,
  char *cmd,
  long max) = 0;

										/* ................................ */
										/* give subclass access to stuff 	*/
protected:

dt_ctl_t *
get_ctl() 
{ 
  return this->ctl; 
}

int
get_connected()
{
  return this->connected;
}
										/* ................................ */
private:

  CMediaType media;						// current input media type

  CCritSec lock;

  dtmj_push_prop_t prop;				// the properties

  char *outbuf;
  dt_ctl_t _ctl;
  dt_ctl_t *ctl;
  dtmsw_t _msw;
  dtmsw_t *msw;
  int connected;

  dtmj_format_t format;
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
