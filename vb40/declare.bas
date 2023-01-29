Option Explicit

Public Type ISVB
  dummy as integer
End Type

Global Const MAXFIELDS = 40

Global Const RC_BAD = 1
Global Const RC_GOOD = 0

Global Const OUTWIN_SMALL_HEIGHT = 5
Global Const global_ym = 320
Global Const global_yb = 280
Global Const global_xm = 80
Global Const global_xb = 120

Global Const CONST_KEY_FEEDBACK = 112
Global Const CONST_KEY_LAUNCHED = 113
Global Const CONST_KEY_STATUS = 114
Global Const CONST_KEY_FINISH = 115
Global Const CONST_KEY_RUN = 116

Global Const CONST_MESSAGE_PAUSE = &H400        ' agree with dtmsw.h
Global Const CONST_MESSAGE_RESUME = &H401
Global Const CONST_MESSAGE_STOP = &H402

Global Const ERROR_STRING = ":@! ERROR !@:"

Global Const SW_SHOW = 5
Global Const WM_KEYDOWN = &H100
Global Const WM_KEYUP = &H101
Global Const WM_QUIT = &H12
Global Const WM_USER = &H0400

#if win16 then
Declare Function WinExec Lib "Kernel" _
  (ByVal lpCmdLine As String, _
   ByVal nCmdShow As Integer) As Integer
Declare Function IsWindow Lib "user" _
  (ByVal hwnd As Integer) As Integer
Declare Function SendMessage Lib "user" _
  (ByVal hWnd As Integer, _
   ByVal wMsg As Integer, _
   ByVal wParam As Integer, _
   ByVal lParam As Any) As Long
Declare Function PostMessage Lib "user" _
  (ByVal hWnd As Integer, _
   ByVal wMsg As Integer, _
   ByVal wParam As Integer, _
   ByVal lParam As Any) As Long
Declare Function GetPrivateProfileString Lib "Kernel" _
  (ByVal lpApplicationName As String, _
   ByVal lpKeyName As Any, _
   ByVal lpDefault As String, _
   ByVal lpReturnedString As String, _
   ByVal nSize As Integer, _
   ByVal lpFileName As String) As Integer
Declare Function WritePrivateProfileString Lib "Kernel" _
  (ByVal lpApplicationName As String, _
   ByVal lpKeyName As Any, _
   ByVal lpString As Any, _
   ByVal lpFileName As String) As Integer

Global Const EM_SETREADONLY = (WM_USER+31)
#end if

#if win32 then
Declare Function WinExec Lib "kernel32" Alias "WinExec" _
  (ByVal lpCmdLine As String, _
   ByVal nCmdShow As Long) As Long
Declare Function IsWindow Lib "user32" Alias "IsWindow" _
  (ByVal hwnd As Long) As Long
Declare Function SendMessage Lib "user32" Alias "SendMessageA" _
  (ByVal hwnd As Long, _
   ByVal wMsg As Long, _
   ByVal wParam As Long, _
   lParam As Long) As Long
Declare Function PostMessage Lib "user32" Alias "PostMessageA" _
  (ByVal hwnd As Long, _
   ByVal wMsg As Long, _
   ByVal wParam As Long, _
   ByVal lParam As Long) As Long
Declare Function GetPrivateProfileString _
   Lib "kernel32" Alias "GetPrivateProfileStringA" _
  (ByVal lpApplicationName As String, _
   ByVal lpKeyName As Any, _
   ByVal lpDefault As String, _
   ByVal lpReturnedString As String, _
   ByVal nSize As Long, _
   ByVal lpFileName As String) As Long
Declare Function WritePrivateProfileString _
   Lib "kernel32" Alias "WritePrivateProfileStringA" _
  (ByVal lpApplicationName As String, _
   ByVal lpKeyName As Any, _
   ByVal lpString As Any, _
   ByVal lpFileName As String) As Long
Declare Function SetEnvironmentVariable _
   Lib "kernel32" Alias "SetEnvironmentVariableA" _
  (ByVal lpName As String, _
   ByVal lpValue As String) As Long
Declare Function OpenProcess _
   Lib "kernel32" Alias "OpenProcess" _
  (ByVal dwDesiredAccess As Long, _
   ByVal bInheritHandle As Long, _
   ByVal dwProcessId As Long) As Long
Declare Function WaitForSingleObject _
   Lib "kernel32" Alias "WaitForSingleObject" _
  (ByVal hHandle As Long, _
   ByVal dwMilliseconds As Long) As Long
Declare Function CloseHandle _
   Lib "kernel32" Alias "CloseHandle" _
  (ByVal hObject As Long) As Long
Declare Function GetLastError _
  Lib "kernel32" Alias "GetLastError" () As Long
Declare Sub Sleep _
  Lib "kernel32" Alias "Sleep" _
 (ByVal dwMilliseconds As Long)

Public Const STANDARD_RIGHTS_REQUIRED = &HF0000
Public Const SYNCHRONIZE = &H100000
Public Const PROCESS_ALL_ACCESS = _
  STANDARD_RIGHTS_REQUIRED Or _
  SYNCHRONIZE Or _
  &H000FFF

Public Const EM_SETREADONLY = &HCF
#end if
