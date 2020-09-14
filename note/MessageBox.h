#pragma once

class CMessageBox : public WindowImplBase
{
public:
	CMessageBox();
	~CMessageBox();
	LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual LPCTSTR GetResourceID() const;
	void OnFinalMessage(HWND hWnd);
	UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();

	CString uStyleSet;
	CLabelUI *m_pLabel, *m_pLabelMin;
	void SetBoxStyle(int nFlag);
	UINT operator() (HWND hwnd, CString szText, UINT uFlags);


protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
};


