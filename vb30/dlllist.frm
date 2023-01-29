VERSION 2.00
Begin Form dlllistform 
   Caption         =   "Dtack DLL List"
   ClientHeight    =   2160
   ClientLeft      =   6645
   ClientTop       =   2280
   ClientWidth     =   5145
   Height          =   2850
   Left            =   6585
   LinkTopic       =   "Form1"
   ScaleHeight     =   2160
   ScaleWidth      =   5145
   Top             =   1650
   Width           =   5265
   Begin ListBox handlelist 
      Height          =   2175
      Left            =   4440
      TabIndex        =   3
      Top             =   0
      Width           =   735
   End
   Begin ListBox linklist 
      Height          =   2175
      Left            =   4080
      TabIndex        =   2
      Top             =   0
      Width           =   375
   End
   Begin ListBox namelist 
      Height          =   225
      Left            =   0
      TabIndex        =   1
      Top             =   4440
      Visible         =   0   'False
      Width           =   2295
   End
   Begin ListBox pathlist 
      Height          =   2175
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   4095
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
   Begin Menu menu_refresh 
      Caption         =   "Re&fresh!"
   End
   Begin Menu menu_reload 
      Caption         =   "Re&load!"
   End
   Begin Menu menu_unload 
      Caption         =   "&Unload!"
   End
End
Sub menu_file_closethis_Click ()
  Me.Hide
End Sub

Sub menu_file_exitall_Click ()
  Call exit_all
End Sub

Sub menu_refresh_Click ()
  Call dlllist_usage_all(Me)
End Sub

Sub menu_reload_Click ()
  Call dlllist_reload_all(Me)
End Sub

Sub menu_unload_Click ()
  Call dlllist_unload_all(Me)
End Sub

