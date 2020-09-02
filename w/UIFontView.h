#pragma once

class CFontViewUI :public CContainerUI
{
public:
	CFontViewUI();

	~CFontViewUI();

	virtual LPCTSTR GetClass() const;
	virtual LPVOID GetInterface(LPCTSTR pstrName);
	void PaintText(HDC hDC);
	void PaintBorder(HDC hDC);
	void SetDrawFont(LOGFONT lf);
	LOGFONT m_uiFontLf;
	CString szFontName;
	//CFont m_font;

private:
};

