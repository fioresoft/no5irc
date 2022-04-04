// View.cpp : implementation of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "TextStream.h"
#include "View.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "AboutDlg.h"
#include "usermsgs.h"

extern CAppModule _Module;
BOOL ToolbarAddControl(HWND hToolBar, HWND hWnd, int pos, int count, BOOL bComboBox);

/*
*
0	White	(255,255,255)
1	Black	(0,0,0)
2	Blue	(0,0,127)
3	Green	(0,147,0)
4	Light Red	(255,0,0)
5	Brown	(127,0,0)
6	Purple	(156,0,156)
7	Orange	(252,127,0)
8	Yellow	(255,255,0)
9	Light Green	(0,252,0)
10	Cyan	(0,147,147)
11	Light Cyan	(0,255,255)
12	Light Blue	(0,0,252)
13	Pink	(255,0,255)
14	Grey	(127,127,127)
15	Light Grey	(210,210,210)
*/

/*
COLORREF ColorFromCode(int code)
{
	switch (code)
	{
		case 0:
			return RGB(255, 255, 255);
		case 1:
			return 0;
		case 2:
			return RGB(0, 0, 127);
		case 3:
			return RGB(0, 147, 0);
		case 4:
			return RGB(255, 0, 0);
		case 5:
			return RGB(127, 0, 0);
		case 6:
			return RGB(156, 0, 156);
		case 7:
			return RGB(252, 127, 0);
		case 8:
			return RGB(255, 255, 0);
		case 9:
			return RGB(0, 252, 0);
		case 10:
			return RGB(0, 147, 147);
		case 11:
			return RGB(0, 255, 255);
		case 12:
			return RGB(0, 0, 252);
		case 13:
			return RGB(255, 0, 255);
		case 14:
			return RGB(127, 127, 127);
		case 15:
			return RGB(210,210, 210);
		default:
			return 0;
	}
}
*/

BOOL CView::PreTranslateMessage(MSG* pMsg)
{
	if (TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) {
		return TRUE;
	}
	return FALSE;
}
void CView::AppendTextIrc(LPCTSTR lpstrText)
{
	// m_iFore
	
	m_ts.ParseIRCCode(lpstrText);
	m_ts.SendStreamToRichEdit(*this);
	//m_ts.atoms.RemoveAll();
}

//int CView::AppendTextIrc(LPCTSTR lpstrText)
//{
//	const TCHAR* p = lpstrText;
//	const TCHAR* pEnd = p;
//	int fore, back;
//	bool setfore = false, setback = false;
//
//	while (*p) {
//		if (*p == 0x3) {
//			if (p > pEnd) {
//				CString s(pEnd, p - pEnd);
//				AppendText(s);
//			}
//			p++;
//			if (*p >= '0' && *p <= '9') {
//				fore = (*p - '0') * 10;
//				setfore = true;
//				p++;
//				if (*p >= '0' && *p <= '9') {
//					fore += (*p - '0');
//					pEnd = ++p;
//				}
//				else {
//					pEnd = p;
//					fore /= 10;
//				}
//				if (*p == ',') {
//					++p;
//					if (*p >= '0' && *p <= '9') {
//						back = (*p - '0') * 10;
//						setback = true;
//						p++;
//						if (*p >= '0' && *p <= '9') {
//							back += (*p - '0');
//							pEnd = ++p;
//						}
//						else {
//							pEnd = p;
//							back /= 10;
//						}
//					}
//				}
//				else
//					pEnd = p;
//			}
//			else {
//			//	ResetFormat();
//				pEnd = p;
//				continue;
//			}
//			if (setfore) {
//				SetTextColor(ColorFromCode(fore));
//				setfore = false;
//			}
//			if (setback) {
//				SetTextBkColor(ColorFromCode(back));
//				setback = false;
//			}
//		}
//		else if (*p == 0x0f) { // reset
//			if (p > pEnd) {
//				CString s(pEnd, p - pEnd);
//				AppendText(s);
//			}
//			ResetFormat();
//			pEnd = ++p;
//		}
//		else if (*p == 0x11) { // monospaced
//			if (p > pEnd) {
//				CString s(pEnd, p - pEnd);
//				AppendText(s);
//			}
//			SetTextFontName(_T("Courier"));
//			pEnd = ++p;
//		}
//		else if (*p == 0x1E) {
//			if (p > pEnd) {
//				CString s(pEnd, p - pEnd);
//				AppendText(s);
//			}
//			SwitchStrike();
//			pEnd = ++p;
//		}
//		else if (*p == 0x1F) {
//			if (p > pEnd) {
//				CString s(pEnd, p - pEnd);
//				AppendText(s);
//			}
//			SwitchUnderline();
//			pEnd = ++p;
//		}
//		else if (*p == 0x1D) {
//			if (p > pEnd) {
//				CString s(pEnd, p - pEnd);
//				AppendText(s);
//			}
//			SwitchItalic();
//			pEnd = ++p;
//		}
//		else if (*p == 0x2) {
//			if (p > pEnd) {
//				CString s(pEnd, p - pEnd);
//				AppendText(s);
//			}
//			SwitchBold();
//			pEnd = ++p;
//		}
//		else if (*(p + 1) == 0 && (p > pEnd))
//		{
//			CString s(pEnd, p - pEnd + 1);
//			AppendText(s);
//			ResetFormat();
//			p++;
//		}
//		else
//			p++;
//	}
//	return 0;
//}


