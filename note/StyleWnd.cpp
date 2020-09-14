#include "stdafx.h"
#include "StyleWnd.h"
#include "Resource.h"


CStyleWnd::CStyleWnd()
{
	iStyleSelect = STYLE_DEFAULT;
}

CStyleWnd::~CStyleWnd()
{
	fontfamily.clear();
	map.clear();
}


LPCTSTR CStyleWnd::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CDuiString CStyleWnd::GetSkinFile()
{
	return L"styledialog.xml";
}

LPCTSTR CStyleWnd::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

UILIB_RESOURCETYPE CStyleWnd::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

CControlUI* CStyleWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("GroupBox")) == 0)
	{
		return new CGroupBoxUI;
	}
	return NULL;
}

CDuiString CStyleWnd::GetSkinFolder()
{
	return _T("");
}


void CStyleWnd::OnExit(TNotifyUI& msg)
{
	::ShowWindow(m_hWnd, SW_HIDE);
	//Close();
}

std::vector<CString> CStyleWnd::fontfamily;
std::multimap<CString, int> CStyleWnd::map;
INT CALLBACK EnumFontNameProc(LOGFONT *plf, TEXTMETRIC* ptm, INT nFontType, LPARAM lParam)
{
	CStyleWnd::fontfamily.push_back(plf->lfFaceName);
	CStyleWnd::map.insert(pair<CString, int>(plf->lfFaceName, plf->lfCharSet));
	//ATLTRACE(L"%d %s\n", plf->lfCharSet, plf->lfFaceName);
	return TRUE;
}

void CStyleWnd::Init(CScintillaCtrl* pSciEdit)
{
	m_ParEdit = pSciEdit;
	UpDateControls();

	suinfo = { 0 };
	suinfo.cBack = m_ParEdit->StyleGetBack(iStyleSelect);
	suinfo.cFore = m_ParEdit->StyleGetFore(iStyleSelect);
	suinfo.szFontFamily = m_ParEdit->StyleGetFont(iStyleSelect);
	suinfo.iFontSize = m_ParEdit->StyleGetSize(iStyleSelect);
	suinfo.bblod = m_ParEdit->StyleGetBold(iStyleSelect);
	suinfo.bitalic = m_ParEdit->StyleGetItalic(iStyleSelect);
	suinfo.bunderline = m_ParEdit->StyleGetUnderline(iStyleSelect);
}

void CStyleWnd::InitWindow()
{
	CDC dc = GetDC(NULL);
	::EnumFontFamilies(dc, (LPTSTR)NULL, (FONTENUMPROC)EnumFontNameProc, NULL);

	m_pListStyle = (CListUI*)m_PaintManager.FindControl(_T("list_style"));
	m_pComFontFamily = (CComboUI*)m_PaintManager.FindControl(_T("combox_font"));
	m_pComFontSize = (CComboUI*)m_PaintManager.FindControl(_T("combox_fontsize"));
	m_pBtnStyleFore = (CButtonUI*)m_PaintManager.FindControl(_T("btn_fore"));
	m_pBtnStyleBack = (CButtonUI*)m_PaintManager.FindControl(_T("btn_back"));
	m_pCheckBlod = (CCheckBoxUI*)m_PaintManager.FindControl(_T("cbox_bold"));
	m_pCheckItalic = (CCheckBoxUI*)m_PaintManager.FindControl(_T("cbox_italic"));
	m_pCheckUnderline = (CCheckBoxUI*)m_PaintManager.FindControl(_T("cbox_underline"));

	m_picker.Create(m_hWnd);
	sort(fontfamily.begin(), fontfamily.end());
	for (size_t i = 0; i < fontfamily.size(); ++i) {
		CListLabelElementUI* pListLabel = new CListLabelElementUI;
		pListLabel->SetText(fontfamily[i]);
		pListLabel->SetToolTip(fontfamily[i]);
		pListLabel->SetAttributeList(_T("height=\"23\""));
		m_pComFontFamily->Add(pListLabel);
	}

	for (int j = 0; j < m_pComFontSize->GetCount(); ++j)
		fontsize.push_back(_wtoi(m_pComFontSize->GetItemAt(j)->GetText()));
}

void CStyleWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn") ||
			msg.pSender->GetName() == _T("btn_save"))
			OnExit(msg);
		if (msg.pSender->GetName() == _T("btn_close")) {
			m_ParEdit->StyleSetBack(iStyleSelect, suinfo.cBack);
			m_ParEdit->StyleSetFore(iStyleSelect, suinfo.cFore);
			m_ParEdit->StyleSetFont(iStyleSelect, suinfo.szFontFamily);
			m_ParEdit->StyleSetSize(iStyleSelect, suinfo.iFontSize);
			m_ParEdit->StyleSetBold(iStyleSelect, suinfo.bblod);
			m_ParEdit->StyleSetItalic(iStyleSelect, suinfo.bitalic);
			m_ParEdit->StyleSetUnderline(0, suinfo.bunderline);
			OnExit(msg);
		}

		if (msg.pSender->GetName() == _T("btn_fore") ||
			msg.pSender->GetName() == _T("btn_back"))
			InitColor(msg.pSender->GetName().GetData());
	}

	if (msg.sType == _T("selectchanged")) {
		if (msg.pSender->GetName() == _T("cbox_bold"))
			m_ParEdit->StyleSetBold(iStyleSelect, m_pCheckBlod->GetCheck());
		if (msg.pSender->GetName() == _T("cbox_italic"))
			m_ParEdit->StyleSetItalic(iStyleSelect, m_pCheckItalic->GetCheck());
		if (msg.pSender->GetName() == _T("cbox_underline"))
			m_ParEdit->StyleSetUnderline(0, m_pCheckUnderline->GetCheck());
	}

	if (msg.sType == _T("itemselect")) {
		if (msg.pSender->GetName() == _T("list_style")) {
			switch (m_pListStyle->GetCurSel()){
			case 0: iStyleSelect = STYLE_DEFAULT; break;
			case 1: iStyleSelect = STYLE_LINENUMBER; break;
			case 2: iStyleSelect = STYLE_CONTROLCHAR; break;
			//default: iStyleSelect = STYLE_DEFAULT;
			ATLTRACE(L"style: %d  font: %s %d %d \n", 
				32, 
				m_ParEdit->StyleGetFont(32),
				m_ParEdit->StyleGetCharacterSet(32),
				m_ParEdit->StyleGetCharacterSet(33));
			}
			UpDateControls();
		}

		if (msg.pSender->GetName() == _T("combox_font")) {
			CString str = m_pComFontFamily->GetItemAt(m_pComFontFamily->GetCurSel())->GetText().GetData();
			m_ParEdit->StyleSetFont(iStyleSelect, str);
			m_ParEdit->StyleSetCharacterSet(iStyleSelect, FindCharSet(str));
			
			ATLTRACE(L"style: %d  font: %s %d %d \n",
				32,
				m_ParEdit->StyleGetFont(32),
				m_ParEdit->StyleGetCharacterSet(32),
				m_ParEdit->StyleGetCharacterSet(33));
		}

		if (msg.pSender->GetName() == _T("combox_fontsize"))
			m_ParEdit->StyleSetSize(iStyleSelect, _wtoi(m_pComFontSize->GetItemAt(m_pComFontSize->GetCurSel())->GetText()));
	}
}

int CStyleWnd::FindFont(CString szFontFamily)
{
	std::vector<CString>::iterator it = std::find(fontfamily.begin(), fontfamily.end(), szFontFamily);
	size_t index = it - fontfamily.begin();
	if (it == fontfamily.end()) return -1;
	return static_cast<int>(index);
}

int CStyleWnd::FindSize(int iFontSize)
{
	std::vector<int>::iterator it = std::find(fontsize.begin(), fontsize.end(), iFontSize);
	size_t index = it - fontsize.begin();
	if (it == fontsize.end()) return -1;
	return static_cast<int>(index);
}

int CStyleWnd::FindCharSet(CString szFontFamily)
{
	std::multimap<CString, int>::iterator iter;
	iter = map.find(szFontFamily);
	if (iter == map.end()) return -1;
	return iter->second;
}

