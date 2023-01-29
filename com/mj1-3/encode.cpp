#include <windows.h>
#include "dtax.h"
#include <dtlist.h>
#include <dtipc.h>
#include <dtos.h>
#include <dtdg.h>
#define DTDEPEND
#include <dtmsw.h>
#include <dterrno.h>

#include "dtmj.h"

#include <streams.h>
#include <initguid.h>
#include <olectl.h>
#include "guid.h"
#include "command.h"
#include "encode_prop.h"
#include "encode.h"
#include "resource.h"

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

AMOVIESETUP_MEDIATYPE dtmj_encode_inpintypes =
{
    &MEDIATYPE_Video,         		// major type
    &MEDIASUBTYPE_RGB24        		// minor type
};

AMOVIESETUP_MEDIATYPE dtmj_encode_outpintypes =
{
    &MEDIATYPE_Stream,         		// major type
    &dtmj_subtype_guid   		// minor type
};

AMOVIESETUP_PIN dtmj_encode_pins[] =
{
    { L"Input",             // Pins string name
      FALSE,                // Is it rendered
      FALSE,                // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &dtmj_encode_inpintypes           // Pin information
    },
    { L"Output",            // Pins string name
      FALSE,                // Is it rendered
      TRUE,                 // Is it an output
      FALSE,                // Are we allowed none
      FALSE,                // And allowed many
      &CLSID_NULL,          // Connects to filter
      NULL,                 // Connects to pin
      1,                    // Number of types
      &dtmj_encode_outpintypes          // Pin information
    }
};

AMOVIESETUP_FILTER dtmj_encode_filter =
{
    &dtmj_encode_guid,          // Filter CLSID
    L"Dtack Mjpeg encoding filter",    // String name
    MERIT_DO_NOT_USE,       // Filter merit
    2,                      // Number of pins
    dtmj_encode_pins   // Pin information
};


// List of class IDs and creator functions for the class factory. This
// provides the link between the OLE entry point in the DLL and an object
// being created. The class factory will call the static CreateInstance

CFactoryTemplate g_Templates[] = {
  {
    L"Dtack Mjpeg encode1-3 transform filter", 
    &dtmj_encode_guid,
    dtmj_encode_c::CreateInstance
  },
  {
    L"Dtack Mjpeg encode1-3 transform filter property page", 
    &dtmj_encode_proppage_guid, 
    dtmj_encode_prop_c::CreateInstance
  }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);


//--------------------------------------------------------------------------
// Constructor
//
dtmj_encode_c::dtmj_encode_c(
  TCHAR *tszName,
  LPUNKNOWN punk,
  HRESULT *phr):
CTransformFilter(tszName, punk, dtmj_encode_guid, phr),
m_lBufferRequest(1),
CPersistStream(punk, phr)
{
  DTAX_F("dtmj_encode_c::dtmj_encode_c: enter");
  memset(&this->prop, 0, sizeof(this->prop));
  this->prop.jquality = 60;

  this->ctl = &this->_ctl;
  memset(this->ctl, 0, 			// clear ctl structure
    sizeof(*this->ctl));

  this->ctl->msw = &this->_msw;
  memset(&this->_msw, 0, sizeof(this->_msw));

  this->_msw.outwin = (void *)(1);

  this->_msw.outbufm = 512;
  this->outbuf =
  this->_msw.outbuf = (char *)malloc(
    this->_msw.outbufm);

  this->fb_contrast = 0;
}

//--------------------------------------------------------------------------
// destructor
//
dtmj_encode_c::~dtmj_encode_c()
{
  if (this->outbuf != NULL)
    free(this->outbuf);
  this->outbuf = NULL;
}


//--------------------------------------------------------------------------
// CreateInstance
//
// Provide the way for COM to create a filter object
//
CUnknown *
dtmj_encode_c::CreateInstance(
  LPUNKNOWN punk, 
  HRESULT *phr)
{
  dtmj_encode_c *pNewObject = 
    new dtmj_encode_c(
      NAME("encode"), 
    punk, phr);
  if (pNewObject == NULL) 
    *phr = E_OUTOFMEMORY;
  return pNewObject;
}

