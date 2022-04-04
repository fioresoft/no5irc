option explicit

Dim txtObj
Set txtObj = no5.Text
txtObj.Clear
txtObj.Bold
txtObj.Strikeout
txtObj.BackColor = 1
txtObj.ForeColor = 8
txtObj.PlainText = "hello world"
call no5.SendActiveChannelMsg(txtObj.Text & vbCrlf)
Call no5.Output(txtObj.PlainText)