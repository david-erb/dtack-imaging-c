Attribute VB_Name = "INSTANCE"
Option Explicit

' ---------------------------------------------------------------------
' make exepath\exename.exe from exename
'
Function instance_executable(f As instanceform, pname As String) As String
  Dim s As String
  Dim executable As String
  s = profile_get(util_ini(), PROFILE_APP_GLOBALS, PROFILE_KEY_DLLPATH)
  If s <> "" Then
    executable = s + "\"
  Else
    executable = ""
  End If

  s = profile_get(util_ini(), pname, PROFILE_KEY_EXECUTABLE)
  If s <> "" Then
    executable = executable + s
  Else
    executable = executable + pname
  End If

  instance_executable = executable + ".exe"
End Function

' ---------------------------------------------------------------------
'
Sub instance_refresh( _
  f As instanceform _
)
  f.procwins.clear
  f.pnames.clear
  f.statuses.clear
  dim k as integer
  for k = 0 to (forms.count-1)
    if typeof forms(k) is outputform then
      call instance_add(f, forms(k))
    end if
  next k
  f.Show
End Sub

' ---------------------------------------------------------------------
'
Sub instance_add( _
  f As instanceform, _
  o as outputform _
)
  dim finished as string
  if o.procwin.caption <> "" then
    if util_iswindow(o.procwin.caption) = 0 then 
      finished = "*"
    else
      finished = ""
    end if
  end if
  f.procwins.additem o.procwin.caption + finished
  f.pnames.additem o.pname.caption
  f.statuses.additem o.status.text
End Sub

' ---------------------------------------------------------------------
' stop button clicked
'
sub instance_exit(f as instanceform)
  unload f
end sub
