VERSION 2.00
Begin Form paramform 
   Caption         =   "paramform"
   ClientHeight    =   2880
   ClientLeft      =   90
   ClientTop       =   1710
   ClientWidth     =   11835
   Height          =   3570
   Left            =   30
   LinkTopic       =   "Form1"
   ScaleHeight     =   2880
   ScaleWidth      =   11835
   Top             =   1080
   Width           =   11955
   Begin TextBox argline 
      Height          =   285
      Left            =   9360
      TabIndex        =   10
      Top             =   360
      Width           =   2535
   End
   Begin TextBox keyinput 
      Height          =   285
      Left            =   9360
      TabIndex        =   8
      Top             =   0
      Width           =   2535
   End
   Begin Timer thetimer 
      Enabled         =   0   'False
      Left            =   120
      Top             =   240
   End
   Begin TextBox field_input 
      Height          =   285
      Index           =   0
      Left            =   1920
      TabIndex        =   0
      Top             =   2520
      Visible         =   0   'False
      Width           =   975
   End
   Begin Label Label2 
      Caption         =   "Argline"
      Height          =   255
      Left            =   8040
      TabIndex        =   12
      Top             =   360
      Width           =   1215
   End
   Begin Label Label1 
      Caption         =   "Key input"
      Height          =   255
      Left            =   8040
      TabIndex        =   11
      Top             =   0
      Width           =   1215
   End
   Begin Label executable 
      Caption         =   "executable"
      Height          =   255
      Left            =   5520
      TabIndex        =   9
      Top             =   2520
      Visible         =   0   'False
      Width           =   1215
   End
   Begin Label field_abbrev 
      Caption         =   "field_abbrev"
      Height          =   255
      Index           =   0
      Left            =   4320
      TabIndex        =   5
      Top             =   2520
      Visible         =   0   'False
      Width           =   1095
   End
   Begin Label field_count 
      Caption         =   "field_count"
      Height          =   255
      Left            =   3000
      TabIndex        =   7
      Top             =   2520
      Visible         =   0   'False
      Width           =   1095
   End
   Begin Label start 
      Caption         =   "start"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   2520
      Visible         =   0   'False
      Width           =   495
   End
   Begin Label pname 
      Caption         =   "pname"
      Height          =   255
      Left            =   6840
      TabIndex        =   4
      Top             =   2520
      Visible         =   0   'False
      Width           =   855
   End
   Begin Label message 
      Caption         =   "message"
      Height          =   240
      Left            =   120
      TabIndex        =   3
      Top             =   0
      Width           =   3855
   End
   Begin Label status 
      Caption         =   "status"
      Height          =   255
      Left            =   6000
      TabIndex        =   2
      Top             =   0
      Width           =   1575
   End
   Begin Label field_label 
      Caption         =   "field_label"
      Height          =   255
      Index           =   0
      Left            =   840
      TabIndex        =   1
      Top             =   2520
      Visible         =   0   'False
      Width           =   975
   End
   Begin Menu menu_file 
      Caption         =   "&File"
      Begin Menu menu_file_save 
         Caption         =   "&Save parameters"
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
   Begin Menu menu_run 
      Caption         =   "&Run!"
   End
   Begin Menu menu_image 
      Caption         =   "&Image"
      Begin Menu menu_image_createempty 
         Caption         =   "&Create empty"
      End
   End
End
Option Explicit

Sub field_input_DragDrop (index As Integer, source As Control, X As Single, Y As Single)
  If TypeOf source Is ISVB Then
    field_input(index).Text = ":" + source.Tag
  End If
End Sub

Sub Form_Unload (cancel As Integer)
  Call param_form_unload(Me)
End Sub

Sub keyinput_KeyDown (keycode As Integer, shift As Integer)
  Call param_key(Me, keycode, shift)
End Sub

Sub menu_file_closethis_Click ()
  Unload Me
End Sub

Sub menu_file_exitall_Click ()
  Call exit_all
End Sub

Sub menu_image_createempty_Click ()
  Call image_virgin(winlistform)
End Sub

Sub menu_run_click ()
  Call param_run(Me)
End Sub

Sub thetimer_Timer ()
  Call param_tick(Me)
End Sub

