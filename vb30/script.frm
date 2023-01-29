VERSION 2.00
Begin Form scriptform 
   Caption         =   "Dtack Script"
   ClientHeight    =   3690
   ClientLeft      =   90
   ClientTop       =   1740
   ClientWidth     =   11280
   Height          =   4380
   Left            =   30
   LinkTopic       =   "Form1"
   ScaleHeight     =   3690
   ScaleWidth      =   11280
   Top             =   1110
   Width           =   11400
   Begin TextBox argline 
      Height          =   285
      Left            =   8520
      TabIndex        =   15
      Top             =   3000
      Width           =   2415
   End
   Begin TextBox keyinput 
      Height          =   285
      Left            =   8520
      TabIndex        =   3
      Top             =   2520
      Width           =   2415
   End
   Begin Timer thetimer 
      Left            =   6840
      Top             =   0
   End
   Begin ListBox done 
      Height          =   3345
      Left            =   0
      TabIndex        =   1
      Top             =   360
      Width           =   7350
   End
   Begin Label Label8 
      Caption         =   "Flags"
      Height          =   255
      Left            =   7440
      TabIndex        =   17
      Top             =   1920
      Width           =   1095
   End
   Begin Label flags 
      BackColor       =   &H0000C0C0&
      Height          =   300
      Left            =   8520
      TabIndex        =   16
      Top             =   1800
      Width           =   2685
   End
   Begin Label Label7 
      Caption         =   "Arg line:"
      Height          =   255
      Left            =   7440
      TabIndex        =   14
      Top             =   3000
      Width           =   1095
   End
   Begin Label outfile 
      BackColor       =   &H0000C0C0&
      Height          =   300
      Left            =   8520
      TabIndex        =   13
      Top             =   0
      Width           =   2685
   End
   Begin Label Label5 
      Caption         =   "Output file"
      Height          =   255
      Left            =   7440
      TabIndex        =   12
      Top             =   120
      Width           =   1095
   End
   Begin Label Label4 
      Caption         =   "Key input:"
      Height          =   255
      Left            =   7440
      TabIndex        =   8
      Top             =   2640
      Width           =   1095
   End
   Begin Label Label6 
      Caption         =   "Common:"
      Height          =   255
      Left            =   7440
      TabIndex        =   7
      Top             =   1560
      Width           =   1095
   End
   Begin Label common 
      BackColor       =   &H0000C0C0&
      Height          =   300
      Left            =   8520
      TabIndex        =   11
      Top             =   1440
      Width           =   2685
   End
   Begin Label env 
      BackColor       =   &H0000C0C0&
      Height          =   300
      Left            =   8520
      TabIndex        =   10
      Top             =   1080
      Width           =   2685
   End
   Begin Label scriptname 
      BackColor       =   &H0000C0C0&
      Height          =   300
      Left            =   8520
      TabIndex        =   9
      Top             =   720
      Width           =   2685
   End
   Begin Label Label3 
      Caption         =   "Env:"
      Height          =   255
      Left            =   7440
      TabIndex        =   6
      Top             =   1200
      Width           =   1095
   End
   Begin Label Label2 
      Caption         =   "Scriptname:"
      Height          =   255
      Left            =   7440
      TabIndex        =   5
      Top             =   840
      Width           =   1095
   End
   Begin Label Label1 
      Caption         =   "Executable:"
      Height          =   255
      Left            =   7440
      TabIndex        =   4
      Top             =   480
      Width           =   1095
   End
   Begin Label executable 
      BackColor       =   &H0000C0C0&
      Height          =   300
      Left            =   8520
      TabIndex        =   2
      Top             =   360
      Width           =   2685
   End
   Begin Label doing 
      Caption         =   "doing"
      Height          =   255
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   4575
   End
   Begin Menu menu_file 
      Caption         =   "&File"
      Begin Menu menu_file_closethis 
         Caption         =   "&Close this"
      End
      Begin Menu menu_line1 
         Caption         =   "-"
      End
      Begin Menu menu_file_exitall 
         Caption         =   "E&xit all"
      End
   End
End
Option Explicit

Sub keyinput_KeyDown (keycode As Integer, shift As Integer)
  Call script_key(Me, keycode, shift)
End Sub

Sub menu_file_closethis_Click ()
  Unload Me
End Sub

Sub menu_file_exitall_Click ()
  Call exit_all
End Sub

Sub thetimer_Timer ()
  Call script_tick(Me)
End Sub

