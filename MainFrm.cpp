// MainFrm.cpp : implmentation of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"
#include "View.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "usermsgs.h"
#define IDC_BAND_MARQUEE	100

static BOOL CALLBACK EnumChildGetDataByName(HWND hWnd, LPARAM lParam);
static BOOL CALLBACK EnumChildActivate(HWND hWnd, LPARAM lParam);
static int CALLBACK CompareLVItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

LRESULT CMyTabbedChildWindow::OnLVDoubleClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	return m_frame.OnLVDoubleClick(idCtrl, pnmh, bHandled);
}
LRESULT CMyTabbedChildWindow::OnTVDoubleClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	return m_frame.OnTVDoubleClick(idCtrl, pnmh, bHandled);
}
LRESULT CMyTabbedChildWindow::OnColumnClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	return m_frame.OnColumnClick(idCtrl, pnmh, bHandled);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	if (_baseClass::PreTranslateMessage(pMsg))
		return TRUE;

	HWND hWnd = MDIGetActive();
	if(hWnd != NULL)
		return (BOOL)::SendMessage(hWnd, WM_FORWARDMSG, 0, (LPARAM)pMsg);

	return FALSE;
}

BOOL CMainFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}

LRESULT CMainFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// create command bar window
	HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
	// attach menu
	m_CmdBar.AttachMenu(GetMenu());
	// load command bar images
	m_CmdBar.LoadImages(IDR_MAINFRAME);
	// remove old menu
	SetMenu(NULL);

	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndCmdBar);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	// marquee
	CreateMarquee();
	AddSimpleReBarBandCtrl(m_hWndToolBar, m_marquee, IDC_BAND_MARQUEE,
		NULL,	// title
		TRUE,	// new row
		0,		// cx
		TRUE);	// full width always
	m_marquee.AddItem(_T("Welcome to NO5 IRC"));

	CreateSimpleStatusBar();

	CreateMDIClient();
	m_CmdBar.SetMDIClient(m_hWndMDIClient);

	UIAddToolBar(hWndToolBar);
	UISetCheck(ID_VIEW_TOOLBAR, 1);
	UISetCheck(ID_VIEW_STATUS_BAR, 1);

	m_path.SetPath(PATH_MODULE);
	m_path = m_path.GetLocation();
	BOOL res = LoadUserSettings();
	ATLASSERT(res);

	m_splitter.Create(m_hWnd, 0, 0, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, WS_EX_CLIENTEDGE);
	m_vsplitter.Create(m_splitter, 0, 0, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, WS_EX_CLIENTEDGE);
	m_hWndClient = m_splitter;
	//m_hWndClient = NULL;
	// left pane
	//m_tab.ModifyTabStyles(0, CTCS_CLOSEBUTTON); // tem que ser antes do Create
	m_tab.Create(m_vsplitter, 0, 0, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, WS_EX_CLIENTEDGE);

	// botttom
	m_bottom.Create(m_splitter, 0, 0, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NOHIDESEL | ES_WANTRETURN|ES_SUNKEN, WS_EX_CLIENTEDGE);
	m_bottom.m_frame = *this;
	m_bottom.m_status = m_hWndStatusBar;
	//
	CreateTreeView();
	CreateListView();
	m_tab.GetTabCtrl().SetCurSel(1);
	//
	m_splitter.SetSplitterPanes(m_vsplitter, m_bottom);
	m_vsplitter.SetSplitterPanes(m_tab, m_hWndMDIClient);
	m_splitter.SetSplitterPosPct(80);
	m_vsplitter.SetSplitterPosPct(40);
	UpdateLayout(TRUE);

	//SetTabOwnerParent(m_hWnd);

	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);
	pLoop->AddMessageFilter(&m_bottom);

	return 0;
}

LRESULT CMainFrame::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	m_CmdBar.AttachMenu(NULL);

	BOOL res = SaveUserSettings();
	ATLASSERT(res);

	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);
	pLoop->RemoveMessageFilter(&m_bottom);

	bHandled = FALSE;
	return 1;
}

LRESULT CMainFrame::OnChildDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	BOOL bClose = TRUE;

	ViewData* p = GetViewData();
	if (p->type == VIEW_CHANNEL) {
		if (m_sock.IsConnected()) {
			if (/*m_bInChannel &&*/ m_NameOrChannel[0] == '#') {
				CString cmd = _T("PART ");
				cmd += m_NameOrChannel;
				cmd += _T("\r\n");
				m_sock.Send(CT2A(cmd), cmd.GetLength());

				// deletes the channel tree node
				m_tv.DeleteItem(m_NameOrChannel, TRUE, TRUE);
			}
		}
	}
	if (p->type == VIEW_SERVER && m_sock.IsConnected()) {
		int res = MessageBox(_T("Disconnect from server?"), L"", MB_OKCANCEL);
		if (res == IDCANCEL)
			bClose = FALSE;
		else {
			m_sock.Shutdown();
		}
	}
	return (LRESULT)bClose;
}

