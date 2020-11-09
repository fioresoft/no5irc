#pragma once
struct IIRCEvents
{
	virtual void OnChannelList(LPCTSTR channel,LPCTSTR users,LPCTSTR topic) = 0;
	virtual void OnBeginningOfList() = 0;
	virtual void OnEndOfList() = 0;
	virtual void OnChannelMode(LPCTSTR channel, LPCTSTR modes) = 0;
	virtual void OnUserMode(LPCTSTR user, LPCTSTR modes) = 0;
	virtual void OnChannelCreationTime(LPCTSTR channel, time_t time) = 0;
	virtual void OnBeginMOTD(LPCTSTR channel) = 0;
	virtual void OnMOTD(LPCTSTR channel,LPCTSTR msg) = 0;
	virtual void OnEndMOTD(LPCTSTR channel) = 0;
	virtual void OnNoTopic(LPCTSTR channel) = 0;
	virtual void OnTopic(LPCTSTR channel, LPCTSTR topic) = 0;
	virtual void OnWhoSetTheTopic(LPCTSTR channel, LPCTSTR user,time_t time) = 0;
	virtual void OnNamesInChannel(LPCTSTR channel, const CSimpleArray<CString>& args) = 0;
	virtual void OnNamesEnd(LPCTSTR channel) = 0;
	virtual void OnChannelMsg(LPCTSTR channel,LPCTSTR user,LPCTSTR msg,bool script) = 0;
	virtual void OnPrivateMsg(LPCTSTR from, LPCTSTR msg,bool script) = 0;
	virtual void OnUserQuit(LPCTSTR channel, LPCTSTR user, LPCTSTR msg) = 0;
	virtual void OnUserJoin(LPCTSTR channel, LPCTSTR user) = 0;
	virtual void OnMeJoin(LPCTSTR channel, LPCTSTR nick) = 0;
	virtual void OnUserPart(LPCTSTR channel, LPCTSTR user, LPCTSTR msg) = 0;
	virtual void OnNotice(LPCTSTR user, LPCTSTR msg,bool script) = 0;
	virtual void OnPing(LPCTSTR code) = 0;
	virtual void OnAction(LPCTSTR channel,LPCTSTR from, LPCTSTR msg) = 0;
	virtual void OnUnknownCmd(LPCTSTR line) = 0;
	virtual void OnTimer(long id) = 0;
};

