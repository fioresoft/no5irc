// View.h : interface of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "TextStream.h"

class CBottom;
class CMainFrame;


class CView : public CWindowImpl<CView, CNo5RichEdit>, public CRichEditCommands<CView>,
	CMessageFilter
{
	HACCEL m_hAccel;
public:
	DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	//virtual int AppendText(LPCTSTR text, BOOL bCanUndo = FALSE);
	CTextStream m_ts;
	CMainFrame& m_frame;

	BEGIN_MSG_MAP(CView)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		CHAIN_MSG_MAP_ALT(CRichEditCommands<CView>,1)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	CView(CMainFrame& frame) :m_frame(frame)
	{
		//
	}
	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual void AppendTextIrc(LPCTSTR lpstrText);
	virtual void ResetFormat();
};

class CHtmlView : public CHtmlEditImpl<CHtmlView>
{
public:
	CTextStream m_ts;
public:
	BEGIN_MSG_MAP(CHtmlView)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		CHAIN_MSG_MAP(CHtmlEditImpl<CHtmlView>)
	END_MSG_MAP()
	LRESULT OnCreate(UINT uMsg, WPARAM, LPARAM, BOOL& bHandled)
	{
		CHtmlEditImpl<CHtmlView>::OnCreate(uMsg, 0, 0, bHandled);
		m_event.WaitWithMsgLoop();
		return 0;
	}
	virtual void AppendTextIrc(LPCTSTR lpstrText);
	virtual void ResetFormat()
	{
		//
	}
	void SetTextColor(COLORREF color,int selection = 0)
	{
		//m_doc.ChangeStyleColor(_T("div"), color);
	}
	void SetBackgroundColor(COLORREF color)
	{
		//m_doc.ChangeStyleBkColor(_T("div"), color);
	}
	void SetTextBkColor(COLORREF color,int selection = 0)
	{
		//m_doc.ChangeStyleBkColor(_T("div"), color);
	}
	void SetTextFontName(LPCTSTR name, int sel = 0)
	{
		//m_doc.ChangeStyleFont(_T("div"), name);
	}
	void SetSelEnd()
	{
		
	}
};

// Child window class that will be subclassed for hosting Active X control
class CChildWindow : public CWindowImpl<CChildWindow>
{
public:
	BEGIN_MSG_MAP(CChildWindow)
	END_MSG_MAP()
};

class CBottom;
class CMainFrame;

class CBottomClient : public CWindowImpl<CBottomClient, CNo5RichEdit>,
	public CRichEditCommands<CBottomClient>
{
	CBottom* m_pBottom;
	CMainFrame &m_frame;
	CSimpleArray<CString> m_lines;
	int m_nPos;
	//
	int FindNames(LPCTSTR partial, CStringArray *pres);
public:
	CBottomClient(CBottom* pBottom,CMainFrame &frame) :m_pBottom(pBottom),m_frame(frame),m_nPos(0)
	{
		//
	}
	DECLARE_WND_SUPERCLASS(_T("BottomFrameClient"),CNo5RichEdit::GetWndClassName())
	BEGIN_MSG_MAP(CBottomClient)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		CHAIN_MSG_MAP_ALT(CRichEditCommands<CBottomClient>,1)
	END_MSG_MAP()

	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled);
};



class CMyComboBox : public CColorPickerComboCtrl
{
	CToolTipCtrl& m_tt;
public:
	DECLARE_WND_SUPERCLASS(NULL, CColorPickerComboCtrl::GetWndClassName())
	CMyComboBox(CToolTipCtrl& tt) :m_tt(tt)
	{
		//
	}

	BEGIN_MSG_MAP(CMyToolTip)
		MESSAGE_RANGE_HANDLER(WM_MOUSEFIRST,WM_MOUSELAST,OnMouseEvent)
		CHAIN_MSG_MAP(CColorPickerComboCtrl)
	END_MSG_MAP()
	LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
	{
		CMessageLoop* loop = _Module.GetMessageLoop();
		MSG msg = loop->m_msg;
		m_tt.RelayEvent(&msg);
		bHandled = FALSE;
		return 0;
	}
};

