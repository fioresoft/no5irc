// View.h : interface of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CView : public CWindowImpl<CView, CNo5RichEdit>, public CRichEditCommands<CView>
{
public:
	DECLARE_WND_SUPERCLASS(NULL, CRichEditCtrl::GetWndClassName())

	BOOL PreTranslateMessage(MSG* pMsg);
	int AppendTextIrc(LPCTSTR lpstrText);
	void ResetFormat();

	BEGIN_MSG_MAP(CView)
		MESSAGE_HANDLER(WM_CREATE,OnCreate)
		CHAIN_MSG_MAP_ALT(CRichEditCommands<CView>,1)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
};

class CBottom;
class CMainFrame;

class CBottomClient : public CWindowImpl<CBottomClient, CNo5RichEdit>,
	public CRichEditCommands<CBottomClient>
{
	CBottom* m_pBottom;
	CMainFrame &m_frame;
public:
	CBottomClient(CBottom* pBottom,CMainFrame &frame) :m_pBottom(pBottom),m_frame(frame)
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

class CBottom : public CFrameWindowImpl<CBottom>,
	public CUpdateUI<CBottom>,
	public CMessageFilter, public CIdleHandler
{
public:
	CMainFrame &m_frame;
	CStatusBarCtrl m_status;
	CBottomClient m_client;
	CFontPickerComboCtrl m_cbFonts;
	CColorPickerComboCtrl m_cbFore;
	CColorPickerComboCtrl m_cbBack;
	CToolBarCtrl m_tb;
	HACCEL m_hAccel;
public:
	DECLARE_FRAME_WND_CLASS(_T("NO5BottomFrame"),IDR_BOTTOM_FRAME)

	CBottom(CMainFrame &frame):m_client(this,frame),m_frame(frame)
	{
		m_hAccel = NULL;
	}

	BEGIN_UPDATE_UI_MAP(CBottom)
	END_UPDATE_UI_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();
	BOOL OnEditComplete(CString& cmd);

	BEGIN_MSG_MAP(CBottom)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		COMMAND_HANDLER(IDC_COMBO_FONTS, CBN_SELENDOK, OnFontsSelChange)
		COMMAND_ID_HANDLER(ID_EDIT_BOLD,OnBold)
		COMMAND_ID_HANDLER(ID_EDIT_ITALIC, OnItalic)
		COMMAND_ID_HANDLER(ID_EDIT_UNDERLINE, OnUnderline)
		COMMAND_HANDLER(IDC_COMBO_FORE, CBN_SELENDOK, OnForeSelChange)
		COMMAND_HANDLER(IDC_COMBO_BACK, CBN_SELENDOK, OnBackSelChange)
		//CHAIN_COMMANDS_MEMBER_ID_RANGE(m_client, ID_EDIT_CLEAR, ID_EDIT_FIND_PREVIOUS)
		CHAIN_CLIENT_COMMANDS()
		REFLECT_NOTIFICATIONS();
		CHAIN_MSG_MAP(CFrameWindowImpl<CBottom>)
	END_MSG_MAP()

	void CreateClient();
	void CreateToolBar();
	void CreateComboFonts();
	void CreateColorCombos();
	void DisableFormat();
	void EnableFormat();

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnFontsSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBold(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnItalic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnUnderline(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnForeSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBackSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	
};
