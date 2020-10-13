// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Path.h"

#define CHAIN_COMMANDS_MEMBER_ID_RANGE(theChainMember, idFirst, idLast) \
    if(uMsg == WM_COMMAND && (LOWORD(wParam) >= idFirst && LOWORD(wParam) <=idLast) && (theChainMember.m_hWnd == GetFocus())) \
        CHAIN_MSG_MAP_MEMBER(theChainMember)

class CMySocket : public CSimpleSocket
{
public:
	CMySocket(ISocketEvents* pSink) :CSimpleSocket(pSink)
	{

	}
	// IResolveEvents
	virtual void OnResolvingAddress(CAsyncResolve* pObj)
	{
		CSimpleSocket::OnResolvingAddress(pObj);
		//m_top.AppendText(_T("resolving address...\r\n"));
	}
	virtual void OnAddressResolved(CAsyncResolve* pObj, int error,
		int buflen)
	{
		CSimpleSocket::OnAddressResolved(pObj, error, buflen);
		//m_top.AppendText(_T("Address resolved\r\n"));
	}
};

class CMainFrame;

class CMyTabbedChildWindow : public CTabbedChildWindow<>
{
	typedef  CTabbedChildWindow<> _baseClass;
	CMainFrame& m_frame;
public:
	CMyTabbedChildWindow(CMainFrame& frame) :m_frame(frame)
	{
		//
	}
	DECLARE_FRAME_WND_CLASS(_T("MyTabbedChildWindow"), COLOR_APPWORKSPACE)
	BEGIN_MSG_MAP(CMyTabbedChildWindow)
		NOTIFY_CODE_HANDLER(CTCN_SELCHANGE, OnSelChange)
		NOTIFY_CODE_HANDLER(CTCN_CLOSE, OnTabClose)
		NOTIFY_HANDLER(IDC_LISTVIEW, NM_DBLCLK, OnLVDoubleClick)
		NOTIFY_HANDLER(IDC_TREEVIEW, NM_DBLCLK, OnTVDoubleClick)
		NOTIFY_HANDLER(IDC_LISTVIEW, LVN_COLUMNCLICK, OnColumnClick)
		//FORWARD_NOTIFICATIONS()
		CHAIN_MSG_MAP(_baseClass)
	END_MSG_MAP()

	LRESULT OnSelChange(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		ATLTRACE(_T("CMyTabbedChildWindow OnSelChange\n"));
		return 0;
	}
	LRESULT OnTabClose(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		ATLTRACE(_T("CMyTabbedChildWindow OnTabClose\n"));
		return 0;
	}
	LRESULT OnLVDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnTVDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnColumnClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
};

template< class T, class TTabCtrl >
class CMyMDITabOwnerImpl : public CMDITabOwnerImpl<T, TTabCtrl>
{
public:
	DECLARE_WND_CLASS_EX(_T("MyMdiTabOwner"), 0, COLOR_APPWORKSPACE)
	typedef CMyMDITabOwnerImpl<T, TTabCtrl> thisClass;
	typedef CMDITabOwnerImpl<T, TTabCtrl> baseClass;
	BEGIN_MSG_MAP(thisClass)
		NOTIFY_CODE_HANDLER(CTCN_SELCHANGE, OnSelChange)
		NOTIFY_CODE_HANDLER(CTCN_CLOSE, OnTabClose)
		CHAIN_MSG_MAP(baseClass)
		// NOTE: CCustomTabCtrl derived classes no longer
		//  need notifications reflected.
		// REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnSelChange(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		LRESULT res = baseClass::OnSelChange(idCtrl, pnmh, bHandled);
		int nSel = m_TabCtrl.GetCurSel();

		if (nSel >= 0) {
			TTabCtrl::TItem* pItem = m_TabCtrl.GetItem(nSel);
			if (pItem)
			{
				::SendMessage(pItem->GetTabView(), WM_NOTIFY, (WPARAM)idCtrl, (LPARAM)pnmh);
			}
		}
		return res;
	}

	LRESULT OnTabClose(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	{
		LRESULT res = baseClass::OnTabClose(idCtrl, pnmh, bHandled);
		int nSel = m_TabCtrl.GetCurSel();
		if (nSel >= 0) {
			TTabCtrl::TItem* pItem = m_TabCtrl.GetItem(nSel);
			if (pItem)
			{
				::SendMessage(pItem->GetTabView(), WM_NOTIFY, (WPARAM)idCtrl, (LPARAM)pnmh);
			}
		}
		return res;
	}
};

template< class TTabCtrl >
class CMyMDITabOwner :
	public CMyMDITabOwnerImpl<CMyMDITabOwner<TTabCtrl>, TTabCtrl>
{
};

typedef CTabbedMDIClient< CDotNetTabCtrl<CTabViewTabItem>, CMyMDITabOwner< CDotNetTabCtrl<CTabViewTabItem> > > CMyTabbedMDIClient;

