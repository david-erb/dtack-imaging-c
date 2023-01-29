#include <windows.h>

#include "dtax.h"

#include <streams.h>
#include <initguid.h>
#include <olectl.h>
#include "guid.h"
#include "decode.h"
#include "resource.h"
#include "dtmj.h"
#include "dtjpeg.h"

//--------------------------------------------------------------------------
// DllRegisterServer
//
// Handles sample registry and unregistry
//
HRESULT DllRegisterServer()
{
    return AMovieDllRegisterServer();

} // DllRegisterServer


//--------------------------------------------------------------------------
// DllUnregisterServer
//
HRESULT DllUnregisterServer()
{
    return AMovieDllUnregisterServer();

} // DllUnregisterServer

//--------------------------------------------------------------------------



// Setup information

AMOVIESETUP_MEDIATYPE dtmj_decode_inpintypes =
{
    &MEDIATYPE_Stream,         		// major type
    &dtmj_subtype_guid   		// minor type
};

AMOVIESETUP_MEDIATYPE dtmj_decode_outpintypes =
{
    &MEDIATYPE_Video,         		// major type
    &MEDIASUBTYPE_RGB24       		// minor type
};

AMOVIESETUP_PIN dtmj_decode_pins[] =
{
    { L"Input",             // Pins string name
      FALSE,                // Is it rendered
      FALSE,                // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &dtmj_decode_inpintypes           // Pin information
    },
    { L"Output",            // Pins string name
      FALSE,                // Is it rendered
      TRUE,                 // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &dtmj_decode_outpintypes          // Pin information
    }
};

