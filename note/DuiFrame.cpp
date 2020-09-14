#include "stdafx.h"
#include "DuiFrame.h"
#include "resource.h"


CDuiFrameWnd::CDuiFrameWnd()
{
	listHeaderPadding = 0;
	menuItemHeight = 0;
	m_bfocus = FALSE;

}

CDuiFrameWnd::~CDuiFrameWnd()
{
	PostQuitMessage(0);
	std::map<CString, CMenuWnd*>::iterator iter;

	for (iter = mpage.begin(); iter != mpage.end(); ++iter)
		iter->second = NULL;
	mpage.clear();
}

LPCTSTR CDuiFrameWnd::GetWindowClassName() const
{
	return _T("TXGuiFoundation");
}

CDuiString CDuiFrameWnd::GetSkinFile()
{
	return L"skin.xml";
}

LPCTSTR CDuiFrameWnd::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

UILIB_RESOURCETYPE CDuiFrameWnd::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

CDuiString CDuiFrameWnd::GetSkinFolder()
{
	return _T("");
}

void CDuiFrameWnd::InitWindow()
{
	Scintilla_RegisterClasses(GetModuleHandle(NULL));

	USES_CONVERSION;
	FILE *file = fopen(W2A(SaveIniFilePath() + M_INI_PATH), "r");
	if (!file)
		GetResourceSetting(IDR_TXT1, _T("txt"), SaveIniFilePath() + M_INI_PATH);

	CRect rect2;
	rect2.left = ::GetPrivateProfileInt(_T("RECT"), _T("Left"), 0, SaveIniFilePath() + M_INI_PATH);
	rect2.right = ::GetPrivateProfileInt(_T("RECT"), _T("Right"), 0, SaveIniFilePath() + M_INI_PATH);
	rect2.top = ::GetPrivateProfileInt(_T("RECT"), _T("Top"), 0, SaveIniFilePath() + M_INI_PATH);
	rect2.bottom = ::GetPrivateProfileInt(_T("RECT"), _T("Bottom"), 0, SaveIniFilePath() + M_INI_PATH);
	::MoveWindow(m_hWnd, rect2.left, rect2.top, rect2.Width(), rect2.Height(), TRUE);

	GetPrivateProfileStructW(_T("Font"), _T("edit font"), (LPVOID)&m_lfont, sizeof(LOGFONT), SaveIniFilePath() + M_INI_PATH);
	m_font.CreateFontIndirect(&m_lfont);

	
	// 跳转
	m_SkipDlg = new CGotoDlg();
	m_SkipDlg->Create(m_hWnd, _T("Goto"), 0, 0, 0, 351, 167);

	// 查找
	m_FindDlg = new CFindDlg();
	m_FindDlg->Create(m_hWnd, _T("Find"), 0, 0, 0, 430, 170);

	// 替换
	m_ReplaceDlg = new CReplaceDlg();
	m_ReplaceDlg->Create(m_hWnd, _T("Replace"), 0, 0, 0, 449, 215);

	// 关于
	m_AboutDlg = new CAboutDlg();
	m_AboutDlg->Create(m_hWnd, _T("About"), 0, 0, 0, 482, 300);

	// 设置
	m_OpctionDlg = new COpctionDlg();
	m_OpctionDlg->Create(m_hWnd, _T("opction"), 0, 0, 0, 600, 373);

	// 样式
	m_StyleDlg = new CStyleWnd();
	m_StyleDlg->Create(m_hWnd, _T("style"), 0, 0, 0, 596, 406);


	pItem1 = (CListHeaderItemUI*)m_PaintManager.FindControl(_T("file"));
	pItem2 = (CListHeaderItemUI*)m_PaintManager.FindControl(_T("edit"));
	pItem3 = (CListHeaderItemUI*)m_PaintManager.FindControl(_T("fmat"));
	pItem4 = (CListHeaderItemUI*)m_PaintManager.FindControl(_T("look"));
	pItem5 = (CListHeaderItemUI*)m_PaintManager.FindControl(_T("help"));

}


