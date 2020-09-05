#include "stdafx.h"
#include "resource.h"


#define HM_KEY2		WM_USER + 1011

class CMenuWnd : public WindowImplBase
{
public:
	CMenuWnd();
	~CMenuWnd();
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
	void Init(CControlUI* pOwner);
	CControlUI* m_pOwner = NULL;
	UINT uMenuID = 0;
	void SetMenuID(UINT nId){ uMenuID = nId; }
	HCURSOR m_MenuCur;
	void ItemSelectFalse();

protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);
};

