VERSION 4.00
Begin VB.Form query_results 
   Caption         =   "Query Results"
   ClientHeight    =   2760
   ClientLeft      =   1140
   ClientTop       =   1515
   ClientWidth     =   6480
   Height          =   3165
   Left            =   1080
   LinkTopic       =   "Form1"
   ScaleHeight     =   2760
   ScaleWidth      =   6480
   Top             =   1170
   Width           =   6600
   Begin VB.CommandButton query 
      Caption         =   "query"
      Height          =   375
      Left            =   5040
      TabIndex        =   5
      Top             =   120
      Width           =   735
   End
   Begin VB.TextBox status 
      Height          =   285
      Left            =   960
      TabIndex        =   3
      Top             =   480
      Width           =   3855
   End
   Begin VB.TextBox results 
      Height          =   1695
      Left            =   120
      MultiLine       =   -1  'True
      TabIndex        =   2
      Top             =   960
      Width           =   6255
   End
   Begin VB.TextBox url 
      Height          =   285
      Left            =   960
      TabIndex        =   0
      Text            =   "http://localhost:5781"
      Top             =   120
      Width           =   3855
   End
   Begin VB.Label Label2 
      Caption         =   "Doc status"
      Height          =   375
      Left            =   120
      TabIndex        =   4
      Top             =   480
      Width           =   1095
   End
   Begin VB.Label Label1 
      Caption         =   "Url"
      Height          =   255
      Left            =   120
      TabIndex        =   1
      Top             =   120
      Width           =   375
   End
   Begin HTTPCTLib.HTTP http 
      Left            =   6120
      Top             =   0
      _ExtentX        =   635
      _ExtentY        =   635
      RemoteHost      =   "127.0.0.1"
      RemotePort      =   80
      ConnectTimeout  =   0
      RecvTimeout     =   0
      NotificationMode=   1
      Document        =   ""
      Method          =   1
   End
End
Attribute VB_Name = "query_results"
Attribute VB_Creatable = False
Attribute VB_Exposed = False
Private Sub http_DocOutput(ByVal DocOutput As DocOutput)
  Dim s As String
  Select Case DocOutput.State
    Case icDocBegin
      status.Text = "icDocBegin"
    Case icDocData
      DocOutput.GetData s
      results.Text = results.Text + s
      status.Text = "icDocData" + _
        Str$(DocOutput.BytesTransferred)
    Case icDocEnd
      status.Text = "icDocEnd" + _
        Str$(DocOutput.BytesTransferred)
  End Select
End Sub

Private Sub Text1_Change()

End Sub


Private Sub http_Error(Number As Integer, Description As String, Scode As Long, Source As String, HelpFile As String, HelpContext As Long, CancelDisplay As Boolean)
  results.Text = results.Text + http.ReplyString
  results.Text = results.Text + "error" + Str$(Number) + ": " + Description
End Sub


Private Sub query_Click()
  status.Text = ""
  results.Text = ""
  http.url = url.Text
  http.GetDoc
End Sub



