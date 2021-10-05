// No5IrcObj.cpp: implementação do CNo5IrcObj

#include "stdafx.h"
#include "resource.h"
#include "CMyScriptSite.h"
#include "No5IrcObj.h"
#include "MainFrm.h"




extern CMainFrame wndMain;


// CNo5IrcObj

STDMETHODIMP CNo5IrcObj::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] = 
	{
		&IID_INo5IrcObj
	};

	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


STDMETHODIMP CNo5IrcObj::SendActiveChannelMsg(BSTR msg)
{
	CComBSTR view;
	HRESULT hr = get_ActiveViewName(&view);
	if (SUCCEEDED(hr) && view[0] == '#') {
		hr = SendChannelMsg(view, msg);
	}

	return hr;
}


STDMETHODIMP CNo5IrcObj::SendChannelMsg(BSTR channel, BSTR msg)
{
	if (m_pirc) {
		m_pirc->SendChannelMsg(COLE2CT(channel), COLE2CT(msg));
		m_pircevts->OnChannelMsg(COLE2CT(channel), m_nick, COLE2CT(msg), false);
	}
	return S_OK;
}


STDMETHODIMP CNo5IrcObj::SendPrivateMsg(BSTR to, BSTR msg)
{
	m_pirc->SendPrivateMsg(COLE2CT(to), COLE2CT(msg));
	m_pircevts->OnPrivateMsg(m_nick, COLE2CT(msg),false);
	return S_OK;
}

