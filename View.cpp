// View.cpp : implementation of the CView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "View.h"
#include "AboutDlg.h"
#include "usermsgs.h"

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
	const TCHAR* pBeginColor = lpstrText, *pBeginBack;
	const TCHAR* pEnd;
	int fore,back;
	bool setfore = false, setback = false;

	while(*p){
		if (*p == 0x3) {
			pBeginColor = p++;
			if (*p >= '0' && *p <= '9') {
				fore = (*p - '0') * 10;
				setfore = true;
				p++;
				if (*p >= '0' && *p <= '9') {
					fore += (*p - '0');
				}
				else {
					pEnd = --p;
				}
				if (*p == ',') {
					pBeginBack = ++p;
					if (*p >= '0' && *p <= '9') {
						back = (*p - '0') * 10;
						setback = true;
						p++;
						if (*p >= '0' && *p <= '9') {
							back += (*p - '0');
						}
						else {
							pEnd = --p;
						}
					}
				}
			}
			if (setfore) {
				SetTextColor(ColorFromCode(fore));
			}
			if (setback) {
				SetTextBkColor(ColorFromCode(back));
			}
			CString s(pBeginColor, p - pBeginColor);
			AppendText(s);
		}
		else {
			if (p != pBeginColor) {
				CString s(pBeginColor, p - pBeginColor);
				AppendText(s);
			}
			p++;
		}
		
	}
	return 0;
}


LRESULT CBottom::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
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

LRESULT CBottom::OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& bHandled)
{
	if (!GetWindowTextLength()) {
		if (wParam == '/') {
			CString cmd;
			CString txt = '/';
			if (OnEditComplete(cmd)) {
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

BOOL CBottom::OnEditComplete(CString& cmd)
{
	BOOL res = FALSE;

	if (GetFocus() == m_hWnd) {
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
