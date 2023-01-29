Attribute VB_Name = "Module3"
Option Explicit

' ------------------------------------------------------------------------
Sub notify_error( _
  message As String _
)
  Call notify_ari(message, "Tableau Video Ceasar Error")
End Sub

' ------------------------------------------------------------------------
Function notify_yesno( _
  message As String, _
  title As String _
) As Integer
  notify_yesno = MsgBox(message, vbYesNo, title)
End Function

' ------------------------------------------------------------------------
Sub notify_ari( _
  message As String, _
  title As String _
)
  Dim answer As Integer
  answer = MsgBox(message, vbAbortRetryIgnore, title)
  If answer = vbRetry Then
    
  End If
  If answer = vbAbort Then
    End
  End If

End Sub