void CView::ResetFormat()
{
	SetTextColor(0);
	SetTextBkColor(0xffffff);
	SetUnderline(FALSE, SCF_SELECTION);
	SetBold(FALSE, SCF_SELECTION);
	SetItalic(FALSE, SCF_SELECTION);
	SetTextFontName(_T("Arial"));
}

LRESULT CView::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	SetTextColor(0, SCF_ALL);
	SetTextBkColor(0xffffff, SCF_ALL);
	SetUnderline(FALSE, SCF_ALL);
	SetBold(FALSE, SCF_ALL);
	SetItalic(FALSE, SCF_ALL);
	SetTextFontName(_T("Arial"), SCF_ALL);
	SetTextHeight(12, SCF_ALL);
	SetIndent(30);
	m_hAccel = LoadAccelerators(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAINFRAME));
	ATLASSERT(SUCCEEDED(m_hAccel));
	CMessageLoop* loop = _Module.GetMessageLoop();
	loop->AddMessageFilter(this);
	bHandled = FALSE;
	return 0;
}

LRESULT CView::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CMessageLoop* loop = _Module.GetMessageLoop();
	loop->RemoveMessageFilter(this);
	return 0;
}

 //CHtmlView
void CHtmlView::AppendTextIrc(LPCTSTR lpstrText)
{
	// m_iFore

	m_ts.ParseIRCCode(lpstrText);
	m_ts.SendStreamToHtmlEdit(*this);
	//m_ts.atoms.RemoveAll();
}
////
// CBottom

void CBottom::CreateClient()
{
	m_client.Create(m_hWnd, rcDefault, 0, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NOHIDESEL | ES_WANTRETURN | ES_SUNKEN, WS_EX_CLIENTEDGE);
	AddTab(m_client,_T("input"));
}

void CBottom::CreateToolBar()
{
	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_rebar, IDR_BOTTOM_FRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	m_tb.Attach(hWndToolBar);
	AddSimpleReBarBand(hWndToolBar, NULL, FALSE);
	UIAddToolBar(hWndToolBar);
	m_tb.SetButtonInfo(0, TBIF_BYINDEX | TBIF_STATE, 0, TBSTATE_HIDDEN, NULL, 0, 0, 0, 0);
	m_tb.SetButtonInfo(ID_EDIT_BOLD, TBIF_STATE|TBIF_STYLE, BTNS_CHECK, TBSTATE_ENABLED, 0, 0, 0, 0, 0);
	m_tb.SetButtonInfo(ID_EDIT_ITALIC, TBIF_STATE| TBIF_STYLE, BTNS_CHECK, TBSTATE_ENABLED, NULL, BTNS_CHECK, 0, 0, 0);
	m_tb.SetButtonInfo(ID_EDIT_UNDERLINE, TBIF_STATE| TBIF_STYLE, BTNS_CHECK, TBSTATE_ENABLED, NULL, BTNS_CHECK, 0, 0, 0);
	
}

