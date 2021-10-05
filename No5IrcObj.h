// No5IrcObj.h: declaração do CNo5IrcObj

#pragma once
#include "stdafx.h"
#include "resource.h"       // símbolos principais
#include "..\NO5TL\no5tlbase.h"
#include "irc12.h"
#include "IIRCEvents.h"
#include "_INo5IrcObjEvents_CP.h"
#include "usermsgs.h"
#include "MarqueeOptions.h"




#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Objetos COM de única thread não são apropriadamente suportados na plataforma Windows CE, como a plataforma Windows Mobile que não possui suporte DCOM completo. Defina _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA para forçar o ATL a suportar a criação de objetos de única thread e para permitir o uso de suas implementações de objetos COM de única thread. Seu modelo de threading nos arquivos rgs foram definidos com 'Free' uma vez que esse é o único modelo de thread suportado em plataformas Windows CE não DCOM."
#endif
#include "IIrc.h"
//#include "IIRCEvents.h"
using namespace ATL;


// CNo5IrcObj

class ATL_NO_VTABLE CNo5IrcObj :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CProxy_INo5IrcObjEvents<CNo5IrcObj>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<CNo5IrcObj>,
	public IProvideClassInfo2Impl<&CLSID_No5IrcObj, &DIID__INo5IrcObjEvents>,
	public IDispatchImpl<INo5IrcObj, &IID_INo5IrcObj>,
	public IDispatchImpl<_INo5IrcObjEvents, &__uuidof(_INo5IrcObjEvents)>
{
	IIrc* m_pirc;
	IIRCEvents* m_pircevts;
	CString m_nick;
	int m_iMarquee;
public:
	CNo5IrcObj()
	{
		m_iMarquee = 0;
		m_pirc = NULL;
		m_pircevts = NULL;
	}
	void Init(IIrc* pIrc, IIRCEvents* pEvents, LPCTSTR nick) {
		ITypeLib* ptl = NULL;
		m_pirc = pIrc;
		m_pircevts = pEvents;
		m_nick = nick;
		HRESULT hr = LoadTypeLib(OLESTR("irc12.tlb"), &ptl);
		if (SUCCEEDED(hr)) {
			ptl->Release();
			//hr = UpdateRegistry(FALSE);
		}
		ATLASSERT(SUCCEEDED(hr));
		if (FAILED(hr)) {
			MessageBox(GetDesktopWindow(), GetErrorDesc(hr), _T("NO5 IRC"), MB_ICONERROR);
		}
	}

	//DECLARE_REGISTRY_RESOURCEID(IDR_NO5IRCOBJ)
	DECLARE_REGISTRY_RESOURCEID(IDR_REGISTRY1)

	DECLARE_NOT_AGGREGATABLE(CNo5IrcObj)

	BEGIN_COM_MAP(CNo5IrcObj)
		COM_INTERFACE_ENTRY(INo5IrcObj)
		COM_INTERFACE_ENTRY2(IDispatch, INo5IrcObj)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
		COM_INTERFACE_ENTRY_IMPL(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo2)
		//COM_INTERFACE_ENTRY(IConnectionPointContainer)
		COM_INTERFACE_ENTRY(_INo5IrcObjEvents)
	END_COM_MAP()

	BEGIN_CONNECTION_POINT_MAP(CNo5IrcObj)
		CONNECTION_POINT_ENTRY(__uuidof(_INo5IrcObjEvents))
	END_CONNECTION_POINT_MAP()
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(SendActiveChannelMsg)(BSTR msg);
	STDMETHOD(SendChannelMsg)(BSTR channel, BSTR msg);
	STDMETHOD(SendPrivateMsg)(BSTR to, BSTR msg);
	STDMETHOD(SendNoticeMsg)(BSTR to, BSTR msg);
	STDMETHOD(JoinChannel)(BSTR channel);
	STDMETHOD(LeaveChannel)(BSTR channel, BSTR msg);
	STDMETHOD(Quit)(BSTR msg);
	STDMETHOD(SendPass)(BSTR pw);
	STDMETHOD(SendNick)(BSTR nick);
	STDMETHOD(SendUser)(BSTR user, BSTR name);
	STDMETHOD(ListChannels)();
	STDMETHOD(ListChannelNames)(BSTR channel);
	STDMETHOD(GetTopic)(BSTR channel);
	STDMETHOD(GetMode)(BSTR NameOrChannel);
	STDMETHOD(WhoIs)(BSTR nick);
	STDMETHOD(Who)(BSTR nick);
	STDMETHOD(WhoWas)(BSTR nick);
	STDMETHOD(Output)(BSTR msg);
	STDMETHOD(SetTimer)(LONG id, LONG ms, LONG* pRes);
	STDMETHOD(get_Marquee)(IDispatch** pRes);
	STDMETHOD(DestroyMarquee)(IDispatch* pRes);
	STDMETHOD(get_ActiveViewName)(BSTR* pRes);
	STDMETHOD(get_Views)(IDispatch** ppDisp);
};
//OBJECT_ENTRY_NON_CREATEABLE_EX_AUTO(CLSID_No5IrcObj, CNo5IrcObj);
//OBJECT_ENTRY_AUTO(CLSID_No5IrcObj, CNo5IrcObj);
//(CLSID_No5IrcObj, CNo5IrcObj);


