#include "stdafx.h"
#include "OpctionDlg.h"
#include "Resource.h"

COpctionDlg::COpctionDlg()
{
	m_FocusItem = _T("normal_set");
	m_nPageIndex = 0;
	m_OptionPage_Nor = new COptionDlgPage();
	m_OptionPage_Ini = new COptionDlgPage();
}

COpctionDlg::~COpctionDlg()
{

}


LPCTSTR COpctionDlg::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CDuiString COpctionDlg::GetSkinFile()
{
	TCHAR szBuf[MAX_PATH] = { 0 };
	_stprintf_s(szBuf, MAX_PATH - 1, _T("%d"), IDR_XML8);
	return szBuf;
}

UILIB_RESOURCETYPE COpctionDlg::GetResourceType() const
{
	return UILIB_RESOURCE;
}

CDuiString COpctionDlg::GetSkinFolder()
{
	return _T("");
}


void COpctionDlg::OnExit(TNotifyUI& msg)
{
	::ShowWindow(m_hWnd, SW_HIDE);
	//Close();
}

LRESULT COpctionDlg::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	bHandled = FALSE;
	return __super::OnCreate(uMsg, wParam, lParam, bHandled);
}

void COpctionDlg::InitWindow()
{
	// 默认设置页面
	// sci edit控件未被创建
	m_OptionPage_Nor->SetCreatePageID(_T("normal_set"));
	m_OptionPage_Nor->Create(m_hWnd, _T(""), WS_CHILD, 0, 0, 0, 0);

	m_OptionPage_Ini->SetCreatePageID(_T("normal_ini"));
	m_OptionPage_Ini->Create(m_hWnd, _T(""), WS_CHILD, 0, 0, 0, 0);

}

void COpctionDlg::Init()
{
	m_nPageIndex = 0;
	m_OptionPage_Nor->ShowWindow();
	m_OptionPage_Nor->GetParentCtrl(m_ParEdit);
	m_OptionPage_Nor->Init();

	ShowPages();
}

void COpctionDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn"))
			OnExit(msg);

		if (msg.pSender->GetName() == _T("normal_set")) {
			m_nPageIndex = 0;
			ShowPages();
		}
		if(msg.pSender->GetName() == _T("normal_ini")){
			m_nPageIndex = 1;
			ShowPages();
		}
	}
}

void COpctionDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT COpctionDlg::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{

	bHandled = FALSE;
	return FALSE;
}


LRESULT COpctionDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
		// 由CWtlColorPicker OnOK发送
	case UPDATE_BTN_COLOR:	::SendMessage(*m_OptionPage_Nor, UPDATE_BTN_COLOR, 0, 0);
	}
	bHandled = FALSE;
	return FALSE;
}

void COpctionDlg::ShowPages()
{
	COptionDlgPage *m_page[] = { m_OptionPage_Nor, m_OptionPage_Ini };
	int i = -1;
	while (++i < _countof(m_page))
		m_page[i]->ShowWindow(i == m_nPageIndex ? SW_SHOW : SW_HIDE);
	
	m_page[m_nPageIndex]->Init();
	m_page[m_nPageIndex]->GetParentCtrl(m_ParEdit);
}
