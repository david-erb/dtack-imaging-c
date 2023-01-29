#include <windows.h>
#include "dtax.h"
#include <dtipc.h>
#include <dtos.h>
#include <dtstr.h>
#include <dterrno.h>
#define DTDEPEND
#include <dtmsw.h>

#include <streams.h>
#include <olectl.h>
#include <initguid.h>
#include "guid.h"
#include "dtmj.h"
#include "command.h"
#include "grabit_prop.h"
#include "grabit.h"
#include "resource.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>
#include <io.h>

AMOVIESETUP_MEDIATYPE pintypes =
{
    &MEDIATYPE_Video,       // Major type
    &MEDIASUBTYPE_NULL      // Minor type
};

AMOVIESETUP_PIN pin =
{
    L"Output",              // Pin string name
    FALSE,                  // Is it rendered
    TRUE,                   // Is it an output
    FALSE,                  // Can we have none
    FALSE,                  // Can we have many
    &CLSID_NULL,            // Connects to filter
    NULL,                   // Connects to pin
    1,                      // Number of types
    &pintypes };            // Pin details

AMOVIESETUP_FILTER dtmj_grabit_filter =
{
    &dtmj_grabit_guid,     // Filter CLSID
    L"Dtack Mjpeg grabit", // String name
    MERIT_DO_NOT_USE,       // Filter merit
    1,                      // Number pins
    &pin	            // Pin details
};


CFactoryTemplate g_Templates[] = {
  { 
    L"Dtack Mjpeg grabit1-4 source filter", 
      &dtmj_grabit_guid, 
      dtmj_grabit_source_c::CreateInstance 
  },
  {
    L"Dtack Mjpeg grabit1-4 source filter property page", 
      &dtmj_grabit_proppage_guid, 
      dtmj_grabit_prop_c::CreateInstance
  }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);

//--------------------------------------------------------------------------
#define CR (0x0d)
#define LF (0x0a)

typedef long tableau_len_t;

#define TABLEAU_HEADER_CAMNAME_MAX (16)
typedef struct {
  tableau_len_t l;			// length of grabitd data
  tableau_len_t n;			// frame sequence number 
  tableau_len_t ms;			// millisecond at acquire
  tableau_len_t xe;			// size of decoded image
  tableau_len_t ye;
  tableau_len_t org;
  char cam[TABLEAU_HEADER_CAMNAME_MAX];
  char codec[16];
} __header_t;

#define TABLEAU_CODEC_UNCOMPRESSED "uncompressed"

//--------------------------------------------------------------------------
// wait for connection
static
void
__dbgmask(
  dt_ctl_t *ctl,
  const char *dbgspec)
{
  DT_F("dtmj_grabit_source_c::__dbgmask");
  long dbg_mask;
  char message[80];
  dt_rc_e rc;
  rc = dt_dbg_parse(ctl, dbgspec,	// parse current debug mask
    &dbg_mask,
    message, sizeof(message));
  if (rc == DT_RC_GOOD)			// string is OK?
    ctl->dbg_mask = dbg_mask;
}

//--------------------------------------------------------------------------
//
// DllRegisterServer
//
// Exported entry points for registration and unregistration
//
HRESULT DllRegisterServer()
{
    return AMovieDllRegisterServer();

} // DllRegisterServer


//--------------------------------------------------------------------------
//
// DllUnregisterServer
//
HRESULT DllUnregisterServer()
{
    return AMovieDllUnregisterServer();

} // DllUnregisterServer


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// (source) CreateInstance
//
CUnknown *
dtmj_grabit_source_c::CreateInstance(
  LPUNKNOWN lpunk, 
  HRESULT *phr)
{
  CUnknown *punk = new dtmj_grabit_source_c(lpunk, phr);
  if (punk == NULL) 
    *phr = E_OUTOFMEMORY;
  return punk;
} 

//--------------------------------------------------------------------------
// (source) Constructor
//
dtmj_grabit_source_c::dtmj_grabit_source_c(
  LPUNKNOWN lpunk, 
  HRESULT *phr):
CSource(NAME("grabit source"),		// base class constructor
  lpunk,
  dtmj_grabit_guid,
  phr),
CPersistStream(lpunk, phr)
{
  CAutoLock lock(&m_cStateLock);

  this->stream =			// construct the stream
    new dtmj_grabit_stream_c(
      this, phr);
}

