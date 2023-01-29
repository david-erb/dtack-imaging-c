Option Explicit

Global Const MAXFIELDS = 40

Global Const RC_BAD = 1
Global Const RC_GOOD = 0

Global Const OUTWIN_SMALL_HEIGHT = 5
Global Const global_ym = 320
Global Const global_yb = 100
Global Const global_xm = 80
Global Const global_xb = 120

Global Const CONST_KEY_FEEDBACK = 112
Global Const CONST_KEY_LAUNCHED = 113
Global Const CONST_KEY_STATUS = 114
Global Const CONST_KEY_FINISH = 115

Global Const CONST_MESSAGE_PAUSE = &H400        ' agree with dtmsw.h
Global Const CONST_MESSAGE_RESUME = &H401
Global Const CONST_MESSAGE_STOP = &H402

Global Const ERROR_STRING = ":@! ERROR !@:"

Declare Function dtutil_string_global Lib "dtutil.dll" (ByVal s As String, byval l as integer) as integer

Declare Function WinExec Lib "Kernel" (ByVal lpCmdLine As String, ByVal nCmdShow As Integer) As Integer
Declare Function lstrcpy Lib "Kernel" (ByVal lpString1 As Any, ByVal lpString2 As Any) As Long
Declare Function lstrlen Lib "Kernel" (ByVal lpString As Any) As Integer
Declare Function lstrcpy2 Lib "Kernel" Alias "lstrcpy" (ByVal lpString1 As long, ByVal lpString2 As string) As Long
Declare Function lstrcpyn2 Lib "Kernel" Alias "lstrcpyn" (ByVal lpString1 As long, ByVal lpString2 As string, byval n as integer) As Long
Declare Function lstrlen2 Lib "Kernel" Alias "lstrlen" (ByVal lpString As long) As Integer

Declare Function LoadLibrary Lib "kernel" (ByVal libname$) As Integer
Declare Sub freelibrary Lib "kernel" (ByVal hLibMod As Integer)
Declare Function GetModuleHandle Lib "kernel" (ByVal modname As String) As Integer
Declare Function GetModuleUsage Lib "kernel" (ByVal handle As Integer) As Integer
Declare Function SendMessage Lib "user" (ByVal hWnd As Integer, ByVal wMsg As Integer, ByVal wParam As Integer, ByVal lParam As Any) As Long
Declare Function PostAppMessage Lib "user" (ByVal hWnd As Integer, ByVal wMsg As Integer, ByVal wParam As Integer, ByVal lParam As Any) As Long

Declare Function GetPrivateProfileString Lib "Kernel" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpDefault As String, ByVal lpReturnedString As String, ByVal nSize As Integer, ByVal lpFileName As String) As Integer
Declare Function WritePrivateProfileString Lib "Kernel" (ByVal lpApplicationName As String, ByVal lpKeyName As Any, ByVal lpString As Any, ByVal lpFileName As String) As Integer

Global Const SW_SHOW = 5
' windows API messages
Global Const WM_KEYDOWN = &H100
Global Const WM_KEYUP = &H101
Global Const WM_QUIT = &H12
Global Const WM_USER = &H0400
Global Const EM_SETREADONLY = (WM_USER+31)

' windows API memory management
Declare Function GlobalAlloc Lib "Kernel" (ByVal wFlags As Integer, ByVal dwBytes As Long) As Integer
Declare Function GlobalSize Lib "Kernel" (ByVal hMem As Integer) As Long
Declare Function GlobalReAlloc Lib "Kernel" (ByVal hMem As Integer, ByVal dwBytes As Long, ByVal wFlags As Integer) As Integer
Declare Function GlobalLock Lib "Kernel" (ByVal hMem As Integer) As Long
Declare Function GlobalFree Lib "Kernel" (ByVal hMem As Integer) As Integer
Declare Function GlobalFlags Lib "Kernel" (ByVal hMem As Integer) As Integer
Declare Function GlobalCompact Lib "Kernel" (ByVal dwMinFree As Long) As Long
Declare Function GlobalUnlock Lib "Kernel" (ByVal hMem As Integer) As Integer
Global Const GMEM_DISCARDABLE = &H100
Global Const GMEM_MOVEABLE = &H2
Global Const GMEM_SHARE = &H2000
Global Const GMEM_FIXED = &H0
Global Const GMEM_ZEROINIT = &H40

' windows API metafiles
Declare Function CopyMetaFile Lib "GDI" (ByVal hMF As Integer, ByVal lpFileName As String) As Integer
Declare Function DeleteMetaFile Lib "GDI" (ByVal hMF As Integer) As Integer
Declare Function SetMetaFileBits Lib "GDI" (ByVal hMF As Integer) As Integer
Declare Function GetMetaFileBits Lib "GDI" (ByVal hMF As Integer) As Integer

' windows API bitmaps
Type BITMAPINFOHEADER '40 bytes
    biSize As Long
    biWidth As Long
    biHeight As Long
    biPlanes As Integer
    biBitCount As Integer
    biCompression As Long
    biSizeImage As Long
    biXPelsPerMeter As Long
    biYPelsPerMeter As Long
    biClrUsed As Long
    biClrImportant As Long
End Type