LRESULT CMainFrame::OnMDIActivate(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CWindow wnd = (HWND)lParam;
	ViewData* p = NULL;

	p = (ViewData*)wnd.SendMessage(WM_CHILDGETDATA);
	ATLASSERT(p && (!p->name.IsEmpty()) && p->pView);

	m_NameOrChannel = p->name;
	if (p->type == VIEW_CHANNEL)
		m_bInChannel = true;
	else
		m_bInChannel = false;
	return 0;

}

LRESULT CMainFrame::OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	PostMessage(WM_CLOSE);
	return 0;
}

LRESULT CMainFrame::OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	OnLogin();
	CreateView(m_server, VIEW_SERVER);

	return 0;
}

LRESULT CMainFrame::OnTabSelChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	NMCTC2ITEMS* pData = (LPNMCTC2ITEMS)pnmh;
	pData->iItem2; // new item selected
	ATLTRACE(_T("item %d selected\n"), pData->iItem2);
	return 0;
}
LRESULT CMainFrame::OnTabSelChanging(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
{
	NMCTC2ITEMS* pData = (LPNMCTC2ITEMS)pnmh;
	pData->iItem2; // new item selected
	ATLTRACE(_T("item %d selected\n"), pData->iItem2);
	return 0;
}
LRESULT CMainFrame::OnTabClose(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}


LRESULT CMainFrame::OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static BOOL bVisible = TRUE;	// initially visible
	bVisible = !bVisible;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, bVisible);
	UISetCheck(ID_VIEW_TOOLBAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
	::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
	UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
	UpdateLayout();
	return 0;
}

LRESULT CMainFrame::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CAboutDlg dlg;
	dlg.DoModal();
	return 0;
}

LRESULT CMainFrame::OnWindowCascade(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDICascade();
	return 0;
}

LRESULT CMainFrame::OnWindowTile(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDITile();
	return 0;
}

LRESULT CMainFrame::OnWindowArrangeIcons(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	MDIIconArrange();
	return 0;
}

LRESULT CMainFrame::OnFilePrint(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//ShellExecute(GetActiveView()->m_hWnd, _T("PRINT"), NULL, NULL, NULL, 0);
	// hwnd is the HWND of the rich edit control.
	// hdc is the HDC of the printer. This value can be obtained for the 
	// default printer as follows:
	PRINTDLG pd = { sizeof(pd) };
	pd.Flags = PD_RETURNDC | PD_RETURNDEFAULT;
	if (PrintDlg(&pd))
	{
		HDC hdc = pd.hDC;

		PrintRTF(GetActiveView()->m_hWnd, hdc);
	}
	return 0;
}

LRESULT CMainFrame::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//CShellFileSaveDialog dlg;
	CFileDialog dlg(FALSE, _T("txt"), _T("dump.txt"), OFN_CREATEPROMPT | OFN_NOCHANGEDIR | OFN_NONETWORKBUTTON | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
		_T("text files\0*.txt\0rich text\0*.rtf\0\0\0"));
	if (IDOK == dlg.DoModal()) {
		CWinFile wf;
		CString file;
		TCHAR buf[MAX_PATH] = { 0 };
		CString ext;
		DWORD written = 0;
		CView* pView = GetActiveView();
		CString text;

		if (!pView)
			return 0;
		file = dlg.m_szFileName;
		ext = file.Right(3);
		if (wf.Create(dlg.m_szFileName, CREATE_ALWAYS)) {
			if (!ext.CompareNoCase(_T("txt"))) {
				if (pView->GetWindowText(text)) {
					if (wf.Write(text.GetBuffer(), pView->GetWindowTextLength()  * sizeof(TCHAR), &written)) {
						MessageBox(_T("ok"));
					}
				}
			}
			else if (!ext.CompareNoCase(_T("rtf"))) {
				written = pView->StreamOutFile(wf);
			}
		}
	}
	return 0;
}

LRESULT CMainFrame::OnLVDoubleClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
	NMITEMACTIVATE* pItem = (NMITEMACTIVATE*)(LPNMHDR)(pnmh);
	int iItem = pItem->iItem;
	TCHAR buf[81] = { 0 };
	LVITEM item = { 0 };
	item.iItem = iItem;
	item.pszText = buf;
	item.cchTextMax = 80;
	item.mask = LVIF_TEXT;
	CString cmd = _T("JOIN ");
	m_lv.SendMessage(LVM_GETITEMTEXT, (WPARAM)iItem, (LPARAM)&item);
	cmd += CT2AEX<>(item.pszText, CP_UTF8);
	cmd += _T("\r\n");
	m_sock.Send(CT2A(cmd), cmd.GetLength());
	//m_tv.DeleteAllItems();
	m_NameOrChannel = item.pszText;
	CreateChannel(item.pszText);
	m_NameOrChannel = item.pszText;
	bHandled = TRUE;
	return 0;
}

LRESULT CMainFrame::OnTVDoubleClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
	CNo5TreeItem item = m_tv.GetSelectedItem();
	CString name;
	CString _name;
	CNo5TreeItem parent;

	if (parent = item.GetParent()) {
		parent = parent.GetParent();
		if (parent) {
			item.GetText(name);
			m_bInChannel = false;
			m_NameOrChannel = name;
			_name = name;
			CView* pView = CreatePrivateChannel(_name);
		}
	}
	return 0;
}

