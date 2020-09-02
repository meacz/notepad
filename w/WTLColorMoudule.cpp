
#include "stdafx.h"
#include "resource.h"
#include "WTLColorMoudule.h"

HWND g_hWndCaller = NULL;	// 保存主窗口句柄
HHOOK g_hHook = NULL;		// 保存钩子句柄

CWTLColorPicker::CWTLColorPicker()
{
	m_bColor = FALSE;
	m_bHue = FALSE;
	row_buffer = new DWORD[256];
}


LRESULT CWTLColorPicker::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	SetWindowText(TEXT("Color Picker"));

	m_ok.SubclassWindow(GetDlgItem(IDOK));
	m_close.SubclassWindow(GetDlgItem(IDC_CLOSE));
	m_cancel.SubclassWindow(GetDlgItem(IDCANCEL));

	m_eRed.SubclassWindow(GetDlgItem(IDC_RED));
	m_eBlue.SubclassWindow(GetDlgItem(IDC_BLUE));
	m_eGreen.SubclassWindow(GetDlgItem(IDC_GREEN));
	m_eHue2.SubclassWindow(GetDlgItem(IDC_HUE));
	m_eSaturation2.SubclassWindow(GetDlgItem(IDC_SATURATION));
	m_eValue.SubclassWindow(GetDlgItem(IDC_VALUE));
	m_eHtml.SubclassWindow(GetDlgItem(IDC_HTML));

	PostMessage(WM_LBUTTONDOWN);

	//m_HotKeyId = ::GlobalAddAtom(_T("wtl_hot_key"));
	//::RegisterHotKey(m_hWnd, m_HotKeyId, 0, VK_TAB);
	//::RegisterHotKey(m_hWnd, m_HotKeyId, 0, VK_RETURN);


	// Color
	CurrCol = {
		255, 255, 255,  // r g b
		0, 0, 100 };	// h s v
	UpdateValues(GetCurrentColour());

	// cursor
	m_arrow = LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_BLUE_ARROW));


	// init color data 

	CRect rect, rtSil;
	HWND hCol1 = GetDlgItem(IDC_PCOLOR);
	::GetWindowRect(hCol1, &rect);

	int w = rect.Width();
	int h = rect.Height();


	if (bmp_handle)
		::DeleteObject(bmp_handle);

	// color1
	bmp_info.bmiHeader.biSize = sizeof (BITMAPINFO);
	bmp_info.bmiHeader.biWidth = w;
	bmp_info.bmiHeader.biHeight = h;
	bmp_info.bmiHeader.biPlanes = 1;
	bmp_info.bmiHeader.biBitCount = 32;
	bmp_info.bmiHeader.biCompression = BI_RGB;
	bmp_info.bmiHeader.biSizeImage = w * h * 32 / 8;
	bmp_info.bmiHeader.biXPelsPerMeter =
		bmp_info.bmiHeader.biYPelsPerMeter = 72 * 2 * 1000;
	bmp_info.bmiHeader.biClrUsed = 0;
	bmp_info.bmiHeader.biClrImportant = 0;
	bmp_data = NULL;
	bmp_handle = CreateDIBSection(
		NULL, &bmp_info, DIB_RGB_COLORS, (void **)&bmp_data, NULL, 0);


	if (bmp_handle2)
		::DeleteObject(bmp_handle2);

	HWND hSild = GetDlgItem(IDC_PSILDER);
	::GetWindowRect(hSild, rtSil);

	// 留12px绘制滑块
	w = rtSil.Width() - 12;
	h = rtSil.Height();
	m_nBuffSize = h;

	// color2
	bmp_info2.bmiHeader.biSize = sizeof (BITMAPINFO);
	bmp_info2.bmiHeader.biWidth = w;
	bmp_info2.bmiHeader.biHeight = h;
	bmp_info2.bmiHeader.biPlanes = 1;
	bmp_info2.bmiHeader.biBitCount = 32;
	bmp_info2.bmiHeader.biCompression = BI_RGB;
	bmp_info2.bmiHeader.biSizeImage = w * h * 32 / 8;
	bmp_info2.bmiHeader.biXPelsPerMeter =
		bmp_info2.bmiHeader.biYPelsPerMeter = 72 * 2 * 1000;
	bmp_info2.bmiHeader.biClrUsed = 0;
	bmp_info2.bmiHeader.biClrImportant = 0;
	bmp_data2 = NULL;
	bmp_handle2 = CreateDIBSection(
		NULL, &bmp_info2, DIB_RGB_COLORS, (void **)&bmp_data2, NULL, 0);

	//SetKeyHook(TRUE);

	CenterWindow();
	return TRUE;
}

