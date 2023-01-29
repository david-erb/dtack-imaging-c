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

#include <windows.h>
#include <windowsx.h>

#include "dtax.h"
#include <dtlist.h>
#include <dtipc.h>
#include <dtos.h>
#include <dtdg.h>
#define DTDEPEND
#include <dtmsw.h>

#include <streams.h>
#include <commctrl.h>
#include <olectl.h>
#include <memory.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include "resource.h"
#include "guid.h"
#include "command.h"
#include "encode_prop.h"
#include "encode.h"


//--------------------------------------------------------------------------
// CreateInstance
//
// Used by the ActiveMovie base classes to create instances
//
CUnknown *dtmj_encode_prop_c::CreateInstance(LPUNKNOWN lpunk, HRESULT *phr)
{
  CUnknown *punk = new dtmj_encode_prop_c(lpunk, phr);
  if (punk == NULL) 
    *phr = E_OUTOFMEMORY;
  return punk;
}


//--------------------------------------------------------------------------
// Constructor
//
dtmj_encode_prop_c::dtmj_encode_prop_c(
  LPUNKNOWN pUnk, 
  HRESULT *phr):
CBasePropertyPage(NAME("prop"), pUnk,phr,IDD_EZrgb24PROP,IDS_TITLE),
prop_i(NULL),
m_bIsInitialized(FALSE)
{
  DTAX_F("dtmj_encode_prop_c::dtmj_encode_prop_c");
  memset(&this->prop, 0, sizeof(this->prop));
  this->prop.jquality = 60;
  ASSERT(phr);
} 


//--------------------------------------------------------------------------
// OnReceiveMessage
//
// Handles the messages for our property window
//
BOOL dtmj_encode_prop_c::OnReceiveMessage(HWND hwnd,
                                          UINT uMsg,
                                          WPARAM wParam,
                                          LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_COMMAND:
        {
            if (m_bIsInitialized)
            {
                m_bDirty = TRUE;
                if (m_pPageSite)
                {
                    m_pPageSite->OnStatusChange(PROPPAGESTATUS_DIRTY);
                }
            }
            return (LRESULT) 1;
        }

    }
    return CBasePropertyPage::OnReceiveMessage(hwnd,uMsg,wParam,lParam);

} // OnReceiveMessage


//--------------------------------------------------------------------------
// OnConnect
//
// Called when we connect to a transform filter
//
HRESULT 
dtmj_encode_prop_c::OnConnect(
  IUnknown *pUnknown)
{
  DTAX_F("dtmj_encode_prop_c::OnConnect");
  ASSERT(this->prop_i == NULL);

  HRESULT hr = pUnknown->QueryInterface(
    dtmj_encode_prop_i_guid, 
    (void **)&this->prop_i);
  if (!SUCCEEDED(hr))
    return E_FAIL;

  ASSERT(this->prop_i);

  this->prop_i->get_prop(&this->prop);
  m_bIsInitialized = FALSE;
  return NOERROR;
} 


//--------------------------------------------------------------------------
// OnDisconnect
//
// Likewise called when we disconnect from a filter
//
HRESULT 
dtmj_encode_prop_c::OnDisconnect()
{
  DTAX_F("dtmj_encode_prop_c::OnDisconnect");
  if (this->prop_i == NULL) 
    return E_UNEXPECTED;
  this->prop_i->Release();
  this->prop_i = NULL;
  return NOERROR;
} 


//--------------------------------------------------------------------------
// OnActivate
//
// We are being activated
//
HRESULT dtmj_encode_prop_c::OnActivate()
{
  DTAX_F("dtmj_encode_prop_c::OnActivate");
  TCHAR   sz[60];

  _stprintf(sz, TEXT("%ld"), this->prop.jquality);
  Edit_SetText(GetDlgItem(m_Dlg, IDC_QUALITY), sz);

  m_bIsInitialized = TRUE;
  return NOERROR;
}


//--------------------------------------------------------------------------
// OnDeactivate
//
// We are being deactivated
//
HRESULT 
dtmj_encode_prop_c::OnDeactivate(void)
{
  DTAX_F("dtmj_encode_prop_c::OnDeactivate");
  ASSERT(this->prop_i);
  this->prop_i->get_prop(&this->prop);
  return NOERROR;
}

//--------------------------------------------------------------------------
// OnApplyChanges
//
// Apply any changes so far made
//
HRESULT dtmj_encode_prop_c::OnApplyChanges()
{
  DTAX_F("dtmj_encode_prop_c::OnApplyChanges");
  ASSERT(this->prop_i);
  TCHAR sz[STR_MAX_LENGTH];

  Edit_GetText(GetDlgItem(m_Dlg, IDC_QUALITY), sz, STR_MAX_LENGTH);
  this->prop.jquality = atol(sz);

  char message[256];
  this->prop_i->put_prop(&this->prop, message, sizeof(message));
  return NOERROR;
} 