STDMETHODIMP CNo5IrcObj::SendNoticeMsg(BSTR to, BSTR msg)
{
	m_pirc->SendNoticeMsg(COLE2CT(to), COLE2CT(msg));
	m_pircevts->OnNotice(m_nick, COLE2CT(msg), false);
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::JoinChannel(BSTR channel)
{
	m_pirc->JoinChannel(COLE2CT(channel));
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::LeaveChannel(BSTR channel, BSTR msg)
{
	m_pirc->LeaveChannel(COLE2CT(channel), COLE2CT(msg));
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::Quit(BSTR msg)
{
	m_pirc->Quit(COLE2CT(msg));
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::SendPass(BSTR pw)
{
	m_pirc->SendPass(COLE2CT(pw));
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::SendNick(BSTR nick)
{
	m_pirc->SendNick(COLE2CT(nick));
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::SendUser(BSTR user, BSTR name)
{
	m_pirc->SendUser(COLE2CT(user),OLE2CT(name));
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::ListChannels()
{
	m_pirc->ListChannels();
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::ListChannelNames(BSTR channel)
{
	m_pirc->ListChannelNames(COLE2CT(channel));
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::GetTopic(BSTR channel)
{
	m_pirc->GetTopic(COLE2CT(channel));
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::GetMode(BSTR NameOrChannel)
{
	m_pirc->GetMode(COLE2CT(NameOrChannel));
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::WhoIs(BSTR nick)
{
	m_pirc->WhoIs(COLE2CT(nick));
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::Who(BSTR nick)
{
	m_pirc->Who(COLE2CT(nick));
	return S_OK;
}
STDMETHODIMP CNo5IrcObj::WhoWas(BSTR nick)
{
	m_pirc->WhoWas(COLE2CT(nick));
	return S_OK;
}

STDMETHODIMP CNo5IrcObj::Output(BSTR msg)
{
	m_pirc->Output(COLE2CT(msg));
	return S_OK;
}

STDMETHODIMP CNo5IrcObj::SetTimer(LONG id, LONG ms, LONG* pRes)
{
	HRESULT hr = E_POINTER;

	if (pRes) {
		*pRes = m_pirc->SetTimer(id, ms);
		hr = S_OK;
	}
	return hr;
}

STDMETHODIMP CNo5IrcObj::get_ActiveViewName(BSTR* pRes)
{
	HRESULT hr = E_POINTER;

	if (pRes) {
		CString res = m_pirc->GetActiveViewName();
		CComBSTR bstr = res;
		hr = bstr.CopyTo(pRes);
	}
	return hr;
}

STDMETHODIMP CNo5IrcObj::get_Marquee(IDispatch** ppDisp)
{
	CComObject<CMarquee>* pObj = NULL;
	HRESULT hr = CComObject<CMarquee>::CreateInstance(&pObj);
	
	m_iMarquee++;
	if (m_iMarquee > 1)
		return S_FALSE;
	if (SUCCEEDED(hr)) {
		pObj->Init(wndMain.mo[1]);
		hr = pObj->QueryInterface(ppDisp);
	}
	else
		hr = S_FALSE;
	return hr;
}

STDMETHODIMP CNo5IrcObj::DestroyMarquee(IDispatch* pDisp)
{
	CComQIPtr<IMyMarquee> sp = pDisp;
	sp->Destroy();
	m_iMarquee = 0;
	return S_OK;
}

STDMETHODIMP CNo5IrcObj::get_Views(IDispatch **ppDisp)
{
	CSimpleArray<ViewData*> datas;
	wndMain.GetActiveViews(datas);
	CComObject<CViews>* pObj = NULL;
	HRESULT hr = CComObject<CViews>::CreateInstance(&pObj);
	CSimpleArray<CComPtr<IView>> views;


	for (int i = 0; i < datas.GetSize() && SUCCEEDED(hr); i++) {
		CComObject<CChannel>* p = NULL;
		hr = CComObject<CChannel>::CreateInstance(&p);
		ATLASSERT(SUCCEEDED(hr));
		p->Init(CT2COLE(datas[i]->name));
		CComPtr<IView> spView;
		p->QueryInterface(&spView);
		views.Add(spView);
	}
	if (SUCCEEDED(hr) && datas.GetSize()) {
		pObj->Init(views);
		hr = pObj->QueryInterface(ppDisp);
	}
	else
		hr = S_FALSE;
	return hr;
}



// CUser
STDMETHODIMP CUser::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* const arr[] =
	{
		&IID_IUser
	};

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i], riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CUser::get_Name(BSTR* pRes)
{
	HRESULT hr = E_POINTER;

	if (pRes) {
		hr = m_name.CopyTo(pRes);
	}
	return hr;
}


// CUsers
STDMETHODIMP CUsers::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] =
	{
		&IID_IUsers,
	};

	for (int i = 0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i], riid))
			return S_OK;
	}
	return S_FALSE;
}

STDMETHODIMP CUsers::get_Count(long* pVal)
{
	HRESULT hr = E_POINTER;

	if (pVal) {
		*pVal = (long)m_users.GetSize();
		hr = S_OK;
	}
	return hr;
}

STDMETHODIMP CUsers::get_Item(long n, IDispatch** pVal)
{
	HRESULT hr = E_POINTER;

	if (pVal) {
		*pVal = NULL;
		if (n >= 0 && n < m_users.GetSize()) {
			CComObject<CUser>* pObj = NULL;
			hr = CComObject<CUser>::CreateInstance(&pObj);
			if (SUCCEEDED(hr)) {
				hr = pObj->QueryInterface(pVal);
				if (SUCCEEDED(hr))
					pObj->Init(m_users[n]);
			}
			if (FAILED(hr))
				delete pObj;
		}
		else
			hr = E_INVALIDARG;
	}
	return hr;
}


STDMETHODIMP CUsers::get__NewEnum(IUnknown** pVal)
{
	HRESULT hr = E_POINTER;

	if (pVal) {
		*pVal = NULL;
		int count = m_users.GetSize();
		VARIANT* arr = new VARIANT[count];
		CComVariant var;
		CComPtr<IDispatch> spDisp;

		if(!count)
			return S_OK;

		ZeroMemory(arr, count * sizeof(VARIANT));
		for (int i = 0; i < count; i++) {
			CComObject<CUser>* pObj = NULL;
			hr = CComObject<CUser>::CreateInstance(&pObj);
			if (SUCCEEDED(hr)) {
				spDisp.Release();
				hr = pObj->QueryInterface(IID_IDispatch,(void **)&spDisp);
				if (SUCCEEDED(hr)) {
					pObj->Init(m_users[i]);
					var = (IDispatch*)spDisp;
					arr[i].vt = VT_EMPTY;
					hr = var.Detach(&arr[i]);
					//spDisp.Release();
				}
			}
			if (FAILED(hr)) {
				delete pObj;
				pObj = NULL;
				break;
			}
		}
		spDisp.Release(); // not really needed
		if (SUCCEEDED(hr)) {
			typedef CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>,
				CComSingleThreadModel> EnumVar;
			CComObject<EnumVar>* pObj = NULL;
			hr = CComObject<EnumVar>::CreateInstance(&pObj);
			ATLASSERT(SUCCEEDED(hr));
			if (SUCCEEDED(hr)) {
				hr = pObj->Init(&arr[0], &arr[count], 0, AtlFlagCopy);
				ATLASSERT(SUCCEEDED(hr));
				if (SUCCEEDED(hr)) {
					hr = pObj->QueryInterface(pVal);
					ATLASSERT(SUCCEEDED(hr));
				}
			}
		}
		if (FAILED(hr)) {
			if (arr) {	// wright ? todo
				CComVariant v;
				for (int i = 0; i < count; i++) {
					v = arr[i];
					v.Clear();
				}
				delete[]arr;
			}
		}
	}
	return hr;
}

// CChannel 

STDMETHODIMP CChannel::get_Users(IDispatch** ppDisp)
{
	HRESULT hr = E_POINTER;
	USES_CONVERSION;

	if (ppDisp) {
		CComObject<CUsers>* pObj = NULL;
		hr = CComObject<CUsers>::CreateInstance(&pObj);
		if (SUCCEEDED(hr)) {
			CSimpleArray<CComBSTR> names;
			CString name = m_name;
			wndMain.SendMessage(WM_GETUSERS, WPARAM(LPCTSTR(name)), (LPARAM)&names);
			if (names.GetSize()) {
				pObj->Init(names);
				hr = pObj->QueryInterface(ppDisp);
			}
			else
				hr = S_FALSE;
		}
	}
	return hr;
}

// CViews

STDMETHODIMP CViews::get_Item(long index,IDispatch ** ppDisp)
{
	HRESULT hr = E_POINTER;

	if (ppDisp) {
		ATLASSERT(*ppDisp == NULL);
		IDispatch* p = NULL;
		IUnknown* pUnk = NULL;
		CComQIPtr<IDispatch> spDisp;
		CComQIPtr<IUsers> spUsers;
		CComObject<CChannel>* pObj = NULL;
		hr = CComObject<CChannel>::CreateInstance(&pObj);
		ATLASSERT(SUCCEEDED(hr));
		CComBSTR name;
		hr = m_aViews[index]->get_Name(&name);
		pObj->Init(name);
		hr = pObj->QueryInterface(ppDisp);
		ATLASSERT(SUCCEEDED(hr));
	}
	ATLASSERT(SUCCEEDED(hr));
	return hr;
}

STDMETHODIMP CViews::get__NewEnum(IUnknown** ppUnk)
{
	HRESULT hr = E_POINTER;

	if (ppUnk) {
		*ppUnk = NULL;
		int count = m_aViews.GetSize();
		VARIANT* arr = new VARIANT[count];
		CComVariant var;
		CComPtr<IDispatch> spDisp;

		if (!count)
			return S_OK;

		ZeroMemory(arr, count * sizeof(VARIANT));
		for (int i = 0; i < count; i++) {
			CComObject<CChannel>* pObj = NULL;
			hr = CComObject<CChannel>::CreateInstance(&pObj);
			if (SUCCEEDED(hr)) {
				spDisp.Release();
				hr = pObj->QueryInterface(IID_IDispatch, (void**)&spDisp);
				if (SUCCEEDED(hr)) {
					CComBSTR name;
					CComQIPtr<IUsers> spUsers;
					//CSimpleArray<CComBSTR> users;
					CComPtr<IDispatch> spDispUsers;
					CComPtr<IDispatch> spDispUser;
					long count2 = 0;
					m_aViews[i]->get_Name(&name);
					m_aViews[i]->get_Users(&spDispUsers);
					spUsers = spDispUsers;
					spUsers->get_Count(&count2);
					for (int i = 0; i < count2 && SUCCEEDED(hr); i++) {
						spDispUser.Release();
						hr = spUsers->get_Item(i, &spDispUser);
						if (SUCCEEDED(hr)) {
							CComQIPtr<IUser> spUser = spDispUser;
							CComBSTR name2;
							hr = spUser->get_Name(&name2);
							if (SUCCEEDED(hr)) {
								//users.Add(name2);
							}
						}
					}
					pObj->Init(name);
					var = (IDispatch*)spDisp;
					arr[i].vt = VT_EMPTY;
					hr = var.Detach(&arr[i]);
					spDisp.Release();
				}
			}
			if (FAILED(hr)) {
				delete pObj;
				pObj = NULL;
				break;
			}
		}
		//spDisp.Release(); // not really needed
		if (SUCCEEDED(hr)) {
			CComObject<EnumVar>* pObj2 = NULL;
			hr = CComObject<EnumVar>::CreateInstance(&pObj2);
			if (SUCCEEDED(hr)) {
				hr = pObj2->Init(&arr[0], &arr[count], 0, AtlFlagCopy);
				if (SUCCEEDED(hr)) {
					hr = pObj2->QueryInterface(ppUnk);
				}
			}
		}
		if (FAILED(hr)) {
			if (arr) {	// wright ? todo
				CComVariant v;
				for (int i = 0; i < count; i++) {
					v = arr[i];
					v.Clear();
				}
				delete[]arr;
			}
		}
	}
	ATLASSERT(SUCCEEDED(hr));
	return hr;
}

STDMETHODIMP CViews::get_Count(long * pCount)
{
	*pCount = m_aViews.GetSize();
	return S_OK;
}

// CMarquee

STDMETHODIMP CMarquee::AddItem(BSTR bstr, OLE_COLOR clr)
{
	HRESULT hr = S_OK;
	CComBSTR s = bstr;

	/*if(wndMain.m_marquee[1].IsWindow())
		wndMain.m_marquee[1].AddItem((LPCTSTR)s,clr);*/
	wndMain.m_spMarquee[1]->AddItem(bstr, clr);
	return hr;
}

STDMETHODIMP CMarquee::SetFontColor(OLE_COLOR color)
{
	/*if (wndMain.m_marquee[1].IsWindow())
		wndMain.m_marquee[1].SetTextColor(color);*/
	
	m_mo.fore = color;
	return S_OK;
}

STDMETHODIMP CMarquee::SetBkColor(OLE_COLOR color)
{
	/*if (wndMain.m_marquee[1].IsWindow())
		wndMain.m_marquee[1].SetBackColor(color);*/
	wndMain.m_spMarquee[1]->put_BackColor(color);
	m_mo.back = color;
	return S_OK;
}

STDMETHODIMP CMarquee::Start()
{
	/*if (wndMain.m_marquee[1].IsWindow())
		wndMain.m_marquee[1].Start();*/
	wndMain.m_spMarquee[1]->Start();
	return S_OK;
}

STDMETHODIMP CMarquee::Stop()
{
	/*if (wndMain.m_marquee[1].IsWindow())
		wndMain.m_marquee[1].Stop();*/
	wndMain.m_spMarquee[1]->Stop();
	return S_OK;
}

STDMETHODIMP CMarquee::Destroy()
{
	CReBarCtrl rebar = wndMain.GetToolbar();
	int nBandIndex = rebar.IdToIndex(IDC_BAND_MARQUEE + 1);	// toolbar is 2nd added band
	rebar.ShowBand(nBandIndex, FALSE);
	//UISetCheck(ID_VIEW_MARQUEE, bVisible);
	//m_mw.DestroyWindow();
	/*wndMain.m_marquee[1].DestroyWindow();
	wndMain.m_marquee[1].Detach();*/
	wndMain.m_spMarquee[1]->get_BackColor(&wndMain.mo[1].back);
	wndMain.m_spMarquee[1]->get_Loop(&wndMain.mo[1].bLoop);
	wndMain.m_spMarquee[1]->get_Elapse((long *)&wndMain.mo[1].Elapse);
	wndMain.m_spMarquee[1].Release();
	wndMain.UpdateLayout();
	return S_OK;
}

void CMarquee::Init(MarqueeOptions& mo)
{
	m_mo = mo;
	
	wndMain.CreateMarquee(1);
	//wndMain.AddSimpleReBarBandCtrl(wndMain.GetToolbar(),wndMain.m_marquee[1], IDC_BAND_MARQUEE + 1,
	//	NULL,	// title
	//	TRUE,	// new row
	//	0,		// cx
	//	TRUE);	// full width always
}