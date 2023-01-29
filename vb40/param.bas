Attribute VB_Name = "PARAM"
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
' key press in input field
' enter key acts like run menu
'
Sub param_keypress( _
  f As paramform, _
  Index As Integer, _
  KeyAscii As Integer _
)

  If Chr$(KeyAscii) = vbCr Then		' enter key?
    Call param_run(f)                   ' run the program

    If Index = -1 Then                  ' key came from the form?
      KeyAscii = 0                      ' don't let the control have it
    End If
  End If

End Sub

' ---------------------------------------------------------------------
' make a single argument line from the form's field values
'
Function param_argline(f As paramform) As String
  Dim Q As String
  Q = Chr$(34)

  Dim argline As String
  argline = ""

  Dim field_count As Integer
  field_count = util_s2i(f.field_count.Caption)

  Dim pname As String
  pname = f.pname.Caption

  Dim i As Integer
  Dim s As String

  ' ------------------------------------------------------
  ' get the parameters
  For i = 0 To (field_count - 1)
    s = Left$(f.field_abbrev(i).Caption, 3)
    If s <> "arg" Then
      Dim c As String
      c = f.field_abbrev(i).Caption
      s = f.field_input(i).Text
      'If s = "" Then
        's = Mid$(c, 2)
        's = profile_param(util_ini(), pname, s)
      'End If
      If s <> "" Or _
        c = "-D" Or _
        c = "-mem" Or _
        c = "-win" Or _
        c = "-yield" _
      Then
        argline = argline + c + " " + Q + s + Q + " "
      End If
    End If
  Next i

  For i = 0 To (field_count - 1)
    s = Left$(f.field_abbrev(i).Caption, 3)
    If s = "arg" Then
      s = f.field_input(i).Text
      'If s = "" Then
        's = f.field_abbrev(i).Caption
        's = profile_param(util_ini(), pname, s)
      'End If
      argline = argline + Q + s + Q + " "
    End If
  Next i

  param_argline = argline

End Function

' ---------------------------------------------------------------------
' called by form_unload
'
Sub param_form_unload(f As paramform)
End Sub

' ---------------------------------------------------------------------
' save current values of all fields
'
Sub param_save( _
  f As paramform, _
  suffix as String _
)
  Dim i As Integer
  Dim n As Integer
  n = util_s2i(f.field_count.Caption)

  Dim pname As String
  pname = f.pname.Caption

  for i = 0 to (n-1) step 1
    call profile_put( _
      util_ini(), _
      pname, _
      Mid$(f.field_abbrev(i).Caption, 2) + suffix, _
      f.field_input(i).Text)
  next i
End Sub

' ---------------------------------------------------------------------
' handle feedback of field locations
'
Sub param_feedback(f As paramform, b As ListBox)

  If b.ListCount = 0 Then
    notify_error "param_feedback: listbox empty!"
    Exit Sub
  End If

  If b.ListCount <= 2 Then              ' no feedback body?
    f.message.Caption = b.List(0)
    f.start.Caption = ""
    f.status.Caption = PARAM_STATUS_WAITING
    Exit Sub
  End If

  Dim i As Integer
  Dim s As String
  Dim n As Integer
  Dim maxy As Single
  Dim Y As Single
  Dim X As Single
  Dim w As Single
  Dim inputwidth As Single
  Dim labelwidth As Single

  Dim field_count As Integer
  field_count = util_s2i(f.field_count.Caption)

  Dim pname As String
  pname = f.pname.Caption

  f.message.Caption = b.List(0)
  ' ignore the initial instruction to user in message field
  f.message.Caption = ""
  f.start.Caption = b.List(1)           ' field which should have first focus

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

    Y = CSng(b.List(i + PARAM_FEEDBACK_Y))
    f.field_label(n).Top = text_y(Y)
    f.field_input(n).Top = text_y(Y)
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

    ' special case of -D: ignore what is in feedback
    If f.field_abbrev(n).Caption = "-D" Then
      s = ""
    Else
      s = b.List(i + PARAM_FEEDBACK_CONTENT)
    End If

    ' ---------------------------------------------------
    ' get previous run value
    '
    s = Mid$(f.field_abbrev(n).Caption, 2)
    s = profile_gimme_lastrun(util_ini(), pname, s)

    ' ---------------------------------------------------
    ' if default for param is empty, see if profile has one
    '
    If s = "" Then
      s = "dt_" + Mid$(f.field_abbrev(n).Caption, 2)
      s = profile_gimme_global(util_ini(), s)
    End If
    ' ---------------------------------------------------

    f.field_input(n).Text = s
 
    n = n + 1
    If Y > maxy Then
      maxy = Y
    End If
  Next
  f.field_count.Caption = Str$(n)
  s = "old=" + Trim$(Str$(f.Height))
  f.Height = text_y(maxy + 3.3)
  f.status.Caption = PARAM_STATUS_WAITING

  call param_save(f, "_program")
