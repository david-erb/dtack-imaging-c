Option Explicit

Global Const UTIL_FEEDBACK_STRING = "DT IMAGE OUTPUT"
Global Const UTIL_FEEDBACK_COUNT = 5
Global Const UTIL_FEEDBACK_WMF = 1
Global Const UTIL_FEEDBACK_WNAME = 2
Global Const UTIL_FEEDBACK_XE = 3
Global Const UTIL_FEEDBACK_YE = 4

' ------------------------------------------------------------------------
function util_ini() as string
  util_ini = profileform.inifile.text
End function

' ------------------------------------------------------------------------
function util_id() as string
  util_id = profile_get( _
    util_ini(), _
    PROFILE_APP_GLOBALS, "id")
end function

' ------------------------------------------------------------------------
Sub exit_all ()
  End
End Sub

' ------------------------------------------------------------------------
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
  text_y = global_ym * (y-1) + global_yb
End Function

' ------------------------------------------------------------------------
function util_iswindow( _
  window as string _
) as integer

#if win16 then
  dim rc as integer
  dim hwnd as integer
  hwnd = util_s2i("&H" + window)
#end if
#if win32 then
  dim rc as long
  dim hwnd as long
  hwnd = util_s2l("&H" + window)
#end if
  rc = IsWindow(hwnd)
  if rc = 0 then
    util_iswindow = 0
  else
    util_iswindow = 1
  end if
end function

' ------------------------------------------------------------------------
sub util_sendmessage( _
  window as string, _
  message as long, _
  wparam as long, _
  lparam as long _
)
#if win16 then
  dim hwnd as integer
  hwnd = util_s2i("&H" + window)
#end if
#if win32 then
  dim hwnd as long
  hwnd = util_s2l("&H" + window)
#end if
  dim rc as long
  rc = SendMessage(hwnd, message, wparam, lparam)
end sub

' ------------------------------------------------------------------------
sub util_postmessage( _
  window as string, _
  message as long, _
  wparam as long, _
  lparam as long _
)
#if win16 then
  dim hwnd as integer
  hwnd = util_s2i("&H" + window)
#end if
#if win32 then
  dim hwnd as long
  hwnd = util_s2l("&H" + window)
#end if
  dim rc as long
  rc = PostMessage(hwnd, message, wparam, lparam)
end sub

' ------------------------------------------------------------------------
sub util_error_log( _
  number as integer, _
  source as string, _
  description as string _
)
  on error goto error_handler

  dim count as integer
  count = cint(errorform.errcount.caption)
  count = count + 1

  dim filename as string
  filename = profile_get(util_ini(), _
    "globals", "logfile")
  open filename for append access write as #2
  print #2, "number " + trim$(str$(count)) + " " + _
            "date: " + format(date, "General Date") + " " + _
            "time: " + format(time, "Long Time")
  if number <> 0 then
    print #2, "  error number:" + str$(number)
    print #2, "  source: " + source
    print #2, "  description: " + description
  end if
  close #2

  if count >= 4 then
    errorform.errcount.caption = "0"
    call notify_ari("Error count" + str$(count) + " reached.", _
      util_id() + " Error Report")
    count = 0
  end if
  errorform.errcount.caption = trim$(str$(count))

  exit sub
error_handler:
  dim lnumber as integer
  dim lsource as string
  dim ldescription as string
  lnumber = err.number
  lsource = err.source
  ldescription = err.description
  call notify_ari( _
    "error number:" + str$( lnumber) + vbCrLf + _
    "source: " + lsource + vbCrLf + _
    "description: " + ldescription + vbCrLf + _
    "could not write error log file " + filename, _
    util_id() + " Error Report")

End sub

' ------------------------------------------------------------------------
Function util_s2l(s as string) as long
  on error goto error_handler
  util_s2l = clng(s)
  exit function
error_handler:
  call util_error_log(err.number, err.source, err.description)
  util_s2l = 0
End Function

' ------------------------------------------------------------------------
Function util_s2i(s as string) as integer
  on error goto error_handler
  util_s2i = cint(s)
  exit function
error_handler:
  call util_error_log(err.number, err.source, err.description)
  util_s2i = 0
End Function

' ------------------------------------------------------------------------
Function util_04x (ByVal number As long) As String
  util_04x = Right$("0000" + Trim$(Hex$(number)), 4)
End Function

' ------------------------------------------------------------------------
Function util_08x (ByVal number As long) As String
  util_08x = Right$("00000000" + Trim$(Hex$(number)), 8)
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
  l = SendMessage(b.hwnd, EM_SETREADONLY, 1, clng(0))
end sub

' ---------------------------------------------------------------------
' launch the application
'
function util_fork( _
  f As form, _
  pname as string, _
  feedbackonly As Integer, _
  timeout as long, _
  after as string, _
  flags as string, _
  argline As String _
) as integer

  screen.mousepointer = 11		' change to hourglass

  dim o as outputform			' create a new output window
  set o = new outputform

  call util_readonly(o.status)
  call util_readonly(o.output)

  dim title as string
  title = _
    util_id() + " " + _
    pname + " Starting"
  o.Caption = title
  o.pname.caption = pname
  o.parent.caption = util_08x(clng(f.keyinput.hwnd))
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
  subcommand = profile_get(util_ini(), pname, PROFILE_KEY_SUBCOMMAND)
  f.subcommand.caption = subcommand

  Dim a As String
  a = util_08x(clng(o.feedback.hwnd))
  a = a + " " + util_08x(clng(o.output.hwnd))
  a = a + " " + util_08x(clng(o.status.hwnd))
  If feedbackonly = 1 Then
    a = a + " feedbackonly "
  End If
  If subcommand <> "" Then
    a = a + " " + subcommand
  End If

  call output_append(o, pname + ": (" + a + ") " + argline)

  a = a + " " + argline			' attach form-supplied arguments

  f.argline.text = a			' put long string in special text box
  dim l as integer
  l = len(a)
  a = util_08x(clng(f.argline.hwnd)) 	' pass text box handle to program
  a = a + "*" + trim$(str$(l))		' also pass length of arg line

  dim rc as long
  rc = WinExec(executable + " " + a, _
    SW_SHOW)

  if rc < 32 then
    dim s as string
    s = "Sorry, could not load "
    s = s + executable
    s = s + " (error " + trim$(str$(rc)) + ")."
    call notify_error(s)
    screen.mousepointer = 1		' change to arrow
    rc = RC_BAD
  else
    title = util_id() + " " + pname
    if feedbackonly = 1 then
      title = title + " Form Description"
    else
      title = title + " Output"
    end if
    o.Caption = title
    o.refresh
    if timeout <> 0 then
      f.thetimer.interval = timeout
      f.thetimer.enabled = true
    end if
    rc = RC_GOOD
  end if
  util_fork = rc
End function
