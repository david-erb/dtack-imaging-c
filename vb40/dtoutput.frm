VERSION 2.00
Begin Form dtoutput 
   Caption         =   "Dtack Program Output"
   ClientHeight    =   1245
   ClientLeft      =   75
   ClientTop       =   7740
   ClientWidth     =   7740
   Height          =   1935
   Left            =   15
   LinkTopic       =   "Form1"
   ScaleHeight     =   1245
   ScaleWidth      =   7740
   Top             =   7110
   Width           =   7860
   Begin ListBox box 
      FontBold        =   -1  'True
      FontItalic      =   0   'False
      FontName        =   "Monospaced"
      FontSize        =   8.25
      FontStrikethru  =   0   'False
      FontUnderline   =   0   'False
      Height          =   240
      Left            =   0
      TabIndex        =   0
      Top             =   0
      Width           =   495
   End
   Begin Menu dtoutput_menu_file 
      Caption         =   "File"
   End
   Begin Menu dtoutput_menu_clear 
      Caption         =   "Clear"
   End
   Begin Menu dtoutput_menu_top 
      Caption         =   "Top"
   End
   Begin Menu dtoutput_menu_bottom 
      Caption         =   "Bottom"
   End
   Begin Menu dtoutput_menu_help 
      Caption         =   "Help"
   End
End

Sub dtoutput_menu_clear_Click ()
  box.Clear
End Sub

Sub dtoutput_size ()

  If dtoutput.WindowState <> 1 Then
    box.Width = dtoutput.Width - 100
    box.Height = dtoutput.Height - 700
  End If

End Sub

Sub Form_Load ()

  dtoutput_size

End Sub

Sub Form_Resize ()

  dtoutput_size

End Sub

