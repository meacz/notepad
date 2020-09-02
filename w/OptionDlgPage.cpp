#include "stdafx.h"
#include "OptionDlgPage.h"
#include "Resource.h"

COptionDlgPage::COptionDlgPage()
{
	uPageID = IDR_XML9;
	szSelectedName = _T("normal_set");
}

COptionDlgPage::~COptionDlgPage()
{

}


LPCTSTR COptionDlgPage::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

void COptionDlgPage::SetCreatePageID(CString szSettingName) 
{
	if (szSettingName == _T("normal_set")){
		uPageID = IDR_XML9;
		szSelectedName = szSettingName;
	}
	if (szSettingName == _T("normal_ini")) {
		uPageID = IDR_XML10;
		szSelectedName = szSettingName;
	}

}


CDuiString COptionDlgPage::GetSkinFile()
{
	TCHAR szBuf[MAX_PATH] = { 0 };
	_stprintf_s(szBuf, MAX_PATH - 1, _T("%d"), uPageID);
	return szBuf;
}

UILIB_RESOURCETYPE COptionDlgPage::GetResourceType() const
{
	return UILIB_RESOURCE;
}

CDuiString COptionDlgPage::GetSkinFolder()
{
	return _T("");
}


void COptionDlgPage::OnExit(TNotifyUI& msg)
{
	//Close();
}


void COptionDlgPage::InitWindow()
{
	m_picker.Create(m_hWnd);

	CRect rect;
	::GetWindowRect(m_hWnd, &rect);
	rect.OffsetRect(153 - rect.left, 36 - rect.top);
	::MoveWindow(m_hWnd, rect.left, rect.top, rect.Width(), rect.Height(), FALSE);
}

void COptionDlgPage::Init()
{

	if (szSelectedName == _T("normal_set")) {

		//pFont_FrColor = (CButtonUI*)m_PaintManager.FindControl(_T("font_frcolor"));
		//pFont_BkColor = (CButtonUI*)m_PaintManager.FindControl(_T("font_bkcolor"));
		//pFont_FrColor->SetBkColor(C2ARGB(m_ParEdit->StyleGetFore(STYLE_DEFAULT)));
		//pFont_BkColor->SetBkColor(C2ARGB(m_ParEdit->StyleGetBack(STYLE_DEFAULT)));

		pText_SelBkColor = (CButtonUI*)m_PaintManager.FindControl(_T("sel_bkcolor"));
		pText_SelFrColor = (CButtonUI*)m_PaintManager.FindControl(_T("sel_frcolor"));
		pWrapLine = (CCheckBoxUI*)m_PaintManager.FindControl(_T("change_line"));

		pText_SelBkColor->SetBkColor(C2ARGB(m_ParEdit->GetSelBack()));
		pText_SelFrColor->SetBkColor(C2ARGB(m_ParEdit->GetSelFore()));
		bWrapLine = m_ParEdit->GetWrapMode();
		pWrapLine->SetCheck(bWrapLine);
	}

	if (szSelectedName == _T("normal_ini")) {
		m_pEditPath = (CEditUI*)m_PaintManager.FindControl(_T("edit_inipath"));
		m_pBtnGoTo = (CButtonUI*)m_PaintManager.FindControl(_T("btn_path"));
		m_pBtnDefSet = (CButtonUI*)m_PaintManager.FindControl(_T("btn_defset"));

		m_pEditPath->SetText(SaveIniFilePath() + _T("\\notepad.ini"));
		m_pEditPath->SetToolTip(SaveIniFilePath() + _T("\\notepad.ini"));
	}
}

void COptionDlgPage::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (/*msg.pSender->GetName() == _T("font_frcolor") ||
			msg.pSender->GetName() == _T("font_bkcolor") ||*/
			msg.pSender->GetName() == _T("sel_bkcolor")  ||
			msg.pSender->GetName() == _T("sel_frcolor")
		) {
			InitColor(msg.pSender->GetName().GetData());
		}

		if (msg.pSender->GetName() == _T("change_line")){
			bWrapLine = !bWrapLine;
			m_ParEdit->SetWrapMode(bWrapLine);
			pWrapLine->SetCheck(!bWrapLine);
			m_ParEdit->SetHScrollBar(!bWrapLine);
		}

		if(msg.pSender->GetName() == _T("btn_path"))
			ShellExecute(NULL, NULL, _T("explorer"), SaveIniFilePath(), NULL, SW_SHOW);

		if (msg.pSender->GetName() == _T("btn_defset"))
			::SendMessage(GetParent(m_ParEdit->m_hWnd), MSG_REMOVE_DEFAULTSET, 0, 0);
	}
}

void COptionDlgPage::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}


void COptionDlgPage::OnUpdateBtnColor()
{
	COLORREF colors;
	CButtonUI *pColorBtn = (CButtonUI *)m_PaintManager.FindControl(m_picker.m_szButtonChange);
	pColorBtn->SetBkColor(C2ARGB(m_picker.GetColorRGB()));


	//if (m_picker.m_szButtonChange == _T("font_frcolor"))
	//	m_ParEdit->StyleSetFore(STYLE_DEFAULT, m_picker.GetColorRGB());
	//if (m_picker.m_szButtonChange == _T("font_bkcolor"))
	//	m_ParEdit->StyleSetBack(STYLE_DEFAULT, m_picker.GetColorRGB());
	if (m_picker.m_szButtonChange == _T("sel_bkcolor"))
		m_ParEdit->SetSelBack(TRUE, m_picker.GetColorRGB());
	if (m_picker.m_szButtonChange == _T("sel_frcolor"))
		m_ParEdit->SetSelFore(TRUE, m_picker.GetColorRGB());
	
}

LRESULT COptionDlgPage::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
	// COptionDlg ×ª·¢
	case UPDATE_BTN_COLOR:	OnUpdateBtnColor(); return TRUE; break;
	}

	bHandled = FALSE;
	return FALSE;
}

DWORD COptionDlgPage::C2ARGB(COLORREF cl) 
{
	int r = GetRValue(cl);
	int g = GetGValue(cl);
	int b = GetBValue(cl);
	return 255 << 24 | r << 16 | g << 8 | b;
}

COLORREF COptionDlgPage::ARGB2C(DWORD dw)
{
	int r = (dw >> 16) & 0x00FF;
	int g = (dw >> 8) & 0x0000FF;
	int b = dw & 0x000000FF;
	
	return RGB(r, g, b);
}

void COptionDlgPage::GetParentCtrl(CScintillaCtrl * pEdit)
{
	m_ParEdit = pEdit;
}

void COptionDlgPage::InitColor(CString szColorBtn)
{
	CButtonUI *pColorBtn = (CButtonUI*)m_PaintManager.FindControl(szColorBtn);

	DWORD dw = pColorBtn->GetBkColor();
	COLORREF color = ARGB2C(dw);
	m_picker.ShowWindow(SW_SHOW);
	m_picker.SetColorRGB(GetRValue(color), GetGValue(color), GetBValue(color));
	m_picker.m_szButtonChange = szColorBtn;
}
