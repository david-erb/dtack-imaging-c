Option Explicit

' ------------------------------------------------------------------------
sub notify_error( _
  message as string _
)
  call notify_ari(message, "Tableau Video Ceasar Error")
end sub

' ------------------------------------------------------------------------
function notify_yesno( _
  message as string, _
  title as string _
) as integer
  notify_yesno = msgbox(message, vbYesNo, title)
end function

' ------------------------------------------------------------------------
sub notify_ari( _
  message as string, _
  title as string _
)
  dim answer as integer
  answer = msgbox(message, vbAbortRetryIgnore, title)
  if answer = vbRetry then
    call main_rerun
  end if
  if answer = vbAbort then
    end
  end if

end sub