HBRUSH CWTLColorPicker::OnCtlColorEdit(CDCHandle dc, CEdit edit)
{
	// Edit外观
	dc.SetBkColor(COLOR_DLG_BK2);
	dc.SetTextColor(COLOR_EDIT_TEXT);
	return (HBRUSH)GetStockObject(NULL_BRUSH);
}

LRESULT CWTLColorPicker::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);
	CRect rtBk, rt;
	GetClientRect(rtBk);
	rt = rtBk;
	// 对话框背景
	dc.FillSolidRect(rtBk, COLOR_DLG_BK1);
	rt.DeflateRect(4, m_ncHeight, 4, 4);
	dc.FillSolidRect(rt, COLOR_DLG_BK2);


	HWND hWndColor, hWndColor2, hWndsel;
	CDC hdc, hcomp;
	CBitmap hbmp;
	CRect rect, rtSilder, rtPer, rtCen, rtBor;
	CPen pen;
	CBrush brush;
	int red, green, blue;

	// Color1  *****

	hWndColor = GetDlgItem(IDC_PCOLOR);
	hdc = ::GetDC(hWndColor);
	hcomp = CreateCompatibleDC(hdc);

	::GetWindowRect(hWndColor, &rect);

	rtBor = rect;
	ScreenToClient(rtBor);
	pen = CreatePen(PS_SOLID, 1, COLOR_SPACE_BORDER);
	dc.SelectPen(pen);
	dc.SelectStockBrush(NULL_BRUSH);
	rtBor.InflateRect(1, 1, 1, 1);
	dc.Rectangle(rtBor);

	red = GetDlgItemInt(IDC_RED);
	green = GetDlgItemInt(IDC_GREEN);
	blue = GetDlgItemInt(IDC_BLUE);


	// HSV H  *****

	DoDrawHSV_Hue();
	hcomp.SelectBitmap(bmp_handle);


	// Draws rect
	double step, stepW, stepH;
	SColour tempcol;

	stepW = 100.0 / rect.Width();
	stepH = 100.0 / rect.Height();

	tempcol.s = (unsigned short)GetDlgItemInt(IDC_SATURATION);
	tempcol.v = (unsigned short)GetDlgItemInt(IDC_VALUE);
	tempcol.UpdateRGB();

	rtCen.SetRect(
		(int)(tempcol.s / stepW) - 1, rect.Height() - (int)(tempcol.v / stepH) - 1,
		(int)(tempcol.s / stepW) + 2, rect.Height() - (int)(tempcol.v / stepH) + 2);

	pen = CreatePen(PS_SOLID, 1, RGB(255 - tempcol.r, 255 - tempcol.g, 255 - tempcol.b));
	brush = CreateSolidBrush(RGB(255 - tempcol.r, 255 - tempcol.g, 255 - tempcol.b));
	hcomp.SelectPen(pen);
	hcomp.SelectBrush(brush);
	hcomp.SetROP2(R2_NOT);

	hcomp.Rectangle(CRect(rtCen.left - 4, rtCen.top, rtCen.left, rtCen.bottom));		// left
	hcomp.Rectangle(CRect(rtCen.left, rtCen.top - 4, rtCen.right, rtCen.top));			// top
	hcomp.Rectangle(CRect(rtCen.right, rtCen.top, rtCen.right + 4, rtCen.bottom));		// right
	hcomp.Rectangle(CRect(rtCen.left, rtCen.bottom, rtCen.right, rtCen.bottom + 4));	// bottom


	hdc.BitBlt(0, 0, rect.right, rect.bottom, hcomp, 0, 0, SRCCOPY);


	// Color2  *****

	hWndColor2 = GetDlgItem(IDC_PSILDER);
	hdc = ::GetDC(hWndColor2);
	hcomp = CreateCompatibleDC(hdc);

	::GetClientRect(hWndColor2, &rect);

	hbmp = CreateCompatibleBitmap(hdc, rect.Width(), rect.Height());
	hcomp.SelectBitmap(hbmp);

	brush = CreateSolidBrush(COLOR_DLG_BK2);
	hcomp.FillRect(rect, brush);


	// HSV H  *****

	// draw silder
	OnDrawSilder();
	CDC m_dc = CreateCompatibleDC(hcomp);
	m_dc.SelectBitmap(bmp_handle2);
	hcomp.BitBlt(6, 0, rect.right, rect.bottom, m_dc, 0, 0, SRCCOPY);


	double hue;
	step = 359.0 / rect.Height();

	hue = (double)GetDlgItemInt(IDC_HUE);
	hue = hue / step;

	// Hue arrows
	brush = CreateSolidBrush(COLOR_SPACE_BORDER);
	pen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));

	POINT pt[] = {
		rect.right - 1, rect.Height() - ((int)(hue)-4),
		rect.right - 1, rect.Height() - ((int)(hue)+4),
		rect.right - 5, rect.Height() - ((int)(hue)),
		rect.right - 1, rect.Height() - ((int)(hue)-4)
	};
	CRgn rgn;
	rgn.CreatePolygonRgn(pt, 4, WINDING);
	hcomp.FillRgn(rgn, brush);
	rtSilder.SetRect(rect.left + 5, rect.Height() - ((int)(hue)+1), rect.right - 5, rect.Height() - ((int)hue - 2));
	hcomp.SelectPen(pen);
	hcomp.SelectBrush(brush);
	hcomp.Rectangle(rtSilder);


	// color2 border
	rtBor = rect;
	pen = CreatePen(PS_SOLID, 1, COLOR_SPACE_BORDER);
	hcomp.SelectPen(pen);
	hcomp.SelectStockBrush(NULL_BRUSH);
	rtBor.DeflateRect(5, 0, 5, 0);
	hcomp.Rectangle(rtBor);

	hdc.BitBlt(0, 0, rect.right, rect.bottom, hcomp, 0, 0, SRCCOPY);



	//// Current color  *****
	hWndsel = GetDlgItem(IDC_PSELECTCL);
	hdc = ::GetDC(hWndsel);

	::GetClientRect(hWndsel, rect);
	hdc.FillSolidRect(rect, RGB(red, green, blue));


	hdc.DeleteDC();
	hcomp.DeleteDC();
	brush.DeleteObject();
	pen.DeleteObject();

	dc.SetBkMode(TRANSPARENT);
	dc.SelectFont(GetFont());
	dc.SetTextColor(COLOR_DLG_TEXT);

	return TRUE;
}


