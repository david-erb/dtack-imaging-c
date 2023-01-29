Option Explicit

Const PARAM_STATUS_WAITING = "Waiting for input"
Const PARAM_STATUS_PREPARING = "Preparing form..."
Const PARAM_STATUS_LAUNCHING = "Launching"

Global Const PARAM_LAUNCH_TIMEOUT = 5000

Global Const PARAM_FEEDBACK_COUNT = 7
Global Const PARAM_FEEDBACK_ABBREV = 0
Global Const PARAM_FEEDBACK_CAPTION = 1
Global Const PARAM_FEEDBACK_CONTENT = 2
Global Const PARAM_FEEDBACK_X = 3
Global Const PARAM_FEEDBACK_Y = 4
Global Const PARAM_FEEDBACK_LABELWIDTH = 5
Global Const PARAM_FEEDBACK_INPUTWIDTH = 6

' ---------------------------------------------------------------------
' make a single argument line from the form's field values
'
Function param_argline (f As paramform) As String
  Dim Q As String
  Q = Chr$(34)

  Dim argline As String
  argline = ""

  Dim field_count As Integer
  field_count = CInt(f.field_count.Caption)

  Dim pname As String
  pname = f.pname.Caption

  Dim i As Integer
  Dim s As String

  ' ------------------------------------------------------
  ' get the parameters
  For i = 0 To (field_count - 1)
    s = Left$(f.field_abbrev(i).Caption, 3)
    If s <> "arg" Then
      s = f.field_input(i).Text
      If s = "" Then
        s = Mid$(f.field_abbrev(i).Caption, 2)
        s = profile_param(PROFILE_INI_MASTER, pname, s)
      End If
      If s <> "" Then
        s = winlist_arg(f, s)
        If s = ERROR_STRING Then
          param_argline = s
          Exit Function
        End If
        argline = argline + f.field_abbrev(i).Caption + " " + Q + s + Q + " "
      End If
    End If
  Next i

  ' ------------------------------------------------------
  ' get the global params
  argline = argline + param_argline_param(pname, "dbgto")
  argline = argline + param_argline_param(pname, "errto")
  argline = argline + param_argline_param(pname, "sayto")

  ' ------------------------------------------------------
  ' get the arguments
  For i = 0 To (field_count - 1)
    s = Left$(f.field_abbrev(i).Caption, 3)
    If s = "arg" Then
      s = f.field_input(i).Text
      If s = "" Then
        s = f.field_abbrev(i).Caption
        s = profile_param(PROFILE_INI_MASTER, pname, s)
      End If
      If s <> "" Then
        s = winlist_arg(f, s)
        If s = ERROR_STRING Then
          param_argline = s
          Exit Function
        End If
      End If
      argline = argline + Q + s + Q + " "
    End If
  Next
  ' ------------------------------------------------------
  param_argline = argline

End Function

' ---------------------------------------------------------------------
' return argline-like form of param if it exists
'
Function param_argline_param (pname As String, key As String) As String
  Dim s As String
  s = profile_param(PROFILE_INI_MASTER, pname, key)
  If s <> "" Then
    Dim Q As String
    Q = Chr$(34)
    param_argline_param = "-" + key + " " + Q + s + Q + " "
  Else
    param_argline_param = ""
  End If
End Function

' ---------------------------------------------------------------------
' called by form_unload
'
Sub param_form_unload (f As paramform)
End Sub

