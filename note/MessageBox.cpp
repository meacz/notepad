#include "stdafx.h"
#include "MessageBox.h"
#include "Resource.h"


CMessageBox::CMessageBox()
{
}

CMessageBox::~CMessageBox()
{

}


LPCTSTR CMessageBox::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CDuiString CMessageBox::GetSkinFile()
{
	return CDuiString(uStyleSet);
}

LPCTSTR CMessageBox::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

UILIB_RESOURCETYPE CMessageBox::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

CDuiString CMessageBox::GetSkinFolder()
{
	return _T("");
}


void CMessageBox::OnExit(TNotifyUI& msg)
{
	Close();
}

void CMessageBox::InitWindow()
{
	m_pLabel = (CLabelUI*)m_PaintManager.FindControl(_T("labbel_text2"));
	m_pLabelMin = (CLabelUI*)m_PaintManager.FindControl(_T("labbel_text"));
	ATLTRACE(L"init -----------\n");
}

void CMessageBox::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn"))
			Close(IDCANCEL);
		if (msg.pSender->GetName() == _T("btn_ok"))
			Close(IDOK);
		if (msg.pSender->GetName() == _T("btn_no"))
			Close(IDNO);
		if (msg.pSender->GetName() == _T("btn_cel"))
			Close(IDCANCEL);
	}
}

void CMessageBox::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CMessageBox::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

UINT CMessageBox::operator() (HWND hwnd, CString szText, UINT uFlags)
{
	CMessageBox *msg = new CMessageBox;
	msg->SetBoxStyle(uFlags);
	msg->Create(hwnd, L"", 0, WS_EX_TOOLWINDOW, 0, 0, 386, 192);
	if (uFlags == MB_OK) {
		msg->m_pLabelMin->SetText(szText);
		msg->m_pLabelMin->SetToolTip(szText);
	}
	
	if (uFlags == MB_YESNOCANCEL){
		msg->m_pLabel->SetText(szText);
		msg->m_pLabel->SetToolTip(szText);
	}

	msg->CenterWindow();
	return msg->ShowModal();
}

void CMessageBox::SetBoxStyle(int nFlag)
{
	if (nFlag == MB_OK)
		uStyleSet = L"msgbox.xml";
	else
		uStyleSet = L"msgbox2.xml";
}

