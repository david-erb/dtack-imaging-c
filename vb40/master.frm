VERSION 4.00
Begin VB.Form masterform 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   Caption         =   "Dtack Program Selection"
   ClientHeight    =   1020
   ClientLeft      =   7200
   ClientTop       =   585
   ClientWidth     =   4740
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
   Height          =   1710
   Icon            =   "MASTER.frx":0000
   Left            =   7140
   LinkTopic       =   "Form1"
   ScaleHeight     =   1020
   ScaleWidth      =   4740
   Top             =   -45
   Width           =   4860
   Begin VB.CommandButton classbutton 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      Caption         =   "class"
      Height          =   375
      Index           =   0
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   615
   End
   Begin VB.ListBox worklist 
      Appearance      =   0  'Flat
      Height          =   810
      Left            =   120
      TabIndex        =   2
      Top             =   1080
      Width           =   3615
   End
   Begin VB.ComboBox programlist 
      Appearance      =   0  'Flat
      Height          =   315
      Left            =   960
      Sorted          =   -1  'True
      TabIndex        =   0
      Top             =   600
      Width           =   2895
   End
   Begin VB.Label pclass 
      Appearance      =   0  'Flat
      BackColor       =   &H80000005&
      ForeColor       =   &H80000008&
      Height          =   255
      Left            =   240
      TabIndex        =   3
      Top             =   600
      Width           =   615
   End
   Begin VB.Image Image1 
      Appearance      =   0  'Flat
      Height          =   480
      Left            =   3180
      Top             =   -30
      Width           =   480
   End
   Begin VB.Menu menu_file 
      Caption         =   "&File"
      Begin VB.Menu menu_profile 
         Caption         =   "Edit &Profile..."
      End
      Begin VB.Menu menu_line1 
         Caption         =   "-"
      End
      Begin VB.Menu menu_file_newimage 
         Caption         =   "&New image..."
      End
      Begin VB.Menu menu_line2 
         Caption         =   "-"
      End
      Begin VB.Menu menu_file_exitall 
         Caption         =   "E&xit All"
      End
   End
   Begin VB.Menu menu_run 
      Caption         =   "&Run!"
   End
   Begin VB.Menu menu_image 
      Caption         =   "&Image"
      Begin VB.Menu menu_image_createempty 
         Caption         =   "&Create empty"
      End
   End
   Begin VB.Menu menu_debug 
      Caption         =   "&Window"
      Begin VB.Menu menu_window_images 
         Caption         =   "&Images"
      End
      Begin VB.Menu menu_window_instances 
         Caption         =   "&Instances"
      End
      Begin VB.Menu menu_window_dlls 
         Caption         =   "&DLLs"
      End
   End
End
Attribute VB_Name = "masterform"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Private Sub classbutton_Click(Index As Integer)
  Call master_button_click(Me, Index)
End Sub

Private Sub menu_file_newimage_Click()
  Dim s As String
  s = image_new(winlistform)
End Sub

Private Sub Form_Load()
  Call master_load(Me)
End Sub

Private Sub menu_file_exitall_Click()
  End
End Sub

Private Sub menu_image_createempty_Click()
  Call image_virgin(winlistform)
End Sub

Private Sub menu_profile_Click()
  profileform.Show
End Sub

Private Sub menu_run_click()
  Dim program_name As String
  program_name = programlist.Text
  Call master_run(Me, program_name)
End Sub

Private Sub menu_window_dlls_Click()
  Call dlllist_usage_all(dlllistform)
End Sub

Private Sub menu_window_images_Click()
  Call winlist_bringup
End Sub

Private Sub menu_window_instances_Click()
  Load instanceform
  Call instance_refresh(instanceform)
End Sub

