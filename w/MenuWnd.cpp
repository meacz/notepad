#include "stdafx.h"
#include "MenuWnd.h"

CMenuWnd::CMenuWnd()
{
}

CMenuWnd::~CMenuWnd()
{
	//PostQuitMessage(0);
}

LPCTSTR CMenuWnd::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CDuiString CMenuWnd::GetSkinFile()
{
	TCHAR szBuf[MAX_PATH] = { 0 };
	_stprintf_s(szBuf, MAX_PATH - 1, _T("%d"), uMenuID);
	return szBuf;
}

UILIB_RESOURCETYPE CMenuWnd::GetResourceType() const
{
	return UILIB_RESOURCE;
}

CDuiString CMenuWnd::GetSkinFolder()
{
	return _T("");
}

void CMenuWnd::OnExit(TNotifyUI& msg)
{
	//Close();
}

void CMenuWnd::Init(CControlUI* pOwner)
{
	m_pOwner = pOwner;
}

void CMenuWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("itemselect")){
		Close();
	}
	else if (msg.sType == _T("itemclick")) {
		CDuiString szMsg;
		if (msg.pSender->GetName() == _T("menu_New"))	   szMsg = _T("menu_New");
		if (msg.pSender->GetName() == _T("menu_Open"))     szMsg = _T("menu_Open");
		if (msg.pSender->GetName() == _T("menu_Save"))     szMsg = _T("menu_Save");
		if (msg.pSender->GetName() == _T("menu_SaveAs"))   szMsg = _T("menu_SaveAs");
		if (msg.pSender->GetName() == _T("menu_PageSet"))  szMsg = _T("menu_PageSet");
		if (msg.pSender->GetName() == _T("menu_Print"))    szMsg = _T("menu_Print");
		if (msg.pSender->GetName() == _T("menu_Exit"))     szMsg = _T("menu_Exit");
		
		if (msg.pSender->GetName() == _T("menu_Undo"))	   szMsg = _T("menu_Undo");
		if (msg.pSender->GetName() == _T("menu_Cut"))      szMsg = _T("menu_Cut");
		if (msg.pSender->GetName() == _T("menu_Copy"))     szMsg = _T("menu_Copy");
		if (msg.pSender->GetName() == _T("menu_Paste"))    szMsg = _T("menu_Paste");
		if (msg.pSender->GetName() == _T("menu_Delete"))   szMsg = _T("menu_Delete");
		if (msg.pSender->GetName() == _T("menu_Find"))     szMsg = _T("menu_Find");
		if (msg.pSender->GetName() == _T("menu_FindNext")) szMsg = _T("menu_FindNext");
		if (msg.pSender->GetName() == _T("menu_Replace"))  szMsg = _T("menu_Replace");
		if (msg.pSender->GetName() == _T("menu_Go"))       szMsg = _T("menu_Go");
		if (msg.pSender->GetName() == _T("menu_SelectAll"))szMsg = _T("menu_SelectAll");
		if (msg.pSender->GetName() == _T("menu_Date"))     szMsg = _T("menu_Date");

		if (msg.pSender->GetName() == _T("menu_Nextline")) szMsg = _T("menu_Nextline");
		if (msg.pSender->GetName() == _T("menu_Date"))     szMsg = _T("menu_Date");

		if (msg.pSender->GetName() == _T("menu_Statebar")) szMsg = _T("menu_Statebar");
		if (msg.pSender->GetName() == _T("menu_Font"))	   szMsg = _T("menu_Font");
		if (msg.pSender->GetName() == _T("menu_Setting")) szMsg = _T("menu_Setting");
		if (msg.pSender->GetName() == _T("menu_About"))    szMsg = _T("menu_About");

		m_pOwner->GetManager()->SendNotify(m_pOwner, szMsg, 0, 0, true);
	}
}

void CMenuWnd::InitWindow()
{
	m_MenuCur = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
}

void CMenuWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CMenuWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

LRESULT CMenuWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close();
	return 0;
}

LRESULT CMenuWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CMenuWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
	}
	bHandled = FALSE;
	return FALSE;
}





