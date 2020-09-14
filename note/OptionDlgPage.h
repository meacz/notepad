
#pragma once
#include "ScintillaCtrl.h"
#include "WTLColorMoudule.h"
#include <map>


//const CDuiString sel_bkcolor = _T("sel_bkcolor");
//const CDuiString sel_frcolor = _T("sel_frcolor");
//const CDuiString edit_bkcolor = _T("edit_bkcolor");


class COptionDlgPage : public WindowImplBase
{
public:

	COptionDlgPage();
	~COptionDlgPage();
	LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual LPCTSTR GetResourceID() const;
	void OnFinalMessage(HWND hWnd);
	UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	void OnUpdateBtnColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void Init();

public:

	CScintillaCtrl *m_ParEdit;
	DWORD C2ARGB(COLORREF cl);
	COLORREF ARGB2C(DWORD dw);
	CWTLColorPicker m_picker;
	void GetParentCtrl(CScintillaCtrl *pEdit);
	void InitColor(CString szColorBtn);

	// 颜色设置
	CButtonUI
		*pTextSelBkColor,
		*pTextSelFrColor,
		*pEditBkColor,
		*pTextFrColor,
		*pCurLineBkColor,
		*pMuiltiTextFrColor,
		*pMuiltiTextBkColor,
		*pCurColor,
		*pMultiCurColor;
	std::multimap<CString, COLORREF> mapcl;
	COLORREF ColorFromName(CString szName);



	// 常规设置
	CCheckBoxUI *pWrapLine;
	CEditUI *m_pEditPath;
	CButtonUI *m_pBtnGoTo;
	CButtonUI *m_pBtnDefSet;


	CString uPageID;
	CString szSelectedName;
	void SetCreatePageID(CString szSettingName);

protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
};

