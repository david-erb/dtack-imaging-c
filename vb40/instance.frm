VERSION 4.00
Begin VB.Form instanceform 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   Caption         =   "Dtack Instances"
   ClientHeight    =   3240
   ClientLeft      =   1080
   ClientTop       =   1770
   ClientWidth     =   7320
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
   Height          =   3930
   Left            =   1020
   LinkTopic       =   "Form1"
   ScaleHeight     =   3240
   ScaleWidth      =   7320
   Top             =   1140
   Width           =   7440
   Begin VB.ListBox statuses 
      Height          =   2985
      Left            =   2400
      TabIndex        =   2
      Top             =   0
      Width           =   4815
   End
   Begin VB.ListBox pnames 
      Height          =   2985
      Left            =   1200
      TabIndex        =   1
      Top             =   0
      Width           =   1095
   End
   Begin VB.ListBox procwins 
      Height          =   2985
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   1095
   End
   Begin VB.Menu file 
      Caption         =   "&File"
      Begin VB.Menu menu_file_closethis 
         Caption         =   "&Close this"
      End
      Begin VB.Menu menu_file_exitall 
         Caption         =   "E&xit all"
      End
   End
   Begin VB.Menu menu_refresh 
      Caption         =   "&Refresh"
   End
End
Attribute VB_Name = "instanceform"
Attribute VB_Creatable = False
Attribute VB_Exposed = False

Private Sub menu_file_closethis_Click()
  Call instance_exit(Me)
End Sub

Private Sub menu_file_exitall_Click()
  Call exit_all
End Sub

Private Sub menu_refresh_Click()
  Call instance_refresh(Me)
End Sub
