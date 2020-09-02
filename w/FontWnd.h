#pragma once
#include <sstream>
#include <algorithm>
#include <map>
#include "UIFontView.h"


class CFontWnd : public WindowImplBase
{
public:
	CFontWnd();
	~CFontWnd();
	LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	void OnFinalMessage(HWND hWnd);
	UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	void InitControls();
	void GetParentFont(LOGFONT *lf);

public:
	CFontViewUI *m_fontview;
	void Init(CControlUI* pOwner);
	std::vector<LPCTSTR> font_style;
	std::vector<CString> font_size;
	static std::vector<CString> s_font;
	static std::multimap<CString,int> map;

	CEditUI* pEdit;
	CListUI* pFontList;
	CEditUI* pFontStyleEdit;
	CListUI* pFontStyleList;
	CEditUI* pFontSizeEdit;
	CListUI* pFontSizeList;
	CComboUI* PComboCharSet;
	CCheckBoxUI* pCheckBoxLine;
	CLabelUI *pLabelSample;
	CHorizontalLayoutUI *pFontView;
	BOOL bUnderline;   // pCheckBoxLine->GetCheck()  这返回值不太符合逻辑....

	LOGFONT m_lf;
	CControlUI* m_pFontOwner = NULL;
	void InitF(CControlUI* pOwner) { m_pFontOwner = pOwner; }
	void FillFontStruct();
	int OldIndex = 0;
	int GetCharSetSel(int CharSet);


protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
};

