// dllmain.cpp : Implementação do DllMain.

#include "pch.h"
#include "framework.h"
#include "resource.h"
#include "Marquee_i.h"
#include "dllmain.h"

CMarqueeModule _AtlModule;
BEGIN_OBJECT_MAP(mymap)
END_OBJECT_MAP()

// Ponto de Entrada DLL
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	hInstance;
	return _AtlModule.DllMain(dwReason, lpReserved);
}
