// dllmain.h : Declaração da classe módulo.

class CMarqueeModule : public ATL::CAtlDllModuleT< CMarqueeModule >
{
public :
	DECLARE_LIBID(LIBID_MarqueeLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MARQUEE, "{52deb09c-a331-4392-b9ab-e1918bec2adf}")
};

extern class CMarqueeModule _AtlModule;
