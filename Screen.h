#pragma once

#include "Views.h"

namespace Graphs
{
	//корневой view в окне, находится напрямую в Window
	struct Screen : public Composite{
		View* view_mouse_capture = 0;
		View* view_key_capture = 0;

		Screen() : Composite(0) {

		}

		//правильно перенаправляет все события своим потомкам
		int mouse_move_event(int x, int y, int virtual_key) override;
		int mouse_event(mouse_buttons button, click_event type, int x, int y, int virtual_key) override;
		int mouse_wheel_event(int x, int y, short delta, int virtual_key) override;
		int key_event(struct_key_event key, int virtual_key) override;
		int char_event(struct_key_event key, wchar_t c) override;
		int paint_background_event(BufferHDC &hdc) override;

		//обеспечивает механику захвата мыши
		void capture(View* view) override;
		void re_capture(View* view) override;
		void key_capture(View* view) override;
		void key_re_capture(View* view) override;
	};
}