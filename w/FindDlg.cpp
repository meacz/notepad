#include "stdafx.h"
#include "FindDlg.h"

#include "stdafx.h"
#include "MenuWnd.h"
#include "GroupBox/UIGroupBox.h"


CFindDlg::CFindDlg()
{
}

CFindDlg::~CFindDlg()
{

}


LPCTSTR CFindDlg::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CDuiString CFindDlg::GetSkinFile()
{
	TCHAR szBuf[MAX_PATH] = { 0 };
	_stprintf_s(szBuf, MAX_PATH - 1, _T("%d"), IDR_XML4);
	return szBuf;
}

UILIB_RESOURCETYPE CFindDlg::GetResourceType() const
{
	return UILIB_RESOURCE;
}

CDuiString CFindDlg::GetSkinFolder()
{
	return _T("");
}

CControlUI* CFindDlg::CreateControl(LPCTSTR pstrClass) 
{
	if (_tcscmp(pstrClass, _T("GroupBox")) == 0)
	{
		return new CGroupBoxUI;
	}
	return NULL;
}

void CFindDlg::OnExit(TNotifyUI& msg)
{
	::ShowWindow(m_hWnd, SW_HIDE);
	//Close();
}

void CFindDlg::Init(CScintillaCtrl* pSciEdit, CString szSelText)
{
	m_ParEdit = pSciEdit;
	m_ParEditSelText = szSelText;

	if (m_ParEditSelText != L"")
		m_FindEdit->SetText(m_ParEditSelText);

}

void CFindDlg::InitWindow()
{
	m_pFindUp = (COptionUI*)m_PaintManager.FindControl(_T("find_up"));
	m_pFindUpperBox = (CCheckBoxUI *)m_PaintManager.FindControl(_T("find_lowercase"));
	m_FindEdit = (CEditUI *)m_PaintManager.FindControl(_T("find_edit"));

}

void CFindDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn") ||
			msg.pSender->GetName() == _T("find_cancel"))
			OnExit(msg);
		if (msg.pSender->GetName() == _T("find_ok"))
			Find();
	}
}

void CFindDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CFindDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

LRESULT CFindDlg::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CFindDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CFindDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

void CFindDlg::Find()
{
	DWORD flags = 0;
	flags |= m_pFindUpperBox->GetCheck() ? SCFIND_MATCHCASE : SCFIND_NONE;
	int pos = 0, fstart, fend;

	Sci_TextToFind ttf;
	CString str = m_FindEdit->GetText().GetData();
	if (m_ParEditSelText != L"") {
		str = m_ParEditSelText;
		m_ParEditSelText = L"";  // 只在窗口初始化的时候使用一次
	}
	CStringA findStr = m_ParEdit->W2UTF8(str, -1);

	// 默认为向上查找的开始位置
	fstart = m_ParEdit->GetLength() - (m_ParEdit->GetLength() - m_ParEdit->GetSelectionStart());
	fend = 0;
	if (!m_pFindUp->IsSelected()) {  // 向下
		fstart = m_ParEdit->GetSelectionEnd();
		fend = m_ParEdit->GetLength();
	}
	ttf.chrg.cpMin = fstart;
	ttf.chrg.cpMax = fend;
	ttf.lpstrText = findStr.GetBuffer(findStr.GetLength());
	pos = m_ParEdit->FindText(flags, &ttf);
	if (pos != -1) {
		m_ParEdit->SetSel(pos, pos + findStr.GetLength());
		findStr.ReleaseBuffer();
		//ATLTRACE(L"%d  %d\n", pos, 0);
	}
	if (pos == -1)
		::MessageBox(m_hWnd, _T("找不到\"") + str + _T("\""), _T("记事本"), MB_OK | MB_ICONQUESTION);

}



