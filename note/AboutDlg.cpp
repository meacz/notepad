#include "stdafx.h"
#include "AboutDlg.h"
#include "resource.h"


CAboutDlg::CAboutDlg()
{
}

CAboutDlg::~CAboutDlg()
{

}


LPCTSTR CAboutDlg::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CDuiString CAboutDlg::GetSkinFile()
{
	return L"aboutdlg.xml";
}

LPCTSTR CAboutDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

UILIB_RESOURCETYPE CAboutDlg::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

CDuiString CAboutDlg::GetSkinFolder()
{
	return _T("");
}


void CAboutDlg::OnExit(TNotifyUI& msg)
{
	::ShowWindow(m_hWnd, SW_HIDE);
	//Close();
}


void CAboutDlg::InitWindow()
{
}

void CAboutDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn") ||
			msg.pSender->GetName() == _T("find_cancel"))
			OnExit(msg);
	}

	if (msg.sType == _T("link"))
	{
		CTextUI* pText = (CTextUI*)msg.pSender;
		CDuiString* str = pText->GetLinkContent(0);
		ShellExecute(NULL, L"open", str->GetData(), NULL, NULL, SW_SHOWNORMAL);
	}
}

void CAboutDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CAboutDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

LRESULT CAboutDlg::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CAboutDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CAboutDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}