void CMainFrame::CreateView(LPCTSTR name, ViewType type)
{
	CChildFrame* pChild = new CChildFrame(name, type, *this);
	pChild->CreateEx(m_hWndMDIClient);
	//pChild->SetIcon(LoadIcon(_Module.GetModuleInstance(),MAKEINTRESOURCE(IDR_MAINFRAME)), FALSE);

	//pChild->SetTabText(name);
	pChild->SetTitle(name, true);
	pChild->m_view.SetAutoURLDetect();
	pChild->m_view.SetEventMask(ENM_LINK);
}

CView* CMainFrame::GetActiveView()
{
	ViewData* p = GetViewData();
	CView* res = NULL;

	if (p) {
		res = p->pView;
		ATLASSERT(res);
		ATLASSERT(res->IsWindow());
	}
	return res;
}

void CMainFrame::CreateTreeView()
{

		m_tv.Create(m_tab, 0, 0, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT,
			WS_EX_CLIENTEDGE, IDC_TREEVIEW);
		m_tab.AddTab(m_tv, _T("Users"));
		ATLASSERT(m_tv.IsWindow());
		/*NO5TL::CNo5TreeItem root = m_tv.InsertItem(_T("root"), NULL, TVI_ROOT);
		m_tv.InsertItem(_T("child"), root, TVI_SORT);
		m_tv.InsertItem(_T("child 2"), root, TVI_SORT);
		NO5TL::CNo5TreeItem item;
		item = m_tv.FindItem(_T("child"), TRUE, TRUE);
		if (item) {
			m_tv.InsertItem(_T("grandchild"), item, TVI_SORT);
		}*/

}

void CMainFrame::CreateListView()
{
	m_lv.Create(m_tab, 0, 0, LVS_REPORT | LVS_SINGLESEL | LVS_SORTASCENDING | WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, WS_EX_CLIENTEDGE,
		IDC_LISTVIEW);
	LVCOLUMN col = { 0 };
	CClientDC dc(m_hWnd);
	CSize sz;
	LVITEM item = { 0 };
	//m_lv.SetFont(m_font);
	dc.GetTextExtent(_T("  CHANNEL    "), strlen("  CHANNEL  "), &sz);
	m_lv.InsertColumn(0, _T("channel"), LVCFMT_LEFT, sz.cx);
	dc.GetTextExtent(_T(" users "), strlen("users     "), &sz);
	m_lv.InsertColumn(1, _T("users"), LVCFMT_LEFT, sz.cx);
	dc.GetTextExtent(_T("  topic                                        "), strlen("topic                                                                    "),
		&sz);
	m_lv.InsertColumn(2, _T("topic"), LVCFMT_LEFT, sz.cx);
	m_tab.AddTab(m_lv, _T("channels"));

}

BOOL CMainFrame::PrintRTF(HWND hwnd, HDC hdc)
{
	DOCINFO di = { sizeof(di) };

	if (!StartDoc(hdc, &di))
	{
		return FALSE;
	}

	int cxPhysOffset = GetDeviceCaps(hdc, PHYSICALOFFSETX);
	int cyPhysOffset = GetDeviceCaps(hdc, PHYSICALOFFSETY);

	int cxPhys = GetDeviceCaps(hdc, PHYSICALWIDTH);
	int cyPhys = GetDeviceCaps(hdc, PHYSICALHEIGHT);

	// Create "print preview". 
	SendMessage(hwnd, EM_SETTARGETDEVICE, (WPARAM)hdc, cxPhys / 2);

	FORMATRANGE fr;

	fr.hdc = hdc;
	fr.hdcTarget = hdc;

	// Set page rect to physical page size in twips.
	fr.rcPage.top = 0;
	fr.rcPage.left = 0;
	fr.rcPage.right = MulDiv(cxPhys, 1440, GetDeviceCaps(hdc, LOGPIXELSX));
	fr.rcPage.bottom = MulDiv(cyPhys, 1440, GetDeviceCaps(hdc, LOGPIXELSY));

	// Set the rendering rectangle to the pintable area of the page.
	fr.rc.left = cxPhysOffset;
	fr.rc.right = cxPhysOffset + cxPhys;
	fr.rc.top = cyPhysOffset;
	fr.rc.bottom = cyPhysOffset + cyPhys;

	SendMessage(hwnd, EM_SETSEL, 0, (LPARAM)-1);          // Select the entire contents.
	SendMessage(hwnd, EM_EXGETSEL, 0, (LPARAM)&fr.chrg);  // Get the selection into a CHARRANGE.

	BOOL fSuccess = TRUE;

	// Use GDI to print successive pages.
	while (fr.chrg.cpMin < fr.chrg.cpMax&& fSuccess)
	{
		fSuccess = StartPage(hdc) > 0;

		if (!fSuccess) break;

		int cpMin = SendMessage(hwnd, EM_FORMATRANGE, TRUE, (LPARAM)&fr);

		if (cpMin <= fr.chrg.cpMin)
		{
			fSuccess = FALSE;
			break;
		}

		fr.chrg.cpMin = cpMin;
		fSuccess = EndPage(hdc) > 0;
	}

	SendMessage(hwnd, EM_FORMATRANGE, FALSE, 0);

	if (fSuccess)
	{
		EndDoc(hdc);
	}

	else

	{
		AbortDoc(hdc);
	}

	return fSuccess;

}

