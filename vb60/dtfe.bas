Attribute VB_Name = "module_dtfe"
Option Explicit
Option Base 0

' -------------------------------------------------------------------------
' Exit if error.

Sub main_iferror_exit(e as string)
  Dim f As String: f = "main_iferror_exit"
  if e = "" then exit sub
  Call util_debug(f, UTIL_LEVEL_ERROR, e)
  end
end sub

' -------------------------------------------------------------------------
' Called by VB at program startup.

Sub main()
  Dim f As String: f = "main"

  Dim path As String
  dim exe as string

  path = util_extract(command(), " ", 0)
  exe = util_extract(command(), " ", 1)
  If path = "" Then path = App.path
  Call util_chdir(path)

  global_title = exe

  util_global_debug_level = "all"

  if false then _
  Call util_debug(f, UTIL_LEVEL_USR1, _
    "curdir=" + util_quote(CurDir) + _
    " app.path=" + util_quote(App.path) + _
    " profile path=" + util_quote(path))

  Call util_debug(f, UTIL_LEVEL_USR1, "rev " & APP_REV & " " & APP_DATE)

End Sub

