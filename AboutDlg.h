// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CAboutDlg : public CDialogImpl<CAboutDlg>
{
	CHyperLink m_hlWebPage;
	CHyperLink m_hlEmail;
public:
	enum { IDD = IDD_ABOUTBOX };

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

class CLoginDlg : public CDialogImpl<CLoginDlg>
{
	CComboBox m_combo;
public:
	CLoginDlg() {
		//
	}
	enum { IDD = IDD_DIALOG2 };

	CString m_nick;
	CString m_real;
	CString m_server;
	CString m_user;
	CString m_pass;
	CString m_JoinChannel;
	CStringArray m_servers;

	BEGIN_MSG_MAP(CAboutDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		m_combo.Attach(GetDlgItem(IDC_COMBO2));
		for (int i = 0; i < m_servers.GetSize(); i++) {
			m_combo.AddString(m_servers[i]);
		}
		m_combo.SelectString(0, m_server);
		CEdit edit = GetDlgItem(IDC_EDIT1);
		edit.AppendText(m_nick);
		edit = GetDlgItem(IDC_EDIT2);
		edit.AppendText(m_real);
		edit = GetDlgItem(IDC_EDIT3);
		edit.AppendText(m_user);
		edit = GetDlgItem(IDC_EDIT4);
		edit.AppendText(m_pass);
		edit = GetDlgItem(IDC_EDIT5);
		edit.AppendText(m_JoinChannel);

		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (wID == IDOK) {
			CEdit edit = GetDlgItem(IDC_EDIT1);

			edit.GetWindowText(m_nick);
			edit = GetDlgItem(IDC_EDIT2);
			edit.GetWindowText(m_real);
			edit = GetDlgItem(IDC_EDIT3);
			edit.GetWindowText(m_user);
			edit = GetDlgItem(IDC_EDIT4);
			edit.GetWindowText(m_pass);
			edit = GetDlgItem(IDC_EDIT5);
			edit.GetWindowText(m_JoinChannel);
			int nSel = m_combo.GetCurSel();
			m_combo.GetLBText(nSel, m_server);
		}
		EndDialog(wID);
		return 0;
	}
};

class CAutoCompleteDlg : \
	public CDialogImpl<CAutoCompleteDlg>,
	public CMessageFilter
{
public:
	enum { IDD = IDD_AUTOCOMPLETE };

	BEGIN_MSG_MAP(CAutoCompleteDlg)
		MESSAGE_HANDLER(WM_INITDIALOG,OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY,OnDestroy)
		MESSAGE_HANDLER(WM_CLOSE,OnClose)
		//MESSAGE_HANDLER(WM_KEYDOWN,OnKeyDown)
		COMMAND_CODE_HANDLER(LBN_DBLCLK,OnDoubleClick)
		COMMAND_CODE_HANDLER(LBN_SELCHANGE,OnSelChange)
	ALT_MSG_MAP(1)
		MESSAGE_HANDLER(WM_KEYDOWN,OnKeyDown)
		MESSAGE_HANDLER(WM_GETDLGCODE,OnGetDlgCode)
	END_MSG_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return IsDialogMessage(pMsg);
	}

	LRESULT OnInitDialog(UINT /*uMsg*/,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		CRect rc;
		GetClientRect(&rc);

		m_lb.SubclassWindow(GetDlgItem(IDC_LIST1));

		for (int i = 0; i < m_count; i++) {
			m_lb.AddString(cmds[i]);
		}

		CRect rcWindow;

		GetWindowRect(&rcWindow);

		MoveWindow(m_ptCaret.x ,m_ptCaret.y - rcWindow.Height(),rcWindow.Width(),
			rcWindow.Height());

		//m_lb.SetFocus();
		m_lb.SetCurSel(0);

		CMessageLoop* pLoop = _Module.GetMessageLoop();
		pLoop->AddMessageFilter(this);

		return 0;
	}
	LRESULT OnDestroy(UINT /*uMsg*/,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		pLoop->RemoveMessageFilter(this);
		return 0;
	}
	LRESULT OnGetDlgCode(UINT /*uMsg*/,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		return DLGC_WANTALLKEYS;
	}

	LRESULT OnDoubleClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int sel = m_lb.GetCurSel();
		int res = IDCANCEL;

		if (sel >= 0) {
			m_lb.GetText(sel,m_res);
			res = IDOK;
		}
		EndDialog(res);
		return 0;
	}
	LRESULT OnSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int sel = m_lb.GetCurSel();

		if (sel >= 0) {
			CString str;
			CString cmd;
			//LPCTSTR cmds[13] = { _T("JOIN"),_T("PART"),_T("TOPIC"),_T("MODE"),_T("NAMES"),_T("INVITE"),_T("PRIVMSG"),_T("NOTICE"),_T("QUIT"),
			//_T("TIME"), _T("WHO"), _T("WHOIS"), _T("WHOWAS")
			//};		

			m_lb.GetText(sel, str);
			for (int i = 0; i < m_count; i++) {
				if (!str.CompareNoCase(cmds[i])) {
					cmd = cmds[i];
					if (cmd == _T("JOIN")) {
						cmd += _T(" channel");
					}
					else if (cmd == _T("PART")) {
						cmd += _T(" channel [:msg]");
					}
					else if (cmd == _T("TOPIC")) {
						cmd += _T(" channel");
					}
					else if (cmd == _T("MODE")) {
						cmd += _T(" channel");
					}
					else if (cmd == _T("NAMES")) {
						cmd += _T(" channel");
					}
					else if (cmd == _T("INVITE")) {
						cmd += _T(" nick");
					}
					else if (cmd == _T("PRIVMSG")) {
						cmd += _T(" channel or nick msg");
					}
					else if (cmd == _T("NOTICE")) {
						cmd += _T(" channel or nick msg");
					}
					else if (cmd == _T("WHO") || cmd == _T("WHOIS") || cmd == _T("WHOWAS")) {
						cmd += _T(" nick");
					}
					else if (cmd == _T("AWAY")) {
						cmd += _T(" [msg]");
					}
					else if (cmd == _T("QUIT")) {
						cmd += _T(" [msg]");
					}
					break;
				}
			}
			m_status.SetText(0,cmd);
		}
		return 0;
	}
	LRESULT OnClose(UINT /*uMsg*/,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		EndDialog(IDCANCEL);
		return 0;
	}
	LRESULT OnKeyDown(UINT /*uMsg*/,WPARAM wParam,LPARAM lParam,BOOL& bHandled)
	{
		bHandled = TRUE;
		if (wParam == VK_ESCAPE)
			PostMessage(WM_CLOSE);
		else if (wParam == VK_RETURN) {
			BOOL b;
			OnDoubleClick(0,0,NULL,b);
		}
		else
			bHandled = FALSE;
		return 0;
	}

	CAutoCompleteDlg(CString& res,const  CPoint& ptCaret, HWND hWndStatusBar) :m_res(res),m_ptCaret(ptCaret),m_lb(this,1),
		m_status(hWndStatusBar)
	{
		//
	}
	~CAutoCompleteDlg()
	{
		//
	}

private:
	CContainedWindowT<CListBox> m_lb;
	LPCTSTR cmds[14] = { _T("JOIN"),_T("PART"),_T("TOPIC"),_T("MODE"),_T("NAMES"),_T("INVITE"),_T("PRIVMSG"),_T("NOTICE"),_T("QUIT"),
		_T("TIME"),_T("WHO"),_T("WHOIS"),_T("WHOWAS"),_T("LIST")};
	const int m_count = 14;
	CString& m_res;
	CPoint m_ptCaret;
	CStatusBarCtrl m_status;
};


