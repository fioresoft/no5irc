// No5IrcObj.cpp: implementação do CNo5IrcObj

#include "stdafx.h"
#include "irc11_i.c"
#include "No5IrcObj.h"


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
	// TODO: Adicione seu código de implementação aqui

	return S_OK;
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

STDMETHODIMP CNo5IrcObj::GetActiveViewName(BSTR* pRes)
{
	HRESULT hr = E_POINTER;

	if (pRes) {
		CString res = m_pirc->GetActiveViewName();
		CComBSTR bstr = res;
		hr = bstr.CopyTo(pRes);
	}
	return hr;
}

//STDMETHODIMP CNo5IrcObj::OnChannelList(BSTR channel, BSTR users, BSTR topic)
//{
//	// TODO: Adicione seu código de implementação aqui
//
//	return S_OK;
//}

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

STDMETHODIMP CUser::Name(BSTR* pRes)
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
			if (SUCCEEDED(hr)) {
				hr = pObj->Init(&arr[0], &arr[count], 0, AtlFlagTakeOwnership);
				if (SUCCEEDED(hr)) {
					hr = pObj->QueryInterface(pVal);
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