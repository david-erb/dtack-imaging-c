Option Explicit

' ---------------------------------------------------------------------

Sub main()
  call main_run
end sub

' ---------------------------------------------------------------------

Sub main_run()

restart:
  load errorform
  on error goto error_handler

  call util_error_log(0, "", "")

  load profileform

  profileform.caption = _
    util_id() + " Profile"

  load outputform

  load paramform
  paramform.pname = util_id()

  If param_init(paramform) <> RC_GOOD Then
    end
  End If
  exit sub

error_handler:
  unload errorform
  unload paramform
  unload outputform
  unload profileform
  dim answer as integer
  answer = notify_yesno("Run front end again?", _
         "Run again")
  if answer = vbYes then
    goto restart
  else
    end
  end if

End Sub

' ---------------------------------------------------------------------

Sub main_rerun()

  on error resume next
  unload paramform
  unload outputform
  unload profileform
  call main_run

End Sub
