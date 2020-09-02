// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "aboutdlg.h"
#include "MainDlg.h"
#include <time.h>

CMainDlg::CMainDlg()
{
	m_bRemoveSet = TRUE;
}

CMainDlg::~CMainDlg()
{
	PostQuitMessage(0);
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->hwnd == m_sciEdit.m_hWnd)
	{
		if (GetKeyState(VK_CONTROL) & 0x80 && (char)pMsg->wParam == 'N')
				{ OnFileNew(); return TRUE; }			// �½�
		if (GetKeyState(VK_CONTROL) & 0x80 && (char)pMsg->wParam == 'O')	
				{ OnFileOpen(); return TRUE; }			// ��
		if (GetKeyState(VK_CONTROL) & 0x80 && (char)pMsg->wParam == 'S')	
				{ OnFileSave(); return TRUE; }			// ����
		if (GetKeyState(VK_CONTROL) & 0x80 && (char)pMsg->wParam == 'A')	
				{ OnEditSelectAll(); return TRUE; }		// ȫѡ
		if (GetKeyState(VK_CONTROL) & 0x80 && (char)pMsg->wParam == 'H')
				{ OnEditReplace(); return TRUE; }		// �滻
		if (GetKeyState(VK_CONTROL) & 0x80 && (char)pMsg->wParam == 'F')
				{ OnEditFind(); return TRUE; }			// ����
		if (GetKeyState(VK_CONTROL) & 0x80 && (char)pMsg->wParam == 'C')
				{ OnEditCopy(); return TRUE; }			// ����
		if (GetKeyState(VK_CONTROL) & 0x80 && (char)pMsg->wParam == 'X')
				{ OnEditCut(); return TRUE; }			// ����
		if (GetKeyState(VK_CONTROL) & 0x80 && (char)pMsg->wParam == 'G')
				{ OnEditGo(); return TRUE; }			// ��ת
		if (GetKeyState(VK_F5) & 0x80)
				{ OnEditDate(); return TRUE; }			// ʱ��
		if (GetKeyState(VK_F3) & 0x80)
				{ OnEditNext(); return TRUE; }			// ������һ��
		if (GetKeyState(VK_CONTROL) & 0x80 && GetKeyState(VK_DOWN) & 0x80)
		{
			m_sciEdit.MoveSelectedLinesDown();			// ����ǰ�������ƶ�
			int nCharPos = m_sciEdit.GetCurrentPos();
			int nLinePos = m_sciEdit.LineFromPosition(nCharPos);
			int charPos  = m_sciEdit.PositionFromLine(nLinePos);
			m_sciEdit.SetSel(charPos, charPos);
			return TRUE;
		}
		if (GetKeyState(VK_CONTROL) & 0x80 && GetKeyState(VK_UP) & 0x80)
		{												// ����ǰ�������ƶ�
			m_sciEdit.MoveSelectedLinesUp();
			int nCharPos = m_sciEdit.GetCurrentPos();
			int nLinePos = m_sciEdit.LineFromPosition(nCharPos);
			int charPos = m_sciEdit.PositionFromLine(nLinePos);
			m_sciEdit.SetSel(charPos, charPos);
			return TRUE;
		}
	}

	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CRect rect, rt;
	::GetWindowRect(GetParent(), &rect);
	ScreenToClient(&rect);
	rect.left += 2;
	rect.top += 57;  // ���������˵����߶�
	rect.right -= 2;
	rect.bottom -= 2;
	MoveWindow(rect);
	

	GetWindowRect(rt);
	SetRect(rt, 0, 0, rt.Width(), rt.Height());
	m_sciEdit.Create(
		0, L"Scintilla", L"",
		WS_CHILD   |
		WS_VISIBLE |
		WS_TABSTOP |
		WS_CLIPCHILDREN,
		rt, m_hWnd);


	USES_CONVERSION;
	FILE *file = fopen(W2A(SaveIniFilePath() + M_INI_PATH), "r");
	if (!file)
		GetResourceSetting(IDR_TXT1, _T("txt"), SaveIniFilePath() + M_INI_PATH);

	// ��ȡ����
	m_lfont = { 0 };
	GetPrivateProfileStructW(_T("Font"), _T("edit font"), (LPVOID)&m_lfont, sizeof(LOGFONT), SaveIniFilePath() + M_INI_PATH);
	CFont m_font;
	m_font.CreateFontIndirect(&m_lfont);
	SetFontInfo(m_lfont);

	// ��ɫ
	COLORREF clTextFore, clTextBack, clSelFore, clSelBack;
	//GetPrivateProfileStructW(L"Color", L"fore_color",
	//	(LPVOID)&clTextFore, sizeof(COLORREF), M_INI_PATH);
	//GetPrivateProfileStructW(L"Color", L"back_color",
	//	(LPVOID)&clTextBack, sizeof(COLORREF), M_INI_PATH);
	//m_sciEdit.StyleSetFore(STYLE_DEFAULT, clTextFore);
	//m_sciEdit.StyleSetBack(STYLE_DEFAULT, clTextBack);

	GetPrivateProfileStructW(L"Color", L"sel_fore",
		(LPVOID)&clSelFore, sizeof(COLORREF), SaveIniFilePath() + M_INI_PATH);
	GetPrivateProfileStructW(L"Color", L"sel_back",
		(LPVOID)&clSelBack, sizeof(COLORREF), SaveIniFilePath() + M_INI_PATH);
	m_sciEdit.SetSelFore(TRUE, clSelFore);
	m_sciEdit.SetSelBack(TRUE, clSelBack);
	m_sciEdit.SetSelAlpha(125);

	BOOL bWrapLine = GetPrivateProfileInt(TEXT("WrapMode"), TEXT("wrap_line"), 0, SaveIniFilePath() + M_INI_PATH);
	m_sciEdit.SetWrapMode(bWrapLine);   // �Զ�������ʾ��ֱ������
	m_sciEdit.SetVScrollBar(bWrapLine);
	m_sciEdit.SetHScrollBar(!bWrapLine);
	m_sciEdit.SetMarginWidthN(TRUE, 0);
	m_sciEdit.SetCaretLineVisibleAlways(TRUE);
	m_sciEdit.ShowWindow(SW_SHOW);



	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);


	if (!m_bRemoveSet) return 0;
	
	CRect rect;
	::GetWindowRect(GetParent(), rect);
	// RECT->��������  Left->�Ⱥ���ֵ  rect.left->�Ⱥ���ֵ
	WriteProfileInt(TEXT("RECT"), TEXT("Left"), rect.left, SaveIniFilePath() + M_INI_PATH);
	WriteProfileInt(TEXT("RECT"), TEXT("Right"), rect.right, SaveIniFilePath() + M_INI_PATH);
	WriteProfileInt(TEXT("RECT"), TEXT("Top"), rect.top, SaveIniFilePath() + M_INI_PATH);
	WriteProfileInt(TEXT("RECT"), TEXT("Bottom"), rect.bottom, SaveIniFilePath() + M_INI_PATH);
	WritePrivateProfileStructW(L"Font", L"edit font",
		(LPVOID)&m_lfont, sizeof(m_lfont), SaveIniFilePath() + M_INI_PATH);

	//WritePrivateProfileStructW(L"Color", L"fore_color", 
	//	(LPVOID)m_childEdit->StyleGetFore(STYLE_DEFAULT), sizeof(COLORREF), M_INI_PATH);
	//WritePrivateProfileStructW(L"Color", L"back_color",
	//	(LPVOID)m_childEdit->StyleGetBack(STYLE_DEFAULT), sizeof(COLORREF), M_INI_PATH);


	COLORREF clselback = m_sciEdit.GetSelBack();
	COLORREF clselfore = m_sciEdit.GetSelFore();
	BOOL bWrapLine = m_sciEdit.GetWrapMode();
	WritePrivateProfileStructW(L"Color", L"sel_back", (LPVOID)&clselback, sizeof(COLORREF), SaveIniFilePath() + M_INI_PATH);
	WritePrivateProfileStructW(L"Color", L"sel_fore", (LPVOID)&clselfore, sizeof(COLORREF), SaveIniFilePath() + M_INI_PATH);
	WriteProfileInt(TEXT("WrapMode"), TEXT("wrap_line"), bWrapLine, SaveIniFilePath() + M_INI_PATH);

	return 0;
}

