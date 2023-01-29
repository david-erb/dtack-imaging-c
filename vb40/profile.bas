Option Explicit

Global Const PROFILE_INI_DLL = "dtdll.ini"

Global Const PROFILE_APP_GLOBALS = "globals"
Global Const PROFILE_APP_CLASSES = "classes"
Global Const PROFILE_APP_DLLS = "dlls"

Global Const PROFILE_KEY_CLASS = "class"
Global Const PROFILE_KEY_DLL = "dll"
Global Const PROFILE_KEY_WINEX = "winex"
Global Const PROFILE_KEY_PROGRAM = "program"
Global Const PROFILE_KEY_EXECUTABLE = "executable"
Global Const PROFILE_KEY_SUBCOMMAND = "subcommand"
Global Const PROFILE_KEY_DESCRIPTION = "description"
Global Const PROFILE_KEY_DLLNAME = "dll"
Global Const PROFILE_KEY_DLLPATH = "dllpath"
Global Const PROFILE_KEY_SCRIPTPATH = "scriptpath"
Global Const PROFILE_KEY_NEEDS = "needs"
Global Const PROFILE_KEY_XBORDER = "xborder"
Global Const PROFILE_KEY_YBORDER = "yborder"

Global Const PROFILE_DEFAULT_DESCRIPTION = "description?"
Global Const PROFILE_DEFAULT_CLASS = "tools"
Global Const PROFILE_DEFAULT_DLLNAME = "?.dll"

' ---------------------------------------------------------------------
' save global params from form
'
Sub profile_globals_save (f as profileform)
  dim i as integer
  for i = 0 to (f.controls.count-1)
  if typeof f(i) is textbox then
    if f(i).tag <> "" then
      Call profile_put( _
        util_ini(), _
        PROFILE_APP_GLOBALS, _
        f(i).tag, f(i).text)
    end if
    if left$(f(i).tag, 3) = "dt_" then
      dim rc as long
      rc = SetEnvironmentVariable( _
        ucase(f(i).tag), f(i).text)
    end if
  end if
  next
end sub

' ---------------------------------------------------------------------
' load form from global params
' called by form_load
'
Sub profile_globals_load (f as profileform)
  dim i as integer

  i = instr(command, " ")	
  if i = 0 then				' no spaces in command line?
    f.inifile.text = command
  else
    f.inifile.text = _
      trim$(left$(command, i))		' first arg from command line
  end if

  if f.inifile.text = "" then
    notify_error "Sorry, this program needs the name of an ini file."
    end 
  end if

  for i = 0 to (f.controls.count-1)
  if typeof f(i) is textbox then
    if f(i).tag <> "" then
      dim t as string
      t = f(i).tag
      f(i).text = profile_get( _
        util_ini(), _
        PROFILE_APP_GLOBALS, t)
    end if
    if left$(f(i).tag, 3) = "dt_" then
      dim rc as long
      rc = SetEnvironmentVariable( _
        ucase(f(i).tag), f(i).text)
    end if
  end if
  next
end sub

' ---------------------------------------------------------------------
' return list of classes
Sub profile_classes (fname As String, retlist As ListBox)
  Dim i As Integer
  Dim s As String
  Dim key As String
  retlist.Clear
  For i = 0 To 9999
    key = PROFILE_KEY_CLASS + Trim$(Str$(i))
    s = profile_get(fname, PROFILE_APP_CLASSES, key)
    If s = "" Then
      Exit For
    End If
    retlist.AddItem Trim$(s)
  Next
End Sub

' ---------------------------------------------------------------------
' return program's description
'
Function profile_program_description (fname As String, pname As String) As String
  Dim s As String
  s = profile_get(fname, pname, PROFILE_KEY_DESCRIPTION) 
  if s = "" then
    s = PROFILE_DEFAULT_DESCRIPTION
  end if
  profile_program_description = trim$(s)
End Function

' ---------------------------------------------------------------------
' return name of dll where program resides
'
Function profile_program_dllname (fname As String, pname As String) As String
  Dim s As String
  s = profile_get(fname, pname, PROFILE_KEY_DLLNAME)
  if s = "" then
    s = PROFILE_DEFAULT_DLLNAME
  end if
  profile_program_dllname = trim$(s)
End Function

' ---------------------------------------------------------------------
' return list of programs in given class
'
Sub profile_programs (fname As String, cname As String, retlist As ListBox)
  Dim i As Integer
  Dim s As String
  Dim key As String
  retlist.Clear
  For i = 0 To 9999
    key = PROFILE_KEY_PROGRAM + Trim$(Str$(i))
    s = profile_get(fname, cname, key)
    If s = "" Then
      Exit For
    End If
    retlist.AddItem Trim$(s)
  Next
End Sub

' ---------------------------------------------------------------------
' get value for global param
'
function profile_globals_get (f as profileform, gname as string) as string
  dim i as integer
  for i = 0 to (f.controls.count-1)
  if typeof f(i) is textbox then
    if f(i).tag = gname then
      profile_globals_get = f(i).text
      exit function
    end if
  end if
  next
  profile_globals_get = ""
end function

' ---------------------------------------------------------------------
' return profile value for given param
'
Function profile_gimme_lastrun( _
  fname As String, _
  pname As String, _
  key as string _
) As String
  Dim s As String
  s = profile_get(fname, pname, key + "_lastrun")
  profile_gimme_lastrun = s
End Function

' ---------------------------------------------------------------------
' return profile value for given param
'
Function profile_gimme_global( _
  fname As String, _
  key as string _
) As String
  Dim s As String
  s = profile_get(fname, _
   PROFILE_APP_GLOBALS, key)
  profile_gimme_global = s
End Function

' ---------------------------------------------------------------------
' get the ini filename
' 
function profile_fname (fname As String) as string
  profile_fname = fname
end function

' ---------------------------------------------------------------------
' lowest level write
' 
Sub profile_put ( _
  fname As String, _
  app As String, _
  key as string, _
  v as string _
)
  dim actual as string
  actual = profile_fname(fname)			' get the actual ini filename
  Dim rc As integer
  rc = WritePrivateProfileString(app, _
    key, v, actual)
  If rc = 0 Then
    call notify_error( _
      "Sorry, could not write <" + _
      actual + ">.")
  End If
end sub

' ---------------------------------------------------------------------
' lowest level read
' 
Function profile_get (fname As String, app As String, key as string) as string
  dim actual as string
  actual = profile_fname(fname)			' get the actual ini filename
  Dim n As Long
  Dim s As String * 256
  n = GetPrivateProfileString(app, key, "", s, Len(s), actual)
  profile_get = left$(s, instr(s, chr$(0))-1)
End function
