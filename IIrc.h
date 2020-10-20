#pragma once
struct IIrc
{
	virtual void SendChannelMsg(LPCTSTR channel, LPCTSTR msg) = 0;
	virtual void SendPrivateMsg(LPCTSTR to, LPCTSTR msg) = 0;
	virtual void SendNoticeMsg(LPCTSTR to, LPCTSTR msg) = 0;
	virtual void JoinChannel(LPCTSTR channel) = 0;
	virtual void LeaveChannel(LPCTSTR channel, LPCTSTR msg) = 0;
	virtual void Quit(LPCTSTR msg) = 0;
	virtual void SendPass(LPCTSTR pass) = 0;
	virtual void SendNick(LPCTSTR nick) = 0;
	virtual void SendUser(LPCTSTR user,LPCTSTR realname) = 0;
	virtual void ListChannels() = 0;
	virtual void ListChannelNames(LPCTSTR channel) = 0;
	virtual void GetTopic(LPCTSTR channel) = 0;
	virtual void GetMode(LPCTSTR NameOrChannel) = 0;
	virtual void Pong(LPCTSTR code) = 0;
	virtual void RequestVersion(LPCTSTR from) = 0;
	virtual void RequestUserinfo(LPCTSTR from) = 0;
	virtual void RequestPing(LPCTSTR from) = 0;
	virtual void RequestTime(LPCTSTR from) = 0;
	virtual void AnswerVersionRequest(LPCTSTR from) = 0;
	virtual void AnswerUserinfoRequest(LPCTSTR from) = 0;
	virtual void AnswerPingRequest(LPCTSTR from) = 0;
	virtual void AnswerTimeRequest(LPCTSTR from) = 0;
	virtual void WhoIs(LPCTSTR nick) = 0;
	virtual void Who(LPCTSTR nick) = 0;
	virtual void WhoWas(LPCTSTR nick) = 0;
};

