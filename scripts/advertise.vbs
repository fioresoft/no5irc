option explicit

Sub no5_OnMeJoin(channel,nick)
	Call no5.Output("channel is " & channel)
	Call no5.SendChannelMsg(channel,"Download NO5 IRC for Windows at https://fioresoft.net")
End Sub