// CMainDlg Init Send
void CDuiFrameWnd::OnChildWindowInit(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	mpage.insert(pair<CString, CMenuWnd*>(_T("file"), m_pMenuFile));
	mpage.insert(pair<CString, CMenuWnd*>(_T("edit"), m_pMenuEdit));
	mpage.insert(pair<CString, CMenuWnd*>(_T("fmat"), m_pMenuFmat));
	mpage.insert(pair<CString, CMenuWnd*>(_T("look"), m_pMenuLook));
	mpage.insert(pair<CString, CMenuWnd*>(_T("help"), m_pMenuHelp));
	std::map<CString, CMenuWnd*>::iterator iter;


	for (iter = mpage.begin(); iter != mpage.end(); ++iter) {
		SetMenuInfo(iter->first);
		iter->second = new CMenuWnd;
		iter->second->SetMenuID(nItemID);
		iter->second->Create((HWND)wParam, L"", WS_CHILD, WS_EX_TOPMOST, 0, 0, 163, menuItemHeight);

		CDuiRect rect, rtParent;
		::GetWindowRect(*(iter->second), &rect);
		rect.Offset(listHeaderPadding - rect.left, - rect.top);
		::MoveWindow(*(iter->second), rect.left, rect.top,
			rect.GetWidth(), rect.GetHeight(), FALSE);
	}
}

LRESULT CDuiFrameWnd::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return __super::OnCreate(uMsg, wParam, lParam, bHandled);
}

void CDuiFrameWnd::OnExit(TNotifyUI& msg)
{
	if(CheckModify()) Close();
}

void CDuiFrameWnd::Notify(TNotifyUI& msg)
{
	if (msg.sType == _T("click")) {
		if (msg.pSender->GetName() == _T("closebtn"))
			OnExit(msg);
		if (msg.pSender->GetName() == _T("minbtn"))
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
		if (msg.pSender->GetName() == _T("maxbtn"))
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		if (msg.pSender->GetName() == _T("restorebtn"))
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
	}

	if (msg.sType == _T("headerclick")) {

		if (msg.pSender->GetName() == _T("file") |
			msg.pSender->GetName() == _T("edit") |
			msg.pSender->GetName() == _T("fmat") |
			msg.pSender->GetName() == _T("look") |
			msg.pSender->GetName() == _T("help")
			) {
			m_bfocus = TRUE;
			std::map<CString, CMenuWnd*>::iterator iter;

			// 初始化所有菜单notify消息
			for (iter = mpage.begin(); iter != mpage.end(); iter++) 
				iter->second->Init(msg.pSender);

			// 显示当前菜单
			iter = mpage.find(CString(msg.pSender->GetName().GetData()));
			iter->second->ShowWindow();
			::SetFocus(*iter->second);
		}
	}

	if (msg.sType == _T("killmenufocus")) m_bfocus = FALSE;

	if (msg.sType == _T("menu_New")) OnFileNew();
	if (msg.sType == _T("menu_Open")) OnFileOpen();
	if (msg.sType == _T("menu_Save")) OnFileSave();
	if (msg.sType == _T("menu_SaveAs")) OnFileSaveAs();
	if (msg.sType == _T("menu_Print")) OnPrint();
	if (msg.sType == _T("menu_Exit")) OnFileExit();

	if (msg.sType == _T("menu_Undo")) OnEditUndo();
	if (msg.sType == _T("menu_Cut")) OnEditCut();
	if (msg.sType == _T("menu_Copy")) OnEditCopy();
	if (msg.sType == _T("menu_Paste")) OnEditPaste();
	if (msg.sType == _T("menu_Delete")) OnEditClear();
	if (msg.sType == _T("menu_Find")) OnEditFind();
	if (msg.sType == _T("menu_FindNext")) OnEditNext();
	if (msg.sType == _T("menu_Replace")) OnEditReplace();
	if (msg.sType == _T("menu_Go")) OnEditGo();
	if (msg.sType == _T("menu_SelectAll")) OnEditSelectAll();
	if (msg.sType == _T("menu_Date")) OnEditDate();
	if (msg.sType == _T("menu_Style")) OnEditStyle();

	if (msg.sType == _T("menu_Font")) {
		m_FontDlg = new CFontWnd;
		m_FontDlg->Create(m_hWnd, _T("font"), 0, 0, 0, 500, 400);
		m_FontDlg->CenterWindow();
		m_FontDlg->ShowWindow();
		m_FontDlg->GetParentFont(&m_lfont);
		m_FontDlg->Init(msg.pSender, m_childEdit);
		m_FontDlg->InitControls();
	}

	if (msg.sType == _T("btn_ok")) {  // font dlg ok btn
		m_lfont = m_FontDlg->m_lf;
		StrCpyW(m_lfont.lfFaceName, m_FontDlg->m_lf.lfFaceName);
		SetFontInfo(m_lfont);
	
		CopyFontStruct(m_lfont);
	}

	if (msg.sType == _T("menu_Setting")) OnSetting();
	if (msg.sType == _T("menu_Help")) OnHelp();
	if (msg.sType == _T("menu_About")) OnAboutDlg();
	
}

