VERSION 5.00
Object = "{48E59290-9880-11CF-9754-00AA00C00908}#1.0#0"; "MSINET.OCX"
Object = "{248DD890-BB45-11CF-9ABC-0080C7E7B78D}#1.0#0"; "MSWINSCK.OCX"
Begin VB.Form Form1 
   Caption         =   "asp"
   ClientHeight    =   2760
   ClientLeft      =   3000
   ClientTop       =   2820
   ClientWidth     =   7440
   BeginProperty Font 
      Name            =   "Fixedsys"
      Size            =   9
      Charset         =   0
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   ScaleHeight     =   2760
   ScaleWidth      =   7440
   Begin VB.TextBox output 
      Height          =   2775
      Left            =   0
      MultiLine       =   -1  'True
      ScrollBars      =   3  'Both
      TabIndex        =   0
      Top             =   0
      Width           =   7455
   End
   Begin MSWinsockLib.Winsock worker 
      Left            =   1800
      Top             =   0
      _ExtentX        =   741
      _ExtentY        =   741
   End
   Begin MSWinsockLib.Winsock server 
      Left            =   2640
      Top             =   0
      _ExtentX        =   741
      _ExtentY        =   741
   End
   Begin InetCtlsObjects.Inet client 
      Left            =   720
      Top             =   0
      _ExtentX        =   1005
      _ExtentY        =   1005
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Dim worker_served As Long



' purpose of this form is to act as a web server
' only one request is honored: to cause the wrangler start a drone and return
'   html which allows the browser to contact the new drone
' the request is:
'   http://xxx.com:5788/FFF?whost=HHH&wport=PPP&sessid=SSS&streamid=TTT
' where:
'   FFF is the template filename
'   HHH is the wrangler's host IP address
'   PPP is the wrangler's port
'   SSS is the fake session ID (normally embedded in a cookie)
'   TTT is the desired stream name
' as a response to the request, a template file is given back to the browser
' in the template file, occurrences of %HOST% and %PORT% are replaced with
'   the new drone's host and port respectively
' in the future, the streamid will be used to look up the wrangler's host:port
 
Private Sub Form_Load()
server.LocalPort = 5788
server.Listen
dprint Str$(Time) + " " + _
  "listining on http://" + server.LocalIP + ":" + _
  Trim$(Str$(server.LocalPort))
End Sub
Private Sub dprint( _
  s As String _
)
  output.SelStart = 0
  output.SelLength = Len(output.Text)
  output.SelText = output.Text + s + vbCrLf
End Sub
  
Private Sub server_ConnectionRequest( _
  ByVal requestID As Long _
)
  If worker.State <> sckClosed Then worker.Close
  worker_served = 0
  worker.Accept requestID
End Sub
Private Function getport( _
  wrangler As String, _
  sessid As String, _
  streamid As String _
) As String                             ' ask wrangler to start drone
  Dim url As String
  url = wrangler + _
    "/start" + _
    "?sessid=" + sessid + _
    "&client=" + worker.RemoteHostIP + _
    "&limit=500" + _
    "&streamid=" + streamid             ' form request for wrangler
    
  Dim s As String
  s = client.OpenURL(url)               ' send request to wrangler
  Dim p As Long
  p = InStr(s, "port: ")                ' find port in wrangler's response
  If p = 0 Then
    p = InStr(s, vbCrLf)
    If p <> 0 Then s = Mid(s, p + 2)
    p = InStr(s, vbCrLf)
    If p <> 0 Then s = Mid(s, 1, p)
    dprint Str$(Time) + " " + s
    s = "0"
  Else
    s = Mid(s, p + 6)
    p = InStr(s, vbCrLf)
    If p <> 0 Then s = Mid(s, 1, p - 1) ' chop off end-of-line
  End If
  getport = s
End Function
Private Sub substitute( _
  s As String, _
  var As String, _
  val As String _
)                                       ' substitute val for var in string
  Dim p As Long
  Dim l As Long
  l = Len(var)
  p = InStr(s, var)
  If p <> 0 Then                        ' var exists in string?
    Dim r As String
    If (p + l < Len(s)) Then
      r = Right(s, Len(s) - p - l + 1)  ' everything after var in string
    Else
      r = ""
    End If
    s = Left(s, p - 1) + val + r        ' do the replacement
  End If
End Sub

Private Sub response( _
  filename As String, _
  whost As String, _
  wport As String, _
  sessid As String, _
  streamid As String _
)                                       ' serve html response to request
  Dim drone_port As String
  Dim wrangler As String
  wrangler = "http://" + _
    whost + ":" + wport                 ' where wrangler lives
  drone_port = getport( _
    wrangler, sessid, streamid)         ' ask wrangler to start new drone
  dprint Str$(Time) + " " + _
    "reply: drone port " + _
    drone_port
  
  On Error GoTo done
  Open filename For Input As #1         ' open html response template
  While True
    Dim line As String
    Line Input #1, line
    Call substitute(line, _
      "%HOST%", whost)                  ' replace %HOST% in html
    Call substitute(line, _
      "%PORT%", drone_port)             ' replace %PORT% in html
    worker.SendData line + vbCrLf       ' send line to browser
  Wend
done:
  Close #1
End Sub
Private Function cgival( _
  request As String, _
  name As String _
)                                       ' get an arg from http request
  Dim s As String
  s = Mid(request, _
    InStr(request, name + "=") + _
    Len(name) + 1)                      ' value after name=
  p = InStr(s, "&")                     ' value ends with an '&'
  If p <> 0 Then s = Left(s, p - 1)
  cgival = s
  dprint "  " + name + "=" + s
End Function
Private Sub worker_DataArrival( _
  ByVal bytesTotal As Long _
)                                       ' request has arrived from browser
  Dim request As String
  worker.GetData request
  
  While Len(request) > 1 And _
        Right(request, 2) = vbCrLf      ' chop of end-of-lines
    request = Left(request, _
      Len(request) - 2)
  Wend
  
  request = Mid(request, 6)             ' get just the meat of the request
  request = Left(request, _
    InStr(request, " "))
  
  dprint Str$(Time) + " " + _
    "request: " + request
  Dim filename As String
  Dim whost As String
  Dim wport As String
  Dim sessid As String
  Dim streamid As String
  
  filename = Mid(request, 1, _
    InStr(request, "?") - 1)            ' pick out template filename
  dprint "  filename=" + filename
  
  whost = cgival(request, "whost")
  wport = cgival(request, "wport")
  sessid = cgival(request, "sessid")
  streamid = cgival(request, "streamid")
  
  If worker_served = 0 Then
    Call response(filename, _
      whost, wport, sessid, streamid)   ' send response to client
    worker_served = 1
  End If
End Sub


Private Sub worker_SendComplete()       ' browser is done sending
  worker.Close
End Sub


