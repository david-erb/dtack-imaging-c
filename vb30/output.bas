Option Explicit

global const OUTPUT_FLAGS_APPEND_WHEN_DONE = " append"
global const OUTPUT_FLAGS_CLOSE_WHEN_DONE = " close"

' ---------------------------------------------------------------------
'
sub output_append(o as outputform, s as string)
  o.output.seltext = s + chr$(&H0d) + chr$(&H0a)
end sub

' ---------------------------------------------------------------------
' twirl the animate bitmap
'
sub output_twirl(o as outputform)
  dim n as integer
  n = cint(o.twirl_count.caption)
  o.animate.picture = o.twirl(n mod 4).picture
  n = n + 1
  o.twirl_count.caption = trim$(str$(n))
end sub

' ---------------------------------------------------------------------
' called when keyboard message received
'
Sub output_key(o As outputform, key as integer, shift as integer)
  if key = CONST_KEY_FEEDBACK then	' handle param feedback
    call output_feedback(o)	
  end if
  if key = CONST_KEY_LAUNCHED then	' handle application launched
    call output_launched(o)	
  end if
  if key = CONST_KEY_STATUS then	' handle application sending status
    call output_status(o)
  end if
  if key = CONST_KEY_FINISH then	' handle image returns
    call output_finish(o)
  end if
End Sub

' ---------------------------------------------------------------------
' copy the feedback to the global outputform and notify the param form
'
sub output_feedback_param(o as outputform)
  outputform.feedback.clear
  dim i as integer
  for i = 0 to (o.feedback.listcount-1)
    outputform.feedback.additem o.feedback.list(i)
  next i
  dim rc as long
  dim flags as long
  flags = 0
  dim parent as integer
  parent = cint("&H" + o.parent.caption)
  rc = SendMessage(parent, WM_KEYDOWN, CONST_KEY_FEEDBACK, flags)
  ' unload o
  call output_append(o, "---would unload---")
end sub

' ---------------------------------------------------------------------
' get image data and put it into requested file
'
sub output_feedback_import(o as outputform)
  dim wname as string
  wname = o.feedback.list(1)		' name of window
  dim k as integer
  k = winlist_search(winlistform, wname)
  if k = -1 then
    Dim Q As String
    Q = Chr$(34)
    error_notify("Sorry, there is no image window named " + Q + wname + Q + ".")
    exit sub
  end if

  dim filename as string
  filename = o.feedback.list(2)		' name of file

  call image_write_wmf(forms(k), filename)

  o.feedback.clear
  call output_post(o, CONST_MESSAGE_RESUME)
end sub

' ---------------------------------------------------------------------
' read given file into image
'
sub output_feedback_export(o as outputform)
  dim wname as string
  wname = o.feedback.list(1)		' name of window
  dim k as integer
  k = winlist_search(winlistform, wname)
  if k = -1 then			' image does not exist yet?
    k = image_create(winlistform, wname)
  end if

  dim xe as integer
  xe = cint(o.feedback.list(2))
  dim ye as integer
  ye = cint(o.feedback.list(3))

  dim filename as string
  filename = o.feedback.list(4)		' name of file

  call image_read_wmf(forms(k), filename)

  o.feedback.clear
  call output_post(o, CONST_MESSAGE_RESUME)
end sub

' ---------------------------------------------------------------------
' three cases:
' 1. we are being told of image import
' 2. we are being told of image export
' 3. we have been notified of application pre-launch feedback
'
sub output_feedback(o as outputform)
  if o.feedback.list(0) = "import" then
    call output_feedback_import(o)
  elseif o.feedback.list(0) = "export" then
    call output_feedback_export(o)
  else
    call output_feedback_param(o)
  end if
end sub

' ---------------------------------------------------------------------
' we have been notified of the application's successful launch
' app's task handle is expected as first and only feedback item
' parent param form is notified of the launch by putting feedback outputform
'
sub output_launched(o as outputform)
  if o.feedback.listcount > 0 then
    o.htask.caption = o.feedback.list(0)
  else
    o.htask.caption = "0000"
  end if
  o.menu_process_pause.enabled = true
  o.menu_process_resume.enabled = false
  o.menu_process_stop.enabled = true
  outputform.feedback.clear
  outputform.feedback.additem "Launched! (instance " + o.hinstance.caption + ")"
  dim rc as long
  dim flags as long
  flags = 0
  dim parent as integer
  parent = cint("&H" + o.parent.caption)
  rc = SendMessage(parent, WM_KEYDOWN, CONST_KEY_FEEDBACK, flags)

  o.feedback.clear
  call output_post(o, CONST_MESSAGE_RESUME)
end sub

' ---------------------------------------------------------------------
' we have been notified of application status change
'
sub output_status(o as outputform)
  o.status.refresh
  call output_twirl(o)
end sub

' ---------------------------------------------------------------------
'
sub output_write_append(o as outputform, filename as string)
  on error resume next
  open filename for append as #2
  if err <> 0 then
    call error_notify("Sorry, could not open <" + filename + ">")
    exit sub
  end if
  print #2, o.output.text;
  close #2
end sub

' ---------------------------------------------------------------------
' we have been notified of application finish
' handle image feedback and kick the parent
'
sub output_finish(o as outputform)
  dim s as string
  s = "program finished"

  dim parent as integer
  parent = cint("&H" + o.parent.caption)
  dim rc as long
  rc = SendMessage(parent, WM_KEYDOWN, CONST_KEY_FINISH, clng(0))

  dim flags as string
  flags = o.flags.caption

  if instr(flags, OUTPUT_FLAGS_APPEND_WHEN_DONE) <> 0 then
    dim filename as string
    filename = o.after.caption
    call output_write_append(o, filename)
  end if

  if instr(flags, OUTPUT_FLAGS_CLOSE_WHEN_DONE) <> 0 then
    unload o
    exit sub
  end if

  o.status.text = s
  o.animate.picture = o.twirl_all.picture
  o.htask.caption = "0000"
  o.hinstance.caption = "0000"
  o.menu_process_pause.enabled = false
  o.menu_process_resume.enabled = false
  o.menu_process_stop.enabled = false
end sub

' ---------------------------------------------------------------------
' post message to application
'
sub output_post(o as outputform, post as integer)
  dim htask as integer
  htask = cint("&H" + o.htask.caption)
  dim wparam as integer
  wparam = 0
  dim lparam as long
  lparam = 0
  dim rc as long
  if htask <> 0 then
    rc = PostAppMessage(htask, post, wparam, lparam)
  end if
end sub

' ---------------------------------------------------------------------
' pause button clicked
'
sub output_pause(o as outputform)
  call output_post(o, CONST_MESSAGE_PAUSE)
  o.menu_process_pause.enabled = false
  o.menu_process_resume.enabled = true
end sub

' ---------------------------------------------------------------------
' resume button clicked
'
sub output_resume(o as outputform)
  call output_post(o, CONST_MESSAGE_RESUME)
  o.menu_process_pause.enabled = true
  o.menu_process_resume.enabled = false
end sub

' ---------------------------------------------------------------------
' stop button clicked
'
sub output_stop(o as outputform)
  call output_post(o, CONST_MESSAGE_STOP)
end sub

' ---------------------------------------------------------------------
' stop button clicked
'
sub output_exit(o as outputform)
  call output_stop(o)
  unload o
end sub
