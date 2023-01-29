VERSION 2.00
Begin Form outputform 
   Caption         =   "Dtack Output"
   ClientHeight    =   2670
   ClientLeft      =   270
   ClientTop       =   5775
   ClientWidth     =   11475
   Height          =   3360
   Left            =   210
   LinkTopic       =   "Form1"
   ScaleHeight     =   2670
   ScaleWidth      =   11475
   Top             =   5145
   Width           =   11595
   Begin TextBox output 
      FontBold        =   -1  'True
      FontItalic      =   0   'False
      FontName        =   "System"
      FontSize        =   9.75
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   2400
      Left            =   0
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Top             =   285
      Width           =   7335
   End
   Begin TextBox status 
      BorderStyle     =   0  'None
      Height          =   285
      Left            =   2535
      TabIndex        =   3
      TabStop         =   0   'False
      Top             =   15
      Width           =   4785
   End
   Begin ListBox feedback 
      Height          =   1005
      Left            =   240
      TabIndex        =   1
      Top             =   2520
      Visible         =   0   'False
      Width           =   7335
   End
   Begin Label twirl_count 
      BackColor       =   &H0000C0C0&
      Caption         =   "0"
      Height          =   255
      Left            =   9120
      TabIndex        =   16
      Top             =   1080
      Width           =   495
   End
   Begin Label Label2 
      Caption         =   "Hinstance"
      Height          =   255
      Index           =   5
      Left            =   7560
      TabIndex        =   15
      Top             =   360
      Width           =   1455
   End
   Begin Label Label2 
      Caption         =   "Htask"
      Height          =   255
      Index           =   4
      Left            =   7560
      TabIndex        =   14
      Top             =   720
      Width           =   1455
   End
   Begin Label Label2 
      Caption         =   "Twirl count"
      Height          =   255
      Index           =   3
      Left            =   7560
      TabIndex        =   13
      Top             =   1080
      Width           =   1455
   End
   Begin Label Label2 
      Caption         =   "Flags"
      Height          =   255
      Index           =   2
      Left            =   7560
      TabIndex        =   12
      Top             =   1800
      Width           =   1455
   End
   Begin Label Label2 
      Caption         =   "After"
      Height          =   255
      Index           =   1
      Left            =   7560
      TabIndex        =   11
      Top             =   1440
      Width           =   1455
   End
   Begin Label Label2 
      Caption         =   "Parent"
      Height          =   255
      Index           =   0
      Left            =   7560
      TabIndex        =   10
      Top             =   0
      Width           =   1455
   End
   Begin Label flags 
      BackColor       =   &H0000C0C0&
      Height          =   255
      Left            =   9120
      TabIndex        =   9
      Top             =   1800
      Width           =   2175
   End
   Begin Label after 
      BackColor       =   &H0000C0C0&
      Height          =   255
      Left            =   9120
      TabIndex        =   8
      Top             =   1440
      Width           =   2175
   End
   Begin Label htask 
      BackColor       =   &H0000C0C0&
      Height          =   255
      Left            =   9120
      TabIndex        =   7
      Top             =   720
      Width           =   735
   End
   Begin Label hinstance 
      BackColor       =   &H0000C0C0&
      Height          =   255
      Left            =   9120
      TabIndex        =   6
      Top             =   360
      Width           =   735
   End
   Begin Label Label1 
      Caption         =   "Status:"
      Height          =   255
      Left            =   1860
      TabIndex        =   5
      Top             =   15
      Width           =   660
   End
   Begin Image twirl_all 
      Height          =   300
      Left            =   9000
      Picture         =   OUTPUT.FRX:0000
      Top             =   2400
      Width           =   300
   End
   Begin Image animate 
      Height          =   285
      Left            =   -15
      Top             =   0
      Width           =   390
   End
   Begin Image twirl 
      Height          =   300
      Index           =   1
      Left            =   7920
      Picture         =   OUTPUT.FRX:016A
      Top             =   2400
      Visible         =   0   'False
      Width           =   300
   End
   Begin Image twirl 
      Height          =   300
      Index           =   2
      Left            =   8280
      Picture         =   OUTPUT.FRX:02D4
      Top             =   2400
      Visible         =   0   'False
      Width           =   300
   End
   Begin Image twirl 
      Height          =   300
      Index           =   3
      Left            =   8640
      Picture         =   OUTPUT.FRX:043E
      Top             =   2400
      Visible         =   0   'False
      Width           =   300
   End
   Begin Image twirl 
      Height          =   300
      Index           =   0
      Left            =   7560
      Picture         =   OUTPUT.FRX:05A8
      Top             =   2400
      Visible         =   0   'False
      Width           =   300
   End
   Begin Label parent 
      BackColor       =   &H0000C0C0&
      Height          =   255
      Left            =   9120
      TabIndex        =   2
      Top             =   0
      Width           =   735
   End
   Begin Menu menu_file 
      Caption         =   "&File"
      Begin Menu menu_file_saveas 
         Caption         =   "&Save as..."
      End
      Begin Menu menu_line1 
         Caption         =   "-"
      End
      Begin Menu menu_file_closethis 
         Caption         =   "&Close this"
      End
      Begin Menu menu_line2 
         Caption         =   "-"
      End
      Begin Menu menu_file_exitall 
         Caption         =   "E&xit all"
      End
   End
   Begin Menu menu_process 
      Caption         =   "&Process"
      Begin Menu menu_process_pause 
         Caption         =   "&Pause"
         Enabled         =   0   'False
      End
      Begin Menu menu_process_resume 
         Caption         =   "&Resume"
         Enabled         =   0   'False
      End
      Begin Menu menu_process_stop 
         Caption         =   "&Stop"
         Enabled         =   0   'False
      End
      Begin Menu menu_process_stopandclosethis 
         Caption         =   "Stop and &close this"
      End
   End
   Begin Menu menu_close 
      Caption         =   "&Close"
   End
End
Option Explicit

Sub menu_close_Click ()
  Call output_exit(Me)
End Sub

Sub menu_file_closethis_Click ()
  Call output_exit(Me)
End Sub

Sub menu_file_exitall_Click ()
  Call exit_all
End Sub

Sub menu_process_pause_Click ()
  Call output_pause(Me)
End Sub

Sub menu_process_resume_Click ()
  Call output_resume(Me)
End Sub

Sub menu_process_stop_Click ()
  Call output_stop(Me)
End Sub

Sub menu_process_stopandclosethis_Click ()
  Call output_exit(Me)
End Sub

Sub output_KeyDown (keycode As Integer, shift As Integer)
  Call output_key(Me, keycode, shift)
End Sub

Sub status_Click ()
  Call output_twirl(Me)
End Sub

