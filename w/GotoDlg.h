#pragma once

#include "ScintillaCtrl.h"

class CGotoDlg : public WindowImplBase
{
public:
	CGotoDlg();
	~CGotoDlg();
	LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	void OnFinalMessage(HWND hWnd);
	UILIB_RESOURCETYPE GetResourceType() const;
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled);
	virtual void InitWindow();


public:
	CScintillaCtrl *m_ParentEdit;
	CLabelUI* m_pLineCountText;
	CEditUI* m_pLineNumEdit;
	CCheckBoxUI *m_pCloseChek;
	int m_nLineCount;
	void SetLineInfo();
	void Init(CScintillaCtrl* pSciEdit);

protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
};

