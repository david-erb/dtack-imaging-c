Option Explicit

' ---------------------------------------------------------------------
' return window spec for window name
'
sub winlist_request(f As winlistform, cmd As String, cancel as integer)
  f.thelist.additem cmd
  cancel = -1
end sub

' ---------------------------------------------------------------------
function winlist_arg(f as paramform, arg as string) as string
  winlist_arg = arg
  exit function

  Dim rc As Integer
  Dim r As String
  Dim k As Integer
  Dim s As String
  Dim g As String

  If Left$(arg, 1) = ":" Then		' name starts with colon?
    g = "c:\tmp"
    if g <> "" then
      g = g + "\"
    end if

    g = g + "dt"
    g = g + util_04x(forms(k).theimage.hMF)
    g = g + ".wmf"

    r = "wmf,," + g

    s = Mid$(arg, 2)			' skip the colon 
    k = winlist_search(winlistform, s)	' search for the window
    If k < 0 Then			' window does not exist yet
      r = "wxx,wmf,0+0+0," + s
    Else				' window already exists
    end if
  End If

  If Left$(arg, 1) = "@" Then
    s = Mid$(arg, 2)
    k = winlist_search(winlistform, s)
    If k < 0 Then
      r = "wxx,dib,0"
    Else
      forms(k).theimage.ErrorCode = 0
      forms(k).theimage.FileOffset = 0
      forms(k).theimage.ExportFileType = "WMF"
      forms(k).theimage.ExportFileSpec = ""
      forms(k).theimage.ExportFileHandle = 1
      
      Dim dibhandle As Long
      dibhandle = forms(k).theimage.ExportFileHandle

      If forms(k).theimage.ErrorCode <> 0 Then
        s = "ExportFileHandle("
        s = s + Trim$(Str$(dibhandle)) + "): "
        s = s + forms(k).theimage.ErrorString
        f.message.Caption = s
        winlist_arg = ERROR_STRING
        Exit Function
      End If
      
      r = "wxx,dib," + Trim$(Str$(dibhandle))
    End If
  End If
  
  ' none of the above?
  If r = "" Then
    r = arg
  End If

  winlist_arg = r
End Function

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
      If k < 0 Then
        mfhandle = 0
      Else
        mfhandle = forms(k).theimage.hMF
      End If
      Dim s As String
      s = Right(Space$(6) + Str$(mfhandle), 6)
      s = s + " " + imgname
      s = s + " topleft=" + Trim$(Str$(forms(k).theimage.bBox_left))
      s = s + "x" + Trim$(Str$(forms(k).theimage.bBox_top))
      s = s + ", rightbottom=" + Trim$(Str$(forms(k).theimage.bBox_right))
      s = s + "x" + Trim$(Str$(forms(k).theimage.bBox_bottom))
      s = s + ", size=" + Trim$(Str$(forms(k).theimage.Width))
      s = s + "x" + Trim$(Str$(forms(k).theimage.Width))
      s = s + ", inch=" + Trim$(Str$(forms(k).theimage.inch))

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

function winlist_add(f as winlistform, iform as form, wname as string) as string
  Dim iname As String
  if wname = "" then
    iname = "image"
  else
    iname = wname
  end if

  dim s as string
  s = iname                             ' assume we can use requested name
  iform.tag = ""						' so we don't find ourself
  if winlist_search(f, s) <> -1 then    ' see if we can use requested name
    ' find a new unique name
    Dim i As Integer
    For i = 2 To 10000
      s = iname + "#" + trim$(Str$(i))
      if winlist_search(f, s) = -1 then
        Exit For
      End If
      if i = 10000 then
        call error_notify("Sorry, you have too many images.")
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
