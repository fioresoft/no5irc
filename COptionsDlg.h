#pragma once
#include "stdafx.h"
#include "resource.h"

// bool to button state
#ifndef BOOL2BST
#define BOOL2BST(iState,bSet) (iState =( bSet ? BST_CHECKED : BST_UNCHECKED ) )
#endif

#ifndef BST2BOOL
#define BST2BOOL(iState,bSet) ( bSet = ( iState == BST_CHECKED ) )
#endif

class COptionsDlgPage: public CPropertyPageImpl<COptionsDlgPage>,
	public CWinDataExchange<COptionsDlgPage>
{
	typedef CPropertyPageImpl<COptionsDlgPage> _baseClass;
public:
	// DECLARE_WND_CLASS(_T("NO5OptionsDlg"));
	bool m_bAllowCTCP;
	int m_iAllowCTCP;
	CString m_userinfo;
public:
	COptionsDlgPage():_baseClass(_T("CTCP"))
	{
		m_bAllowCTCP = false;
		m_iAllowCTCP = BST_UNCHECKED;
		//BOOL2BST(m_iAllowCTCP,m_bAllowCTCP);
	}

	enum { IDD = IDD_PROPPAGE_SMALL };

	BEGIN_DDX_MAP(COptionsDlgPage)
		DDX_CHECK(IDC_CHECK1, m_iAllowCTCP)
		DDX_TEXT(IDC_EDIT1, m_userinfo)
	END_DDX_MAP()

	BEGIN_MSG_MAP(COptionsDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_CHECK1, OnCheck)
		COMMAND_CODE_HANDLER(EN_CHANGE,OnEditChange)
		//FORWARD_NOTIFICATIONS();
		CHAIN_MSG_MAP(_baseClass)
		//CHAIN_MSG_MAP(CWinDataExchange<COptionsDlgPage>)
	END_MSG_MAP()
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		BOOL2BST(m_iAllowCTCP, m_bAllowCTCP);
		DoDataExchange(DDX_LOAD);
		bHandled = FALSE;
		return 0;
	}

	int OnApply()
	{
		DoDataExchange(DDX_SAVE);
		BST2BOOL(m_iAllowCTCP, m_bAllowCTCP);
		return PSNRET_NOERROR;
	}
	LRESULT OnCheck(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		SetModified();
		return 0;
	}
	LRESULT OnEditChange(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		SetModified();
		return 0;
	}
};