class ATL_NO_VTABLE CUser :
	public CComObjectRootEx<CComSingleThreadModel>,
	public ISupportErrorInfo,
	public IDispatchImpl<IUser, &IID_IUser>
{
	CComBSTR m_name;
public:
	CUser()
	{

	}
	virtual ~CUser()
	{

	}
	void Init(LPCTSTR name) {
		m_name = name;
	}

	DECLARE_NOT_AGGREGATABLE(CUser)

	BEGIN_COM_MAP(CUser)
		//COM_INTERFACE_ENTRY(IUnknown)
		COM_INTERFACE_ENTRY(IUser)
		COM_INTERFACE_ENTRY2(IDispatch, IUser)
		//COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(get_Name)(BSTR* pRes);
};

class CUsers : \
	public CComObjectRootEx<CComSingleThreadModel>,
	public ISupportErrorInfo,
	public IDispatchImpl<IUsers, &IID_IUsers>
{
	CSimpleArray<CComBSTR> m_users;
public:
	// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);

	DECLARE_NOT_AGGREGATABLE(CUsers)
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	BEGIN_COM_MAP(CUsers)
		//COM_INTERFACE_ENTRY(IUnknown)
		COM_INTERFACE_ENTRY(IUsers)
		COM_INTERFACE_ENTRY2(IDispatch, IUsers)
		//COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(ISupportErrorInfo)
	END_COM_MAP()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	void CUsers::Init(const CSimpleArray<CComBSTR>& users)
	{
		CopySimpleArray(m_users, users);
	}
	STDMETHOD(get_Count)(long* pVal);
	STDMETHOD(get_Item)(long n, IDispatch** pVal);
	STDMETHOD(get__NewEnum)(IUnknown** pVal);
};

class ATL_NO_VTABLE CChannel :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IView, &IID_IView /*wMajor = 1, /*wMinor = 0*/>,
	public IProvideClassInfo2Impl<&CLSID_Channel, &IID_IView>
{
public:
	CChannel()
	{
	}

	//DECLARE_REGISTRY_RESOURCEID(IDR_NO5IRCOBJ)

	DECLARE_NOT_AGGREGATABLE(CChannel)

	BEGIN_COM_MAP(CChannel)
		COM_INTERFACE_ENTRY(IView)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
		COM_INTERFACE_ENTRY(IProvideClassInfo2)
	END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	CComBSTR m_name;
	void Init(LPCOLESTR name)
	{
		m_name = name;
	}
	STDMETHOD(get_Name)(BSTR* pName)
	{
		HRESULT hr = m_name.CopyTo(pName);
		return hr;
	}
	STDMETHOD(get_Users)(IDispatch** ppDisp);
};

typedef CComEnum<IEnumVARIANT, &IID_IEnumVARIANT, VARIANT, _Copy<VARIANT>,
	CComSingleThreadModel> EnumVar;

class ATL_NO_VTABLE CViews :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IViews, &IID_IViews>,
	//public IDispatchImpl< EnumVar,&IID_IEnumVARIANT>,
	public IProvideClassInfo2Impl<&CLSID_Views, &IID_IViews>
{
public:
	CViews()
	{
	}

	//DECLARE_REGISTRY_RESOURCEID(IDR_REGISTRY1)

	DECLARE_NOT_AGGREGATABLE(CViews)

	BEGIN_COM_MAP(CViews)
		COM_INTERFACE_ENTRY(IViews)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IProvideClassInfo2)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	CSimpleArray<CComPtr<IView>> m_aViews;
	void Init(CSimpleArray<CComPtr<IView>> aViews)
	{
		for (int i = 0; i < aViews.GetSize(); i++) {
			m_aViews.Add(aViews[i]);
		}
	}
	STDMETHOD(get_Count)(long* pCount);
	STDMETHOD(get_Item)(long index, IDispatch** ppDisp);
	STDMETHOD(get__NewEnum)(IUnknown** ppUnk);
	
};

class ATL_NO_VTABLE CMarquee :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IMyMarquee,&IID_IMyMarquee>,
	//public IDispatchImpl< EnumVar,&IID_IEnumVARIANT>,
	public IProvideClassInfo2Impl<&CLSID_Marquee, &IID_IMyMarquee>
{
public:
	CMarquee()
	{
	}

	//DECLARE_REGISTRY_RESOURCEID(IDR_NO5IRCOBJ)

	DECLARE_NOT_AGGREGATABLE(CMarquee)

	BEGIN_COM_MAP(CMarquee)
		COM_INTERFACE_ENTRY(IMyMarquee)
		COM_INTERFACE_ENTRY2(IDispatch, IMyMarquee)
		COM_INTERFACE_ENTRY(IProvideClassInfo2)
		COM_INTERFACE_ENTRY(IProvideClassInfo)
	END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
	MarqueeOptions m_mo;
public:
	void Init(MarqueeOptions& mo);
	STDMETHOD(AddItem)(BSTR bstr,OLE_COLOR clr);
	STDMETHOD(SetFontColor)(OLE_COLOR color);
	STDMETHOD(SetBkColor)(OLE_COLOR color);
	STDMETHOD(Destroy)();
	STDMETHOD(Start)();
	STDMETHOD(Stop)();
};
