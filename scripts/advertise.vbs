option explicit

Dim channel
channel = Empty ' pick your channel here

Sub no5_OnMeJoin(chann,n)
	channel = chann
	Call no5.Output("channel is " & channel)
	Call no5.SendChannelMsg(channel,"Download NO5 IRC for Windows at https://fioresoft.net" & vbCrLf)
End Sub

If NOT IsEmpty(channel) Then
Call no5.SendChannelMsg(channel,"Download NO5 IRC for Windows at https://fioresoft.net")
Call no5.Output("Download NO5 IRC for Windows at https://fioresoft.net")
Else 
	Call no5.Output("channel is empty")
End If

