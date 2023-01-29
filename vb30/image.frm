VERSION 2.00
Begin Form imageform 
   Caption         =   "Dtack Image"
   ClientHeight    =   1560
   ClientLeft      =   6585
   ClientTop       =   4980
   ClientWidth     =   2055
   Height          =   2250
   Left            =   6525
   LinkTopic       =   "Form1"
   ScaleHeight     =   1.083
   ScaleMode       =   5  'Inch
   ScaleWidth      =   1.427
   Top             =   4350
   Width           =   2175
   Begin Timer thetimer 
      Enabled         =   0   'False
      Left            =   1680
      Top             =   0
   End
   Begin TextBox thewname 
      Height          =   285
      Left            =   0
      TabIndex        =   0
      Text            =   "thewname"
      Top             =   0
      Width           =   975
   End
   Begin ISVB theimage 
      BackColor       =   &H00000000&
      ClipboardOpt    =   0  'MetaFile
      Display         =   -1  'True
      DragMode        =   1  'Automatic
      ErrorPrmt       =   0   'False
      FileOffset      =   0
      Height          =   1290
      Left            =   0
      Left_DClk       =   0  'None
      Left_ShiftDClk  =   0  'None
      MarginBottom    =   0
      MarginLeft      =   0
      MarginRight     =   0
      MarginTop       =   0
      PaletteMode     =   0  'None
      Preferences     =   -1  'True
      PrintSize       =   0  'Actual Size
      Right_DClk      =   0  'None
      Right_ShiftDClk =   0  'None
      Top             =   285
      ViewMode        =   0  'Actual Size
      Width           =   2055
      Zoom            =   100
   End
   Begin Menu menu_file 
      Caption         =   "&File"
      Begin Menu menu_file_openimage 
         Caption         =   "&Open image..."
      End
      Begin Menu menu_file_newimage 
         Caption         =   "&New image..."
      End
      Begin Menu menu_file_saveimageas 
         Caption         =   "&Save image as..."
      End
      Begin Menu line1 
         Caption         =   "-"
      End
      Begin Menu line2 
         Caption         =   "-"
      End
      Begin Menu menu_file_systempallette 
         Caption         =   "&System pallette"
      End
      Begin Menu line3 
         Caption         =   "-"
      End
      Begin Menu menu_file_ownpallette 
         Caption         =   "&Own pallette"
      End
      Begin Menu menu_file_ownpallette_5seconds 
         Caption         =   "O&wn pallete (5 seconds)"
      End
      Begin Menu line4 
         Caption         =   "-"
      End
      Begin Menu menu_file_grouppallette 
         Caption         =   "&Group pallette"
      End
      Begin Menu menu_file_grouppallette_5seconds 
         Caption         =   "G&roup pallette (5 seconds)"
      End
      Begin Menu line5 
         Caption         =   "-"
      End
      Begin Menu line6 
         Caption         =   "-"
      End
      Begin Menu menu_file_closeimage 
         Caption         =   "&Close image"
      End
      Begin Menu menu_file_exitall 
         Caption         =   "E&xit all"
      End
   End
End
Option Explicit

Sub Form_Resize ()
  Call image_clip(Me)
End Sub

Sub menu_file_closeimage_Click ()
  Unload Me
End Sub

Sub menu_file_exitall_Click ()
  End
End Sub

Sub menu_file_newimage_Click ()
  Dim s As String
  s = image_new(winlistform)
End Sub

Sub menu_file_openimage_Click ()
  Dim s As String
  s = image_open(Me, winlistform)
End Sub

Sub menu_file_saveimageas_Click ()
  Me.theimage.Preferences = True
  Me.theimage.SaveAs = True
End Sub

Sub menu_file_grouppallette_5seconds_Click ()
  Call pallette_set(Me, PALLETTE_GROUP, 5000)
End Sub

Sub menu_file_grouppallette_Click ()
  Call pallette_set(Me, PALLETTE_GROUP, 0)
End Sub

Sub menu_file_ownpallette_5seconds_Click ()
  Call pallette_set(Me, PALLETTE_OWN, 5000)
End Sub

Sub menu_file_ownpallette_Click ()
  Call pallette_set(Me, PALLETTE_OWN, 0)
End Sub

Sub menu_file_systempallette_Click ()
  Call pallette_set(Me, PALLETTE_SYSTEM, 0)
End Sub

Sub thetimer_Timer ()
  Me.thetimer.Enabled = False
  Call pallette_set(Me, PALLETTE_SYSTEM, 0)
End Sub

Sub thewname_Change ()
  Me.Tag = Me.thewname.Text
  Me.theimage.Tag = Me.thewname.Text
End Sub

