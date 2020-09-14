
#include "stdafx.h"
#include "DuiFrame.h"
#include "WinImplBase.h"
#include "MainDlg.h"


CAppModule _Module;


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{


	CPaintManagerUI::SetInstance(hInstance);

	HRESULT Hr = ::CoInitialize(NULL);
	if (FAILED(Hr)) return 0;

	CDuiFrameWnd* pFrame = new CDuiFrameWnd();
	if (pFrame == NULL) return 0;
	pFrame->Create(NULL, _T("notepad"), WS_VISIBLE | UI_WNDSTYLE_FRAME, WS_EX_ACCEPTFILES | WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
	pFrame->CenterWindow();


	// wtl

	HRESULT hRes = ::CoInitialize(NULL);
	ATLASSERT(SUCCEEDED(hRes));

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	{
		CMessageLoop theLoop;
		_Module.AddMessageLoop(&theLoop);
		CMainDlg dlgMain;

		pFrame->m_hWndChild = dlgMain.Create(*pFrame);
		pFrame->m_hWndChildEdit = dlgMain.m_sciEdit.m_hWnd;
		pFrame->m_childEdit = &dlgMain.m_sciEdit;
		pFrame->m_lfEdit = &dlgMain.m_lfont;
		StrCpyW(pFrame->m_lfEdit->lfFaceName, dlgMain.m_lfont.lfFaceName);

		dlgMain.ShowWindow(nCmdShow);
		pFrame->OpenFileFromCommandLine(lpCmdLine);
		int nRet = theLoop.Run();
		_Module.RemoveMessageLoop();
		return nRet;
	}

	_Module.Term();
	::CoUninitialize();

	CPaintManagerUI::MessageLoop();

	return 0;
}
