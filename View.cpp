// View.cpp : implementation of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "View.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "AboutDlg.h"
#include "usermsgs.h"

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

BOOL CView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

int CView::AppendTextIrc(LPCTSTR lpstrText)
{
	const TCHAR* p = lpstrText;
	const TCHAR* pEnd = p;
	int fore, back;
	bool setfore = false, setback = false;

	while (*p) {
		if (*p == 0x3) {
			if (p > pEnd) {
				CString s(pEnd, p - pEnd);
				AppendText(s);
			}
			p++;
			if (*p >= '0' && *p <= '9') {
				fore = (*p - '0') * 10;
				setfore = true;
				p++;
				if (*p >= '0' && *p <= '9') {
					fore += (*p - '0');
					pEnd = ++p;
				}
				else {
					pEnd = p;
					fore /= 10;
				}
				if (*p == ',') {
					++p;
					if (*p >= '0' && *p <= '9') {
						back = (*p - '0') * 10;
						setback = true;
						p++;
						if (*p >= '0' && *p <= '9') {
							back += (*p - '0');
							pEnd = ++p;
						}
						else {
							pEnd = p;
							back /= 10;
						}
					}
				}
				else
					pEnd = p;
			}
			else {
				ResetFormat();
				pEnd = p;
				continue;
			}
			if (setfore) {
				SetTextColor(ColorFromCode(fore));
				setfore = false;
			}
			if (setback) {
				SetTextBkColor(ColorFromCode(back));
				setback = false;
			}
		}
		else if (*p == 0x0f) { // reset
			ResetFormat();
			pEnd = p;
		}
		else if (*p == 0x11) { // monospaced
			SetTextFontName(_T("Courier"));
			pEnd = p;
		}
		else if (*p == 0x1E) {
			SwitchStrike();
			pEnd = p;
		}
		else if (*p == 0x1F) {
			SwitchUnderline();
			pEnd = p;
		}
		else if (*p == 0x1D) {
			SwitchItalic();
			pEnd = p;
		}
		else if (*p == 0x2) {
			SwitchBold();
			pEnd = p;
		}
		if (*(p + 1) == 0 && (p > pEnd))
		{
			CString s(pEnd, p - pEnd + 1);
			AppendText(s);
			ResetFormat();
		}
		p++;
	}
	return 0;
}


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
	bHandled = FALSE;
	return 0;
}
//

void CBottom::CreateClient()
{
	m_client.Create(m_hWnd, rcDefault, 0, WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | ES_NOHIDESEL | ES_WANTRETURN | ES_SUNKEN, WS_EX_CLIENTEDGE,
		IDR_BOTTOM_FRAME);
}

void CBottom::CreateToolBar()
{
	HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_BOTTOM_FRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

	m_tb.Attach(hWndToolBar);
	CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
	AddSimpleReBarBand(hWndToolBar, NULL, TRUE);
	UIAddToolBar(hWndToolBar);
	m_tb.SetButtonInfo(0, TBIF_BYINDEX | TBIF_STATE, 0, TBSTATE_HIDDEN, NULL, 0, 0, 0, 0);
	m_tb.SetButtonInfo(ID_EDIT_BOLD, TBIF_STATE, 0, TBSTATE_ENABLED, NULL, 0, 0, 0, 0);
	m_tb.SetButtonInfo(ID_EDIT_ITALIC, TBIF_STATE , 0, TBSTATE_ENABLED, NULL, 0, 0, 0, 0);
	m_tb.SetButtonInfo(ID_EDIT_UNDERLINE, TBIF_STATE, 0, TBSTATE_ENABLED, NULL, 0, 0, 0, 0);
}

LRESULT CBottom::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CreateToolBar();
	CreateComboFonts();
	CreateColorCombos();
	CreateClient();
	m_hAccel = ::LoadAccelerators(_Module.GetModuleInstance(), MAKEINTRESOURCE(IDR_MAINFRAME));
	m_hWndClient = m_client;
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
	m_cbFonts.Create(m_tb, rc, (LPCTSTR)0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | \
		CBS_DROPDOWNLIST | CBS_SORT | CBS_OWNERDRAWVARIABLE | CBS_HASSTRINGS, 0, IDC_COMBO_FONTS);
	m_cbFonts.SetExtendedFontStyle(FPS_EX_TYPEICON | FPS_EX_SHOWFACE);
	ToolbarAddControl(m_tb, m_cbFonts, 1, 7, TRUE);
	m_cbFonts.Dir();
	int index = m_cbFonts.FindString(0, _T("Arial"));
	if (index >= 0)
		m_cbFonts.SetCurSel(index);
	UpdateLayout(FALSE);
}

void CBottom::CreateColorCombos()
{
	CRect rc;

	rc.bottom = 250;
	m_cbFore.Create(m_tb, rc, 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED, 0, IDC_COMBO_FORE);
	ToolbarAddControl(m_tb, m_cbFore, 12, 3, TRUE);
	for (int i = 0; i <= 15; i++) {
		m_cbFore.AddColor(i, ColorFromCode(i));
	}
	m_cbBack.Create(m_tb, rc, 0, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | CBS_DROPDOWNLIST | CBS_OWNERDRAWFIXED, 0, IDC_COMBO_BACK);
	ToolbarAddControl(m_tb, m_cbBack, 16, 3, TRUE);
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
	return m_frame.SendMessage(WM_ONFONTCHANGE, 0, (LPARAM)(LPCTSTR)font);
}

LRESULT CBottom::OnBold(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static bool checked = false;

	checked = !checked;
	m_client.AppendText(_T("\002"), TRUE);
	return 0;
}
LRESULT CBottom::OnItalic(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static bool checked = false;

	checked = !checked;
	m_client.AppendText(_T("\x1d"), TRUE);
	return 0;
}
LRESULT CBottom::OnUnderline(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	static bool checked = false;

	checked = !checked;
	m_client.AppendText(_T("\x1f"), TRUE);
	return 0;
}
LRESULT CBottom::OnForeSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int fore = m_cbFore.GetCurSel();
	int back = m_cbBack.GetCurSel();
	CString code;

	code.Format(_T("\003%02d,%02d"), fore, back);
	m_client.AppendText(code, TRUE);
	return 0;
}
LRESULT CBottom::OnBackSelChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int fore = m_cbFore.GetCurSel();
	int back = m_cbBack.GetCurSel();
	CString code;

	/*code.Format(_T("\003%02d,%02d"), fore, back);
	m_client.AppendText(code, TRUE);*/
	return 0;
}

/// <summary>
/// 
/// </summary>
/// <param name=""></param>
/// <param name="wParam"></param>
/// <param name=""></param>
/// <param name="bHandled"></param>
/// <returns></returns>

LRESULT CBottomClient::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (wParam == VK_RETURN) {
		CString str;
		GetWindowText(str);
		str += _T("\r\n");
		SetWindowText(_T(""));
		m_frame.SendMessage(WM_MSGFROMBOTTOM, (WPARAM)(LPCTSTR)str);
		bHandled = TRUE;
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
				AppendText(txt);
			}
			bHandled = TRUE;
		}
		else
			bHandled = FALSE;
	}
	else
		bHandled = FALSE;
	if (wParam == '\r' || wParam == '\n') {
		bHandled = TRUE;
	}
	return 0;
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