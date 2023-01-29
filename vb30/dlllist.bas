Option Explicit

' ---------------------------------------------------------------------
'
function dlllist_search (f as dlllistform, dllname as string) as integer
  dim k as integer
  for k = 0 to (f.namelist.listcount-1)
    if f.namelist.list(k) = dllname then
      dlllist_search = k
      exit function
    end if
  next k
  dlllist_search = -1
end function

' ---------------------------------------------------------------------
'
function dlllist_add (f as dlllistform, dllname as string) as integer
  dim k as integer
  k = dlllist_search(f, dllname)
  if k = -1 then
    f.namelist.additem dllname
    k = f.namelist.listcount
  end if
  dlllist_add = k 
end function

' ---------------------------------------------------------------------
' read list of dlls from profile
'
sub dlllist_profile (f as dlllistform)
  Dim i As Integer
  Dim s As String
  Dim key As String
  For i = 0 To 9999
    key = PROFILE_KEY_DLL + Trim$(Str$(i))
    s = profile_get(PROFILE_INI_DLL, PROFILE_APP_DLLS, key)
    If s = "" Then
      Exit For
    End If
    dim k as integer
    k = dlllist_add(f, s)		' add name to internal list
  Next
End Sub

' ---------------------------------------------------------------------
' make dllpath\dllname.dll from dllname
'
function dlllist_path (f As dlllistform, dllname as string) as string
  dim s as string
  s = profile_get(PROFILE_INI_MASTER, PROFILE_APP_GLOBALS, PROFILE_KEY_DLLPATH)
  if s <> "" then
    s = s + "\" + dllname
  else
    s = dllname
  end if
  dlllist_path = s + ".dll"
end function

' ---------------------------------------------------------------------
'
sub dlllist_reload_all (f as dlllistform)
  Call dlllist_unload_all(f)
  Call dlllist_load_all(f)
End Sub

' ---------------------------------------------------------------------
'
sub dlllist_load_all (f as dlllistform)
  call dlllist_profile(f)		' reread the profile
  f.WindowState = 0
  f.Show

  Dim i As Integer
  For i = 0 To (f.namelist.ListCount - 1)
    dim s as string
    s = f.namelist.list(i)
    Call dlllist_reload_one(f, s)
    Call dlllist_usage_all(f)
  Next
End Sub

' ---------------------------------------------------------------------
'
sub dlllist_unload_all (f as dlllistform)
  call dlllist_profile(f)		' reread the profile
  f.WindowState = 0
  f.Show

  Dim i As Integer
  For i = (f.namelist.ListCount - 1) To 0 Step -1
    dim s as string
    s = f.namelist.list(i)
    Call dlllist_unload_one(f, s)
    Call dlllist_usage_all(f)
  Next
End Sub

' ---------------------------------------------------------------------
'
sub dlllist_usage_all (f as dlllistform)
  load f
  if f.namelist.listcount = 0 then	' profile not read yet?
    call dlllist_profile(f)
  end if
  f.WindowState = 0			' make window visible
  f.Show
  
  Dim i As Integer
  For i = 0 To (f.namelist.ListCount - 1)
    dim s as string
    s = f.namelist.list(i)
    Call dlllist_usage_one(f, s)
  Next

  f.pathlist.refresh
  f.linklist.refresh
  f.handlelist.refresh
End Sub

' ---------------------------------------------------------------------
'
sub dlllist_reload_one (f as dlllistform, dllname As String)
  Call dlllist_unload_one(f, dllname)

  Dim handle As Integer
  handle = dlllist_load_one(f, dllname)

  call dlllist_usage_one(f, dllname)
End Sub

' ---------------------------------------------------------------------
' load the given dll, but first all its dependencies
'
function dlllist_load_depends (f as dlllistform, dllname as string) as integer
  Dim i As Integer
  Dim s As String
  Dim key As String
  dim handle as integer

  For i = 0 To 9999
    key = PROFILE_KEY_NEEDS + Trim$(Str$(i))
    s = profile_get(PROFILE_INI_DLL, dllname, key)
    If s = "" Then
      Exit For
    End If
    handle = dlllist_load_one(f, s)
    if handle = -1 then
      dlllist_load_depends = handle
      exit function
    end if
  Next
  handle = dlllist_load_one(f, dllname)	' finally, load the given dll
  dlllist_load_depends = handle
End function

' ---------------------------------------------------------------------
'
sub dlllist_unload_one (f as dlllistform, dllname As String)
  dim path as string
  path = dlllist_path(f, dllname)
  Dim handle As Integer
  handle = GetModuleHandle(path)
  Dim usage As Integer
  usage = GetModuleUsage(handle)
  Dim i As Integer
  For i = 1 To usage
    freelibrary (handle)
  Next
  call dlllist_usage_one(f, dllname)
End Sub

' ---------------------------------------------------------------------
'
sub dlllist_usage_one (f as dlllistform, dllname As String)
  dim path as string
  path = dlllist_path(f, dllname)
  Dim handle As Integer
  handle = GetModuleHandle(path)
  Dim usage As Integer
  usage = GetModuleUsage(handle)
  dim k as integer
  k = dlllist_add(f, dllname)
  f.pathlist.list(k) = path
  f.linklist.list(k) = Right$(Space$(3) + Str$(usage), 3) + " "
  f.handlelist.list(k) = Right$(Space$(6) + Str$(handle), 6)
End Sub

' ---------------------------------------------------------------------
'
function dlllist_ifload_one (f As dlllistform, dllname as string) as integer
  dim path as string
  path = dlllist_path(f, dllname)
  Dim handle As Integer
  handle = GetModuleHandle(path)
  Dim usage As Integer
  usage = GetModuleUsage(handle)
  if usage = 0 then
    dlllist_ifload_one = dlllist_load_one(f, dllname)
  else
    dlllist_ifload_one = handle
  end if
end function

' ---------------------------------------------------------------------
'
function dlllist_load_one (f As dlllistform, dllname as string) as integer
  dim path as string
  path = dlllist_path(f, dllname)

  Dim handle As Integer
  handle = LoadLibrary(path)

  if handle < 32 then
    dim s as string
    s = "Sorry, could not load "
    s = s + path
    s = s + " (error " + trim$(str$(handle)) + ")."
    call error_notify(s)
    handle = -1
  end if
  dlllist_load_one = handle
end function
