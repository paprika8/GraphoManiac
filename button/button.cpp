#include "button.h"
namespace BoolApp{
	PButton::PButton(HWND ahwnd, View *aview) : ProcessView(ahwnd, aview)
	{
		/*resize = right_form; [](ProcessView* pv, Point p, Size s) -> void{
			HRGN rgn = CreateRectRgn(0, 0, 0 + s.width, 0 + s.height);
			HRGN rgn2 = CreateRectRgn(0, 0, 0 + s.width - s.height, 0 + s.height);
			HRGN round_rgn = 	CreateEllipticRgn(s.width - s.height * 2, - s.height * 0.5, s.width, s.height * 1.5);
			CombineRgn(rgn, rgn, round_rgn, RGN_AND);
			CombineRgn(rgn, rgn, rgn2, RGN_OR);
			DeleteObject(round_rgn);
			pv->padding.right = s.width / 8;
			//HRGN rgn = CreateRectRgn(0, 0, 0 + s.width, 0 + s.height);
			SetWindowRgn(pv->hwnd, rgn, 1);
		};*/
	}
	void PButton::construction() 
	{
		hwnd = CreateWindowEx(
			WS_EX_TRANSPARENT,
			view->getSzWindowClass().c_str(),
			L"",
			WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
			0, 0, 0, 0,
			hwnd,
			0,
			instance,
			0);
		SetWindowLongPtr(hwnd, 0, (LONG_PTR)this);
		int res = SendMessage(hwnd, WM_SETFONT, ((Button*)view)->font, true);
	};
	void Button::paint(HWND hwnd)
	{
		PAINTSTRUCT pstruct;
		HDC hdc = BeginPaint(hwnd, &pstruct);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(text_color.GetR(), text_color.GetG(), text_color.GetB()));
		SelectObject(hdc, font);
		Gdiplus::Graphics g(hdc);
		auto rcDirty = pstruct.rcPaint;
		Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush ( background );
		g.FillRectangle ( brush , rcDirty.left , rcDirty.top , ( int ) ( rcDirty.right - rcDirty.left ) , ( int ) ( rcDirty.bottom - rcDirty.top ) );
		delete brush;
		RECT r = pstruct.rcPaint;
		PV->padding.toAbsolut(PV->getAbsoluteSize()).reRect(r);
		RECT rc = r;
		DrawText(hdc, text.c_str(), text.size(), &rc, flag_format | DT_CALCRECT | DT_WORDBREAK);
		int textHeight = rc.bottom - rc.top;

		// Корректируем положение
		int offsetY = (r.bottom - r.top - textHeight) / 2;
		rc = r;
		rc.top += offsetY;

		// Рисуем текст
		DrawText(hdc, text.c_str(), text.size(), &rc, flag_format & ~DT_CALCRECT | DT_WORDBREAK);
		EndPaint(hwnd, &pstruct);
	};
}