class CMainFrame : 
	public CTabbedMDIFrameWindowImpl<CMainFrame, CMyTabbedMDIClient>, 
	public CUpdateUI<CMainFrame>,
	public CMessageFilter, public CIdleHandler,
	public ISocketEvents
{
	typedef CTabbedMDIFrameWindowImpl<CMainFrame, CMyTabbedMDIClient> _baseClass;
	typedef CDotNetTabCtrl<CTabViewTabItem> _tabControl;
public:
	TTabCtrl& TabCtrl = GetMDITabCtrl();
private:
	CWindow  TabOwnerParent = GetTabOwnerParent();
	CTabbedMDICommandBarCtrl m_CmdBar;
	CHorSplitterWindow m_splitter;
	CBottom m_bottom;
	CMyTabbedChildWindow m_tab;
	CSplitterWindow m_vsplitter;
	CListBox m_lb;
	CListBox m_lb2;
	CFont m_font;
	CNo5TreeCtrl m_tv;
	CListViewCtrl m_lv;
	CMarqueeWnd m_marquee;
	CMySocket m_sock;
	CString m_server;
	CString m_nick;
	CString m_name;
	CString m_user;
	CString m_pass;
	CPath m_path;
	CStringArray m_servers;
	CString m_NameOrChannel;
	bool m_bInChannel;
	CString m_JoinChannel; // channel to join on connection
	bool m_bNoColors; // channel supports colors or no
	//
	void CreateTreeView();
	void CreateListView();
	void CreateMarquee();
	BOOL PrintRTF(HWND hwnd, HDC hdc);
	BOOL LoadUserSettings();
	BOOL SaveUserSettings();
	void OnLogin();
	bool ParseUserName(const CString& user, CString& nick, CString& name, CString& ip);
	void CreateView(LPCTSTR name = NULL, ViewType type = VIEW_NONE);
public:
	ViewData* GetViewData();
	CView* GetActiveView();
	ViewData* GetViewDataByName(const CString& name);
	CView* GetViewByName(const CString& name);
	CView* CreateViewIfNoExist(LPCTSTR name, ViewType type);
	CView* CreatePrivateChannel(const CString& name);
	CView* CreateChannel(const CString& channel);
	bool ActivateViewByName(const CString& name);
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)
	CMainFrame() :m_sock((ISocketEvents*)this),m_tab(*this)
	{
		m_bInChannel = false;
		m_servers.Add(_T("irc.freenode.net"));
		m_servers.Add(_T("irc.gimp.org"));
		m_servers.Add(_T("irc.austnet.org"));
		m_servers.Add(_T("irc.brasirc.org"));
		m_servers.Add(_T("irc.efnet.org"));
		m_servers.Add(_T("irc.dal.net"));
		//
		m_server = _T("irc.freenode.net");
		m_NameOrChannel = m_server;
		m_bNoColors = false;
		//
		//m_CmdBar.m_hIconChildMaximized = LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAINFRAME));
	}
public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_MSGFROMBOTTOM, OnMsgFromBottom)
		MESSAGE_HANDLER(WM_CHILDDESTROY, OnChildDestroy)
		MESSAGE_HANDLER(WM_MDIACTIVATE, OnMDIActivate)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_FILE_SAVE,OnFileSave)
		COMMAND_ID_HANDLER(ID_FILE_PRINT,OnFilePrint)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_WINDOW_CASCADE, OnWindowCascade)
		COMMAND_ID_HANDLER(ID_WINDOW_TILE_HORZ, OnWindowTile)
		COMMAND_ID_HANDLER(ID_WINDOW_ARRANGE, OnWindowArrangeIcons)
		CHAIN_COMMANDS_MEMBER_ID_RANGE(m_bottom, ID_EDIT_CLEAR, ID_EDIT_FIND_PREVIOUS)
		COMMAND_ID_HANDLER(ID_EDIT_INCFONT, OnIncFont)
		COMMAND_ID_HANDLER(ID_EDIT_DECFONT, OnDecFont)
		NOTIFY_HANDLER(IDC_LISTVIEW, NM_DBLCLK, OnLVDoubleClick)
		NOTIFY_HANDLER(IDC_TREEVIEW, NM_DBLCLK, OnTVDoubleClick)
		NOTIFY_HANDLER(IDC_LISTVIEW,LVN_COLUMNCLICK, OnColumnClick)
		CHAIN_MDI_CHILD_COMMANDS()
		CHAIN_MSG_MAP(CUpdateUI<CMainFrame>)
		CHAIN_MSG_MAP(_baseClass)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnMsgFromBottom(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFilePrint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowTile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnIncFont(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDecFont(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTabSelChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
	LRESULT OnTabSelChanging(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
	LRESULT OnTabClose(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnMDIActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChildDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLVDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnTVDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnColumnClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);

	//
	virtual void OnSockError(int error);
	virtual void OnSockRead(int error);
	virtual void OnSockWrite(int error);
	virtual void OnSockConnect(int error);
	virtual void OnSockClose(int error);
	virtual void OnSockAccept(int error);
	virtual void OnSockOutOfBand(int error);
	virtual void OnSockConnecting(void);
	virtual void OnSockConnectTimeout(void);
	virtual void OnSockResolvingAddress(void);
	virtual void OnSockAddressResolved(int error);
};
