/*******************************************************************************
    SNAPPY.H

    SNAPNTSC.DLL API (function prototypes)

    API functions:

    SnapGetDLLVersion
    SnapAlloc
    SnapFree
    SnapScanAndProcess
    SnapWriteDibFile
    SnapFreeDib

    Copyright(c) 1995, Play Incorporated
*******************************************************************************/

#if !defined(_snappy_h)
#define _snappy_h

#define STRICT
#include <windows.h>

#if defined(WIN32) || defined(_WIN32)
 #error SNAPNTSC.DLL is a 16 bit DLL
#endif

#define SNAPAPI WINAPI _loadds

typedef LPVOID SNAPPTR;     // define type of pointer to snap object

#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

////////////////////////////////////////////////////////////////////////////////
// Function: SnapGetDLLVersion
// Purpose:  To determine the version of the DLL that is loaded into memory
// Returns:  long version with the format: (ex. 1.0.058)
//           HIBYTE(HIWORD(ver)) = Major version (ex. 1)
//           LOBYTE(HIWORD(ver)) = Minor version (ex. 0)
//           LOWORD(ver)         = revision (ex. 058)
long SNAPAPI SnapGetDLLVersion(void);

////////////////////////////////////////////////////////////////////////////////
// Function:
//      SnapAlloc
// Parameters:
//      none
// Returns:
//      new snap object
// Purpose:
//      Alllocate a "snap" object.  Use to keep track of state of scanning
//      and all scanned data
SNAPPTR SNAPAPI
SnapAlloc(void);

////////////////////////////////////////////////////////////////////////////////
// Function:
//      SnapFree
// Parameters:
//      SNAPPTR snapobj - ptr to a snap object (may be NULL)
// Returns:
//      none
// Purpose:
//      Free a "snap" object
void SNAPAPI
SnapFree(SNAPPTR snapobj);

////////////////////////////////////////////////////////////////////////////////
// Function:
//      ScanAndProcess
// Parameters:
//      SNAPPTR snapobj
//      HGLOBAL hDib - handle to an existing image or NULL
// Returns:
//      HGLOBAL - handle of Processed DIB
//                this is the same as hDib if hDib was supplied,
//                otherwise it is the handle to a DIB that this routine created.
// Purpose:
//      Scan (Digitize) current video and process to the image specified by
//      the snap object's settings.  This process makes an image from the
//      current video source and converts it to a processed image.
//      The snapobj will retain the YUV information from the new image so that
//      ProcYUVToRGB() may be called to "re-process" the image.
//
//      If an existing image is supplied in hDib, the hDib will be replaced with
//      the new image.  Otherwise a new Dib will be allocated.
HGLOBAL SNAPAPI
SnapScanAndProcess(SNAPPTR snapobj, HGLOBAL hDib);


////////////////////////////////////////////////////////////////////////////////
// Function:
//      SnapWriteDibFile
// Parameters:
//      SNAPPTR snapobj
//      HGLOBAL hDib- The handle of the DIB to write
//      LPSTR FileName - Pointer to string of file name to write
// Returns:
//      BOOL - TRUE if successful
//             FALSE if not
// Purpose:
//      Writes out a Dib

BOOL SNAPAPI
SnapWriteDibFile(SNAPPTR snapobj, HGLOBAL hDib, LPSTR FileName);

////////////////////////////////////////////////////////////////////////////////
// Function:
//      SnapFreeDib
// Parameters:
//      HGLOBAL hDib- The handle of the DIB to free
// Purpose:
//      Free RAM of a DIB
#define SnapFreeDib(_hDib) GlobalFree(_hDib)

#ifdef __cplusplus
} // extern C
#endif  /* __cplusplus */

#endif // _snappy_h Sentry

