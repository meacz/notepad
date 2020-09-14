
#pragma once

#include "OptionDlgPage.h"
#include "ScintillaCtrl.h"


class COpctionDlg : public WindowImplBase
{
public:

	COpctionDlg();
	~COpctionDlg();
	LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual LPCTSTR GetResourceID() const;
	void OnFinalMessage(HWND hWnd);
	UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual void InitWindow();
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void Init();

public:
	COptionDlgPage *m_OptionPage_Nor, *m_OptionPage_Col;
	CScintillaCtrl *m_ParEdit;
	CString m_FocusItem;
	void GetParentCtrl(CScintillaCtrl *pEdit) { m_ParEdit = pEdit; }
	void ShowPages();
	int m_nPageIndex;

protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
};

