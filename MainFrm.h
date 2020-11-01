// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "Path.h"
#include "IIRCEvents.h"
#include "IIrc.h"
#include "usermsgs.h"
#include "View.h"
#include "IFontOptions.h"
#ifdef NO5_SSL
#include "openssl/ssl.h"
#include "openssl/ssl3.h"
#include "openssl/sslerr.h"
#include "openssl/tls1.h"
#include "openssl/err.h"
#endif

class CMainFrame;

class CChannelsViewFrame :
	public CFrameWindowImpl<CChannelsViewFrame>,
	public CUpdateUI<CChannelsViewFrame>,
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(_T("ChannelsViewFrame"), IDR_CHANNELSVIEW)
	CListViewCtrl &m_lv;
	CMainFrame* m_pf;
	CChannelsViewFrame(CListViewCtrl& lv, CMainFrame *pFrame) :m_lv(lv),m_pf(pFrame)
	{
		//
	}
	typedef  CFrameWindowImpl<CChannelsViewFrame> _baseClass;
public:
	BEGIN_MSG_MAP(CChannelsViewFrame)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		COMMAND_ID_HANDLER(ID_FILE_SAVE, OnFileSave)
		COMMAND_ID_HANDLER(ID_FILE_OPEN, OnFileOpen)
		COMMAND_ID_HANDLER(ID_FILE_UPDATE, OnFileUpdate)
		FORWARD_NOTIFICATIONS()
		//REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CUpdateUI<CChannelsViewFrame>)
		CHAIN_MSG_MAP(CFrameWindowImpl< CChannelsViewFrame>)
	END_MSG_MAP()

	BEGIN_UPDATE_UI_MAP(CChannelsViewFrame)
	END_UPDATE_UI_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileUpdate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

