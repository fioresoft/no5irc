// aboutdlg.cpp : implementation of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "aboutdlg.h"

LRESULT CAboutDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	CWindow wnd = GetDlgItem(IDHELP);
	NO5TL::CVersionInfo vi;

	if (vi.Init(PRODUCT_NAME)) {
		CString s, tmp;
		const int cp = 0x40904b0;
		if (vi.GetString(cp, _T("ProductName"), tmp))
			s += tmp;
		if (vi.GetString(cp, _T("CompanyName"), tmp)) {
			s += "\r\n";
			s += tmp;
		}
		if (vi.GetString(cp, _T("ProductVersion"), tmp)) {
			s += "\r\n";
			s += tmp;
		}
		wnd.SetWindowText(s);
	}
	m_hlWebPage.SubclassWindow(GetDlgItem(IDC_STATIC1));
	m_hlWebPage.SetHyperLink(_T("https://fioresoft.net"));
	m_hlWebPage.SetLabel(_T("https://fioresoft.net"));
	return TRUE;
}

LRESULT CAboutDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
