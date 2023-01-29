Option Explicit

global const OUTPUT_FLAGS_APPEND_WHEN_DONE = "append"
global const OUTPUT_FLAGS_CLOSE_WHEN_DONE = "close"
global const OUTPUT_ALIVE_TIMEOUT = 5000

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
  n = util_s2i(o.twirl_count.caption) mod 4
  o.animate.picture = o.twirl(n).picture
  n = (n + 1) mod 4
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
  call util_sendmessage( _
    o.parent.caption, _
    WM_KEYDOWN, _
    CONST_KEY_FEEDBACK, 0)
end sub

' ---------------------------------------------------------------------
' get image data and put it into requested file
'
sub output_feedback_import(o as outputform)
  dim wname as string
  wname = o.feedback.list(1)		' 2. name of window
  dim k as integer
  k = winlist_search(winlistform, wname)
  if k = -1 then
    Dim Q As String
    Q = Chr$(34)
    notify_error( _
      "Sorry, there is no" + _
      " image window named " + _
      Q + wname + Q + ".")
    exit sub
  end if

  dim filetype as string
  filetype = o.feedback.list(2)		' 3. type of file

  dim filename as string
  filename = o.feedback.list(3)		' 4. name of file

  call image_write( _
    forms(k), filename, filetype)

  o.feedback.clear
  call output_post(o, CONST_MESSAGE_RESUME)
end sub

' ---------------------------------------------------------------------
' read given file into image
'
sub output_feedback_export(o as outputform)
  dim wname as string
  wname = o.feedback.list(1)		' 2. name of window
  dim k as integer
  k = winlist_search(winlistform, wname)
  if k = -1 then			' image does not exist yet?
    k = image_create(winlistform, wname)
  end if

  dim x0 as integer
  x0 = util_s2i(o.feedback.list(2))		' 3. width
  dim y0 as integer
  y0 = util_s2i(o.feedback.list(3))		' 4. height

  dim xe as integer
  xe = util_s2i(o.feedback.list(4))		' 5. width
  dim ye as integer
  ye = util_s2i(o.feedback.list(5))		' 6. height

  dim filetype as string
  filetype = o.feedback.list(6)		' 7. type of file

  dim filename as string
  filename = o.feedback.list(7)		' 8. name of file

  call image_read( _
    forms(k), filename, filetype)

  if x0 = -1 then
    x0 = 0
  end if

  forms(k).left = _
    x0 * Screen.TwipsPerPixelX

  if y0 = -1 then
    y0 = 0
  end if

  forms(k).top = _
    y0 * Screen.TwipsPerPixelY

  call image_unclip(forms(k))		' display entire image
  forms(k).show				' show the image on the display

  o.feedback.clear			' clear feedback buffer
  call output_post(o, _
    CONST_MESSAGE_RESUME)
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
' expects exactly two feedback items: procwin and procid
' procwin is a window handle to which stop and pause should be posted
' procid can be used to wait on or to issue a terminate to
' parent param form is notified of the launch by putting feedback outputform
'
sub output_launched(o as outputform)
  if o.feedback.listcount > 0 then
    o.procwin.caption = o.feedback.list(0)
  else
    o.procwin.caption = "00000000"
  end if
  if o.feedback.listcount > 1 then
    o.procid.caption = o.feedback.list(1)
  else
    o.procid.caption = "00000000"
  end if
  o.menu_process_pause.enabled = true
  o.menu_process_resume.enabled = false
  o.menu_process_stop.enabled = true
  o.menu_pause.enabled = true
  o.menu_resume.enabled = false
  o.menu_stop.enabled = true
  outputform.feedback.clear

  dim winid as string
  winid = "[" + o.procwin.caption + _
    "/" + o.procid.caption + "]"

  o.caption = _
    util_id() + " " + _
      o.pname.caption + " " + _
      winid + " Output"

  outputform.feedback.additem _
    "Launched " + winid

  call util_sendmessage( _
    o.parent.caption, _
    WM_KEYDOWN, _
    CONST_KEY_FEEDBACK, 0)

  o.feedback.clear
  call output_post(o, CONST_MESSAGE_RESUME)

  o.thetimer.Interval = _
    OUTPUT_ALIVE_TIMEOUT
  o.thetimer.Enabled = True		' start the timer

end sub

' ---------------------------------------------------------------------
' called when timer goes off
'
Sub output_tick(o as outputform)
  dim flags as long
  flags = PROCESS_ALL_ACCESS

  dim procid as long
  procid = util_s2l("&H" + o.procid.caption)

  dim handle as long
  handle = OpenProcess( _
    flags, clng(0), procid)		' see if process is still alive

  If handle <> 0 Then
'    call output_append(o, _
'      "vb40_output_tick: procid " + _
'      o.procid.caption + _
'      " is still alive")
    CloseHandle(handle)
    o.thetimer.Interval = _
      OUTPUT_ALIVE_TIMEOUT
    o.thetimer.Enabled = True		' restart the timer
  Else
    o.thetimer.Enabled = False		' terminate the timer
    call output_append(o, _
      "vb40_output_tick: procid " + _
      o.procid.caption + _
      " seems to have died")
    call output_finish(o)		' clean up and leave
  End If

  
End Sub

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
    call notify_error("Sorry, could not open <" + filename + ">")
    exit sub
  end if
  print #2, o.output.text;
  close #2
end sub

' ---------------------------------------------------------------------
' we have been notified of application finish
' kick the parent
' append output to file if desired
' close output window if desired
'
sub output_finish(o as outputform)
  o.thetimer.Enabled = False		' terminate the timer

  dim s as string
  s = "program finished"

  If o.status.text <> s	Then		' only do this stuff once
    call util_postmessage( _
      o.parent.caption, _
      WM_KEYDOWN, _
      CONST_KEY_FINISH, 0)		' notify the parent

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

    o.animate.picture = o.twirl_all.picture
    o.menu_process_pause.enabled = false
    o.menu_process_resume.enabled = false
    o.menu_process_stop.enabled = false
    o.menu_pause.enabled = false
    o.menu_resume.enabled = false
    o.menu_stop.enabled = false
    o.status.text = s
  End if
end sub

' ---------------------------------------------------------------------
' post message to application
'
sub output_post(o as outputform, post as long)
  call util_postmessage(o.procwin.caption, post, 0, 0)
end sub

' ---------------------------------------------------------------------
' pause button clicked
'
sub output_pause(o as outputform)
  call output_post(o, CONST_MESSAGE_PAUSE)
  o.menu_process_pause.enabled = false
  o.menu_process_resume.enabled = true
  o.menu_pause.enabled = false
  o.menu_resume.enabled = true
end sub

' ---------------------------------------------------------------------
' resume button clicked
'
sub output_resume(o as outputform)
  call output_post(o, CONST_MESSAGE_RESUME)
  o.menu_process_pause.enabled = true
  o.menu_process_resume.enabled = false
  o.menu_pause.enabled = true
  o.menu_resume.enabled = false
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
