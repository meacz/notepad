#pragma once
#include "resource.h"

#define COLOR_CLOSE_BTN_SELECTED  RGB(235, 59, 58)
#define COLOR_CLOSE_BTN_HOVER     RGB(214, 97, 96)
#define COLOR_CLOSE_BTN_NORMAL    RGB(47, 49, 54)
#define COLOR_ORDER_BTN_NORMAL    RGB(47, 49, 54)
#define COLOR_ORDER_BTN_HOVER     RGB(83, 81, 107)
#define COLOR_ORDER_BTN_SELECTED  RGB(79, 83, 91)
#define COLOR_BTN_TEXT			  RGB(214, 219, 229)


class CBitmapBtn : public CWindowImpl<CBitmapBtn, CButton>
{
public:
	BEGIN_MSG_MAP_EX(CBitmapBtn)
		MSG_OCM_DRAWITEM(OnReflectedDrawItem)
		MSG_WM_MOUSEMOVE(OnMouseMove)
		DEFAULT_REFLECTION_HANDLER()
		END_MSG_MAP()

public:
	CRect m_rect;
	COLORREF m_color;
	BOOL m_bHover = FALSE;
	CImage m_iclose, m_imin;

public:

	BOOL SubclassWindow(HWND hWnd)
	{
		ATLASSERT(m_hWnd == NULL);
		ATLASSERT(::IsWindow(hWnd));
		BOOL bRet = CWindowImpl<CBitmapBtn, CButton>::SubclassWindow(hWnd);

		LoadImageFromResource(&m_iclose, IDB_PICKER_CLOSE, TEXT("PNG"));  // close png

		if (bRet == FALSE)
			return bRet;

		return bRet;
	}

	void OnReflectedDrawItem(UINT nIDCtl, LPDRAWITEMSTRUCT lpDIS)
	{
		CDC dc;
		dc.Attach(lpDIS->hDC);
		CRect rt, rect(lpDIS->rcItem);
		m_rect = rect;
		m_color = COLOR_ORDER_BTN_NORMAL;

		if (m_bHover)
		{
			m_color = COLOR_ORDER_BTN_HOVER;
			if (lpDIS->CtlID == IDC_CLOSE)
				m_color = COLOR_CLOSE_BTN_HOVER;

		}

		if (lpDIS->itemState & ODS_SELECTED)
		{
			if (m_bHover)
			{
				m_bHover = FALSE;
				Invalidate(TRUE);
			}
			m_color = COLOR_ORDER_BTN_SELECTED;
			if (lpDIS->CtlID == IDC_CLOSE) {
				m_color = COLOR_CLOSE_BTN_SELECTED;
				::SendMessage(GetParent(), WM_CLOSE, 0, 0);
			}
		}

		CBrush br = CreateSolidBrush(m_color);
		CPen pen = CreatePen(PS_SOLID, 1, RGB(102, 102, 102));
		CPen pen2 = CreatePen(0, 0, 0);
		dc.SelectBrush(br);
		dc.SelectPen(pen);
		dc.Rectangle(rect);


		if (lpDIS->CtlID == IDC_CLOSE){
			dc.SelectPen(pen2);
			dc.FillSolidRect(rect, m_color);
			m_iclose.TransparentBlt(dc, CRect(5, 4, 10 + 5, 10 + 4), RGB(255, 255, 255));
		}
		
		CString szText;
		GetWindowText(szText);
		dc.SetBkMode(TRANSPARENT);
		dc.SetTextColor(COLOR_BTN_TEXT);
		dc.SelectFont(GetFont());

		dc.DrawText(szText, -1, rect, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
		dc.Detach();
	}

	void OnMouseMove(UINT nFlags, CPoint point)
	{
		if (m_rect.PtInRect(point))
		{
			if (!m_bHover)
			{
				this->SetCapture();
				m_bHover = TRUE;
				Invalidate(TRUE);
				//ATLTRACE("true\n");
			}
		}
		else
		{
			ReleaseCapture();
			if (m_bHover)
			{
				m_bHover = FALSE;
				Invalidate(TRUE);
				//ATLTRACE("false\n");
			}
		}
	}

	// 资源加载图像
	BOOL LoadImageFromResource(CImage* pImage, UINT nResID, LPCWSTR lpTyp)
	{
		if (pImage == NULL)
			return FALSE;

		pImage->Destroy();

		// 查找资源
		HRSRC hRsrc = ::FindResource(GetModuleHandle(NULL), MAKEINTRESOURCE(nResID), lpTyp);
		if (hRsrc == NULL)
			return FALSE;

		// 加载资源
		HGLOBAL hImgData = ::LoadResource(GetModuleHandle(NULL), hRsrc);
		if (hImgData == NULL)
		{
			::FreeResource(hImgData);
			return FALSE;
		}

		// 锁定内存中的指定资源
		LPVOID lpVoid = ::LockResource(hImgData);

		LPSTREAM pStream = NULL;
		DWORD dwSize = ::SizeofResource(GetModuleHandle(NULL), hRsrc);
		HGLOBAL hNew = ::GlobalAlloc(GHND, dwSize);
		LPBYTE lpByte = (LPBYTE)::GlobalLock(hNew);
		::memcpy(lpByte, lpVoid, dwSize);

		// 解除内存中的指定资源
		::GlobalUnlock(hNew);

		// 从指定内存创建流对象
		HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
		if (ht != S_OK)
		{
			GlobalFree(hNew);
		}
		else
		{
			// 加载图片
			pImage->Load(pStream);
			GlobalFree(hNew);
		}

		// 释放资源
		::FreeResource(hImgData);
		return TRUE;
	}
};