LRESULT CBottom::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	_baseClass::OnCreate(uMsg, wParam, lParam, bHandled);
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE| RBS_TOOLTIPS);
	m_rebar.Attach(m_hWndToolBar);
	//UIAddChildWindowContainer(m_rebar);
	CreateComboFonts();
	CreateComboSize();
	CreateColorCombos();
	CreateToolBar();
	CreateToolTip();
	CreateClient();
	m_hAccel = ::LoadAccelerators(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAINFRAME));
	m_hWndClient = m_client;
	
	//
	//
	return 0;
}

LRESULT CBottom::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	return 0;
}


BOOL CBottom::PreTranslateMessage(MSG* pMsg)
{
	if ((m_hAccel != NULL) && ::TranslateAccelerator(this->m_hWnd, m_hAccel, pMsg))
		return TRUE;
	return FALSE;
}
BOOL CBottom::OnIdle()
{
	return FALSE;
}

BOOL CBottom::OnEditComplete(CString& cmd)
{
	BOOL res = FALSE;

	if (true /*GetFocus() == m_hWnd*/) {
		CPoint ptCaret;
		CWindow frame = GetParent();
		CRect rcEdit;
		HWND hWndFocus;

		//frame = frame.GetParent();
		GetCaretPos(&ptCaret);
		GetWindowRect(rcEdit);

		ClientToScreen(&ptCaret);
		frame.ScreenToClient(&ptCaret);
		ptCaret.y = rcEdit.top;
		ptCaret.x = rcEdit.left;

		CAutoCompleteDlg dlg(cmd, ptCaret, m_status);

		hWndFocus = GetFocus();

		if (dlg.DoModal() == IDOK) {
			res = TRUE;
		}
		::SetFocus(hWndFocus);

	}
	return res;
}

void CBottom::CreateComboFonts()
{
	CRect rc;

	rc.left = rc.top = 0;
	rc.right = 0;
	rc.bottom = 250;
	m_cbFonts.Create(m_rebar, rc, (LPCTSTR)0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | \
		CBS_DROPDOWNLIST | CBS_SORT | CBS_OWNERDRAWVARIABLE | CBS_HASSTRINGS, 0, IDC_COMBO_FONTS);
	m_cbFonts.SetExtendedFontStyle(FPS_EX_TYPEICON | FPS_EX_SHOWFACE);
	//ToolbarAddControl(m_tb, m_cbFonts, 1, 6, TRUE);
	AddSimpleReBarBand(m_cbFonts,_T("Fonts"),FALSE,150,TRUE);
	m_cbFonts.Dir();
	int index = m_cbFonts.FindString(0, _T("Arial"));
	if (index >= 0)
		m_cbFonts.SetCurSel(index);
	UpdateLayout(FALSE);
}

void CBottom::CreateComboSize()
{
	CRect rc;

	rc.bottom = 250;
	m_cbSize.Create(m_rebar, rc, (LPCTSTR)0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | CBS_DROPDOWNLIST, 0,
		IDC_COMBO_SIZE);
	CString text;

	for (int i = 6; i <= 36; i += 2) {
		text.Format(_T("%d"), i);
		m_cbSize.AddString(text);
	}
	m_cbSize.SelectString(0, _T("12"));
	//ToolbarAddControl(m_tb, m_cbSize, 8, 3, TRUE);
	AddSimpleReBarBand(m_cbSize,_T("Size:"),FALSE,50,TRUE);
	m_cbSize.SetFont(AtlGetDefaultGuiFont());
}