AMOVIESETUP_FILTER dtmj_decode_filter =
{
    &dtmj_decode_guid,          // Filter CLSID
    L"Dtack Mjpeg decoding filter",    // String name
    MERIT_DO_NOT_USE,       // Filter merit
    2,                      // Number of pins
    dtmj_decode_pins  // Pin information
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance

CFactoryTemplate g_Templates[] = {
    {L"Dtack Mjpeg decode1-3 transform filter", 
     &dtmj_decode_guid, 
     dtmj_decode_c::CreateInstance}
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


//--------------------------------------------------------------------------
// Constructor
//
dtmj_decode_c::dtmj_decode_c(
  TCHAR *tszName,
  LPUNKNOWN punk,
  HRESULT *phr):
CTransformFilter(tszName, punk, dtmj_decode_guid, phr)
{
  DTAX_F("dtmj_decode_c::dtmj_decode_c");
  DbgLog((LOG_TRACE, 1, TEXT("%s"), F));
}

//--------------------------------------------------------------------------
// CreateInstance
//
CUnknown *
dtmj_decode_c::CreateInstance(
  LPUNKNOWN punk, 
  HRESULT *phr)
{
    dtmj_decode_c *pNewObject = 
      new dtmj_decode_c(
        NAME("decode"), 
      punk, phr);
    if (pNewObject == NULL) {
        *phr = E_OUTOFMEMORY;
    }
    return pNewObject;

}


//--------------------------------------------------------------------------
// GetSetupData
//
LPAMOVIESETUP_FILTER 
dtmj_decode_c::GetSetupData()
{
  return &dtmj_decode_filter;
}


//--------------------------------------------------------------------------
// Transform
//
HRESULT dtmj_decode_c::Transform(
  IMediaSample *isample, 
  IMediaSample *osample)
{
  DTAX_F("dtmj_decode_c::Transform"); 
  BYTE *ibuff;
  BYTE *obuff;

  isample->GetPointer(&ibuff);
  osample->GetPointer(&obuff);

  long isize =				// jpeg compressed size
    isample->GetActualDataLength();	

  long osize = osample->GetSize();	// how much is allocated for output

  dtax_rc_e rc;
  long xe, ye;
  rc = dtjpeg_decode(ibuff, isize,	// decode a jpeg frame
    obuff, &osize, &xe, &ye);

  if (rc != DTAX_RC_GOOD)
  {
    DbgLog((LOG_ERROR, 1, TEXT("%s: dtjpeg_decode failed"), F));
    return E_UNEXPECTED;
  }

  osample->SetActualDataLength(		// tell how much we used
    osize);

  return NOERROR;
}

//--------------------------------------------------------------------------
// __candecode
//
// Check if this is a RGB24 true color format
//
static
BOOL 
__candecode(
  const CMediaType *media)
{
  int istype = IsEqualGUID(*media->Type(), MEDIATYPE_Stream);
  int issubtype = IsEqualGUID(*media->Subtype(), dtmj_subtype_guid);
  int isformat = IsEqualGUID(*media->FormatType(), dtmj_format_guid);

  if (istype && issubtype)
    return TRUE;

  return FALSE;
} 

//--------------------------------------------------------------------------
// Check the input type is OK - return an error otherwise

static
void
print_media(
  const CMediaType *media,
  const TCHAR *F,
  const TCHAR *prefix)
{
  DbgLog((LOG_TRACE, 1, TEXT("%s: %s"), F, prefix));
  DbgLog((LOG_TRACE, 1, TEXT("  ::Type() = %s"),
    GuidNames[*media->Type()]));
  DbgLog((LOG_TRACE, 1, TEXT("  ::Subtype() = %s"),
    GuidNames[*media->Subtype()]));
  DbgLog((LOG_TRACE, 1, TEXT("  ::FormatType() = %s"),
    GuidNames[*media->FormatType()]));
  DbgLog((LOG_TRACE, 1, TEXT("  ::FormatLength() = %lu"),
    media->FormatLength()));
}

//--------------------------------------------------------------------------
// Check the input type is OK - return an error otherwise

HRESULT 
dtmj_decode_c::CheckInputType(
  const CMediaType *media)
{
  DTAX_F("dtmj_decode_c::CheckInputType");

  int isformat = IsEqualGUID(*media->FormatType(), dtmj_format_guid);

  if (!isformat)
    return E_INVALIDARG;

  if (__candecode(media))
    return NOERROR;

  return E_FAIL;
}

//--------------------------------------------------------------------------
// Checktransform
//
// Check a transform can be done between these formats
//
HRESULT dtmj_decode_c::CheckTransform(
  const CMediaType *imedia, 
  const CMediaType *omedia)
{
  DTAX_F("dtmj_decode_c::CheckTransform");
  DbgLog((LOG_TRACE, 1, TEXT("%s"), F));
  print_media(imedia, F, TEXT("imedia"));
  print_media(omedia, F, TEXT("omedia"));
  if (__candecode(imedia)) 
    return NOERROR;
  return E_FAIL;

} 

//--------------------------------------------------------------------------
// GetMediaType
//
HRESULT dtmj_decode_c::GetMediaType(
  int iPosition, 
  CMediaType *media)
{
  DTAX_F("dtmj_decode_c::GetMediaType");

  if (m_pInput->IsConnected() == FALSE)
  {
    DbgLog((LOG_ERROR, 1, TEXT("%s: not yet connected"), F));
    return E_UNEXPECTED;
  }

  if (iPosition < 0) 			// this should never happen
    return E_INVALIDARG;
					
  if (iPosition > 0)			// no more items to offer
    return VFW_S_NO_MORE_ITEMS;

  media->SetType(&MEDIATYPE_Video);
  media->SetSubtype(&MEDIASUBTYPE_RGB24);
  media->SetFormatType(&FORMAT_VideoInfo);

  VIDEOINFO *videoinfo = (VIDEOINFO *)	// get space for videoinfo structure
    media->AllocFormatBuffer(
      sizeof(VIDEOINFO));
  if (NULL == videoinfo)
    return(E_OUTOFMEMORY);
  ZeroMemory(videoinfo, sizeof(VIDEOINFO));

  dtmj_format_t *format =		// get input pin's format structure
    (dtmj_format_t *)m_pInput->
      CurrentMediaType().Format();

  DbgLog((LOG_TRACE, 1, TEXT("%s: xe=%ld ye=%ld"), F, format->xe, format->ye));

  videoinfo->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
  videoinfo->bmiHeader.biWidth		= format->xe;
  videoinfo->bmiHeader.biHeight		= format->ye;
  videoinfo->bmiHeader.biPlanes		= 1;
  videoinfo->bmiHeader.biCompression    = BI_RGB;
  videoinfo->bmiHeader.biBitCount       = 24;
  videoinfo->bmiHeader.biSizeImage	= format->xe*format->ye*3;
  videoinfo->bmiHeader.biClrUsed	= 0;
  videoinfo->bmiHeader.biClrImportant	= 0;

  SetRectEmpty(&(videoinfo->rcSource));	// we want the whole image area rendered.
  SetRectEmpty(&(videoinfo->rcTarget));	// no particular destination rectangle

  media->SetFormat((BYTE *)videoinfo, sizeof(*videoinfo));
  media->SetTemporalCompression(FALSE);
  media->SetSampleSize(videoinfo->bmiHeader.biSizeImage);

  DbgLog((LOG_TRACE, 1, TEXT("%s: returning, videoinfosize=%d"), 
    F, sizeof(*videoinfo)));
  
  return NOERROR;
} 

//--------------------------------------------------------------------------
// DecideBufferSize
//
// Tell the output pin's allocator what size buffers we
// require. Can only do this when the input is connected
//
HRESULT 
dtmj_decode_c::DecideBufferSize(
  IMemAllocator *pAlloc,
  ALLOCATOR_PROPERTIES *pProperties)
{
  DTAX_F("dtmj_decode_c::DecideBufferSize");

  if (m_pInput->IsConnected() == FALSE)
  {
    DbgLog((LOG_ERROR, 1, TEXT("%s: not yet connected"), F));
    return E_UNEXPECTED;
  }


  ASSERT(pAlloc);
  ASSERT(pProperties);
  HRESULT hr = NOERROR;

  pProperties->cBuffers = 1;

  dtmj_format_t *format =		// get input pin's format structure
    (dtmj_format_t *)m_pInput->
      CurrentMediaType().Format();

  pProperties->cbBuffer = format->xe * format->ye * 3;
  ASSERT(pProperties->cbBuffer);

  DbgLog((LOG_TRACE, 1, TEXT("%s pProperties->cbBuffer=%ld"), 
    F, pProperties->cbBuffer));

    // Ask the allocator to reserve us some sample memory, NOTE the function
    // can succeed (that is return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties, &Actual);
    if (FAILED(hr)) {
        return hr;
    }

    ASSERT( Actual.cBuffers == 1 );

    if (pProperties->cBuffers > Actual.cBuffers ||
            pProperties->cbBuffer > Actual.cbBuffer) {
                return E_FAIL;
    }
    return NOERROR;

}

