VERSION 4.00
Begin VB.Form winlistform 
   Appearance      =   0  'Flat
   BackColor       =   &H80000005&
   Caption         =   "Dtack Window List"
   ClientHeight    =   3540
   ClientLeft      =   45
   ClientTop       =   1470
   ClientWidth     =   11865
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
   Height          =   3945
   Left            =   -15
   LinkMode        =   1  'Source
   LinkTopic       =   "window"
   ScaleHeight     =   3540
   ScaleWidth      =   11865
   Top             =   1125
   Width           =   11985
   Begin VB.ListBox thelist 
      Appearance      =   0  'Flat
      Height          =   3540
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   11895
   End
End
Attribute VB_Name = "winlistform"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Option Explicit

Private Sub Form_LinkExecute(cmdstr As String, cancel As Integer)
 Call winlist_request(Me, cmdstr, cancel)
End Sub

