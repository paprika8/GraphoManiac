#pragma once

#include "Views.h"

namespace Graphs
{
	struct Button : public View{

		std::function<void(Button*)> click = [](Button*)->void{};

		StringFormat str_format; 
		Color normal_colour = Color(200,200,0);
		Color down_colour = Color(100,100,0);
		std::wstring text = L"";
		SolidBrush text_brush = SolidBrush(Color(0,0,0));
		int text_size = 12;
		int realy_text_size = 12;
		bool is_down = 0;

		Button(View* aparent) : View(aparent) {
			background.SetColor(normal_colour);
		}

		int paint_background_event(BufferHDC &hdc) override {
			Point_ p = abs_position;
			Size_ s = abs_size;
			hdc.graphic->FillRectangle(&border, abs_position.x, abs_position.y, abs_size.width, abs_size.height);
			abs_padding.reRect(p, s);
			hdc.graphic->FillRectangle(&background, p.x, p.y, s.width, s.height);
			return 0;
		};
		int paint_event(BufferHDC &hdc) override {
			hdc.graphic->DrawString(text.c_str(), -1, get_font(realy_text_size), PointF(abs_position.x + abs_padding.left, abs_position.y + abs_padding.top), &text_brush);
			return 0;
		};
		int mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key);

		int Button::resize_event() override;

	};
}