class CMySocket : public CSimpleSocket
{
#ifdef NO5_SSL
	SSL_CTX* ctx;
public:
	SSL* ssl;
#endif
public:
	CMySocket(ISocketEvents* pSink) :CSimpleSocket(pSink)
	{
#ifdef NO5_SSL
		ctx = NULL;
		ssl = NULL;
#endif
	}
	virtual ~CMySocket()
	{
#ifdef NO5_SSL
		if (ctx != NULL) {
			SSL_CTX_free(ctx);
			SSL_free(ssl);
		}
#endif
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
#ifdef NO5_SSL
	void OutputSSLError(int res)
	{
		res = SSL_get_error(ssl, res);
		switch (res) {
		case SSL_ERROR_NONE:
			ATLTRACE(_T("SSL_ERROR_NONE\n"));
			break;
		case SSL_ERROR_ZERO_RETURN:
			ATLTRACE(_T("SSL_ERROR_ZERO_RETURN\n"));
			break;
		case SSL_ERROR_WANT_READ:
			ATLTRACE(_T("SSL_ERROR_WANT_READ\n"));
			//_OnRead(0);
			break;
		case SSL_ERROR_WANT_WRITE:
			ATLTRACE(_T("SSL_ERROR_WANT_WRITE\n"));
			//_OnWrite(0);
			break;
		default:
			char buf[256] = { 0 };
			ERR_error_string_n(ERR_get_error(), buf, sizeof(buf) - 1);
			CString s = buf;
			ATLTRACE(_T("Error %s\n"), s);
			break;
		}
	}
	BOOL InitSSL()
	{
		int res;
		res = SSL_library_init();
		ATLASSERT(res == 1);
		res = SSL_load_error_strings();
		ATLASSERT(res == 1);
		const SSL_METHOD* m = TLS_client_method();
		ctx = SSL_CTX_new(m);
		ATLASSERT(ctx != NULL);
		res = SSL_CTX_use_certificate_file(ctx, "freenode.pem", SSL_FILETYPE_PEM);
		if (res != 1) {
			OutputSSLError(res);
		}
		res = SSL_CTX_use_PrivateKey_file(ctx, "fd.key", SSL_FILETYPE_PEM);
		if (res != 1) {
			OutputSSLError(res);
		}
		res = SSL_CTX_load_verify_locations(ctx, "freenode.pem", NULL);
		ATLASSERT(res == 1);
		SSL_CTX_set_verify_depth(ctx, 1);
		ssl = SSL_new(ctx);
		if (SSL_get_verify_result(ssl) == X509_V_OK) {
			SSL_set_options(ssl, SSL_MODE_ASYNC);
			SSL_set_connect_state(ssl);
			return TRUE;
		}
		else
			return FALSE;
	}
	virtual int RecvSSL(char* buf, int len, int flags = 0)
	{
		//size_t read = 0;
		m_canread = false;
		int res = SSL_read(ssl, buf, len);
		//ATLASSERT(res >= 0);
		if (res < 0) {
			OutputSSLError(res);
		}
		return res;
	}
	virtual int SendSSL(char* buf, int len, int flags = 0)
	{
		int res;
		//size_t written = 0;

		m_canwrite = false;
		res = SSL_write(ssl, buf, len);
		if (res != SOCKET_ERROR) {
			// send will only send more notifications when send
			// fails with error WSAEWOULDBLOCK, so we set the flag
			// here
			m_canwrite = true;
		}
		else if (WSAEWOULDBLOCK != GetLastError()) {
			m_canwrite = true;
			// since we do not always check the return value for this
			// error code ...
			//ATLASSERT(0);
			_OnError(GetLastError());
		}
		else {
			// an FD_WRITE event will be sent when we can write again,
			// for now, we cant
		}
		return res;
	}
	virtual int SendStringSSL(const char* buf)
	{
		return SendSSL(const_cast<char*>(buf), strlen(buf));
	}
#endif
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
		NOTIFY_HANDLER(IDC_TREEVIEW, NM_RCLICK, OnTVRightClick)
		NOTIFY_HANDLER(IDC_TREEVIEW, TVN_GETINFOTIP,OnTVGetInfoTip)
		//FORWARD_NOTIFICATIONS()
		//REFLECT_NOTIFICATIONS()
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
	LRESULT OnTVRightClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnTVGetInfoTip(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
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
	public ISocketEvents,
	public IIRCEvents,
	public IIrc
{
	typedef CTabbedMDIFrameWindowImpl<CMainFrame, CMyTabbedMDIClient> _baseClass;
	typedef CDotNetTabCtrl<CTabViewTabItem> _tabControl;
public:
	TTabCtrl& TabCtrl = GetMDITabCtrl();
	struct MarqueeOptions
	{
		COLORREF fore;
		COLORREF back;
		int Elapse;
		MarqueeOptions()
		{
			fore = 0xffffff;
			back = 0x008000;
			Elapse = 4;
		}
	};
private:
	CWindow  TabOwnerParent = GetTabOwnerParent();
	CTabbedMDICommandBarCtrl m_CmdBar;
	CHorSplitterWindow m_splitter;
	CBottom m_bottom;
	CMyTabbedChildWindow m_tab;
	CSplitterWindow m_vsplitter;
	CFont m_font;
	CNo5TreeCtrl m_tv;
	CListViewCtrl m_lv;
	CMarqueeWnd m_marquee;
	CChannelsViewFrame m_ChannelsView;
	CMySocket m_sock;
	CString m_server;
	CPort m_port;
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
	CImageListManaged m_il;
	time_t m_t;	// PING request time
	BOOL m_bAllowCTCP; // answer CTCP queries?
	CString m_userinfo;
	MarqueeOptions mo;
	bool m_bssl;
	IFontOptions* m_pfo;
	
	//
	void CreateTreeView();
	void CreateListView();
	void CreateMarquee();
	void CreateImageList();
	BOOL PrintRTF(HWND hwnd, HDC hdc);
	BOOL LoadUserSettings();
	BOOL SaveUserSettings();
	void OnLogin();
	bool ParseUserName(const CString& user, CString& nick, CString& name, CString& ip);
	void CreateView(LPCTSTR name = NULL, ViewType type = VIEW_NONE);
	CString GetTimeString() const;
public:
	bool m_bDarkMode;
	//
	ViewData* GetViewData();
	CView* GetActiveView();
	ViewData* GetViewDataByName(LPCTSTR name);
	CView* GetViewByName(LPCTSTR name);
	CView* CreateViewIfNoExist(LPCTSTR name, ViewType type);
	CView* CreatePrivateChannel(LPCTSTR name);
	CView* CreateChannel(LPCTSTR name);
	bool ActivateViewByName(LPCTSTR name);
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)
	CMainFrame() :m_sock((ISocketEvents*)this),m_tab(*this),m_ChannelsView(m_lv,this),m_bottom(*this)
	{
		m_bInChannel = false;
		m_servers.Add(_T("irc.freenode.net"));
		m_servers.Add(_T("irc.gimp.org"));
		m_servers.Add(_T("irc.austnet.org"));
		m_servers.Add(_T("irc.brasirc.org"));
		m_servers.Add(_T("irc.efnet.org"));
		m_servers.Add(_T("irc.dal.net"));
		m_servers.Add(_T("localhost"));
		//
		m_server = _T("irc.freenode.net");
		m_NameOrChannel = m_server;
		m_bNoColors = false;
		m_bAllowCTCP = FALSE;
		m_bDarkMode = false;
		m_bssl = false;
		//
		//m_CmdBar.m_hIconChildMaximized = LoadIcon(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAINFRAME));
	}
	~CMainFrame()
	{
		
	}
public:

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_MARQUEE, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_DARKMODE,UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_MSGFROMBOTTOM, OnMsgFromBottom)
		MESSAGE_HANDLER(WM_CHILDDESTROY, OnChildDestroy)
		MESSAGE_HANDLER(WM_MDIACTIVATE, OnMDIActivate)
		MESSAGE_HANDLER(WM_CHANNELSFILEOPEN,OnChannelsFileOpen)
		MESSAGE_HANDLER(WM_CHANNELSFILESAVE, OnChannelsFileSave)
		MESSAGE_HANDLER(WM_CHANNELSFILEUPDATE, OnChannelsFileUpdate)
		MESSAGE_HANDLER(WM_ONFONTCHANGE,OnFontChange)
		MESSAGE_HANDLER(WM_ONFONTSIZECHANGE, OnFontSizeChange)
		MESSAGE_HANDLER(WM_FINDUSER,OnFindUser)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_FILE_SAVE,OnFileSave)
		COMMAND_ID_HANDLER(ID_FILE_PRINT,OnFilePrint)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_VIEW_MARQUEE, OnViewMarquee)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_WINDOW_CASCADE, OnWindowCascade)
		COMMAND_ID_HANDLER(ID_WINDOW_TILE_HORZ, OnWindowTile)
		COMMAND_ID_HANDLER(ID_WINDOW_ARRANGE, OnWindowArrangeIcons)
		COMMAND_ID_HANDLER(ID_VIEW_OPTIONS,OnViewOptions)
		COMMAND_ID_HANDLER(ID_VIEW_DARKMODE, OnViewDarkMode)
		COMMAND_ID_HANDLER(ID_EDIT_INCFONT, OnIncFont)
		COMMAND_ID_HANDLER(ID_EDIT_DECFONT, OnDecFont)
		COMMAND_HANDLER(IDC_MARQUEE1, MQN_CHANGED,OnMarqueeChange)
		NOTIFY_HANDLER(IDC_LISTVIEW, NM_DBLCLK, OnLVDoubleClick)
		NOTIFY_HANDLER(IDC_TREEVIEW, NM_DBLCLK, OnTVDoubleClick)
		NOTIFY_HANDLER(IDC_LISTVIEW,LVN_COLUMNCLICK, OnColumnClick)
		NOTIFY_HANDLER(IDC_TREEVIEW,NM_RCLICK,OnTVRightClick)
		NOTIFY_HANDLER(IDC_TREEVIEW, TVN_GETINFOTIP, OnTVGetInfoTip)
		CHAIN_COMMANDS_MEMBER_ID_RANGE(m_bottom, ID_EDIT_CLEAR, ID_EDIT_FIND_PREVIOUS)
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
	LRESULT OnChannelsFileOpen(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChannelsFileSave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnChannelsFileUpdate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnFontChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnFontSizeChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnFindUser(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFilePrint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewMarquee(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowTile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnIncFont(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDecFont(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewOptions(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnViewDarkMode(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnMarqueeChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnTabSelChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
	LRESULT OnTabSelChanging(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/);
	LRESULT OnTabClose(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnMDIActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnChildDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLVDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnTVDoubleClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnColumnClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnTVRightClick(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);
	LRESULT OnTVGetInfoTip(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/);

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
	//
	virtual void OnChannelList(LPCTSTR channel, LPCTSTR users, LPCTSTR topic);
	virtual void OnBeginningOfList();
	virtual void OnEndOfList();
	virtual void OnChannelMode(LPCTSTR channel, LPCTSTR modes);
	virtual void OnUserMode(LPCTSTR user, LPCTSTR modes);
	virtual void OnChannelCreationTime(LPCTSTR channel, time_t time);
	virtual void OnBeginMOTD(LPCTSTR channel);
	virtual void OnMOTD(LPCTSTR channel, LPCTSTR msg);
	virtual void OnEndMOTD(LPCTSTR channel);
	virtual void OnNoTopic(LPCTSTR channel);
	virtual void OnTopic(LPCTSTR channel, LPCTSTR topic);
	virtual void OnWhoSetTheTopic(LPCTSTR channel, LPCTSTR user, time_t time);
	virtual void OnNamesInChannel(LPCTSTR channel,const CSimpleArray<CString>& args);
	virtual void OnNamesEnd(LPCTSTR channel);
	virtual void OnChannelMsg(LPCTSTR channel,LPCTSTR user, LPCTSTR msg);
	virtual void OnPrivateMsg(LPCTSTR from, LPCTSTR msg);
	virtual void OnUserQuit(LPCTSTR channel, LPCTSTR user, LPCTSTR msg);
	virtual void OnUserJoin(LPCTSTR channel, LPCTSTR user);
	virtual void OnUserPart(LPCTSTR channel, LPCTSTR user, LPCTSTR msg);
	virtual void OnNotice(LPCTSTR user, LPCTSTR msg);
	virtual void OnPing(LPCTSTR code);
	virtual void OnAction(LPCTSTR channel,LPCTSTR from, LPCTSTR msg);
	virtual void OnUnknownCmd(LPCTSTR line);
	//
	virtual void SendChannelMsg(LPCTSTR channel, LPCTSTR msg);
	virtual void SendPrivateMsg(LPCTSTR to, LPCTSTR msg);
	virtual void SendNoticeMsg(LPCTSTR to, LPCTSTR msg);
	virtual void JoinChannel(LPCTSTR channel);
	virtual void LeaveChannel(LPCTSTR channel, LPCTSTR msg);
	virtual void Quit(LPCTSTR msg);
	virtual void SendPass(LPCTSTR pass);
	virtual void SendNick(LPCTSTR nick);
	virtual void SendUser(LPCTSTR user,LPCTSTR realname);
	virtual void ListChannels();
	virtual void ListChannelNames(LPCTSTR channel);
	virtual void GetTopic(LPCTSTR channel);
	virtual void GetMode(LPCTSTR NameOrChannel);
	virtual void Pong(LPCTSTR code);
	virtual void RequestVersion(LPCTSTR from);
	virtual void RequestUserinfo(LPCTSTR from);
	virtual void RequestPing(LPCTSTR from);
	virtual void RequestTime(LPCTSTR from);
	virtual void SendAction(LPCTSTR channel,LPCTSTR msg);
	virtual void AnswerVersionRequest(LPCTSTR from);
	virtual void AnswerUserinfoRequest(LPCTSTR from);
	virtual void AnswerPingRequest(LPCTSTR from);
	virtual void AnswerTimeRequest(LPCTSTR from);
	virtual void WhoIs(LPCTSTR nick);
	virtual void Who(LPCTSTR nick);
	virtual void WhoWas(LPCTSTR nick);
};
