#include "stdafx.h"
#include "OptionDlgPage.h"
#include "Resource.h"

COptionDlgPage::COptionDlgPage()
{
	uPageID = L"optionpage_nor.xml";
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
		uPageID = L"optionpage_nor.xml";
		szSelectedName = szSettingName;
	}
	if (szSettingName == _T("normal_col")) {
		uPageID = L"optionpage_col.xml";
		szSelectedName = szSettingName;
	}

}


CDuiString COptionDlgPage::GetSkinFile()
{
	return CDuiString(uPageID);
}

LPCTSTR COptionDlgPage::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

UILIB_RESOURCETYPE COptionDlgPage::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
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

		pWrapLine = (CCheckBoxUI*)m_PaintManager.FindControl(_T("change_line"));
		pWrapLine->SetCheck(m_ParEdit->GetWrapMode());
		
		m_pEditPath = (CEditUI*)m_PaintManager.FindControl(_T("edit_inipath"));
		m_pBtnGoTo = (CButtonUI*)m_PaintManager.FindControl(_T("btn_path"));
		m_pBtnDefSet = (CButtonUI*)m_PaintManager.FindControl(_T("btn_defset"));

		m_pEditPath->SetText(SaveIniFilePath() + _T("\\notepad.ini"));
		m_pEditPath->SetToolTip(SaveIniFilePath() + _T("\\notepad.ini"));
	}

	if (szSelectedName == _T("normal_col")) {
		pTextSelBkColor = (CButtonUI*)m_PaintManager.FindControl(L"sel_bkcolor");
		pTextSelFrColor = (CButtonUI*)m_PaintManager.FindControl(L"sel_frcolor");
		pEditBkColor = (CButtonUI*)m_PaintManager.FindControl(L"edit_bkcolor");
		pTextFrColor = (CButtonUI*)m_PaintManager.FindControl(L"text_frcolor");
		pCurLineBkColor = (CButtonUI*)m_PaintManager.FindControl(L"curline_bkcolor");
		pMuiltiTextFrColor = (CButtonUI*)m_PaintManager.FindControl(L"multitext_frcolor");
		pMuiltiTextBkColor = (CButtonUI*)m_PaintManager.FindControl(L"multitext_bkcolor");
		pCurColor = (CButtonUI*)m_PaintManager.FindControl(L"cursor_color");
		pMultiCurColor = (CButtonUI*)m_PaintManager.FindControl(L"multicur_color");

		pTextSelBkColor->SetBkColor(C2ARGB(m_ParEdit->GetSelBack()));
		pTextSelFrColor->SetBkColor(C2ARGB(m_ParEdit->GetSelFore()));
		pEditBkColor->SetBkColor(C2ARGB(m_ParEdit->StyleGetBack(STYLE_DEFAULT)));
		pTextFrColor->SetBkColor(C2ARGB(m_ParEdit->StyleGetFore(0)));
		pCurLineBkColor->SetBkColor(C2ARGB(m_ParEdit->GetCaretLineBack()));
		pMuiltiTextFrColor->SetBkColor(C2ARGB(m_ParEdit->GetSelFore()));
		pMuiltiTextBkColor->SetBkColor(C2ARGB(m_ParEdit->GetSelBack()));
		pCurColor->SetBkColor(C2ARGB(m_ParEdit->GetCaretFore()));
		pMultiCurColor->SetBkColor(C2ARGB(m_ParEdit->GetAdditionalCaretFore()));
		
		mapcl.insert(pair<CString, COLORREF>(L"sel_bkcolor", m_ParEdit->GetSelBack()));
		mapcl.insert(pair<CString, COLORREF>(L"sel_frcolor", m_ParEdit->GetSelFore()));
		mapcl.insert(pair<CString, COLORREF>(L"edit_bkcolor", m_ParEdit->StyleGetBack(STYLE_DEFAULT)));
		mapcl.insert(pair<CString, COLORREF>(L"text_frcolor", m_ParEdit->StyleGetFore(0)));
		mapcl.insert(pair<CString, COLORREF>(L"curline_bkcolor", m_ParEdit->GetCaretLineBack()));
		mapcl.insert(pair<CString, COLORREF>(L"multitext_frcolor", m_ParEdit->GetSelFore()));
		mapcl.insert(pair<CString, COLORREF>(L"multitext_bkcolor", m_ParEdit->GetSelBack()));
		mapcl.insert(pair<CString, COLORREF>(L"cursor_color", m_ParEdit->GetCaretFore()));
		mapcl.insert(pair<CString, COLORREF>(L"multicur_color", m_ParEdit->GetAdditionalCaretFore()));
		
	}
}

void COptionDlgPage::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == L"sel_bkcolor"		||
			msg.pSender->GetName() == L"sel_frcolor"		||
			msg.pSender->GetName() == L"edit_bkcolor"		||
			msg.pSender->GetName() == L"text_frcolor"		||
			msg.pSender->GetName() == L"curline_bkcolor"	||
			msg.pSender->GetName() == L"multitext_frcolor"	||
			msg.pSender->GetName() == L"multitext_bkcolor"	||
			msg.pSender->GetName() == L"cursor_color"		||
			msg.pSender->GetName() == L"multicur_color"		
			

		) {
			// init color btn info
			InitColor(msg.pSender->GetName().GetData());
		}

		if(msg.pSender->GetName() == _T("btn_path"))
			ShellExecute(NULL, NULL, _T("explorer"), SaveIniFilePath(), NULL, SW_SHOW);

		if (msg.pSender->GetName() == _T("btn_defset"))  // MainDlg
			::SendMessage(GetParent(m_ParEdit->m_hWnd), MSG_REMOVE_DEFAULTSET, 0, 0);
	}

	if (msg.sType == _T("selectchanged")) {
		if (msg.pSender->GetName() == _T("change_line")) {
			BOOL bmodify = m_ParEdit->GetWrapMode();
			pWrapLine->SetCheck(!bmodify);
			m_ParEdit->SetWrapMode(pWrapLine->GetCheck());
			m_ParEdit->SetHScrollBar(TRUE);
		}
	}
}

