VERSION 4.00
Begin VB.Form outputform 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   Caption         =   "outputform"
   ClientHeight    =   2685
   ClientLeft      =   2205
   ClientTop       =   4470
   ClientWidth     =   7350
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
   Height          =   3375
   Icon            =   "OUTPUT.frx":0000
   Left            =   2145
   LinkTopic       =   "Form1"
   ScaleHeight     =   2685
   ScaleWidth      =   7350
   Top             =   3840
   Width           =   7470
   Begin VB.Timer thetimer 
      Left            =   360
      Top             =   0
   End
   Begin VB.TextBox output 
      Appearance      =   0  'Flat
      BeginProperty Font 
         name            =   "Courier New"
         charset         =   0
         weight          =   400
         size            =   9
         underline       =   0   'False
         italic          =   0   'False
         strikethrough   =   0   'False
      EndProperty
      Height          =   2400
      Left            =   0
      MaxLength       =   30000
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Top             =   285
      Width           =   7335
   End
   Begin VB.TextBox status 
      Appearance      =   0  'Flat
      BorderStyle     =   0  'None
      Height          =   285
      Left            =   2535
      MultiLine       =   -1  'True
      TabIndex        =   3
      TabStop         =   0   'False
      Top             =   15
      Width           =   4785
   End
   Begin VB.ListBox feedback 
      Appearance      =   0  'Flat
      Height          =   1005
      Left            =   240
      TabIndex        =   1
      Top             =   2640
      Visible         =   0   'False
      Width           =   7335
   End
   Begin VB.Label procid 
      BackColor       =   &H0000C0C0&
      Height          =   255
      Left            =   8760
      TabIndex        =   19
      Top             =   1440
      Width           =   975
   End
   Begin VB.Label Label4 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Procid"
      Height          =   255
      Left            =   7560
      TabIndex        =   18
      Top             =   1440
      Width           =   1095
   End
   Begin VB.Label subcommand 
      BackColor       =   &H0000C0C0&
      Height          =   255
      Left            =   8760
      TabIndex        =   17
      Top             =   720
      Width           =   975
   End
   Begin VB.Label Label3 
      BackColor       =   &H00FFFFFF&
      Caption         =   "Subcommand"
      Height          =   255
      Left            =   7560
      TabIndex        =   16
      Top             =   720
      Width           =   1095
   End
   Begin VB.Label twirl_count 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      Caption         =   "0"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   8760
      TabIndex        =   4
      Top             =   1800
      Width           =   975
   End
   Begin VB.Label label6 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Pname"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   5
      Left            =   7560
      TabIndex        =   15
      Top             =   360
      Width           =   1095
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Procwin"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   4
      Left            =   7560
      TabIndex        =   14
      Top             =   1080
      Width           =   1095
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Twirl count"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   3
      Left            =   7560
      TabIndex        =   13
      Top             =   1800
      Width           =   1095
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Flags"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   2
      Left            =   7560
      TabIndex        =   12
      Top             =   2520
      Width           =   1095
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "After"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   1
      Left            =   7560
      TabIndex        =   11
      Top             =   2160
      Width           =   1095
   End
   Begin VB.Label Label2 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Parent"
      ForeColor       =   &H80000008&
      Height          =   255
      Index           =   0
      Left            =   7560
      TabIndex        =   10
      Top             =   0
      Width           =   1095
   End
   Begin VB.Label flags 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   8760
      TabIndex        =   9
      Top             =   2520
      Width           =   2175
   End
   Begin VB.Label after 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   8760
      TabIndex        =   8
      Top             =   2160
      Width           =   2175
   End
   Begin VB.Label procwin 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   8760
      TabIndex        =   7
      Top             =   1080
      Width           =   975
   End
   Begin VB.Label pname 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   8760
      TabIndex        =   6
      Top             =   360
      Width           =   975
   End
   Begin VB.Label Label1 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "Status:"
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   1860
      TabIndex        =   5
      Top             =   15
      Width           =   660
   End
   Begin VB.Image twirl_all 
      Appearance      =   0  'Flat
      Height          =   300
      Left            =   9000
      Picture         =   "OUTPUT.frx":030A
      Top             =   2880
      Width           =   300
   End
   Begin VB.Image animate 
      Appearance      =   0  'Flat
      Height          =   285
      Left            =   -15
      Top             =   0
      Width           =   390
   End
   Begin VB.Image twirl 
      Appearance      =   0  'Flat
      Height          =   300
      Index           =   1
      Left            =   7920
      Picture         =   "OUTPUT.frx":047C
      Top             =   2880
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image twirl 
      Appearance      =   0  'Flat
      Height          =   300
      Index           =   2
      Left            =   8280
      Picture         =   "OUTPUT.frx":05EE
      Top             =   2880
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image twirl 
      Appearance      =   0  'Flat
      Height          =   300
      Index           =   3
      Left            =   8640
      Picture         =   "OUTPUT.frx":0760
      Top             =   2880
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Image twirl 
      Appearance      =   0  'Flat
      Height          =   300
      Index           =   0
      Left            =   7560
      Picture         =   "OUTPUT.frx":08D2
      Top             =   2880
      Visible         =   0   'False
      Width           =   300
   End
   Begin VB.Label parent 
      Appearance      =   0  'Flat
      BackColor       =   &H0000C0C0&
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   8760
      TabIndex        =   2
      Top             =   0
      Width           =   975
   End
   Begin VB.Menu menu_file 
      Caption         =   "&File"
      Begin VB.Menu menu_file_closethis 
         Caption         =   "&Close this"
      End
      Begin VB.Menu menu_line2 
         Caption         =   "-"
      End
      Begin VB.Menu menu_file_exitall 
         Caption         =   "E&xit all"
      End
   End
   Begin VB.Menu menu_process 
      Caption         =   "&Process"
      Begin VB.Menu menu_process_pause 
         Caption         =   "&Pause"
         Enabled         =   0   'False
      End
      Begin VB.Menu menu_process_resume 
         Caption         =   "&Resume"
         Enabled         =   0   'False
      End
      Begin VB.Menu menu_process_stop 
         Caption         =   "&Stop"
         Enabled         =   0   'False
      End
      Begin VB.Menu menu_process_stopandclosethis 
         Caption         =   "Stop and &close this"
      End
   End
   Begin VB.Menu menu_pause 
      Caption         =   "P&ause!"
   End
   Begin VB.Menu menu_resume 
      Caption         =   "&Resume!"
   End
   Begin VB.Menu menu_stop 
      Caption         =   "&Stop!"
   End
   Begin VB.Menu menu_close 
      Caption         =   "&Close!"
   End
End
Attribute VB_Name = "outputform"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Private Sub menu_close_Click()
  On Error GoTo error_handler
  Call output_exit(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_file_closethis_Click()
  On Error GoTo error_handler
  Call output_exit(Me)
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

Private Sub menu_process_pause_Click()
  On Error GoTo error_handler
  Call output_pause(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_process_resume_Click()
  On Error GoTo error_handler
  Call output_resume(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_process_stop_Click()
  On Error GoTo error_handler
  Call output_stop(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_pause_Click()
  On Error GoTo error_handler
  Call output_pause(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_resume_Click()
  On Error GoTo error_handler
  Call output_resume(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_stop_Click()
  On Error GoTo error_handler
  Call output_stop(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub menu_process_stopandclosethis_Click()
  On Error GoTo error_handler
  Call output_exit(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub output_KeyDown(keycode As Integer, shift As Integer)
  On Error GoTo error_handler
  Call output_key(Me, keycode, shift)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub status_Click()
  On Error GoTo error_handler
  Call output_twirl(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub

Private Sub thetimer_Timer()
  On Error GoTo error_handler
  Call output_tick(Me)
  Exit Sub
error_handler:
  Call util_error_log(Err.number, _
    Err.source, Err.description)
End Sub
