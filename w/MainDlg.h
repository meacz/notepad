// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "ScintillaCtrl.h"

#define  M_INI_PATH  _T("\\notepad.ini")


class CMainDlg : public CDialogImpl<CMainDlg>, public CUpdateUI<CMainDlg>,
		public CMessageFilter, public CIdleHandler
{
public:
	CMainDlg();
	~CMainDlg();
	enum { IDD = IDD_MAINDLG };

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnIdle();

	BEGIN_UPDATE_UI_MAP(CMainDlg)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(MSG_REMOVE_DEFAULTSET, OnRemoveDefaultSet)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		MSG_WM_DROPFILES(OnDropFiles)
	END_MSG_MAP()

	
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnRemoveDefaultSet(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	void OnDropFiles(HDROP hDropInfo);
	void CloseDialog(int nVal);


public:
	void OnFileNew();
	void OnFileOpen();
	void OnFileSave();
	void OnEditCopy();
	void OnEditDate();
	void OnEditUndo();
	void OnEditCut();
	void OnEditPaste();
	void OnEditNext();
	void OnEditReplace();
	void OnEditFind();
	void OnEditSelectAll();
	void OnEditGo();

public:
	CScintillaCtrl m_sciEdit;
	BOOL GetResourceSetting(UINT uID, CString szType, CString szOutPath);
	void SetFontInfo(LOGFONT & lf);
	LOGFONT m_lfont;
	BOOL WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue, LPCTSTR m_pszProfileName);
	BOOL m_bRemoveSet;

};
