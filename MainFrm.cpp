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

LRESULT CMyTabbedChildWindow::OnTVRightClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	return m_frame.OnTVRightClick(idCtrl, pnmh, bHandled);
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
	CreateImageList();
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
	pLoop->AddIdleHandler(&m_ChannelsView);
	pLoop->AddMessageFilter(&m_ChannelsView);
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
	pLoop->RemoveMessageFilter(&m_ChannelsView);
	pLoop->RemoveIdleHandler(&m_ChannelsView);

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

LRESULT CMainFrame::OnChannelsFileOpen(UINT, WPARAM, LPARAM, BOOL&)
{
	CString file = m_server + _T(".txt");
	file = m_path + file;
	CFileDialog dlg(TRUE, _T("txt"), file,  OFN_NOCHANGEDIR | OFN_NONETWORKBUTTON | OFN_EXPLORER,
		_T("text files\0*.txt\0\0\0\0"));
	if (IDOK == dlg.DoModal()) {
		CWinFile wf;
		TCHAR buf[MAX_PATH] = { 0 };
		CString ext;
		DWORD written = 0;
		CString text;
		CDataBuffer<TCHAR> db;

		file = dlg.m_szFileName;
		ext = file.Right(3);
		if (wf.Create(dlg.m_szFileName, OPEN_ALWAYS)) {
			if (!ext.CompareNoCase(_T("txt"))) {
				CString channel;
				CString users;
				CString topic;
				CString line;
				CSimpleArray<CString> lines;
				CDataBuffer<TCHAR> db;
				BOOL res = wf.ReadLines(lines);
				//BOOL res = wf.ReadAll(db);
				ATLASSERT(res);
				if (res) {
					for (int i = 0; i < lines.GetSize(); i++) {
						int index = lines[i].Find(' ', 0);
						if (index > 0) {
							StringCopyN(channel, lines[i], 0, index);
							int index2 = lines[i].Find(':', index);
							if (index2 > index) {
								StringCopyN(users, lines[i], index, index2 - index);
								StringCopyN(topic, lines[i], index2, lines[i].GetLength() - index2 + 1);

								int insert = m_lv.InsertItem(LVIF_TEXT, 0, channel, 0, 0, 0, 0);
								m_lv.SetItemText(insert, 1, users);
								m_lv.SetItemText(insert, 2, topic);
							}
						}
					}
				}
			}
		}
	}
	return 0;
}

LRESULT CMainFrame::OnChannelsFileSave(UINT, WPARAM, LPARAM, BOOL&)
{
	CString file = m_server + _T(".txt");
	file = m_path + file;
	CFileDialog dlg(FALSE, _T("txt"),file, OFN_CREATEPROMPT | OFN_NOCHANGEDIR | OFN_NONETWORKBUTTON | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
		_T("text files\0*.txt\0\0\0\0"));
	if (IDOK == dlg.DoModal()) {
		CWinFile wf;
		TCHAR buf[MAX_PATH] = { 0 };
		CString ext;
		DWORD written = 0;
		CString text;
		CDataBuffer<TCHAR> db;

		file = dlg.m_szFileName;
		ext = file.Right(3);
		if (wf.Create(dlg.m_szFileName, CREATE_ALWAYS)) {
			if (!ext.CompareNoCase(_T("txt"))) {
				const int count = m_lv.GetItemCount();
				CString channel;
				CString users;
				CString topic;
				CString line;
				BOOL res;
				for (int i = 0; i < count; i++) {

					m_lv.GetItemText(i, 0, channel);
					m_lv.GetItemText(i, 1, users);
					m_lv.GetItemText(i, 2, topic);

					line = channel + ' ' + users + topic + _T("\r\n");
					//db.Add<LPCWSTR>(line);
					res = wf.WriteString(line, &written);
					ATLASSERT(res);
				}
				//res = wf.Write(db);
				// ATLASSERT(res);
				//ATLASSERT(written == db.GetDataLen());
			}
		}
	}
	return 0;
}