//--------------------------------------------------------------------------
// IAMediaSetup::GetSetupData
//
LPAMOVIESETUP_FILTER dtmj_encode_c::GetSetupData()
{
  return &dtmj_encode_filter;
}


//--------------------------------------------------------------------------
// NonDelegatingQueryInterface
//
STDMETHODIMP 
dtmj_encode_c::NonDelegatingQueryInterface(
  REFIID riid, 
  void **ppv)
{
  DTAX_F("dtmj_encode_c::NonDelegatingQueryInterface");
  CheckPointer(ppv,E_POINTER);

  if (riid == dtmj_encode_prop_i_guid) 
    return GetInterface((dtmj_encode_prop_i *)this, ppv);
  else 
  if (riid == dtmj_command_i_guid) 
    return GetInterface((dtmj_command_i *)this, ppv);
  else 
  if (riid == IID_ISpecifyPropertyPages) 
    return GetInterface((ISpecifyPropertyPages *)this, ppv);
  else 
  if (riid == IID_IPersistStream)
    return GetInterface((IPersistStream *) this, ppv);
  else 
    return CTransformFilter::NonDelegatingQueryInterface(riid, ppv);
}


//--------------------------------------------------------------------------
// Transform
//
HRESULT dtmj_encode_c::Transform(
  IMediaSample *isample, 
  IMediaSample *osample)
{
  DTAX_F("dtmj_encode_c::Transform");
  VIDEOINFO *videoinfo;
  long xe, ye;
  BYTE *ibuff;
  long isize;
  BYTE *obuff;
  long osize;
  CMediaType omedia;
  dtax_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_TRACE,
    "called");

  DTAX_C(isample->GetPointer,(&ibuff));

  DTAX_C(osample->GetPointer,(&obuff));

  videoinfo = (VIDEOINFO *)m_pInput->	// get input pin's format structure
    CurrentMediaType().Format();
  xe = videoinfo->bmiHeader.biWidth;
  ye = videoinfo->bmiHeader.biHeight;

  osize = osample->GetSize();		// how much is allocated for use

  isize = isample->GetActualDataLength();

  DTAX_C(dtjpeg_encode,(ibuff, xe, ye,	// encode a jpeg frame
    this->prop.jquality, 
    obuff, &osize));

  osample->SetActualDataLength(osize);	// tell how much we used

  this->GetMediaType(0, &omedia);	// get our sample media type
  osample->SetMediaType(&omedia);	// tell osample about media type
  
  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
    "q=%d s=%d",
    this->prop.jquality, osize);
  
cleanup:

  if (rc != DT_RC_GOOD)
    NotifyEvent(EC_ERRORABORT, 0, 0);	// tell the filter graph
  
  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
// can_encode
//
// Check if this is a RGB24 true colour format
//
BOOL dtmj_encode_c::can_encode(const CMediaType *media) const
{
  if (IsEqualGUID(*media->Type(), MEDIATYPE_Video)) 
  {
    if (IsEqualGUID(*media->Subtype(), MEDIASUBTYPE_RGB24)) 
    {
      VIDEOINFO *pvi = (VIDEOINFO *) media->Format();
      return (pvi->bmiHeader.biBitCount == 24);
    }
  }
  return FALSE;

} 

//--------------------------------------------------------------------------
// Check the input type is OK - return an error otherwise

HRESULT 
dtmj_encode_c::CheckInputType(
  const CMediaType *media)
{
  if (*media->FormatType() !=
      FORMAT_VideoInfo)
    return E_INVALIDARG;

  if (this->can_encode(media))
    return NOERROR;

  return E_FAIL;
}

