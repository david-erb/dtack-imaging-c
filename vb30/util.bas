Option Explicit

Global Const UTIL_FEEDBACK_STRING = "DT IMAGE OUTPUT"
Global Const UTIL_FEEDBACK_COUNT = 5
Global Const UTIL_FEEDBACK_WMF = 1
Global Const UTIL_FEEDBACK_WNAME = 2
Global Const UTIL_FEEDBACK_XE = 3
Global Const UTIL_FEEDBACK_YE = 4

' ------------------------------------------------------------------------
Sub exit_all ()
  End
End Sub

' ------------------------------------------------------------------------
Sub main ()
  'Call dll_reload_all

  'Call image_create
  'Dim k As Integer
  'k = dtwinlst_search("image  0")
  'forms(k).theimage.ImportFileSpec = "c:\sw\imgstrm0\images\gargoyle.bmp"
  'Call program_fork("gen")
End Sub

Function program_string (program As String) As String
  program_string = "<<" + program + ">>"
End Function

Function text_h (h As Single) As Single
  text_h = global_ym * h
End Function

Function text_w (w As Single) As Single
  text_w = global_xm * w
End Function

Function text_x (x As Single) As Single
  text_x = global_xm * x + global_xb
End Function

Function text_y (y As Single) As Single
  text_y = global_ym * y + global_yb
End Function

' ------------------------------------------------------------------------
Function util_04x (ByVal number As Integer) As String
  util_04x = Right$("0000" + Trim$(Hex$(number)), 4)
End Function

' ------------------------------------------------------------------------
Function util_08x (ByVal number As long) As String
  util_08x = Right$("00000000" + Trim$(Hex$(number)), 8)
End Function

' ---------------------------------------------------------------------
' allocate global memory and copy string to it
'
Function util_string_global (s As String) As Integer
  dim l as integer
  l = len(s)

  Dim handle As Integer
  handle = GlobalAlloc(GMEM_MOVEABLE or GMEM_SHARE, l+1)

  Dim p As Long
  p = GlobalLock(handle)
  
  Dim q As Long
  q = lstrcpy(p, s)
  l = lstrlen(q)

  Dim rc As Integer
  rc = GlobalUnlock(handle)
  util_string_global = handle
End Function

' ---------------------------------------------------------------------
'
Function util_hiword (p As Long) As Integer
  util_hiword = (p / &H100 / &H100) And &HFFFF
End Function

' ---------------------------------------------------------------------
'
Function util_loword (p As Long) As Integer
  util_loword = Int(p / &H10000) * &H10000 - p
End Function

' ---------------------------------------------------------------------
'
sub util_readonly(b as textbox)
  dim l as long
  l = 0
  l = SendMessage(b.hwnd, EM_SETREADONLY, 1, l)
end sub

' ---------------------------------------------------------------------
' launch the application
'
function util_fork (f As form, pname as string, feedbackonly As Integer, timeout as long, after as string, flags as string, argline As String) as integer

  dim rc as integer
  screen.mousepointer = 11		' change to hourglass

  dim o as outputform			' create a new output window
  set o = new outputform

  call util_readonly(o.status)
  call util_readonly(o.output)

  dim title as string
  title = "Dtack " + program_string(pname) + " Starting"
  o.Caption = title
  o.parent.caption = util_04x(f.keyinput.hwnd)
  o.after.caption = after
  o.flags.caption = flags

  call util_readonly(o.status)

  If feedbackonly <> 1 Then		' leave form creation window invisible
    o.show
  end if

  dim executable as string
  executable = instance_executable(instanceform, pname)
  f.executable.caption = executable

  dim subcommand as string
  subcommand = profile_get(PROFILE_INI_MASTER, pname, PROFILE_KEY_SUBCOMMAND)

  Dim a As String
  a = util_04x(o.feedback.hwnd)
  a = a + " " + util_04x(o.output.hwnd)
  a = a + " " + util_04x(o.status.hwnd)
  If feedbackonly = 1 Then
    a = a + " feedbackonly "
  End If
  If subcommand <> "" Then
    a = a + " " + subcommand
  End If

  call output_append(o, pname + ": (" + a + ") " + argline)

  a = a + " " + argline			' attach form-supplied arguments

  dim winex as string
  winex = profile_get(PROFILE_INI_MASTER, PROFILE_APP_GLOBALS, PROFILE_KEY_WINEX)
  if winex <> "" then
    winex = instance_executable(instanceform, "dtwinex") + " "
  end if

  f.argline.text = a			' put long string in special text box
  dim l as integer
  l = len(a)
  a = util_04x(f.argline.hwnd) 		' pass text box handle to program
  a = a + "*" + trim$(str$(l))		' also pass length of arg line

  dim hinstance as integer
  hinstance = WinExec(winex + executable + " " + a, SW_SHOW)

  if hinstance < 32 then
    dim s as string
    s = "Sorry, could not load "
    s = s + executable
    s = s + " (error " + trim$(str$(hinstance)) + ")."
    call error_notify(s)
    screen.mousepointer = 1		' change to arrow
    rc = RC_BAD
  else
    title = "Dtack " + program_string(pname) + ":"
    title = title + util_04x(hinstance)
    if feedbackonly = 1 then
      title = title + " Form Description"
    else
      title = title + " Output"
    end if
    o.Caption = title
    o.hinstance.caption = util_04x(hinstance)
    o.refresh
    if timeout <> 0 then
      f.thetimer.interval = timeout
      f.thetimer.enabled = true
    end if
    rc = RC_GOOD
  end if
  util_fork = rc
End Sub
