VERSION 4.00
Begin VB.Form dlllistform 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   Caption         =   "Dtack DLL List"
   ClientHeight    =   2160
   ClientLeft      =   6645
   ClientTop       =   2280
   ClientWidth     =   5145
   BeginProperty Font 
      name            =   "MS Sans Serif"
      charset         =   1
      weight          =   700
      size            =   8.25
      underline       =   0   'False
      italic          =   0   'False
      strikethrough   =   0   'False
   EndProperty
   ForeColor       =   &H80000008&
   Height          =   2850
   Left            =   6585
   LinkTopic       =   "Form1"
   ScaleHeight     =   2160
   ScaleWidth      =   5145
   Top             =   1650
   Width           =   5265
   Begin VB.ListBox handlelist 
      Appearance      =   0  'Flat
      Height          =   2175
      Left            =   4440
      TabIndex        =   3
      Top             =   0
      Width           =   735
   End
   Begin VB.ListBox linklist 
      Appearance      =   0  'Flat
      Height          =   2175
      Left            =   4080
      TabIndex        =   2
      Top             =   0
      Width           =   375
   End
   Begin VB.ListBox namelist 
      Appearance      =   0  'Flat
      Height          =   225
      Left            =   0
      TabIndex        =   1
      Top             =   4440
      Visible         =   0   'False
      Width           =   2295
   End
   Begin VB.ListBox pathlist 
      Appearance      =   0  'Flat
      Height          =   2175
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   4095
   End
   Begin VB.Menu menu_file 
      Caption         =   "&File"
      Begin VB.Menu menu_file_closethis 
         Caption         =   "&Close this"
      End
      Begin VB.Menu menu_line1 
         Caption         =   "-"
      End
      Begin VB.Menu menu_file_exitall 
         Caption         =   "E&xit all"
      End
   End
   Begin VB.Menu menu_refresh 
      Caption         =   "Re&fresh!"
   End
   Begin VB.Menu menu_reload 
      Caption         =   "Re&load!"
   End
   Begin VB.Menu menu_unload 
      Caption         =   "&Unload!"
   End
End
Attribute VB_Name = "dlllistform"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Private Sub menu_file_closethis_Click()
  Me.Hide
End Sub

Private Sub menu_file_exitall_Click()
  Call exit_all
End Sub

Private Sub menu_refresh_Click()
  Call dlllist_usage_all(Me)
End Sub

Private Sub menu_reload_Click()
  Call dlllist_reload_all(Me)
End Sub

Private Sub menu_unload_Click()
  Call dlllist_unload_all(Me)
End Sub

