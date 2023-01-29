Option Explicit
' ------------------------------------------------------------------------
' 
function lead_oneliner(f as form) as string
  dim s as string
  s = s + " height=" + Trim$(Str$(f.theimage.height))
  lead_oneliner = s
end function

' ------------------------------------------------------------------------
' interactively browse for input image filename

function lead_open( _
  f as form, _
  filetype as string _
) as string

  f.thedialog.showopen

  dim rc as integer
  rc = lead_read(f, f.thedialog.filename, filetype)

  if rc = RC_GOOD then
    lead_open = f.thedialog.filename
  else
    lead_open = ERROR_STRING
  end if
End function

' ------------------------------------------------------------------------
' 
Sub lead_saveas(f as form)

  f.thedialog.showsave

end sub

' ------------------------------------------------------------------------
' set form to match image width

Sub lead_unclip_x(f as form)
  dim fid as string
  fid = "lead_unclip_x"

  f.theimage.left = _
    f.thewname.left

  f.theimage.width = _
    f.theimage.infowidth * _
    screen.TwipsPerPixelX

  dim s as string
  s = profile_get(util_ini(), PROFILE_APP_GLOBALS, PROFILE_KEY_XBORDER)
  f.Width = CDbl(s) + _
    f.theimage.left + _
    f.theimage.width

  f.thewname.width = f.scalewidth
end sub

' ------------------------------------------------------------------------
' set form to match image height
Sub lead_unclip_y(f as form)
  dim fid as string
  fid = "isvb_unclip_y"

  f.theimage.top = _
    f.thewname.top + _
    f.thewname.height

  f.theimage.height = _
    f.theimage.infoheight * _
    screen.TwipsPerPixelY

  dim s as string
  s = profile_get(util_ini(), PROFILE_APP_GLOBALS, PROFILE_KEY_YBORDER)
  f.Height = CDbl(s) + _
    f.theimage.top + _
    f.theimage.height

end sub

' ------------------------------------------------------------------------
' set form to match image
Sub lead_unclip(f as form)
  call lead_unclip_x(f)
  call lead_unclip_y(f)
end sub

' ------------------------------------------------------------------------
' clip image inside window limits

Sub lead_clip(f as form)
  ' this function may get called during unclipping!!
end sub

' ------------------------------------------------------------------------
' read file into image

function lead_read( _
  f as form, _
  filename as string, _
  filetype as string _
) as integer

  Dim Q As String
  Q = Chr$(34)
  dim rc as integer

  f.theimage.enablemethoderrors = false

  rc = f.theimage.getfileinfo(filename, 0)

  if rc <> 0 then
    notify_error _
      "Sorry, could not get info from file " + _
      Q + filename + Q + _
      " (" + lead_error_string(rc) + ")."
    lead_read = RC_BAD
    exit function
  end if

  rc = f.theimage.load(filename, 0, 0)

  if rc <> 0 then
    notify_error _
      "Sorry, could not load file " + _
      Q + filename + Q + _
      " (" + lead_error_string(rc) + ")."
    lead_read = RC_BAD
    exit function
  end if

  lead_read = RC_GOOD
end function

' ------------------------------------------------------------------------
' write image into file

Sub lead_write( _
  f as form, _
  filename as string, _
  filetype as string _
)
  Dim Q As String
  Q = Chr$(34)
  dim rc as integer

  f.theimage.enablemethoderrors = false

  dim typecode as integer
  typecode = lead_type_code(filetype)

  rc = f.theimage.save(filename, typecode, 8, 0, 0)

  if rc <> 0 then
    notify_error _
      "Sorry, could not save file " + _
      Q + filename + Q + _
      " as type " + filetype + _
      " (" + lead_error_string(rc) + ")."
    exit sub
  end if

end sub

' ------------------------------------------------------------------------
sub lead_pallette_set( _
  f as form, _
  mode as integer _
)

End sub

