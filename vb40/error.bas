Option Explicit

' ------------------------------------------------------------------------
sub error_notify(message as string)
  call notify_error(message)
end sub

' ------------------------------------------------------------------------
function notify_yesno(message as string, title as string) as integer
  notify_yesno = notify_yesno(message, title)
end function
