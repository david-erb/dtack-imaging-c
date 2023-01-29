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
  if mode = PALLETTE_SYSTEM then
    f.theimage.palettemode = 0          
  else
  if mode = PALLETTE_OWN then
    f.theimage.palettemode = 1          
  else
  if mode = PALLETTE_GROUP then
    f.theimage.palettemode = 2          
  end if
  end if
  end if
End sub
