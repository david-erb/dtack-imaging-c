VERSION 2.00
Begin Form winlistform 
   Caption         =   "Dtack Window List"
   ClientHeight    =   3540
   ClientLeft      =   45
   ClientTop       =   1470
   ClientWidth     =   11865
   Height          =   3945
   Left            =   -15
   LinkMode        =   1  'Source
   LinkTopic       =   "window"
   ScaleHeight     =   3540
   ScaleWidth      =   11865
   Top             =   1125
   Width           =   11985
   Begin ListBox thelist 
      Height          =   3540
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   11895
   End
End
Option Explicit

Sub Form_LinkExecute (cmdstr As String, cancel As Integer)
 Call winlist_request(Me, cmdstr, cancel)
End Sub

