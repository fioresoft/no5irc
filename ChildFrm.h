// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "usermsgs.h"

#define CHAIN_MDICLIENT_COMMANDS() \
	if((uMsg == WM_COMMAND) && (this->m_hWndMDIClient != NULL)) \
		::SendMessage(this->m_hWndMDIClient, uMsg, wParam, lParam);

enum ViewType
{
	VIEW_SERVER,
	VIEW_CHANNEL,
	VIEW_PVT,
	VIEW_NONE,
};

struct ViewData
{
	CView* pView;
	CString name;
	ViewType type;

	ViewData(CView* view, CString _name, ViewType _type)
	{
		pView = view;
		name = _name;
		type = _type;
	}
	ViewData()
	{
		pView = NULL;
		type = VIEW_NONE;
	}
};

class CMainFrame;

class CChildFrame : public CTabbedMDIChildWindowImpl<CChildFrame>
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MDICHILD)
	CChildFrame(CString name, ViewType type, CMainFrame& frame) :m_data(NULL, name, type), m_frame(frame)
	{
		//
	}
	CView m_view;
	CFont m_font;
	//
	ViewData m_data;
	CMainFrame& m_frame;

	virtual void OnFinalMessage(HWND /*hWnd*/);

	BEGIN_MSG_MAP(CChildFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_FORWARDMSG, OnForwardMsg)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_CHILDGETDATA, OnGetData)
		NOTIFY_CODE_HANDLER(EN_LINK, OnEditLink)
		MESSAGE_HANDLER(WM_MDIACTIVATE, OnMdiActivate)
		NOTIFY_CODE_HANDLER(CTCN_SELCHANGE, OnTabSelChange)
		CHAIN_CLIENT_COMMANDS()
		CHAIN_MSG_MAP(CTabbedMDIChildWindowImpl<CChildFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnForwardMsg(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnGetData(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		return (LRESULT)&m_data;
	}
	LRESULT OnEditLink(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnTabSelChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
	LRESULT OnMdiActivate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
};
