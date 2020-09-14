

#pragma once
#include "BitmapBtn.h"
#include "EditEx.h"
#include <windows.h>
#include <string>
#include <sstream>

#define HM_KEY WM_USER + 101

// dialog color
#define COLOR_DLG_BK1		RGB(47, 49, 54)
#define COLOR_DLG_BK2		RGB(32, 34, 37)
#define COLOR_DLG_TEXT		RGB(174, 175, 178)
#define COLOR_SPACE_BORDER	RGB(174, 175, 178)
#define COLOR_EDIT_BORDER	RGB(54, 57, 62)
#define COLOR_EDIT_TEXT		RGB(219, 219, 219)


#define _IS_IN(min, max, x)  (((x)>(min)) && ((x)<(max)))
#define CLAMP(x, m, M){\
if ((x)<(m)) (x) = (m); \
if ((x)>(M)) (x) = (M); \
}

#define MAX(a, b, c) ((a)>(b)? ((a)>(c)?(a):(c)) : ((b)>(c)?(b):(c)))
#define MIN(a, b, c) ((a)<(b)? ((a)<(c)?(a):(c)) : ((b)<(c)?(b):(c)))


struct SColour
{
	unsigned short r, g, b;		// R G B
	unsigned short h, s, v;		// H S V

	// 颜色空间转换
	void UpdateRGB();			// Updates RGB from HSV
	void UpdateHSV();			// Updates HSV from RGB
	double HSL2RGB(double v1, double v2, double vH);	// HSL helper function
	void RGB_F_HTML(CString szValue);					// Updates RGB from HTML
};



class CWTLColorPicker : public CDialogImpl<CWTLColorPicker>
{
public:
	enum { IDD = IDD_COLORDLG };

	CWTLColorPicker();
	const char* upcolor = "upcolor";
	const char* noupcolor = "noupcolor";

	BEGIN_MSG_MAP(CWTLColorPicker)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_NCHITTEST, OnNcHitTest)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkgnd)
		MESSAGE_HANDLER(WM_CTLCOLORSTATIC, OnCtlColorStatic)
		MESSAGE_HANDLER(WM_HOTKEY, OnHotKey)
		MESSAGE_HANDLER(WM_UPDATECOLOR, OnUpdateColor) // 更新颜色
		MSG_WM_MOUSEMOVE(OnMouseMove)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOVE(OnMove)
		MSG_WM_SETCURSOR(OnSetCursor)
		MSG_WM_CTLCOLOREDIT(OnCtlColorEdit)
		COMMAND_CODE_HANDLER_EX(EN_CHANGE, OnEditControlChange)
		COMMAND_HANDLER(IDC_CLOSE, BN_CLICKED, OnCloseCmd)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		//MESSAGE_HANDLER(HM_KEY, OnHookKey)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnHotKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);
	LRESULT OnNcHitTest(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnEraseBkgnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCtlColorStatic(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnUpdateColor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	HBRUSH OnCtlColorEdit(CDCHandle dc, CEdit edit);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnMove(CPoint ptPos);
	void OnEditControlChange(UINT uNotifyCode, int nID, CWindow wndCtl);
	BOOL OnSetCursor(CWindow wnd, UINT nHitTest, UINT message);
	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	//LRESULT OnHookKey(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/);


	BOOL WINAPI SetKeyHook(BOOL bInstall);
public:
	int m_ncHeight = 28;  // 标题栏高度

	// 控件自绘
	CBitmapBtn m_close, m_cancel, m_ok, m_min;
	CEditEx m_eRed, m_eBlue, m_eGreen;
	CEditEx m_eHtml;
	CEditEx m_eHue2, m_eSaturation2, m_eValue;
	CStatic m_prompt;

public:
	void SetRGB(unsigned short r, unsigned short g, unsigned short b);
	void SetHSV(unsigned short h, unsigned short s, unsigned short v);
	void SetRGB_F_HSL(unsigned short h, unsigned short s, unsigned short l);
	void SetRGB_F_HTML();

	// 获取结构体数值
	SColour GetCurrentColour();

	void SetSel2(int nID);  // 处理文本框光标位置
	void ColorFromPos();
	void UpdateValues(struct SColour col);

	BOOL m_bColor; // mouse in color1
	BOOL m_bHue;   // mouse in color2
	CStatic m_dper;
	UINT m_HotKeyId;

	const int	int_extend = 20;
	void HSV_SAT(DWORD *buffer, int samples, double hue, double val_fp);
	void HSV_HUE(DWORD *buffer, int samples, double sat, double val_fp);
	void set(unsigned long **buffer, unsigned long value, size_t count);

private:
	SColour CurrCol;

public:

	//填充数据

	BITMAPINFO bmp_info;
	HBITMAP bmp_handle;
	DWORD *bmp_data;

	// Color1
	void DoCalculateColors(COLORREF &lefttop,
		COLORREF &righttop, COLORREF &leftbottom, COLORREF &rightbottom);
	void DoDrawHSV_Hue();


	// Color2
	BITMAPINFO bmp_info2;
	HBITMAP bmp_handle2;
	DWORD *bmp_data2;
	DWORD *row_buffer;
	int m_nBuffSize;
	void OnDrawSilder();
	void DrawHSV_H();


public:
	HCURSOR m_arrow;


public:
	void SetColorRGB(int r, int g, int b);
	COLORREF GetColorRGB();
	CString m_szButtonChange;
};
