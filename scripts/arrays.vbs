Dim user

Sub no5_OnNamesInChannel(channel, users)
	Call no5.Output("" & users.Count)
	For Each user In users
		Call no5.Output(user.Name)
	Next
End Sub