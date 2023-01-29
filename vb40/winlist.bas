Option Explicit

' ---------------------------------------------------------------------
' return window spec for window name
'
sub winlist_request(f As winlistform, cmd As String, cancel as integer)
  f.thelist.additem cmd
  cancel = -1
end sub

' ---------------------------------------------------------------------
' show the current window list
'
Sub winlist_bringup ()
  Load winlistform
  Call winlist_remake(winlistform)
  winlistform.Show
End Sub

' ---------------------------------------------------------------------
' remake the current window list
'
Sub winlist_remake (f As winlistform)
  Dim i As Integer
  Dim n As Integer
  n = 0
  f.thelist.Clear
  For i = 0 To (forms.Count - 1)
    If TypeOf forms(i) Is imageform Then
      Dim imgname As String
      imgname = forms(i).Tag
      Dim k As Integer
      k = winlist_search(f, imgname)
      Dim mfhandle As Integer
      mfhandle = 0
      Dim s As String
      s = Right(Space$(6) + Str$(mfhandle), 6)
      s = s + " " + imgname
      s = s + image_oneliner(forms(k))
      f.thelist.AddItem s
      Dim rc As Integer
      n = n + 1
    End If
  Next
  If n = 0 Then
    f.thelist.AddItem "no image windows"
  End If
End Sub

' ---------------------------------------------------------------------
' return -1 if not found
' return -2 if ambiguous
' return form index otherwise
'
Function winlist_search (f as winlistform, wname As String) As Integer
  Dim i As Integer
  Dim n As Integer
  n = 0
  For i = 0 To (forms.Count - 1)
    If TypeOf forms(i) Is imageform Then
      If forms(i).Tag = wname Then
        If n = 0 Then
          winlist_search = i
        Else
          winlist_search = -n
        End If
        n = n + 1
      End If
    End If
  Next
  If n = 0 Then
    winlist_search = -1
  End If
End Function


' ---------------------------------------------------------------------
' delete name from list if it exists

function winlist_add( _
  f as winlistform, _
  iform as form, _
  wname as string _
) as string
  Dim iname As String
  if wname = "" then
    iname = "image"
  else
    iname = wname
  end if

  dim s as string
  s = iname                             ' assume we can use requested name
  iform.tag = ""			' so we don't find ourself
  if winlist_search(f, s) <> -1 then    ' see if we can use requested name
    ' find a new unique name
    Dim i As Integer
    For i = 2 To 10000
      s = iname + "#" + trim$(Str$(i))
      if winlist_search(f, s) = -1 then
        Exit For
      End If
      if i = 10000 then
        call notify_error("Sorry, you have too many images.")
        winlist_add = ERROR_STRING 
        exit function
      end if
    Next
  end if

  iform.tag = s
  iform.theimage.tag = s
  iform.thewname.text = s
  winlist_add = s
end function
