#pragma once
#include "CFileSender.h"

class CFileTransferMonitor
{
	class CFileTransfer : public IFileTransferEvents
	{
		virtual void OnInit(LPCTSTR file, LPCTSTR nick, u_long filesize);
		virtual void OnConnect(const CSocketAddress& sa);
		virtual void OnRead(int totalRead);
		virtual void OnWrite(int totalWritten);
		virtual void OnClose();
		virtual void OnError(LPCTSTR msg);
		CListViewCtrl &m_lv;
	public:
		CFileTransfer(CListViewCtrl& lv) :m_lv(lv)
		{

		}
	};
public:
	CListViewCtrl m_lv;
	CSimpleValArray<IFileTransferEvents*> m_transfers;
public:
	HWND CreateListView(HWND m_hWndParent);
	IFileTransferEvents* GetNewFileTransfer()
	{
		IFileTransferEvents*p = new CFileTransfer(m_lv);
		m_transfers.Add(p);
		return p;
	}
};