LRESULT CWTLColorPicker::OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	POINT pt = { LOWORD(lParam), HIWORD(lParam) };
	ScreenToClient(&pt);
	CRect rect;
	GetClientRect(rect);
	rect.bottom = m_ncHeight;
	return rect.PtInRect(pt) ? HTCAPTION : TRUE;
}

LRESULT CWTLColorPicker::OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	HDC hdc = (HDC)wParam;
	//HWND hwnd = (HWND)lParam;
	HBRUSH hbr = (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	//CDC dc(hdc);
	::SetTextColor(hdc, COLOR_DLG_TEXT);
	::SetBkMode(hdc, TRANSPARENT);
	return (LRESULT)hbr;
}

LRESULT CWTLColorPicker::OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return TRUE;
}


LRESULT CWTLColorPicker::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowWindow(SW_HIDE);
	//::DestroyWindow(m_hWnd);
	return 0;
}


LRESULT CWTLColorPicker::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	::PostMessage(GetParent(), UPDATE_BTN_COLOR, 0, 0);
	//::DestroyWindow(m_hWnd);
	ShowWindow(SW_HIDE);
	return 0;
}

LRESULT CWTLColorPicker::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	ShowWindow(SW_HIDE);
	//::DestroyWindow(m_hWnd);
	return 0;
}



void CWTLColorPicker::OnLButtonDown(UINT nFlags, CPoint point)
{
	CRect rectC1, rectC2, rtPer;
	HWND Color1, Color2;

	Color1 = GetDlgItem(IDC_PCOLOR);
	Color2 = GetDlgItem(IDC_PSILDER);

	::GetWindowRect(Color1, &rectC1);
	::GetWindowRect(Color2, &rectC2);
	ClientToScreen(&point);

	// 隐藏edit光标
	::HideCaret(::GetFocus());
	//::SetFocus(WindowFromPoint(point));
	m_ok.SetFocus();

	// color1
	if (rectC1.PtInRect(point))
	{
		m_bColor = TRUE;
		m_bHue = FALSE;
	}

	// color2
	if (rectC2.PtInRect(point))
	{
		m_bHue = TRUE;
		m_bColor = FALSE;
	}


	ColorFromPos();

	// 隐藏光标
	//if (m_bColor || m_bHue || m_bAlpha)
	//{
	//	while (ShowCursor(FALSE) >= 0)
	//		ShowCursor(FALSE);
	//}

}

