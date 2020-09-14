
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <atlconv.h>
#include <string>
#include <io.h>
#include <fcntl.h>
#include <sstream>
#include <locale>
#include <codecvt>
#include <map>
#include "MenuWnd.h"
#include "ScintillaCtrl.h"
#include "FontWnd.h"
#include "FindDlg.h"
#include "ReplaceDlg.h"
#include "GotoDlg.h"
#include "OpctionDlg.h"
#include "AboutDlg.h"
#include "OptionDlgPage.h"
#include "StyleWnd.h"
#include "MessageBox.h"


#define  M_INI_PATH  _T("\\notepad.ini")
#define ENCODING_ASCII	   0
#define ENCODING_UTF8	   1
#define ENCODING_UTF16LE   2
#define ENCODING_UTF16BE   3

const int WM_FINDREPLACE = ::RegisterWindowMessage(FINDMSGSTRING);


class CDuiFrameWnd : public WindowImplBase
{
public:

	CDuiFrameWnd();
	~CDuiFrameWnd();

public:
	inline BOOL GetModify()
	{
		return m_childEdit->GetModify();
	}

	inline void Setmodify()
	{
		m_childEdit->SetSavePoint();
	}

	void OnFinalMessage(HWND hWnd);
	UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetWindowClassName() const;
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual LPCTSTR GetResourceID() const;
	virtual LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual void InitWindow();
	virtual LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	CMessageBox MsgBox;
public:

	void OnFormatFont();
	void OnFileNew();
	void OnFileOpen();
	void OnFileSaveAs();
	void OnFileSave();
	void OnFileExit();
	void OnEditUndo();
	void OnEditCut();
	void OnEditPaste();
	void OnEditNext();
	void OnEditReplace();
	void OnEditFind();
	void OnEditChangeLine();
	void OnEditSelectAll();
	void OnEditGo();
	void OnEditCopy();
	void OnEditDate();
	void OnEditClear();
	void OnEditStyle();
	void OnAboutDlg();
	void OnPrint();
	void OnSetting();
	void OnHelp();
	void OpenFileFromCommandLine(CString szPath);
	void OnChildWindowInit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void OnKillMenuFocus();

public:

	enum MENUINFO
	{
		FILE_PADDING = 0,
		EDIT_PADDING = 60 * 1,
		FMAT_PADDING = 60 * 2,
		LOOK_PADDING = 60 * 3,
		HELP_PADDING = 60 * 4,

		FILE_HEIGHT = 26 * 7,
		EDIT_HEIGHT = 26 * 11,
		FMAT_HEIGHT = 26 * 2,
		LOOK_HEIGHT = 26 * 2,
		HELP_HEIGHT = 26 * 3
	};

	int listHeaderPadding;
	int menuItemHeight;
	CString nItemID;

public:
	CFontWnd *m_FontDlg, *m_pFontWnd;
	CFindDlg *m_FindDlg;
	CStyleWnd *m_StyleDlg, *m_pStyleWnd;
	CReplaceDlg *m_ReplaceDlg;
	CGotoDlg *m_SkipDlg;
	COpctionDlg *m_OpctionDlg;
	CAboutDlg *m_AboutDlg;
	CLabelUI *pTitle;
	CString m_szFile;

	std::map<CString, CMenuWnd*> mpage;
	std::map<CString, CRect> mrect;
	CMenuWnd *m_pMenuFile, *m_pMenuEdit, *m_pMenuFmat, *m_pMenuLook, *m_pMenuHelp;
	CListHeaderItemUI *pItem1;
	CListHeaderItemUI *pItem2;
	CListHeaderItemUI *pItem3;
	CListHeaderItemUI *pItem4;
	CListHeaderItemUI *pItem5;
	BOOL m_bfocus;
	void ShowMenuPage(CString szMenu);
	void HideMenuPage();


	int encoding;
	void SetTitle();
	void ReadFile(std::string path);
	void SetMenuInfo(CString lpszType);
	void SetFontInfo(LOGFONT & lf);
	BOOL CheckModify();
	BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry,
		int nValue, LPCTSTR m_pszProfileName);
	CFont m_font;
	LOGFONT m_lfont, *m_lfEdit;
	void CopyFontStruct(LOGFONT lf);

	CScintillaCtrl *m_childEdit;
	HWND m_hWndChild, m_hWndChildEdit;
	BOOL GetResourceSetting(UINT uID, CString szType, CString szOutPath);

protected:

	void OnExit(TNotifyUI& msg);
	void Notify(TNotifyUI& msg);

public:

	std::string to_utf8(const wchar_t* buffer, int len);
	std::string to_utf8(const std::wstring& str);

};