#include "button.h"
namespace Graphs
{
	PButton::PButton(HWND ahwnd, View* aview) : ProcessView(ahwnd, aview) {
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

	Size PButton::GetContentSize(Size size) {
		if (margin.type & MarginType::CONTENT) {
			//Size abs = getAbsoluteSize();
			//padding.toAbsolut(abs).reSize(abs);
			auto s = get_size(((Button*)view)->text.data, ((Button*)view)->stringFormat, -1,((Button*)view)->text_size);
			(-padding.toAbsolut(s)).reSize(s);
			return s;
		}
		return size;
	}

	void PButton::construction() {
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
	};

	void PButton::Move(Point apoint, Size asize) {
		ProcessView::Move(apoint, asize);
		Button* view = this->view;
		if(margin.type & MarginType::CONTENT || !view->text_resizer)
			return;
		padding.toAbsolut(asize).reRect(apoint, asize);
		view->set_font_size(generate_text_size(hwnd, view->text, view->stringFormat, view->text_size,asize));
	}

	void Button::paint(HWND hwnd) {
		PAINTSTRUCT pstruct;
		HDC hdc = BeginPaint(hwnd, &pstruct);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(text_color.GetR(), text_color.GetG(), text_color.GetB()));
		Gdiplus::Graphics g(hdc);
		auto rcDirty = pstruct.rcPaint;
		Gdiplus::SolidBrush* brush = new Gdiplus::SolidBrush(background);
		g.FillRectangle(brush, rcDirty.left, rcDirty.top, (int)(rcDirty.right - rcDirty.left), (int)(rcDirty.bottom - rcDirty.top));
		delete brush;
		RECT r = pstruct.rcPaint;
		PV->padding.toAbsolut(PV->getAbsoluteSize()).reRect(r);
		RECT rc = r;

		Size get_text_size = get_size(&g, text, stringFormat, r.right - r.left + 1,text_size);
		rc = RECT(rc.left, rc.top, rc.left + get_text_size.width, rc.top + get_text_size.height);

		int textHeight = rc.bottom - rc.top;

		// Корректируем положение
		int offsetY = (r.bottom - r.top - textHeight) / 2;
		rc = r;
		rc.top += offsetY;

		// Рисуем текст
		Gdiplus::RectF rect_text(rc.left, rc.top, rc.right - rc.left + 1, rc.bottom - rc.top + 1);
		brush = new Gdiplus::SolidBrush(text_color);
		g.DrawString(text.data, -1, get_font(text_size), rect_text, stringFormat, brush);
		delete brush;
		EndPaint(hwnd, &pstruct);
	};
}