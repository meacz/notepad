#pragma once
#include "ScintillaCtrl.h"
#include "MessageBox.h"


class CFindDlg : public WindowImplBase
{
public:

	CFindDlg();
	~CFindDlg();
	LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual LPCTSTR GetResourceID() const;
	void OnFinalMessage(HWND hWnd);
	UILIB_RESOURCETYPE GetResourceType() const;
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual void InitWindow();


public:
	
	void Init(CScintillaCtrl* pSciEdit, CString szSelText);
	CControlUI* m_pOwner = NULL;
	CScintillaCtrl *m_ParEdit;
	COptionUI *m_pFindUp;
	CCheckBoxUI *m_pFindUpperBox;
	CEditUI *m_FindEdit;
	void Find();
	CString m_ParEditSelText;
	CMessageBox MsgBox;

protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
};

