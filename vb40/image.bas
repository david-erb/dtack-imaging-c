Option Explicit

Global Const LEAD_FILETYPE_ANY = "*.*"

' ------------------------------------------------------------------------
' 
function image_control(source as control) as integer
  image_control = 0

  #if isvb then
    if typeof source is isvb then
      image_control = 1
    end if
  #end if

  #if lead then
    if typeof source is lead then
      image_control = 1
    end if
  #end if
end function

' ------------------------------------------------------------------------
' 
function image_oneliner(f as imageform)
  #if isvb then
    image_oneliner = isvb_oneliner(f)
  #end if
  #if lead then
    image_oneliner = lead_oneliner(f)
  #end if
end function

' ------------------------------------------------------------------------
' 
Sub image_saveas(f as imageform)
  #if isvb then
    call isvb_saveas(f)
  #end if
  #if lead then
    call lead_saveas(f)
  #end if
end sub

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
    call notify_error("Created image " + Q + s + Q + ", but now can't find it!")
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
  dim s as string

  #if isvb then
    s = isvb_open(f)			' returns name of image created
  #end if

  #if lead then
    s = lead_open(f, LEAD_FILETYPE_ANY)	' returns name of image created
  #end if

  if s <> ERROR_STRING then
    s = winlist_add(winlist, f, s)	' add image to list of images
    call image_unclip(f)		' display entire image
    f.show				' show the image on the display
  end if

  image_open = s			' return name of image we created
End function

' ------------------------------------------------------------------------
Sub image_unclip(f as form)
  if f.WindowState <> 1 Then
    #if isvb then
      call isvb_unclip(f)
    #end if
    #if lead then
      call lead_unclip(f)
    #end if
  end if
end sub

' ------------------------------------------------------------------------
Sub image_clip(f as form)
  if f.WindowState <> 1 Then
    #if isvb then
      call isvb_clip(f)
    #end if
    #if lead then
      call lead_clip(f)
    #end if
  end if
end sub

' ------------------------------------------------------------------------
' read wmf file into image

Sub image_read( _
  f as form, _
  filename as string, _
  filetype as string _
)
  dim rc as integer
  #if isvb then
    rc = isvb_read(f, filename, filetype)
  #end if
  #if lead then
    rc = lead_read(f, filename, filetype)
  #end if

  if rc = RC_GOOD then
    call image_unclip(f)
  end if

end sub

' ------------------------------------------------------------------------
' write image into wmf file

Sub image_write(f as form, filename as string, filetype as string)
  #if isvb then
    call isvb_write(f, filename, filetype)
  #end if
  #if lead then
    call lead_write(f, filename, filetype)
  #end if
end sub