void COptionDlgPage::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

// 由基类转发
void COptionDlgPage::OnUpdateBtnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	COLORREF colors;
	CButtonUI *pColorBtn = (CButtonUI *)m_PaintManager.FindControl(m_picker.m_szButtonChange);
	if (!pColorBtn) return;
	pColorBtn->SetBkColor(C2ARGB(m_picker.GetColorRGB()));


	if (m_picker.m_szButtonChange == L"sel_bkcolor")
		m_ParEdit->SetSelBack(TRUE, m_picker.GetColorRGB());
	if (m_picker.m_szButtonChange == L"sel_frcolor")
		m_ParEdit->SetSelFore(TRUE, m_picker.GetColorRGB());
	if (m_picker.m_szButtonChange == L"edit_bkcolor") {
		m_ParEdit->StyleSetBack(STYLE_DEFAULT, m_picker.GetColorRGB());
		m_ParEdit->StyleSetBack(0, m_picker.GetColorRGB());
	}
	if (m_picker.m_szButtonChange == L"text_frcolor")
		m_ParEdit->StyleSetFore(0, m_picker.GetColorRGB());
	if (m_picker.m_szButtonChange == L"curline_bkcolor") {
		m_ParEdit->SetCaretLineBack(m_picker.GetColorRGB());
	}
	if (m_picker.m_szButtonChange == L"multitext_frcolor")
		m_ParEdit->SetAdditionalSelFore(m_picker.GetColorRGB());
	if (m_picker.m_szButtonChange == L"multitext_bkcolor")
		m_ParEdit->SetAdditionalSelBack(m_picker.GetColorRGB());
	if (m_picker.m_szButtonChange == L"cursor_color")
		m_ParEdit->SetCaretFore(m_picker.GetColorRGB());
	if (m_picker.m_szButtonChange == L"multicur_color")
		m_ParEdit->SetAdditionalCaretFore(m_picker.GetColorRGB());

	if ((const char*)wParam == "upcolor") {  // update color list
		
		mapcl.erase(m_picker.m_szButtonChange);
		COLORREF color = m_picker.GetColorRGB();
		mapcl.insert(pair<CString, COLORREF>(m_picker.m_szButtonChange, color));

		//std::multimap<CString, COLORREF>::iterator iter;
		//for (iter = mapcl.begin(); iter != mapcl.end(); iter++)
		//	ATLTRACE(L"Click____%s R:%d G:%d B:%d\n", iter->first,
		//		GetRValue(iter->second),
		//		GetGValue(iter->second),
		//		GetBValue(iter->second));
	}

	if ((const char*)wParam == "noupcolor") {


		m_ParEdit->SetSelBack(TRUE, ColorFromName(L"sel_bkcolor"));
		m_ParEdit->SetSelFore(TRUE, ColorFromName(L"sel_frcolor"));
		m_ParEdit->StyleSetBack(STYLE_DEFAULT, ColorFromName(L"edit_bkcolor"));
		m_ParEdit->StyleSetBack(0, ColorFromName(L"edit_bkcolor"));
		m_ParEdit->StyleSetFore(0, ColorFromName(L"text_frcolor"));
		m_ParEdit->SetCaretLineBack(ColorFromName(L"curline_bkcolor"));
		m_ParEdit->SetAdditionalSelFore(ColorFromName(L"multitext_frcolor"));
		m_ParEdit->SetAdditionalSelBack(ColorFromName(L"multitext_frcolor"));
		m_ParEdit->SetCaretFore(ColorFromName(L"cursor_color"));
		m_ParEdit->SetAdditionalCaretFore(ColorFromName(L"multicur_color"));

		pTextSelBkColor->SetBkColor(C2ARGB(ColorFromName(L"sel_bkcolor")));
		pTextSelFrColor->SetBkColor(C2ARGB(ColorFromName(L"sel_frcolor")));
		pEditBkColor->SetBkColor(C2ARGB(ColorFromName(L"edit_bkcolor")));
		pTextFrColor->SetBkColor(C2ARGB(ColorFromName(L"text_frcolor")));
		pCurLineBkColor->SetBkColor(C2ARGB(ColorFromName(L"curline_bkcolor")));
		pMuiltiTextFrColor->SetBkColor(C2ARGB(ColorFromName(L"multitext_frcolor")));
		pMuiltiTextBkColor->SetBkColor(C2ARGB(ColorFromName(L"multitext_frcolor")));
		pCurColor->SetBkColor(C2ARGB(ColorFromName(L"cursor_color")));
		pMultiCurColor->SetBkColor(C2ARGB(ColorFromName(L"multicur_color")));

		//std::multimap<CString, COLORREF>::iterator iter2;
		//for (iter2 = mapcl.begin(); iter2 != mapcl.end(); iter2++)
		//	ATLTRACE(L"C____%s R:%d G:%d B:%d\n", iter2->first,
		//		GetRValue(iter2->second),
		//		GetGValue(iter2->second),
		//		GetBValue(iter2->second));
	}

}

COLORREF COptionDlgPage::ColorFromName(CString szName)
{
	std::multimap<CString, COLORREF>::iterator iter;
	iter = mapcl.find(szName);
	return iter->second;
}


LRESULT COptionDlgPage::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
	// COptionDlg 转发
	case UPDATE_BTN_COLOR:	OnUpdateBtnColor(uMsg, wParam, lParam, bHandled); return TRUE; break;
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
