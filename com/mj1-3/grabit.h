/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




class dtmj_grabit_stream_c: 
  public CSourceStream
{

public:
  HRESULT FillBuffer(IMediaSample *pms);

  HRESULT DecideBufferSize(
    IMemAllocator *pIMemAlloc,
    ALLOCATOR_PROPERTIES *pProperties);

  HRESULT CheckMediaType(
    const CMediaType *media);
  HRESULT GetMediaType(
    int physician, 
    CMediaType *media);

  dtmj_grabit_stream_c(					// public constructor
    CSource *source,
    HRESULT *phr);

  void get_prop(						// get properties
    dtmj_grabit_prop_t *prop);
  void put_prop(						// set properties
    dtmj_grabit_prop_t *prop, 
    char *message, int max);
  void put_ctl(							// set ctl environment
    dt_ctl_t *ctl);

  HRESULT dtmj_command_i_process(
    const char *command);

private:
  HRESULT 
  notify_event(
    long code,
    long param1,
    long param2);

  ~dtmj_grabit_stream_c();

  dtmj_grabit_prop_t prop;				// the properties

  CCritSec proplock;					// lock on property access
  CCritSec shared;						// shared lock

  char *outbuf;
  dt_ctl_t _ctl;
  dt_ctl_t *ctl;
  dtmsw_t _msw;
  dtmsw_t *msw;

  long xe, ye;
  long frame;
};

										// ................................
class dtmj_grabit_source_c: 
  public CSource, 
  public dtmj_grabit_prop_i,
  public ISpecifyPropertyPages,
  public CPersistStream,
  public dtmj_command_i
{

public:
  DECLARE_IUNKNOWN;
  static CUnknown *CreateInstance(
    LPUNKNOWN lpunk, 
    HRESULT *phr);
  
  STDMETHODIMP NonDelegatingQueryInterface(
    REFIID riid, 
    void ** ppv);


  STDMETHODIMP GetPages(CAUUID *pages);	// return property page(s)
  STDMETHODIMP get_prop(				// get properties
    dtmj_grabit_prop_t *prop);
  STDMETHODIMP put_prop(				// set properties
    dtmj_grabit_prop_t *prop, 
    char *message, int max);
  STDMETHODIMP put_ctl(					// set ctl environment
    dt_ctl_t *ctl);

  // CPersistStream override
  STDMETHODIMP GetClassID(CLSID *pClsid);
  HRESULT WriteToStream(IStream *pStream);
  HRESULT ReadFromStream(IStream *pStream);
  int SizeMax();
  DWORD GetSoftwareVersion();

  LPAMOVIESETUP_FILTER GetSetupData();

  STDMETHODIMP dtmj_command_i_process(const char *command);

private:
  dtmj_grabit_source_c(					// private constructor
    LPUNKNOWN lpunk, 
    HRESULT *phr);

  dtmj_grabit_stream_c *stream;			// our associated stream

  CCritSec proplock;					// lock on property access
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
