VERSION 4.00
Begin VB.Form restartform 
   Caption         =   "Tableau Restart"
   ClientHeight    =   960
   ClientLeft      =   3345
   ClientTop       =   1950
   ClientWidth     =   2565
   Height          =   1365
   Left            =   3285
   LinkTopic       =   "Form1"
   ScaleHeight     =   960
   ScaleWidth      =   2565
   Top             =   1605
   Width           =   2685
   Begin VB.Timer thetimer 
      Left            =   2760
      Top             =   360
   End
   Begin VB.CommandButton cancel 
      Caption         =   "Cancel"
      Height          =   375
      Left            =   1440
      TabIndex        =   3
      Top             =   480
      Width           =   735
   End
   Begin VB.CommandButton go 
      Caption         =   "Go"
      Height          =   375
      Left            =   360
      TabIndex        =   2
      Top             =   480
      Width           =   735
   End
   Begin VB.Label parent 
      BackColor       =   &H0000C0C0&
      Height          =   255
      Left            =   3720
      TabIndex        =   6
      Top             =   0
      Width           =   1455
   End
   Begin VB.Label Label3 
      Caption         =   "Parent"
      Height          =   255
      Left            =   2760
      TabIndex        =   5
      Top             =   0
      Width           =   855
   End
   Begin VB.Label Label2 
      Caption         =   "seconds"
      Height          =   255
      Left            =   1800
      TabIndex        =   4
      Top             =   120
      Width           =   735
   End
   Begin VB.Label Label1 
      Caption         =   "Time remaining"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   1095
   End
   Begin VB.Label remaining 
      Caption         =   "10"
      BeginProperty Font 
         name            =   "MS Sans Serif"
         charset         =   0
         weight          =   400
         size            =   13.5
         underline       =   0   'False
         italic          =   0   'False
         strikethrough   =   0   'False
      EndProperty
      Height          =   375
      Left            =   1320
      TabIndex        =   0
      Top             =   0
      Width           =   375
   End
End
Attribute VB_Name = "restartform"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Private Sub go_Click()
  Call restart_go(Me)
End Sub


Private Sub cancel_Click()
  Call restart_cancel(Me)
End Sub


Private Sub thetimer_Timer()
  Call restart_tick(Me)
End Sub


Private Sub Form_Load()
  Call restart_load(Me)
End Sub