void CDuiFrameWnd::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

LRESULT CDuiFrameWnd::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int cx = LOWORD(lParam);
	int cy = HIWORD(lParam);
	CRect rect(0, 0, cx, cy), rt;
	rect.left += 2;
	rect.right -= 2;
	rect.top += 57;  // 57 标题栏和菜单条高度
	rect.bottom -= 2;


	::MoveWindow(m_hWndChild, rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
	::MoveWindow(m_hWndChildEdit, 0, 0, cx - 4, cy - 57, FALSE);
	::RedrawWindow(m_hWndChildEdit, 0, 0, RDW_INVALIDATE);


	bHandled = FALSE;
	return FALSE;
}

LRESULT CDuiFrameWnd::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	m_bfocus = FALSE;
	::SetFocus(m_hWnd);

	bHandled = FALSE;
	return FALSE;
}

LRESULT CDuiFrameWnd::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::SetFocus(m_hWndChildEdit);
	bHandled = FALSE;
	return FALSE;
}

LRESULT CDuiFrameWnd::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::SetFocus(m_hWndChildEdit);
	bHandled = FALSE;
	return FALSE;
}

LRESULT CDuiFrameWnd::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	CPoint pt(LOWORD(lParam), HIWORD(lParam));
	CRect rtFile, rtEdit, rtFmat, rtLook, rtHelp;

	if (m_bfocus) {

		rtFile = pItem1->GetPos();
		rtEdit = pItem2->GetPos();
		rtFmat = pItem3->GetPos();
		rtLook = pItem4->GetPos();
		rtHelp = pItem5->GetPos();

		if (rtFile.PtInRect(pt)) ShowMenuPage(_T("file"));
		if (rtEdit.PtInRect(pt)) ShowMenuPage(_T("edit"));
		if (rtFmat.PtInRect(pt)) ShowMenuPage(_T("fmat"));
		if (rtLook.PtInRect(pt)) ShowMenuPage(_T("look"));
		if (rtHelp.PtInRect(pt)) ShowMenuPage(_T("help"));
	}


	bHandled = FALSE;
	return FALSE;
}


void CDuiFrameWnd::ShowMenuPage(CString szMenu)
{
	std::map<CString, CMenuWnd*>::iterator iter;
	for (iter = mpage.begin(); iter != mpage.end(); iter++) {
		iter->second->ShowWindow(_tcscmp(szMenu, iter->first) ? FALSE : TRUE);
		if(!_tcscmp(szMenu, iter->first))
			::SetFocus(*(iter->second));   // 将焦点设置到菜单
	}
}

void CDuiFrameWnd::HideMenuPage()
{
	std::map<CString, CMenuWnd*>::iterator iter;
	for (iter = mpage.begin(); iter != mpage.end(); iter++)
		iter->second->ShowWindow(FALSE);
}

void CDuiFrameWnd::OnKillMenuFocus()
{
	m_bfocus = FALSE;
	::SetFocus(m_hWnd);
}

LRESULT CDuiFrameWnd::OnDropFiles(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TCHAR sFile[MAX_PATH];
	HDROP hDropInfo = (HDROP)wParam;
	DragQueryFile(hDropInfo, 0, sFile, _countof(sFile));
	m_szFile = sFile;
	SetTitle();
	m_childEdit->ClearAll();
	USES_CONVERSION;
	ReadFile(W2A(sFile));
	m_childEdit->EmptyUndoBuffer();
	m_childEdit->GotoPos(0);
	m_childEdit->SetSavePoint();
	return TRUE;
}

// 拖拽txt到exe上
void CDuiFrameWnd::OpenFileFromCommandLine(CString szPath)
{
	// 路径包含" 
	szPath.Replace(_T("\""), _T(""));

	USES_CONVERSION;
	if (szPath.GetLength() == 0) return;
	if (!fopen(W2A(szPath), "r")) return;

	m_szFile = szPath;
	SetTitle();
	m_childEdit->ClearAll();
	ReadFile(W2A(m_szFile));
	m_childEdit->EmptyUndoBuffer();
	m_childEdit->GotoPos(0);
	m_childEdit->SetSavePoint();
}


