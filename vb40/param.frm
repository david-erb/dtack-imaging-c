VERSION 4.00
Begin VB.Form paramform 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   Caption         =   "paramform"
   ClientHeight    =   3105
   ClientLeft      =   1845
   ClientTop       =   615
   ClientWidth     =   7695
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
   Height          =   3795
   Icon            =   "PARAM.frx":0000
   KeyPreview      =   -1  'True
   Left            =   1785
   LinkTopic       =   "Form1"
   ScaleHeight     =   3105
   ScaleWidth      =   7695
   Top             =   -15
   Width           =   7815
   Begin VB.TextBox argline 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      Height          =   285
      Left            =   9360
      TabIndex        =   10
      Top             =   360
      Width           =   2535
   End
   Begin VB.TextBox keyinput 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      Height          =   285
      Left            =   9360
      TabIndex        =   8
      Top             =   0
      Width           =   2535
   End
   Begin VB.Timer thetimer 
      Enabled         =   0   'False
      Left            =   120
      Top             =   240
   End
   Begin VB.TextBox field_input 
      Appearance      =   0  'Flat
      Height          =   285
      Index           =   0
      Left            =   2280
      TabIndex        =   0
      Top             =   360
      Visible         =   0   'False
      Width           =   975
   End
   Begin VB.Label subcommand 
      Height          =   255
      Left            =   9360
      TabIndex        =   18
      Top             =   1440
      Width           =   1215
   End
   Begin VB.Label Label7 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Subcommand"
      Height          =   255
      Left            =   8040
      TabIndex        =   17
      Top             =   1440
      Width           =   1215
   End
   Begin VB.Label Label6 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Start"
      Height          =   255
      Left            =   8040
      TabIndex        =   16
      Top             =   2160
      Width           =   1095
   End
   Begin VB.Label Label5 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Executable"
      Height          =   255
      Left            =   8040
      TabIndex        =   15
      Top             =   1080
      Width           =   1215
   End
   Begin VB.Label Label4 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Field count"
      Height          =   255
      Left            =   8040
      TabIndex        =   14
      Top             =   1800
      Width           =   1215
   End
   Begin VB.Label Label3 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Pname"
      Height          =   255
      Left            =   8040
      TabIndex        =   13
      Top             =   720
      Width           =   1215
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Argline"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   8040
      TabIndex        =   12
      Top             =   360
      Width           =   1215
   End
   Begin VB.Label Label1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Key input"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   8040
      TabIndex        =   11
      Top             =   0
      Width           =   1215
   End
   Begin VB.Label executable 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   9360
      TabIndex        =   9
      Top             =   1080
      Visible         =   0   'False
      Width           =   1215
   End
   Begin VB.Label field_abbrev 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "field_abbrev"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   0
      Left            =   3480
      TabIndex        =   5
      Top             =   360
      Visible         =   0   'False
      Width           =   1095
   End
   Begin VB.Label field_count 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   9360
      TabIndex        =   7
      Top             =   1800
      Visible         =   0   'False
      Width           =   1215
   End
   Begin VB.Label start 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   9360
      TabIndex        =   6
      Top             =   2160
      Visible         =   0   'False
      Width           =   495
   End
   Begin VB.Label pname 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   9360
      TabIndex        =   4
      Top             =   720
      Visible         =   0   'False
      Width           =   1215
   End
   Begin VB.Label message 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "message"
      ForeColor       =   &H80000008&
      Height          =   240
      Left            =   120
      TabIndex        =   3
      Top             =   0
      Width           =   3855
   End
   Begin VB.Label status 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "status"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   6000
      TabIndex        =   2
      Top             =   0
      Width           =   1575
   End
   Begin VB.Label field_label 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "field_label"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   0
      Left            =   1080
      TabIndex        =   1
      Top             =   360
      Visible         =   0   'False
      Width           =   975
   End
   Begin VB.Menu menu_file 
      Caption         =   "&File"
      Begin VB.Menu menu_profile 
         Caption         =   "Edit &Profile..."
      End
      Begin VB.Menu menu_line1 
         Caption         =   "-"
      End
      Begin VB.Menu menu_file_closethis 
         Caption         =   "&Close this"
      End
      Begin VB.Menu menu_line2 
         Caption         =   "-"
      End
      Begin VB.Menu menu_file_exitall 
         Caption         =   "E&xit all"
      End
      Begin VB.Menu menu_die 
         Caption         =   "Die"
      End
   End
   Begin VB.Menu menu_run 
      Caption         =   "&Run!"
   End
End
Attribute VB_Name = "paramform"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Private Sub field_input_DragDrop(Index As Integer, source As Control, X As Single, Y As Single)
  On Error GoTo error_handler
  Call param_drop(Me, Index, source, X, Y)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub field_input_KeyPress(Index As Integer, KeyAscii As Integer)
  On Error GoTo error_handler
  Call param_keypress(Me, Index, KeyAscii)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_die_Click()
  On Error GoTo error_handler
  Call param_die(Me)
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

Private Sub Form_KeyPress(KeyAscii As Integer)
  On Error GoTo error_handler
  Call param_keypress(Me, -1, KeyAscii)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub Form_Unload(cancel As Integer)
  On Error GoTo error_handler
  Call param_form_unload(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub keyinput_KeyDown(keycode As Integer, shift As Integer)
  On Error GoTo error_handler
  Call param_key(Me, keycode, shift)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_file_closethis_Click()
  On Error GoTo error_handler
  Unload Me
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_file_exitall_Click()
  On Error GoTo error_handler
  Call util_exit_all
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_run_click()
  On Error GoTo error_handler
  Call param_run(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub thetimer_Timer()
  On Error GoTo error_handler
  Call param_tick(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

