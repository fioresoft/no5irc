// irc09.cpp : main source file for irc09.exe
//

#include "stdafx.h"
#include "resource.h"
#include "View.h"
#include "aboutdlg.h"
#include "ChildFrm.h"
#include "MainFrm.h"
#include "irc11_i.c"

CAppModule _Module;

//class CNo5IrcModule : public ATL::CAtlExeModuleT< CNo5IrcModule >
//{
//public:
//	DECLARE_LIBID(LIBID_MyServerLib)
//	//DECLARE_REGISTRY_APPID_RESOURCEID(IDR_MYSERVER, "{24061199-b890-43b7-bda3-cf5b7d82a6f4}")
//};
//
//CNo5IrcModule _AtlModule;

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()


int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}
	wndMain.ShowWindow(nCmdShow);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));
	CWinSockDLL dll(2, 2);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES| ICC_TREEVIEW_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance,&LIBID_MyServerLib);
	ATLASSERT(SUCCEEDED(hRes));

	HMODULE hInstRich = ::LoadLibrary(CRichEditCtrl::GetLibraryName());
	ATLASSERT(hInstRich != NULL);

	AtlAxWinInit();

	int nRet = Run(lpstrCmdLine, nCmdShow);

	::FreeLibrary(hInstRich);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