LRESULT CDuiFrameWnd::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Scintilla_ReleaseResources();
	return FALSE;
}

// 新建
void CDuiFrameWnd::OnFileNew()
{
	if (CheckModify() == FALSE)
		return;
	m_szFile.Empty();
	SetTitle();
	m_childEdit->ClearAll();
}

// 打开
void CDuiFrameWnd::OnFileOpen()
{
	TCHAR openFileName[MAX_PATH] = L"\0";
	OPENFILENAME ofn = { sizeof(OPENFILENAME) };
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.lpstrFile = openFileName;
	ofn.nMaxFile = sizeof(openFileName);
	TCHAR filter[] = _T("文本文件 (*.txt;*.ini;)\0*.txt;*.ini;\0所有文件 (*.*)\0*.*\0\0");

	ofn.lpstrFilter = filter;
	ofn.lpstrCustomFilter = 0;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = _T("打开文件");
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if (!::GetOpenFileName(&ofn))
	{
		m_childEdit->GotoPos(0);
		return;
	}

	USES_CONVERSION;
	m_szFile = openFileName;
	SetTitle();
	ReadFile(W2A(m_szFile));
	m_childEdit->EmptyUndoBuffer();
	m_childEdit->GotoPos(0);
}

// 另存为
void CDuiFrameWnd::OnFileSaveAs()
{
	TCHAR openFileName[MAX_PATH] = L"\0";
	OPENFILENAME ofn = { sizeof(OPENFILENAME) };
	ofn.hwndOwner = m_hWnd;
	ofn.hInstance = GetModuleHandle(NULL);
	ofn.lpstrFile = openFileName;
	ofn.nMaxFile = sizeof(openFileName);
	TCHAR sFilter[] = TEXT("文本文件 (*.txt)\0*.txt\0所有文件 (*.*)\0*.*\0\0");  // 必须以\0\0结尾, 否则有可能出现其它选项

	ofn.lpstrFilter = sFilter;
	ofn.lpstrCustomFilter = 0;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrTitle = _T("保存文件");
	ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if (!GetSaveFileName(&ofn))
	{
		m_childEdit->GotoPos(0);
		return;
	}
	CString szName;
	if (ofn.nFilterIndex == 1)
		szName = _T(".txt");

	m_szFile = openFileName + szName;
	OnFileSave();

}

// 保存
void CDuiFrameWnd::OnFileSave()
{
	if (m_szFile.GetLength() == 0)
	{
		OnFileSaveAs();
		return;
	}

	std::ofstream fs;
	USES_CONVERSION;
	fs.open(W2A(m_szFile), ios::out | ios::binary);
	if (fs.is_open()) {
		int i = 0;
		int nCount = m_childEdit->GetLineCount();
		if (encoding == ENCODING_ASCII) {
			while (i < nCount) {
				CString str = m_childEdit->GetLine(i);
				//str.Replace(L"\r\n", L"\n");
				fs << W2A(str);
				i++;
			}
		}
		else {
			uint8_t utf8BOM[] = { 0xEF,0xBB,0xBF };
			fs.write((char*)utf8BOM, sizeof(utf8BOM));
			while (i < nCount) {
				CString str = m_childEdit->GetLine(i);
				//str.Replace(L"\r\n", L"\n");
				fs << std::string(to_utf8(str.GetString()));
				i++;
			}
		}
		fs.close();
	}

	SetTitle();
	m_childEdit->SetSavePoint();
}

// 打印
void CDuiFrameWnd::OnPrint()
{

}

// 退出
void CDuiFrameWnd::OnFileExit()
{
	Close();
}

// 删除
void CDuiFrameWnd::OnEditClear()
{
	int nCharPos = m_childEdit->GetCurrentPos();
	int nLinePos = m_childEdit->LineFromPosition(nCharPos);
	int charPos = m_childEdit->PositionFromLine(nLinePos);
	int nLen = m_childEdit->LineLength(nLinePos);
	int selStart = m_childEdit->GetSelectionStart();
	int selEnd = m_childEdit->GetSelectionEnd();

	if (selStart == selEnd)
		m_childEdit->DeleteRange(charPos, nLen);
	else
		m_childEdit->Clear();
}

