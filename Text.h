#pragma once

#include "Views.h"



namespace Graphs{
	struct Line_Text : public View{
		StringFormat str_format; 
		std::wstring text;
		int text_size = 16;

		SolidBrush text_brush = SolidBrush(Color(0, 0, 0));

		Line_Text(View* aparent) : View(aparent) {
			background.SetColor(Color(50, 20, 150));
		}

		int paint_event(BufferHDC &hdc) override {
			Gdiplus::RectF rc(abs_position.x, abs_position.y, abs_size.width, abs_size.height);
			hdc.graphic->DrawString(text.data(), -1, get_font(text_size), rc, &str_format, &text_brush);
			return 0;
		}
	};
}