void CWTLColorPicker::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bColor = FALSE;
	m_bHue = FALSE;
	ReleaseCapture();

	//while (ShowCursor(TRUE) < 0)
	//	ShowCursor(TRUE);

}

void CWTLColorPicker::OnMouseMove(UINT nFlags, CPoint point)
{
	if (nFlags &MK_LBUTTON)
	{
		this->SetCapture();
		ColorFromPos();
	}
}


void CWTLColorPicker::OnMove(CPoint ptPos)
{


}

LRESULT CWTLColorPicker::OnHotKey(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
	UINT  uVirtKey = (UINT)HIWORD(lParam);
	if (uVirtKey == VK_TAB) {
		HWND hWndEdit = GetFocus();
		int nID = ::GetDlgCtrlID(hWndEdit);
		if (hWndEdit == FALSE) return FALSE;
		GotoDlgCtrl(GetDlgItem(IDC_RED));

		if (nID >= IDC_RED && nID < IDC_VALUE)
			GotoDlgCtrl(GetDlgItem(nID + 1));

		if (nID + 1 > IDC_VALUE)
			GotoDlgCtrl(GetDlgItem(IDC_RED));
	}
	return FALSE;
}


// 颜色绘图实现函数 

void CWTLColorPicker::DoDrawHSV_Hue(void)
{
	CRect rect;
	HWND hWndCo = GetDlgItem(IDC_PCOLOR);
	::GetWindowRect(hWndCo, &rect);
	ScreenToClient(rect);
	int		i;		// loop counters

	DWORD	*p;		// pointer to first pixel from the blend & a loop pointer

	int		width = rect.Width();
	int		height = rect.Height();
	int		blend_width = width;  // rect.Width();
	int		blend_height = height; // rect.Height();

	double	hue;
	double	val, val_adv;


	p = bmp_data;

	// constant
	hue = GetDlgItemInt(IDC_HUE);

	// outer loop - rows
	i = blend_height;

	// initial and change
	val = 0.0;
	val_adv = 1.0 / i;

	while (i--)
	{
		HSV_SAT(p, blend_width, hue, val);
		val += val_adv;
		p += blend_width;
	}
}

void CWTLColorPicker::DrawHSV_H()
{
	HSV_HUE(row_buffer, m_nBuffSize, 1, 1);
}


// 绘图管理函数
void CWTLColorPicker::OnDrawSilder()
{
	CRect rtSil;
	HWND hSil = GetDlgItem(IDC_PSILDER);
	::GetWindowRect(hSil, rtSil);

	DrawHSV_H();

	int	i;

	int		nSkip;	// number of pixels to skip after current row and before next one

	DWORD	*p;		// loop pointers

	int		width = rtSil.Width() - 12;
	int		height = rtSil.Height();
	int		blend_width = width;
	int		blend_height = height;

	// prepare
	p = bmp_data2;
	nSkip = 0;


	DWORD	*source = row_buffer;

	i = blend_height;
	while (i--)
	{
		set(&p, *source++, blend_width), p += nSkip;
	}
}


void CWTLColorPicker::SetRGB(unsigned short r, unsigned short g, unsigned short b)
{
	CurrCol.r = min(r, 255);
	CurrCol.g = min(g, 255);
	CurrCol.b = min(b, 255);

	CurrCol.UpdateHSV();
}

void CWTLColorPicker::SetHSV(unsigned short h, unsigned short s, unsigned short v)
{
	CurrCol.h = min(h, 359);
	CurrCol.s = min(s, 100);
	CurrCol.v = min(v, 100);

	CurrCol.UpdateRGB();
}


void CWTLColorPicker::SetRGB_F_HTML()
{
	CString str;
	m_eHtml.GetWindowText(str);

	CurrCol.RGB_F_HTML(str);
}


SColour CWTLColorPicker::GetCurrentColour()
{
	return CurrCol;
}

