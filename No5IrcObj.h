// No5IrcObj.h: declaração do CNo5IrcObj

#pragma once
#include "stdafx.h"
#include "..\NO5TL\no5tlbase.h"
#include "resource.h"       // símbolos principais
#include "irc11.h"
#include "IIRCEvents.h"
#include "_INo5IrcObjEvents_CP.h"



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
	public IProvideClassInfo2Impl<&CLSID_No5IrcObj,&DIID__INo5IrcObjEvents>,
	public IDispatchImpl<INo5IrcObj, &IID_INo5IrcObj>,
	public IDispatchImpl<_INo5IrcObjEvents, &__uuidof(_INo5IrcObjEvents)>
{
	IIrc* m_pirc;
	IIRCEvents* m_pircevts;
	CString m_nick;
public:
	CNo5IrcObj()
	{
		m_pirc = NULL;
		m_pircevts = NULL;
	}
	void Init(IIrc* pIrc, IIRCEvents *pEvents,LPCTSTR nick) {
		ITypeLib* ptl = NULL;
		m_pirc = pIrc;
		m_pircevts = pEvents;
		m_nick = nick;
		HRESULT hr = LoadTypeLib(OLESTR("irc11.tlb"), &ptl);
		if (SUCCEEDED(hr)) {
			ptl->Release();
			//hr = UpdateRegistry(FALSE);
		}
		ATLASSERT(SUCCEEDED(hr));
		if (FAILED(hr)) {
			MessageBox(GetDesktopWindow(), GetErrorDesc(hr), _T("NO5 IRC"), MB_ICONERROR);
		}
	}

DECLARE_REGISTRY_RESOURCEID(IDR_NO5IRCOBJ)

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
	STDMETHOD(SendUser)(BSTR user,BSTR name);
	STDMETHOD(ListChannels)();
	STDMETHOD(ListChannelNames)(BSTR channel);
	STDMETHOD(GetTopic)(BSTR channel);
	STDMETHOD(GetMode)(BSTR NameOrChannel);
	STDMETHOD(WhoIs)(BSTR nick);
	STDMETHOD(Who)(BSTR nick);
	STDMETHOD(WhoWas)(BSTR nick);
	STDMETHOD(Output)(BSTR msg);
	STDMETHOD(SetTimer)(LONG id, LONG ms, LONG* pRes);
	STDMETHOD(GetActiveViewName)(BSTR* pRes);
	//STDMETHOD(OnChannelList)(BSTR channel, BSTR users, BSTR topic);
};

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
	STDMETHOD(Name)(BSTR* pRes);
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