LRESULT CMainFrame::OnMsgFromBottom(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (m_sock.IsConnected()) {
		CString msg = (LPCTSTR)wParam;
		if (msg.GetLength()) {
			CStringToken st;
			CSimpleArray<CString> arr;
			int count;

			st.Init(msg,_T(" "));
			if (msg[0] == '/') {
				if (msg[1] != '/') {
					msg = msg.Right(msg.GetLength() - 1);
				}
				count = st.GetAll2(arr);
				//msg.Append(_T("\r\n"));
				m_sock.Send(CT2CA(msg), msg.GetLength());
				if (count >= 2 && (!arr[0].CompareNoCase(_T("/PRIVMSG"))) && (!arr[1].IsEmpty())) {
					if (arr[1].GetAt(0) != '#') {
						CString s;
						CView* pView = CreatePrivateChannel(arr[1]);

						pView->SetSelEnd();
						pView->SetTextColor(0xaa0000);
						pView->AppendText(m_nick);
						pView->AppendText(_T(": "));
						pView->SetTextColor(0);
						pView->AppendText(msg);
						//msg = _T("PRIVMSG ");
						//msg += arr[1];
					}
					else {
						CView* pView = CreateChannel(arr[1]);
						pView->SetSelEnd();
						pView->SetTextColor(0xaa0000);
						pView->AppendText(m_nick);
						pView->AppendText(_T(": "));
						pView->SetTextColor(0);
						pView->AppendText(msg);
					}
				}
			}
			else if (!m_bInChannel) {
				CString s(_T("PRIVMSG "));
				ViewData* p = GetViewData();
				CView* pView = CreatePrivateChannel(p->name);
				s += p->name;
				s += _T(" :");
				s += msg;

				m_sock.Send(CT2CA(s), s.GetLength());

				p->pView->SetSelEnd();
				p->pView->SetTextColor(0xaa0000);
				p->pView->AppendText(m_nick);
				p->pView->AppendText(_T(": "));
				p->pView->SetTextColor(0);
				p->pView->AppendText(msg);
			}
			//#ifdef _DEBUG
			//				m_top.AppendText(msg);
			//#endif
			else if (m_bInChannel) {
				CString s;
				CView* pView = CreateChannel(m_NameOrChannel);

				s = _T("PRIVMSG ");
				s += m_NameOrChannel;
				s += _T(" :");
				s += msg;
				//s += _T("\r\n");
				m_sock.Send(CT2CA(s), s.GetLength());
				pView->SetSelEnd();
				pView->SetTextColor(0xaa0000);
				pView->AppendText(m_nick);
				pView->AppendText(_T(": "));
				pView->SetTextColor(0);
				pView->AppendText(msg);
				//pView->AppendText(_T("\r\n"));
			}

		}
	}
	return 0;
}

LRESULT CMainFrame::OnIncFont(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CView* p = GetActiveView();
	if (p) {
		LONG s = p->GetTextHeight(FALSE);
		s += 5;
		p->SetTextHeight(s, SCF_ALL);
	}
	return 0;
}

LRESULT CMainFrame::OnDecFont(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CView* p = GetActiveView();
	if (p) {
		LONG s = p->GetTextHeight(FALSE);
		s -= 5;
		p->SetTextHeight(s, SCF_ALL);
	}
	return 0;
}

ViewData* CMainFrame::GetViewData() {
	ViewData* p = NULL;
	CWindow wnd = MDIGetActive();
	if (wnd.IsWindow()) {
		p = (ViewData*)wnd.SendMessage(WM_CHILDGETDATA);
		ATLASSERT(p && (!p->name.IsEmpty()) && p->pView);
	}
	return p;
}

struct EnumData
{
	CString name;
	ViewData* pData;
	CWindow hmdiclient;
	EnumData(const CString& _name, CWindow& mdiclient) :name(_name), hmdiclient(mdiclient)
	{
		pData = NULL;
	}
};
struct EnumData2
{
	CString name;
	CWindow hmdiclient;
};

ViewData* CMainFrame::GetViewDataByName(const CString& name)
{
	EnumData data(name, CWindow(m_hWndMDIClient));
	EnumChildWindows(m_hWndMDIClient, EnumChildGetDataByName, (LPARAM)&data);
	return data.pData;
}

CView* CMainFrame::GetViewByName(const CString& name)
{
	ViewData* p = GetViewDataByName(name);
	if (p)
		return p->pView;
	return NULL;
}

CView* CMainFrame::CreateViewIfNoExist(LPCTSTR name, ViewType type)
{
	CView* p = GetViewByName(name);

	if (!p) {
		CreateView(name, type);
		p = GetViewByName(name);
	}
	ATLASSERT(p);
	return p;
}