// HSV 转 RGB
void SColour::UpdateRGB()
{
	int conv;
	double hue, sat, val;
	int base;

	hue = (float)h / 100.0f;
	sat = (float)s / 100.0f;
	val = (float)v / 100.0f;

	if ((float)s == 0) // Acromatic color (gray). Hue doesn't mind.
	{
		conv = (unsigned short)(255.0f * val);
		r = b = g = conv;
		return;
	}

	base = (unsigned short)(255.0f * (1.0 - sat) * val);

	switch ((unsigned short)((float)h / 60.0f))
	{
		case 0:
			r = (unsigned short)(255.0f * val);
			g = (unsigned short)((255.0f * val - base) * (h / 60.0f) + base);
			b = base;
			break;

		case 1:
			r = (unsigned short)((255.0f * val - base) * (1.0f - ((h % 60) / 60.0f)) + base);
			g = (unsigned short)(255.0f * val);
			b = base;
			break;

		case 2:
			r = base;
			g = (unsigned short)(255.0f * val);
			b = (unsigned short)((255.0f * val - base) * ((h % 60) / 60.0f) + base);
			break;

		case 3:
			r = base;
			g = (unsigned short)((255.0f * val - base) * (1.0f - ((h % 60) / 60.0f)) + base);
			b = (unsigned short)(255.0f * val);
			break;

		case 4:
			r = (unsigned short)((255.0f * val - base) * ((h % 60) / 60.0f) + base);
			g = base;
			b = (unsigned short)(255.0f * val);
			break;

		case 5:
			r = (unsigned short)(255.0f * val);
			g = base;
			b = (unsigned short)((255.0f * val - base) * (1.0f - ((h % 60) / 60.0f)) + base);
			break;
	}
}

// RGB 转 HSV
void SColour::UpdateHSV()
{
	unsigned short max, min, delta;
	short temp;

	max = MAX(r, g, b);
	min = MIN(r, g, b);
	delta = max - min;

	if (max == 0)
	{
		s = h = v = 0;
		return;
	}

	v = (unsigned short)((double)max / 255.0*100.0);
	s = (unsigned short)(((double)delta / max)*100.0);

	if (r == max)
		temp = (short)(60 * ((g - b) / (double)delta));
	else if (g == max)
		temp = (short)(60 * (2.0 + (b - r) / (double)delta));
	else
		temp = (short)(60 * (4.0 + (r - g) / (double)delta));

	if (temp < 0)
		h = temp + 360;
	else
		h = temp;
}

// hsl helper function
double SColour::HSL2RGB(double v1, double v2, double vH)
{
	if (vH < 0) vH += 1;
	if (vH > 1) vH -= 1;
	if (6.0 * vH < 1) return v1 + (v2 - v1) * 6.0 * vH;
	if (2.0 * vH < 1) return v2;
	if (3.0 * vH < 2) return v1 + (v2 - v1) * ((2.0 / 3.0) - vH) * 6.0;
	return (v1);
}

// HTML转RGB
void SColour::RGB_F_HTML(CString szValue)
{
	std::string hex = CW2A(szValue.GetString());

	int* rgb = new int[3];
	std::stringstream ss;
	std::string str;

	// HTML
	if (hex[0] == '#') {
		hex.erase(0, 1);
	}

	int size = hex.size();

	for (int i = 0; i < 3; i++) {
		// Determine 3 or 6 character format.
		if (size == 3) { str = std::string(2, hex[i]); }
		else if (size == 6) { str = hex.substr(i * 2, 2); }
		else { break; }

		ss << std::hex << str;
		ss >> rgb[i];
		ss.clear();
	}
	r = rgb[0];
	g = rgb[1];
	b = rgb[2];
	//ATLTRACE(L"%d %d %d\n", r, g, b);
}


// Main
void CWTLColorPicker::UpdateValues(SColour col)
{
	CString str;
	SetDlgItemInt(IDC_RED, col.r);
	SetDlgItemInt(IDC_GREEN, col.g);
	SetDlgItemInt(IDC_BLUE, col.b);
	SetDlgItemInt(IDC_HUE, col.h);
	SetDlgItemInt(IDC_SATURATION, col.s);
	SetDlgItemInt(IDC_VALUE, col.v);

	str.Format(TEXT("#%02X%02X%02X"), col.r, col.g, col.b);
	SetDlgItemText(IDC_HTML, str);


	SetSel2(IDC_RED);
	SetSel2(IDC_GREEN);
	SetSel2(IDC_BLUE);  	// r g b
	SetSel2(IDC_HUE);
	SetSel2(IDC_SATURATION);
	SetSel2(IDC_VALUE);		// h s v
	SetSel2(IDC_HTML);

}

