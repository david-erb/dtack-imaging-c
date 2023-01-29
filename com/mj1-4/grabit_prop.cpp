#include <windows.h>
#include <windowsx.h>

#include "dtax.h"
#include <dtipc.h>
#include <dtos.h>
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
#include "grabit_prop.h"
#include "grabit.h"


//--------------------------------------------------------------------------
// CreateInstance
//
// Used by the ActiveMovie base classes to create instances
//
CUnknown *
dtmj_grabit_prop_c::CreateInstance(
  LPUNKNOWN lpunk, 
  HRESULT *phr)
{
  CUnknown *punk = new dtmj_grabit_prop_c(lpunk, phr);
  if (punk == NULL) 
    *phr = E_OUTOFMEMORY;
  return punk;

}


//--------------------------------------------------------------------------
// Constructor
//
dtmj_grabit_prop_c::dtmj_grabit_prop_c(
  LPUNKNOWN pUnk, 
  HRESULT *phr) :
CBasePropertyPage(NAME("prop"), pUnk, phr, IDD_EZrgb24PROP, IDS_TITLE),
prop_i(NULL),
initialized(FALSE)
{
  ASSERT(phr);
}


//--------------------------------------------------------------------------
// OnReceiveMessage
//
// Handles the messages for our property window
//
BOOL dtmj_grabit_prop_c::OnReceiveMessage(HWND hwnd,
                                          UINT uMsg,
                                          WPARAM wParam,
                                          LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_COMMAND:
        {
            if (this->initialized)
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
HRESULT dtmj_grabit_prop_c::OnConnect(IUnknown *pUnknown)
{
  ASSERT(this->prop_i == NULL);
  HRESULT hr;

  hr = pUnknown->QueryInterface(
    dtmj_grabit_prop_i_guid, 
    (void **)&this->prop_i);
  if (FAILED(hr)) 
    return E_NOINTERFACE;

  ASSERT(this->prop_i);

  this->prop_i->get_prop(&this->prop);
					// ................................
  this->initialized = FALSE ;

  return NOERROR;

} // OnConnect


//--------------------------------------------------------------------------
// OnDisconnect
//
// Likewise called when we disconnect from a filter
//
HRESULT dtmj_grabit_prop_c::OnDisconnect()
{
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
HRESULT dtmj_grabit_prop_c::OnActivate()
{
  Edit_SetText(GetDlgItem(m_Dlg, IDC_IPCSPEC), this->prop.ipcspec);
  Edit_SetText(GetDlgItem(m_Dlg, IDC_DBGSPEC), this->prop.dbgspec);

  this->initialized = TRUE;
  return NOERROR;
}


//--------------------------------------------------------------------------
// OnDeactivate
//
// We are being deactivated
//
HRESULT dtmj_grabit_prop_c::OnDeactivate(void)
{
  ASSERT(this->prop_i);
  this->prop_i->get_prop(&this->prop);

  return NOERROR;
}


//--------------------------------------------------------------------------
// OnApplyChanges
//
// Apply any changes so far made
//
HRESULT dtmj_grabit_prop_c::OnApplyChanges()
{
  DTAX_F("dtmj_grabit_prop_c::OnApplyChanges");
  ASSERT(this->prop_i);

  if (this->prop_i)
  {
    Edit_GetText(GetDlgItem(m_Dlg, 
      IDC_IPCSPEC), 
      this->prop.ipcspec, 
      sizeof(this->prop.ipcspec));
    Edit_GetText(GetDlgItem(m_Dlg, 
      IDC_DBGSPEC), 
      this->prop.dbgspec, 
      sizeof(this->prop.dbgspec));
    this->prop_i->put_prop(&this->prop, 
      NULL, 0);
  }

  return NOERROR;

} 

