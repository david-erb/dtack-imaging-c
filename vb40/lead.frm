VERSION 4.00
Begin VB.Form imageform 
   Appearance      =   0  'Flat
   BackColor       =   &H00C0E0FF&
   Caption         =   " "
   ClientHeight    =   1830
   ClientLeft      =   6585
   ClientTop       =   4980
   ClientWidth     =   2445
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
   Height          =   2520
   Left            =   6525
   LinkTopic       =   "Form1"
   ScaleHeight     =   1830
   ScaleWidth      =   2445
   Top             =   4350
   Width           =   2565
   Begin VB.Timer thetimer 
      Enabled         =   0   'False
      Left            =   1920
      Top             =   24
   End
   Begin VB.TextBox thewname 
      Appearance      =   0  'Flat
      Height          =   285
      Left            =   0
      TabIndex        =   0
      Text            =   "thewname"
      Top             =   0
      Width           =   975
   End
   Begin LeadLib.Lead theimage 
      Height          =   1095
      Left            =   120
      TabIndex        =   1
      Top             =   600
      Width           =   2175
      _Version        =   393216
      _ExtentX        =   3836
      _ExtentY        =   1931
      _StockProps     =   97
      BackColor       =   16761024
      ScaleHeight     =   73
      ScaleWidth      =   145
      DataField       =   ""
   End
   Begin MSComDlg.CommonDialog thedialog 
      Left            =   1440
      Top             =   0
      _Version        =   65536
      _ExtentX        =   847
      _ExtentY        =   847
      _StockProps     =   0
   End
   Begin VB.Menu menu_file 
      Caption         =   "&File"
      Begin VB.Menu menu_file_openimage 
         Caption         =   "&Open image..."
      End
      Begin VB.Menu menu_file_newimage 
         Caption         =   "&New image..."
      End
      Begin VB.Menu menu_file_saveimageas 
         Caption         =   "&Save image as..."
      End
      Begin VB.Menu line1 
         Caption         =   "-"
      End
      Begin VB.Menu line2 
         Caption         =   "-"
      End
      Begin VB.Menu menu_file_systempallette 
         Caption         =   "&System pallette"
      End
      Begin VB.Menu line3 
         Caption         =   "-"
      End
      Begin VB.Menu menu_file_ownpallette 
         Caption         =   "&Own pallette"
      End
      Begin VB.Menu menu_file_ownpallette_5seconds 
         Caption         =   "O&wn pallete (5 seconds)"
      End
      Begin VB.Menu line4 
         Caption         =   "-"
      End
      Begin VB.Menu menu_file_grouppallette 
         Caption         =   "&Group pallette"
      End
      Begin VB.Menu menu_file_grouppallette_5seconds 
         Caption         =   "G&roup pallette (5 seconds)"
      End
      Begin VB.Menu line5 
         Caption         =   "-"
      End
      Begin VB.Menu line6 
         Caption         =   "-"
      End
      Begin VB.Menu menu_file_closeimage 
         Caption         =   "&Close image"
      End
      Begin VB.Menu menu_file_exitall 
         Caption         =   "E&xit all"
      End
   End
End
Attribute VB_Name = "imageform"
Attribute VB_Creatable = False
Attribute VB_Exposed = False



Option Explicit

Private Sub Form_Resize()
  Call image_clip(Me)
End Sub

Private Sub menu_file_closeimage_Click()
  Unload Me
End Sub

Private Sub menu_file_exitall_Click()
  End
End Sub

Private Sub menu_file_newimage_Click()
  Dim s As String
  s = image_new(winlistform)
End Sub

Private Sub menu_file_openimage_Click()
  Dim s As String
  s = image_open(Me, winlistform)
End Sub

Private Sub menu_file_saveimageas_Click()
  Call image_saveas(Me)
End Sub

Private Sub menu_file_grouppallette_5seconds_Click()
  Call pallette_set(Me, PALLETTE_GROUP, 5000)
End Sub

Private Sub menu_file_grouppallette_Click()
  Call pallette_set(Me, PALLETTE_GROUP, 0)
End Sub

Private Sub menu_file_ownpallette_5seconds_Click()
  Call pallette_set(Me, PALLETTE_OWN, 5000)
End Sub

Private Sub menu_file_ownpallette_Click()
  Call pallette_set(Me, PALLETTE_OWN, 0)
End Sub

Private Sub menu_file_systempallette_Click()
  Call pallette_set(Me, PALLETTE_SYSTEM, 0)
End Sub

Private Sub thetimer_Timer()
  Me.thetimer.Enabled = False
  Call pallette_set(Me, PALLETTE_SYSTEM, 0)
End Sub

Private Sub thewname_Change()
  Me.Tag = Me.thewname.Text
  Me.theimage.Tag = Me.thewname.Text
End Sub

