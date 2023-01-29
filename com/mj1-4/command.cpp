#include <windows.h>
#include "dtax.h"
#define DTDEPEND
#include <dtmsw.h>

#include <streams.h>
#include "guid.h"
#include "command.h"

//--------------------------------------------------------------------------

HRESULT
dtmj_command_passthrough(
  dt_ctl_t *ctl,
  CBaseFilter *us,
  const char *command)
{
  DTAX_F("dtmj_command_passthrough");
  int i, n;
  CBasePin *cpin;
  PIN_DIRECTION dir;
  PIN_INFO info;	
  IPin *ipin = NULL;
  IFilter *filter = NULL;
  dtmj_command_i *command_i = NULL;
  HRESULT hr;
  dt_rc_e rc = DT_RC_GOOD;

  n = us->GetPinCount();		// check our pin count
  dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
    "pin count %d", n);
  for (i=0; i<n; i++)
  {
    cpin = us->GetPin(i);		// address the next pin

    if (!cpin->IsConnected())		// input pin is not connected?
    {
      dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "pin %d is not connected", i);
      continue;
    }

    DTAX_C(cpin->QueryDirection,(	// check pin's direction
      &dir));

    if (dir != PINDIR_INPUT)		// not an input pin to us?
    {
      dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "pin %d is not an input pin", i);
      continue;
    }
    
    ipin = cpin->GetConnected();	// find upstream connected pin
    
    DTAX_C(ipin->QueryPinInfo,(&info));
    filter = info.pFilter;		// upstream pin's owning filter
    
    if (filter == NULL)
    {
      dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "pin %d is not associated with a filter", i);
      continue;
    }
      
    hr = filter->QueryInterface( 	// see if filter does command_i
      dtmj_command_i_guid, 
      (void **)&command_i);

    if (!(hr & 0x80000000) &&		// QueryInterface succeeded?
	command_i != NULL)		// filter does command_i?
    {
      DTAX_C(command_i->		// give command to interface
        dtmj_command_i_process,(
          command));
    
      command_i->Release();
      command_i = NULL;
    }
    else
      dt_dbg(ctl, F, DT_DBG_MASK_COMMAND,
        "pin %d does not implement command_i", i);
      
    filter->Release();
    filter = NULL;
  }

cleanup:
  if (command_i != NULL)		// we accessed this interface?
    command_i->Release();		// release the interface
  if (filter != NULL)
    filter->Release();

  return rc == DT_RC_GOOD?
    NOERROR: E_FAIL;
}
