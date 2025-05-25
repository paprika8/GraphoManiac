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
	struct Console_Text : public View{
		StringFormat str_format; 
		std::wstring text;
		int text_size = 16;

		SolidBrush text_brush = SolidBrush(Color(0, 0, 0));

		Console_Text(View* aparent) : View(aparent) {
			background.SetColor(Color(50, 20, 150));
			str_format.SetFormatFlags(StringFormatFlagsLineLimit);
		}

		int paint_event(BufferHDC &hdc) override {
			Gdiplus::RectF rc(abs_position.x, abs_position.y, abs_size.width, abs_size.height);
			hdc.graphic->DrawString(text.data(), -1, get_font(text_size, 1), rc, &str_format, &text_brush);
			return 0;
		}

		Size_ get_content_size(Size_ size = Size_()) override;
	};

	struct Edit_Text : public View{
		StringFormat str_format; 
		std::wstring text = L"";
		int text_size = 16;

		SolidBrush text_brush = SolidBrush(Color(0, 0, 0));

		Edit_Text(View* aparent) : View(aparent) {
			background.SetColor(Color(50, 20, 150));
		}

		int paint_event(BufferHDC &hdc) override {
			Gdiplus::RectF rc(abs_position.x, abs_position.y, abs_size.width, abs_size.height);
			hdc.graphic->DrawString(text.data(), -1, get_font(text_size), rc, &str_format, &text_brush);
			return 0;
		}

		int mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) override{
			parent->key_capture(this);
			return 0;
		}

		int key_event(struct_key_event key, int virtual_key) override;

		int char_event(struct_key_event key, wchar_t c) override;
	};
}