template <class TTabCtrl = CDotNetTabCtrl<CTabViewTabItem> >
class CFrameTabbedChildWindow :
	public CTabbedFrameImpl<CFrameTabbedChildWindow<TTabCtrl>, TTabCtrl, CFrameWindowImpl<CFrameTabbedChildWindow<TTabCtrl>, ATL::CWindow,
	TabbedChildWindowWinTraits> >
{
protected:
	typedef CFrameTabbedChildWindow<TTabCtrl> thisClass;
	typedef CTabbedFrameImpl<CFrameTabbedChildWindow<TTabCtrl>, TTabCtrl, CFrameWindowImpl<CFrameTabbedChildWindow<TTabCtrl>, ATL::CWindow,
		TabbedChildWindowWinTraits> > baseClass;

	// Constructors
public:
	CFrameTabbedChildWindow(bool bReflectNotifications = false) :
		baseClass(bReflectNotifications)
	{
	}

	// Message Handling
public:
	DECLARE_FRAME_WND_CLASS_EX(_T("FrameTabbedChildWindow"), 0, 0, COLOR_APPWORKSPACE)

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		//if(baseClass::PreTranslateMessage(pMsg))
		//	return TRUE;

		//return m_view.PreTranslateMessage(pMsg);

		HWND hWndFocus = ::GetFocus();
		if (m_hWndActive != NULL && ::IsWindow(m_hWndActive) &&
			(m_hWndActive == hWndFocus || ::IsChild(m_hWndActive, hWndFocus)))
		{
			//active.PreTranslateMessage(pMsg);
			if (::SendMessage(m_hWndActive, WM_FORWARDMSG, 0, (LPARAM)pMsg))
			{
				return TRUE;
			}
		}

		return FALSE;
	}


	BEGIN_MSG_MAP(thisClass)
		CHAIN_MSG_MAP(baseClass)
	END_MSG_MAP()
};


class CBottom : \
	public CFrameTabbedChildWindow<CDotNetTabCtrl<CTabViewTabItem>>,
	public CUpdateUI<CBottom>,
	public CMessageFilter, public CIdleHandler
{
	typedef CFrameTabbedChildWindow<CDotNetTabCtrl<CTabViewTabItem>> _baseClass;
public:
	CMainFrame &m_frame;
	CStatusBarCtrl m_status;
	CBottomClient m_client;
	CFontPickerComboCtrl m_cbFonts;
	CComboBox m_cbSize;
	CMyComboBox m_cbFore;
	CColorPickerComboCtrl m_cbBack;
	CToolBarCtrl m_tb;
	CToolTipCtrl m_tt;
	HACCEL m_hAccel;
	CReBarCtrl m_rebar;
public:
	DECLARE_FRAME_WND_CLASS(_T("NO5BottomFrame"),IDR_BOTTOM_FRAME)

	CBottom(CMainFrame &frame):m_client(this,frame),m_frame(frame),m_cbFore(m_tt)
	{
		m_hAccel = NULL;
	}
	virtual ~CBottom()
	{
		m_tt.DestroyWindow();
		m_rebar.DestroyWindow();
		m_tb.DestroyWindow();
	}

	BEGIN_UPDATE_UI_MAP(CBottom)
	END_UPDATE_UI_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
	BOOL OnEditComplete(CString& cmd);

	BEGIN_MSG_MAP(CBottom)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		COMMAND_HANDLER(IDC_COMBO_FONTS, CBN_SELENDOK, OnFontsSelChange)
		COMMAND_HANDLER(IDC_COMBO_SIZE, CBN_SELENDOK, OnSizeSelChange)
		COMMAND_ID_HANDLER(ID_EDIT_BOLD, OnBold)
		COMMAND_ID_HANDLER(ID_EDIT_ITALIC, OnItalic)
		COMMAND_ID_HANDLER(ID_EDIT_UNDERLINE, OnUnderline)
		COMMAND_HANDLER(IDC_COMBO_FORE, CBN_SELENDOK, OnForeSelChange)
		COMMAND_HANDLER(IDC_COMBO_BACK, CBN_SELENDOK, OnBackSelChange)
		NOTIFY_CODE_HANDLER(CTCN_SELCHANGE, OnSelChange)
		NOTIFY_CODE_HANDLER(TTN_SHOW,OnShowTT)
		NOTIFY_HANDLER(IDC_LV_TRANSFERS,NM_RCLICK,OnLVTransfersRightClick)
		CHAIN_COMMANDS_MEMBER_ID_RANGE(m_client, ID_EDIT_CLEAR, ID_EDIT_FIND_PREVIOUS)
		//CHAIN_CLIENT_COMMANDS()
		REFLECT_NOTIFICATIONS_MSG_FILTERED(WM_DRAWITEM)
		REFLECT_NOTIFICATIONS_MSG_FILTERED(WM_MEASUREITEM)
		REFLECT_NOTIFICATIONS_MSG_FILTERED(WM_COMPAREITEM)
		REFLECT_NOTIFICATIONS_MSG_FILTERED(WM_DELETEITEM)
		//FORWARD_NOTIFICATIONS()
		CHAIN_MSG_MAP(_baseClass)
	END_MSG_MAP()

	void CreateClient();
	void CreateToolBar();
	void CreateComboFonts();
	void CreateComboSize();
	void CreateColorCombos();
	void DisableFormat();
	void EnableFormat();
	HWND CreateToolTip();

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnFontsSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnSizeSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBold(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnItalic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnUnderline(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnForeSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBackSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnShowTT(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
	{
		//m_tt.Popup();
		return 0;
	}
	LRESULT OnSelChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled);
	LRESULT OnLVTransfersRightClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
	int GetDesiredHeight();
	
};
