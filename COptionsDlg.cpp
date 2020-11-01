#include "stdafx.h"
#include "resource.h"
#include "COptionsDlg.h"

LRESULT CFontOptionsDlgPage::OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
{
	DoDataExchange(DDX_LOAD);
	CWindow wnd = GetDlgItem(IDC_STATIC1);
	RECT r = { 0 };

	m_edit.Create(m_hWnd, r, NULL, WS_CHILD | WS_VISIBLE);
	wnd.GetWindowRect(&r);
	ScreenToClient(&r);
	wnd.ShowWindow(SW_HIDE);
	m_edit.MoveWindow(&r);
	m_edit.ShowWindow(SW_SHOWNORMAL);

	cb.Attach(GetDlgItem(IDC_COMBO1));
	ATLASSERT(cb.IsWindow());
	cb.AddString(_T("user name"));				// 0
	cb.AddString(_T("user text"));				// 1
	cb.AddString(_T("my name"));				// 2
	cb.AddString(_T("my text"));				// 3
	cb.AddString(_T("topic"));					// 4
	cb.AddString(_T("message of the day"));		// 5
	cb.AddString(_T("user enters"));			// 6
	cb.AddString(_T("user leaves"));			// 7
	cb.SetCurSel(0);
	BOOL dummy;
	OnComboOk(0, 0, 0, dummy);
	bHandled = FALSE;
	return 0;
}

LRESULT CFontOptionsDlgPage::OnComboOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int index = cb.GetCurSel();
	CColor fore, back;

	m_edit.SetWindowText(_T(""));
	if (index == 0 || index == 1) {
		m_pfo->GetUserName(fore, back);
		m_edit.SetTextColor(fore);
		m_edit.SetTextBkColor(back);
		m_edit.AppendText(_T("user: "));
		m_pfo->GetUserText(fore, back);
		m_edit.SetTextColor(fore);
		m_edit.SetTextBkColor(back);
		m_edit.AppendText(_T("some text"));
	}
	else if (index == 2 || index == 3)
	{
		m_edit.SetTextColor(m_pfo->MeFore(false,false));
		m_edit.SetTextBkColor(m_pfo->MeBack(false, false));
		m_edit.AppendText(_T("myname: "));
		m_edit.SetTextColor(m_pfo->MyTextFore(false, false));
		m_edit.SetTextBkColor(m_pfo->MyTextBack(false, false));
		m_edit.AppendText(_T("some text"));
	}
	else if (index == 4) {
		CColor fore, back;

		m_pfo->Topic(fore, back, false);
		m_edit.SetTextColor(fore);
		m_edit.SetTextBkColor(back);
		m_edit.AppendText(_T("some topic"));
	}
	else if (index == 5) {
		CColor fore, back;

		m_pfo->motd(fore, back, false);
		m_edit.SetTextColor(fore);
		m_edit.SetTextBkColor(back);
		m_edit.AppendText(_T("some message"));
	}
	else if (index == 6)
	{
		CColor fore;

		fore = m_pfo->UserEnters(false, false);
		m_edit.SetTextColor(fore);
		m_edit.AppendText(_T("user enters."));
	}
	else if (index == 7)
	{
		CColor fore;

		fore = m_pfo->UserLeaves(false, false);
		m_edit.SetTextColor(fore);
		m_edit.AppendText(_T("user leaves."));
	}
	return 0;
}