void CBottom::CreateColorCombos()
{
	CRect rc;

	rc.bottom = 250;
	m_cbFore.Create(m_rebar, rc, 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED, 0, IDC_COMBO_FORE);
	//ToolbarAddControl(m_tb, m_cbFore, 15, 3, TRUE);
	AddSimpleReBarBand(m_cbFore, _T("Foreground:"), FALSE, 70, TRUE);
	for (int i = 0; i <= 15; i++) {
		m_cbFore.AddColor(i, ColorFromCode(i));
	}
	m_cbBack.Create(m_rebar, rc, 0, WS_CHILD|WS_VISIBLE | WS_VSCROLL | WS_BORDER | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED, 0, IDC_COMBO_BACK);
	//ToolbarAddControl(m_tb, m_cbBack, 19, 3, TRUE);
	AddSimpleReBarBand(m_cbBack, _T("Background:"), FALSE, 70, TRUE);
	for (int i = 0; i <= 15; i++) {
		m_cbBack.AddColor(i, ColorFromCode(i));
	}
	m_cbFore.SelectColor(0);
	m_cbBack.SelectColor(0xffffff);

}

void CBottom::DisableFormat()
{
	m_tb.SetButtonInfo(ID_EDIT_BOLD, TBIF_STATE, 0, TBSTATE_INDETERMINATE, NULL, 0, 0, 0, 0);
	m_tb.SetButtonInfo(ID_EDIT_ITALIC, TBIF_STATE, 0, TBSTATE_INDETERMINATE, NULL, 0, 0, 0, 0);
	m_tb.SetButtonInfo(ID_EDIT_UNDERLINE, TBIF_STATE, 0, TBSTATE_INDETERMINATE, NULL, 0, 0, 0, 0);
	m_cbFore.EnableWindow(FALSE);
	m_cbBack.EnableWindow(FALSE);
}

void CBottom::EnableFormat()
{
	m_tb.SetButtonInfo(ID_EDIT_BOLD, TBIF_STATE, 0, TBSTATE_ENABLED, NULL, 0, 0, 0, 0);
	m_tb.SetButtonInfo(ID_EDIT_ITALIC, TBIF_STATE, 0, TBSTATE_ENABLED, NULL, 0, 0, 0, 0);
	m_tb.SetButtonInfo(ID_EDIT_UNDERLINE, TBIF_STATE, 0, TBSTATE_ENABLED, NULL, 0, 0, 0, 0);
	m_cbFore.EnableWindow(TRUE);
	m_cbBack.EnableWindow(TRUE);
}

LRESULT CBottom::OnFontsSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int index = m_cbFonts.GetCurSel();
	CString font;
	m_cbFonts.GetLBText(index, font);
	m_client.SetTextFontName(font, SCF_ALL);
	return m_frame.SendMessage(WM_ONFONTCHANGE, 0, (LPARAM)(LPCTSTR)font);
}

LRESULT CBottom::OnSizeSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int index = m_cbSize.GetCurSel();
	CString text;
	m_cbSize.GetLBText(index, text);
	int size = _wtoi(text);
	m_client.SetTextHeight(size, SCF_ALL);
	return m_frame.SendMessage(WM_ONFONTSIZECHANGE, 0, (LPARAM)size);
}

