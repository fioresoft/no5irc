#pragma once
#include "No5IrcObj.h"

void GetScriptEngines(CSimpleArray<CString>& vv);

class ATL_NO_VTABLE CMyScriptSite :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CScriptSiteImpl
{
	CComQIPtr<INo5IrcObj> m_spNo5;
public:
	CMyScriptSite()
	{
		
	}
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	BEGIN_COM_MAP(CMyScriptSite)
		COM_INTERFACE_ENTRY(IActiveScriptSite)
		COM_INTERFACE_ENTRY(IActiveScriptSiteWindow)
	END_COM_MAP()
	HRESULT Init(CComPtr<IDispatch> spNo5,LPCTSTR lang) {
		m_spNo5 = spNo5;
		HRESULT hr = Initiate(lang, GetDesktopWindow());
		if (SUCCEEDED(hr)) {
			hr = AddObject(_T("no5"), TRUE, TRUE);
		}
		ATLASSERT(SUCCEEDED(hr));
		return hr;
	}
	STDMETHOD(LookupNamedItem)(LPCOLESTR pstrName, LPUNKNOWN* ppunkItem)
	{
		CComBSTR name = pstrName;
		HRESULT hr = TYPE_E_ELEMENTNOTFOUND;

		if (name == OLESTR("no5")) {
			ATLASSERT(ppunkItem != NULL);
			hr = m_spNo5->QueryInterface(IID_INo5IrcObj,(void **) ppunkItem);
		}
		return hr;
	}
};