' ------------------------------------------------------------------------
function lead_type_code( _
  filetype as string _
) as integer
  
  select case filetype
# if lead then
  case "wmf"
    lead_type_code = FILE_WMF
  case "bmp"
    lead_type_code = FILE_BMP
# end if
  case else
    lead_type_code = -1
  end select

end function

' ------------------------------------------------------------------------
function lead_error_string( _
  rc as integer _
) as string

  select case rc
  case 20001
    lead_error_string = "Not enough memory available."
  case 20002
    lead_error_string = "Invalid bitmap handle."
  case 20003
    lead_error_string = "Not enough memory available."
  case 20004
    lead_error_string = "Error seeking to position."
  case 20005
    lead_error_string = "Error writing file."
  case 20006
    lead_error_string = "File not present - abort."
  case 20007
    lead_error_string = "Error reading file."
  case 20008
    lead_error_string = "Invalid filename specified."
  case 20009
    lead_error_string = "Invalid file format."
  case 20010
    lead_error_string = "File not found."
  case 20011
    lead_error_string = "Invalid width/height."
  case 20012
    lead_error_string = "Image format recognized, but sub-type not supported."
  case 20013
    lead_error_string = "Invalid parmameter passed."
  case 20014
    lead_error_string = "Not able to open file."
  case 20015
    lead_error_string = "Unknown compression format."
  case 20016
    lead_error_string = "Not available for file format requested."
  case 20017
    lead_error_string = "VGA card only supports 256 colors (8 bit)."
  case 20018
    lead_error_string = "Printer error."
  case 20019
    lead_error_string = "Data CRC check error."
  case 20021
    lead_error_string = "Invalid QFactor specified."
  case 20022
    lead_error_string = "TARGA not installed."
  case 20023
    lead_error_string = "Invalid compression format."
  case 20024
    lead_error_string = "X origin specified invalid."
  case 20025
    lead_error_string = "Y origin specified invalid."
  case 20026
    lead_error_string = "Invalid video mode."
  case 20027
    lead_error_string = "Invalid bits/pixel."
  case 20028
    lead_error_string = "Invalid window size."
  case 20029
    lead_error_string = "Escape key pressed."
  case 20063
    lead_error_string = "Fixed palette data not found."
  case 20064
    lead_error_string = "Fixed palette data not found."
  case 20065
    lead_error_string = "Stamp not found."
  case 20070
    lead_error_string = "Invalid group 3 code."
  case 20071
    lead_error_string = "Group3 code end of file."
  case 20072
    lead_error_string = "Group3 code end of code."
  case 20073
    lead_error_string = "Premature EOF at scanline."
  case 20074
    lead_error_string = "Premature end-of-line code."
  case 20075
    lead_error_string = "Premature end-of-line code."
  case 20076
    lead_error_string = "Premature end-of-line code."
  case 20077
    lead_error_string = "Premature end-of-line code."
  case 20078
    lead_error_string = "Bad version number."
  case 20079
    lead_error_string = "Source manager not found."
  case 20080
    lead_error_string = "Failure due to unknown causes."
  case 20081
    lead_error_string = "Not enough memory to perform operation."
  case 20082
    lead_error_string = "No Data Source."
  case 20083
    lead_error_string = "DS is connected to max possible apps."
  case 20084
    lead_error_string = "DS or DSM reported error, app shouldn't."
  case 20085
    lead_error_string = "Unknown capability."
  case 20086
    lead_error_string = "Unrecognized MSG DG DAT combination."
  case 20087
    lead_error_string = "Data parameter out of range."
  case 20088
    lead_error_string = "DG DAT MSG out of expected sequence."
  case 20089
    lead_error_string = "Unknown destination App/Src in DSM_Entry."
  case 20090
    lead_error_string = "Operation was cancelled."
  case 20100
    lead_error_string = "The user cancelled the operation."
  case else
    lead_error_string = "Unknown error" + str$(rc) + "."
  end select
End function
