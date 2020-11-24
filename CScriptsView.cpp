#include "stdafx.h"
#include "CMyScriptSite.h"
#include "CScriptsView.h"

CScriptView::CScriptView(CComPtr<IDispatch> &spNo5)
{
	m_scriptsFolder.SetPath(PATH_MODULE);
	m_scriptsFolder = m_scriptsFolder.GetLocation();
	CString path = m_scriptsFolder;
	path += _T("scripts\\");
	m_scriptsFolder = path;
	m_spNo5 = spNo5;
	m_editor = _T("notepad.exe");
}

LRESULT CScriptView::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	//pLoop->AddIdleHandler(this);
	//UIAddChildWindowContainer(m_hWnd);

	m_lb1.Attach(GetDlgItem(IDC_LIST1));
	m_lb2.Attach(GetDlgItem(IDC_LIST2));
	m_bt1.Attach(GetDlgItem(IDC_BUTTON1));
	DlgResize_Init(false, false, WS_CLIPCHILDREN);
	ReloadScriptList();
	return TRUE;
}

LRESULT CScriptView::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	ReleaseRunning();
	return 0;
}

LRESULT CScriptView::OnList1DClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int idx = m_lb1.GetCurSel();
	if (idx >= 0) {
		CString sFile;
		CSimpleArray<CString> languages;

		GetScriptEngines(languages);

		if (m_lb1.GetText(idx, sFile) > 0) {
			CPath path = sFile;
			CComObject<CMyScriptSite>* pObj = NULL;
			HRESULT hr = CComObject<CMyScriptSite>::CreateInstance(&pObj);
			
			CString ext = path.GetExtension();
			CString lang = _T("vbscript"); // default

			if (!ext.CompareNoCase(_T(".vbs"))) {
				lang = _T("vbscript");
			}
			else if (!ext.CompareNoCase(_T(".js"))) {
				lang = _T("javascript");
			}
			else if (!ext.CompareNoCase(_T(".php"))) {
				if (languages.Find(CString(_T("php")))) {
					lang = _T("php");
				}
			}
			/*CString s = "supported languages:\n";
			for (int i = 0; i < languages.GetSize(); i++) {
				s += languages[i];
				s += '\n';
			}
			MessageBox(s);*/
			if (SUCCEEDED(hr)) {
				CComQIPtr<IActiveScriptSite> sp;
				hr = pObj->QueryInterface(&sp);
				if (SUCCEEDED(hr)) {
					sp.Detach();
					hr = pObj->Init(m_spNo5,lang);
					if (SUCCEEDED(hr)) {
						CString buf;
						BOOL res = ReadScript(sFile, buf);
						if (res) {
							hr = pObj->AddScript(buf);
							ATLASSERT(SUCCEEDED(hr));
							if (SUCCEEDED(hr)) {
								hr = pObj->Run();
								ATLASSERT(SUCCEEDED(hr));
							}
						}
						else
							hr = E_FAIL;
					}
				}
			}
			if (FAILED(hr) && pObj != NULL) {
				CString msg;
				msg.Format(_T("Error %08x\n%s"), hr, (LPCTSTR)(NO5TL::GetErrorDesc(hr)));
				MessageBox(msg);
				pObj->Release();
				delete pObj;
			}
			else {
				// dont delete we may want to run it more than once
				// m_lb1.DeleteString(idx);
				idx = m_lb2.AddString(sFile);
				if (idx >= 0)
					m_lb2.SetItemDataPtr(idx, (void*)pObj);
				//g_app.IncScript();
			}
		}
	}
	return 0;
}

LRESULT CScriptView::OnList2DClick(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int idx = m_lb2.GetCurSel();
	if (idx >= 0) {
		CString sFile;

		if (m_lb2.GetText(idx, sFile) > 0) {
			CComObject<CMyScriptSite>* pObj = (CComObject<CMyScriptSite>*)m_lb2.GetItemDataPtr(idx);

			if (pObj) {
				HRESULT hr = pObj->Terminate();
				if (SUCCEEDED(hr)) {
					ULONG x = pObj->Release();
					m_lb2.SetItemData(idx, 0);
					m_lb2.DeleteString(idx);
					//idx = m_lb1.AddString(sFile);
					//g_app.DecScript();
				}
			}
		}
	}
	return 0;
}