//--------------------------------------------------------------------------
// Checktransform
//
// Check a transform can be done between these formats
//
HRESULT 
dtmj_encode_c::CheckTransform(
  const CMediaType *imedia, 
  const CMediaType *omedia)
{
  if (this->can_encode(imedia)) 
    if (IsEqualGUID(*omedia->Type(), MEDIATYPE_Stream) &&
	IsEqualGUID(*omedia->Subtype(), dtmj_subtype_guid))
      return NOERROR;
  return E_FAIL;
} 

//--------------------------------------------------------------------------
// GetMediaType
//
HRESULT dtmj_encode_c::GetMediaType(
  int iPosition, 
  CMediaType *omedia)
{
  DTAX_F("dtmj_encode_c::GetMediaType");

  if (m_pInput->IsConnected() == FALSE)
  {
    dt_err(ctl, F,
      "not yet connected");
    return E_UNEXPECTED;
  }

  if (iPosition < 0) 			// this should never happen
    return E_INVALIDARG;
					
  if (iPosition > 0)			// no more items to offer
    return VFW_S_NO_MORE_ITEMS;

  omedia->SetType(&MEDIATYPE_Stream);
  omedia->SetSubtype(&dtmj_subtype_guid);
  omedia->SetFormatType(&dtmj_format_guid);

  dtmj_format_t *format =		// get space for format structure 
    (dtmj_format_t *)
      omedia->AllocFormatBuffer(
        sizeof(dtmj_format_t));
  if (NULL == format)
    return(E_OUTOFMEMORY);
  ZeroMemory(format, 
    sizeof(dtmj_format_t));

  VIDEOINFO *videoinfo =		// get input pin's format structure
    (VIDEOINFO *)m_pInput->
      CurrentMediaType().Format();
  format->xe = videoinfo->bmiHeader.biWidth;
  format->ye = videoinfo->bmiHeader.biHeight;

  format->jpeg_quality =		// update stuff from our props
    this->prop.jquality;

  format->fb_contrast = this->fb_contrast; 	// update other stuff we have

  omedia->SetTemporalCompression(FALSE);
  omedia->SetSampleSize(		// upper bound on jpeg output size
    format->xe*format->ye*3);
  
  return NOERROR;
} 

//--------------------------------------------------------------------------
// DecideBufferSize
//
// Tell the output pin's allocator what size buffers we
// require. Can only do this when the input is connected
//
HRESULT dtmj_encode_c::DecideBufferSize(
  IMemAllocator *pAlloc,
  ALLOCATOR_PROPERTIES *pProperties)
{
  DTAX_F("dtmj_encode_c::DecideBufferSize");

  if (m_pInput->IsConnected() == FALSE)
  {
    dt_err(ctl, F,
      "not yet connected");
    return E_UNEXPECTED;
  }

  ASSERT(pAlloc);
  ASSERT(pProperties);
  HRESULT hr = NOERROR;

  pProperties->cBuffers = 1;
  pProperties->cbBuffer = m_pInput->CurrentMediaType().GetSampleSize();
  ASSERT(pProperties->cbBuffer);

    // Ask the allocator to reserve us some sample memory, NOTE the function
    // can succeed (that is return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if (FAILED(hr)) {
        return hr;
    }

    ASSERT( Actual.cBuffers == 1 );

    if (pProperties->cBuffers > Actual.cBuffers ||
            pProperties->cbBuffer > Actual.cbBuffer) {
                return E_FAIL;
    }
    return NOERROR;

} // DecideBufferSize


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// CPersistStream::GetClassID
//
STDMETHODIMP 
dtmj_encode_c::GetClassID(
  CLSID *pClsid)
{
  DTAX_F("dtmj_encode_c::GetClassID");
  *pClsid = dtmj_encode_guid;
  return NOERROR;
}

//--------------------------------------------------------------------------
// CPersistStream::SizeMax
//
int
dtmj_encode_c::SizeMax()
{
  return sizeof(this->prop);
}

//--------------------------------------------------------------------------
// CPersistStream::GetSoftwareVersion
//
DWORD
dtmj_encode_c::GetSoftwareVersion()
{
  return (DWORD)0x00010001;
}