// 处理光标焦点
void CWTLColorPicker::SetSel2(int nID)
{
	CPoint p;
	GetCaretPos(&p);
	HWND hwnd = GetDlgItem(nID);

	DWORD dwRet = (DWORD)::SendMessage(hwnd, EM_CHARFROMPOS, 0, MAKELPARAM(p.x, p.y));
	// 当前字符索引
	int i = (int)((short)LOWORD(dwRet));
	if (GetDlgItemInt(nID) == 0)	    i = 1;
	if (GetDlgItemInt(nID) / 10 == 1)  i = 2;
	if (GetDlgItemInt(nID) / 100 == 1)  i = 3;
	::SendMessage(hwnd, EM_SETSEL, i, i);  // 重新设置光标位置
}



void CWTLColorPicker::OnEditControlChange(UINT uNotifyCode, int nID, CWindow wndCtl)
{
	int r, g, b;
	int h, s, v;
	

	if (IDC_RED == nID || IDC_GREEN == nID || IDC_BLUE == nID) {

		r = GetDlgItemInt(IDC_RED);
		g = GetDlgItemInt(IDC_GREEN);
		b = GetDlgItemInt(IDC_BLUE);
		SetRGB(r, g, b);

		UpdateValues(GetCurrentColour());
		SendMessage(WM_PAINT, 0, 0);
	}

	if (IDC_HUE == nID || IDC_SATURATION == nID || IDC_VALUE == nID){

		h = GetDlgItemInt(IDC_HUE);
		s = GetDlgItemInt(IDC_SATURATION);
		v = GetDlgItemInt(IDC_VALUE);
		SetHSV(h, s, v);

		UpdateValues(GetCurrentColour());
		SendMessage(WM_PAINT, 0, 0);
	}

	if (IDC_HTML == nID){

		SetRGB_F_HTML();
		SetRGB(GetCurrentColour().r, GetCurrentColour().g, GetCurrentColour().b);

		CString str;
		SColour col = GetCurrentColour();

		// 手动更新
		SetDlgItemInt(IDC_RED, col.r);
		SetDlgItemInt(IDC_GREEN, col.g);
		SetDlgItemInt(IDC_BLUE, col.b);
		SetDlgItemInt(IDC_HUE, col.h);
		SetDlgItemInt(IDC_SATURATION, col.s);
		SetDlgItemInt(IDC_VALUE, col.v);

		SendMessage(WM_PAINT, 0, 0);
	}

}


// 滚动更新数值
LRESULT CWTLColorPicker::OnUpdateColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int nID;
	CString str;

	// mouse wheel color
	if ((int)wParam == UPDATE_WHEEL_COLOR)
	{
		nID = LOWORD(lParam);
		if (IDC_RED == nID || IDC_GREEN == nID || IDC_BLUE == nID){
			SetDlgItemInt(nID, HIWORD(lParam));
			OnEditControlChange(NULL, nID, NULL);
		}

		if (IDC_HUE == nID || IDC_SATURATION == nID || IDC_VALUE == nID){
			SetDlgItemInt(nID, HIWORD(lParam));
			OnEditControlChange(NULL, nID, NULL);
		}
	}
	return TRUE;
}


void CWTLColorPicker::ColorFromPos()
{
	CPoint p;
	CRect rectC1, rectC2;
	HWND Color1, Color2;
	HWND hDlg = m_hWnd;

	Color1 = GetDlgItem(IDC_PCOLOR);
	Color2 = GetDlgItem(IDC_PSILDER);

	::GetWindowRect(Color1, &rectC1);
	::GetWindowRect(Color2, &rectC2);

	::GetCursorPos(&p);


	// Color1
	if (m_bColor)
	{
		int x = (p.x - rectC1.left);
		int y = (p.y - rectC1.top);
		CLAMP(x, 0, rectC1.Width());
		CLAMP(y, 0, rectC1.Height());

		// HSV H
		SetHSV((unsigned short)GetDlgItemInt(IDC_HUE),
			(unsigned short)((float)(x) / rectC1.Width()*100.0),
			(unsigned short)((float)(rectC1.Height() - y) / (rectC1.Height()*1.0)*100.0));

		//ATLTRACE(L"%f\n, %d %f", ((float)(x) / ww)*100.0, x, (float)(x) / ww);
		UpdateValues(GetCurrentColour());
	}

	// Color2
	if (m_bHue)
	{
		int y = (p.y - rectC2.top);
		CLAMP(y, 0, rectC2.Height());

		// HSV H
		SetHSV((unsigned short)(((float)rectC2.Height() - y) / rectC2.Height()*359.0),
			(unsigned short)GetDlgItemInt(IDC_SATURATION),
			(unsigned short)GetDlgItemInt(IDC_VALUE));
		UpdateValues(GetCurrentColour());
	}

	SendMessage(WM_PAINT, 0, 0);
}