//--------------------------------------------------------------------------
// (source) GetSetupData
//
LPAMOVIESETUP_FILTER 
dtmj_grabit_source_c::GetSetupData()
{
  return &dtmj_grabit_filter;
}

//--------------------------------------------------------------------------
// (source) NonDelegatingQueryInterface
//
STDMETHODIMP 
dtmj_grabit_source_c::NonDelegatingQueryInterface(
  REFIID riid, 
  void **ppv)
{
  CheckPointer(ppv,E_POINTER);

  if (riid == dtmj_grabit_prop_i_guid)
    return GetInterface((dtmj_grabit_prop_i *)this, ppv);
  else 
  if (riid == dtmj_command_i_guid) 
    return GetInterface((dtmj_command_i *)this, ppv);
  else 
  if (riid == IID_ISpecifyPropertyPages) 
    return GetInterface((ISpecifyPropertyPages *)this, ppv);
  else 
  if (riid == IID_IPersistStream)
    return GetInterface((IPersistStream *)this, ppv);
  else
    return CSource::NonDelegatingQueryInterface(riid, ppv);

} // NonDelegatingQueryInterface


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// (source) CPersistStream::GetClassID
//
STDMETHODIMP 
dtmj_grabit_source_c::GetClassID(
  CLSID *pClsid)
{
  *pClsid = dtmj_grabit_guid;
  return NOERROR;
}

//--------------------------------------------------------------------------
// (source) CPersistStream::SizeMax
//
int
dtmj_grabit_source_c::SizeMax()
{
  return sizeof(dtmj_grabit_prop_t);
}

//--------------------------------------------------------------------------
// (source) CPersistStream::GetSoftwareVersion
//
DWORD
dtmj_grabit_source_c::GetSoftwareVersion()
{
  return (DWORD)0x00010001;
}

//--------------------------------------------------------------------------
// (source) CPersistStream::WriteToStream
//
HRESULT 
dtmj_grabit_source_c::WriteToStream(
  IStream *pStream)
{
  DTAX_F("dtmj_grabit_source_c::WriteToStream");
  dtmj_grabit_prop_t prop;
  this->stream->get_prop(&prop);
      DbgLog((LOG_TRACE, 10, 
        TEXT("%s: ipcspec %s"),
        F, prop.ipcspec));
  HRESULT hr = pStream->Write(&prop, sizeof(prop), NULL);
  return hr;
}

