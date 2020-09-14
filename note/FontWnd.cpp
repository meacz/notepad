#include "stdafx.h"
#include "FontWnd.h"
#include "resource.h"


CFontWnd::CFontWnd()
{
}

CFontWnd::~CFontWnd()
{
	map.clear();
	s_font.clear();
	font_size.clear();
	font_style.clear();

}

LPCTSTR CFontWnd::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CDuiString CFontWnd::GetSkinFile()
{
	return L"fontdlg.xml";
}

LPCTSTR CFontWnd::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

UILIB_RESOURCETYPE CFontWnd::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

CDuiString CFontWnd::GetSkinFolder()
{
	return _T("");
}

CControlUI* CFontWnd::CreateControl(LPCTSTR pstrClass)
{
	if (_tcscmp(pstrClass, _T("FontView")) == 0)
	{
		return new CFontViewUI;
	}
	return NULL;
}

void CFontWnd::OnExit(TNotifyUI& msg)
{
	Close();
}

void CFontWnd::Init(CControlUI* pOwner, CScintillaCtrl *sciEdit)
{
	m_ParEdit = sciEdit;
	m_pFontOwner = pOwner;
}

std::vector<CString> CFontWnd::s_font;
std::multimap<CString,int> CFontWnd::map;
INT CALLBACK NEnumFontNameProc(LOGFONT *plf, TEXTMETRIC* ptm, INT nFontType, LPARAM lParam)
{
	CFontWnd::s_font.push_back(plf->lfFaceName);
	CFontWnd::map.insert(pair<CString, int>(plf->lfFaceName, plf->lfCharSet));
	//ATLTRACE(L"%d %s\n", plf->lfCharSet, plf->lfFaceName);
	return TRUE;
}

void CFontWnd::InitControls()
{
	pEdit = (CEditUI*)m_PaintManager.FindControl(_T("f_edit"));
	pFontList = (CListUI*)m_PaintManager.FindControl(_T("f_list"));
	pFontStyleEdit = (CEditUI*)m_PaintManager.FindControl(_T("fstyle_edit"));
	pFontStyleList = (CListUI*)m_PaintManager.FindControl(_T("fstyle_list"));
	pFontSizeEdit = (CEditUI*)m_PaintManager.FindControl(_T("fsize_edit"));
	pFontSizeList = (CListUI*)m_PaintManager.FindControl(_T("fsize_list"));
	PComboCharSet = (CComboUI*)m_PaintManager.FindControl(_T("font_charset"));
	pFontView = (CHorizontalLayoutUI*)m_PaintManager.FindControl(_T("font_view"));
	pCheckBoxLine = (CCheckBoxUI*)m_PaintManager.FindControl(_T("ckbox_underline"));
	m_fontview = (CFontViewUI*)m_PaintManager.FindControl(_T("ff"));
	//m_fontview->m_uiFontLf = &m_lf;
	

	CDC dc = GetDC(NULL);
	::EnumFontFamilies(dc, (LPTSTR)NULL, (FONTENUMPROC)NEnumFontNameProc, NULL);
	CString szLabelStyle, szElementStyle, szListStyle;

		
	szLabelStyle= L"font=\"1\"";
	szElementStyle = L"padding=\"1, 1, 1, 1\" inset=\"10,0,0,0\" width=\"260\" height=\"20\"";
	szListStyle = L"itemselectedbkcolor=\"#FFD8D8D8\" itemhotbkcolor=\"#FFF4F4F4\"";

	font_style.push_back(L"常规");
	font_style.push_back(L"倾斜");
	font_style.push_back(L"粗体");
	font_style.push_back(L"粗体 倾斜");
	font_style.push_back(L"");
	font_style.push_back(L"");
	font_style.push_back(L"");
	font_style.push_back(L"");

	font_size.push_back(L"8");
	font_size.push_back(L"9");
	font_size.push_back(L"10");
	font_size.push_back(L"11");
	font_size.push_back(L"12");
	font_size.push_back(L"14");
	font_size.push_back(L"16");
	font_size.push_back(L"18");
	font_size.push_back(L"20");
	font_size.push_back(L"22");
	font_size.push_back(L"24");
	font_size.push_back(L"26");
	font_size.push_back(L"28");
	font_size.push_back(L"36");
	font_size.push_back(L"48");
	font_size.push_back(L"72");

	sort(s_font.begin(), s_font.end());
	for (size_t i = 0; i < s_font.size(); i++)
	{
		CListContainerElementUI *pElement = new CListContainerElementUI;
		CLabelUI *pLabel = new CLabelUI;

		pLabel->SetText(s_font[i]);
		pLabel->SetToolTip(s_font[i]);
		pElement->SetToolTip(s_font[i]);
		pElement->SetText(s_font[i]);
		pElement->Add(pLabel);
		pFontList->Add(pElement);

		pLabel->SetAttributeList(szLabelStyle);
		pElement->SetAttributeList(szElementStyle);
		pFontList->SetAttributeList(szListStyle);
	}


	for (size_t i = 0; i < font_style.size(); i++)
	{
		CListContainerElementUI *pElement = new CListContainerElementUI;
		CLabelUI *pLabel = new CLabelUI;

		pLabel->SetText(font_style[i]);
		pElement->Add(pLabel);
		pFontStyleList->Add(pElement);

		pLabel->SetAttributeList(szLabelStyle);
		pElement->SetAttributeList(szElementStyle);
		pFontStyleList->SetAttributeList(szListStyle);
	}

	for (size_t i = 0; i < font_size.size(); i++)
	{
		CListContainerElementUI *pElement = new CListContainerElementUI;
		CLabelUI *pLabel = new CLabelUI;

		pLabel->SetText(font_size[i]);
		pElement->Add(pLabel);
		pFontSizeList->Add(pElement);

		pLabel->SetAttributeList(szLabelStyle);
		pElement->SetAttributeList(szElementStyle);
		pFontSizeList->SetAttributeList(szListStyle);
	}

	size_t nIndex = -1;
	std::vector<CString>::iterator it = std::find(s_font.begin(), s_font.end(), m_ParEdit->StyleGetFont(STYLE_DEFAULT));
	if(it != s_font.end()) nIndex = it - s_font.begin();


	CString str;
	size_t nIndex2 = -1;
	str.Format(L"%d", m_ParEdit->StyleGetSize(STYLE_DEFAULT));
	std::vector<CString>::iterator it2 = std::find(font_size.begin(), font_size.end(), str);
	if (it2 != font_size.end()) nIndex2 = it2 - font_size.begin();
	
	int nItalicSel = 0;
	int nCharsetSel = 0;


	if (m_ParEdit->StyleGetItalic(STYLE_DEFAULT))
		nItalicSel = 1;
	if (m_ParEdit->StyleGetBold(STYLE_DEFAULT))
		nItalicSel = 2;
	if (m_ParEdit->StyleGetItalic(STYLE_DEFAULT) && 
		m_ParEdit->StyleGetBold(STYLE_DEFAULT))
		nItalicSel = 3;


	pFontList->SelectItem(nIndex);
	pFontSizeList->SelectItem(nIndex2);
	if (nIndex2 == -1) pFontSizeEdit->SetText(str);
	pFontSizeEdit->SetText(str);
	pCheckBoxLine->SetCheck(m_ParEdit->StyleGetUnderline(0));
	pFontStyleList->SelectItem(nItalicSel);

}