BOOL CWTLColorPicker::OnSetCursor(CWindow wnd, UINT nHitTest, UINT message)
{
	SetCursor(m_arrow);
	return TRUE;
}

// help function

void CWTLColorPicker::HSV_SAT(DWORD *buffer, int samples, double hue, double val_fp)
{
	// value, but as integer in [0, 255 << int_extend]
	int		val;

	// loop counter
	int		j;

	// coefficients and advances
	signed int		coef1, coef2, coef3;
	signed int		coef1_adv, coef2_adv, coef3_adv;

	double	intp, frac;

	//
	// hue - const, in [0, 360)
	//	intp - const in 0, 1, 2, 3, 4, 5
	//	frac - const in [0, 1)
	// sat - increments, in [0, 1]; indirectly (coefficients)
	// val - const, in [0, (255 << int_extend)]
	//
	// coef1 => val * (1 - sat)              => changes from val to 0
	// coef2 => val * (1 - sat * frac)       => changes from val to val * (1 - frac)
	// coef3 => val * (1 - sat * (1 - frac)) => changes from val to val * frac
	//

	// constants
	val = (int)(val_fp * 255) << int_extend;
	frac = modf(hue / 60.0, &intp);

	// prepare
	j = samples;

	coef1 = val;
	coef1_adv = -coef1 / samples;
	coef2 = val;
	coef2_adv = (int)((1 - frac) * val - coef2) / samples;
	coef3 = val;
	coef3_adv = (int)(frac * val - coef3) / samples;

	switch ((int)intp)
	{
		case	0:
			while (j--) *buffer++ = RGB((BYTE)(coef1 >> int_extend), (BYTE)(coef3 >> int_extend), (BYTE)(val >> int_extend))
				, coef1 += coef1_adv, coef3 += coef3_adv;
			break;
		case	1:
			while (j--) *buffer++ = RGB((BYTE)(coef1 >> int_extend), (BYTE)(val >> int_extend), (BYTE)(coef2 >> int_extend))
				, coef1 += coef1_adv, coef2 += coef2_adv;
			break;
		case	2:
			while (j--) *buffer++ = RGB((BYTE)(coef3 >> int_extend), (BYTE)(val >> int_extend), (BYTE)(coef1 >> int_extend))
				, coef1 += coef1_adv, coef3 += coef3_adv;
			break;
		case	3:
			while (j--) *buffer++ = RGB((BYTE)(val >> int_extend), (BYTE)(coef2 >> int_extend), (BYTE)(coef1 >> int_extend))
				, coef1 += coef1_adv, coef2 += coef2_adv;
			break;
		case	4:
			while (j--) *buffer++ = RGB((BYTE)(val >> int_extend), (BYTE)(coef1 >> int_extend), (BYTE)(coef3 >> int_extend))
				, coef1 += coef1_adv, coef3 += coef3_adv;
			break;
		case	5:
			while (j--) *buffer++ = RGB((BYTE)(coef2 >> int_extend), (BYTE)(coef1 >> int_extend), (BYTE)(val >> int_extend))
				, coef1 += coef1_adv, coef2 += coef2_adv;
			break;
	}
}