' ---------------------------------------------------------------------
' handle feedback of field locations
'
Sub param_feedback(f As paramform, b as listbox)

  if b.listcount = 0 then
    error_notify "param_feedback: listbox empty!"
    exit sub
  end if

  if b.listcount <= 2 then		' no feedback body?
    f.message.Caption = b.List(0)
    f.start.Caption = ""
    f.status.Caption = PARAM_STATUS_WAITING
    exit sub
  end if

  Dim i As Integer
  Dim s As String
  Dim n As Integer
  Dim maxy As Single
  Dim y As Single
  Dim X As Single
  Dim w As Single
  Dim inputwidth As Single
  Dim labelwidth As Single

  Dim field_count As Integer
  field_count = CInt(f.field_count.Caption)

  Dim pname As String
  pname = f.pname.Caption

  f.message.Caption = b.List(0)
  ' ignore the initial instruction to user in message field
  f.message.Caption = ""
  f.start.Caption = b.List(1)

  n = 0
  maxy = 0
  For i = 2 To (b.ListCount - 1) Step PARAM_FEEDBACK_COUNT
    If n > field_count Then
      Load f.field_label(n)
      Load f.field_input(n)
      Load f.field_abbrev(n)
    End If
    f.field_label(n).TabIndex = n
    f.field_label(n).Caption = b.List(i + PARAM_FEEDBACK_CAPTION)

    y = CSng(b.List(i + PARAM_FEEDBACK_Y))
    f.field_label(n).Top = text_y(y)
    f.field_input(n).Top = text_y(y)
    X = CSng(b.List(i + PARAM_FEEDBACK_X))
    labelwidth = CSng(b.List(i + PARAM_FEEDBACK_LABELWIDTH))
    w = f.TextWidth(f.field_label(n) + "  ")
    f.field_label(n).Left = text_x(X + labelwidth) - w
    f.field_label(n).Width = w
    f.field_input(n).Left = text_x(X + labelwidth)
    inputwidth = CSng(b.List(i + PARAM_FEEDBACK_INPUTWIDTH))
    f.field_input(n).Width = text_w(inputwidth)
    f.field_label(n).Visible = 1
    f.field_input(n).Visible = 1
    f.field_abbrev(n).Caption = b.List(i + PARAM_FEEDBACK_ABBREV)

    ' ---------------------------------------------------
    ' if default for param is empty, see if profile has one
    '
    s = b.List(i + PARAM_FEEDBACK_CONTENT)
    If s = "" Then
      s = Mid$(f.field_abbrev(n).Caption, 2)
      s = profile_param(PROFILE_INI_MASTER, pname, s)
    End If
    f.field_input(n).Text = s
    ' ---------------------------------------------------
 
    n = n + 1
    If y > maxy Then
      maxy = y
    End If
  Next
  f.field_count.Caption = Str$(n)
  s = "old=" + trim$(str$(f.height))
  f.height = text_y(maxy + 3.3)
  f.status.Caption = PARAM_STATUS_WAITING
End Sub

' ---------------------------------------------------------------------
' called when timer goes off
'
sub param_tick(f As paramform)
  f.thetimer.enabled = false		' stop the timer
  error_notify f.executable.caption + " died!"
  screen.mousepointer = 1
  if f.status.Caption = PARAM_STATUS_PREPARING then
    unload f
  end if
end sub

' ---------------------------------------------------------------------
' called when keyboard message received
' a feedback list is assumed in the global outputform
'
Sub param_key(f As paramform, key as integer, shift as integer)
  f.keyinput.text = "key=0x" + hex$(key) + " shift=0x" + hex$(shift)
  if key = CONST_KEY_FEEDBACK then
    Call param_feedback(f, outputform.feedback)
    f.thetimer.enabled = false		' stop the timer
    screen.mousepointer = 1
  end if
End Sub

' ---------------------------------------------------------------------
' called by master
'
Function param_init (f As paramform) As Integer
  Dim pname As String
  pname = f.pname.Caption
  f.Caption = "Dtack " + program_string(pname) + " Parameters"

  f.field_count.Caption = "0"
  
  f.status.Caption = PARAM_STATUS_PREPARING
  f.message.caption = ""
  f.show
  f.refresh

  Dim argline As String
  argline = argline + " -D all"
  argline = argline + " -errto c:\tmp\prep.out"
  argline = argline + " -dbgto c:\tmp\prep.out"
  argline = argline + " -sayto c:\tmp\prep.out"

  param_init = util_fork(f, pname, 1, clng(0), "", "", argline)
End Function

' ---------------------------------------------------------------------
' called when Run button clicked
'
Sub param_run (f As paramform)
  f.message.Caption = ""
  f.start.Caption = ""
  f.status.Caption = PARAM_STATUS_LAUNCHING
  f.Show

  Dim argline As String
  argline = param_argline(f)		' build the argument line
  If argline = ERROR_STRING Then
    Exit Sub
  End If

  dim pname as string
  pname = f.pname
  dim rc as integer
  rc = util_fork(f, pname, 0, clng(0), "", "", argline)

End Sub
