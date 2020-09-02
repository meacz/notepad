
#pragma once
#include "ScintillaCtrl.h"
#include "WTLColorMoudule.h"


class COptionDlgPage : public WindowImplBase
{
public:

	COptionDlgPage();
	~COptionDlgPage();
	LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	void OnFinalMessage(HWND hWnd);
	UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	void OnUpdateBtnColor();
	void Init();

public:

	CScintillaCtrl *m_ParEdit;
	DWORD C2ARGB(COLORREF cl);
	COLORREF ARGB2C(DWORD dw);
	CWTLColorPicker m_picker;
	void GetParentCtrl(CScintillaCtrl *pEdit);
	void InitColor(CString szColorBtn);

	// 常规设置
	CButtonUI *pFont_FrColor;
	CButtonUI *pFont_BkColor;
	CButtonUI *pText_SelBkColor;
	CButtonUI *pText_SelFrColor;
	CCheckBoxUI *pWrapLine;
	BOOL bWrapLine;

	// 配置文件
	CEditUI *m_pEditPath;
	CButtonUI *m_pBtnGoTo;
	CButtonUI *m_pBtnDefSet;


	UINT uPageID;
	CString szSelectedName;
	void SetCreatePageID(CString szSettingName);

protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
};

