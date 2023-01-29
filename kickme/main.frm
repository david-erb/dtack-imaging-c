VERSION 4.00
Begin VB.Form main 
   Caption         =   "Tableau Kickme"
   ClientHeight    =   4245
   ClientLeft      =   2310
   ClientTop       =   3630
   ClientWidth     =   9900
   Height          =   4650
   Left            =   2250
   LinkTopic       =   "Form1"
   ScaleHeight     =   4245
   ScaleWidth      =   9900
   Top             =   3285
   Width           =   10020
   Begin VB.CommandButton query 
      Caption         =   "query"
      Height          =   375
      Left            =   2280
      TabIndex        =   11
      Top             =   120
      Width           =   975
   End
   Begin VB.CommandButton exit 
      Caption         =   "exit"
      Height          =   375
      Left            =   1200
      TabIndex        =   10
      Top             =   120
      Width           =   975
   End
   Begin VB.TextBox output 
      Height          =   375
      Left            =   120
      TabIndex        =   9
      Top             =   1920
      Width           =   5295
   End
   Begin VB.TextBox arg 
      Height          =   405
      Left            =   1200
      TabIndex        =   8
      Text            =   "c:\tableau\config\kickme.arg"
      Top             =   1320
      Width           =   4215
   End
   Begin VB.TextBox command 
      Height          =   375
      Left            =   1200
      TabIndex        =   5
      Text            =   "c:\tableau\00-25\tableau2"
      Top             =   720
      Width           =   4215
   End
   Begin VB.TextBox kicks 
      Height          =   375
      Left            =   4440
      TabIndex        =   4
      Top             =   2400
      Width           =   975
   End
   Begin VB.TextBox status 
      Height          =   375
      Left            =   720
      TabIndex        =   1
      Top             =   2400
      Width           =   2775
   End
   Begin VB.CommandButton start 
      Caption         =   "start"
      Height          =   375
      Left            =   120
      TabIndex        =   0
      Top             =   120
      Width           =   975
   End
   Begin VB.Label Label4 
      Caption         =   "arg"
      Height          =   255
      Left            =   120
      TabIndex        =   7
      Top             =   1440
      Width           =   975
   End
   Begin VB.Label Label3 
      Caption         =   "command"
      Height          =   255
      Left            =   120
      TabIndex        =   6
      Top             =   840
      Width           =   975
   End
   Begin VB.Label Label2 
      Caption         =   "Kick count"
      Height          =   255
      Left            =   3600
      TabIndex        =   3
      Top             =   2520
      Width           =   855
   End
   Begin VB.Label Label1 
      Caption         =   "Status"
      Height          =   255
      Left            =   120
      TabIndex        =   2
      Top             =   2520
      Width           =   495
   End
End
Attribute VB_Name = "main"
Attribute VB_Creatable = False
Attribute VB_Exposed = False

Private Sub exit_Click()
  End
End Sub

Private Sub output_KeyDown(KeyCode As Integer, Shift As Integer)
  If kicks.Text = "" Then
    kicks.Text = "0"
  End If
  kicks.Text = Trim$(Str$(CInt(kicks.Text) + 1))
End Sub


Private Sub query_Click()
  Dim q As query_results
  Set q = New query_results
  q.Show
End Sub

Private Sub start_Click()
  kicks.Text = "0"
  status.Text = util_08x(CLng(status.hwnd))
  
  Dim s As String
  s = Command.Text + " 0 " + _
    util_08x(CLng(output.hwnd)) + " " + _
    util_08x(CLng(status.hwnd)) + " @" + _
    arg.Text
   
  Dim rc As Long
  rc = WinExec(s, SW_SHOW)
  If rc < 32 Then
    Call notify_error("Sorry, error " + _
      Trim$(Str$(rc)) + _
      " in " + s)
  End If
End Sub


Private Sub status_Change()
Dim t As String
t = Left(status.Text, 4)
If t = "file" Then
  filename = Right(status.Text, 4)
End If
End Sub