// 复制
void CDuiFrameWnd::OnEditCopy()
{
	m_childEdit->CopyAllowLine();
}

// 时间/日期
void CDuiFrameWnd::OnEditDate()
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
	m_childEdit->ReplaceSel(W2A(str));
}

// 撤销
void CDuiFrameWnd::OnEditUndo()
{
	m_childEdit->Undo();
}

// 剪切
void CDuiFrameWnd::OnEditCut()
{
	int nCharPos = m_childEdit->GetCurrentPos();				// 光标所在字符位置
	int nLinePos = m_childEdit->LineFromPosition(nCharPos);		// 行索引编号
	int charPos = m_childEdit->PositionFromLine(nLinePos);		// 当前行第一个字符位置
	int nLen = m_childEdit->LineLength(nLinePos);				// 行长度
	int selStart = m_childEdit->GetSelectionStart();			// 选中文本的开始位置
	int selEnd = m_childEdit->GetSelectionEnd();

	m_childEdit->CopyAllowLine();
	if (selStart == selEnd)
		m_childEdit->DeleteRange(charPos, nLen);
	else
		m_childEdit->Cut();

}

// 粘贴
void CDuiFrameWnd::OnEditPaste()
{
	m_childEdit->Paste();
}

// 查找下一个
void CDuiFrameWnd::OnEditNext()
{
	OnEditFind();
}

// 替换
void CDuiFrameWnd::OnEditReplace()
{
	m_ReplaceDlg->Init(m_childEdit, m_childEdit->GetSelText());
	m_ReplaceDlg->CenterWindow();
	m_ReplaceDlg->ShowWindow();
}

// 查找
void CDuiFrameWnd::OnEditFind()
{
	m_FindDlg->m_FindEdit->SetText(_T(""));
	m_FindDlg->Init(m_childEdit, m_childEdit->GetSelText());
	m_FindDlg->CenterWindow();
	m_FindDlg->ShowWindow();
	m_FindDlg->m_FindEdit->SetFocus();
}

// 自动换行
void CDuiFrameWnd::OnEditChangeLine()
{
}

// 全选
void CDuiFrameWnd::OnEditSelectAll()
{
	m_childEdit->SelectAll();
}

// 跳转
void CDuiFrameWnd::OnEditGo()
{
	m_SkipDlg->Init(m_childEdit);
	m_SkipDlg->CenterWindow();
	m_SkipDlg->ShowWindow();
	m_SkipDlg->m_pLineNumEdit->SetText(_T(""));
	m_SkipDlg->m_pLineNumEdit->SetFocus();
}

// 选择字体
void CDuiFrameWnd::OnFormatFont()
{

}

// 样式设置
void CDuiFrameWnd::OnEditStyle()
{
	m_StyleDlg->Init(m_childEdit);
	m_StyleDlg->CenterWindow();
	m_StyleDlg->ShowWindow();
}


// 关于
void CDuiFrameWnd::OnAboutDlg()
{
	m_AboutDlg->CenterWindow();
	m_AboutDlg->ShowWindow();
}

// 设置
void CDuiFrameWnd::OnSetting()
{
	m_OpctionDlg->GetParentCtrl(m_childEdit);
	m_OpctionDlg->Init();
	m_OpctionDlg->CenterWindow();
	m_OpctionDlg->ShowWindow();
}

// 帮助
void CDuiFrameWnd::OnHelp()
{
	CString  str, szFile;
	GetModuleFileName(GetModuleHandle(NULL), szFile.GetBuffer(MAX_PATH), MAX_PATH);
	szFile.ReleaseBuffer();
	str = szFile.Left(szFile.ReverseFind('\\')) + L"\\ReadMe.txt";

	GetResourceSetting(IDR_TXT2, L"txt", str);
}



