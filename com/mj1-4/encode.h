/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




//==========================================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  Copyright (c) 1992 - 1996  Microsoft Corporation.  All Rights Reserved.
//
//--------------------------------------------------------------------------;

class dtmj_encode_c : 
  public CTransformFilter,
  public dtmj_encode_prop_i,
  public ISpecifyPropertyPages,
  public CPersistStream,
  public dtmj_command_i
{

public:

    DECLARE_IUNKNOWN;
    static CUnknown *CreateInstance(LPUNKNOWN punk, HRESULT *phr);

    // Reveals IEZrgb24 and ISpecifyPropertyPages
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);


    // Overrriden from CTransformFilter base class

    HRESULT Transform(IMediaSample *pIn, IMediaSample *pOut);
    HRESULT CheckInputType(const CMediaType *mtIn);
    HRESULT CheckTransform(const CMediaType *mtIn, const CMediaType *mtOut);
    HRESULT DecideBufferSize(IMemAllocator *pAlloc,
			     ALLOCATOR_PROPERTIES *pProperties);
    HRESULT GetMediaType(int iPosition, CMediaType *pMediaType);

  STDMETHODIMP GetPages(CAUUID *pages);	// return property page(s)
  STDMETHODIMP get_prop(				// get properties
    dtmj_encode_prop_t *prop);
  STDMETHODIMP put_prop(				// set properties
    dtmj_encode_prop_t *prop, 
    char *message, int max);
  STDMETHODIMP put_ctl(					// set ctl environment
    dt_ctl_t *ctl);

  STDMETHODIMP GetClassID(CLSID *pClsid);
  HRESULT WriteToStream(IStream *pStream);
  HRESULT ReadFromStream(IStream *pStream);
  int SizeMax();
  DWORD GetSoftwareVersion();

  LPAMOVIESETUP_FILTER GetSetupData();

  STDMETHODIMP dtmj_command_i_process(const char *command);

private:
  ~dtmj_encode_c();

  dtmj_encode_c(TCHAR *tszName, LPUNKNOWN punk, HRESULT *phr);

  BOOL can_encode(const CMediaType *pMediaType) const;

  CCritSec	lock;
  dtmj_encode_prop_t prop;
  const long m_lBufferRequest;			// The number of buffers to use

  char *outbuf;
  dt_ctl_t _ctl;
  dt_ctl_t *ctl;
  dtmsw_t _msw;
  dtmsw_t *msw;

  int fb_brightness;
  int fb_contrast;
  int fb_hue;
  int fb_panx;
  int fb_pany;
  int fb_quality;
  int fb_saturation;
  int fb_source;
  int fb_zoom;
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
