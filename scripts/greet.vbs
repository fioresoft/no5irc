Sub no5_OnUserJoin(channel,user)
	Call no5.SendChannelMsg(channel,"hello " & user & ", welcome to " & channel & " just in time " & Time)
	Call no5.Output(channel,"hello " & user & ", welcome to " & channel & " just in time " & Time)
End Sub