CView* CMainFrame::CreatePrivateChannel(const CString& name)
{
	CView* p = CreateViewIfNoExist(name, VIEW_PVT);
	ATLASSERT(p);
	// TODO: do a whois on the person
	ActivateViewByName(name);
	return p;
}
CView* CMainFrame::CreateChannel(const CString& channel)
{
	ATLASSERT(!channel.IsEmpty());
	CView* p = CreateViewIfNoExist(channel, VIEW_CHANNEL);
	ATLASSERT(p);
	// TODO: query the mode of the channel
	ActivateViewByName(channel);
	return p;
}
bool CMainFrame::ActivateViewByName(const CString& name)
{
	EnumData2 data;
	data.name = name;
	data.hmdiclient = m_hWndMDIClient;
	EnumChildWindows(m_hWndMDIClient, EnumChildActivate, (LPARAM)&data);
	return false;
}

static BOOL CALLBACK EnumChildGetDataByName(HWND hWnd, LPARAM lParam)
{
	CWindow wnd = hWnd;
	EnumData* data = (EnumData*)lParam;

	if (wnd.GetParent() == data->hmdiclient) {
		CString name;
		wnd.GetWindowText(name);
		if (!name.CompareNoCase(data->name)) {
			ViewData* pData = (ViewData*)wnd.SendMessage(WM_CHILDGETDATA);
			data->pData = pData;
			return FALSE;
		}
	}
	return TRUE;
}

static BOOL CALLBACK EnumChildActivate(HWND hWnd, LPARAM lParam)
{
	CWindow wnd = hWnd;
	EnumData2* data = (EnumData2*)lParam;
	CString name;

	wnd.GetWindowText(name);
	if (!name.CompareNoCase(data->name)) {
		SendMessage(data->hmdiclient, WM_MDIACTIVATE, (WPARAM)(HWND)wnd, 0);
		return FALSE;
	}
	return TRUE;
}

BOOL CMainFrame::LoadUserSettings()
{
	CPrivateIniFile ini;
	BOOL res = FALSE;
	CString path;
	CSimpleArray<CString> servers;

	path = m_path;
	path += _T("no5irc.ini");

	if (ini.Create(path)) {
		m_nick = ini.GetString(_T("settings"), _T("nick"), NULL, MAX_PATH);
		m_user = ini.GetString(_T("settings"), _T("user"), NULL, MAX_PATH);
		m_name = ini.GetString(_T("settings"), _T("real"), NULL, MAX_PATH);
		m_pass = ini.GetString(_T("settings"), _T("pass"), NULL, MAX_PATH);
		m_server = ini.GetString(_T("settings"), _T("server"), _T("irc.freenode.net"), MAX_PATH);
		//ini.GetStringList(_T("settings"), _T("servers"), m_servers);
		res = TRUE;
	}
	return res;

}

BOOL CMainFrame::SaveUserSettings()
{
	CPrivateIniFile ini;
	BOOL res = FALSE;
	CString path;

	path = m_path;
	path += _T("no5irc.ini");

	if (ini.Create(path)) {
		if (ini.WriteString(_T("settings"), _T("nick"), m_nick)) {
			if (ini.WriteString(_T("settings"), _T("user"), m_user)) {
				if (ini.WriteString(_T("settings"), _T("real"), m_name)) {
					if (ini.WriteString(_T("settings"), _T("pass"), m_pass)) {
						if (ini.WriteString(_T("settings"), _T("server"), m_server)) {
							//res = ini.WriteStringList(_T("settings"), _T("servers"), m_servers);
							res = TRUE;
						}
					}
				}
			}
		}
	}
	return res;
}

void CMainFrame::OnLogin()
{
	CLoginDlg dlg;
	dlg.m_servers = m_servers;
	dlg.m_nick = m_nick;
	dlg.m_real = m_name;
	dlg.m_user = m_user;
	dlg.m_pass = m_pass;
	dlg.m_server = m_server;
	int nRes = dlg.DoModal();
	if (nRes == IDOK) {
		USES_CONVERSION;
		m_lv.DeleteAllItems();
		m_tv.DeleteAllItems();
		m_servers = dlg.m_servers;
		m_server = dlg.m_server;
		m_nick = dlg.m_nick;
		m_name = dlg.m_real;
		m_user = dlg.m_user;
		m_pass = dlg.m_pass;
		BOOL res = m_sock.CreateSocket();
		if (res) {
			res = m_sock.Connect(CT2CA(m_server), 6667);
			if (res) {
				res = m_sock.AsyncSelect();
			}
			if (!res) {
				OnSockError(::WSAGetLastError());
			}
		}
		if (res) {
			m_tv.InsertItem(m_server, NULL, TVI_ROOT);
		}
	}

}


struct LVSORT
{
	CListViewCtrl m_lv;
	int nCol;
	bool sort; // true = ascending
};

LRESULT CMainFrame::OnColumnClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
	LPNMLISTVIEW pnmv = (LPNMLISTVIEW)pnmh;
	int nCol = pnmv->iSubItem;
	LVSORT lvs;
	lvs.m_lv = m_lv;
	lvs.nCol = nCol;
	if (nCol == 0) {
		static bool sort = true;
		sort = !sort;
		lvs.sort = sort;
		m_lv.SortItemsEx(CompareLVItems, (LPARAM)&lvs);
	}
	else if (nCol == 1) {
		static bool sort = true;
		sort = !sort;
		lvs.sort = sort;
		m_lv.SortItemsEx(CompareLVItems, (LPARAM)&lvs);
	}
	return 0;
}

