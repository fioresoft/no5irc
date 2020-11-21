var id = no5.SetTimer(1,2000);

function no5::OnTimer( timer )
{
	var d = new Date();
	no5.Output(d.toTimeString());
}


