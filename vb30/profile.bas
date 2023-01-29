Option Explicit

Global Const PROFILE_INI_MASTER = "dtack.ini"
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
    Dim s As String
    s = f(i).Text
    dim t as string
    t = f(i).tag
    Call profile_put(PROFILE_INI_MASTER, PROFILE_APP_GLOBALS, t, s)
  end if
  next
end sub

' ---------------------------------------------------------------------
' load form from global params
' called by form_load
'
Sub profile_globals_load (f as profileform)
  f.inipath.text = command
  dim i as integer
  for i = 0 to (f.controls.count-1)
  if typeof f(i) is textbox then
    if f(i).tag <> "" then
      dim t as string
      t = f(i).tag
      f(i).text = profile_get(PROFILE_INI_MASTER, PROFILE_APP_GLOBALS, t)
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
Function profile_param (fname As String, pname As String, key as string) As String
  Dim s As String
  s = profile_get(fname, pname, key)
  if s = "" then
    s = profile_get(fname, PROFILE_APP_GLOBALS, key)
  end if
  profile_param = s
End Function

' ---------------------------------------------------------------------
' get the ini filename
' 
function profile_fname (fname As String) as string
  dim s as string
  if profileform.inipath.text <> "" then
    s = profileform.inipath.text + "\" + fname
  else
    s = fname
  end if
  profile_fname = s
end function

' ---------------------------------------------------------------------
' lowest level write
' 
Sub profile_put (fname As String, app As String, key as string, v as string)
  dim actual as string
  actual = profile_fname(fname)			' get the actual ini filename
  Dim rc As integer
  rc = WritePrivateProfileString(app, key, v, actual)
  If rc = 0 Then
    call error_notify("Sorry, could not write <" + actual + ">.")
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
