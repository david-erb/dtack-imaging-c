VERSION 4.00
Begin VB.Form profileform 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   Caption         =   "profileform"
   ClientHeight    =   4920
   ClientLeft      =   6375
   ClientTop       =   2325
   ClientWidth     =   5520
   BeginProperty Font 
      name            =   "MS Sans Serif"
      charset         =   0
      weight          =   700
      size            =   8.25
      underline       =   0   'False
      italic          =   0   'False
      strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   Height          =   5610
   Icon            =   "PROFILE.frx":0000
   Left            =   6315
   LinkTopic       =   "Form1"
   ScaleHeight     =   4920
   ScaleWidth      =   5520
   Tag             =   "5"
   Top             =   1695
   Width           =   5640
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   12
      Left            =   2280
      TabIndex        =   28
      Tag             =   "dt_yield"
      Top             =   4080
      Width           =   3135
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   11
      Left            =   2280
      TabIndex        =   25
      Tag             =   "dt_output"
      Top             =   3000
      Width           =   3135
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   10
      Left            =   2280
      TabIndex        =   23
      Tag             =   "dt_mem"
      Top             =   3360
      Width           =   3135
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   9
      Left            =   2280
      TabIndex        =   21
      Tag             =   "dt_win"
      Top             =   3720
      Width           =   3135
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   8
      Left            =   2280
      TabIndex        =   20
      Tag             =   "dllpath"
      Top             =   480
      Width           =   3135
   End
   Begin VB.TextBox inifile 
      Appearance      =   0  'Flat
      Height          =   285
      Left            =   2280
      TabIndex        =   18
      Top             =   120
      Width           =   3135
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   7
      Left            =   2280
      TabIndex        =   16
      Tag             =   "dt_d"
      Top             =   1200
      Width           =   3135
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   6
      Left            =   4680
      TabIndex        =   11
      Tag             =   "yborder"
      Top             =   4560
      Width           =   735
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   5
      Left            =   3000
      TabIndex        =   10
      Tag             =   "xborder"
      Top             =   4560
      Width           =   735
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   4
      Left            =   2280
      TabIndex        =   8
      Tag             =   "dt_datadir"
      Top             =   2640
      Width           =   3135
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   3
      Left            =   2280
      TabIndex        =   7
      Tag             =   "scriptpath"
      Top             =   840
      Width           =   3135
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   2
      Left            =   2280
      TabIndex        =   5
      Tag             =   "dt_sayto"
      Top             =   2280
      Width           =   3135
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   1
      Left            =   2280
      TabIndex        =   4
      Tag             =   "dt_errto"
      Top             =   1920
      Width           =   3135
   End
   Begin VB.TextBox fields 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   0
      Left            =   2280
      TabIndex        =   3
      Tag             =   "dt_dbgto"
      Top             =   1560
      Width           =   3135
   End
   Begin VB.Label datadir_label 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Yield mode"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   4
      Left            =   120
      TabIndex        =   27
      Top             =   4080
      Width           =   2055
   End
   Begin VB.Label datadir_label 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Temporary directory"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   3
      Left            =   120
      TabIndex        =   26
      Top             =   3000
      Width           =   2055
   End
   Begin VB.Label datadir_label 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Memory manager"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   2
      Left            =   120
      TabIndex        =   24
      Top             =   3360
      Width           =   2055
   End
   Begin VB.Label datadir_label 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Window manager"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   1
      Left            =   120
      TabIndex        =   22
      Top             =   3720
      Width           =   2055
   End
   Begin VB.Label Label6 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Path to scripts"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   19
      Top             =   840
      Width           =   2055
   End
   Begin VB.Label Label5 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Path to .INI files"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   17
      Top             =   120
      Width           =   2055
   End
   Begin VB.Label Label4 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Debug mask"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   15
      Top             =   1200
      Width           =   2055
   End
   Begin VB.Label Label3 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Y"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   4320
      TabIndex        =   14
      Top             =   4560
      Width           =   255
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "X"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   2640
      TabIndex        =   13
      Top             =   4560
      Width           =   255
   End
   Begin VB.Label Label1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Image window border"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   12
      Top             =   4560
      Width           =   2055
   End
   Begin VB.Label datadir_label 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Data directory"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   0
      Left            =   120
      TabIndex        =   9
      Top             =   2640
      Width           =   2055
   End
   Begin VB.Label path_label 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Path to DLLs and EXEs"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Tag             =   "dllpath"
      Top             =   480
      Width           =   2055
   End
   Begin VB.Label sayto_label 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Informational output to"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   2280
      Width           =   2055
   End
   Begin VB.Label errto_label 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Error output to"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   1920
      Width           =   2055
   End
   Begin VB.Label dbgto_label 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Debug output to"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   1560
      Width           =   2055
   End
   Begin VB.Menu menu_file 
      Caption         =   "&File"
      Begin VB.Menu menu_save 
         Caption         =   "&Save profile"
      End
      Begin VB.Menu line1 
         Caption         =   "-"
      End
      Begin VB.Menu menu_file_closethis 
         Caption         =   "&Close this"
      End
      Begin VB.Menu menu_file_exitall 
         Caption         =   "&Exit All"
      End
   End
End
Attribute VB_Name = "profileform"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_Load()
  On Error GoTo error_handler
  Call profile_globals_load(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_profile_Click()
  On Error GoTo error_handler
  profileform.Show
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_file_closethis_Click()
  On Error GoTo error_handler
  Me.Hide
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_file_exitall_Click()
  On Error GoTo error_handler
  Call exit_all
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_save_Click()
  On Error GoTo error_handler
  Call profile_globals_save(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