void CWTLColorPicker::HSV_HUE(DWORD *buffer, int samples, double sat, double val_fp)
{
	// value, but as integer in [0, 255 << int_extend]
	int		val;

	// loop counter
	int		j;

	// coefficients and advances
	int		coef1, coef2, coef3;
	int		coef2_adv, coef3_adv;

	// current position and advance to the next one
	double	pos, pos_adv;

	//
	// hue increments in [0, 360); indirectly
	//	intp changes - 0, 1, 2, 3, 4, 5; indirectly (separate loops)
	//	frac increments in [0, 1) six times; indirectly (coefficients)
	// sat - const, in [0, 1]
	// val - const, in [0, (255 << int_extend)]
	//
	// coef1 => val * (1 - sat)              => const, = val * (1 - sat)
	// coef2 => val * (1 - sat * frac)       => changes from val to val * (1 - sat)
	// coef3 => val * (1 - sat * (1 - frac)) => changes from val * (1 - sat) to val
	//

	// constants
	val = (int)(val_fp * 255) << int_extend;
	coef1 = (int)(val * (1 - sat));

	// prepare
	pos = 0;
	pos_adv = (double)samples / 6.0;

	// hue in [0, 60)
	pos += pos_adv;
	j = (int)pos;
	coef3 = coef1, coef3_adv = (int)((val - coef3) / (j - 1));
	while (j--) *buffer++ = RGB((BYTE)(coef1 >> int_extend), (BYTE)(coef3 >> int_extend), (BYTE)(val >> int_extend))
		, coef3 += coef3_adv;

	pos += pos_adv;
	j = (int)pos - (int)(1 * pos_adv);
	coef2 = val, coef2_adv = (int)((val * (1.0 - sat) - coef2) / (j - 1));
	while (j--) *buffer++ = RGB((BYTE)(coef1 >> int_extend), (BYTE)(val >> int_extend), (BYTE)(coef2 >> int_extend))
		, coef2 += coef2_adv;

	pos += pos_adv;
	j = (int)pos - (int)(2 * pos_adv);
	coef3 = coef1, coef3_adv = (int)((val - coef3) / (j - 1));
	while (j--) *buffer++ = RGB((BYTE)(coef3 >> int_extend), (BYTE)(val >> int_extend), (BYTE)(coef1 >> int_extend))
		, coef3 += coef3_adv;

	pos += pos_adv;
	j = (int)pos - (int)(3 * pos_adv);
	coef2 = val, coef2_adv = (int)((val * (1.0 - sat) - coef2) / (j - 1));
	while (j--) *buffer++ = RGB((BYTE)(val >> int_extend), (BYTE)(coef2 >> int_extend), (BYTE)(coef1 >> int_extend))
		, coef2 += coef2_adv;

	pos += pos_adv;
	j = (int)pos - (int)(4 * pos_adv);
	coef3 = coef1, coef3_adv = (int)((val - coef3) / (j - 1));
	while (j--) *buffer++ = RGB((BYTE)(val >> int_extend), (BYTE)(coef1 >> int_extend), (BYTE)(coef3 >> int_extend))
		, coef3 += coef3_adv;

	pos += (pos_adv + 0.1);	// + 0.1 because of floating-point math's rounding errors
	j = (int)pos - (int)(5 * pos_adv);
	coef2 = val, coef2_adv = (int)((val * (1.0 - sat) - coef2) / (j - 1));
	while (j--) *buffer++ = RGB((BYTE)(coef2 >> int_extend), (BYTE)(coef1 >> int_extend), (BYTE)(val >> int_extend))
		, coef2 += coef2_adv;
}

void CWTLColorPicker::set(unsigned long **buffer, unsigned long value, size_t count)
{
	unsigned long	*p = *buffer;

	while (count--)
		*p++ = value;
	*buffer = p;
}

// public

void CWTLColorPicker::SetColorRGB(int r, int g, int b) 
{
	SetRGB(r, g, b);
	UpdateValues(CurrCol);
	SendMessage(WM_PAINT);
}

COLORREF CWTLColorPicker::GetColorRGB()
{
	return RGB(CurrCol.r, CurrCol.g, CurrCol.b);
}

// 键盘钩子

LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || nCode == HC_NOREMOVE)
		return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);

	if (lParam & 0x40000000)	// 消息重复就交给下一个hook链
	{
		return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
	}

	// 通知主窗口。wParam参数为虚拟键码, lParam参数包含了此键的信息
	::PostMessage(g_hWndCaller, HM_KEY, wParam, lParam);

	return ::CallNextHookEx(g_hHook, nCode, wParam, lParam);
}

// 安装、卸载钩子的函数
BOOL WINAPI CWTLColorPicker::SetKeyHook(BOOL bInstall)
{
	BOOL bOk;
	g_hWndCaller = m_hWnd;

	if (bInstall)
	{
		g_hHook = ::SetWindowsHookEx(WH_KEYBOARD, KeyHookProc,
			GetModuleHandle(NULL), GetCurrentThreadId());
		bOk = (g_hHook != NULL);
	}
	else
	{
		bOk = ::UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
	}

	return bOk;
}

//LRESULT CWTLColorPicker::OnHookKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
//{
//	if ((char)wParam == VK_TAB) {
//		HWND hWndEdit = GetFocus();
//		int nID = ::GetDlgCtrlID(hWndEdit);
//		if (hWndEdit == FALSE) return FALSE;
//		GotoDlgCtrl(GetDlgItem(IDC_RED));
//
//		if (nID >= IDC_RED && nID < IDC_VALUE)
//			GotoDlgCtrl(GetDlgItem(nID + 1));
//
//
//		if (nID + 1 > IDC_VALUE)
//			GotoDlgCtrl(GetDlgItem(IDC_RED));
//	}
//
//	return FALSE;
//}


