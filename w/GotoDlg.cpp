#include "stdafx.h"
#include "GotoDlg.h"
#include "Resource.h"


CGotoDlg::CGotoDlg()
{
}

CGotoDlg::~CGotoDlg()
{

}


LPCTSTR CGotoDlg::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CDuiString CGotoDlg::GetSkinFile()
{
	TCHAR szBuf[MAX_PATH] = { 0 };
	_stprintf_s(szBuf, MAX_PATH - 1, _T("%d"), IDR_XML6);
	return szBuf;
}

UILIB_RESOURCETYPE CGotoDlg::GetResourceType() const
{
	return UILIB_RESOURCE;
}

CDuiString CGotoDlg::GetSkinFolder()
{
	return _T("");
}


void CGotoDlg::OnExit(TNotifyUI& msg)
{
	::ShowWindow(m_hWnd, SW_HIDE);
	//Close();
}

void CGotoDlg::Init(CScintillaCtrl* pSciEdit)
{
	m_ParentEdit = pSciEdit;

	m_pLineCountText = (CLabelUI*)m_PaintManager.FindControl(_T("skip_linecount"));
	m_pLineNumEdit = (CEditUI*)m_PaintManager.FindControl(_T("skip_linenumedit"));
	m_pCloseChek = (CCheckBoxUI*)m_PaintManager.FindControl(_T("check_goto"));

	SetLineInfo();
}

void CGotoDlg::InitWindow()
{

}

void CGotoDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn") ||
			msg.pSender->GetName() == _T("skip_cancel"))
			OnExit(msg);
		if (msg.pSender->GetName() == _T("skip_go")) {
			int nLine = _wtoi(m_pLineNumEdit->GetText());
			m_ParentEdit->GotoLine(nLine - 1);
			m_ParentEdit->SetFocus();
			if(m_pCloseChek->GetCheck())
				OnExit(msg);
		}
	}

}

void CGotoDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CGotoDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

LRESULT CGotoDlg::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CGotoDlg::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	SetLineInfo();

	bHandled = FALSE;
	return 0;
}

LRESULT CGotoDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CGotoDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

void CGotoDlg::SetLineInfo()
{
	m_nLineCount = m_ParentEdit->GetLineCount();
	m_nLineCount = max(m_nLineCount, 1);
	CString szLineNum;
	szLineNum.Format(_T("ÐÐºÅ(1 - %d):"), m_nLineCount);
	m_pLineCountText->SetText(szLineNum);
}





