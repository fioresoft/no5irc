// ChildFrm.cpp : implementation of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "View.h"
#include "ChildFrm.h"
#include "MainFrm.h"

void CChildFrame::OnFinalMessage(HWND /*hWnd*/)
{
	delete this;
}

LRESULT CChildFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_hWndClient = m_view.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VSCROLL  |\
		ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL | ES_SAVESEL | ES_READONLY | ES_WANTRETURN, WS_EX_CLIENTEDGE);
	m_font = AtlCreateControlFont();
	//m_view.SetFont(m_font);
	m_data.pView = &m_view;

	bHandled = FALSE;
	return 1;
}

LRESULT CChildFrame::OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
	LPMSG pMsg = (LPMSG)lParam;

	if(CTabbedMDIChildWindowImpl<CChildFrame>::PreTranslateMessage(pMsg))
		return TRUE;

	return m_view.PreTranslateMessage(pMsg);
}

LRESULT CChildFrame::OnTabSelChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	NMCTC2ITEMS* pData = (LPNMCTC2ITEMS)pnmh;
	CTabViewTabItem* itemOld = NULL;
	ATLTRACE(_T("item %d selected\n"), pData->iItem2);
	CTabViewTabItem *itemNew = m_frame.TabCtrl.GetItem(pData->iItem2);
	if (pData->iItem1 >= 0) {
		itemOld = m_frame.TabCtrl.GetItem(pData->iItem1);
		m_frame.SendMessage(WM_MDIACTIVATE, (WPARAM)itemOld->GetTabView(), (LPARAM)itemNew->GetTabView());
	}
	else
		m_frame.SendMessage(WM_MDIACTIVATE, (WPARAM)NULL, (LPARAM)itemNew->GetTabView());
	return 0;
}


LRESULT CChildFrame::OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	BOOL close = m_frame.SendMessage(WM_CHILDDESTROY, 0, (LPARAM)&m_data);

	if (!close)
		bHandled = TRUE;
	else
		bHandled = FALSE;
	return 0;
}

LRESULT CChildFrame::OnEditLink(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	ENLINK* P = (ENLINK*)pnmh;
	if (P->msg == WM_LBUTTONUP) {
		CView* pView = m_frame.GetActiveView();
		CString link = pView->GetTextRangeString(P->chrg);
		ShellExecute(m_hWnd, _T("open"), link, NULL, NULL, SW_SHOW);
	}
	return 0;
}