void CFontWnd::GetParentFont(LOGFONT *lf) {

	m_lf = *lf;
	StrCpyW(m_lf.lfFaceName, lf->lfFaceName);
}


void CFontWnd::Notify(TNotifyUI& msg)
{
	//ATLTRACE(L" %s \n", msg.sType);

	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn"))
			OnExit(msg);

		// 转发到父窗口
		if (msg.pSender->GetName() == _T("btn_ok")) {
			m_pFontOwner->GetManager()->SendNotify(m_pFontOwner, _T("btn_ok"), 0, 0, true);
			OnExit(msg);
		}
		if (msg.pSender->GetName() == _T("btn_cancel")) {
			m_pFontOwner->GetManager()->SendNotify(m_pFontOwner, _T("btn_cancel"), 0, 0, true);
			OnExit(msg);
		}
	}
	if (msg.sType == _T("selectchanged")) {
		if (msg.pSender->GetName() == _T("ckbox_underline")) {
			FillFontStruct();
		}
	}

	if (msg.sType == _T("itemselect")){
		if (msg.pSender->GetName() == _T("f_list")) {
			int nindex = pFontList->GetCurSel();
			CString str = pFontList->GetItemAt(nindex)->GetText().GetData();
			pEdit->SetText(str);
			std::multimap<CString, int>::iterator iter;
			iter = map.find(s_font[nindex]);
			m_lf.lfCharSet = iter->second;
			int nCharsetSel = GetCharSetSel(m_lf.lfCharSet);
			PComboCharSet->SelectItem(nCharsetSel);
			FillFontStruct();
		}

		if (msg.pSender->GetName() == _T("fstyle_list")) {
			int nindex2 = pFontStyleList->GetCurSel();
			if (nindex2 >= 0)
				pFontStyleEdit->SetText(font_style[nindex2]);
			FillFontStruct();
		}

		if (msg.pSender->GetName() == _T("fsize_list")) {
			int nindex3 = pFontSizeList->GetCurSel();
			if (nindex3 >= 0)
				pFontSizeEdit->SetText(font_size[nindex3]);
			FillFontStruct();
		}

		if (msg.pSender->GetName() == _T("font_charset")) {
			int fontCharSet = 0;
			switch (PComboCharSet->GetCurSel())
			{
			case 0: 	fontCharSet = ANSI_CHARSET; break;
			case 1: 	fontCharSet = DEFAULT_CHARSET; break;
			case 2: 	fontCharSet = SYMBOL_CHARSET; break;
			case 3: 	fontCharSet = MAC_CHARSET; break;
			case 4: 	fontCharSet = SHIFTJIS_CHARSET; break;
			case 5: 	fontCharSet = HANGUL_CHARSET; break;
			case 6: 	fontCharSet = CHINESEBIG5_CHARSET; break;
			case 7: 	fontCharSet = GB2312_CHARSET; break;
			case 8: 	fontCharSet = GREEK_CHARSET; break;
			case 9: 	fontCharSet = TURKISH_CHARSET; break;
			case 10: 	fontCharSet = VIETNAMESE_CHARSET; break;
			case 11: 	fontCharSet = HEBREW_CHARSET; break;
			case 12: 	fontCharSet = ARABIC_CHARSET; break;
			case 13: 	fontCharSet = BALTIC_CHARSET; break;
			case 14: 	fontCharSet = RUSSIAN_CHARSET; break;
			case 15: 	fontCharSet = THAI_CHARSET; break;
			case 16: 	fontCharSet = EASTEUROPE_CHARSET; break;
			case 17: 	fontCharSet = OEM_CHARSET; break;
			}
			m_lf.lfCharSet = fontCharSet;
			FillFontStruct();
		}
	}
	if (msg.sType == _T("return")) {
		if (msg.pSender->GetName() == _T("f_edit")) {
			size_t nIndex = -1;
			std::vector<CString>::iterator it = std::find(s_font.begin(), s_font.end(), pEdit->GetText().GetData());
			if(it != s_font.end()) nIndex = it - s_font.begin();
			pFontList->SelectItem(nIndex);
			FillFontStruct();
		}

		if (msg.pSender->GetName() == _T("fsize_edit")) {
			size_t nIndex = -1;
			std::vector<CString>::iterator it = std::find(font_size.begin(), font_size.end(), pFontSizeEdit->GetText().GetData());
			if(it != font_size.end()) nIndex = it - font_size.begin();
			pFontSizeList->SelectItem(nIndex);
			FillFontStruct();
		}
	}
}

void CFontWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}


void CFontWnd::FillFontStruct()
{
	CDC dc = GetDC(NULL);

	int nindex = pFontList->GetCurSel();
	int fontsize = -(int)(_wtoi(pFontSizeEdit->GetText()) * GetDeviceCaps(dc, LOGPIXELSY) / 72 + 0.5);

	
	BOOL bBord = 400;
	BOOL bItalic = FALSE;


	int selStyle = pFontStyleList->GetCurSel();
	switch (selStyle)
	{
	case 0:
		bBord = 400;
		bItalic = FALSE;
		break;
	case 1:
		bItalic = TRUE;
		break;
	case 2:
		bBord = 900;
		break;
	case 3:
		bBord = 900;
		bItalic = TRUE;
		break;
	}

	m_lf.lfWeight = bBord;
	m_lf.lfItalic = bItalic;
	m_lf.lfUnderline = pCheckBoxLine->GetCheck();
	m_lf.lfHeight = fontsize;
	StrCpyW(m_lf.lfFaceName, s_font[nindex]);

	m_fontview->m_uiFontLf = m_lf;
	m_fontview->Invalidate();
}


LRESULT CFontWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = FALSE;

	bHandled = FALSE;
	return lRes;
}

int CFontWnd::GetCharSetSel(int CharSet)
{

	int nCharsetSel = 0;
	switch (m_lf.lfCharSet)
	{
	case ANSI_CHARSET:			nCharsetSel = 0; break;
	case DEFAULT_CHARSET:		nCharsetSel = 1; break;
	case SYMBOL_CHARSET:		nCharsetSel = 2; break;
	case MAC_CHARSET:			nCharsetSel = 3; break;
	case SHIFTJIS_CHARSET:		nCharsetSel = 4; break;
	case HANGUL_CHARSET:		nCharsetSel = 5; break;
	case JOHAB_CHARSET:			nCharsetSel = 6; break;
	case GB2312_CHARSET:		nCharsetSel = 7; break;
	case CHINESEBIG5_CHARSET:	nCharsetSel = 8; break;
	case GREEK_CHARSET:			nCharsetSel = 9; break;
	case TURKISH_CHARSET:		nCharsetSel = 10; break;
	case VIETNAMESE_CHARSET:	nCharsetSel = 11; break;
	case HEBREW_CHARSET:		nCharsetSel = 12; break;
	case ARABIC_CHARSET:		nCharsetSel = 13; break;
	case BALTIC_CHARSET:		nCharsetSel = 14; break;
	case RUSSIAN_CHARSET:		nCharsetSel = 15; break;
	case THAI_CHARSET:			nCharsetSel = 16; break;
	case EASTEUROPE_CHARSET:	nCharsetSel = 17; break;
	case OEM_CHARSET:			nCharsetSel = 18; break;
	}
	return nCharsetSel;
}