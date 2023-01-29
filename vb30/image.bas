Option Explicit

' ------------------------------------------------------------------------
' create new image instance and return its forms index

function image_create (winlist As winlistform, wname as string) as integer
  Dim f As imageform
  Set f = New imageform
  Load f

  dim s as string
  s = winlist_add(winlist, f, wname)

  dim k as integer
  k = winlist_search(winlist, s)	' search for k of new name
  if k = -1 then
    Dim Q As String
    Q = Chr$(34)
    call error_notify("Created image " + Q + s + Q + ", but now can't find it!")
    unload f
  end if

  image_create = k
End function

' ------------------------------------------------------------------------
sub image_virgin (winlist as winlistform) 
  dim k as integer
  k = image_create(winlist, "")
  if k <> -1 then
    forms(k).show
  end if
end sub

' ------------------------------------------------------------------------
function image_new (winlist as winlistform) as string
  dim k as integer
  k = image_create(winlist, "")
  if k = -1 then
    image_new = ERROR_STRING 
    exit function
  end if

  dim s as string
  s = image_open(forms(k), winlist)

  if s <> ERROR_STRING then
    forms(k).show
  end if
  image_new = s
End function

' ------------------------------------------------------------------------
' interactively browse for input image filename

function image_open (f as form, winlist as winlistform) as string
  f.theimage.ErrorPrmt = True
  f.theimage.Preferences = True
  f.theimage.Open = True

  if f.theimage.errorcode <> 0 then
    image_open = ERROR_STRING
    exit function
  end if

  dim s as string
  s = f.theimage.importfilespec		' assume image name is filename
  s = winlist_add(winlist, f, s)	' add image to list of images
  call image_unclip(f)			' display entire image
  image_open = s			' return name of image we created
End function

' ------------------------------------------------------------------------
' set form to match image width
Sub image_unclip_isvb_x(f as form)
  dim fid as string
  fid = "image_unclip_isvb_x"

  dim xe as integer
  xe = f.theimage.bbox_right 

  dim s as string
  s = profile_get(PROFILE_INI_MASTER, PROFILE_APP_GLOBALS, PROFILE_KEY_XBORDER)
  f.Width = CDbl(s) + xe * screen.TwipsPerPixelX

  f.thewname.width = xe			' full width for image name box

  f.theimage.width = xe

end sub

' ------------------------------------------------------------------------
' set form to match image height
Sub image_unclip_isvb_y(f as form)
  dim fid as string
  fid = "image_unclip_isvb_y"

  dim ye as integer
  ye = f.theimage.bbox_bottom

  dim s as string
  s = profile_get(PROFILE_INI_MASTER, PROFILE_APP_GLOBALS, PROFILE_KEY_YBORDER)
  f.Height = CDbl(s) + (ye + f.thewname.top) * screen.TwipsPerPixelY

  f.theimage.height = ye

end sub

' ------------------------------------------------------------------------
' set form to match image
Sub image_unclip_isvb(f as form)
  f.theimage.inch = 96			' so stretchdib in mf maps 1-to-1
  call image_unclip_isvb_x(f)
  call image_unclip_isvb_y(f)
end sub

' ------------------------------------------------------------------------
Sub image_unclip(f as form)
  if f.WindowState <> 1 Then
    if typeof f is imageform then
      if typeof f.theimage is isvb then
        call image_unclip_isvb(f)
      end if
    end if
  end if
end sub

' ------------------------------------------------------------------------
' clip image inside window limits

Sub image_clip_isvb(f as form)
  ' this function may get called during unclipping!!
end sub

' ------------------------------------------------------------------------
Sub image_clip(f as form)
  if f.WindowState <> 1 Then
    if typeof f is imageform then
      if typeof f.theimage is isvb then
        call image_clip_isvb(f)
      end if
    end if
  end if
end sub

' ------------------------------------------------------------------------
' read wmf file into image

Sub image_read_wmf(f as form, filename as string)
  Dim Q As String
  Q = Chr$(34)
  dim s as string

    f.theimage.Visible = false
    f.theimage.ErrorCode = 0
    f.theimage.FileOffset = 0
    f.theimage.ImportFileType = "WMF"
    f.theimage.ImportFileSpec = filename

    if f.theimage.ErrorCode <> 0 Then
      s = "Sorry, could not read image window " + Q + f.thewname.text + Q
      s = s + " from file " + filename
      s = s + " (" + f.theimage.ErrorString + ")."
      call error_notify(s)
      exit function
    else
      call image_unclip(f)		' adjust window to fit image
      f.theimage.Visible = true
      f.Show
    end if

end sub

' ------------------------------------------------------------------------
' write image into wmf file

Sub image_write_wmf(f as form, filename as string)
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
      call error_notify(s)
      exit function
    end if

end sub