LRESULT CFontOptionsDlgPage::OnButtonClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CButton bt;
	BOOL bHandled;
	CColorDialog dlg;
	CColor fore, back, fore2, back2;
	int index = cb.GetCurSel();
	CString face, face2;

	bt.Attach(GetDlgItem(wID));
	switch (wID) {
	case IDC_BUTTON1:
		m_pfo->RestoreDefaults();
		OnComboOk(0, 0, NULL, bHandled);
		SetModified();
		break;
	case IDC_BUTTON2:	// fore color
	{
		int res = dlg.DoModal();
		if (res == IDOK) {
			SetModified();
			if (index == 0) {
				m_pfo->GetUserName(fore, back);
				fore = dlg.GetColor();
				m_pfo->SetUserName(fore, back);
			}
			else if (index == 1) {
				m_pfo->GetUserText(fore, back);
				fore = dlg.GetColor();
				m_pfo->SetUserText(fore, back);
			}
			else if (index == 2){
				fore = m_pfo->MeFore(false,false);
				fore = dlg.GetColor();
				m_pfo->MeFore(fore);
			}
			else if (index == 3) {
				fore = m_pfo->MyTextFore(false,false);
				fore = dlg.GetColor();
				m_pfo->MyTextFore(fore);
			}
			else if (index == 4) {
				m_pfo->Topic(fore, back, false);
				fore = dlg.GetColor();
				m_pfo->Topic(fore, back);
			}
			else if (index == 5) {
				m_pfo->motd(fore, back, false);
				fore = dlg.GetColor();
				m_pfo->motd(fore, back);
			}
			else if (index == 6) {
				fore = dlg.GetColor();
				m_pfo->UserEnters(fore);
			}
			else if (index == 7) {
				fore = dlg.GetColor();
				m_pfo->UserLeaves(fore);
			}
		}
		OnComboOk(0, 0, NULL, bHandled);
	}
	break;
	case IDC_BUTTON3: // default fore color
		m_pfo->GetUserName(fore, back);
		if (index == 0) {
			m_pfo->SetUserNameDef();
			m_pfo->GetUserNameDef(fore2, back2);
			m_pfo->SetUserName(fore2, back);
		}
		else if (index == 1) {
			m_pfo->SetUserTextDef();
			m_pfo->GetUserTextDef(fore2, back2);
			m_pfo->SetUserText(fore2, back);
		}
		else if (index == 2) {
			m_pfo->MeFore((m_pfo->MeFore(true,true)));
		}
		else if (index == 3) {
			m_pfo->MyTextFore(m_pfo->MyTextFore(true,true));
		}
		else if (index == 4) {
			m_pfo->Topic(fore, back, false);
			m_pfo->Topic(fore2, back2, true);
			m_pfo->Topic(fore2, back);
		}
		else if (index == 5) {
			m_pfo->motd(fore, back, false);
			m_pfo->motd(fore2, back2, true);
			m_pfo->motd(fore2, back);
		}
		else if (index == 6) {
			fore = m_pfo->UserEnters(true, true);
			m_pfo->UserEnters(fore);
		}
		else if (index == 7) {
			fore = m_pfo->UserLeaves(true, true);
			m_pfo->UserLeaves(fore);
		}
		OnComboOk(0, 0, NULL, bHandled);
		SetModified();
		break;
	case IDC_BUTTON4:// back color
	{
		if (index == 6 || index == 7) {
			MessageBeep(MB_ICONERROR);
			break;
		}
		int res = dlg.DoModal();
		if (res == IDOK) {
			SetModified();
			if (index == 0) {
				m_pfo->GetUserName(fore, back);
				back = dlg.GetColor();
				m_pfo->SetUserName(fore, back);
			}
			else if (index == 1) {
				m_pfo->GetUserText(fore, back);
				back = dlg.GetColor();
				m_pfo->SetUserText(fore, back);
			}
			else if (index == 2) {
				back = dlg.GetColor();
				m_pfo->MeBack(back);
			}
			else if (index == 3) {
				back = dlg.GetColor();
				m_pfo->MyTextBack(back);
			}
			else if (index == 4) {
				m_pfo->Topic(fore, back, false);
				back = dlg.GetColor();
				m_pfo->Topic(fore,back);
			}
			else if (index == 5) {
				m_pfo->motd(fore, back, false);
				back = dlg.GetColor();
				m_pfo->motd(fore, back);
			}

		}
		OnComboOk(0, 0, NULL, bHandled);
	}
	break;
	case IDC_BUTTON5: // default back color
		if (index == 0) {
			m_pfo->GetUserName(fore, back);
			m_pfo->SetUserNameDef();
			m_pfo->GetUserNameDef(fore2, back2);
			m_pfo->SetUserName(fore, back2);
		}
		else if (index == 1) {
			m_pfo->GetUserText(fore, back);
			m_pfo->SetUserTextDef();
			m_pfo->GetUserTextDef(fore2, back2);
			m_pfo->SetUserText(fore, back2);
		}
		else if (index == 2) {
			m_pfo->MeBack(m_pfo->MeBack(true,true));
		}
		else if (index == 3) {
			m_pfo->MyTextBack(m_pfo->MyTextBack(true,true));
		}
		else if (index == 4) {
			m_pfo->Topic(fore, back, false);
			m_pfo->Topic(fore2, back2, true);
			m_pfo->Topic(fore, back2);
		}
		else if (index == 5) {
			m_pfo->motd(fore, back, false);
			m_pfo->motd(fore2, back2, true);
			m_pfo->motd(fore, back2);
		}
		if (index == 6 || index == 7) {
			MessageBeep(MB_ICONERROR);
			break;
		}
		OnComboOk(0, 0, NULL, bHandled);
		SetModified();
		break;
	}
	return 0;
}