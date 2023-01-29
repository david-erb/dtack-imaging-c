Option Explicit

' ---------------------------------------------------------------------
'
Sub restart_load( _
  f As restartform _
)
  f.thetimer.interval = 1000
  f.thetimer.enabled =  true
  f.remaining.caption = "10"
End Sub

' ---------------------------------------------------------------------
'
Sub restart_go( _
  f As restartform _
)
  f.thetimer.enabled = false
  call util_sendmessage( _
    f.parent.caption, _
    WM_KEYDOWN, _
    CONST_KEY_RUN, 0)			' give a run to the param
  f.hide 
End Sub

' ---------------------------------------------------------------------
'
Sub restart_cancel( _
  f As restartform _
)
  f.thetimer.enabled = false
  f.hide
End Sub

' ---------------------------------------------------------------------
'
Sub restart_tick( _
  f As restartform _
)
  f.thetimer.enabled = false
  dim remaining as integer
  remaining = util_s2i(f.remaining.caption)
  if remaining = 1 then
    call restart_go(f)
    exit sub
  else
    remaining = remaining - 1
    f.remaining.caption = trim$(str$(remaining))
    f.remaining.refresh
  end if
  f.thetimer.interval = 1000
  f.thetimer.enabled =  true
End Sub
