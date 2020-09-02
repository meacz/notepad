#pragma once

#include "ScintillaCtrl.h"


class CReplaceDlg : public WindowImplBase
{
public:
	CReplaceDlg();
	~CReplaceDlg();

	LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	void OnFinalMessage(HWND hWnd);
	UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();

public:
	void FindReplace(UINT nFlag);
	void Init(CScintillaCtrl* pSciEdit, CString szSelText);
	int newPos = 0;
	CScintillaCtrl *m_ParEditRep;
	CEditUI *m_pRFindEdit, *m_pRepEdit;
	CCheckBoxUI *m_pRFindUpperBox;
	CString m_RParEditSelText;


protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
};

