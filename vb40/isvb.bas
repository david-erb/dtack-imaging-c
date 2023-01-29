Option Explicit
' ------------------------------------------------------------------------
' 
function isvb_oneliner(f as form) as string
  dim s as string
      s = s + " topleft=" + Trim$(Str$(f.theimage.bBox_left))
      s = s + "x" + Trim$(Str$(f.theimage.bBox_top))
      s = s + ", rightbottom=" + Trim$(Str$(f.theimage.bBox_right))
      s = s + "x" + Trim$(Str$(f.theimage.bBox_bottom))
      s = s + ", size=" + Trim$(Str$(f.theimage.Width))
      s = s + "x" + Trim$(Str$(f.theimage.Width))
      s = s + ", inch=" + Trim$(Str$(f.theimage.inch))
  isvb_oneliner = s
end function


' ------------------------------------------------------------------------
' 
Sub isvb_saveas(f as form)
  f.theimage.Preferences = True
  f.theimage.SaveAs = True
end sub

' ------------------------------------------------------------------------
' set form to match image width
Sub isvb_unclip_x(f as form)
  dim fid as string
  fid = "isvb_unclip_x"

  dim xe as integer
  xe = f.theimage.bbox_right 

  dim s as string
  s = profile_get(util_ini(), PROFILE_APP_GLOBALS, PROFILE_KEY_XBORDER)
  f.Width = CDbl(s) + xe * screen.TwipsPerPixelX

  f.thewname.width = xe			' full width for image name box

  f.theimage.width = xe
end sub

' ------------------------------------------------------------------------
' set form to match image height
Sub isvb_unclip_y(f as form)
  dim fid as string
  fid = "isvb_unclip_y"

  dim ye as integer
  ye = f.theimage.bbox_bottom

  dim s as string
  s = profile_get(util_ini(), PROFILE_APP_GLOBALS, PROFILE_KEY_YBORDER)
  f.Height = CDbl(s) + (ye + f.thewname.top) * screen.TwipsPerPixelY

  f.theimage.height = ye
end sub

' ------------------------------------------------------------------------
' set form to match image
Sub isvb_unclip(f as form)
  f.theimage.inch = 96			' so stretchdib in mf maps 1-to-1
  call isvb_unclip_x(f)
  call isvb_unclip_y(f)
end sub

' ------------------------------------------------------------------------
' clip image inside window limits

Sub isvb_clip(f as form)
  ' this function may get called during unclipping!!
end sub

' ------------------------------------------------------------------------
' read wmf file into image

function isvb_read( _
  f as form, _
  filename as string, _
  filetype as string _
) as integer
  Dim Q As String
  Q = Chr$(34)
  dim s as string

  f.theimage.Visible = false
  f.theimage.ErrorCode = 0
  f.theimage.FileOffset = 0
  f.theimage.ImportFileType = "WMF"
  f.theimage.ImportFileSpec = filename
   if f.theimage.ErrorCode <> 0 Then
    s = "Sorry, could not read image window "
    s = s + Q + f.thewname.text + Q
    s = s + " from file " + filename
    s = s + " (" + f.theimage.ErrorString + ")."
    call notify_error(s)
    isvb_read = RC_BAD
    exit function
  else
    call isvb_unclip(f)			' adjust window to fit image
    f.theimage.Visible = true
    f.Show
  end if

  isvb_read = RC_GOOD

end function

' ------------------------------------------------------------------------
' write image into wmf file

Sub isvb_write_wmf(f as form, filename as string)
  Dim Q As String
  Q = Chr$(34)
  dim s as string

    f.theimage.ErrorCode = 0
    f.theimage.FileOffset = 0
    f.theimage.ExportFileType = "WMF"
    f.theimage.ExportFileSpec = filename

    if f.theimage.ErrorCode <> 0 Then
      s = "Sorry, could not write image window " + Q + f.thewname.text + Q
      s = s + " to file " + filename
      s = s + " (" + f.theimage.ErrorString + ")."
      call notify_error(s)
      exit sub
    end if

end sub

' ------------------------------------------------------------------------
' interactively browse for input image filename

function isvb_open( _
  f as form _
) as string

  f.theimage.ErrorPrmt = True
  f.theimage.Preferences = True
  f.theimage.Open = True

  if f.theimage.errorcode <> 0 then
    isvb_open = ERROR_STRING
    exit function
  end if

  isvb_open = f.theimage.importfilespec
End function

' ------------------------------------------------------------------------
sub isvb_pallette_set( _
  f as form, _
  mode as integer _
)
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
