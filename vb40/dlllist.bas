Attribute VB_Name = "DLLLIST"
Option Explicit

' ---------------------------------------------------------------------
'
Function dlllist_search(f As dlllistform, dllname As String) As Integer
  Dim k As Integer
  For k = 0 To (f.namelist.ListCount - 1)
    If f.namelist.List(k) = dllname Then
      dlllist_search = k
      Exit Function
    End If
  Next k
  dlllist_search = -1
End Function

' ---------------------------------------------------------------------
'
Function dlllist_add(f As dlllistform, dllname As String) As Integer
  Dim k As Integer
  k = dlllist_search(f, dllname)
  If k = -1 Then
    f.namelist.AddItem dllname
    k = f.namelist.ListCount
  End If
  dlllist_add = k
End Function

' ---------------------------------------------------------------------
' read list of dlls from profile
'
Sub dlllist_profile(f As dlllistform)
  Dim i As Integer
  Dim s As String
  Dim key As String
  For i = 0 To 9999
    key = PROFILE_KEY_DLL + Trim$(Str$(i))
    s = profile_get(PROFILE_INI_DLL, PROFILE_APP_DLLS, key)
    If s = "" Then
      Exit For
    End If
    Dim k As Integer
    k = dlllist_add(f, s)               ' add name to internal list
  Next
End Sub

' ---------------------------------------------------------------------
' make dllpath\dllname.dll from dllname
'
Function dlllist_path(f As dlllistform, dllname As String) As String
  Dim s As String
  s = profile_get(util_ini(), PROFILE_APP_GLOBALS, PROFILE_KEY_DLLPATH)
  If s <> "" Then
    s = s + "\" + dllname
  Else
    s = dllname
  End If
  dlllist_path = s + ".dll"
End Function

' ---------------------------------------------------------------------
'
Sub dlllist_reload_all(f As dlllistform)
  Call dlllist_unload_all(f)
  Call dlllist_load_all(f)
End Sub

' ---------------------------------------------------------------------
'
Sub dlllist_load_all(f As dlllistform)
  Call dlllist_profile(f)               ' reread the profile
  f.WindowState = 0
  f.Show

  Dim i As Integer
  For i = 0 To (f.namelist.ListCount - 1)
    Dim s As String
    s = f.namelist.List(i)
    Call dlllist_reload_one(f, s)
    Call dlllist_usage_all(f)
  Next
End Sub

' ---------------------------------------------------------------------
'
Sub dlllist_unload_all(f As dlllistform)
  Call dlllist_profile(f)               ' reread the profile
  f.WindowState = 0
  f.Show

  Dim i As Integer
  For i = (f.namelist.ListCount - 1) To 0 Step -1
    Dim s As String
    s = f.namelist.List(i)
    Call dlllist_unload_one(f, s)
    Call dlllist_usage_all(f)
  Next
End Sub

' ---------------------------------------------------------------------
'
Sub dlllist_usage_all(f As dlllistform)
  Load f
  If f.namelist.ListCount = 0 Then      ' profile not read yet?
    Call dlllist_profile(f)
  End If
  f.WindowState = 0                     ' make window visible
  f.Show
  
  Dim i As Integer
  For i = 0 To (f.namelist.ListCount - 1)
    Dim s As String
    s = f.namelist.List(i)
    Call dlllist_usage_one(f, s)
  Next

  f.pathlist.Refresh
  f.linklist.Refresh
  f.handlelist.Refresh
End Sub

' ---------------------------------------------------------------------
'
Sub dlllist_reload_one(f As dlllistform, dllname As String)
  Call dlllist_unload_one(f, dllname)

  Dim handle As Integer
  handle = dlllist_load_one(f, dllname)

  Call dlllist_usage_one(f, dllname)
End Sub

' ---------------------------------------------------------------------
' load the given dll, but first all its dependencies
'
Function dlllist_load_depends(f As dlllistform, dllname As String) As Integer
  Dim i As Integer
  Dim s As String
  Dim key As String
  Dim handle As Integer

  For i = 0 To 9999
    key = PROFILE_KEY_NEEDS + Trim$(Str$(i))
    s = profile_get(PROFILE_INI_DLL, dllname, key)
    If s = "" Then
      Exit For
    End If
    handle = dlllist_load_one(f, s)
    If handle = -1 Then
      dlllist_load_depends = handle
      Exit Function
    End If
  Next
  handle = dlllist_load_one(f, dllname) ' finally, load the given dll
  dlllist_load_depends = handle
End Function

' ---------------------------------------------------------------------
'
Sub dlllist_unload_one(f As dlllistform, dllname As String)
  Dim path As String
  path = dlllist_path(f, dllname)
  Dim handle As Integer
'  handle = GetModuleHandle(path)
  Dim usage As Integer
'  usage = GetModuleUsage(handle)
  Dim i As Integer
  For i = 1 To usage
'    freelibrary (handle)
  Next
  Call dlllist_usage_one(f, dllname)
End Sub

' ---------------------------------------------------------------------
'
Sub dlllist_usage_one(f As dlllistform, dllname As String)
  Dim path As String
  path = dlllist_path(f, dllname)
  Dim handle As Integer
'  handle = GetModuleHandle(path)
  Dim usage As Integer
'  usage = GetModuleUsage(handle)
  Dim k As Integer
  k = dlllist_add(f, dllname)
  f.pathlist.List(k) = path
  f.linklist.List(k) = Right$(Space$(3) + Str$(usage), 3) + " "
  f.handlelist.List(k) = Right$(Space$(6) + Str$(handle), 6)
End Sub

' ---------------------------------------------------------------------
'
Function dlllist_ifload_one(f As dlllistform, dllname As String) As Integer
  Dim path As String
  path = dlllist_path(f, dllname)
  Dim handle As Integer
'  handle = GetModuleHandle(path)
  Dim usage As Integer
'  usage = GetModuleUsage(handle)
  If usage = 0 Then
    dlllist_ifload_one = dlllist_load_one(f, dllname)
  Else
    dlllist_ifload_one = handle
  End If
End Function

' ---------------------------------------------------------------------
'
Function dlllist_load_one(f As dlllistform, dllname As String) As Integer
  Dim path As String
  path = dlllist_path(f, dllname)

  Dim handle As Integer
'  handle = LoadLibrary(path)

  If handle < 32 Then
    Dim s As String
    s = "Sorry, could not load "
    s = s + path
    s = s + " (error " + Trim$(Str$(handle)) + ")."
    Call notify_error(s)
    handle = -1
  End If
  dlllist_load_one = handle
End Function
