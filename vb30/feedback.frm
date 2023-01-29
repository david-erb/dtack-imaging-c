VERSION 2.00
Begin Form feedback 
   Caption         =   "DT Feedback"
   ClientHeight    =   1590
   ClientLeft      =   1845
   ClientTop       =   6420
   ClientWidth     =   7695
   Height          =   1995
   Left            =   1785
   LinkTopic       =   "Form1"
   ScaleHeight     =   1590
   ScaleWidth      =   7695
   Top             =   6075
   Visible         =   0   'False
   Width           =   7815
   Begin ListBox box 
      Height          =   225
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   855
   End
End

Sub Form_Resize ()
  If Me.WindowState <> 1 Then
    box.Width = feedback.Width - 100
    box.Height = feedback.Height - 700
  End If
End Sub