//--------------------------------------------------------------------------
// (source) CPersistStream::ReadFromStream
//
HRESULT 
dtmj_grabit_source_c::ReadFromStream(
  IStream *pStream)
{
  DTAX_F("dtmj_grabit_source_c::ReadFromStream");
  dtmj_grabit_prop_t prop;
  HRESULT hr = pStream->Read(&prop, sizeof(prop), NULL);
      DbgLog((LOG_TRACE, 10, 
        TEXT("%s: ipcspec %s"),
        F, prop.ipcspec));
  this->stream->put_prop(&prop, NULL, 0);
  return hr;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// (source) ISpecifyPropertyPages::GetPages
//
// Returns the clsid's of the property pages we support
//
STDMETHODIMP dtmj_grabit_source_c::GetPages(CAUUID *pPages)
{
  pPages->cElems = 1;
  pPages->pElems = (GUID *)CoTaskMemAlloc(sizeof(GUID));
  if (pPages->pElems == NULL) 
    return E_OUTOFMEMORY;
  *(pPages->pElems) = dtmj_grabit_proppage_guid;
  return NOERROR;
} 



//--------------------------------------------------------------------------
// dtmj_grabit_prop_i::get_prop
// return current jpeg encoding quality
// called by property page or VB
//
STDMETHODIMP 
dtmj_grabit_source_c::get_prop(
  dtmj_grabit_prop_t *prop)
{
  CAutoLock lock(&this->proplock);
  CheckPointer(prop, E_POINTER);

  this->stream->get_prop(prop);

  return NOERROR;
}

//--------------------------------------------------------------------------
// dtmj_grabit_source_c::put_prop
// change jpeg encoding quality starting with next frame
// called by property page or VB
//
STDMETHODIMP 
dtmj_grabit_source_c::put_prop(
  dtmj_grabit_prop_t *prop,
  char *message,
  int max)
{
  CAutoLock lock(&this->proplock);

  this->stream->put_prop(prop, message, max);
  
  SetDirty(TRUE);			// needs to be written by IPersist

  return NOERROR;
}

//--------------------------------------------------------------------------
// dtmj_grabit_source_c::put_ctl
// called by main filter graph program
//
STDMETHODIMP 
dtmj_grabit_source_c::put_ctl(
  dt_ctl_t *ctl)
{
  CAutoLock lock(&this->proplock);

  this->stream->put_ctl(ctl);

  return NOERROR;
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// (source) dtmj_grabit_source_c::dtmj_command_i_process
//
STDMETHODIMP 
dtmj_grabit_source_c::dtmj_command_i_process(
  const char *command)
{
  CAutoLock lock(&this->proplock);

  DTAX_F("dtmj_grabit_source_c::dtmj_command_i_process");

  this->stream->dtmj_command_i_process(command);

  return NOERROR;
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// (stream) constructor
//
dtmj_grabit_stream_c::dtmj_grabit_stream_c(
  CSource *source,
  HRESULT *phr):
CSourceStream(NAME("grabit stream"),	// base class constructor
  phr,
  source,
  L"Output"),
outbuf(NULL),
xe(0),
ye(0),
frame(0)
{
  CAutoLock lock(&this->shared);

  DTAX_F("dtmj_grabit_stream_c::dtmj_grabit_stream_c");

  strcpy(this->prop.ipcspec, "rocket,localhost:5720");
  strcpy(this->prop.dbgspec, "usr1");

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

  __dbgmask(ctl, this->prop.dbgspec);	// check for new debug mask
}

//--------------------------------------------------------------------------
// (stream) destructor
//
dtmj_grabit_stream_c::~dtmj_grabit_stream_c()
{
  if (this->outbuf != NULL)
    free(this->outbuf);
  this->outbuf = NULL;
}

//--------------------------------------------------------------------------
// send arbitrary command to tabgrab
static
HRESULT 
__send(
  dt_ctl_t *ctl,			// environment control
  const char *ipcspec,			// ipc server spec
  const char *command)
{
  DT_F("dtmj_grabit_c::__send");
  dtipc_t *ipc = NULL;
  int ipc_initialized = 0;
  char crlf[3] = {CR, LF, '\0'};
  char s[256];
  int l;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,		// space for ipc structure
    (void **)&ipc, sizeof(*ipc),
    F, "ipc structure"));

  DT_C(dtipc_find,(ctl, ipc,		// find tabgrab server
    ipcspec, 0, 0, 1000));
  ipc_initialized = 1;

  dtstr_printf(ctl, s, sizeof(s),	// formulate command for tabgrab
    "%s%c%c", command, CR, LF);
  DT_C(dtipc_write_all,(ctl, ipc, 	// write command to tabgrab
    s, strlen(s)));

  DT_C(dtipc_read_line,(ctl, ipc,	// wait for tabgrab to reply
    s, sizeof(s)));

  l = strlen(s);
  while (l > 0 &&			// nuke the crlf's in the reply
	 s[l-1] == LF ||
	 s[l-1] == CR)
    s[--l] = '\0';

  dt_dbg(ctl, F, DT_DBG_MASK_REPLY,
    "reply: %s", s);

  if (!strncmp(s, "error", 5))		// tabgrab replied with an error?
    rc = dt_err(ctl, F,
      "tabgrab server error");

cleanup:
  if (ipc_initialized)
    DT_I(dtipc_unfind,(ctl, ipc));
  if (ipc != NULL)
    DT_I(dtos_free2,(ctl, ipc,
      F, "ipc structure"));
  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
// grab new frame
static
HRESULT 
__grab(
  dt_ctl_t *ctl,			// environment control
  const char *ipcspec,			// ipc server spec
  long *frame,				// frame in/out
  char *filename,			// filename returned
  long max)				// max length of filename
{
  DT_F("dtmj_grabit_c::__grab");
  dtipc_t *ipc = NULL;
  int ipc_initialized = 0;
  char crlf[3] = {CR, LF, '\0'};
  char s[256];
  int l;
  dt_rc_e rc;

  DT_C(dtos_malloc2,(ctl,		// space for ipc structure
    (void **)&ipc, sizeof(*ipc),
    F, "ipc structure"));

  DT_C(dtipc_find,(ctl, ipc,		// find tabgrab server
    ipcspec, 0, 0, 1000));
  ipc_initialized = 1;

  dtstr_printf(ctl, s, sizeof(s),	// formulate command for tabgrab
    "grab,%ld%c%c", *frame, CR, LF);
  DT_C(dtipc_write_all,(ctl, ipc, 	// write command to tabgrab
    s, strlen(s)));

  DT_C(dtipc_read_line,(ctl, ipc,	// wait for tabgrab to reply
    s, sizeof(s)));

  l = strlen(s);
  while (l > 0 &&			// nuke the crlf's in the reply
	 s[l-1] == LF ||
	 s[l-1] == CR)
    s[--l] = '\0';

  dt_dbg(ctl, F, DT_DBG_MASK_REPLY,
    "reply: %s", s);

  if (strncmp(s, "error", 5))		// not an error?
  {
    char *p = strchr(s, ',');		// find the comma in the reply
    if (p != NULL)
    {
      *p++ = '\0';
      DT_C(dtstr_to_long,(ctl,		// get the new frame
        p, frame));
    }
    dtstr_printf(ctl, filename, max,	// return the filename
      "%s", s);
  }
  else					// tabgrab replied with error?
    rc = dt_err(ctl, F,
      "tabgrab server error");

cleanup:
  if (ipc_initialized)
    DT_I(dtipc_unfind,(ctl, ipc));
  if (ipc != NULL)
    DT_I(dtos_free2,(ctl, ipc,
      F, "ipc structure"));
  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}
	
  
//--------------------------------------------------------------------------
// access shared memory
static
HRESULT 
__copyit(
  dt_ctl_t *ctl,			// environment control
  const char *filename,			// shared memory filename
  void *buffer,
  long max,
  long *xe,
  long *ye)
{
  DT_F("dtmj_grabit_c::__copyit");
  HANDLE mapping = NULL;
  HANDLE semaphore = NULL;
  void *b = NULL;
  int have_semaphore = 0;
  __header_t *header;
  DWORD e;
  dt_rc_e rc = DT_RC_GOOD;

  mapping = OpenFileMapping(
    FILE_MAP_READ, FALSE, filename);
  e = GetLastError();
  if (mapping == NULL)			// could not open mapped file?
  {
    rc = dt_err(ctl, F,
      "OpenFileMapping("
      "FILE_MAP_READ, FALSE, "
      "\"%s\")"
      DTERRNO_LASTF,
      filename,
      DTERRNO_LASTE(e));
    goto cleanup;
  }

  b = MapViewOfFile(			// get pointer to mapped file
    mapping,
    FILE_MAP_READ,
    0, 0, 0);
  e = GetLastError();
  if (b == NULL)			// could not get mapped pointer? 	
  {
    rc = dt_err(ctl, F,
      "MapViewOfFile("
      "0x%08lx, "
      "FILE_MAP_READ, "
      "0, 0, 0)"
      DTERRNO_LASTF,
      mapping, 
      DTERRNO_LASTE(e));
    goto cleanup;
  }

  semaphore = OpenSemaphore(		// access the semaphore
    SEMAPHORE_ALL_ACCESS, FALSE,
    filename+1);
  e = GetLastError();
  if (semaphore == NULL)		// could not get semaphore?
  {
    rc = dt_err(ctl, F,
      "OpenSemaphore("
      "SEMAPHORE_ALL_ACCESS, FALSE, "
      "\"%s\")"
      DTERRNO_LASTF,
      filename+1,
      DTERRNO_LASTE(e));
    goto cleanup;
  }

  DWORD result;
  result = WaitForSingleObject(		// wait for OK to copy
    semaphore, 2000);
  if (result == WAIT_TIMEOUT)
  {
    rc = dt_err(ctl, F,
      "timeout waiting for semaphore");
    goto cleanup;
  }
  have_semaphore = 1;
  
  header = (__header_t *)b;		// address header at start of buffer
  max = DT_MIN(max, header->l);		// don't overfill buffer
  memcpy(buffer, 			// copy contents of buffer
    (char *)b+sizeof(__header_t), 
    max);

  *xe = header->xe / 3;			// presume byte triplets
  *ye = header->ye;

cleanup:
  if (have_semaphore)
    ReleaseSemaphore(			// signal copy is done	
      semaphore, 1, NULL);
  if (semaphore != NULL)
    CloseHandle(semaphore);
  if (b != NULL)
    UnmapViewOfFile(b);
  if (mapping != NULL)
    CloseHandle(mapping);
  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
// read frame data from shared file
static
HRESULT 
__readit(
  dt_ctl_t *ctl,			// environment control
  const char *filename,			// filename to read
  void *buffer,				// buffer returned
  long max,
  long *xe,
  long *ye)
{
  DT_F("dtmj_grabit_c::__reopen");
  dtfd_t _fd, *fd = &_fd;
  dt_rc_e rc;

  int sysrc;
  int e;
  sysrc = _sopen(			// open shared file
    filename,
    O_RDONLY|O_BINARY,
    SH_DENYNO,
    S_IREAD|S_IWRITE);
  e = errno;

  if (sysrc == -1)			// file failed to open?
  {
    rc = dt_err(ctl, F,
      "sopen(%s, "
      "O_RDONLY|O_BINARY, "
      "SH_DENYNO, "
      "S_IREAD|S_IWRITE)"
      DTERRNO_FMT,
      filename, DTERRNO_ARG(e));
    goto cleanup;
  }

  DT_C(dtfd_fd,(ctl, fd,		// wrap fd around shared file 	       
    filename, sysrc));	

  __header_t header;
  DT_C(dtfd_read_all,(ctl, fd,		// read header from file
    &header, sizeof(header)));

  max = DT_MIN(max, header.l);		// don't overfill buffer

  DT_C(dtfd_read_all,(ctl, fd,		// read bitmap data from file
    buffer, max));

  *xe = header.xe / 3;			// presume byte triplets
  *ye = header.ye;

cleanup:
  if (fd->fd != -1)
    DT_C(dtfd_close,(ctl, fd));		// close file
  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}

//--------------------------------------------------------------------------
// get data from tabgrab
static
HRESULT 
__fillit(
  dt_ctl_t *ctl,			// environment control
  const char *ipcspec,
  long *frame,
  void *buffer,				// buffer returned
  long max,
  long *xe,
  long *ye)
{
  DT_F("dtmj_grabit_c::__fillit");
  char filename[256];

  DTAX_Q(__grab,(ctl, ipcspec, frame,	// tell tabgrab we want it
    filename, sizeof(filename)));

  if (filename[0] == '*')		// this is a shared file?
    DTAX_Q(__copyit,(ctl,	 	// copy shared file
      filename, buffer, max, xe, ye))
  else					// normal file?
    DTAX_Q(__readit,(ctl, 		// read file
      filename, buffer, max, xe, ye));		

  return NOERROR;
}

//--------------------------------------------------------------------------
// (stream) NotifyEvent
//
HRESULT 
dtmj_grabit_stream_c::notify_event(
  long code,
  long param1,
  long param2)
{
  DTAX_F("dtmj_grabit_stream_c::notify_event");
  PIN_INFO pinfo;
  FILTER_INFO finfo;
  IFilter *filter = NULL;
  IFilterGraph *graph = NULL;
  IMediaEventSink *sink = NULL;
  dt_rc_e rc;
  
  DTAX_C(this->QueryPinInfo,(&pinfo));	// info about our pin persona
  filter = pinfo.pFilter;		// our owning filter
  if (filter == NULL)
    goto cleanup;

  DTAX_C(filter->QueryFilterInfo,(	// info about our filter
    &finfo));
  graph = finfo.pGraph;			// specifically the graph
  if (graph == NULL)
    goto cleanup;

  DTAX_C(graph->QueryInterface,(	// the graph's event sink
    IID_IMediaEventSink,
    (void **)&sink));
  if (sink != NULL)
    goto cleanup;

  DTAX_C(sink->Notify,(code,		// send straight to the sink
    param1, param2));

cleanup:
    if (sink != NULL)
      sink->Release();
    if (graph != NULL)
      graph->Release();
    if (filter != NULL)
      filter->Release();
  return NOERROR;
}

//--------------------------------------------------------------------------
// (stream) FillBuffer
//
// Plots a ball into the supplied video buffer
//
HRESULT 
dtmj_grabit_stream_c::FillBuffer(
  IMediaSample *osample)
{
  CAutoLock lock(&this->shared);
  DTAX_F("dtmj_grabit_stream_c::FillBuffer");
  BYTE *obuff;
  long osize;
  long xe, ye;
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_TRACE,
    "called");

  osample->GetPointer(&obuff);		// access filter graph buffer
  osize = osample->GetSize();

  DTAX_C(__fillit,(this->ctl,		// get frame from tabgrab
    this->prop.ipcspec,
    &this->frame, obuff, osize,
    &xe, &ye));
      
  if (xe != this->xe ||			// check size
      ye != this->ye)
  {
    rc = dt_err(ctl, F,
      "stream size changed from %dx%d to %dx%d",
      this->xe, this->ye, xe, ye);
    goto cleanup;
  }

  osample->SetSyncPoint(TRUE);		// every mjpeg frame is key

  dt_dbg(ctl, F, DT_DBG_MASK_USR2,
    "grabbed one");
  
cleanup:
  if (rc != DT_RC_GOOD)
    this->notify_event(			// tell the filter graph 
      EC_ERRORABORT, 0, 0);

  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}


//--------------------------------------------------------------------------
// (stream) GetMediaType
//
HRESULT dtmj_grabit_stream_c::GetMediaType(
  int iPosition, 
  CMediaType *media)
{
  CAutoLock lock(m_pFilter->pStateLock());

  DTAX_F("dtmj_grabit_stream_c::GetMediaType");
  dt_rc_e rc;

  dt_dbg(ctl, F, DT_DBG_MASK_TRACE,
    "called, iPosition=%d", iPosition);

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
  memset(videoinfo, 0,
    sizeof(VIDEOINFO));

  if (this->xe == 0 ||			// haven't done this yet?
      this->ye == 0)
  {
    DTAX_C(__fillit,(this->ctl,		// get first frame from tabgrab
      this->prop.ipcspec,
      &this->frame, NULL, 0,		// not interested in the data
      &this->xe, &this->ye));
    dt_dbg(ctl, F, DT_DBG_MASK_USR1,
      "initial grab size %dx%d",
      this->xe, this->ye);
  }

  videoinfo->bmiHeader.biSize		= sizeof(BITMAPINFOHEADER);
  videoinfo->bmiHeader.biWidth		= this->xe;
  videoinfo->bmiHeader.biHeight		= this->ye;
  videoinfo->bmiHeader.biPlanes		= 1;
  videoinfo->bmiHeader.biCompression    = BI_RGB;
  videoinfo->bmiHeader.biBitCount       = 24;
  videoinfo->bmiHeader.biSizeImage	= 
    (long)videoinfo->bmiHeader.biWidth *
    (long)videoinfo->bmiHeader.biHeight * 3;
  videoinfo->bmiHeader.biClrUsed	= 0;
  videoinfo->bmiHeader.biClrImportant	= 0;

  media->SetFormat((BYTE *)videoinfo, sizeof(*videoinfo));
  media->SetTemporalCompression(FALSE);
  media->SetSampleSize(videoinfo->bmiHeader.biSizeImage);

cleanup:
  if (rc != DT_RC_GOOD)
    this->notify_event(			// tell the filter graph 
      EC_ERRORABORT, 0, 0);

  dt_dbg(ctl, F, DT_DBG_MASK_TRACE,
    "returning, rc=%d", rc);
  
  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
} 


//--------------------------------------------------------------------------
// (stream) CheckMediaType
//
HRESULT 
dtmj_grabit_stream_c::CheckMediaType(
  const CMediaType *media)
{
  CAutoLock lock(m_pFilter->pStateLock());
  DTAX_F("dtmj_grabit_stream_c::CheckMediaType");

  int istype = IsEqualGUID(*media->Type(), MEDIATYPE_Video);
  int issubtype = IsEqualGUID(*media->Subtype(), MEDIASUBTYPE_RGB24);
  int isformat = IsEqualGUID(*media->FormatType(), FORMAT_VideoInfo);
  
  dt_rc_e rc = DT_RC_GOOD;

  if (!istype ||			// check type and format
      !issubtype ||
      !isformat)
    return E_INVALIDARG;

  if (!media->IsFixedSize())		// we only do fixed-size samples
    return E_INVALIDARG;

  VIDEOINFO *videoinfo = (VIDEOINFO *)	// look into the format
    media->Format();

  if (videoinfo == NULL)		// no format?
    return E_INVALIDARG;

  long xe, ye;
  xe = videoinfo->bmiHeader.biWidth;	// get format's size
  ye = videoinfo->bmiHeader.biHeight;

  if (this->xe == 0 ||			// haven't done this yet?
      this->ye == 0)
    DTAX_C(__fillit,(this->ctl,		// get first frame from tabgrab
      this->prop.ipcspec,
      &this->frame, NULL, 0,		// not interested in the data
      &this->xe, &this->ye));
  
  if (xe != this->xe ||			// check size
      ye != this->ye)
    return E_INVALIDARG;

cleanup:
  if (rc != DT_RC_GOOD)
    this->notify_event(			// tell the filter graph 
      EC_ERRORABORT, 0, 0);

  return rc == DT_RC_GOOD?
    S_OK: E_FAIL;
}

//--------------------------------------------------------------------------
// (stream) DecideBufferSize
//
// This will always be called after the format has been sucessfully
// negotiated. So we have a look at m_mt to see what size image we agreed.
// Then we can ask for buffers of the correct size to contain them.
//
HRESULT 
dtmj_grabit_stream_c::DecideBufferSize(
  IMemAllocator *pAlloc,
  ALLOCATOR_PROPERTIES *pProperties)
{
  CAutoLock lock(m_pFilter->pStateLock());

    ASSERT(pAlloc);
    ASSERT(pProperties);
    HRESULT hr = NOERROR;

    VIDEOINFO *videoinfo = (VIDEOINFO *) 
      m_mt.Format();
    pProperties->cBuffers = 1;
    pProperties->cbBuffer = videoinfo->bmiHeader.biSizeImage;

    ASSERT(pProperties->cbBuffer);

    // Ask the allocator to reserve us some sample memory, NOTE the function
    // can succeed (that is return NOERROR) but still not have allocated the
    // memory that we requested, so we must check we got whatever we wanted

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pProperties,&Actual);
    if (FAILED(hr)) 
      return hr;

    // Is this allocator unsuitable

    if (Actual.cbBuffer < pProperties->cbBuffer) 
        return E_FAIL;

    ASSERT( Actual.cBuffers == 1 );
    return NOERROR;

}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// (stream) dtmj_grabit_prop_i::get_prop
// return current jpeg encoding quality
// called by property page or VB
//
void
dtmj_grabit_stream_c::get_prop(
  dtmj_grabit_prop_t *prop)
{
  CAutoLock lock(&this->proplock);

  *prop = this->prop;
}

//--------------------------------------------------------------------------
// (stream) dtmj_grabit_prop_i::put_prop
// change jpeg encoding quality starting with next frame
// called by property page or VB
//
void
dtmj_grabit_stream_c::put_prop(
  dtmj_grabit_prop_t *prop,
  char *message,
  int max)
{
  CAutoLock lock(&this->proplock);

  this->prop = *prop;
  
  __dbgmask(ctl, this->prop.dbgspec);	// set new debug mask
}

//--------------------------------------------------------------------------
// (stream) dtmj_grabit_stream_c::put_ctl
// called by source filter
//
void
dtmj_grabit_stream_c::put_ctl(
  dt_ctl_t *ctl)
{
  DT_F("dtmj_grabit_stream_c::put_ctl");

  CAutoLock lock(&this->proplock);

  this->ctl = ctl;

  dt_dbg(this->ctl, F, DT_DBG_MASK_TRACE,
    "new ctl set");
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
// (stream) dtmj_grabit_stream_c::dtmj_command_i_process
//
HRESULT
dtmj_grabit_stream_c::dtmj_command_i_process(
  const char *command)
{
  CAutoLock lock(&this->shared);

  DTAX_F("dtmj_grabit_stream_c::dtmj_command_i_process");

  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "sending \"%s\" to tabgrab", command);

  DTAX_Q(__send,(this->ctl,		// get frame from tabgrab
    this->prop.ipcspec,
    command));
      
  return NOERROR;
}
