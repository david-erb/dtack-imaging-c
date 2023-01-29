VERSION 2.00
Begin Form masterform 
   Caption         =   "Dtack Program Selection"
   ClientHeight    =   960
   ClientLeft      =   7950
   ClientTop       =   675
   ClientWidth     =   3960
   Height          =   1650
   Icon            =   MASTER.FRX:0000
   Left            =   7890
   LinkTopic       =   "Form1"
   ScaleHeight     =   960
   ScaleWidth      =   3960
   Top             =   45
   Width           =   4080
   Begin CommandButton classbutton 
      Caption         =   "class"
      Height          =   375
      Index           =   0
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   615
   End
   Begin ListBox worklist 
      Height          =   810
      Left            =   120
      TabIndex        =   2
      Top             =   1080
      Width           =   3615
   End
   Begin ComboBox programlist 
      Height          =   300
      Left            =   960
      Sorted          =   -1  'True
      TabIndex        =   0
      Top             =   600
      Width           =   2895
   End
   Begin Label pclass 
      Height          =   255
      Left            =   240
      TabIndex        =   3
      Top             =   600
      Width           =   615
   End
   Begin Image Image1 
      Height          =   480
      Left            =   3180
      Picture         =   MASTER.FRX:0302
      Top             =   -30
      Width           =   480
   End
   Begin Menu menu_file 
      Caption         =   "&File"
      Begin Menu menu_profile 
         Caption         =   "Edit &Profile..."
      End
      Begin Menu line1 
         Caption         =   "-"
      End
      Begin Menu menu_file_newimage 
         Caption         =   "&New image..."
      End
      Begin Menu line2 
         Caption         =   "-"
      End
      Begin Menu menu_file_exitall 
         Caption         =   "E&xit All"
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
   Begin Menu menu_debug 
      Caption         =   "&Window"
      Begin Menu menu_window_images 
         Caption         =   "&Images"
      End
      Begin Menu menu_window_instances 
         Caption         =   "&Instances"
      End
      Begin Menu menu_window_dlls 
         Caption         =   "&DLLs"
      End
   End
End
Option Explicit

Sub classbutton_Click (index As Integer)
  Call master_button_click(Me, index)
End Sub

Sub menu_file_newimage_Click ()
  Dim s As String
  s = image_new(winlistform)
End Sub

Sub Form_Load ()
  Call master_load(Me)
End Sub

Sub menu_file_exitall_Click ()
  End
End Sub

Sub menu_image_createempty_Click ()
  Call image_virgin(winlistform)
End Sub

Sub menu_profile_Click ()
  profileform.Show
End Sub

Sub menu_run_click ()
  Dim program_name As String
  program_name = programlist.Text
  Call master_run(Me, program_name)
End Sub

Sub menu_window_dlls_Click ()
  Call dlllist_usage_all(dlllistform)
End Sub

Sub menu_window_images_Click ()
  Call winlist_bringup
End Sub

Sub menu_window_instances_Click ()
  Call instance_refresh(instanceform)
End Sub

