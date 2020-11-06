#pragma once

class CMainFrame;
class CMySocket;

struct IFileTransferEvents
{
	virtual void OnInit(LPCTSTR file, LPCTSTR nick, u_long filesize) = 0;
	virtual void OnConnect(const CSocketAddress& sa) = 0;
	virtual void OnRead(int totalRead) = 0;
	virtual void OnWrite(int totalWritten) = 0;
	virtual void OnClose() = 0;
	virtual void OnError(LPCTSTR msg) = 0;
};

struct IFileTransfer
{
	virtual void Abort() = 0;
};

class CFileSender : public ISocketEvents, public IFileTransfer
{
	CMainFrame& m_frame;
	CMySocket *m_ps;
	CPath m_path;
	CDataBuffer<BYTE> m_db;
	CString m_nick;
	volatile u_long m_totalSent;
	volatile bool m_bAck;
	IFileTransferEvents* m_pSink;
	//
	BOOL SendCTCPRequest(CSocketAddress& sa);
public:
	CFileSender(CMainFrame& frame, CPath& path, LPCTSTR nick, IFileTransferEvents* pSink);
	virtual ~CFileSender();
public:
	void SetSink(IFileTransferEvents* pSink)
	{
		m_pSink = pSink;
	}
	virtual void OnSockError(int error);
	virtual void OnSockRead(int error);
	virtual void OnSockWrite(int error);
	virtual void OnSockConnect(int error) {}
	virtual void OnSockClose(int error);
	virtual void OnSockAccept(int error);
	virtual void OnSockOutOfBand(int error) {}
	virtual void OnSockConnecting(void) {}
	virtual void OnSockConnectTimeout(void) {}
	virtual void OnSockResolvingAddress(void) {}
	virtual void OnSockAddressResolved(int error) {}
	// IFileTransfer
	virtual void Abort();
};

class CFileReceiver : public ISocketEvents, public IFileTransfer
{
	CMainFrame& m_frame;
	CMySocket* m_ps;
	CPath m_path;
	CDataBuffer<BYTE> m_db;
	CString m_nick;
	volatile u_long m_totalRecv;
	CSocketAddress m_sa;
	const u_long m_filesize;
	IFileTransferEvents* m_pSink;
public:
	CFileReceiver(CMainFrame& frame, CPath& path, LPCTSTR nick,CSocketAddress &sa,u_long filesize, IFileTransferEvents *pSink);
	virtual ~CFileReceiver();
public:
	void SetSink(IFileTransferEvents* pSink)
	{
		m_pSink = pSink;
	}
	virtual void OnSockError(int error);
	virtual void OnSockRead(int error);
	virtual void OnSockWrite(int error);
	virtual void OnSockConnect(int error);
	virtual void OnSockClose(int error);
	virtual void OnSockAccept(int error) {}
	virtual void OnSockOutOfBand(int error) {}
	virtual void OnSockConnecting(void) {}
	virtual void OnSockConnectTimeout(void) {}
	virtual void OnSockResolvingAddress(void) {}
	virtual void OnSockAddressResolved(int error) {}
	// IFileTransfer
	virtual void Abort();
};