static int CALLBACK CompareLVItems(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	LVSORT* pSort = (LVSORT*)lParamSort;
	CString str1, str2;
	pSort->m_lv.GetItemText(lParam1, pSort->nCol, str1);
	pSort->m_lv.GetItemText(lParam2, pSort->nCol, str2);
	int res = pSort->nCol == 0 ? str1.CompareNoCase(str2) : _wtoi(str1) - _wtoi(str2);
	return pSort->sort ? res : -res;
}


// :nick!~name@ip||server||ipv6
// ou
// :nick!server
bool CMainFrame::ParseUserName(const CString& user, CString& nick, CString& name, CString& ip)
{
	int nBang = user.Find('!', 0);
	int nTio = user.Find('~', nBang);
	int nAt = -1;
	if (nTio >= 0)
		nAt = user.Find('@', nTio);

	if (user[0] == ':')
		nick = user.Right(user.GetLength() - 1);
	nick = user.Left(nBang);
	nick = nick.Right(nick.GetLength() - 1);
	if(nick[0] == ':')
		nick = nick.Right(nick.GetLength() - 1);
	if (nTio >= 0 && nAt >= 0) {
		name = user.Mid(nTio + 1, nAt - nTio - 1);
		ip = user.Right(user.GetLength() - nAt + 1);
	}
	else
		ip = user.Right(user.GetLength() - nBang + 1);
	return nBang >= 0 && !nick.IsEmpty();
}

void CMainFrame::CreateMarquee(void)
{
	CRect r;
	{
		CClientDC dc = m_hWnd;
		HFONT hFont;
		TEXTMETRIC tm;
		CFont font;

		//font = (HFONT)GetStockObject(ANSI_FIXED_FONT);
		font.CreatePointFont(100, _T("courier"));
		hFont = dc.SelectFont(font);

		dc.GetTextMetrics(&tm);
		dc.SelectFont(hFont);
		GetClientRect(&r);
		r.top = 0;
		r.bottom = 3 * tm.tmHeight / 2;
	}

	m_marquee.Create(m_hWnd, r, NULL, 0, 0, IDC_MARQUEE1);
	m_marquee.SetInc(m_marquee.GetCharWidth() / 8);
	m_marquee.SetSpace(2 * m_marquee.GetCharWidth());
	m_marquee.SetLoop(false);
	m_marquee.SetTextColor(0xffffff);
	m_marquee.SetBackColor(0x008000);
	m_marquee.SetElapse(4);
	m_marquee.Start();
}