LRESULT CDuiFrameWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	LRESULT lRes = 0;
	switch (uMsg)
	{
	case WM_DROPFILES:			lRes = OnDropFiles(uMsg, wParam, lParam, bHandled); break;
	case WM_DESTROY:			lRes = OnDestroy(uMsg, wParam, lParam, bHandled); break;
	case MSG_MENU_NEW:			OnFileNew(); lRes = TRUE; break;
	case MSG_MENU_OPEN:			OnFileOpen(); lRes = TRUE; break;
	case MSG_MENU_SAVE:			OnFileSave(); lRes = TRUE; break;
	case MSG_MENU_FIND:			OnEditFind(); lRes = TRUE; break;
	case MSG_MENU_REPLACE:		OnEditReplace(); lRes = TRUE; break;
	case MSG_MENU_GOTOLINE:		OnEditGo(); lRes = TRUE; break;
	case WM_NCLBUTTONDOWN:		::SetFocus(m_hWndChildEdit); m_bfocus = FALSE; lRes = TRUE; break;
	case MSG_CHILDWINDOW_INIT:	OnChildWindowInit(uMsg, wParam, lParam, bHandled); lRes = TRUE; break;
	case MSG_KILLMENUFOCUS:		OnKillMenuFocus(); lRes = TRUE; break;
	}

	bHandled = FALSE;
	return lRes;
}

void CDuiFrameWnd::SetMenuInfo(CString lpszType)
{
	if (lpszType == _T("file")) {
		listHeaderPadding = FILE_PADDING;
		menuItemHeight = FILE_HEIGHT;
		nItemID = L"file.xml";
	}
	else if (lpszType == _T("edit")) {
		listHeaderPadding = EDIT_PADDING;
		menuItemHeight = EDIT_HEIGHT;
		nItemID = L"edit.xml";
	}
	else if (lpszType == _T("fmat")) {
		listHeaderPadding = FMAT_PADDING;
		menuItemHeight = FMAT_HEIGHT;
		nItemID = L"fmat.xml";
	}
	else if (lpszType == _T("look")) {
		listHeaderPadding = LOOK_PADDING;
		menuItemHeight = LOOK_HEIGHT;
		nItemID = L"look.xml";
	}
	else if (lpszType == _T("help")) {
		listHeaderPadding = HELP_PADDING;
		menuItemHeight = HELP_HEIGHT;
		nItemID = L"help.xml";
	}
}


// 设置主窗口标题
void CDuiFrameWnd::SetTitle()
{
	CString szFile;
	int i = m_szFile.ReverseFind(TEXT('\\'));
	if (i >= 0)
		szFile = m_szFile.Mid(i + 1);
	else
		szFile = m_szFile;
	if (szFile.IsEmpty())
		szFile = TEXT("notepad");
	pTitle = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("apptitle")));
	pTitle->SetText(szFile + TEXT(" - 记事本"));

}

// 检查文本状态(是否被修改)
BOOL CDuiFrameWnd::CheckModify()
{
	if (GetModify())
	{
		CString str;
		if (m_szFile.IsEmpty())
			str = TEXT("是否将更改保存到notepad");
		else
			str = TEXT("是否将更改保存到\r\n") + m_szFile;
		int nRet = MsgBox(m_hWnd, str, MB_YESNOCANCEL);
		if (IDCANCEL == nRet)
			return FALSE;
		if (IDOK == nRet)
			OnFileSave();
	}
	return TRUE;
}


// 读取文本
void CDuiFrameWnd::ReadFile(std::string path)
{
	std::string result;
	std::ifstream ifs(path.c_str(), std::ios::binary);
	std::stringstream ss;
	encoding = ENCODING_ASCII;

	if (!ifs.is_open()) {
		// Unable to read file
		result.clear();
		//return result;
		::MessageBox(NULL, L"Cant't not open file", L"error", 0);
	}
	else if (ifs.eof()) {
		result.clear();
	}
	else {
		int ch1 = ifs.get();
		int ch2 = ifs.get();
		if (ch1 == 0xff && ch2 == 0xfe) {
			// The file contains UTF-16LE BOM
			encoding = ENCODING_UTF16LE;
		}
		else if (ch1 == 0xfe && ch2 == 0xff) {
			// The file contains UTF-16BE BOM
			encoding = ENCODING_UTF16BE;
		}
		else {
			int ch3 = ifs.get();
			if (ch1 == 0xef && ch2 == 0xbb && ch3 == 0xbf) {
				// The file contains UTF-8 BOM
				encoding = ENCODING_UTF8;
			}
			else {
				// The file does not have BOM
				encoding = ENCODING_ASCII;
				ifs.seekg(0);
			}
		}
	}
	ss << ifs.rdbuf() << '\0';
	if (encoding == ENCODING_UTF16LE) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utfconv;
		result = utfconv.to_bytes(std::wstring((wchar_t *)ss.str().c_str()));
	}
	else if (encoding == ENCODING_UTF16BE) {
		std::string src = ss.str();
		std::string dst = src;
		// Using Windows API
		_swab(&src[0u], &dst[0u], src.size() + 1);
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utfconv;
		result = utfconv.to_bytes(std::wstring((wchar_t *)dst.c_str()));
	}
	else if (encoding == ENCODING_UTF8) {
		result = ss.str();
	}
	else {
		result = ss.str();
	}

	// change
	CString str;
	if (encoding == ENCODING_ASCII) {
		str = result.c_str();
	}
	else {
		std::string utf8Content = result;
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> utfconv;
		std::wstring utf16LeContent = utfconv.from_bytes(utf8Content);
		_setmode(_fileno(stdout), _O_U8TEXT);
		str = utf16LeContent.c_str();
	}
	m_childEdit->ClearAll();
	m_childEdit->AddText(str.GetLength(), str);

}


