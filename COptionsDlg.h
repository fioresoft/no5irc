#pragma once
#include "stdafx.h"
#include "resource.h"
#include "IFontOptions.h"

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
	int m_iPingPong;
	bool m_bPingPong;
public:
	COptionsDlgPage():_baseClass(_T("CTCP"))
	{
		m_bAllowCTCP = false;
		m_iAllowCTCP = BST_UNCHECKED;
		m_iPingPong = BST_CHECKED;
		m_bPingPong = true;
		//BOOL2BST(m_iAllowCTCP,m_bAllowCTCP);
	}

	enum { IDD = IDD_PROPPAGE_SMALL };

	BEGIN_DDX_MAP(COptionsDlgPage)
		DDX_CHECK(IDC_CHECK1, m_iAllowCTCP)
		DDX_TEXT(IDC_EDIT1, m_userinfo)
		DDX_CHECK(IDC_CHECK2,m_iPingPong)
	END_DDX_MAP()

	BEGIN_MSG_MAP(COptionsDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_ID_HANDLER(IDC_CHECK1, OnCheck)
		COMMAND_ID_HANDLER(IDC_CHECK2, OnCheck)
		COMMAND_CODE_HANDLER(EN_CHANGE,OnEditChange)
		//FORWARD_NOTIFICATIONS();
		CHAIN_MSG_MAP(_baseClass)
		//CHAIN_MSG_MAP(CWinDataExchange<COptionsDlgPage>)
	END_MSG_MAP()
	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		BOOL2BST(m_iAllowCTCP, m_bAllowCTCP);
		BOOL2BST(m_iPingPong, m_bPingPong);
		DoDataExchange(DDX_LOAD);
		bHandled = FALSE;
		return 0;
	}

	int OnSetActive()
	{
		// 0 = allow activate
		// -1 = go back that was active
		// page ID = jump to page
		//DoDataExchange(DDX_LOAD);
		return 0;
	}
	BOOL OnKillActive()
	{
		// FALSE = allow deactivate
		// TRUE = prevent deactivation
		//DoDataExchange(DDX_SAVE);
		return FALSE;
	}

	int OnApply()
	{
		DoDataExchange(DDX_SAVE);
		BST2BOOL(m_iAllowCTCP, m_bAllowCTCP);
		BST2BOOL(m_iPingPong, m_bPingPong);
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


class CFontOptionsDlgPage : public CPropertyPageImpl<CFontOptionsDlgPage>,
	public CWinDataExchange<CFontOptionsDlgPage>
{
	typedef CPropertyPageImpl<CFontOptionsDlgPage> _baseClass;
	IFontOptions* m_pfo;
	CComboBox cb;
	CNo5RichEdit m_edit;
	IFontOptions* m_pFO;
public:
	//DECLARE_WND_CLASS(_T("NO5FontOptionsDlg"));
public:
	CFontOptionsDlgPage(IFontOptions *pfo):m_pFO(pfo),_baseClass(_T("font options"))
	{
		m_pfo = NULL;
		CreateFontOptions(&m_pfo);
		m_pFO->CopyTo(&m_pfo);
	}
	virtual ~CFontOptionsDlgPage()
	{
		DestroyFontOptions(&m_pfo);
	}

	enum { IDD = IDD_PROPPAGE_SMALL1 };

	BEGIN_MSG_MAP(CFontOptionsDlgPage)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		COMMAND_CODE_HANDLER(BN_CLICKED,OnButtonClicked)
		COMMAND_HANDLER(IDC_COMBO1,CBN_SELENDOK,OnComboOk)
		CHAIN_MSG_MAP(_baseClass)
	END_MSG_MAP()

	BEGIN_DDX_MAP(CFontOptionsDlgPage)
	END_DDX_MAP()

	LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL& bHandled);

	int OnSetActive()
	{
		// 0 = allow activate
		// -1 = go back that was active
		// page ID = jump to page
		//DoDataExchange(DDX_LOAD);
		return 0;
	}
	BOOL OnKillActive()
	{
		// FALSE = allow deactivate
		// TRUE = prevent deactivation
		//DoDataExchange(DDX_SAVE);
		return FALSE;
	}

	int OnApply()
	{
		DoDataExchange(DDX_SAVE);
		m_pfo->CopyTo(&m_pFO);
		return PSNRET_NOERROR;
	}
	LRESULT OnComboOk(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnButtonClicked(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};