LRESULT CMainDlg::OnRemoveDefaultSet(UINT, WPARAM, LPARAM, BOOL &)
{

	CString str = SaveIniFilePath() + M_INI_PATH;
	str.Replace(L"\\", L"\\\\");
	DeleteFile(str);
	GetResourceSetting(IDR_TXT1, _T("txt"), SaveIniFilePath() + M_INI_PATH);

	m_bRemoveSet = FALSE;  // ��д���������

	return TRUE;
}


LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}


void CMainDlg::OnDropFiles(HDROP hDropInfo)
{
	::SendMessage(GetParent(), WM_DROPFILES, (WPARAM)hDropInfo, 0);
}

// �½�
void CMainDlg::OnFileNew()
{
	::SendMessage(GetParent(), MSG_MENU_NEW, 0, 0);
}

// ��
void CMainDlg::OnFileOpen()
{
	::SendMessage(GetParent(), MSG_MENU_OPEN, 0, 0);
}

// ���Ϊ
void CMainDlg::OnFileSave()
{
	::SendMessage(GetParent(), MSG_MENU_SAVE, 0, 0);
}

// ����
void CMainDlg::OnEditCopy()
{
	m_sciEdit.CopyAllowLine();
}

// ʱ��/����
void CMainDlg::OnEditDate()
{
	time_t rawtime;
	tm *ptminfo;
	time(&rawtime);
	ptminfo = localtime(&rawtime);
	CString str;
	str.Format(L"%02d-%02d-%02d %02d:%02d:%02d",
		ptminfo->tm_year + 1900,
		ptminfo->tm_mon + 1,
		ptminfo->tm_mday,
		ptminfo->tm_hour,
		ptminfo->tm_min,
		ptminfo->tm_sec);
	USES_CONVERSION;
	m_sciEdit.ReplaceSel(W2A(str));
}