LRESULT CScriptView::OnBtRun(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL b;
	OnList1DClick(0, 0, 0, b);
	return 0;
}

// edit ( only not running scripts )
LRESULT CScriptView::OnBtEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int idx = m_lb1.GetCurSel();
	if (idx >= 0) {
		CString sFile;
		CPath path = m_scriptsFolder + sFile;

		if (m_lb1.GetText(idx, sFile) > 0) {
			ShellExecute(m_hWnd, _T("open"), m_editor,_T(" ") + sFile, m_scriptsFolder, SHOW_OPENWINDOW);
		}
	}
	return 0;
}

// new 
LRESULT CScriptView::OnBtNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShellExecute(m_hWnd, _T("open"), m_editor, NULL, m_scriptsFolder, SHOW_OPENWINDOW);
	return 0;
}

// stop all
LRESULT CScriptView::OnBtStopAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int count = m_lb2.GetCount();
	BOOL b;

	while (count > 0) {
		m_lb2.SetCurSel(0);
		OnList2DClick(0, 0, 0, b);
		count = m_lb2.GetCount();
	}

	return 0;
}

LRESULT CScriptView::OnBtStop(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	BOOL b;
	OnList2DClick(0, 0, 0, b);
	return 0;
}

LRESULT CScriptView::OnBtReload(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ReloadScriptList();
	return 0;
}

LRESULT CScriptView::OnEditor(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CString save = m_editor;
	m_editor.SetPath(PATH_MODULE);
	CString file = _T("notepad.exe");
	
	CFileDialog dlg(TRUE, _T("exe"), file, OFN_NOCHANGEDIR | OFN_NONETWORKBUTTON | OFN_EXPLORER,
		_T("exe files\0*.exe\0\0\0\0"),::GetParent(m_hWnd));
	if (IDOK == dlg.DoModal()) {
		m_editor = dlg.m_szFileName;
	}
	else {
		MessageBox(GetErrorDesc(GetLastError()));
		m_editor = save;
	}
		
		
	return 0;

}


void CScriptView::ReloadScriptList(void)
{
	WIN32_FIND_DATA f = { 0 };
	HANDLE hFind = NULL;
	CString sSearch;
	LPCTSTR exts[] = { _T("*.vbs"),_T("*.js") };
	const int count = sizeof(exts) / sizeof(exts[0]);

	m_lb1.ResetContent();
	for (int i = 0; i < count; i++) {
		sSearch.Empty();
		sSearch += m_scriptsFolder;
		sSearch += exts[i];
		hFind = ::FindFirstFile(sSearch, &f);
		if (hFind != INVALID_HANDLE_VALUE) {
			if ((f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
				m_lb1.AddString(f.cFileName);
				while (FindNextFile(hFind, &f)) {
					if ((f.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) {
						m_lb1.AddString(f.cFileName);
					}
				}
			}
			FindClose(hFind);
		}
	}
}

BOOL CScriptView::ReadScript(LPCTSTR file, CString& buf)
{
	CPath path = m_scriptsFolder;
	CWinFile wf;
	CString s = path.GetLocation();
	s += file;
	path = s;

	BOOL res = wf.Create(path, OPEN_EXISTING, GENERIC_READ);

	if (res) {
		CDataBuffer<char> db;

		res = wf.ReadAll<char>(db,0);
		if (res) {
			db.AddNull();
			buf = (LPCSTR)db.GetData();
		}
	}
	return res;
}

void CScriptView::ReleaseRunning(void)
{
	// release all objects in the running objects list
	const int count = m_lb2.GetCount();
	int i;
	for (i = 0; i < count; i++) {
		CComObject<CMyScriptSite>* pObj = (CComObject<CMyScriptSite>*)m_lb2.GetItemDataPtr(i);

		if (pObj) {
			HRESULT hr = pObj->Terminate();
			pObj->Release();
			m_lb2.SetItemData(i, 0);
		}
	}
}


