// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

// dlg menu msg
#define MSG_MENU_FIND		WM_USER + 111    // ������һ��
#define MSG_MENU_REPLACE	WM_USER + 112    // �滻
#define MSG_MENU_GOTOLINE	WM_USER + 113    // ��ת
#define MSG_MENU_NEW		WM_USER + 114    // �½�
#define MSG_MENU_OPEN		WM_USER + 115    // ��
#define MSG_MENU_SAVE		WM_USER + 116    // ���Ϊ


// color picker msg
#define WM_UPDATECOLOR			WM_USER + 117
#define UPDATE_WHEEL_COLOR		WM_USER + 118
#define UPDATE_BTN_COLOR		WM_USER + 119


// opction dlg msg
#define MSG_REMOVE_DEFAULTSET	WM_USER + 120










#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

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
// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�


CString SaveIniFilePath();