// ����
void CMainDlg::OnEditUndo()
{
	m_sciEdit.Undo();
}

// ����
void CMainDlg::OnEditCut()
{
	int nCharPos = m_sciEdit.GetCurrentPos();				// ��������ַ�λ��
	int nLinePos = m_sciEdit.LineFromPosition(nCharPos);	// ���������
	int charPos = m_sciEdit.PositionFromLine(nLinePos);		// ��ǰ�е�һ���ַ�λ��
	int nLen = m_sciEdit.LineLength(nLinePos);				// �г���
	int selStart = m_sciEdit.GetSelectionStart();			// ѡ���ı��Ŀ�ʼλ��
	int selEnd = m_sciEdit.GetSelectionEnd();

	m_sciEdit.CopyAllowLine();
	if (selStart == selEnd)
		m_sciEdit.DeleteRange(charPos, nLen);
	else
		m_sciEdit.Cut();

}

// ճ��
void CMainDlg::OnEditPaste()
{
	m_sciEdit.Paste();
}

// ������һ��
void CMainDlg::OnEditNext()
{
	OnEditFind();
}

// �滻
void CMainDlg::OnEditReplace()
{
	::SendMessage(GetParent(), MSG_MENU_REPLACE, 0, 0);
}

// ����
void CMainDlg::OnEditFind()
{
	::SendMessage(GetParent(), MSG_MENU_FIND, 0, 0);
}

// ��ת
void CMainDlg::OnEditGo()
{
	::SendMessage(GetParent(), MSG_MENU_GOTOLINE, 0, 0);
}

// ȫѡ
void CMainDlg::OnEditSelectAll()
{
	m_sciEdit.SelectAll();
}



// ��ȡ��Դ�е������ļ�
BOOL CMainDlg::GetResourceSetting(UINT uID, CString szType, CString szOutPath)
{
	HMODULE hModule = GetModuleHandle(NULL);
	HRSRC hRes = FindResource(hModule, MAKEINTRESOURCE(uID), szType);
	if (hRes == NULL)
	{
		FreeResource(hRes);
		return FALSE;
	}

	HGLOBAL hglobal = LoadResource(hModule, hRes);
	if (hglobal == NULL)
	{
		FreeResource(hglobal);
		return FALSE;
	}

	//�ͷ��ļ�
	LPBYTE lpByte = (LPBYTE)LockResource(hglobal);
	DWORD dwRcSize = SizeofResource(hModule, hRes);
	FILE* outFile = fopen(CT2A(szOutPath), "w+");
	fprintf(outFile, "%s", lpByte);   // output format ansi
	fclose(outFile);
	return TRUE;
}

void CMainDlg::SetFontInfo(LOGFONT & lf)
{
	CDC dc = GetDC();
	BOOL bBord = lf.lfWeight == 400 ? FALSE : TRUE;
	int fontSize = (int)(-lf.lfHeight*72.0 / GetDeviceCaps(dc, LOGPIXELSY) + 0.5);

	m_sciEdit.StyleSetFont(STYLE_DEFAULT, lf.lfFaceName);
	m_sciEdit.StyleSetSize(STYLE_DEFAULT, fontSize);
	m_sciEdit.StyleSetItalic(STYLE_DEFAULT, lf.lfItalic);
	m_sciEdit.StyleSetBold(STYLE_DEFAULT, bBord);
	m_sciEdit.StyleSetCharacterSet(STYLE_DEFAULT, lf.lfCharSet);
	m_sciEdit.StyleSetUnderline(STYLE_DEFAULT, lf.lfUnderline);
}

BOOL CMainDlg::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry, int nValue, LPCTSTR m_pszProfileName)
{
	TCHAR szT[16];
	_stprintf_s(szT, _countof(szT), _T("%d"), nValue);
	return ::WritePrivateProfileString(lpszSection, lpszEntry, szT, m_pszProfileName);
}



