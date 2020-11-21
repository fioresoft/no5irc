option explicit

Dim timer
Dim counter
Dim channel

counter = 1

timer = no5.SetTimer(1,2000) ' set a timer of 2 secs and id == 1
Call no5.Output("timerid is " & timer)

Sub no5_OnTimer( id )
	channel = no5.GetActiveViewName()
	Call no5.SendChannelMsg(channel,"time: " & Time & "" & vbCrLf)
	Call no5.Output("time: " & Time & "" & vbCrLf)
	counter = counter + 1
	If counter = 5 Then
		Call no5.SetTimer(1,0) ' pass 0 secs to kill the timer 1
	End If
End Sub