<%
  response.buffer = true
  textmode = false
  script = request.servervariables("SCRIPT_NAME")

  tabrobot_host = request("host")
  if tabrobot_host = "" then
    tabrobot_host = "localhost"
  end if

  tabrobot_port = request("port")
  if tabrobot_port = "" then
    tabrobot_port = "5790"
  end if

  tabrobot_hostport = tabrobot_host & ":" & tabrobot_port
  suspend_filename = "c:\tmp\tabroam\tabroam.suspend"

  distance = array()
  redim distance(7)
  distance(0) = 0
  distance(1) = 1
  distance(2) = 2
  distance(3) = 5
  distance(4) = 10
  distance(5) = 20
  distance(6) = 50

' -------------------------------------------------------------------------
function emit( _
  line _
)
  response.write line
  response.flush
end function
' -------------------------------------------------------------------------
function emitnl( _
  line _
)
  emit(line & vbcrlf)
end function
' -------------------------------------------------------------------------
function emitbrnl( _
  line _
)
  if textmode then
    emit(line & vbcrlf)
  else
    emit(line & "<br>" & vbcrlf)
  end if
end function
' -------------------------------------------------------------------------
function head_html( _
  title _
)
  emitnl("<html>")
  emitnl("<head>")
  emitnl("<title>")
  emitnl(title)
  emitnl("</title>")
  emitnl("</head>")
end function
' -------------------------------------------------------------------------
function head_text( _
  title _
)
  response.contenttype = "text/plain"
  emitnl("")
  textmode = true
end function
' -------------------------------------------------------------------------
function body( _
)
  emitnl("<body bgcolor='#ffffff'>")
end function
' -------------------------------------------------------------------------
function command( _
  s _
)
  emitbrnl "command '" & s & "'"
  set tcp = Server.CreateObject("NETUTILS.TCPCLIENT")
  emitbrnl "connecting to tabrobot at " & tabrobot_hostport
  tcp.ConnectWith tabrobot_host, tabrobot_port
  if tcp.IsConnected() then
    emitbrnl "connected to tabrobot at " & tabrobot_hostport
    tcp.Send(s & vbcrlf)
    tcp.Send(vbcrlf)			' tabrobot terminates on blank line
    do while tcp.IsConnected()
      r = tcp.Get()
      emitnl(r)
'      if r = "go" then exit do
'      if left(r, 5) = "error" then exit do
    loop
  
   emitbrnl "disconnecting from tabrobot at " & tabrobot_hostport
  else
   emitbrnl "unable to connect to tabrobot at " & tabrobot_hostport
  end if
  set tcp = nothing

end function

' ----------------------------------------------------------------------------
' Open text file for reading if ~mode~ is "r", otherwise for writing.

function util_open(filename, mode)
  set util_fsobject = server.createobject("scripting.filesystemobject")
  dim file
'  err.number = 0
'  on error resume next
  if lcase(mode) = "r" then
    set file = util_fsobject.opentextfile(filename)
  else
    set file = util_fsobject.createtextfile(filename)
  end if
  if isobject(file) then
    set util_open = file
  else
    set util_open = nothing
  end if
end function

' ----------------------------------------------------------------------------
' Delete file.

function util_delete(filename)
  set util_fsobject = server.createobject("scripting.filesystemobject")
  dim f: f = "util_delete"
'  on error resume next
  if util_fsobject.fileexists(filename) then
    emitbrnl("resuming")
    util_fsobject.deletefile(filename)
  else
    emitbrnl("already resumed")
  end if
end function

' -------------------------------------------------------------------------
function execute( _
)
  if request("commandbutton") <> "" then
    command(request("command"))
  end if

  d = request("distance")
  if d = "" then d = "1"

  if request("left") <> "" then
    command("ptz,<x," & d)
  end if
  if request("right") <> "" then
    command("ptz,>x," & d)
  end if
  if request("up") <> "" then
    command("ptz,>y," & d)
  end if
  if request("down") <> "" then
    command("ptz,<y," & d)
  end if
  if request("wide") <> "" then
    command("ptz,<z," & d)
  end if
  if request("tight") <> "" then
    command("ptz,>z," & d)
  end if
  if request("close") <> "" then
    command("ptz,<o," & d)
  end if
  if request("open") <> "" then
    command("ptz,>o," & d)
  end if
  if request("suspend") <> "" then
    set ffile = util_open(suspend_filename, "w")
    if not isobject(ffile) then
      emitbrnl("error opening " & suspend_filename)
    else
      emitbrnl("suspended")
      ffile.close()
    end if
  end if
  if request("resume") <> "" then
    util_delete(suspend_filename)
  end if
  if request("go") <> "" then
    command("ptz,G" & cint(request("go")))
  end if