LRESULT CBottom::OnBold(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static bool checked = false;

	checked = !checked;
	
	//m_client.AppendText(_T("\002"), TRUE);*/
	
	this->m_frame.m_bold = this->m_tb.IsButtonChecked(ID_EDIT_BOLD);
	return 0;
}
LRESULT CBottom::OnItalic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static bool checked = false;

	checked = !checked;
	//m_client.AppendText(_T("\x1d"), TRUE);
	m_frame.m_italic = this->m_tb.IsButtonChecked(ID_EDIT_ITALIC);
	return 0;
}
LRESULT CBottom::OnUnderline(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static bool checked = false;

	checked = !checked;
	//m_client.AppendText(_T("\x1f"), TRUE);
	m_frame.m_underline = this->m_tb.IsButtonChecked(ID_EDIT_UNDERLINE);
	return 0;
}
LRESULT CBottom::OnForeSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int fore = m_cbFore.GetCurSel();
	//int back = m_cbBack.GetCurSel();
	/*CString code;

	code.Format(_T("\003%02d,%02d"), fore, back);
	m_client.AppendText(code, TRUE);*/
	m_frame.m_iFore = fore;
	return 0;
}
LRESULT CBottom::OnBackSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//int fore = m_cbFore.GetCurSel();
	int back = m_cbBack.GetCurSel();
	//CString code;

	/*code.Format(_T("\003%02d,%02d"), fore, back);
	m_client.AppendText(code, TRUE);*/
	m_frame.m_iBack = back;
	return 0;
}

LRESULT CBottom::OnSelChange(int /*idCtrl*/, LPNMHDR pnmh, BOOL& bHandled)
{
	NMCTC2ITEMS* pData = (NMCTC2ITEMS*)pnmh;
	CReBarCtrl rebar = m_hWndToolBar;
	int nBandIndex = 0;// rebar.IdToIndex(IDR_BOTTOM_FRAME);	// toolbar is 2nd added band

	if (pData->iItem2 == 1 || pData->iItem2 == 2) {
		//rebar.ShowBand(nBandIndex, FALSE);
		m_rebar.ShowWindow(SW_HIDE);
		//UISetCheck(ID_VIEW_TOOLBAR, FALSE);
		UpdateLayout();
	}
	else if (pData->iItem2 == 0) {
		//rebar.ShowBand(nBandIndex, TRUE);
		m_rebar.ShowWindow(SW_SHOW);
		//UISetCheck(ID_VIEW_TOOLBAR, TRUE);
		UpdateLayout();
	}
	bHandled = FALSE;
	return 0;
}

LRESULT CBottom::OnLVTransfersRightClick(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
{
	return m_frame.OnLVFileTransfersRightClick(idCtrl, pnmh, bHandled);
}

int CBottom::GetDesiredHeight()
{
	CReBarCtrl rb = m_hWndToolBar;
	CClientDC dc(m_client);
	LPCTSTR text = _T("M");
	CSize sz;
	int h = 0;

	h = rb.GetBarHeight();
	dc.GetTextExtent(text, 1, &sz);
	h += sz.cy;
	// borders
	h += 150; // oh fuck, whatever TODO
	return h;
}

HWND CBottom::CreateToolTip()
{
	// Create the tooltip. g_hInst is the global instance handle.
	HWND hwndTip = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
		WS_POPUP | TTS_ALWAYSTIP | TTS_BALLOON,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		m_hWnd, NULL,
		_Module.GetModuleInstance(), NULL);

	//m_tt.Create(m_hWnd, 0, 0, WS_CHILD|WS_VISIBLE|TTS_ALWAYSTIP | TTS_NOPREFIX, 0,IDC_TT_FORE);

	m_tt = hwndTip;
	ATLASSERT(m_tt.IsWindow());
	//::SetWindowPos(m_tt, HWND_TOPMOST, 0, 0, 0, 0,SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	TTTOOLINFO ti = { sizeof(ti) };
	ti.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
	ti.uId = (UINT_PTR)(HWND(m_cbFore));
	ti.hwnd = m_hWnd;// m_tb;
	ti.lpszText = _T("Foreground Color");
	BOOL res = m_tt.AddTool(&ti);
	ATLASSERT(res);
	ti.uId = (UINT_PTR)(HWND(m_cbBack));
	ti.lpszText = _T("Background Color");
	res = m_tt.AddTool(&ti);
	ATLASSERT(res);

	return hwndTip;
}

// CBottomClient

