Option Explicit

global const PALLETTE_SYSTEM = 1
global const PALLETTE_OWN    = 2
global const PALLETTE_GROUP  = 3

' ------------------------------------------------------------------------
sub pallette_set (f as imageform, mode as integer, duration as long)
  if duration <> 0 then                 ' caller wants temporary pallette?
    f.thetimer.interval = duration
    f.thetimer.enabled = true
  end if
  #if isvb then
    call isvb_pallette_set(f, mode)
  #end if
  #if lead then
    call lead_pallette_set(f, mode)
  #end if
End sub
