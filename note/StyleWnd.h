
#pragma once
#include "ScintillaCtrl.h"
#include "UIGroupBox.h"
#include <map>
#include <algorithm>
#include "WTLColorMoudule.h"


struct StyleInfo
{
	CString szFontFamily;
	int iFontSize;
	COLORREF cFore, cBack;
	BOOL bblod, bitalic, bunderline;
};


class CStyleWnd : public WindowImplBase
{
public:
	CStyleWnd();
	~CStyleWnd();
	LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual LPCTSTR GetResourceID() const;
	void OnFinalMessage(HWND hWnd);
	UILIB_RESOURCETYPE GetResourceType() const;
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual void InitWindow();
	CControlUI* CreateControl(LPCTSTR pstrClass);
	void OnUpdateBtnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

public:

	static std::vector<CString> fontfamily;
	static std::multimap<CString, int> map;
	std::vector<int> fontsize;

	CScintillaCtrl *m_ParEdit;
	CWTLColorPicker m_picker;
	void Init(CScintillaCtrl* pSciEdit);
	int FindFont(CString szFontFamily);
	int FindSize(int iFontSize);
	int FindCharSet(CString szFontFamily);
	void InitColor(CString szColorBtn);
	void UpDateControls();
	DWORD C2ARGB(COLORREF cl);
	COLORREF ARGB2C(DWORD dw);

	int iStyleSelect;
	CListUI *m_pListStyle;
	CComboUI *m_pComFontFamily, *m_pComFontSize;
	CButtonUI *m_pBtnStyleFore, *m_pBtnStyleBack;
	CCheckBoxUI *m_pCheckBlod, *m_pCheckItalic, *m_pCheckUnderline;

	StyleInfo suinfo;

protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
};

