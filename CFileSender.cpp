#include "stdafx.h"
#include "resource.h"
#include "CFileSender.h"
#include "View.h"
#include "ChildFrm.h"
#include "MainFrm.h"	

CFileSender::~CFileSender()
{
	delete m_ps; 
}
BOOL CFileSender::SendCTCPRequest(CSocketAddress& sa)
{
	CString s;
	s.Format(_T("\001DCC SEND %s %u %u %u\001"), (LPCTSTR)m_path.GetFileName(), sa.GetIP().Get(true), sa.GetPort().Get(true),
		m_db.GetDataLen());
	m_frame.SendPrivateMsg(m_nick, s);
	if (m_pSink)
		m_pSink->OnError(_T("CTCP DCC SEND"));
	return TRUE;
}

CFileSender::CFileSender(CMainFrame &frame,CPath &path,LPCTSTR nick, IFileTransferEvents* pSink) : m_path(path), m_nick(nick),m_frame(frame),
	m_pSink(pSink)
{
	CWinFile wf;

	m_totalSent = 0;
	m_bAck = false;
	m_ps = new CMySocket(this);
	if (wf.Create(m_path, OPEN_EXISTING)) {
		if (wf.ReadAll(m_db, 0)) {
			if (m_pSink)
				m_pSink->OnInit(path, nick, m_db.GetDataLen());
			BOOL res = m_ps->CreateSocket();
			if (res) {
				res = m_ps->BindToAny(CPort(0));
				if (res) {
					CSocketAddress local;
					CSocketAddress local2;
					res = m_frame.m_sock.GetLocalAddress(local2); // for the ip
					ATLASSERT(res);
					res = m_ps->GetLocalAddress(local); // for the port
					local.Set(local2.GetIP(), local.GetPort());
					if (res) {
						SendCTCPRequest(local);
						res = m_ps->Listen(2);
						if (res) {
							res = m_ps->AsyncSelect(FD_ACCEPT);
						}
					}
				}
			}
		}
	}
}

void CFileSender::OnSockAccept(int error)
{
	CSocketAddress sa;
	BOOL res;
	int totalSend = (int)m_db.GetDataLen();
	int check = 0;
	int sent = 0;

	if (error)
		return;
	res = TRUE;
	SOCKET s;
	if (res) {
		s = m_ps->Accept(sa);
		res = s != INVALID_SOCKET;
		ATLASSERT(res);
		if (m_pSink)
			m_pSink->OnConnect(sa);
		res = m_ps->CloseSocket();
		ATLASSERT(res);
		res = m_ps->AttachAcceptedSocket(s);
		if (res) {
			res = m_ps->AsyncSelect(FD_READ | FD_WRITE | FD_CLOSE);
			ATLASSERT(res);
			while (sent < totalSend) {
				if (m_ps->IsHandleValid()) {
					check = m_ps->Send((char*)m_db.GetData() + sent, (m_db.GetDataLen() - sent) < 1024 ? (m_db.GetDataLen() - sent) : 1024);
					if (check != SOCKET_ERROR) {
						sent += check;
						ATLTRACE(_T("bytes sent %d\n"), sent);
						if (m_pSink)
							m_pSink->OnWrite(sent);
					}
					else {
						OnSockError(m_ps->GetLastError());
					}
				}
			}
			m_ps->CloseSocket();
		}
	}
	ATLASSERT(res);
}

void CFileSender::OnSockRead(int error)
{
	u_long read = 0;
	int check;

	check = m_ps->Recv((char*)&read, sizeof(u_long));
	ATLASSERT(check == sizeof(u_long));
	if (check != SOCKET_ERROR) {
		m_totalSent = ntohl(read);
		ATLTRACE(_T("bytes ack %d\n"), m_totalSent);
		m_bAck = true;
		if (m_pSink)
			m_pSink->OnRead(m_totalSent);
	}
	else if (WSAEWOULDBLOCK == m_ps->GetError() || NOERROR == m_ps->GetError())
		return;
	else {
		OnSockError(m_ps->GetLastError());
	}
}
void CFileSender::OnSockWrite(int error)
{

}
void CFileSender::OnSockClose(int error)
{
	ATLTRACE(_T("server socket closed\n"));
	if (m_pSink)
		m_pSink->OnClose();
}

void CFileSender::OnSockError(int error)
{
	CString err = m_ps->GetErrorDesc(error);
	ATLTRACE(_T("%s\n"),err);
	if (m_pSink)
		m_pSink->OnError(err);
}

void CFileSender::Abort()
{
	if (m_ps->IsConnected()) {
		m_ps->CloseSocket();
	}
}

// CFileReceiver
CFileReceiver::CFileReceiver(CMainFrame& frame, CPath& path, LPCTSTR nick, CSocketAddress& sa,u_long filesize, IFileTransferEvents*pSink) :\
	m_frame(frame),m_path(path), m_nick(nick),m_sa(sa),m_filesize(filesize),m_pSink(pSink)
{
	m_ps = new CMySocket(this);
	BOOL res = m_ps->CreateSocket();

	if (m_pSink) {
		m_pSink->OnInit(path, nick, filesize);
		m_pSink->OnError(_T("pending"));
	}

	m_totalRecv = 0;
	if (res) {
		res = m_ps->AsyncSelect(FD_READ | FD_WRITE |FD_CONNECT| FD_CLOSE);
		if (res) {
			res = m_ps->Connect(&m_sa);
		}
	}
	ATLASSERT(res);
}

CFileReceiver::~CFileReceiver()
{
	delete m_ps;
}

void CFileReceiver::OnSockError(int error)
{
	CString errstr = m_ps->GetErrorDesc(error);
	ATLTRACE(_T("%s\n"), errstr);
	if (m_pSink)
		m_pSink->OnError(errstr);
}
void CFileReceiver::OnSockRead(int error)
{
	int read;
	const int len = 1024;
	char buf[len] = { 0 };
	if (m_ps->IsHandleValid())
		read = m_ps->Recv(buf, len);
	else
		read = 0;
	if (read >= 0) {
		this->m_totalRecv += read;
		if (m_pSink)
			m_pSink->OnRead(m_totalRecv);
		if(read)
			m_db.Add((LPBYTE)buf, (DWORD)read);
		if ((m_filesize && m_totalRecv >= m_filesize) || (read == 0)) {
			m_ps->CloseSocket();
			// save the file
			CWinFile wf;
			BOOL res = wf.Create(m_path, OPEN_ALWAYS);
			if (res) {
				res = wf.Write(m_db);
			}
			ATLASSERT(res);
		}
		if(read) {
			if (m_ps->IsHandleValid()) {
				m_ps->Send((char*)&m_totalRecv, sizeof(m_totalRecv));
				if (m_pSink)
					m_pSink->OnWrite(m_totalRecv);
			}
		}
	}
}
void CFileReceiver::OnSockWrite(int error)
{
	ATLTRACE(_T("OnSockWrite: %d\n"), error);
}
void CFileReceiver::OnSockConnect(int error)
{
	//CSocketAddress sa;
	ATLTRACE(_T("OnSockConnect: %d\n"), error);
	//m_ps->GetRemoteAddress(sa);
	if (!error && m_pSink)
		m_pSink->OnConnect(m_sa);
}
void CFileReceiver::OnSockClose(int error)
{
	ATLTRACE(_T("OnSockClose: %d\n"), error);
	if (m_pSink)
		m_pSink->OnClose();
}

void CFileReceiver::Abort()
{
	if (m_ps->IsConnected()) {
		m_ps->CloseSocket();
	}
}