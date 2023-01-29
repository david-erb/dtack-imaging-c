Option Explicit

' ---------------------------------------------------------------------
' handle class button click
Sub master_button_click (f As masterform, index As Integer)
  Dim pclass As String
  pclass = f.classbutton(index).Caption
  Call master_programs_list(f, PROFILE_INI_MASTER, pclass)
  f.pclass.caption = pclass
End Sub

' ---------------------------------------------------------------------
' define class buttons for display
'
Sub master_buttons_define (f As masterform, fname As String)
  Call profile_classes(fname, f.worklist)
  Dim i As Integer
  For i = 0 To (f.worklist.ListCount - 1)
    If i > 0 Then
      Load f.classbutton(i)
      f.classbutton(i).Left = f.classbutton(i - 1).Left + f.classbutton(i - 1).Width * 1.2
      f.worklist.List(i) = f.worklist.List(i)
      f.classbutton(i).Visible = True
    End If
    f.classbutton(i).Caption = f.worklist.List(i)
  Next
  f.Show
End Sub

' ---------------------------------------------------------------------
' called by master form_load
'
Sub master_load (f As masterform)
  Call master_buttons_define(f, PROFILE_INI_MASTER)
  Call master_programs_list(f, PROFILE_INI_MASTER, PROFILE_DEFAULT_CLASS)
  load outputform

  'call master_class_run(f, "test1.vbb", "")
  call master_app_run(f, "dttxchg")
End Sub

' ---------------------------------------------------------------------
' load list with programs for class
'
Sub master_programs_list (f As masterform, fname As String, pclass As String)
  Call profile_programs(fname, pclass, f.worklist)
  masterform.programlist.Clear
  Dim i As Integer
  For i = 0 To (f.worklist.ListCount - 1)
    f.programlist.AddItem f.worklist.List(i)
  Next
End Sub

' ---------------------------------------------------------------------
' called by master when run button is clicked
'
Sub master_run (f as masterform, pname As String)
  if f.pclass.caption = "script" then
    dim flags as string
    flags = flags + OUTPUT_FLAGS_APPEND_WHEN_DONE
    flags = flags + OUTPUT_FLAGS_CLOSE_WHEN_DONE
    call master_class_run(f, pname, flags)
  else
    call master_app_run(f, pname)
  end if
End Sub


' ---------------------------------------------------------------------
'
Sub master_app_run (f as masterform, pname As String)
  Dim param As paramform
  Set param = New paramform
  param.pname = pname

  Dim rc As Integer
  rc = param_init(param)
  If rc <> RC_GOOD Then
    Unload param
  End If
End Sub


' ---------------------------------------------------------------------

Sub master_class_run (f as masterform, sname As String, flags as string)
  Dim script As scriptform
  Set script = New scriptform
  call util_readonly(script.keyinput)
  script.show
  script.flags.caption = flags
  call script_run(script, sname, "", "c:\tmp\test.out")
End Sub

