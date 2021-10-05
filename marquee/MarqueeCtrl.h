// MarqueeCtrl.h: declaração do CMarqueeCtrl
#pragma once
#include "resource.h"       // símbolos principais
#include <atlctl.h>
#include "Marquee_i.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Objetos COM de única thread não são apropriadamente suportados na plataforma Windows CE, como a plataforma Windows Mobile que não possui suporte DCOM completo. Defina _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA para forçar o ATL a suportar a criação de objetos de única thread e para permitir o uso de suas implementações de objetos COM de única thread. Seu modelo de threading nos arquivos rgs foram definidos com 'Free' uma vez que esse é o único modelo de thread suportado em plataformas Windows CE não DCOM."
#endif

using namespace ATL;



// CMarqueeCtrl
class ATL_NO_VTABLE CMarqueeCtrl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CStockPropImpl<CMarqueeCtrl, IMarqueeCtrl>,
	public IPersistStreamInitImpl<CMarqueeCtrl>,
	public IOleControlImpl<CMarqueeCtrl>,
	public IOleObjectImpl<CMarqueeCtrl>,
	public IOleInPlaceActiveObjectImpl<CMarqueeCtrl>,
	public IViewObjectExImpl<CMarqueeCtrl>,
	public IOleInPlaceObjectWindowlessImpl<CMarqueeCtrl>,
	public ISupportErrorInfo,
	public IObjectWithSiteImpl<CMarqueeCtrl>,
	public IServiceProviderImpl<CMarqueeCtrl>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<CMarqueeCtrl>,
#endif
	public IProvideClassInfo2Impl<&CLSID_MarqueeCtrl, nullptr, &LIBID_MarqueeLib>,
	public CComCoClass<CMarqueeCtrl, &CLSID_MarqueeCtrl>,
	public CComControl<CMarqueeCtrl>
{
public:
	//CContainedWindow m_ctlStatic;
	CContainedWindowT<CMarqueeWnd> m_marquee;
	CMarqueeWnd m_marqueeWnd;

#pragma warning(push)
#pragma warning(disable: 4355) // 'this' : usado na lista de inicialização do membro base


	CMarqueeCtrl()
		: m_marquee(_T("MarqueeWndCls"),this, 1)
	{
		m_bWindowOnly = TRUE;
	}

#pragma warning(pop)

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_MARQUEECTRL)


DECLARE_NOT_AGGREGATABLE(CMarqueeCtrl)

BEGIN_COM_MAP(CMarqueeCtrl)
	COM_INTERFACE_ENTRY(IMarqueeCtrl)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(IPersistStreamInit)
	COM_INTERFACE_ENTRY2(IPersist, IPersistStreamInit)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
	COM_INTERFACE_ENTRY(IObjectWithSite)
	COM_INTERFACE_ENTRY(IServiceProvider)
END_COM_MAP()

BEGIN_PROP_MAP(CMarqueeCtrl)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
#ifndef _WIN32_WCE
	PROP_ENTRY_TYPE("BackColor", DISPID_BACKCOLOR, CLSID_StockColorPage, VT_UI4)
#endif
	// Exemplo de entradas
	// PROP_ENTRY_TYPE("Nome da Propriedade", dispid, clsid, vtType)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CMarqueeCtrl)
	MESSAGE_HANDLER(WM_CREATE, OnCreate)
	MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
	//MESSAGE_HANDLER(WM_PAINT,OnPaint)
	CHAIN_MSG_MAP(CComControl<CMarqueeCtrl>)
ALT_MSG_MAP(1)
	// Substitua isto por entradas do mapa de mensagem para a subclasse Static
	MESSAGE_HANDLER(WM_CREATE,OnMarqueeCreate)
	CHAIN_MSG_MAP_MEMBER(m_marquee)
	//CHAIN_MSG_MAP_MEMBER(m_ctlStatic)
