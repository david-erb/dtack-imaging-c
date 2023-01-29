VERSION 2.00
Begin Form profileform 
   Caption         =   "Dtack Profile"
   ClientHeight    =   3840
   ClientLeft      =   6345
   ClientTop       =   2325
   ClientWidth     =   5550
   Height          =   4530
   Left            =   6285
   LinkTopic       =   "Form1"
   ScaleHeight     =   3840
   ScaleWidth      =   5550
   Tag             =   "5"
   Top             =   1695
   Width           =   5670
   Begin TextBox fields 
      Height          =   285
      Index           =   8
      Left            =   2280
      TabIndex        =   20
      Tag             =   "dllpath"
      Top             =   480
      Width           =   3135
   End
   Begin TextBox inipath 
      Height          =   285
      Left            =   2280
      TabIndex        =   18
      Top             =   120
      Width           =   3135
   End
   Begin TextBox fields 
      Height          =   285
      Index           =   7
      Left            =   2280
      TabIndex        =   16
      Tag             =   "D"
      Top             =   1200
      Width           =   3135
   End
   Begin TextBox fields 
      Height          =   285
      Index           =   6
      Left            =   4680
      TabIndex        =   11
      Tag             =   "yborder"
      Top             =   3000
      Width           =   735
   End
   Begin TextBox fields 
      Height          =   285
      Index           =   5
      Left            =   3000
      TabIndex        =   10
      Tag             =   "xborder"
      Top             =   3000
      Width           =   735
   End
   Begin TextBox fields 
      Height          =   285
      Index           =   4
      Left            =   2280
      TabIndex        =   8
      Tag             =   "datadir"
      Top             =   2640
      Width           =   3135
   End
   Begin TextBox fields 
      Height          =   285
      Index           =   3
      Left            =   2280
      TabIndex        =   7
      Tag             =   "scriptpath"
      Top             =   840
      Width           =   3135
   End
   Begin TextBox fields 
      Height          =   285
      Index           =   2
      Left            =   2280
      TabIndex        =   5
      Tag             =   "sayto"
      Top             =   2280
      Width           =   3135
   End
   Begin TextBox fields 
      Height          =   285
      Index           =   1
      Left            =   2280
      TabIndex        =   4
      Tag             =   "errto"
      Top             =   1920
      Width           =   3135
   End
   Begin TextBox fields 
      Height          =   285
      Index           =   0
      Left            =   2280
      TabIndex        =   3
      Tag             =   "dbgto"
      Top             =   1560
      Width           =   3135
   End
   Begin Label Label6 
      Caption         =   "Path to scripts"
      Height          =   255
      Left            =   120
      TabIndex        =   19
      Top             =   840
      Width           =   2055
   End
   Begin Label Label5 
      Caption         =   "Path to .INI files"
      Height          =   255
      Left            =   120
      TabIndex        =   17
      Top             =   120
      Width           =   2055
   End
   Begin Label Label4 
      Caption         =   "Debug mask"
      Height          =   255
      Left            =   120
      TabIndex        =   15
      Top             =   1200
      Width           =   2055
   End
   Begin Label Label3 
      Caption         =   "Y"
      Height          =   255
      Left            =   4320
      TabIndex        =   14
      Top             =   3000
      Width           =   255
   End
   Begin Label Label2 
      Caption         =   "X"
      Height          =   255
      Left            =   2640
      TabIndex        =   13
      Top             =   3000
      Width           =   255
   End
   Begin Label Label1 
      Caption         =   "Image window border"
      Height          =   255
      Left            =   120
      TabIndex        =   12
      Top             =   3000
      Width           =   2055
   End
   Begin Label datadir_label 
      Caption         =   "Data directory"
      Height          =   255
      Left            =   120
      TabIndex        =   9
      Top             =   2640
      Width           =   2055
   End
   Begin Label path_label 
      Caption         =   "Path to DLLs and EXEs"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   480
      Width           =   2055
   End
   Begin Label sayto_label 
      Caption         =   "Informational outout to"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   2280
      Width           =   2055
   End
   Begin Label errto_label 
      Caption         =   "Error output to"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   1920
      Width           =   2055
   End
   Begin Label dbgto_label 
      Caption         =   "Debug output to"
      Height          =   255
      Left            =   120
      TabIndex        =   0
      Top             =   1560
      Width           =   2055
   End
   Begin Menu menu_file 
      Caption         =   "&File"
      Begin Menu menu_save 
         Caption         =   "&Save profile"
      End
      Begin Menu menu_file_closethis 
         Caption         =   "&Close this"
      End
      Begin Menu menu_file_exitall 
         Caption         =   "&Exit All"
      End
   End
End
Option Explicit

Sub Form_Load ()
  Call profile_globals_load(Me)
End Sub

Sub menu_file_closethis_Click ()
  Me.Hide
End Sub

Sub menu_file_exitall_Click ()
  Call exit_all
End Sub

Sub menu_save_Click ()
  Call profile_globals_save(Me)
End Sub

