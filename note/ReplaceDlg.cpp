#include "stdafx.h"
#include "ReplaceDlg.h"
#include "Resource.h"

CReplaceDlg::CReplaceDlg()
{
}

CReplaceDlg::~CReplaceDlg()
{
}

LPCTSTR CReplaceDlg::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CDuiString CReplaceDlg::GetSkinFile()
{
	return L"ReplaceDlg.xml";
}

LPCTSTR CReplaceDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

UILIB_RESOURCETYPE CReplaceDlg::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

CDuiString CReplaceDlg::GetSkinFolder()
{
	return _T("");
}

void CReplaceDlg::OnExit(TNotifyUI& msg)
{
	::ShowWindow(m_hWnd, SW_HIDE);
	//Close();
}

void CReplaceDlg::Init(CScintillaCtrl* pSciEdit, CString szSelText)
{
	m_ParEditRep = pSciEdit;
	m_RParEditSelText = szSelText;

	if (m_RParEditSelText != "")
		m_pRFindEdit->SetText(m_RParEditSelText);
}

void CReplaceDlg::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn") ||
			msg.pSender->GetName() == _T("rep_cancel"))
			OnExit(msg);
		if (msg.pSender->GetName() == _T("findnext")) 
			FindReplace(FR_FINDNEXT);
		if (msg.pSender->GetName() == _T("replace"))
			FindReplace(FR_REPLACE);
		if (msg.pSender->GetName() == _T("replaceall"))
			FindReplace(FR_REPLACEALL);
	}
}

void CReplaceDlg::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

void CReplaceDlg::InitWindow() 
{
	m_pRFindEdit = (CEditUI*)m_PaintManager.FindControl(_T("rfind_edit"));
	m_pRepEdit = (CEditUI*)m_PaintManager.FindControl(_T("rep_edit"));
	m_pRFindUpperBox = (CCheckBoxUI *)m_PaintManager.FindControl(_T("ckbox_uorlcase"));

}

LRESULT CReplaceDlg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

LRESULT CReplaceDlg::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CReplaceDlg::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CReplaceDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

void CReplaceDlg::FindReplace(UINT nFlag)
{
	DWORD flags;
	flags |= m_pRFindUpperBox->GetCheck() ? SCFIND_MATCHCASE : SCFIND_NONE;

	int pos = 0, fstart, fend;

	Sci_TextToFind ttf;
	CString str = m_pRFindEdit->GetText().GetData();
	if (m_RParEditSelText != "") {
		str = m_RParEditSelText;
		m_RParEditSelText = L"";  // 只在窗口初始化的时候使用一次
	}
	CString sRep = m_pRepEdit->GetText().GetData();
	CStringA findStr = m_ParEditRep->W2UTF8(str, -1);

	if (nFlag == FR_FINDNEXT) {  // 查找
		fstart = m_ParEditRep->GetSelectionEnd();
		fend = m_ParEditRep->GetLength();

		ttf.chrg.cpMin = fstart;
		ttf.chrg.cpMax = fend;
		ttf.lpstrText = findStr.GetBuffer(findStr.GetLength());
		pos = m_ParEditRep->FindText(flags, &ttf);
		if (pos != -1) {
			m_ParEditRep->SetSel(pos, pos + findStr.GetLength());
			findStr.ReleaseBuffer();
			ATLTRACE(L"%d  %d\n", pos, 0);
		}
	}

	// 替换
	if (nFlag == FR_REPLACE) {
		if (newPos != 0) {
			m_ParEditRep->SetSel(newPos, newPos + findStr.GetLength());
			m_ParEditRep->ReplaceSel(sRep);
			newPos += sRep.GetLength() - str.GetLength();
		}

		ttf.chrg.cpMin = m_ParEditRep->GetSelectionEnd();
		ttf.chrg.cpMax = m_ParEditRep->GetLength();
		ttf.lpstrText = findStr.GetBuffer(findStr.GetLength());
		pos = m_ParEditRep->FindText(flags, &ttf);
		if (pos != -1) {
			m_ParEditRep->SetSel(pos, pos + findStr.GetLength());
			newPos = pos + findStr.GetLength();
			findStr.ReleaseBuffer();
		}
	}

	// 全部替换
	if (nFlag == FR_REPLACEALL) {
		if (m_pRFindEdit->GetText() == _T("")) return;
		m_ParEditRep->GotoPos(0);
		while (pos != -1) {
			ttf.chrg.cpMin = m_ParEditRep->GetSelectionEnd();
			ttf.chrg.cpMax = m_ParEditRep->GetLength();
			ttf.lpstrText = findStr.GetBuffer(findStr.GetLength());
			pos = m_ParEditRep->FindText(flags, &ttf);
			if (pos != -1) {
				m_ParEditRep->SetSel(pos, pos + findStr.GetLength());
				m_ParEditRep->ReplaceSel(sRep);
			}
		}
		m_ParEditRep->GotoPos(0);
		findStr.ReleaseBuffer();
		return;
	}

	if (pos == -1)
		MsgBox(m_hWnd, _T("找不到\"") + str + _T("\""),MB_OK);
}
