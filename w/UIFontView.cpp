#include "stdafx.h"
#include "UIFontView.h"

CFontViewUI::CFontViewUI()
{
	szFontName = L"";
}


CFontViewUI::~CFontViewUI()
{
	memset(&m_uiFontLf, 0, sizeof(LOGFONT));

}

//	@return LPCTSTR 返回控件类名
//	@note	本函数返回控件类，格式为LPCTSTR
LPCTSTR CFontViewUI::GetClass() const
{
	return TEXT("");
}


// @return LPVOID类型
//	@note 获取接口
LPVOID CFontViewUI::GetInterface(LPCTSTR pstrName)
{
	if (_tcscmp(pstrName, TEXT("FontView")) == 0)
	{
		return static_cast<CFontViewUI*>(this);
	}

	return CContainerUI::GetInterface(pstrName);
}



void CFontViewUI::PaintText(HDC hDC)
{

}


void CFontViewUI::PaintBorder(HDC hDC)
{
	CDCHandle dc = hDC;
	CRect rect(GetPos());

	CFont m_font;
	//m_font.CreateFont(m_uiFontLf.lfHeight,
	//	m_uiFontLf.lfWidth,
	//	m_uiFontLf.lfEscapement,
	//	m_uiFontLf.lfOrientation,
	//	m_uiFontLf.lfWeight,
	//	m_uiFontLf.lfItalic,
	//	m_uiFontLf.lfUnderline,
	//	m_uiFontLf.lfStrikeOut,
	//	m_uiFontLf.lfCharSet,
	//	m_uiFontLf.lfOutPrecision,
	//	m_uiFontLf.lfClipPrecision,
	//	m_uiFontLf.lfQuality,
	//	m_uiFontLf.lfPitchAndFamily,
	//	szFontName);

	m_font.CreateFontIndirect(&m_uiFontLf);
	HFONT font = dc.SelectFont(m_font);
	dc.DrawText(_T("字体Abc"), -1, rect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	
	dc.SelectFont(font);
	m_font.DeleteObject();
}

void CFontViewUI::SetDrawFont(LOGFONT lf)
{
}
