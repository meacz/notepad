#pragma once



class CAboutDlg : public WindowImplBase
{
public:

	CAboutDlg();
	~CAboutDlg();
	LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	LPCTSTR GetResourceID() const;
	void OnFinalMessage(HWND hWnd);
	UILIB_RESOURCETYPE GetResourceType() const;
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();

public:


protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
};

