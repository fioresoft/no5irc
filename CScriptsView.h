#pragma once
#include "stdafx.h"
#include "resource.h"

class CScriptView : \
	public CDialogImpl<CScriptView>,
	public CMessageFilter,
	public CDialogResize<CScriptView>
{
	CPath m_scriptsFolder;
	CComQIPtr<IDispatch> m_spNo5;
public:
	CScriptView(CComPtr<IDispatch> &sp);
	virtual ~CScriptView()
	{
		//
	}

	enum { IDD = IDD_SCRIPTS };

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		return IsDialogMessage(pMsg);
	}

	BEGIN_MSG_MAP(CScriptView)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_HANDLER(IDC_LIST1, LBN_DBLCLK, OnList1DClick)
		COMMAND_HANDLER(IDC_LIST2, LBN_DBLCLK, OnList2DClick)
		COMMAND_ID_HANDLER(IDC_BUTTON1, OnBtRun)
		COMMAND_ID_HANDLER(IDC_BUTTON2, OnBtEdit)
		COMMAND_ID_HANDLER(IDC_BUTTON3, OnBtStopAll)
		COMMAND_ID_HANDLER(IDC_BUTTON4, OnBtStop)
		COMMAND_ID_HANDLER(IDC_BUTTON5, OnBtReload)
		COMMAND_ID_HANDLER(IDC_BUTTON6, OnBtNew)
		COMMAND_ID_HANDLER(IDC_BUTTON7,OnEditor)
		CHAIN_MSG_MAP(CDialogResize<CScriptView>)
	END_MSG_MAP()

	BEGIN_DLGRESIZE_MAP(CScriptView)
		DLGRESIZE_CONTROL(IDC_LIST1, DLSZ_SIZE_X | DLSZ_SIZE_Y)
		DLGRESIZE_CONTROL(IDC_BUTTON1, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BUTTON2, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_STATIC1, DLSZ_SIZE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_LIST2, DLSZ_SIZE_X | DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BUTTON4, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BUTTON3, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BUTTON5, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BUTTON6, DLSZ_MOVE_Y)
		DLGRESIZE_CONTROL(IDC_BUTTON7, DLSZ_MOVE_Y)
	END_DLGRESIZE_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	LRESULT OnList1DClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnList2DClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtRun(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtStopAll(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtReload(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBtNew(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnEditor(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	CListBox m_lb1, m_lb2;
	CButton m_bt1;
	void ReloadScriptList(void);
	BOOL ReadScript(LPCTSTR file, CString& buf);
	void ReleaseRunning(void);	// release running scripts
public:
	CPath m_editor;
};