LRESULT CBottomClient::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (wParam == VK_RETURN) {
		CString str;
		GetWindowText(str);
		m_lines.Add(str);
		str += _T("\r\n");
		SetWindowText(_T(""));
		m_frame.SendMessage(WM_MSGFROMBOTTOM, (WPARAM)(LPCTSTR)str);
		bHandled = TRUE;
	}
	else if (wParam == VK_TAB) {
		CHARRANGE cr;
		
		GetSel(cr);
		if (cr.cpMin == cr.cpMax) {
			TEXTRANGE tr;

			cr.cpMin = FindWordBreak(WB_LEFTBREAK, cr.cpMax);
			cr.cpMin == 0 ? 0 : cr.cpMin++;
			tr.chrg = cr;
			if (cr.cpMax > cr.cpMin) {
				CString sPartial;
				NO5TL::CStringArray arr;

				{
					NO5TL::CStringBuffer buf(sPartial, cr.cpMax - cr.cpMin + 1);
					tr.lpstrText = LPTSTR(buf);
					GetTextRange(&tr);
				}

				if (FindNames(sPartial, &arr)) {
					CString name;

					if (arr.GetSize() == 1) { // single match. append it
						name = arr[0];
					}
					else {
						CPoint ptCaret;
						CWindow frame;
						CRect rcEdit;
						HWND hWndFocus;

						GetCaretPos(&ptCaret);
						ClientToScreen(&ptCaret);
						m_frame.ScreenToClient(&ptCaret);
						GetWindowRect(rcEdit);
						ptCaret.y = rcEdit.top;

						CAutoCompleteDlg dlg(name, ptCaret, NULL, &arr);

						hWndFocus = GetFocus();
						if (dlg.DoModal(m_pBottom->m_hWnd) == IDOK) {

						}
						else
							name.Empty();
						::SetFocus(hWndFocus);

					}
					if (name.GetLength()) {
						SetSel(cr);
						ReplaceSel(name, TRUE);
					}
				}
				else
					bHandled = FALSE;
			}
		}
	}
	else if (wParam == VK_DOWN) {
		m_nPos++;

		if (m_nPos == m_lines.GetSize())
			m_nPos--;
		if (m_nPos >= 0 && m_lines.GetSize()) {
			CString line = m_lines[m_nPos];
			SetWindowText(line);
		}

	}
	else if (wParam == VK_UP) {
		m_nPos--;

		if (m_nPos < 0)
			m_nPos = 0;
		if (m_lines.GetSize()) {
			CString line = m_lines[m_nPos];
			SetWindowText(line);
		}
	}
	else
		bHandled = FALSE;
	return 0;
}

LRESULT CBottomClient::OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (!GetWindowTextLength()) {
		if (wParam == '/') {
			CString cmd;
			CString txt = '/';
			if (m_pBottom->OnEditComplete(cmd)) {
				txt += cmd;
				SetWindowText(txt);
			}
			bHandled = TRUE;
		}
		else
			bHandled = FALSE;
	}
	else if (wParam == '\t')
		bHandled = TRUE;
	else
		bHandled = FALSE;
	if (wParam == '\r' || wParam == '\n') {
		bHandled = TRUE;
	}
	return 0;
}

int CBottomClient::FindNames(LPCTSTR partial, CStringArray *pres)
{
	return (int)m_frame.SendMessage(WM_FINDUSER, (WPARAM)partial, (LPARAM)pres);
}

BOOL ToolbarAddControl(HWND hToolBar, HWND hWnd, int pos, int count, BOOL bComboBox)
{
	CToolBarCtrl tb = hToolBar;
	CWindow wnd = hWnd;
	CRect rcButton;
	BOOL res;

	res = tb.GetItemRect(pos, &rcButton);
	if (res) {
		if (bComboBox) {
			CRect rcWnd;
			CComboBox cb = hWnd;

			cb.SetItemHeight(-1, rcButton.Height() - 5);
			wnd.GetWindowRect(&rcWnd);
			wnd.MoveWindow(rcButton.left, rcButton.top, count * rcButton.Width(), rcWnd.Height());
		}
		else
			wnd.MoveWindow(rcButton.left, rcButton.top, count * rcButton.Width(), rcButton.Height());
	}
	return res;
}