LRESULT CMainFrame::OnChannelsFileUpdate(UINT, WPARAM, LPARAM, BOOL&)
{
	if (m_sock.IsConnected()) {
		CStringA cmd = "LIST\r\n";
		m_sock.SendString(cmd);
	}
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
	m_lv.SendMessage(LVM_GETITEMTEXT, (WPARAM)iItem, (LPARAM)&item);
	JoinChannel(item.pszText);
	//m_tv.DeleteAllItems();
	m_NameOrChannel = item.pszText;
	CreateChannel(item.pszText);
	bHandled = TRUE;
	return 0;
}

LRESULT CMainFrame::OnTVDoubleClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
	CNo5TreeItem item = m_tv.GetSelectedItem();
	CString name;
	CString _name;
	CNo5TreeItem parent;

	if (m_sock.IsConnected()) {
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
	}
	return 0;
}

LRESULT CMainFrame::OnTVRightClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
	CNo5TreeItem item = m_tv.GetSelectedItem();
	CString name;
	CNo5TreeItem parent;

	if (parent = item.GetParent()) {
		parent = parent.GetParent();
		if (parent) {
			item.GetText(name);
			CMenuHandle menu;
			CPoint pt;

			GetCursorPos(&pt);
			menu.LoadMenu(IDR_MENU1);
			menu.Attach(menu.GetSubMenu(0));
			UINT id = menu.TrackPopupMenu(TPM_CENTERALIGN | TPM_VCENTERALIGN | TPM_RETURNCMD | TPM_LEFTBUTTON | TPM_HORNEGANIMATION | \
				TPM_VERNEGANIMATION, pt.x, pt.y, m_hWnd);

			switch (id) {
				case ID_USER_SENDVERSIONREQUEST:
					RequestVersion(name);
					break;
				case ID_USER_SENDPINGREQUEST:
					RequestPing(name);
					break;
				case ID_USER_SENDUSERINFOREQUEST:
					RequestUserinfo(name);
					break;
				case ID_USER_SENDTIMEREQUEST:
					RequestTime(name);
					break;
				default:
					break;
			}
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
	m_ChannelsView.CreateEx(m_tab, rcDefault, WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);
	m_lv.Create(m_ChannelsView, 0, 0, LVS_REPORT | LVS_SINGLESEL | LVS_SORTASCENDING | LVS_SHOWSELALWAYS| WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
		WS_EX_CLIENTEDGE,IDC_LISTVIEW);
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
	m_ChannelsView.m_hWndClient = m_lv;
	m_tab.AddTab(m_ChannelsView, _T("channels"));

}

void CMainFrame::CreateImageList()
{
	BOOL res = m_il.Create(18, 18, ILC_COLOR32, 6, 1);
	ATLASSERT(res);
	if (res) {
		CClientDC dc(m_hWnd);
		CIconHandle icon;
		HBITMAP hbmp;
		UINT imgs[] = { IDI_ICON2,IDI_ICON3,IDI_ICON4,IDI_ICON6,IDI_ICON8,IDI_ICON5};
		const int count = sizeof(imgs) / sizeof(imgs[0]);

		for (int i = 0; i < count; i++) {
			icon.LoadIcon(imgs[i],18,18, LR_LOADTRANSPARENT);
			ATLASSERT(!icon.IsNull());
			m_il.AddIcon(icon.Detach());
		}
		m_tv.SetImageList(m_il);
	}
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
				if (!msg.Compare(_T("LIST")))
					m_lv.DeleteAllItems();
				else if (!msg.Compare(_T("QUIT"))) {
					msg.Append(_T(" :NO5 IRC https://fioresoft.net"));
				}
				else if (!msg.Compare(_T("PART")))	  {
					msg.Append(_T(" :NO5 IRC https://fioresoft.net"));
				}
				msg.Append(_T("\r\n"));
				m_sock.Send(CT2CA(msg), msg.GetLength());
				if (count >= 2 && (!arr[0].CompareNoCase(_T("/PRIVMSG"))) && (!arr[1].IsEmpty())) {
					if (arr[1].GetAt(0) != '#') {
						CString s;
						CView* pView = CreatePrivateChannel(arr[1]);

						pView->SetSelEnd();
						pView->SetTextColor(0xaa0000);
						pView->AppendText(m_nick);
						pView->AppendText(_T(": "));
						pView->ResetFormat();
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
						pView->ResetFormat();
						pView->AppendText(msg);
					}
				}
			}
			else if (!m_bInChannel) {
				ViewData* p = GetViewData();
				CView* pView = CreatePrivateChannel(p->name);
				
				SendPrivateMsg(p->name, msg);

				pView->SetSelEnd();
				pView->SetTextColor(0xaa0000);
				pView->AppendText(m_nick);
				pView->AppendText(_T(": "));
				pView->ResetFormat();
				pView->AppendText(msg);
			}
			else if (m_bInChannel) {
				CView* pView = CreateChannel(m_NameOrChannel);
				SendChannelMsg(m_NameOrChannel, msg);
				pView->SetSelEnd();
				pView->SetTextColor(0xaa0000);
				pView->AppendText(m_nick);
				pView->AppendText(_T(": "));
				pView->ResetFormat();
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

ViewData* CMainFrame::GetViewDataByName(LPCTSTR name)
{
	CString _name = name;
	EnumData data(_name, CWindow(m_hWndMDIClient));
	EnumChildWindows(m_hWndMDIClient, EnumChildGetDataByName, (LPARAM)&data);
	return data.pData;
}

CView* CMainFrame::GetViewByName(LPCTSTR name)
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

CView* CMainFrame::CreatePrivateChannel(LPCTSTR name)
{
	CView* p = CreateViewIfNoExist(name, VIEW_PVT);
	ATLASSERT(p);
	// TODO: do a whois on the person
	ActivateViewByName(name);
	return p;
}
CView* CMainFrame::CreateChannel(LPCTSTR channel)
{
	CView* p = CreateViewIfNoExist(channel, VIEW_CHANNEL);
	ATLASSERT(p);
	// TODO: query the mode of the channel
	ActivateViewByName(channel);
	return p;
}
bool CMainFrame::ActivateViewByName(LPCTSTR name)
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
		m_JoinChannel = ini.GetString(_T("settings"), _T("channel"), NULL, MAX_PATH);
		res = ini.GetStringList(_T("settings"), _T("servers"), m_servers);
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
							if (ini.WriteString(_T("settings"), _T("channel"), m_JoinChannel)) {
								res = ini.WriteStringList(_T("settings"), _T("servers"), m_servers);
							}
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
	dlg.m_JoinChannel = m_JoinChannel;
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
		m_JoinChannel = dlg.m_JoinChannel;
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
			m_tv.InsertItem(m_server,6,6, NULL, TVI_ROOT);
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
	int nTio;

	if (nBang >= 0)
		nTio = user.Find('~', nBang);
	else
		nTio = user.Find('~', 0);
	int nAt = -1;
	if (nTio >= 0)
		nAt = user.Find('@', nTio);

	if (user[0] == ':')
		nick = user.Right(user.GetLength() - 1);
	if(nBang >= 0)
		nick = user.Left(nBang);
	nick = nick.Right(nick.GetLength() - 1);
	if(nick[0] == ':')
		nick = nick.Right(nick.GetLength() - 1);
	if (nTio >= 0 && nAt >= 0) {
		name = user.Mid(nTio + 1, nAt - nTio - 1);
		ip = user.Right(user.GetLength() - nAt + 1);
	}
	else if(nBang >= 0)
		ip = user.Right(user.GetLength() - nBang + 1);
	return /*nBang >= 0 && */ !nick.IsEmpty();
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
	//wsbuf.len = sizeof(buf) - 1;
	wsbuf.len = sizeof(buf);
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
	if (res == (sizeof(buf))) {
		ATLTRACE(_T("returning %d\n"), buffer.GetDataLen());
		//::Sleep(100);
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
					OnChannelList(channel, users, topic);
				}
			}
			else if (code.Compare(_T("321")) == 0) {
				OnBeginningOfList();
			}
			else if (code.Compare(_T("323")) == 0) {
				OnEndOfList();
			}
			else if (code.Compare(_T("324")) == 0) { // mode of the channel
				if (count2 >= 5) {
					channel = params[3]; ATLASSERT(channel[0] == '#');
					OnChannelMode(channel, params[4]);
				}
			}
			else if (!code.Compare(_T("221"))) { // mode of user
				if (count2 >= 5) {
					nick = params[3];
					OnUserMode(nick, params[4]);
				}
			}
			else if (code.Compare(_T("329")) == 0) { // creation time of the channel
				if (count2 >= 5) {
					time_t time = _wtoi(params[4]);
					OnChannelCreationTime(params[3], time);
				}
			}
			else if (!code.Compare(_T("375"))) {
				OnBeginMOTD(m_NameOrChannel);
			}
			else if (!code.Compare(_T("376"))) {
				OnEndMOTD(m_NameOrChannel);
			}
			else if (!code.Compare(_T("372"))) { // MOTD line
				CString msg;

				for (int i = 3; i < params.GetSize(); i++) {
					msg += params[i];
					msg += ' ';
				}
				OnMOTD(m_NameOrChannel, msg);
			}
			else if (!code.Compare(_T("353"))) { // names in channel
				if (count2 >= 5 && params[4].GetAt(0) == '#')
					m_NameOrChannel = params[4];
				OnNamesInChannel(params[4], params);
			}
			else if (!code.Compare(_T("331"))) { // no topic
				if (count2 >= 4) {
					if (m_NameOrChannel.IsEmpty() || params[3].GetAt(0) == '#')
						m_NameOrChannel = params[3];
					OnNoTopic(m_NameOrChannel);
				}
			}
			else if (!code.Compare(_T("332"))) { // topic
				if (m_NameOrChannel.IsEmpty() || params[3].GetAt(0) == '#')
					m_NameOrChannel = params[3];
				
				OnTopic(m_NameOrChannel, lines[i]);
			}
			else if (!code.Compare(_T("333"))) { // who set the topic and when
				if (count2 >= 6) {
					CString channel = params[3];
					CString nick = params[4];
					time_t time = _wtoi(params[5]);
					
					OnWhoSetTheTopic(channel,nick, time);
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
					
				for (int i = 3; i < params.GetSize(); i++) {
					msg += params[i];
					msg += ' ';
				}
				msg += _T("\n");
				if (m_bInChannel) {
					OnChannelMsg(channel,nick, msg);
				}
				else {
					OnPrivateMsg(channel, nick, msg);
				}
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
				OnUserQuit(m_NameOrChannel, nick, str);

			}
			else if (!code.CompareNoCase(_T("JOIN"))) {
				CString nick, user, ip;

				channel = params[2];
				m_NameOrChannel = channel;
				bool b = ParseUserName(str, nick, user, ip);
				ATLASSERT(nick[0] != ':');
				OnUserJoin(channel, nick);
			}
			else if (!code.CompareNoCase(_T("PART"))) {
				CView* p = GetViewByName(m_NameOrChannel);

				if (!p || !p->IsWindow())
					continue;
				str = params[0];
				CString nick, user, ip;
				bool b = ParseUserName(str, nick, user, ip);
				//ATLASSERT(b);

				if (params[1]) {
					for (int i = 1; i < count2; i++) {
						str += params[i];
						str += ' ';
					}
					str += _T("\n");
				}
				OnUserPart(params[2], nick, str);

			}
			else if (!code.CompareNoCase(_T("NOTICE"))) {
				OnNotice(NULL, NULL, lines[i]);
				pView->AppendText(_T("\n"));
			}
			else if (_wtoi(code)) {
				OnUnknownCmd(lines[i]);
			}
		}

		if (count2 >= 1) {

			cmd = params[0];

			if (cmd.CompareNoCase(_T("PING")) == 0) {
				OnPing();
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
	CStringA cmd;
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
		SendPass(m_pass);
	}
	SendNick(m_nick);
	SendUser(m_user, m_name);

	//m_sock.SendString("USER phpnewbie 0 * :fernando\r\n");
	if (m_JoinChannel.IsEmpty()) {
		//::Sleep(1000);
		//ListChannels();
	}
	else {
		JoinChannel(m_JoinChannel);
	}
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

void CMainFrame::OnChannelList(LPCTSTR channel, LPCTSTR users, LPCTSTR topic)
{
	int insert = m_lv.InsertItem(LVIF_TEXT, 0, channel, 0, 0, 0, 0);
	m_lv.SetItemText(insert, 1, users);
	m_lv.SetItemText(insert, 2, topic);
}
void CMainFrame::OnBeginningOfList()
{
	CView* pView = GetActiveView();
	pView->AppendText(_T("beggining of list \n"));
}
void CMainFrame::OnEndOfList()
{
	CView* pView = GetActiveView();
	pView->AppendText(_T("end of list \n"));
}
void CMainFrame::OnChannelMode(LPCTSTR channel, LPCTSTR modes)
{
	CView* pView = GetActiveView();
	CString msg;

	if(wcschr(modes,'c') != NULL)
		m_bNoColors = true;
	else
		m_bNoColors = false;
	msg = _T("MODE of "); msg += channel; msg += _T(" is "); msg += modes;
	pView->AppendText(msg + '\n');
}

void CMainFrame::OnUserMode(LPCTSTR user, LPCTSTR modes)
{
	CView* pView = GetActiveView();
	CString msg;

	msg = _T("MODE of "); msg += user; msg += _T(" is "); msg += modes;
	pView->AppendText(msg + '\n');
}

void CMainFrame::OnChannelCreationTime(LPCTSTR channel, time_t time)
{
	CString msg;

	if (time) {
		CView* pView = GetActiveView();
		struct tm* ptm = std::localtime(&time);
		CString t = _tasctime(ptm);
		msg = channel; msg += _T(" was created on ") + t;
		pView->AppendText(msg);
	}
}
void CMainFrame::OnBeginMOTD(LPCTSTR channel)
{
	CView* pView = GetActiveView();
	pView->SetSelEnd();
	pView->SetTextColor(0x00cc00, SCF_SELECTION);
	pView->AppendText(_T("message of the day:\n"));
}
void CMainFrame::OnMOTD(LPCTSTR channel, LPCTSTR msg)
{
	CView* pView = GetActiveView();

	pView->SetSelEnd();
	pView->SetTextColor(0x00aa00, SCF_SELECTION);
	pView->SetTextFontName(_T("Courier"), SCF_SELECTION);
	pView->AppendText(msg);
	pView->AppendText(_T("\n"));
	pView->ResetFormat();
}
void CMainFrame::OnEndMOTD(LPCTSTR channel)
{
	CView* pView = GetActiveView();
	pView->SetSelEnd();
	pView->SetTextColor(0x00cc00, SCF_SELECTION);
	pView->AppendText(_T("end of message of the day:\n"));

}
void CMainFrame::OnNoTopic(LPCTSTR channel)
{
	CView* p = CreateViewIfNoExist(channel,ViewType::VIEW_CHANNEL);

	CNo5TreeItem parent = m_tv.FindItem(m_server, TRUE, FALSE);
	if (!m_tv.FindItem(channel, FALSE, TRUE))
		m_tv.InsertItem(channel,6,6, parent, TVI_SORT);
	GetMode(channel);
}
void CMainFrame::OnTopic(LPCTSTR channel, LPCTSTR topic)
{
	CView* p = CreateViewIfNoExist(channel, ViewType::VIEW_CHANNEL);

	CNo5TreeItem parent = m_tv.FindItem(m_server, TRUE, FALSE);
	if (!m_tv.FindItem(channel, FALSE, TRUE))
		m_tv.InsertItem(channel,6,6, parent, TVI_SORT);
	/*if (m_bNoColors)
		p->AppendText(topic);
	else*/
		p->AppendTextIrc(topic);
	p->AppendText(_T("\n"));
	p->ResetFormat();
	GetMode(channel);

}
void CMainFrame::OnWhoSetTheTopic(LPCTSTR channel, LPCTSTR user, time_t time)
{
	CString t;
	CView* pView = GetViewByName(channel);

	if (time) {
		struct tm* ptm = std::localtime(&time);
		t = _tasctime(ptm);
	}

	CString msg = user; msg += _T(" set the topic for channel: "); msg += channel;
	if (time) {
		msg += _T(" on ") + t;
	}
	msg += '\n';
	pView->AppendText(msg);
}
void CMainFrame::OnNamesInChannel(LPCTSTR channel, const CSimpleArray<CString>& args)
{
	CView* p = CreateChannel(channel);
	ActivateViewByName(channel);
	CNo5TreeItem parent = m_tv.FindItem(channel, FALSE, TRUE);
	
	if (parent) {
		CString name;
		int iImage;

		for (int i = 5; i < args.GetSize(); i++) {
			if (args[i].GetAt(0) == ':') {
				name = args[i].Right(args[i].GetLength() - 1);
			}
			else
				name = args[i];
			if (name[0] == '~') {
				// founder
				iImage = 0;
			}
			else if (name[0] == '&') {
				// protected
				iImage = 1;
			}
			else if (name[0] == '@') {
				// operator
				iImage = 2;
			}
			else if (name[0] == '%') {
				// half op
				iImage = 3;
			}
			else if (name[0] == '+') {
				// voice
				iImage = 4;
			}
			else {
				// normal user
				iImage = 5;
			}
			if (iImage < 5) {
				name = name.Right(name.GetLength() - 1);
			}
			m_tv.InsertItem(name,iImage,iImage, parent, TVI_SORT);
		}
	}
}

void CMainFrame::OnChannelMsg(LPCTSTR channel,LPCTSTR user, LPCTSTR msg)
{
	CString str = user; str += ':';

	m_NameOrChannel = channel;
	CView* p = CreateViewIfNoExist(channel,ViewType::VIEW_CHANNEL);
	p->SetTextColor(0x000088);
	p->AppendText(str);
	p->SetTextColor(0);
	if (m_bNoColors)
		p->AppendText(msg);
	else {
		p->AppendTextIrc(msg);
		p->ResetFormat();
	}
	p->AppendText(_T("\n"));
}
void CMainFrame::OnPrivateMsg(LPCTSTR channel, LPCTSTR from, LPCTSTR msg)
{
	if (wcschr(msg,'\x1')) { // ctcp
		CView* pView = GetActiveView();
		CString tag = RemoveDelimiters2(msg);
		tag = tag.Trim();
		if (!tag.Compare(_T("VERSION"))) {
			AnswerVersionRequest(from);
			GetMode(channel);
		}
		else if (!tag.Find(_T("PING"))) {
			AnswerPingRequest(from);
		}
		else if (!tag.Compare(_T("USERINFO"))) {
			AnswerUserinfoRequest(from);
		}
		else if (!tag.Compare(_T("TIME"))) {
			AnswerTimeRequest(from);
		}
	}
	else {
		CView *p = CreateViewIfNoExist(from, ViewType::VIEW_PVT);
		p->SetTextColor(0x000088);
		p->AppendText(from);
		p->AppendText(_T(": "));
		p->SetTextColor(0);
		if (m_bNoColors) {
			p->AppendText(msg);
		}
		else {
			p->AppendTextIrc(msg);
			p->ResetFormat();
		}
		p->AppendText(_T("\n"));
	}
}

void CMainFrame::OnUserQuit(LPCTSTR channel, LPCTSTR user, LPCTSTR msg)
{
	CView* pView = GetViewByName(channel);
	ATLASSERT(pView);
	pView->SetTextColor(0xa0a0a0);
	pView->AppendText(msg);
	m_tv.DeleteItem(user, TRUE, TRUE);
}
void CMainFrame::OnUserJoin(LPCTSTR channel, LPCTSTR user)
{
	CView* p;
	CString msg;

	if (!m_nick.CompareNoCase(user)) {
		p = GetActiveView();
	}
	else {
		p = GetViewByName(channel);
	}

	msg = user; msg += _T(" enters the channel ");
	msg += channel;
	msg += '\n';
	if (p) {
		p->SetTextColor(0x108800);
		p->AppendText(msg);
	}
	m_marquee.AddItem(msg);
	if (user != m_nick) {
		CNo5TreeItem parent = m_tv.FindItem(channel, FALSE, TRUE);

		if (parent) {
			CString name = user;
			int iImage;

			if(name[0] == ':') 
				name =name.Right(name.GetLength() - 1);
				
				if (name[0] == '~') {
					// founder
					iImage = 0;
				}
				else if (name[0] == '&') {
					// protected
					iImage = 1;
				}
				else if (name[0] == '@') {
					// operator
					iImage = 2;
				}
				else if (name[0] == '%') {
					// half op
					iImage = 3;
				}
				else if (name[0] == '+') {
					// voice
					iImage = 4;
				}
				else {
					// normal user
					iImage = 5;
				}
				if (iImage < 5) {
					name = name.Right(name.GetLength() - 1);
				}
				m_tv.InsertItem(name, iImage, iImage, parent, TVI_SORT);
		}
	}
}
void CMainFrame::OnUserPart(LPCTSTR channel, LPCTSTR user, LPCTSTR msg)
{
	CView* p = GetActiveView();
	CString txt = user;

	txt += _T(" leaves the channel: "); txt += channel; txt += ' '; txt += msg;
	p->SetTextColor(0x108800);
	p->AppendText(txt);
	p->AppendText(_T("\n"));
	m_marquee.AddItem(txt);
	m_tv.DeleteItem(user, TRUE, TRUE);
}
void CMainFrame::OnNotice(LPCTSTR channel, LPCTSTR user, LPCTSTR msg)
{
	CView* pView = GetActiveView();
	if (wcschr(msg, '\x1')) { // ctcp
		CString tag = RemoveDelimiters2(msg);

		//tag = tag.Trim();
		if (!tag.Find(_T("PING"))) {
			CString code;
			int index = tag.Find(' ');
			tag = tag.Right(tag.GetLength() - index - 1);
			time_t t = _wtoi(tag);
			double dif = difftime(t, m_t);
			m_t = 0;
			code.Format(_T("difference in ping is %f\n"), dif);
			pView->AppendText(code);
		}
		pView->AppendText(tag);
	}
	else {
		if (m_bNoColors)
			pView->AppendText(msg);
		else
			pView->AppendTextIrc(msg);
	}
	//pView->AppendText(_T("\n"));
}
void CMainFrame::OnPing()
{
	CStringA pong;
	CView* pView = GetActiveView();

	pong += "PONG ";
	pong += m_nick;
	pong += "\n";

	m_sock.SendString(pong);
	pView->AppendText(_T("ping received - pong returned\r\n"));
}
void CMainFrame::OnUnknownCmd(LPCTSTR line)
{
	CView* p = GetActiveView();
	p->AppendText(line);
	p->AppendText(_T("\n"));
}

void CMainFrame::SendChannelMsg(LPCTSTR channel, LPCTSTR msg)
{
	CStringA  code = "PRIVMSG ";

	code += channel;
	code += " :";
	code += msg;
	code += "\r\n";
	m_sock.SendString(code);
}
void CMainFrame::SendPrivateMsg(LPCTSTR to, LPCTSTR msg)
{
	CStringA  code = "PRIVMSG ";

	code += to;
	code += " :";
	code += msg;
	code += "\r\n";
	m_sock.SendString(code);
}

void CMainFrame::SendNoticeMsg(LPCTSTR to, LPCTSTR msg)
{
	CStringA  code = "NOTICE ";

	code += to;
	code += " :";
	code += msg;
	code += "\r\n";
	m_sock.SendString(code);
}

void CMainFrame::JoinChannel(LPCTSTR channel)
{
	CStringA  code = "JOIN ";

	code += channel;
	code += "\r\n";
	m_sock.SendString(code);
}
void CMainFrame::LeaveChannel(LPCTSTR channel, LPCTSTR msg)
{
	CStringA  code = "PART ";

	code += channel;
	if (msg) {
		code += " :";
		code += msg;
	}
	code += "\r\n";
	m_sock.SendString(code);
}
void CMainFrame::Quit(LPCTSTR msg)
{
	CStringA  code = "QUIT";

	if (msg) {
		code += " :";
		code += msg;
	}
	code += "\r\n";
	m_sock.SendString(code);
}
void CMainFrame::SendPass(LPCTSTR pass)
{
	CStringA  code = "PASS ";

	code += pass;
	code += "\r\n";
	m_sock.SendString(code);
}
void CMainFrame::SendNick(LPCTSTR nick)
{
	CStringA  code = "NICK ";

	code += nick;
	code += "\r\n";
	m_sock.SendString(code);
}
void CMainFrame::SendUser(LPCTSTR user, LPCTSTR realname)
{
	CStringA code = "USER ";

	code += user;
	code += ' ';
	code += "0 * :";
	code += realname;
	code += "\r\n";
	m_sock.SendString(code);
}
void CMainFrame::ListChannels()
{
	CStringA  code = "LIST";
	code += "\r\n";
	m_sock.SendString(code);
}

void CMainFrame::GetMode(LPCTSTR NameOrChannel)
{
	CStringA cmd;
	cmd = "MODE "; cmd += NameOrChannel; cmd += "\r\n";
	m_sock.SendString(cmd);
}

void CMainFrame::RequestVersion(LPCTSTR from)
{
	SendPrivateMsg(from, _T("\001VERSION\001"));
}
void CMainFrame::RequestUserinfo(LPCTSTR from)
{
	SendPrivateMsg(from, _T("\001USERINFO\001"));
}
void CMainFrame::RequestPing(LPCTSTR from)
{
	time_t t = time(&m_t);
	CString code;
	
	code.Format(_T("\001PING %lu\001"),(unsigned int)t);
	SendPrivateMsg(from, code);

}
void CMainFrame::RequestTime(LPCTSTR from)
{
	SendPrivateMsg(from, _T("\001TIME\001"));
}

void CMainFrame::AnswerVersionRequest(LPCTSTR from)
{
	NO5TL::CVersionInfo vi;
	CString tag;
	CView* pView = GetActiveView();

	BOOL res = vi.Init(PRODUCT_NAME); ATLASSERT(res);
	CString product, version;
	const int cp = 0x40904b0;

	vi.GetString(cp, _T("ProductName"), product);
	vi.GetString(cp, _T("ProductVersion"), version);
	tag.Format(_T("\001VERSION %s-%s-Windows by https://fioresoft.net\001"), (LPCTSTR)product, (LPCTSTR)version);
	SendNoticeMsg(from, tag);
	pView->AppendText(_T("Sent CTCP VERSION reply -> ") + tag + '\n');
}
void CMainFrame::AnswerUserinfoRequest(LPCTSTR from)
{
	CString code;

	code.Format(_T("\001USERINFO %s\001"), _T("this is just a test"));
	SendNoticeMsg(from, code);
	CView* pView = GetActiveView();
	pView->AppendText(_T("Sent CTCP USERINFO reply -> ") + code + '\n');

}
void CMainFrame::AnswerPingRequest(LPCTSTR from)
{
	time_t t = time(&t);
	CString code;

	code.Format(_T("\001PING %lu\001"), (unsigned int)t);
	SendNoticeMsg(from, code);
	CView* pView = GetActiveView();
	pView->AppendText(_T("Sent CTCP PING reply -> ") + code + '\n');
}
void CMainFrame::AnswerTimeRequest(LPCTSTR from)
{
	time_t t = time(&t);
	struct tm* ptm = localtime(&t);
	CString s = asctime(ptm);
	CString msg;

	s.TrimRight();
	msg.Format(_T("\001TIME %s\001"), (LPCTSTR)s);
	SendNoticeMsg(from, msg);
	CView* pView = GetActiveView();
	pView->AppendText(_T("Sent CTCP TIME reply -> ") + msg + '\n');
}

// CChannelsViewFrame
LRESULT CChannelsViewFrame::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_CHANNELSVIEW, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	UIAddToolBar(hWndToolBar);
	m_hWndClient = m_lv;
	return 0;
}

LRESULT CChannelsViewFrame::OnFileOpen(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_pf->SendMessage(WM_CHANNELSFILEOPEN);
	return 0;
}

LRESULT CChannelsViewFrame::OnFileSave(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_pf->SendMessage(WM_CHANNELSFILESAVE);
	return 0;
}

LRESULT CChannelsViewFrame::OnFileUpdate(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_pf->SendMessage(WM_CHANNELSFILEUPDATE);
	return 0;
}

BOOL CChannelsViewFrame::PreTranslateMessage(MSG* pMsg)
{
	if (_baseClass::PreTranslateMessage(pMsg))
		return TRUE;

	return FALSE;
}

BOOL CChannelsViewFrame::OnIdle()
{
	UIUpdateToolBar();
	return FALSE;
}