end function
' -------------------------------------------------------------------------
function frame_parent( _
)
  head_html("Tableau PTZ Control")
  emitnl("<frameset rows='*,200'>")
  emitnl("<frame name='top' src='" & script & "?frame=top'>")
  emitnl("<frame name='bot' src='" & script & "?frame=bot'>")
  emitnl("</frameset>")
end function
' -------------------------------------------------------------------------
function frame_top( _
)
  head_html("Tableau PTZ Command")
  body()
  emitbrnl("tabrobot: " & tabrobot_hostport)
  emitnl("<form target=bot action='" & script & "'>")
  emitnl("host: <input type=text name=host value='" & tabrobot_host & "'>")
  emitnl("port: <input type=text name=port value='" & tabrobot_port & "'>")
  emitnl("<br>")
  emitnl("<input type=hidden name=frame value=bot>")
  emitnl("<input type=text name=command value='" & _
    server.htmlencode(request("command")) & "'>")
  emitnl("<input type=submit value=Execute name=commandbutton>")
  emitnl("<table>")
  emitnl("<tr>")
  emitnl("<tr><td>distance</td>")
  for i=lbound(distance) to ubound(distance)-1
    emitnl("<td align=center>" & distance(i) & "</td>")
  next
  emitnl("</tr>")
  emitnl("<tr><td>&nbsp;</td>")
  for i=lbound(distance) to ubound(distance)-1
    emitnl("<td align=center><input type=radio name=distance value='" & distance(i) & "'></td>")
  next
  emitnl("</tr>")
  emitnl("</table>")

  emitnl("<table>")
  emitnl("<tr><td align=center>pan</td><td align=center>tilt</td><td align=center>zoom</td><td align=center>iris</td><td align=center>pause</td></tr>")

  emitnl("<tr><td>")
  emitnl("<table>")
  emitnl("<tr><td align=center><input type=submit value=Left name=left></td></tr>")
  emitnl("<tr><td align=center><input type=submit value=Right name=right></td></tr>")
  emitnl("</table>")

  emitnl("</td><td>")
  emitnl("<table>")
  emitnl("<tr><td align=center><input type=submit value=Up name=up></td></tr>")
  emitnl("<tr><td align=center><input type=submit value=Down name=down></td></tr>")
  emitnl("</table>")

  emitnl("</td><td>")
  emitnl("<table>")
  emitnl("<tr><td align=center><input type=submit value=Tight name=tight></td></tr>")
  emitnl("<tr><td align=center><input type=submit value=Wide name=wide></td></tr>")
  emitnl("</table>")

  emitnl("</td><td>")
  emitnl("<table>")
  emitnl("<tr><td align=center><input type=submit value=Open name=open></td></tr>")
  emitnl("<tr><td align=center><input type=submit value=Close name=close></td></tr>")
  emitnl("</table>")

  emitnl("</td><td>")
  emitnl("<table>")
  emitnl("<tr><td align=center><input type=submit value=Suspend name=suspend></td></tr>")
  emitnl("<tr><td align=center><input type=submit value=Resume name=resume></td></tr>")
  emitnl("</table>")

  emitnl("</td></tr>")
  emitnl("</table>")

  emitbrnl("go to preset:")
  for i=0 to 15
    emitnl("<tr><td align=center><input type=submit value='" & right("00" & cstr(i), 2) & "' name=go></td></tr>")
  next

  emitnl("</form>")
end function
' -------------------------------------------------------------------------
function frame_bot( _
)
  head_text("Tableau PTZ Response")
  execute()
end function
' -------------------------------------------------------------------------
  if request("frame") = "" then frame_parent()
  if request("frame") = "top" then frame_top()
  if request("frame") = "bot" then frame_bot()
%>