//--------------------------------------------------------------------------
// CPersistStream::WriteToStream
//
HRESULT 
dtmj_encode_c::WriteToStream(
  IStream *pStream)
{
  DTAX_F("dtmj_encode_c::WriteToStream");
  DTAX_Q(pStream->Write,(&this->prop, sizeof(this->prop), NULL));
  return NOERROR;
}

//--------------------------------------------------------------------------
// CPersistStream::ReadFromStream
//
HRESULT 
dtmj_encode_c::ReadFromStream(
  IStream *pStream)
{
  DTAX_F("dtmj_encode_c::ReadFromStream");
  DTAX_Q(pStream->Read,(&this->prop, sizeof(this->prop), NULL)); 
  return NOERROR;
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// ISpecifyPropertyPages::GetPages
//
// Returns the clsid's of the property pages we support
//
STDMETHODIMP dtmj_encode_c::GetPages(CAUUID *pPages)
{
  pPages->cElems = 1;
  pPages->pElems = (GUID *)CoTaskMemAlloc(sizeof(GUID));
  if (pPages->pElems == NULL) 
    return E_OUTOFMEMORY;
  *(pPages->pElems) = dtmj_encode_proppage_guid;
  return NOERROR;
} 


//--------------------------------------------------------------------------
// dtmj_encode_prop_i::get_prop
// return current jpeg encoding quality
// called by property page or VB
//
STDMETHODIMP 
dtmj_encode_c::get_prop(
  dtmj_encode_prop_t *prop)
{
  CAutoLock lock(&this->lock);
  CheckPointer(prop, E_POINTER);

  *prop = this->prop;

  return NOERROR;
}

//--------------------------------------------------------------------------
// dtmj_encode_prop_i::put_prop
// change jpeg encoding quality starting with next frame
// called by property page or VB
//
STDMETHODIMP 
dtmj_encode_c::put_prop(
  dtmj_encode_prop_t *prop,
  char *message,
  int max)
{
  CAutoLock lock(&this->lock);

  this->prop = *prop;

  SetDirty(TRUE);			// needs to be written by IPersist
  return NOERROR;
}

//--------------------------------------------------------------------------
// dtmj_encode_c::put_ctl
// called by main filter graph program
//
STDMETHODIMP 
dtmj_encode_c::put_ctl(
  dt_ctl_t *ctl)
{
  DT_F("dtmj_encode_c::put_ctl");
  
  CAutoLock lock(&this->lock);

  this->ctl = ctl;

  dt_dbg(this->ctl, F, DT_DBG_MASK_TRACE,
    "new ctl set");

  return NOERROR;
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
STDMETHODIMP 
dtmj_encode_c::dtmj_command_i_process(
  const char *command)
{
  CAutoLock lock(&this->lock);

  DTAX_F("dtmj_encode_c::dtmj_command_i_process");
  int changed = 0;

  dtmj_encode_prop_t prop;

  DTAX_Q(this->get_prop,(&prop));

# define JQUALITY "jquality,"
# define CONTRAST "contrast,"
  if (DT_ABBREV(command, JQUALITY))
  {
    int jquality = atoi(command + strlen(JQUALITY));
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "quality %d->%d", 
      prop.jquality, jquality);
    if (prop.jquality != jquality)
      changed++;
    prop.jquality = jquality;
  }
  else
  if (DT_ABBREV(command, CONTRAST))
  {
    int fb_contrast = atoi(command + strlen(CONTRAST));
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "contrast %d->%d", 
      this->fb_contrast, fb_contrast);
    if (this->fb_contrast != fb_contrast)
      changed++;
    this->fb_contrast = fb_contrast;
    DTAX_Q(dtmj_command_passthrough,(	// passs command up to next filter
      ctl, this, command));
  }
  else
  {
    dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
      "\"%s\" not for us", command);
    DTAX_Q(dtmj_command_passthrough,(	// passs command up to next filter
      ctl, this, command));
  }

  if (changed)
  DTAX_Q(this->put_prop,(&prop, NULL, 0));

  return NOERROR;
}