void CMainFrame::OnSockError(int error)
{
	CView* pView = GetActiveView();
	//USES_CONVERSION;

	if (pView && pView->IsWindow()) {
		pView->AppendText(CA2CTEX<>(CSocket::GetErrorDesc(error)));
		pView->AppendText(_T("\n"));
	}
}
void CMainFrame::OnSockRead(int error)
{
	static CDataBuffer<char> buffer(4096);
	static char buf[4096] = { 0 };
	CSimpleArray<CString> lines;
	CView* pView = GetActiveView();
	DWORD res = 0;
	WSABUF wsbuf = { 0 };
	DWORD dwFlags = 0;
	wsbuf.buf = buf;
	wsbuf.len = sizeof(buf) - 1;
#ifdef _DEBUG
	/*static CWinFile wf;

	if (!wf.IsHandleValid()) {
		wf.Create(_T("debug.txt"), CREATE_ALWAYS,GENERIC_WRITE);
	}*/
#endif

	if (error)
		return;
	memset(buf, 0, sizeof(buf));
	if (m_sock.CanRead()) {
		//res = m_sock.Recv(buf, sizeof(buf) - 1);
		if (::WSARecv(m_sock.GetHandle(), &wsbuf, 1, &res, &dwFlags, NULL, NULL)) {
			OnSockError(WSAGetLastError());
			return;
		}
	}
	else {
		Sleep(100);
		return;
	}
	/*if (res == SOCKET_ERROR) {
		OnSockError(res);
		return;
	}*/
	if (buffer.GetDataLen() == 0) {
		buffer.Reset();
	}
	//buffer.Add<LPCSTR>(buf);
	//buffer.Add<LPCTSTR>(CA2CTEX<4096>(buf));
	// buffer.Add((LPBYTE)(LPCTSTR)CA2CTEX<4096>(buf, CP_UTF8), res * sizeof(TCHAR));
	buffer.Add((LPBYTE)buf, res);
	//buffer.AddNull();
	ATLTRACE(_T("read %d\n"), res);
	if (res == (sizeof(buf) - 1)) {
		ATLTRACE(_T("returning %d\n"), buffer.GetDataLen());
		return;
	}
#ifdef _DEBUG
	//if(wf.IsHandleValid())
	//	wf.Write(buffer);
#endif
	//int voids = buffer.ReplaceChars(0, '.');
	//buffer.AddNull();
	//ATLTRACE("\n nulls = %d\n", voids);
	buffer.Init(("\r\n"), false);
	const int count = buffer.GetAll2(lines);
	buffer.Reset();

	for (int i = 0; i < count; i++) {
		CStringToken st2;
		CSimpleArray<CString> params;
		CString channel, code, topic, cmd, users, server, nick, str;

		lines[i] = lines[i].TrimLeft();
		st2.Init(lines[i],_T(" "));
		str = lines[i];

		const int count2 = st2.GetAll3(params);
		if (count2 >= 2) {
			server = params[0];
			code = params[1];
			if (code.Compare(_T("322")) == 0) {
				if (count2 >= 4) {
					nick = params[2];
					channel = params[3];
					if (count2 >= 5) {
						users = params[4];

						for (int i = 5; i < params.GetSize(); i++) {
							topic += params[i];
							topic += ' ';
						}
					}
					int insert = m_lv.InsertItem(LVIF_TEXT, 0, channel, 0, 0, 0, 0);
					m_lv.SetItemText(insert, 1,users);
					m_lv.SetItemText(insert, 2,topic);
				}
			}
			else if (code.Compare(_T("321")) == 0) {
				pView->AppendText(_T("\nbeggining of list \n"));
			}
			else if (code.Compare(_T("323")) == 0) {
				pView->AppendText(_T("\nend of list \n"));
			}
			else if (!code.Compare(_T("375"))) {
				pView->SetSelEnd();
				pView->SetTextColor(0x00cc00, SCF_SELECTION);
				pView->AppendText(_T("message of the day:\n"));
			}
			else if (!code.Compare(_T("376"))) {
				pView->AppendText(_T("end of message of the day:\n"));
				pView->SetTextColor(0, SCF_SELECTION);
			}
			else if (!code.Compare(_T("372"))) { // MOTD line
				CString msg;

				pView->SetSelEnd(); 
				pView->SetTextColor(0x00aa00, SCF_SELECTION);
				for (int i = 3; i < params.GetSize(); i++) {
					msg += params[i];
					msg += ' ';
				}
				pView->AppendText(msg);
				pView->AppendText(_T("\n"));
			}
			else if (!code.Compare(_T("353"))) { // names in channel
				if (count2 >= 4 && params[4].GetAt(0) == '#')
					m_NameOrChannel = params[4];
				CView* p = CreateChannel(m_NameOrChannel);
				ActivateViewByName(m_NameOrChannel);
				CNo5TreeItem parent = m_tv.FindItem(m_NameOrChannel, FALSE, TRUE);
				//ATLASSERT(parent);
				if (parent) {
					for (int i = 5; i < count2; i++) {
						if (params[i].GetAt(0) == ':') {
							params[i] = params[i].Right(params[i].GetLength() - 1);
						}
						m_tv.InsertItem(params[i], parent, TVI_SORT);
					}
				}
			}
			else if (!code.Compare(_T("332"))) { // topic
				CNo5TreeItem parent = m_tv.FindItem(m_server, TRUE, FALSE);
				if (m_NameOrChannel.IsEmpty() || params[3].GetAt(0) == '#')
					m_NameOrChannel = params[3];
				CView* p = GetViewByName(m_NameOrChannel);

				if (!m_tv.FindItem(m_NameOrChannel, FALSE, TRUE))
					m_tv.InsertItem(m_NameOrChannel, parent, TVI_SORT);
				if (!p) {
					//p = CreateViewIfNoExist(m_NameOrChannel, VIEW_CHANNEL);
				}
				else {
					p->AppendText(lines[i]);
					p->AppendText(_T("\n"));
				}
			}
			else if (!code.CompareNoCase(_T("PRIVMSG"))) {
				CString msg;
				CString nick, name, ip;
				CView* p = NULL;

				channel = _T("");
				if (count2 >= 3) {
					channel = params[2];
					if (channel[0] == '#') {
						m_bInChannel = true;
						m_NameOrChannel = channel;
					}
					else {
						m_bInChannel = false;
						m_NameOrChannel.Empty();
					}
				}

				bool b = ParseUserName(params[0], nick, name, ip);
				//ATLASSERT(b);
				if (m_bInChannel) {
					p = CreateChannel(channel);
					m_NameOrChannel = channel;
				}
				else {
					p = CreatePrivateChannel(nick);
				}
				p->SetTextColor(0x0000aa);
				p->AppendText(nick);
				p->AppendText(_T(": "));
				p->SetTextColor(0);
				for (int i = 3; i < params.GetSize(); i++) {
					msg += params[i];
					msg += ' ';
				}
				msg += _T("\n");
				p->AppendText(msg);
			}
			else if (!code.CompareNoCase(_T("QUIT"))) {
				str = lines[i];
				m_marquee.AddItem(str);
				CString nick, user, ip;
				bool b = ParseUserName(str, nick, user, ip);
				//ATLASSERT(b);
				str = nick + _T(" quits ");
				if (count2 >= 3) {
					for (int i = 2; i < count2; i++) {
						str += params[i];
						str += ' ';
					}
					str += _T("\n");
				}
				pView->SetTextColor(0xa0a0a0);
				pView->AppendText(str);
				// m_marquee.AddItem(str);
				//m_lb.DeleteString(m_lb.FindStringExact(0, nick));
				m_tv.DeleteItem(nick, TRUE, TRUE);

			}
			else if (!code.CompareNoCase(_T("JOIN"))) {
				str = params[0];
				//m_NameOrChannel = params[2];
				CString nick, user, ip;

				if (m_NameOrChannel.IsEmpty()) {
					channel = params[2];
				}
				else
					channel = m_NameOrChannel;
				CView* p = GetViewByName(channel);
				bool b = ParseUserName(str, nick, user, ip);
				ATLASSERT(nick[0] != ':');
				str = nick + _T(" enters the channel ");
				str += channel;
				str += '\n';
				if (p) {
					p->SetTextColor(0x10aa00);
					p->AppendText(str);
				}
				m_marquee.AddItem(str);
				//m_marquee.AddItem(lines[i]);
				if (nick != m_nick) {
					CNo5TreeItem parent = m_tv.FindItem(channel, TRUE, TRUE);
					m_tv.InsertItem(nick, parent, TVI_SORT);
				}
				else
					m_NameOrChannel = params[2];
			}
			else if (!code.CompareNoCase(_T("PART"))) {
				CView* p = GetViewByName(m_NameOrChannel);

				if (!p || !p->IsWindow())
					continue;
				str = params[0];
				CString nick, user, ip;
				bool b = ParseUserName(str, nick, user, ip);
				//ATLASSERT(b);

				str = nick + _T(" leaves the channel ");
				if (params[1]) {
					for (int i = 1; i < count2; i++) {
						str += params[i];
						str += ' ';
					}
					str += _T("\n");
				}
				p->SetTextColor(0x10aa00);
				p->AppendText(str);
				m_marquee.AddItem(str);
				m_tv.DeleteItem(nick, TRUE, TRUE);

			}
			else if (!code.CompareNoCase(_T("NOTICE"))) {
				pView->AppendText(lines[i]);
				pView->AppendText(_T("\n"));
			}
			else if (_wtoi(code)) {
				if (pView->IsWindow()) {
					pView->AppendText(lines[i]);
					pView->AppendText(_T("\n"));
				}
			}
		}

		if (count2 >= 1) {

			cmd = params[0];

			if (cmd.CompareNoCase(_T("PING")) == 0) {
				CString pong;

				pong += _T("PONG ");
				pong += params[1];
				pong += _T("\n");

				m_sock.Send(CT2A(pong), pong.GetLength());
				pView->AppendText(_T("ping received - pong returned\r\n"));
			}
			else {
				//pView->AppendText(cmd += _T("\r\n"));
			}
		}
	}
	if (buffer.GetDataLen() == 0) {
		buffer.Reset();
	}
}
void CMainFrame::OnSockWrite(int error) {}
void CMainFrame::OnSockConnect(int error)
{
	CString cmd;
	CView* pView = GetActiveView();
	static bool reconnecitng = false;

	ATLASSERT(pView);
	if (error == SOCKET_ERROR && !reconnecitng) {
		BOOL res;
		::Sleep(500);
		CSimpleArray<CIPAddress> ips = m_sock.m_addr.GetIPs();
		CSocketAddress addr;
		//res = m_sock.Connect(CT2A(m_server), 6667);

		reconnecitng = true;
		for (int i = 0; i < ips.GetSize(); i++) {
			addr.Set(ips[i], CPort(6667));
			res = m_sock.Connect((SOCKADDR_IN*)&addr);
			if (res) {
				res = m_sock.AsyncSelect();
				break;
			}
			if (!res) {
				OnSockError(::WSAGetLastError());
			}
		}
		reconnecitng = false;
		if (!res)
			return;
	}
	pView->AppendText(_T("connected!\n"));

	//if (m_sock.CanWrite()) {
	if (!m_pass.IsEmpty()) {
		cmd = "PASS ";
		cmd += m_pass;
		cmd += "\r\n";
		m_sock.SendString(CT2CA(cmd));
	}

	cmd = "NICK ";
	cmd += m_nick;
	cmd += "\r\n";
	m_sock.SendString(CT2CA(cmd));


	cmd = "USER ";
	cmd += m_user;
	cmd += " 0 * :";
	cmd += m_name; cmd += "\r\n";
	m_sock.SendString(CT2CA(cmd));

	//m_sock.SendString("USER phpnewbie 0 * :fernando\r\n");
	m_sock.SendString("LIST\r\n");
	cmd = "MODE ";
	cmd += m_nick;
	cmd += " -i\r\n";
	m_sock.SendString(CT2CA(cmd));
	//}
}
void CMainFrame::OnSockClose(int error)
{
	CView* pView = GetActiveView();
	if (pView && pView->IsWindow())
		pView->AppendText(_T("connection closed!\n"));
}
void CMainFrame::OnSockAccept(int error) {}
void CMainFrame::OnSockOutOfBand(int error) {}
void CMainFrame::OnSockConnecting(void)
{
	CView* pView = GetActiveView();
	pView->AppendText(_T("connecting..."));
}
void CMainFrame::OnSockConnectTimeout(void) {}
void CMainFrame::OnSockResolvingAddress(void)
{
	CView* pView = GetActiveView();
	if (pView && pView->IsWindow())
		pView->AppendText(_T("resolving address...\n"));
}
void CMainFrame::OnSockAddressResolved(int error)
{
	if (error)
		return;
	CView* pView = GetActiveView();
	if (pView && pView->IsWindow())
		pView->AppendText(_T("address resolved\n"));
}

