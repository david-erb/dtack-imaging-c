Option Explicit

Global Const SCRIPT_FINISH_TIMEOUT = 2000

' ---------------------------------------------------------------------
' read one line lines from file and process
' each line in file must have three strings:
' 1. program name, must have section in dtack.ini
' 2. params to go before environment
' 3. arguments to go after environment
'
Sub script_next(f As scriptform)
  dim pname as string
  dim params as string
  dim args as string
  dim s as string
  dim rc as integer

  do
    if eof(1) then			' end of file #1
      call script_finished(f)
      exit sub
    end if

    on error resume next
    input #1, pname, params, args
    if err <> 0 then
      s = f.scriptname
      call notify_error( _
        "Sorry, read error <" + _
        err.description + ">" + _
        " in script " + s + ".")
      call script_finished(f)
      exit sub
    end if
  
    s = ""
    if f.common <> "" then
      s = s + " " + f.common 
    end if
    if f.env <> "" then
      s = s + " " + f.env
    end if
    if params <> "" then
      s = s + " " + params
    end if
    if args <> "" then
      s = s + " " + args
    end if
  
    f.doing.caption = "doing " + pname
  
    dim timeout as long
    timeout = 0
    dim after as string
    after = f.outfile.caption
    dim flags as string
    flags = f.flags
    rc = util_fork(f, pname, 0, timeout, after, flags, s)
  
    if rc <> RC_GOOD then
      f.done.additem "MISS " + f.executable.caption + " " + f.subcommand.caption
    end if
  loop while rc <> RC_GOOD

End Sub

' ---------------------------------------------------------------------
' read lines from given file and process
'
Sub script_run( _
  f As scriptform, _
  scriptname as string, _
  env as string, _
  outfile as string _
)
  dim fd as integer
  fd = FreeFile()

  dim s as string
  s = profile_get(util_ini(), _
    PROFILE_APP_GLOBALS, _
    PROFILE_KEY_SCRIPTPATH)
  if s <> "" then
    s = s + "\"
  end if

  s = s + scriptname

  f.caption = _
    util_id() + " " + s
  f.scriptname = s
  f.env = env
  f.outfile = outfile

  on error resume next
  open s for input shared as #1
  if err <> 0 then
    call notify_error("Sorry, could not open <" + s + ">")
    call script_finished(f)
    exit sub
  end if

  dim common as string
  input #1, common
  if err <> 0 then
    call notify_error( _
      "Sorry, read error <" + _
      err.description + ">" + _
      " in script " + s + ".")
    call script_finished(f)
    exit sub
  end if

  f.common = common

  call script_next(f)			' run first command
End Sub

' ---------------------------------------------------------------------
'
Sub script_finished(f as scriptform)
  f.doing.caption = "all finished"
  close #1
End Sub

' ---------------------------------------------------------------------
' called when timer goes off
'
sub script_tick(f As scriptform)
  f.thetimer.enabled = false		' stop the timer
  screen.mousepointer = 1		' change mouse pointer to arrow
  call script_next(f)			' run next command
end sub

' ---------------------------------------------------------------------
' called when keyboard message received
'
Sub script_key( _
  f As scriptform, _
  key as integer, _
  shift as integer _
)
  f.keyinput.text = "key=0x" + hex$(key) + " shift=0x" + hex$(shift)

  if key = CONST_KEY_FINISH then	' previous command finished?
    f.done.additem "DONE " + _
      f.executable.caption + " " + _
      f.subcommand.caption
    f.doing.caption = "pausing..."
    f.thetimer.interval = SCRIPT_FINISH_TIMEOUT
    f.thetimer.enabled = true		' set timer to wait longer
  end if
End Sub
