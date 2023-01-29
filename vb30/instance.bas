Option Explicit

' ---------------------------------------------------------------------
' make exepath\exename.exe from exename
'
function instance_executable (f As instanceform, pname as string) as string
  dim s as string
  dim executable as string
  s = profile_get(PROFILE_INI_MASTER, PROFILE_APP_GLOBALS, PROFILE_KEY_DLLPATH)
  if s <> "" then
    executable = s + "\"
  else
    executable = ""
  end if

  s = profile_get(PROFILE_INI_MASTER, pname, PROFILE_KEY_EXECUTABLE)
  if s <> "" then
    executable = executable + s
  else
    executable = executable + pname
  end if

  instance_executable = executable + ".exe"
end function

' ---------------------------------------------------------------------
'
sub instance_refresh (f As instanceform)
  f.show
end sub

' ---------------------------------------------------------------------
' add instance to running list
'
sub instance_add(f As instanceform, p as paramform, o as outputform, h as integer)
  dim hs as string
  hs = util_04x(h)

  o.tag = hs

  dim pname as string
  pname = p.pname.caption

  dim s as string
  s = "instance " + hs
  s = s + " pname=" + pname
  s = s + " outwin=" + util_04x(o.output.hwnd)

  f.thelist.additem s
end function