End Sub

' ---------------------------------------------------------------------
' called when timer goes off
'
Sub param_tick(f As paramform)
  f.thetimer.Enabled = False            ' stop the timer
  Call notify_error( _
    f.executable.Caption + " died!")
  Screen.MousePointer = 1
  If f.status.Caption = _
     PARAM_STATUS_PREPARING Then
    Unload f
  End If
End Sub

' ---------------------------------------------------------------------
'
Sub param_restart( _
  f As paramform _
)
  dim auto_restart as string
  auto_restart = profile_get( _
    util_ini(), _
    f.pname.caption, "auto_restart")

  dim n as long
  n = util_s2l("0" + auto_restart)
  if n > 0 then				' auto restart is enabled?
    sleep(2000)				' sleep a bit
    restartform.parent.caption = _
      util_08x(f.keyinput.hwnd)	
    restartform.thetimer.interval = 1000
    restartform.thetimer.enabled =  true
    restartform.remaining.caption = "10"
    restartform.show			' commence countdown
    restartform.zorder 0
  End If

End Sub

' ---------------------------------------------------------------------
' called when keyboard message received
' a feedback list is assumed in the global outputform
'
Sub param_key( _
  f As paramform, _
  key As Integer, _
  shift As Integer)

  f.keyinput.Text = util_04x(key)
  If key = CONST_KEY_FEEDBACK Then
    Call param_feedback(f, _
      outputform.feedback)
    f.thetimer.Enabled = False          ' stop the timer
    Screen.MousePointer = 1
  elseif key = CONST_KEY_FINISH Then    ' finish before any feedback?
    Call param_feedback(f, _
      outputform.feedback)
    f.thetimer.Enabled = False          ' stop the timer
    Screen.MousePointer = 1
    call param_restart(f)		' restart if enabled
  elseif key = CONST_KEY_RUN Then
    Call param_run(f)                   ' run the program
  End If

End Sub

' ---------------------------------------------------------------------
' called by master
'
Function param_init(f As paramform) As Integer
  Dim pname As String
  pname = f.pname.Caption

  Dim program_id As String
  program_id = profile_get( _
    util_ini(), pname, "id")

  f.Caption = _
    util_id() + " " + _
    program_id + " Parameters"

  f.field_count.Caption = "0"
  
  f.status.Caption = PARAM_STATUS_PREPARING
  f.message.Caption = ""
  f.Show
  f.Refresh

  Dim argline As String
  'argline = argline + " -D all+assert"
  'argline = argline + " -errto c:\tmp\prep.out"
  'argline = argline + " -dbgto c:\tmp\prep.out"
  'argline = argline + " -sayto c:\tmp\prep.out"

  param_init = util_fork( _
    f, _
    pname, _
    1, _
    0, _
    "", _
    OUTPUT_FLAGS_CLOSE_WHEN_DONE, _
    argline)

End Function

' ---------------------------------------------------------------------
' delete inactive output forms
'
Sub param_delete_inactive( _
  f As paramform _
)
  dim k as integer
again:
  for k = 0 to (forms.count-1)
    if typeof forms(k) is outputform then
      unload forms(k)
      k = forms.count
    end if
  next k
  if k = (forms.count+1) then goto again
End Sub

' ---------------------------------------------------------------------
' called when Run button clicked
'
Sub param_run( _
  f As paramform _
)
  call param_save(f, "_lastrun")	' save current params

  call param_delete_inactive(f)		' delete inactive output forms

  f.message.Caption = ""
  f.start.Caption = ""
  f.status.Caption = PARAM_STATUS_LAUNCHING
  f.Show

  Dim argline As String
  argline = param_argline(f)            ' build the argument line
  If argline = ERROR_STRING Then
    Exit Sub
  End If

  Dim pname As String
  pname = f.pname
  Dim rc As Integer

    dim flags as string
    flags = profile_get(util_ini(), pname, "output_disposition")
    dim after as string
    after = profile_get(util_ini(), pname, "output_file")

  rc = util_fork(f, pname, 0, clng(0), _
    after, flags, argline)

End Sub

' ---------------------------------------------------------------------
Sub param_drop( _
  f As paramform, _
  Index As Integer, _
  source As Control, _
  X As Single, _
  Y As Single _
)

  If image_control(source) = 1 Then
    f.field_input(Index).Text = ":" + source.Tag
  End If

End Sub

' ---------------------------------------------------------------------
Sub param_die( _
  f As paramform _
)
  dim x as integer
  x = 1 / 0
  call notify_error("Successful divide by zero!")

End Sub
