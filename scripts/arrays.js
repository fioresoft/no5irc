function no5::OnNamesInChannel(channel, users){
	var count = users.Count;
	for(var i = 0; i < count; i++){
		no5.Output(users.Item(i).Name);
	}
	no5.Output("users: " + count);
}