BOOL CDuiFrameWnd::WriteProfileInt(LPCTSTR lpszSection, LPCTSTR lpszEntry,
	int nValue, LPCTSTR m_pszProfileName)
{
	TCHAR szT[16];
	_stprintf_s(szT, _countof(szT), _T("%d"), nValue);
	return ::WritePrivateProfileString(lpszSection, lpszEntry, szT, m_pszProfileName);
}

void CDuiFrameWnd::CopyFontStruct(LOGFONT lf)
{
	// sci edit控件字体
	m_lfEdit->lfHeight = lf.lfHeight;
	m_lfEdit->lfWidth = lf.lfWidth;
	m_lfEdit->lfEscapement = lf.lfEscapement;
	m_lfEdit->lfOrientation = lf.lfOrientation;
	m_lfEdit->lfWeight = lf.lfWeight;
	m_lfEdit->lfItalic = lf.lfItalic;
	m_lfEdit->lfUnderline = lf.lfUnderline;
	m_lfEdit->lfStrikeOut = lf.lfStrikeOut;
	m_lfEdit->lfCharSet = lf.lfCharSet;
	m_lfEdit->lfOutPrecision = lf.lfOutPrecision;
	m_lfEdit->lfClipPrecision = lf.lfClipPrecision;
	m_lfEdit->lfQuality = lf.lfQuality;
	m_lfEdit->lfPitchAndFamily = lf.lfPitchAndFamily;

	StrCpyW(m_lfEdit->lfFaceName, lf.lfFaceName);
}

void CDuiFrameWnd::SetFontInfo(LOGFONT & lf)
{
	CDC dc = GetDC(NULL);
	BOOL bBord = lf.lfWeight == 400 ? FALSE : TRUE;
	int fontSize = (int)(-lf.lfHeight*72.0 / GetDeviceCaps(dc, LOGPIXELSY) + 0.5);

	m_childEdit->StyleSetFont(STYLE_DEFAULT, lf.lfFaceName);
	m_childEdit->StyleSetSize(STYLE_DEFAULT, fontSize);
	m_childEdit->StyleSetItalic(STYLE_DEFAULT, lf.lfItalic);
	m_childEdit->StyleSetBold(STYLE_DEFAULT, bBord);
	m_childEdit->StyleSetCharacterSet(STYLE_DEFAULT, lf.lfCharSet);
	m_childEdit->StyleSetUnderline(0, lf.lfUnderline);
}


// 获取资源中配置文件
BOOL CDuiFrameWnd::GetResourceSetting(UINT uID, CString szType, CString szOutPath)
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

	//释放文件
	LPBYTE lpByte = (LPBYTE)LockResource(hglobal);
	DWORD dwRcSize = SizeofResource(hModule, hRes);


	USES_CONVERSION;
	std::ofstream fs(W2A(szOutPath), ios::out);
	fs << lpByte;
	fs.close();

	return TRUE;
}

std::string CDuiFrameWnd::to_utf8(const wchar_t* buffer, int len)
{
	int nChars = ::WideCharToMultiByte(
		CP_UTF8,
		0,
		buffer,
		len,
		NULL,
		0,
		NULL,
		NULL);
	if (nChars == 0) return "";

	string newbuffer;
	newbuffer.resize(nChars);
	::WideCharToMultiByte(
		CP_UTF8,
		0,
		buffer,
		len,
		const_cast<char*>(newbuffer.c_str()),
		nChars,
		NULL,
		NULL);

	return newbuffer;
}

std::string CDuiFrameWnd::to_utf8(const std::wstring& str)
{
	return to_utf8(str.c_str(), (int)str.size());
}