END_MSG_MAP()
// Protótipos de manipuladores:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		LRESULT lRes = CComControl<CMarqueeCtrl>::OnSetFocus(uMsg, wParam, lParam, bHandled);
		if (m_bInPlaceActive)
		{
			/*if(!IsChild(::GetFocus()))
				m_ctlStatic.SetFocus();*/
		}
		return lRes;
	}

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		RECT rc;
		GetClientRect(&rc);
		/*rc.right -= rc.left;
		rc.bottom -= rc.top;
		rc.top = rc.left = 0;*/
		m_marqueeWnd.Create(m_hWnd, rc, NULL, WS_CHILD | WS_VISIBLE);
		m_marquee.SubclassWindow(m_marqueeWnd);
		ATLASSERT(::IsWindow(m_marquee.m_hWnd));
		m_marqueeWnd.SetInc(m_marqueeWnd.GetCharWidth() / 8);
		m_marqueeWnd.SetSpace(2 * m_marqueeWnd.GetCharWidth());
		m_marquee.SetInc(m_marqueeWnd.GetCharWidth() / 8);
		m_marquee.SetSpace(2 * m_marqueeWnd.GetCharWidth());
		return 0;
	}

	//LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//{
	//	return 0;
	//}

	STDMETHOD(SetObjectRects)(LPCRECT prcPos,LPCRECT prcClip)
	{
		IOleInPlaceObjectWindowlessImpl<CMarqueeCtrl>::SetObjectRects(prcPos, prcClip);
		int cx, cy;
		cx = prcPos->right - prcPos->left;
		cy = prcPos->bottom - prcPos->top;
		::SetWindowPos(m_marquee.m_hWnd, nullptr, 0,
			0, cx, cy, SWP_NOZORDER | SWP_NOACTIVATE);
		return S_OK;
	}

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* const arr[] =
		{
			&IID_IMarqueeCtrl,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}
	

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IMarqueeCtrl
	OLE_COLOR m_clrBackColor;
	void OnBackColorChanged()
	{
		ATLTRACE(_T("OnBackColorChanged\n"));
	}
	STDMETHOD(_InternalQueryService)(REFGUID /*guidService*/, REFIID /*riid*/, void** /*ppvObject*/)
	{
		return E_NOTIMPL;
	}

	STDMETHOD(AddItem)(BSTR bstr, OLE_COLOR clr)
	{
		m_marquee.AddItem(COLE2CT(bstr), static_cast<COLORREF>(clr));
		return S_OK;
	}
	STDMETHOD(Start)()
	{
		m_marquee.Start();
		return S_OK;
	}
	STDMETHOD(Stop)()
	{
		m_marquee.Stop();
		return S_OK;
	}
	STDMETHOD(put_Loop)(VARIANT_BOOL bLoop)
	{
		m_marquee.SetLoop(bLoop == VARIANT_TRUE);
		return S_OK;
	}
	STDMETHOD(get_Loop)(VARIANT_BOOL *bLoop)
	{
		*bLoop = m_marquee.GetLoop() ? VARIANT_TRUE : VARIANT_FALSE;
		return S_OK;
	}
	STDMETHOD(put_Elapse)(long  ms)
	{
		m_marquee.SetElapse(ms);
		return S_OK;
	}
	STDMETHOD(get_Elapse)(long* ms)
	{
		*ms = m_marquee.GetElapse();
		return S_OK;
	}
	STDMETHOD(put_BackColor)(OLE_COLOR clr)
	{
		m_marquee.SetBackColor(static_cast<COLORREF>(clr));
		return S_OK;
	}
	STDMETHOD(get_BackColor)(OLE_COLOR *clr)
	{
		COLORREF color = m_marquee.GetBackColor();
		*clr = static_cast<OLE_COLOR>(color);
		return S_OK;
	}
	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

	LRESULT OnMarqueeCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		bHandled = FALSE;
		return 0;
	}
	virtual HRESULT OnDraw(_In_ ATL_DRAWINFO& di)
	{
		return S_OK;		// stops ATL from drawing "ATL:XX:XX"
	}
};

OBJECT_ENTRY_AUTO(__uuidof(MarqueeCtrl), CMarqueeCtrl)
