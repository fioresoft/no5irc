#include "stdafx.h"
#include "CFileTransferMonitor.h"
#include "resource.h"

HWND CFileTransferMonitor::CreateListView(HWND hWndParent)
{
	m_lv.Create(hWndParent, 0, 0, LVS_REPORT | LVS_SINGLESEL | LVS_SORTASCENDING | LVS_SHOWSELALWAYS | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		0, IDC_LV_TRANSFERS);
	LVCOLUMN col = { 0 };
	CClientDC dc(m_lv);
	CSize sz;
	LVITEM item = { 0 };
	//m_lv.SetFont(m_font);
	dc.GetTextExtent(_T(" nick "), strlen("nick     "), &sz);
	m_lv.InsertColumn(0, _T("nick"), LVCFMT_LEFT, sz.cx);
	dc.GetTextExtent(_T("  peer address    "), strlen("  peer address  "), &sz);
	m_lv.InsertColumn(1, _T("peer address"), LVCFMT_LEFT, sz.cx);
	dc.GetTextExtent(_T(" file name "), strlen(" file name "),&sz);
	m_lv.InsertColumn(2, _T("file name"), LVCFMT_LEFT, sz.cx);
	dc.GetTextExtent(_T(" file size "), strlen(" file size "), &sz);
	m_lv.InsertColumn(3, _T("file size"), LVCFMT_LEFT, sz.cx);
	dc.GetTextExtent(_T(" read "), strlen(" read "), &sz);
	m_lv.InsertColumn(4, _T("read"), LVCFMT_LEFT, sz.cx);
	dc.GetTextExtent(_T(" written "), strlen(" written "), &sz);
	m_lv.InsertColumn(5, _T("written"), LVCFMT_LEFT, sz.cx);
	dc.GetTextExtent(_T("    status    "), strlen("    status    "), &sz);
	m_lv.InsertColumn(6, _T("status"), LVCFMT_LEFT, sz.cx);
	
	return m_lv.m_hWnd;
}

void CFileTransferMonitor::CFileTransfer::OnInit(LPCTSTR file, LPCTSTR nick, u_long filesize)
{
	CString s;
	int index;
	float size = filesize;
	CString unit = _T("Bytes");

	index = m_lv.InsertItem(0, nick,0);
	m_lv.SetItemText(index, 2, file);
	if (size > 1024) {
		size /= 1024;
		unit = _T("KB");
	}
	s.Format(_T("%.02f %s"), size,LPCTSTR(unit));
	m_lv.SetItemText(index, 3, s);
}
void CFileTransferMonitor::CFileTransfer::OnConnect(const CSocketAddress& sa)
{
	m_lv.SetItemText(0, 1, (LPCTSTR)sa.ToString());
}
void CFileTransferMonitor::CFileTransfer::OnRead(int totalRead)
{
	CString s;
	float size = totalRead;
	CString unit = _T("Bytes");

	if (size > 1024) {
		size /= 1024;
		unit = _T("KB");
	}
	s.Format(_T("%.02f %s"), size,LPCTSTR(unit));
	m_lv.SetItemText(0, 4, (LPCTSTR)s);
}
void CFileTransferMonitor::CFileTransfer::OnWrite(int totalWritten)
{
	CString s;
	float size = totalWritten;
	CString unit = _T("Bytes");

	if (size > 1024) {
		size /= 1024;
		unit = _T("KB");
	}
	s.Format(_T("%.02f %s"), size, LPCTSTR(unit));

	m_lv.SetItemText(0, 5,(LPCTSTR)s);
}
void CFileTransferMonitor::CFileTransfer::OnClose()
{
	m_lv.SetItemText(0, 6,_T("closed"));
}
void CFileTransferMonitor::CFileTransfer::OnError(LPCTSTR msg)
{
	m_lv.SetItemText(0, 6,msg);
}
