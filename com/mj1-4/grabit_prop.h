/*!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
  ! Copyright (C) 1998 Dtack Inc. All Rights Reserved
  ! To use this file, you must have signed a license agreement with Dtack Inc.
  ! Under no circumstances may you redistribute this file.
  ! This software is provided AS IS with no warranty expressed or implied.
  ! Dtack Inc. accepts no liability for use or misuse of this file.
  ! http://www.dtack.com  dtack@dtack.com  telephone +360.417.6700
  ! Dtack Inc, P.O. Box 3061, Port Angeles, WA USA 98362
  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! */




#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------------
// the structure

typedef struct {
  char ipcspec[128];
  char dbgspec[128];
} dtmj_grabit_prop_t;

//--------------------------------------------------------------------------
// the interface

DECLARE_INTERFACE_(dtmj_grabit_prop_i, IUnknown)
{
  STDMETHOD(get_prop) (THIS_
	dtmj_grabit_prop_t *prop
  ) PURE;

  STDMETHOD(put_prop) (THIS_
    dtmj_grabit_prop_t *prop, char *message, int max
  ) PURE;

  STDMETHOD(put_ctl) (THIS_
    dt_ctl_t *ctl
  ) PURE;
};

#ifdef __cplusplus
}
#endif

//--------------------------------------------------------------------------
// the class

class dtmj_grabit_prop_c: 
  public CBasePropertyPage
{

public:
  static CUnknown *CreateInstance(LPUNKNOWN lpunk, HRESULT *phr);

private:

  BOOL OnReceiveMessage(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
  HRESULT OnConnect(IUnknown *pUnknown);
  HRESULT OnDisconnect();
  HRESULT OnActivate();
  HRESULT OnDeactivate();
  HRESULT OnApplyChanges();

  dtmj_grabit_prop_c(LPUNKNOWN lpunk, HRESULT *phr);

  BOOL initialized;   
  dtmj_grabit_prop_t prop;
  dtmj_grabit_prop_i *prop_i; 

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
