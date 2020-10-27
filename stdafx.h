// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
//#include <sdkddkver.h>
#define WINVER			0x0601
#define _WIN32_WINNT	0x0601
#define _WIN32_IE		0x0700
#define _RICHEDIT_VER	0x0500

#ifdef _DEBUG
#define PRODUCT_NAME	_T("irc11.exe")
#else
#define PRODUCT_NAME	_T("no5irc.exe")
#endif

#define CHAIN_COMMANDS_MEMBER_ID_RANGE(theChainMember, idFirst, idLast) \
    if(uMsg == WM_COMMAND && (LOWORD(wParam) >= idFirst && LOWORD(wParam) <=idLast)/* && (theChainMember.m_hWnd == GetFocus())*/) \
        CHAIN_MSG_MAP_MEMBER(theChainMember)

#include <strsafe.h>
#include <ctime>
#include <atlbase.h>
#include <atlapp.h>
#include <atlwin.h>

extern CAppModule _Module;

#include <atlmisc.h>
#include <atlcom.h>
#include <atlhost.h>
#include <atlwin.h>
#include <atlctl.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlcoll.h>
#include <atlctrlx.h>
#include <atlddx.h>
#include <winsock2.h>
#include <mstcpip.h>
#include <ws2tcpip.h>
//#include <atlsplit.h> with this doesn't work
#include "../NO5TL/atlsplit2.h"
#include <include/atlgdix.h>
#include <include/DotNetTabCtrl.h>
#include <include/TabbedFrame.h>
#include <include/tabbedmdi.h>
//#include <utf8/utf8.h>

#include "../NO5TL/No5Controls.h"
#include "..\NO5TL\WinSocketWithHandler.h"
#include "..\NO5TL\DataBuffer.h"
#include "..\NO5TL\mystring.h"
#include "..\NO5TL\WinFile.h"
#include "..\NO5TL\No5RichEdit.h"
#include "..\NO5TL\IniFile.h"
//#include "..\NO5TL\MarqueeMM.h"
#include "..\NO5TL\MarqueeWnd.h"
#include "..\NO5TL\VersionInfo.h"
#include "..\NO5TL\Color.h"
#include "viksoe/FontCombo.h"
#include "viksoe/ColorCombo.h"
using namespace NO5TL;


#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
