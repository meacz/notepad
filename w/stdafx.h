// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

// dlg menu msg
#define MSG_MENU_FIND		WM_USER + 111    // 查找下一个
#define MSG_MENU_REPLACE	WM_USER + 112    // 替换
#define MSG_MENU_GOTOLINE	WM_USER + 113    // 跳转
#define MSG_MENU_NEW		WM_USER + 114    // 新建
#define MSG_MENU_OPEN		WM_USER + 115    // 打开
#define MSG_MENU_SAVE		WM_USER + 116    // 另存为


// color picker msg
#define WM_UPDATECOLOR			WM_USER + 117
#define UPDATE_WHEEL_COLOR		WM_USER + 118
#define UPDATE_BTN_COLOR		WM_USER + 119


// opction dlg msg
#define MSG_REMOVE_DEFAULTSET	WM_USER + 120










#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <atlbase.h>
#include <atlstr.h>

#include "UIlib.h"
#include "WinImplBase.h"
using namespace DuiLib;

// WTL

#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>


#include <atlcrack.h>
#include <atltypes.h>
#include <atlimage.h>
#include <atlddx.h>



#if defined _M_IX86
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
// TODO:  在此处引用程序需要的其他头文件


CString SaveIniFilePath();