void CStyleWnd::InitColor(CString szColorBtn)
{
	CButtonUI *pColorBtn = (CButtonUI*)m_PaintManager.FindControl(szColorBtn);

	DWORD dw = pColorBtn->GetBkColor();
	COLORREF color = ARGB2C(dw);
	m_picker.ShowWindow(SW_SHOW);
	m_picker.SetColorRGB(GetRValue(color), GetGValue(color), GetBValue(color));
	m_picker.m_szButtonChange = szColorBtn;
}

void CStyleWnd::UpDateControls()
{
	m_pBtnStyleFore->SetBkColor(C2ARGB(m_ParEdit->StyleGetFore(iStyleSelect)));
	m_pBtnStyleBack->SetBkColor(C2ARGB(m_ParEdit->StyleGetBack(iStyleSelect)));
	m_pComFontFamily->SelectItem(FindFont(m_ParEdit->StyleGetFont(iStyleSelect)));
	m_pComFontSize->SelectItem(FindSize(m_ParEdit->StyleGetSize(iStyleSelect)));
	m_pCheckBlod->SetCheck(m_ParEdit->StyleGetBold(iStyleSelect));
	m_pCheckItalic->SetCheck(m_ParEdit->StyleGetItalic(iStyleSelect));
	m_pCheckUnderline->SetCheck(m_ParEdit->StyleGetUnderline(0));
}

void CStyleWnd::OnUpdateBtnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CButtonUI *pColorBtn = (CButtonUI *)m_PaintManager.FindControl(m_picker.m_szButtonChange);
	if (!pColorBtn) return;
	pColorBtn->SetBkColor(C2ARGB(m_picker.GetColorRGB()));

	if (m_picker.m_szButtonChange == _T("btn_fore"))
		m_ParEdit->StyleSetFore(iStyleSelect, m_picker.GetColorRGB());
	if (m_picker.m_szButtonChange == _T("btn_back")) {
		m_ParEdit->StyleSetBack(iStyleSelect, m_picker.GetColorRGB());
		if(iStyleSelect == STYLE_DEFAULT)
			m_ParEdit->StyleSetBack(0, m_picker.GetColorRGB());
	}

	if ((const char*)wParam == "upcolor") {
		if(m_picker.m_szButtonChange == _T("btn_fore"))
			suinfo.cFore = m_picker.GetColorRGB();
		else
			suinfo.cBack = m_picker.GetColorRGB();
	}

	if ((const char*)wParam == "noupcolor") {
		if (m_picker.m_szButtonChange == _T("btn_fore")) {
			m_ParEdit->StyleSetFore(iStyleSelect, suinfo.cFore);
			m_pBtnStyleFore->SetBkColor(C2ARGB(suinfo.cFore));
		}
		else {
			m_ParEdit->StyleSetBack(iStyleSelect, suinfo.cBack);
			m_pBtnStyleBack->SetBkColor(C2ARGB(suinfo.cBack));
			if (iStyleSelect == STYLE_DEFAULT)
				m_ParEdit->StyleSetBack(0, suinfo.cBack);
		}
	}

}

void CStyleWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CStyleWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return FALSE;
}

LRESULT CStyleWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(m_ParEdit) 
		UpDateControls();

	bHandled = FALSE;
	return 0;
}

LRESULT CStyleWnd::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CStyleWnd::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CStyleWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (uMsg)
	{
	case UPDATE_BTN_COLOR:	OnUpdateBtnColor(uMsg, wParam, lParam, bHandled); return TRUE; break;
	}

	bHandled = FALSE;
	return FALSE;
}


DWORD CStyleWnd::C2ARGB(COLORREF cl)
{
	int r = GetRValue(cl);
	int g = GetGValue(cl);
	int b = GetBValue(cl);
	return 255 << 24 | r << 16 | g << 8 | b;
}

COLORREF CStyleWnd::ARGB2C(DWORD dw)
{
	int r = (dw >> 16) & 0x00FF;
	int g = (dw >> 8) & 0x0000FF;
	int b = dw & 0x000000FF;

	return RGB(r, g, b);
}




